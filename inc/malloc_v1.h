#ifndef MALLOC_V1_H  
#define MALLOC_V1_H

#include "types.h"

extern t_heap* global_heap;

#define SMALL_HEAP_ALLOCATION_SIZE 20
#define LARGE_HEAP_ALLOCATION_SIZE 20
#define SMALL_PAGE_MAX_CHUNK_SIZE 512
#define LARGE_PAGE_MAX_CHUNK_SIZE (LARGE_HEAP_ALLOCATION_SIZE * PAGE_SIZE / 12)

void* create_block(t_heap* heap, size_t size);

#define HEAP_SHIFT(start)((char *)start + sizeof(t_heap))
#define BLOCK_SHIFT(start)((char *)start + sizeof(t_block))

#define TINY_HEAP_ALLOCATION_SIZE (4 * getpagesize())
#define TINY_BLOCK_SIZE (TINY_HEAP_ALLOCATION_SIZE / 128)
#define SMALL_BLOCK_SIZE (SMALL_PAGE_ALLOCATION_SIZE / 128)

#define MAX_BLOCKS 20 

void create_heap(t_heap** heap, size_t size);

#endif