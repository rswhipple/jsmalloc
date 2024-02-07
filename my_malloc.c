#include <stdio.h>
#include "my_malloc.h"
#include <sys/mman.h>

t_heap *global_heap = NULL;

void *my_malloc(size_t size) {
    int allocation_size = size < TINY_BLOCK_SIZE ? TINY_HEAP_ALLOCATION_SIZE : SMALL_HEAP_ALLOCATION_SIZE;

    if (global_heap == NULL) {
        global_heap = mmap(0, allocation_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);;
        global_heap->prev = NULL;
        global_heap->next = NULL;
        
        // Use size to determine if it's a tiny or small allocation
        global_heap->total_size = allocation_size;
        global_heap->free_size = allocation_size;
        global_heap->block_count = 0;

        // Create a block
        t_block *block = BLOCK_SHIFT(global_heap);
        block->prev = NULL;
        block->next = NULL;
        block->data_size = size;
        block->freed = false;
        global_heap->block_count++;
        global_heap->free_size -= size;
        return BLOCK_SHIFT(global_heap);
    }
    return NULL;
};

void my_free(size_t nitems, size_t size) {
};

void *my_realloc(void *ptr, size_t size) {
    return NULL;
};

void *calloc(size_t nitems, size_t size) {
    return NULL;
};

int main() {
    // char *ptr = my_malloc(10);

    // printf("pointer to malloced node: %s\n", ptr);
    return 0;
}