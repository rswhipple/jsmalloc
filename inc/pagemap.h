
#ifndef PAGEMAP_H
#define PAGEMAP_H

#include "types.h"
#include <stdint.h>

#define PAGEMAP_SHIFT(start)((uint8_t *)start + sizeof(t_pagemap))  
#define SPAN_SHIFT(start)((char *)start + sizeof(t_span))
#define PAGE_SHIFT(start)((char *)start + sizeof(t_page))
#define FASTPAGE_SHIFT(start)((char *)start + sizeof(t_fpage))
#define CHUNK_SHIFT(start)((char *)start + sizeof(t_chunk))
#define TINY_CHUNK_SHIFT(start)((char *)start + sizeof(t_tiny_chunk))
#define HASH_SHIFT(start)((char *)start + sizeof(t_hash))
#define MEMORY_SHIFT(start, data_size)((char *)start + data_size)

#define PAGE_SIZE getpagesize()
#define BASE_HEAP_SIZE (48 * PAGE_SIZE)
#define FAST_PAGE_ALLOCATION_SIZE 8
#define FAST_PAGE_MAX_CHUNK_SIZE 64

t_span* create_base_span(t_pagemap* pagemap);
t_span* add_span(t_pagemap* pagemap, void* start, size_t size);
void create_fpages(t_span* span);
t_fpage* create_base_fpage(t_span* span);
t_fpage* create_fpage(t_fpage* prev_page);
void create_pages(t_pagemap* pagemap, t_span* span);
t_page* create_base_page(t_pagemap* pagemap, t_span* span);
t_page* create_page(t_page* prev_page, t_span* span, int pagetype);
void destroy_active_page(t_page* page);
void destroy_page(t_page* page);
void create_pagemap(t_pagemap** pagemap);
void destroy_pagemap(t_pagemap* pagemap);

#endif