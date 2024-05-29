

#ifndef PAGES_H
#define PAGES_H

#include "types.h"

void create_fpages(t_span* span, void* start);
t_fpage* create_base_fpage(void* start);
t_fpage* create_fpage(t_fpage* prev_page, size_t chunk_size);
void create_pages(t_pagemap* pagemap, t_span* span);
t_page* create_base_page(t_pagemap* pagemap, t_span* span);
t_page* create_page(t_page* prev_page, t_span* span, int pagetype);
void destroy_active_page(t_page* page);
void destroy_page(t_page* page);

#endif