#include <stdio.h>
#include <sys/mman.h>
#include "../inc/my_malloc.h" 
#include "../inc/hash_table.h" 

t_heap *global_heap = NULL;

void create_heap(t_heap **heap, size_t size) {
    *heap = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    (*heap)->prev = NULL;
    (*heap)->next = NULL;
    (*heap)->total_size = size;
    (*heap)->free_size = size;
    (*heap)->block_count = 0;
    (*heap)->ht = NULL;

    *heap = HEAP_SHIFT(heap);   // TODO: I think we need this but want to double check my logic
}

void destroy_heap(t_heap *heap) {
    munmap(heap, heap->total_size);
}

void *create_block(t_heap *heap, size_t size) {

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
        t_block *current_block = BLOCK_SHIFT(heap);
        while (current_block->next != NULL) {
            printf("current_block: %zu\n", current_block->data_size);
            current_block = current_block->next;
        }
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
        create_heap(&global_heap, allocation_size);

        return create_block(global_heap, size);
    } else {
        t_heap *current_heap = global_heap;
        while (current_heap != NULL) {
            if (current_heap->total_size == (size_t)allocation_size && current_heap->free_size >= size) {
                printf("current_heap: %zu\n", current_heap->total_size);
                return create_block(current_heap, size);
            }
            current_heap = current_heap->next;
        }
    }

    printf("returning NULL\n");
    return NULL;
};

void my_free(size_t nitems, size_t size) {
    UNUSED(nitems);
    UNUSED(size);
    // int allocation_size = size < TINY_BLOCK_SIZE ? TINY_HEAP_ALLOCATION_SIZE : SMALL_HEAP_ALLOCATION_SIZE;

    // Find the Corresponding Block to Free:
    
    // Mark the Block as Freed:

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
    char *ptr1 = my_malloc(10);
    char *ptr2 = my_malloc(25);
    char *ptr3 = my_malloc(30);

    printf("pointer to malloced node: %p\n", ptr1);
    printf("pointer to malloced node: %p\n", ptr2);
    printf("pointer to malloced node: %p\n", ptr3);

    print_blocks(global_heap);
    return 0;
}