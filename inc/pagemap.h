
#ifndef PAGEMAP_H
#define PAGEMAP_H

#include "my_malloc.h"

void create_page(t_pagemap **pagemap);
t_span *create_base_span(void *start);
t_span *add_span(t_pagemap *pagemap, void *start, size_t size);
void create_pages(t_pagemap *pagemap, t_span *span);
void create_base_page(t_span *span);
void destroy_active_page(t_page *page);
void destroy_page(t_page *page);
void destroy_pageheap(t_pagemap *pagemap);

#endif