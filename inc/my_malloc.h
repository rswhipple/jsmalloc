#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpointer-arith"
#endif

#define UNUSED(x) (void)(x)

// #include "hash_table.h"
typedef struct hash_table_s t_hash;
typedef unsigned int (hash_function)(size_t input, uint32_t);

typedef struct s_heap {
    struct s_heap *prev;
    struct s_heap *next;
    size_t total_size;
    size_t free_size;
    int block_count;
    t_hash *ht;
} t_heap;

typedef struct s_block {
    struct s_block *prev;
    struct s_block *next;
    size_t data_size;
    bool freed;
    void *object;
} t_block;

struct hash_table_s {
    uint32_t size;
    hash_function *hash;
    t_block **elements;
};

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

#define MAX_BLOCKS 20   // TODO: 20 is an arbitrary size 

extern t_heap *global_heap;

void *my_malloc(size_t size);
void my_free(void *ptr);
void *my_realloc(void *ptr, size_t size);
void *my_calloc(size_t nitems, size_t size);
void *create_block(t_heap *heap, size_t size);
void print_blocks(t_heap *heap);

#endif
