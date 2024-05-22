#include "../inc/chunk.h" 

/*
void print_blocks(t_heap *heap) {
    t_block *current_block = (t_block *)BLOCK_SHIFT(heap);
    while (current_block != NULL) {
        printf("block size: %zu\n", current_block->data_size);
        current_block = current_block->next;
    }
}
*/
t_chunk *create_chunk(t_page *page) {
    t_chunk *chunk = (t_chunk *)PAGE_SHIFT(page);
    chunk->prev = NULL;
    chunk->next = NULL;
    chunk->chunk_size;

    return chunk;
}

t_tiny_chunk *create_tiny_chunk(t_page *page, size_t size) {
    t_tiny_chunk *tiny = (t_tiny_chunk *)PAGE_SHIFT(page);
    tiny->next = NULL;
    tiny->data;

    return tiny;
}

t_chunk *split_chunk(t_chunk *chunk, size_t size) {

}

