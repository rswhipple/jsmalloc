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
| first page            |
|                       |
|    chunks             |
+-----------------------+
+-----------------------+


## Chunk Header Metadata

Each chunk has a header that includes metadata necessary for managing memory. For free chunks, this header includes:

- **Size and Status**: The size of the chunk, including a flag indicating whether it is free or allocated.
- **Previous Size**: The size of the previous chunk, allowing backward traversal.

### Tracking Adjacent Chunks

JSMalloc uses the following methods to track adjacent chunks:

- **Boundary Tags**: At the end of each chunk, a boundary tag replicates the size of the chunk.
- **In-Place Headers**: When a chunk is freed, the allocator uses the size and previous size fields to check the status of adjacent chunks.

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

## Chunk Structure Definition

In this example, we use a single `chunk` struct that includes all necessary fields, simplifying the code and reducing redundancy.

### Chunk Structure Definition

```c
typedef struct chunk {
    size_t size;          // Size of the chunk (includes the size of the header)
    size_t prev_size;     // Size of the previous chunk
    struct chunk* fd;     // Forward pointer for the free list
    struct chunk* bk;     // Backward pointer for the free list
} chunk;

// Helper macros to access boundary tags
#define CHUNK_SIZE(chunk) ((chunk)->size & ~0x7) // Mask out lower bits used for status
#define NEXT_CHUNK(chunk) ((chunk*)((char*)(chunk) + CHUNK_SIZE(chunk)))
#define PREV_CHUNK(chunk) ((chunk*)((char*)(chunk) - (chunk)->prev_size))

// Alignment to ensure proper boundaries
#define ALIGN_SIZE 8
#define ALIGN_MASK (ALIGN_SIZE - 1)
#define ALIGN(n) (((n) + ALIGN_MASK) & ~ALIGN_MASK)

#define CHUNK_OVERHEAD sizeof(size_t) // Simplified, normally includes additional metadata
#define MIN_CHUNK_SIZE (sizeof(chunk) + CHUNK_OVERHEAD)
```

## Functions to Write Boundary Tag and Free Chunks

Next, we implement the functions to free a chunk, write a boundary tag, and coalesce adjacent free chunks.

### Function to Write Boundary Tag

```c
void write_boundary_tag(chunk* ch) {
    size_t* boundary_tag = (size_t*)((char*)ch + CHUNK_SIZE(ch) - sizeof(size_t));
    *boundary_tag = ch->size;
}
```

### Function to Free a Chunk and Coalesce if Possible

```c
void free_chunk(chunk* ch) {
    chunk* next_chunk = NEXT_CHUNK(ch);

    // Coalesce with next chunk if it's free
    if (!(next_chunk->size & 1)) { // Check if next chunk is free
        ch->size += CHUNK_SIZE(next_chunk);
        next_chunk = NEXT_CHUNK(ch);
    }

    // Write the boundary tag
    write_boundary_tag(ch);

    // Coalesce with previous chunk if it's free
    if (!(ch->prev_size & 1)) { // Check if previous chunk is free
        chunk* prev_chunk = PREV_CHUNK(ch);
        prev_chunk->size += CHUNK_SIZE(ch);
        ch = prev_chunk;

        // Write the boundary tag
        write_boundary_tag(ch);
    }

    // Insert chunk into the free list (simplified for this example)
    ch->fd = free_list;
    if (free_list) {
        free_list->bk = ch;
    }
    free_list = ch;
}

// Simplified global free list for demonstration purposes
chunk* free_list = NULL;
```

### Usage Example

Here's how you might use these functions in a simple program:

```c
int main() {
    // Simulate allocation of a chunk (simplified)
    size_t chunk_size = ALIGN(100); // Requested size
    chunk* ch = (chunk*)malloc(chunk_size + sizeof(chunk));
    ch->size = chunk_size | 1; // Mark as allocated
    ch->prev_size = 0; // No previous chunk in this example

    // Simulate freeing the chunk
    ch->size &= ~1; // Mark as free
    free_chunk(ch);

    // For demonstration purposes, print the size
```

```

```
