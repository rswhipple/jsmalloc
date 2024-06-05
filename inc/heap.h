
#ifndef HEAP_H
#define HEAP_H

#include "types.h"

// Macros and constants
#define HEAP_SHIFT(start)((char *)start + sizeof(t_heap))
#define BLOCK_SHIFT(start)((char *)start + sizeof(t_block))

#define TINY_HEAP_ALLOCATION_SIZE (4 * getpagesize())
#define TINY_BLOCK_SIZE (TINY_HEAP_ALLOCATION_SIZE / 128)
#define SMALL_BLOCK_SIZE (SMALL_PAGE_ALLOCATION_SIZE / 128)

#define MAX_BLOCKS 20 

void create_heap(t_heap** heap, size_t size);

#endif