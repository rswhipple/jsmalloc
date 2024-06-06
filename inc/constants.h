#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "types.h"

// =================== Constants ===================

extern size_t min_chunk_size;
extern size_t pointer_size;
extern t_pagemap* g_pagemap;

#define FAST_PAGE_ALLOCATION_SIZE 8
#define FAST_PAGE_MAX_CHUNK_SIZE 64

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

// =================== Shifters ===================

#define PAGEMAP_SHIFT(start)((uint8_t *)start + sizeof(t_pagemap))
#define CACHE_SHIFT(start)((uint8_t *)start + sizeof(t_cache))  
#define HASH_SHIFT(start)((char *)start + sizeof(t_cache_table))
#define SPAN_SHIFT(start)((char *)start + sizeof(t_span))
#define PAGE_SHIFT(start)((char *)start + sizeof(t_page))
#define FASTPAGE_SHIFT(start)((char *)start + sizeof(t_fpage))
#define CHUNK_SHIFT(start)((char *)start + sizeof(t_chunk))
#define TINY_CHUNK_SHIFT(start)((char *)start + sizeof(t_tiny_chunk))
#define MEMORY_SHIFT(start, data_size)((char *)start + data_size)

#define PAGE_SIZE getpagesize()
#define BASE_HEAP_SIZE (48 * PAGE_SIZE)
#define NUM_BINS 149

#endif // CONSTANTS_H
