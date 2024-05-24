## JSMalloc: Two-Tiered Dynamic Allocator

### Motivation

JSMalloc introduces a two-tiered dynamic memory allocation system to address various challenges in modern computing environments:

- **Enhanced Scalability:** By segregating memory management into backend pageheap and frontend cache, JSMalloc can better handle concurrent memory access in multi-threaded applications.
- **Efficient Memory Utilization:** Utilizing different page sizes and chunk categorizations allows for optimal memory utilization across various object sizes.
- **Flexible Resource Management:** JSMalloc can efficiently manage memory resources by dynamically adjusting cache sizes and employing different allocation strategies based on object size.

### Overview

JSMalloc comprises two main components:

1. **Backend Pageheap:** Manages memory allocation at the page level and interacts with the operating system for memory allocation and deallocation.
2. **Frontend Cache:** Provides fast allocation and deallocation operations for small and medium-sized objects, reducing contention and improving performance.

### JSMalloc Backend Pageheap

- **Span Organization:** Memory is organized into spans, representing isolated heaps of memory allocated from the operating system using the mmap() system call.
- **Page Management:** Spans are further divided into pages, with each page representing a contiguous block of memory of a fixed size determined by the operating system.
- **Pagemap Structure:** The pagemap structure facilitates efficient mapping of memory addresses to spans, enabling fast retrieval and management of memory resources.

### JSMalloc Frontend Cache

- **Tiered Allocation:** Objects are categorized into different size classes based on their size, with separate handling mechanisms for tiny, small, and large objects.
- **Per-Thread or Per-CPU Caching:** The frontend cache can operate in per-thread mode, where each thread has its local cache, or per-CPU mode, where each logical CPU has its cache, improving efficiency and scalability in multi-threaded environments.
- **Dynamic Page Movement:** Pages are moved between the backend pageheap and frontend cache based on usage patterns, ensuring efficient utilization of memory resources.

### Memory Chunk Categorization

- **Tiny Allocation:** For objects ranging from min_chunk_size to 64 bytes, each page contains objects of a single size, simplifying allocation and deallocation operations.
- **Small Allocation:** Objects ranging from 72 to 512 bytes are managed using a double-linked list structure, with efficient handling of coalescing and free chunk sorting.
- **Large Allocation:** Objects larger than 512 bytes and less than 4KB are managed similarly to small allocations but with larger chunk sizes and optimized handling for larger objects.

### Huge Size Allocation

- Objects larger than 4KB are allocated directly from the operating system using the mmap() system call, bypassing the frontend cache and backend pageheap.
- This approach ensures efficient allocation and deallocation of very large memory blocks without incurring the overhead of cache management.

### Conclusion

JSMalloc's two-tiered dynamic allocator provides a robust and efficient solution for memory management in modern computing environments. By combining backend pageheap and frontend cache mechanisms, JSMalloc achieves optimal memory utilization, scalability, and performance for diverse application workloads.
