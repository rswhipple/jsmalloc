
#ifndef PAGEMAP_H
#define PAGEMAP_H

#include "types.h"

// Macros that return the address of datatypes within the heap
#define PAGEMAP_SHIFT(start)((u_int8_t *)start + sizeof(t_pagemap))
#define SPAN_SHIFT(start)((u_int8_t *)start + sizeof(t_span))
#define PAGE_SHIFT(start)((u_int8_t *)start + sizeof(t_page))
#define CHUNK_SHIFT(start)((u_int8_t *)start + sizeof(t_chunk))
#define HASH_SHIFT(start)((u_int8_t *)start + sizeof(t_hash))
#define MEMORY_SHIFT(start, data_size)((u_int8_t *)start + data_size)

#define PAGE_SIZE getpagesize()
#define BASE_HEAP_SIZE (48 * PAGE_SIZE)
#define FAST_PAGE_ALLOCATION_SIZE (8 * PAGE_SIZE)
#define FAST_PAGE_MAX_CHUNK_SIZE 64
#define SMALL_HEAP_ALLOCATION_SIZE (20 * PAGE_SIZE)
#define SMALL_PAGE_MAX_CHUNK_SIZE 512
#define LARGE_HEAP_ALLOCATION_SIZE (20 * PAGE_SIZE)
#define LARGE_PAGE_MAX_CHUNK_SIZE (LARGE_HEAP_ALLOCATION_SIZE / 12)

t_span* create_base_span(t_pagemap* pagemap);
t_span* add_span(t_pagemap* pagemap, void* start, size_t size);
void create_pages(t_pagemap* pagemap, t_span* span);
t_page* create_base_page(void* start);
t_page* create_page(t_page* prev_page, t_pagemap* pagemap);
void destroy_active_page(t_page* page);
void destroy_page(t_page* page);
void create_pageheap(t_pagemap** pagemap);
void destroy_pageheap(t_pagemap* pagemap);

#endif