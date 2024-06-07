
#include "../../inc/main.h"

void* search_fast_cache(size_t size) {
    int index = get_fpage_index(size);
    t_tiny_chunk** f_cache = g_pagemap->frontend_cache->fast_cache;
    t_page* fpage = NULL;
    t_tiny_chunk* tiny;

    if (f_cache[index]) {
        // allocate top chunk in link list and replace
        tiny = f_cache[index];
        f_cache[index] = f_cache[index]->next;
    }
    else {
        // split off new chunk
        // TODO implement logic to find correct fpage
        // tiny = create_tiny_chunk(fpage);
        tiny = NULL;
        UNUSED(tiny);
    }

    // return (void*)MEMORY_SHIFT(tiny, TINY_CHUNK_OVERHEAD);
    printf("Returning NULL\n");
    return NULL;
}

void* search_cache(size_t size, int page_type) {
    UNUSED(size);
    UNUSED(page_type);
    printf("Returning NULL\n");
    return NULL;
}

void print_fast_cache(t_tiny_chunk** fast_cache) {
    t_tiny_chunk* temp;
    int count;
    int min_align;

    count = (min_chunk_size == 8) ? 8 : 7;
    min_align = (count == 8) ? 1 : 2;

    for (int i = 0; i < count; i++) {
        temp = fast_cache[i];
        if (temp) {
            while (temp) {
                log_info("tiny chunk");
                printf("size = %d\n", (i + min_align) * 8);
                print_tiny_chunk(temp);
                temp = temp->next;
            }
        }
        else {
            printf("fast_cache[%i] = NULL\n", i);
        }

    }
}

t_cache* create_frontend_cache(t_pagemap* pagemap) {
    log_info("creating frontend cache");
    t_cache* cache = (t_cache*)PAGEMAP_SHIFT(pagemap);
    printf("cache pointer: %p\n", cache);
    printf("sizeof(t_cache): %zu\n", sizeof(t_cache));
    if (min_chunk_size == 8) cache->fcache_size = 8;
    else cache->fcache_size = 7;
    cache->fast_cache = create_fast_cache(cache);
    cache->cache_table = cache_table_create(cache);     // add create_cache_table here or after create pages in base_page?
    cache->unsorted_cache = NULL;
    void* last_byte = (void*)MEMORY_SHIFT(cache, (NUM_BINS * sizeof(t_chunk*) + sizeof(t_cache)));
    printf("cache end: %p\n", last_byte);

    return cache;
}

t_tiny_chunk** create_fast_cache(t_cache* cache) {
    t_tiny_chunk** fast_cache = (t_tiny_chunk**)CACHE_SHIFT(cache);

    printf("fast_cache pointer: %p\n", fast_cache);
    for (size_t i = 0; i < cache->fcache_size; ++i) {
        fast_cache[i] = NULL;
    }

    void* last_byte = (void*)MEMORY_SHIFT(cache, (cache->fcache_size * sizeof(t_tiny_chunk*)));
    printf("fast_cache end: %p\n", last_byte);
    return fast_cache;
}

