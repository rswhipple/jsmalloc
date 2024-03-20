#include "../inc/block.h" 

void print_blocks(t_heap *heap) {
    t_block *current_block = (t_block *)BLOCK_SHIFT(heap);
    while (current_block != NULL) {
        printf("block size: %zu\n", current_block->data_size);
        current_block = current_block->next;
    }
}

void *create_block(t_heap *heap, size_t size) {
    if (heap->block_count == 0) {
        t_block *block = (t_block *)BLOCK_SHIFT(heap);
        block->prev = NULL;
        block->next = NULL;
        block->data_size = size;
        block->freed = false;
        block->object = (t_block *)MEMORY_SHIFT(heap, size);
        heap->block_count++;
        heap->free_size -= size;
        printf("block created: %zu\n", block->data_size);
        printf("heap free size: %zu\n", heap->free_size);
        return block;
    } else {
        t_block *current_block = (t_block *)BLOCK_SHIFT(heap);
        // if (heap->block_count) {
            while (current_block->next != NULL) {
                current_block = current_block->next;
            }
        // }
        t_block *new_block = (t_block *)((char *)current_block + sizeof(t_block));
        
        new_block->prev = current_block;
        new_block->next = NULL;
        new_block->data_size = size;
        new_block->freed = false;
        new_block->object = (t_block *)MEMORY_SHIFT(heap, size);
        heap->block_count++;
        heap->free_size -= size;
        current_block->next = new_block;
        printf("block created: %zu\n", new_block->data_size);
        printf("heap free size: %zu\n", heap->free_size);
        return new_block;
    }
}
