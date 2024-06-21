#include "../../inc/main.h" 


void tiny_chunk_print(t_tiny_chunk* tiny) {
    printf("t_tiny_chunk pointer = %p\n", tiny);
    if (tiny->next) printf("next t_tiny_chunk = %p\n", tiny->next);
}

t_tiny_chunk* tiny_chunk_top_create(t_fpage* page) {
    t_tiny_chunk* tiny = (t_tiny_chunk*)FASTPAGE_SHIFT(page);
    tiny->size = page->chunk_size;
    tiny->next = NULL;

    return tiny;
}

t_tiny_chunk* tiny_chunk_create(t_fpage* fpage) {
    t_tiny_chunk* tiny;
    tiny = (t_tiny_chunk*)MEMORY_SHIFT(fpage->last_chunk, fpage->chunk_size);
    fpage->chunk_count += 1;
    fpage->last_chunk = tiny;
    tiny->next = NULL;

    return tiny;
}

void tiny_chunk_free(void* ptr, size_t size) {
    t_tiny_chunk** f_cache = g_pagemap->frontend_cache->fast_cache;

    int index = get_fpage_index(size);
    t_tiny_chunk* tiny = (t_tiny_chunk*)((char*)ptr - sizeof(size_t));
    tiny->size = size;

    // insert tiny_chunk into head of fast_cache
    tiny->next = f_cache[index]->next;
    f_cache[index] = tiny;
}

