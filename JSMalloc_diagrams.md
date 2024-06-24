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

## Frontend Cache (Detailed)

### Summary

- **Fast Cache Array**: Quick access to small allocations using a simple array and singly linked lists (`t_tiny_chunk`).
- **Unsorted Cache**: Temporary storage for recently freed chunks, organized as a singly linked list (`t_chunk`).
- **Sorted Cache**: Efficient management of small and large allocations using a hash table and doubly linked lists (`t_chunk`).

### Fast Cache

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

```plaintext
+---------------------------+
|      Unsorted Cache       |
|  +----------+      +----------+
|  | t_chunk  | -->  | t_chunk  | --> NULL
|  +----------+      +----------+
+---------------------------+
```

### Sorted Cache

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

## Pageheap Diagram

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
| [t_chunk]             |
|                       |
|                       |
+-----------------------+
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