#include "../inc/main.h" 


void* my_malloc(size_t size) {
    if (g_pagemap == NULL) {
        system_settings();
        pagemap_create(&g_pagemap);
    }

    // get category & search heap
    int page_type = get_page_type(size);
    void* ptr = search_heap(size, page_type);
    memset(ptr, 0, size);
    if (ptr) return ptr;

    printf("returning NULL\n");
    return NULL;
};

void* search_heap(size_t size, int page_type) {
    switch (page_type) {
    case 1: return search_fast_cache(size);
    case 2:
    case 3: return search_cache(size, page_type);
    case 4: return huge_chunk_allocate(size);
    default: break;
    }

    return NULL;
}

int get_page_type(size_t size) {
    if (size < FAST_MAX_CHUNK_SIZE - TINY_CHUNK_OVERHEAD) return 1;
    else if (size < (size_t)(SMALL_MAX_CHUNK_SIZE - CHUNK_OVERHEAD)) return 2;
    else if (size < (size_t)(LARGE_MAX_CHUNK_SIZE - CHUNK_OVERHEAD)) return 3;
    else if (size < (size_t)BASE_HEAP_SIZE - CHUNK_OVERHEAD) return 4;
    else return 0;
}

