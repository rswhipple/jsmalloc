#ifndef BACKEND_HEAP_H  
#define BACKEND_HEAP_H

#include "types.h"

t_span* create_base_span(t_cache* cache);
t_span* add_span(t_pagemap* pagemap, void* start, size_t size);
void create_pagemap(t_pagemap** pagemap);
void destroy_pagemap(t_pagemap* pagemap);

void create_fpages(t_pagemap* pagemap);
t_fpage* create_base_fpage(t_pagemap* pagemap);
t_fpage* create_fpage(t_fpage* prev_page, int page_count,
        size_t chunk_size, t_cache* cache);
void create_pages(t_pagemap* pagemap, t_span* span);
t_page* create_base_page(t_pagemap* pagemap, t_span* span);
t_page* create_page(t_page* prev_page, t_span* span, int pagetype);
void destroy_active_page(t_page* page);
void destroy_page(t_page* page);


#endif