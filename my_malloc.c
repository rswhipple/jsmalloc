#include <stdio.h>
#include "my_malloc.h"
#include <sys/mman.h>

t_heap *global_heap = NULL;

void create_heap(t_heap *heap, size_t size) {
    heap = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    heap->prev = NULL;
    heap->next = NULL;
    heap->total_size = size;
    heap->free_size = size;
    heap->block_count = 0;
};

void *my_malloc(size_t size) {
    // Still need to handle LARGE_HEAP_ALLOCATION_SIZE
    int allocation_size = size < TINY_BLOCK_SIZE ? TINY_HEAP_ALLOCATION_SIZE : SMALL_HEAP_ALLOCATION_SIZE;

    if (global_heap == NULL) {
        create_heap(global_heap, allocation_size);

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