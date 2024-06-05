#include "../inc/main.h" 


void print_tiny_chunk(t_tiny_chunk* tiny) {
    printf("t_tiny_chunk pointer = %p\n", tiny);
    printf("data pointer (same as *next): %p\n", tiny->next);
    if (tiny->next) printf("next t_tiny_chunk = %p\n", tiny->next);
}

t_tiny_chunk* create_top_tiny_chunk(t_fpage* page) {
    t_tiny_chunk* tiny = (t_tiny_chunk*)FASTPAGE_SHIFT(page);
    tiny->size = page->chunk_size;
    tiny->next = NULL;
    // tiny->data = (void*)MEMORY_SHIFT(TINY_CHUNK_SHIFT(tiny), 0);

    log_info("creating top tiny chunk");
    printf("tiny chunk pointer: %p\n", tiny);
    printf("data pointer (same as *next): %p\n", tiny->next);
    printf("sizeof(t_tiny_chunk): %zu\n", sizeof(t_tiny_chunk));

    return tiny;
}

t_tiny_chunk* create_tiny_chunk(t_fpage* fpage) {
    t_tiny_chunk* tiny;
    tiny = (t_tiny_chunk*)MEMORY_SHIFT(fpage->last_chunk, fpage->chunk_size);

    // initialize new tiny chunk
    tiny->next = NULL;
    // tiny->data = (void*)MEMORY_SHIFT(TINY_CHUNK_SHIFT(tiny), 0);

    return tiny;
}

void free_tiny_chunk(t_tiny_chunk* tiny) {
    UNUSED(tiny);
    // find fast_cache bin by size


    // pagemap->frontend_cache->fast_cache[0]

}
