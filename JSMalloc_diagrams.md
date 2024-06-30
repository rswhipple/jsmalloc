# JSMalloc

## Overview

The memory allocator operates through multiple layers: the application, the frontend cache, the backend page heap, and the operating system. Each layer has specific responsibilities for managing memory allocation and deallocation.

### High-Level Architecture

```plaintext
+--------------------------------------------+
|                Application                 |
+--------------------------------------------+
                |         ^
                |         |
                v         |
+--------------------------------------------+
|               Frontend Cache               |
|  +--------------------------------------+  |
|  |          Fast Cache Array            |  |
|  |  +-------------------------------+   |  |
|  |  | - Tiny Allocation (min to 64) |   |  |
|  |  | t_tiny_chunk** fast_cache     |   |  |
|  |  | size_t fcache_size            |   |  |
|  |  +-------------------------------+   |  |
|  |          Unsorted Cache              |  |
|  |  +-------------------------------+   |  |
|  |  | t_chunk* unsorted_cache       |   |  |
|  |  +-------------------------------+   |  |
|  |          Sorted Cache                |  |
|  |  +-------------------------------+   |  |
|  |  | t_cache_table* cache_table    |   |  |
|  |  +-------------------------------+   |  |
|  +--------------------------------------+  |
+--------------------------------------------+
                |         ^
                |         |
                v         |
+--------------------------------------------+
|              Backend Pageheap              |
|  +--------------------------------------+  |
|  | Span Organization:                   |  |
|  | - Spans allocated with mmap()        |  |
|  | - Pages within spans                 |  |
|  | - Pagemap structure                  |  |
|  +--------------------------------------+  |
+--------------------------------------------+
                |         ^
                |         |
                v         |
+--------------------------------------------+
|               Operating System             |
|  +--------------------------------------+  |
|  |  Memory Allocation:                  |  |
|  |  - Direct allocation for huge sizes  |  |
|  |    (larger than 4KB) using mmap()    |  |
|  +--------------------------------------+  |
+--------------------------------------------+
```

### Memory Chunk Categorization

- **Tiny Allocation:** For objects ranging from min_chunk_size to 64 bytes, each page contains objects of a single size, simplifying allocation and deallocation operations.
- **Small Allocation:** Objects ranging from 72 to 512 bytes are managed using a double-linked list structure, with efficient handling of coalescing and free chunk sorting.
- **Large Allocation:** Objects larger than 512 bytes and less than 4KB are managed similarly to small allocations but with larger chunk sizes and optimized handling for larger objects.
- **Huge Allocation:** - Objects larger than 4KB are allocated directly from the operating system using the mmap() system call, bypassing the frontend cache and backend pageheap.

## Frontend Cache (Detailed)

### Summary

- **Fast Cache Array**: Quick access to small allocations using a simple array and singly linked lists (`t_tiny_chunk`).
- **Unsorted Cache**: Temporary storage for recently freed chunks, organized as a singly linked list (`t_chunk`).
- **Sorted Cache**: Efficient management of small and large allocations using a hash table and doubly linked lists (`t_chunk`).

### Fast Cache
The array holds 7 or 8 bins, determined by the system settings of the OS.
Each bin holds a linked list of `t_tiny_chunk` data structures all of the same size.
The `t_tiny_chunk` struct holds 2 fields, `size` and `next` – a pointer to the next `t_tiny_chunk`. When the `t_tiny_chunk` is in use the data is written over the `next` pointer.

```plaintext
+---------------------------+
|        Fast Cache         |
|  +-----------+            |
|  | [0]       | --> NULL   |
|  +-----------+            |
|  | [1]       | --> +------------+     +------------+     +------------+
|  +-----------+     |t_tiny_chunk|-->  |t_tiny_chunk|-->  |t_tiny_chunk|--> NULL
|  | [2]       |     +------------+     +------------+     +------------+
|  +-----------+            |
|  | [3]       | --> NULL   |
|  +-----------+            |
|  | [4]       | --> NULL   |
|  +-----------+     +------------+
|  | ...       |     | ...        |
|  +-----------+     +------------+
+---------------------------+
```

### Unsorted Cache
When a `t_chunk` is freed it is added to the Unsorted Cache, which is accessed via `t_pagemap`. 
For a detailed overview of the `t_chunk` struct see the Chunk Header Metadata section. 
Each time JSMalloc is called to allocate a new chunk of memory it first checks the Unsorted Cache, 
dynamically sorting each chunk into the Sorted Cache if it does not fit the size needs. 

```plaintext
+---------------------------+
|      Unsorted Cache       |
|  +----------+      +----------+
|  | t_chunk  | -->  | t_chunk  | --> NULL
|  +----------+      +----------+
+---------------------------+
```

### Sorted Cache
A `t_cache_table` struct, which is a hash table holding 149 bins. 
Each `cache_table_entry` has a `key` and a `value` which points to the head of a doubly linked list of `t_chunks`.
Each cache_table_entry holds only one size (see Future Improvements).

```plaintext
+---------------------------+
|       Sorted Cache        |
|  +-----------+            |
|  | [0]       | --> NULL   |
|  +-----------+            |
|  | [1]       | --> +----------+     +----------+     +----------+
|  +-----------+     | t_chunk  |<--> | t_chunk  |<--> |  t_chunk |<--> NULL
|  | [2]       |     +----------+     +----------+     +----------+
|  +-----------+            |
|  | ...       |            |
|  +-----------+     +----------+     +----------+
|  | [N]       | --> | t_chunk  |<--> | t_chunk  |<--> NULL
|  +-----------+     +----------+     +----------+
+---------------------------+
```

## Chunk Header Metadata

Each chunk has a header that includes metadata necessary for managing memory. This header includes:

- **Size and Status**: The size of the chunk, including a flag indicating whether it is free or allocated.

For free chunks, the header also includes:
- **Forward and Backward Pointers**: These pointers are only in use in the unsorted and sorted caches.


### Tracking Adjacent Chunks

JSMalloc uses the following methods to track adjacent chunks:

- **Boundary Tags**: At the end of each chunk, a boundary tag replicates the size of the chunk.
- **In-Place Headers**: When a chunk is freed, the allocator uses the size and boundary tags to check the status of adjacent chunks.

### Example of Adjacent Chunk Tracking

```plaintext
[Chunk A (allocated)]
+------------------+
| Size             | <- includes a flag indicating in_use
+------------------+
| User Data        |
+------------------+
| Boundary Tag     | <- size of Chunk A
+------------------+

[Chunk B (free)]
+------------------+
| Size             | <- includes a flag indicating free
| FD -> Chunk C    |
| BK -> Bin Head   |
+------------------+
| ...              |
+------------------+
| Boundary Tag     | <- size of Chunk B
+------------------+

[Chunk C (free)]
+------------------+
| Size             |
| FD -> Bin Tail   | <- NULL
| BK -> Chunk B    |
+------------------+
| ...              |
+------------------+
| Boundary Tag     | <- size of Chunk C
+------------------+
```

### Coalescing
When a `t_chunk` is freed the previous and next t_chunk in the pageheap is checked using the boundary tags.
If either of the chunks are free the two chunks are merged into one to reduce fragmentation.

## Low-level Architecture

### Pageheap Diagram


```plaintext
+-----------------------+
|[t_pagemap] = p        |
| p->frontend_cache     |
| p->span_head          |
| p->top_chunk          |
| ...                   |
+-----------------------+          
|[t_cache] = c          |
| c->fast_cache         |
| c->cache_table        |
| c->unsorted_cache     |
| ...                   |
+-----------------------+
| fast_cache            |
| single linked list    |
| [t_tiny_chunk*]       |
| ...                   |
+-----------------------+ 
| [t_cache_table] = ct  |
| ct->entries           |
| ct->capacity          |
| ...                   |
+-----------------------+
| cache_table_entries   |
|                       |
|                       |
+-----------------------+
| [t_span] = s          |
+-----------------------+
| [t_pages] = p         |
| p->next               |
| p->base_chunk         |
| p->mem_size           |
| ...                   |
+-----------------------+
| [t_fpages] = fp       |
| fp->next              |
| fp->memory            |
| fp->chunk_size        |
| ...                   |
+-----------------------+
| [t_chunk]             |
| all t_chunks          |
|                       |
+-----------------------+
| [t_tiny_chunk]        |
| all t_tiny_chunks     |
|                       |
+-----------------------+
```


## Future Improvements

### Sorted Cache Improvements
- **Custom Handling:** Employ different allocation strategies based on object size. Large size t_chunks will not be rounded to predetermined sizes. They will be held under a cache_table key which represents a range. 

### JSMalloc Backend Pageheap Upgrade
- **Flexible Resource Management:** JSMalloc will efficiently manage memory resources by dynamically adjusting cache sizes.
- Create a flag that tells JSMalloc to allocate memory if all space is used or reduce size if little space is being used.
- Dynamically return individual pages or spans to memory.

### Multi-threaded Access
- **Enhanced Scalability:** By segregating memory management into backend pageheap and frontend cache, JSMalloc will better handle concurrent memory access in multi-threaded applications.
