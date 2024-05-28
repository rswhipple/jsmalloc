#include "../inc/main.h" 



t_tiny_chunk* create_top_tiny_chunk(t_fpage* page) {
    t_tiny_chunk* tiny = (t_tiny_chunk*)FASTPAGE_SHIFT(page);
    tiny->next = NULL;
    tiny->data = (void*)MEMORY_SHIFT(TINY_CHUNK_SHIFT(tiny), 0);
    page->top_chunk = tiny;
    log_info("creating top tiny chunk");
    printf("tiny chunk pointer: %p\n", tiny);
    printf("data pointer: %p\n", tiny->data);
    // printf("tiny chunk size: %zu\n", page->chunk_size);
    printf("sizeof(t_tiny_chunk): %zu\n", sizeof(t_tiny_chunk));

    return tiny;
}

t_tiny_chunk* split_tiny_chunk(t_fpage* page, t_tiny_chunk* prev_chunk) {
    t_tiny_chunk* tiny;
    tiny = (t_tiny_chunk*)MEMORY_SHIFT(prev_chunk->data, page->chunk_size);

    // initialize new tiny chunk
    tiny->next = NULL;
    tiny->data = (void*)MEMORY_SHIFT(TINY_CHUNK_SHIFT(tiny), 0);

    // add to linked list
    if (prev_chunk) {
        prev_chunk->next = tiny;
    }

    return tiny;
}


// t_chunk *split_chunk(t_chunk *chunk, size_t size) {

// }

