
#ifndef PAGEMAP_H
#define PAGEMAP_H

#include "types.h"
#include <stdint.h>

#define PAGEMAP_SHIFT(start)((uint8_t *)start + sizeof(t_pagemap))
#define CACHE_SHIFT(start)((uint8_t *)start + sizeof(t_cache))  
#define HASH_SHIFT(start)((char *)start + sizeof(t_hash))
#define SPAN_SHIFT(start)((char *)start + sizeof(t_span))
#define PAGE_SHIFT(start)((char *)start + sizeof(t_page))
#define FASTPAGE_SHIFT(start)((char *)start + sizeof(t_fpage))
#define CHUNK_SHIFT(start)((char *)start + sizeof(t_chunk))
#define TINY_CHUNK_SHIFT(start)((char *)start + sizeof(t_tiny_chunk))
#define MEMORY_SHIFT(start, data_size)((char *)start + data_size)

#define PAGE_SIZE getpagesize()
#define BASE_HEAP_SIZE (48 * PAGE_SIZE)

t_span* create_base_span(t_cache* cache);
t_span* add_span(t_pagemap* pagemap, void* start, size_t size);
void create_pagemap(t_pagemap** pagemap);
void destroy_pagemap(t_pagemap* pagemap);

#endif