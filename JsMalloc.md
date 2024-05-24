# JSMalloc: Thread-Caching Malloc (or is it? 😁)

## Motivation

JSMalloc is an alternative memory allocator with key features:

- **Fast allocation/deallocation:** Per-thread or per-CPU caching reduces contention and improves scaling in multi-threaded applications.
- **Flexible memory usage:** Frees memory can be reused for different object sizes or returned to the OS.
- **Low overhead:** Allocates "pages" of objects of the same size for space efficiency.
- **Detailed memory usage insights:** Low overhead sampling provides detailed insights into application memory usage.

## Usage

Specify JSMalloc as the `malloc` attribute on binary rules in Bazel.

## Overview

JSMalloc consists of three main components:

1. **Front-end:** Fast allocation and deallocation of memory.
2. **Middle-end:** Refills the front-end cache.
3. **Back-end:** Fetches memory from the OS.

The front-end can operate in per-CPU or legacy per-thread mode. The back-end supports hugepage-aware or legacy pageheap.

## JSMalloc Front-end

- **Per-Thread Mode:** Each thread has a local cache, scaling poorly with high thread counts.
- **Per-CPU Mode:** Each logical CPU has its own cache, improving efficiency and scalability.

## Small and Large Object Allocation

- Small objects are allocated based on size-classes (60-80 classes).
- Large objects exceed `kMaxSize` and are allocated directly from the back-end.

## Deallocation

- Small objects return to the front-end cache.
- Large objects return directly to the pageheap.

### Per-CPU Mode

- Memory is allocated in a large block, divided among CPUs.
- Metadata includes headers per size-class and pointers to available objects.
- Capacity management adapts dynamically based on allocation and deallocation patterns.

### Restartable Sequences

- Ensures uninterrupted execution of per-CPU operations without locks or atomic instructions.

### Legacy Per-Thread Mode

- Each thread has a local cache with lists of free objects per size-class.
- Capacity management adapts dynamically.

### Runtime Sizing of Front-end Caches

- Dynamically adjusts cache sizes based on usage patterns to balance memory efficiency and allocation speed.

## JSMalloc Middle-end

- **Transfer Cache:** Rapidly moves objects between CPUs/threads.
- **Central Free List:** Manages memory in spans and interacts with the back-end.

### Pagemap and Spans

- Memory divided into pages managed by spans.
- Pagemap uses a radix tree to map memory addresses to spans.

### Storing Small Objects in Spans

- Uses unrolled linked lists to manage objects within spans.

## JSMalloc Page Sizes

- Page sizes: 4KiB, 8KiB, 32KiB, 256KiB.
- Smaller pages reduce memory overhead; larger pages reduce management costs.

## JSMalloc Backend

### Legacy Pageheap

- Array of free lists for contiguous page runs.
- Allocates and frees memory by managing page runs.

### Hugepage Aware Allocator

- Manages memory in hugepage-size chunks.
- Three caches: filler cache (partial hugepages), region cache (straddling hugepages), hugepage cache (large allocations).

## Caveats

- **Metadata Overhead:** Grows with heap and virtual address range.
- **Large Memory Reservations:** Requests large chunks from OS, leading to large VSS but smaller RSS.
- **Compatibility:** Not suitable for dynamic loading into running binaries due to potential conflicts with system malloc.
