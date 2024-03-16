
#include "../inc/my_malloc.h" 
#include "../inc/hash_table.h" 
#include "../inc/heap.h" 

t_heap *global_heap = NULL;

void *my_malloc(size_t size) {
    // Still need to handle LARGE_HEAP_ALLOCATION_SIZE
    int allocation_size = size < (size_t)TINY_BLOCK_SIZE ? TINY_HEAP_ALLOCATION_SIZE : SMALL_HEAP_ALLOCATION_SIZE;

    if (global_heap == NULL) {
        printf("global_heap is NULL\n");
        create_heap(&global_heap, allocation_size);
        return create_block(global_heap, size);
    } else {
        t_heap *current_heap = global_heap;
        printf("current_heap: %zu\n", current_heap->free_size);
        while (current_heap->next != NULL) {
            if (current_heap->total_size == (size_t)allocation_size && current_heap->free_size >= size) {
                printf("found correct heap size: %zu\n", current_heap->free_size);
                return create_block(current_heap, size);
            } 
            printf("moving to next heap\n");
            current_heap = current_heap->next;
        }

        t_heap *new_heap = NULL;
        create_heap(&new_heap, allocation_size);
        printf("new_heap count: %d\n", new_heap->block_count);
        // Add the new heap to the end of the heap list
        t_heap *last_heap = global_heap;
        while (last_heap->next != NULL) {
            last_heap = last_heap->next;
        }
        last_heap->next = new_heap;
        printf("new_heap: %zu\n", new_heap->free_size);
        // Return the newly allocated block
        return create_block(new_heap, size);
    }

    printf("returning NULL\n");
    return NULL;
};



