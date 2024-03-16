#include <stdio.h>
#include <sys/mman.h>
#include "../inc/my_malloc.h" 
#include "../inc/hash_table.h" 

t_heap *global_heap = NULL;

void create_heap(t_heap **heap, size_t size) {
    *heap = (t_heap *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    // (*heap)->prev = NULL;
    (*heap)->next = NULL;
    (*heap)->total_size = size;
    (*heap)->free_size = size;
    (*heap)->block_count = 0;
    (*heap)->ht = NULL;

}

void destroy_heap(t_heap *heap) {
    munmap(heap, heap->total_size);
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
        current_block->next = new_block;
        return new_block;
    }
}


void print_blocks(t_heap *heap) {
    t_block *current_block = BLOCK_SHIFT(heap);
    while (current_block != NULL) {
        printf("block size: %zu\n", current_block->data_size);
        current_block = current_block->next;
    }
}

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

void *my_realloc(void *ptr, size_t size) {
    UNUSED(ptr);
    UNUSED(size);
    return NULL;
};

void *calloc(size_t nitems, size_t size) {
    UNUSED(size);
    UNUSED(nitems);
    return NULL;
};

int main() {

    // For hash table testing
    const int tablesize = (1<<12);
    // initiate heap first
    t_heap *heap = NULL;
    int heap_size = 1000000;

    create_heap(&heap, heap_size);
    hash_table_create(heap, tablesize, my_hash_function);

    // For my_malloc testing
    printf("\n=====mallocing for small size heap=====\n");
    const void *ptr1 = my_malloc(10);
    printf("\n=====mallocing big size heap=====\n");
    const void *ptr2 = my_malloc(40);

    printf("\n=====mallocing medium?=====\n");
    const void *ptr3 = my_malloc(30);
    const void *ptr4 = my_malloc(900);
    const void *ptr5 = my_malloc(80);


    printf("pointer to malloced node: %p\n", ptr1);
    printf("pointer to malloced node: %p\n", ptr2);
    printf("pointer to malloced node: %p\n", ptr3);
    printf("pointer to malloced node: %p\n", ptr4);
    printf("pointer to malloced node: %p\n", ptr5);

    t_heap *current_heap = global_heap;
    while (current_heap != NULL) {
        printf("heap size: %zu\n", current_heap->free_size);
        print_blocks(current_heap);
        current_heap = current_heap->next;
    }

    // my_free(ptr1);
    return 0;
}