#include "../inc/main.h" 

/*
void print_chunks() {
    t_block *current_block = (t_block *)BLOCK_SHIFT(heap);
    while (current_block != NULL) {
        printf("block size: %zu\n", current_block->data_size);
        current_block = current_block->next;
    }
}
*/


t_chunk* create_top_chunk(t_page* page) {
    t_chunk* chunk = (t_chunk*)PAGE_SHIFT(page);
    chunk->size = page->memory - (sizeof(t_chunk));    // set size to 0 when chunk is IN USE
    chunk->prev = NULL;
    chunk->next = NULL;
    chunk->data = (void*)MEMORY_SHIFT(CHUNK_SHIFT(chunk), 0);
    chunk->chunk_size = page->memory - (sizeof(t_chunk));
    log_info("creating top chunk");
    printf("chunk pointer: %p\n", chunk);
    printf("chunk's data pointer: %p\n", chunk->data);
    printf("chunk size = %zu\n", chunk->chunk_size);
    printf("sizeof t_chunk header: %zu\n", sizeof(t_chunk));
    // TODO figure out how to write chunk_size after the data block
    // TODO make sure that you should subtract t_chunk here and not later
    page->top_chunk = chunk;
    return chunk;
}

t_tiny_chunk* create_top_tiny_chunk(t_fpage* page) {
    t_tiny_chunk* tiny = (t_tiny_chunk*)FASTPAGE_SHIFT(page);
    tiny->parent = page;
    tiny->next = NULL;
    tiny->data = (void*)MEMORY_SHIFT(TINY_CHUNK_SHIFT(tiny), 0);
    page->top_chunk = tiny;

    return tiny;
}

t_tiny_chunk* split_tiny_chunk(t_fpage* page, t_tiny_chunk* prev_chunk) {
    t_tiny_chunk* tiny;
    tiny = (t_tiny_chunk*)MEMORY_SHIFT(prev_chunk->data, page->chunk_size);

    // initialize new tiny chunk
    tiny->parent = page;
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

