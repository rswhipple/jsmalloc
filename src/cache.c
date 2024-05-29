
#include "../inc/main.h"


t_cache* create_frontend_cache(t_pagemap* pagemap) {
    log_info("creating frontend cache");
    t_cache* cache = (t_cache*)PAGEMAP_SHIFT(pagemap);;
    cache->fast_cache = NULL;
    if (min_chunk_size == 8) cache->fcache_size = 8; 
    else cache->fcache_size = 7; 
    cache->sorted_cache = NULL;     // add create_hash_table here or after create pages in base_page?
    cache->unsorted_cache = NULL;
}

