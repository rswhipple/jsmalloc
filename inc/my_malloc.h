#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include "hash_table.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct s_heap {
    struct s_heap *prev;
    struct s_heap *next;
    size_t total_size;
    size_t free_size;
    size_t block_count;
    hash_table *ht;
} t_heap;

typedef struct s_block {
    struct s_block *prev;
    struct s_block *next;
    size_t data_size;
    bool freed;
    void *object;
} t_block;

// Macros that return the address of the block and heap
// based on the start of the heap
#define HEAP_SHIFT(start)((void *)start + sizeof(t_heap))
#define BLOCK_SHIFT(start)((void *)start + sizeof(t_block))

#define HASH_SHIFT(start)((void *)start + sizeof(t_hash))
#define MEMORY_SHIFT(start, data_size)((void *)start + data_size)

#define TINY_HEAP_ALLOCATION_SIZE (4 * getpagesize())
#define TINY_BLOCK_SIZE (TINY_HEAP_ALLOCATION_SIZE / 128)
#define SMALL_HEAP_ALLOCATION_SIZE (16 * getpagesize())
#define SMALL_BLOCK_SIZE (SMALL_HEAP_ALLOCATION_SIZE / 128)

void *my_malloc(size_t size);
void my_free(size_t nitems, size_t size);
void *my_realloc(void *ptr, size_t size);
void *my_calloc(size_t nitems, size_t size);

#endif
