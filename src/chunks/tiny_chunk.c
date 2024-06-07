#include "../../inc/main.h" 


void print_tiny_chunk(t_tiny_chunk* tiny) {
    printf("t_tiny_chunk pointer = %p\n", tiny);
    if (tiny->next) printf("next t_tiny_chunk = %p\n", tiny->next);
}

t_tiny_chunk* create_top_tiny_chunk(t_fpage* page) {
    t_tiny_chunk* tiny = (t_tiny_chunk*)FASTPAGE_SHIFT(page);
    tiny->size = page->chunk_size;
    tiny->next = NULL;
    // char* data = (char*)MEMORY_SHIFT(tiny, sizeof(size_t));

    // log_info("creating top tiny chunk");
    // printf("tiny chunk pointer: %p\n", tiny);
    // printf("data pointer (same as memory location *next): %p\n", data);
    // printf("sizeof(t_tiny_chunk): %zu\n", sizeof(t_tiny_chunk));

    return tiny;
}

t_tiny_chunk* create_tiny_chunk(t_fpage* fpage) {
    t_tiny_chunk* tiny;
    tiny = (t_tiny_chunk*)MEMORY_SHIFT(fpage->last_chunk, fpage->chunk_size);
    fpage->chunk_count += 1;
    fpage->last_chunk = tiny;
    // TODO: log to double check that chunk_count and last_chunk are being assigned properly

    // initialize new tiny chunk
    tiny->next = NULL;

    return tiny;
}

void free_tiny_chunk(void* data) {
    t_tiny_chunk** f_cache = g_pagemap->frontend_cache->fast_cache;
    // TODO: make sure negative memory shift works
    size_t size = 
            (size_t)MEMORY_SHIFT(data, -TINY_CHUNK_OVERHEAD);

    int index = get_fpage_index(size);
    t_tiny_chunk* tiny = (t_tiny_chunk*)size;
    tiny->size = size;

    // insert tiny_chunk into head of fast_cache linked list
    tiny->next = f_cache[index]->next;
    f_cache[index] = tiny;
}

