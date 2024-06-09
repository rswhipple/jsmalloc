
#include "../../inc/main.h"

t_cache* create_frontend_cache(t_pagemap* pagemap) {
    // log_info("creating frontend cache");
    t_cache* cache = (t_cache*)PAGEMAP_SHIFT(pagemap);
    // printf("cache pointer: %p\n", cache);
    // printf("sizeof(t_cache): %zu\n", sizeof(t_cache));
    if (min_chunk_size == 8) cache->fcache_size = 8;
    else cache->fcache_size = 7;
    cache->fast_cache = create_fast_cache(cache);
    cache->cache_table = cache_table_create(cache);     // add create_cache_table here or after create pages in base_page?
    cache->unsorted_cache = NULL;
    // void* last_byte = (void*)MEMORY_SHIFT(cache, (NUM_BINS * sizeof(t_chunk*) + sizeof(t_cache)));
    // printf("cache end: %p\n", last_byte);

    return cache;
}

t_tiny_chunk** create_fast_cache(t_cache* cache) {
    t_tiny_chunk** fast_cache = (t_tiny_chunk**)CACHE_SHIFT(cache);

    // printf("fast_cache pointer: %p\n", fast_cache);
    for (size_t i = 0; i < cache->fcache_size; ++i) {
        fast_cache[i] = NULL;
    }

    // void* last_byte = (void*)MEMORY_SHIFT(cache, (cache->fcache_size * sizeof(t_tiny_chunk*)));
    // printf("fast_cache end: %p\n", last_byte);
    return fast_cache;
}

void* search_fast_cache(size_t size) {
    int index = get_fpage_index(size);
    t_tiny_chunk** f_cache = g_pagemap->frontend_cache->fast_cache;
    t_page* fpage = NULL;
    t_tiny_chunk* tiny;
    UNUSED(tiny);
    UNUSED(fpage);

    if (f_cache[index]) {
        // allocate top chunk in link list and replace
        tiny = f_cache[index];
        f_cache[index] = f_cache[index]->next;
    }
    else {
        // split off new chunk
        // TODO check logic
        fpage = g_pagemap->span_head->fastpages;
        while (index > 0) {
            fpage = fpage->next;
            index--;
        }
        tiny = create_tiny_chunk(fpage);
    }

    return (void*)MEMORY_SHIFT(tiny, TINY_CHUNK_OVERHEAD);
}

void* search_unsorted_cache(size_t size) {
    t_chunk* unsorted_cache = g_pagemap->frontend_cache->unsorted_cache;
    while (unsorted_cache) {
        printf("unsorted_cache->size: %zu\n", unsorted_cache->size);
        if (unsorted_cache->size >= size) {
            return unsorted_cache;
        }
        unsorted_cache = unsorted_cache->fd;
    }

    return NULL;
}


void* search_sorted_cache(size_t size, int page_type) {
    char key[32];
    snprintf(key, sizeof(key), "%zu", size);

    cache_table* cache_table = g_pagemap->frontend_cache->cache_table;
    t_page* page_head = g_pagemap->span_head->page_head;
    if (page_type == 3) {
        while ((int)page_head->pagetype != page_type) {
            page_head = page_head->next;
        }
        page_head = g_pagemap->span_head->page_head;
    }
    t_chunk* chunk = page_head->top_chunk;

    if ((cache_table_set(cache_table, key, chunk)) != NULL) {
        return cache_table_get(cache_table, key);
    }

    printf("Returning NULL\n");
    return NULL;
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