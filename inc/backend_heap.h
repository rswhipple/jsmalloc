#ifndef BACKEND_HEAP_H  
#define BACKEND_HEAP_H

#include "types.h"

t_span* span_base_create(t_cache* cache);
t_span* span_add(t_pagemap* pagemap, size_t size);
void pagemap_create(t_pagemap** pagemap);
void pagemap_destroy(t_pagemap* pagemap);

void fpages_create(t_pagemap* pagemap);
t_fpage* fpage_base_create(t_pagemap* pagemap);
t_fpage* fpage_create(t_fpage* prev_page, int page_count,
        size_t chunk_size, t_cache* cache);
        
void pages_create(t_pagemap* pagemap, t_span* span);
t_page* page_base_create(t_pagemap* pagemap, t_span* span);
t_page* page_create(t_page* prev_page, int pagetype);
void destroy_active_page(t_page* page);
void destroy_page(t_page* page);


#endif