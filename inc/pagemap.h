
#ifndef PAGEMAP_H
#define PAGEMAP_H

#include "my_malloc.h"

void create_pageheap(t_pagemap **pagemap);
t_span *create_base_span(void *start, size_t size);
t_span *add_span(void *start, size_t size);
void create_base_page(t_span *span);
void destroy_active_page(t_page *page);
void destroy_page(t_page *page);
void destroy_pageheap(t_pagemap *pagemap);

#endif