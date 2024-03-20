#include "../inc/my_malloc.h" 

void search_ptr(t_heap **ptr_heap, t_block **ptr_block,
    t_heap *heap, void *ptr) {
    t_block *block;

    block = NULL;
    while (heap->next != NULL) {   
        block = (t_block *)BLOCK_SHIFT(heap);
        while (block) {
            if (block == ptr) {   
                printf("block found: %p\n", block);
                *ptr_heap = heap;
                *ptr_block = block;
                return ;
            }
            block = block->next;
        }
        heap = heap->next;
    }
    log_info("block not found");
    *ptr_heap = NULL;
    *ptr_block = NULL;
}

void unmap_heap(t_heap *heap) {
    if (heap->prev != NULL) {
        heap->prev->next = heap->next;
    }
    if (heap->next != NULL) {
        heap->next->prev = heap->prev;
    }
    if (heap == global_heap) {
        global_heap = heap->next;
    }
    munmap(heap, heap->total_size);
}

void my_free(void *ptr) {
    if (global_heap == NULL || ptr == NULL) {
        printf("Attempting to free NULL pointer or heap not initialized.\n");
        return;
    }

    t_heap	*heap;
    t_block	*block;
    heap = global_heap;
    // Find the Corresponding Block to Free:
    log_info("searching for block");
    search_ptr(&heap, &block, heap, ptr);
    // Mark the Block as Freed:
    if (block && heap) {
        log_info("found block and freeing");
        printf("%zu\n", block->data_size);
        printf("heap free size: %zu\n", heap->free_size);
        block->freed = true;
        // Update Heap Metadata:
        heap->free_size += block->data_size;
        heap->block_count--;
        printf("block freed: %zu\n", block->data_size);
        printf("heap free size: %zu\n", heap->free_size);

        // Coalesce Free Blocks:
        if (block->prev != NULL) {
            block->prev->next = block->next;
        }
        if (block->next != NULL) {
            block->next->prev = block->prev;
        }

        // Update Hash Table:

        // Unmap Heap if Empty:
        if (heap->block_count == 0) {
            unmap_heap(heap);
        }
    }
};