
#include "../../inc/main.h"


t_cache* frontend_cache_create(t_pagemap* pagemap) {
    t_cache* cache = (t_cache*)PAGEMAP_SHIFT(pagemap);

    if (min_chunk_size == 8) cache->fcache_size = 8;
    else cache->fcache_size = 7;

    cache->fast_cache = fast_cache_create(cache);
    cache->cache_table = cache_table_create(cache);
    cache->unsorted_cache = NULL;

    return cache;
}

void* search_cache(size_t size, int page_type) {
    void* ptr = NULL;
    size = round_up_to_next(size);

    if ((ptr = search_unsorted_cache(size)) != NULL) {
        return ptr;
    }
    if (page_type == 2) {
        return search_sorted_cache(size, page_type);
    }
    else if (page_type == 3) {
        return search_unsorted_cache(size);
    }

    printf("Returning NULL\n");
    return NULL;
}
