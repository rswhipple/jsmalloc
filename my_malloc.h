#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#ifndef MY_MALLOC_H
#define MY_MALLOC_H

typedef struct s_heap {
    struct s_heap *prev;
    struct s_heap *next;
    size_t total_size;
    size_t free_size;
    size_t block_count;
} t_heap;

typedef struct s_block {
    struct s_block *prev;
    struct s_block *next;
    size_t data_size;
    bool freed;
} t_block;

#define HEAP_SHIFT(start)((void *)start + sizeof(t_heap))
#define BLOCK_SHIFT(start)((void *)start + sizeof(t_block))

#define TINY_HEAP_ALLOCATION_SIZE (4 * getpagesize())
#define TINY_BLOCK_SIZE (TINY_HEAP_ALLOCATION_SIZE / 128)
#define SMALL_HEAP_ALLOCATION_SIZE (16 * getpagesize())
#define SMALL_BLOCK_SIZE (SMALL_HEAP_ALLOCATION_SIZE / 128)

void *my_malloc(size_t size);
void my_free(size_t nitems, size_t size);
void *my_realloc(void *ptr, size_t size);
void *my_calloc(size_t nitems, size_t size);

#endif
