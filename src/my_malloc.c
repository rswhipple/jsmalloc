#include "../../inc/main.h" 

void* my_malloc(size_t size) {
    if (g_pagemap == NULL) {
        create_pagemap(&g_pagemap);
    }

    // error check for negatives

    // get category
    int category = get_size_category(size);

    // search heap
    void* ptr = search_heap(size, category);
    UNUSED(ptr);

    printf("returning NULL\n");
    return NULL;
};

void* search_heap(size_t size, int category) {
    switch (category) {
        case 1: return search_fast_cache(size);
        case 2: return search_cache(size, category);
        case 3: return search_cache(size, category);
        case 4: return allocate_huge_chunk(size);
        default: break;
    }

    printf("returning NULL\n");
    return NULL;
}

int get_size_category(size_t size) {
    if (size < FAST_MAX_CHUNK_SIZE) return 1;
    else if (size < SMALL_MAX_CHUNK_SIZE) return 2;
    else if (size < LARGE_MAX_CHUNK_SIZE) return 3;
    else if (size < BASE_HEAP_SIZE) return 4;
    else return 0;
}

