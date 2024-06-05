#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <unistd.h>

#include "types.h"

// =================== Constants ===================

#define UNUSED(x) (void)(x)

extern size_t min_chunk_size;
extern size_t pointer_size;
extern t_heap* global_heap;
extern t_pagemap* g_pagemap;

#define FAST_PAGE_ALLOCATION_SIZE 8
#define FAST_PAGE_MAX_CHUNK_SIZE 64
#define SMALL_HEAP_ALLOCATION_SIZE 20
#define SMALL_PAGE_MAX_CHUNK_SIZE 512
#define LARGE_HEAP_ALLOCATION_SIZE 20
#define LARGE_PAGE_MAX_CHUNK_SIZE (LARGE_HEAP_ALLOCATION_SIZE * PAGE_SIZE / 12)

// Helper macros to set and check t_chunk status
#define SET_IN_USE(chunk) ((chunk)->size |= 0x1)
#define IS_IN_USE(chunk) ((chunk)->size & 0x1)
#define SET_FREE(chunk) ((chunk)->size &= ~0x1)

// Helper macros to set and check t_chunk size
#define SET_CHUNK_SIZE(chunk, sz) ((chunk)->size = ((sz) & SIZE_MASK) | ((chunk)->size & ~SIZE_MASK))
#define SIZE_MASK (~0x7)
#define CHUNK_SIZE(chunk) ((chunk)->size & SIZE_MASK) // Mask out lower 3 bits used for status

// Helper macros to access boundary tags
#define CHUNK_OVERHEAD sizeof(size_t * 2)
#define NEXT_CHUNK(chunk) ((chunk*)((char*)(chunk) + CHUNK_SIZE(chunk)))
#define PREV_CHUNK(chunk, prev_size) ((chunk*)((char*)(chunk) - prev_size))
#define TINY_CHUNK_OVERHEAD sizeof(size_t)

// Helper macros to access boundary tags
#define NEXT_CHUNK(chunk) ((chunk*)((char*)(chunk) + CHUNK_SIZE(chunk)))
#define CHUNK_SIZE(chunk) ((chunk)->size & SIZE_MASK) // Mask out lower 3 bits used for status

// Alignment to ensure proper boundaries
#define ALIGN_SIZE 8
#define ALIGN_MASK (ALIGN_SIZE - 1)
#define ALIGN(n) (((n) + ALIGN_MASK) & ~ALIGN_MASK)

#endif // CONSTANTS_H
