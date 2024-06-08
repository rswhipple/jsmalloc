#include "../inc/main.h" 

void* my_malloc(size_t size) {
    if (g_pagemap == NULL) {
        create_pagemap(&g_pagemap);
    }

    // TODO error check for negatives

    // get category
    int page_type = get_page_type(size);

    // search heap
    void* ptr = search_heap(size, page_type);

    if (ptr) return ptr;

    printf("returning NULL\n");
    return NULL;
};

void* search_heap(size_t size, int page_type) {
    switch (page_type) {
    case 1: return search_fast_cache(size);
    case 2:
    case 3: return search_cache(size, page_type);
    case 4: return allocate_huge_chunk(size);
    default: break;
    }

    printf("returning NULL\n");
    return NULL;
}

int get_page_type(size_t size) {
    if (size < FAST_MAX_CHUNK_SIZE - TINY_CHUNK_OVERHEAD) return 1;
    else if (size < (size_t)(SMALL_MAX_CHUNK_SIZE - CHUNK_OVERHEAD)) return 2;
    else if (size < (size_t)(LARGE_MAX_CHUNK_SIZE - CHUNK_OVERHEAD)) return 3;
    else if (size < (size_t)BASE_HEAP_SIZE - CHUNK_OVERHEAD) return 4;
    else return 0;
}

