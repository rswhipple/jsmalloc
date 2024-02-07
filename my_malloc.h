#include <stdio.h>
#include <stdbool.h>

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

void *my_malloc(size_t size);
void my_free(size_t nitems, size_t size);
void *my_realloc(void *ptr, size_t size);
void *my_calloc(size_t nitems, size_t size);

#endif
