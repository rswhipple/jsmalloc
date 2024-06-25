## JSMalloc: Two-Tiered Dynamic Allocator

### Motivation

JSMalloc is a two-tiered dynamic memory allocation system:

- **Efficient Memory Utilization:** Utilizing different page sizes and chunk categorizations allows for optimal memory utilization across various object sizes.
- **Flexible Resource Management:** JSMalloc can efficiently manage memory resources by dynamically adjusting cache sizes and employing different allocation strategies based on object size. STATUS: dynamic adjustment of cache sizes will be implemented in a future iteration.
- **Enhanced Scalability:** By segregating memory management into backend pageheap and frontend cache, JSMalloc will better handle concurrent memory access in multi-threaded applications. STATUS: Handling of multi-threaded applications will be implemented in future iterations.

### Overview

JSMalloc comprises two main components:

1. **Backend Pageheap:** Manages memory allocation at the page level and interacts with the operating system for memory allocation and deallocation. STATUS: structure and framework there, functionality incomplete.
2. **Frontend Cache:** Provides fast allocation and deallocation operations for small and medium-sized objects, using a variety of search algorithms and caching structures.

### JSMalloc Backend Pageheap

- **Span Organization:** Memory is organized into spans, representing isolated heaps of memory allocated from the operating system using the mmap() system call.
- **Page Management:** Spans are further divided into pages, with each page representing a contiguous block of memory of a fixed size.
- **Pagemap Structure:** The pagemap structure facilitates efficient mapping of memory addresses to pages, enabling fast retrieval and management of memory resources.

### JSMalloc Frontend Cache

- **Tiered Allocation:** Objects are categorized into different size classes based on their size, with separate handling mechanisms for tiny, small, and large objects.
- **Fast Cache:** Provides fast allocation and deallocation operations for tiny allocation sizes by creating predefined memory blocks where all chunks are of the same size and accessed via a simple array of linked lists. 
- **Unsorted Cache:** This cache has a singly linked list of recently freed chunks. Chunks are dynamically sorted into the Sorted Cache during the search algo for the Unsorted Cache.  
- **Sorted Cache:** A hash table with bins for various chunk sizes, each hash points to the head of a doubly linked list of chunks of the same size or similar size once we enter the larger object sizes. STATUS: A future imporvement will be implement the hash table so that the memory for the bins is allocated dynamically. Currently we have a fixed section allocating enough space for all possible bins.

### Memory Chunk Categorization

- **Tiny Allocation:** For objects ranging from min_chunk_size to 64 bytes, each page contains objects of a single size, simplifying allocation and deallocation operations.
- **Small Allocation:** Objects ranging from 72 to 512 bytes are managed using a double-linked list structure, with efficient handling of coalescing and free chunk sorting.
- **Large Allocation:** Objects larger than 512 bytes and less than 4KB are managed similarly to small allocations but with larger chunk sizes and optimized handling for larger objects.

### Huge Size Allocation

- Objects larger than 4KB are allocated directly from the operating system using the mmap() system call, bypassing the frontend cache and backend pageheap.
- This approach ensures efficient allocation and deallocation of very large memory blocks without incurring the overhead of cache management.

