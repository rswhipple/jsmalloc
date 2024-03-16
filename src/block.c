#include "../inc/block.h" 

void print_blocks(t_heap *heap) {
    t_block *current_block = BLOCK_SHIFT(heap);
    while (current_block != NULL) {
        printf("block size: %zu\n", current_block->data_size);
        current_block = current_block->next;
    }
}

void *create_block(t_heap *heap, size_t size) {
    printf("heap total size: %zu\n", heap->total_size);
    if (heap->block_count == 0) {
        t_block *block = BLOCK_SHIFT(heap);
        printf("block: %zu\n", block->data_size);
        block->prev = NULL;
        block->next = NULL;
        block->data_size = size;
        block->freed = false;
        block->object = MEMORY_SHIFT(heap, size);
        heap->block_count++;
        heap->free_size -= size;
        return block;
    } else {
        t_block *current_block = (t_block *)BLOCK_SHIFT(heap);
        printf("current block data size: %zu\n", current_block->data_size);
        printf("heap block count: %d\n", heap->block_count);
        // if (heap->block_count) {
            while (current_block->next != NULL) {
                printf("iterating current_block: %zu\n", current_block->data_size);
                current_block = current_block->next;
            }
        // }
        t_block *new_block = (t_block *)((char *)current_block + sizeof(t_block));
        
        printf("new_block: %zu\n", new_block->data_size);
        new_block->prev = current_block;
        new_block->next = NULL;
        new_block->data_size = size;
        new_block->freed = false;
        new_block->object = MEMORY_SHIFT(heap, size);
        heap->block_count++;
        heap->free_size -= size;
        current_block->next = new_block;
        return new_block;
    }
}
