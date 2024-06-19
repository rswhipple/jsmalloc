
#include "../../inc/main.h"


/*

*/

t_cache* create_frontend_cache(t_pagemap* pagemap) {
    // log_info("creating frontend cache");
    t_cache* cache = (t_cache*)PAGEMAP_SHIFT(pagemap);
    // printf("cache pointer: %p\n", cache);
    // printf("sizeof(t_cache): %zu\n", sizeof(t_cache));
    if (min_chunk_size == 8) cache->fcache_size = 8;
    else cache->fcache_size = 7;
    cache->fast_cache = create_fast_cache(cache);
    cache->cache_table = cache_table_create(cache);
    cache->unsorted_cache = NULL;
    // void* last_byte = (void*)MEMORY_SHIFT(cache, (NUM_BINS * sizeof(t_chunk*) + sizeof(t_cache)));
    // printf("cache end: %p\n", last_byte);
    // log_info("created frontend cache");

    // printf("cache->cache_table->capacity: %zu\n", cache->cache_table->capacity);
    return cache;
}

void* search_cache(size_t size, int page_type) {
    void* ptr = NULL;

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
