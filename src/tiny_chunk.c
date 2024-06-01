#include "../inc/main.h" 


void print_tiny_chunk(t_tiny_chunk* tiny) {
    printf("data pointer = %p\n", tiny->data);
    if (tiny->next) printf("next pointer = %p\n", tiny->next);
}

t_tiny_chunk* create_top_tiny_chunk(t_fpage* page) {
    t_tiny_chunk* tiny = (t_tiny_chunk*)FASTPAGE_SHIFT(page);
    tiny->next = NULL;
    tiny->data = (void*)MEMORY_SHIFT(TINY_CHUNK_SHIFT(tiny), 0);

    log_info("creating top tiny chunk");
    printf("tiny chunk pointer: %p\n", tiny);
    printf("data pointer: %p\n", tiny->data);
    printf("sizeof(t_tiny_chunk): %zu\n", sizeof(t_tiny_chunk));

    return tiny;
}

t_tiny_chunk* create_tiny_chunk(t_fpage* fpage) {
    t_tiny_chunk* tiny;
    tiny = (t_tiny_chunk*)MEMORY_SHIFT(fpage->last_chunk, fpage->chunk_size);

    // initialize new tiny chunk
    tiny->next = NULL;
    tiny->data = (void*)MEMORY_SHIFT(TINY_CHUNK_SHIFT(tiny), 0);

    return tiny;
}



