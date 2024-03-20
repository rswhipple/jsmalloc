#include "../inc/my_malloc.h" 

void search_ptr(t_heap **ptr_heap, t_block **ptr_block,
    t_heap *heap, void *ptr) {
    t_block *block;

    block = NULL;
    while (heap)
    {
        block = (t_block *)HEAP_SHIFT(heap);
        while (block)
        {
            if (BLOCK_SHIFT(block) == ptr)
            {
                *ptr_heap = heap;
                *ptr_block = block;
                return ;
            }
            block = block->next;
        }
        heap = heap->next;
    }
    *ptr_heap = NULL;
    *ptr_block = NULL;
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
    if (block && heap)
    {
        printf("found block: %zu\n", block->data_size);
        printf("heap free size: %zu\n", heap->free_size);
        block->freed = true;
        heap->free_size += block->data_size;
        printf("block freed: %zu\n", block->data_size);
        printf("heap free size: %zu\n", heap->free_size);
    }
    // Coalesce Free Blocks:

    // Update Heap Metadata:

    // Update Hash Table:

    // Unmap Heap if Empty:

};