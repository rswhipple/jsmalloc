#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>
#include "utils.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpointer-arith"
#endif

#define UNUSED(x) (void)(x)

typedef struct s_pagemap t_pagemap;
typedef struct s_span t_span;
typedef struct s_page t_page;
typedef struct s_chunk t_chunk;
typedef struct s_tiny_chunk t_tiny_chunk;

struct s_pagemap {
    t_span *span_head;
    size_t total_pages;
};

struct s_span {
    t_span *next;   
    t_page *page_head;
    t_chunk *top_chunk;
    t_chunk *last_chunk;
    size_t num_pages;    
    bool pages_returned;
};

struct s_page {
    t_page *next;  
    t_page *prev;
    char type;   
    size_t memory;       
    size_t chunk_count;   
    t_chunk *top_chunk;     
};

struct s_chunk {
    size_t size;    // bounding size marker
                    // TODO: figure out how to incorporate an in_use flag into the size bytes ?
    t_chunk* prev;
    t_chunk* next;
    void* data;     // TODO: figure out how to write over the pointers while in use
    size_t chunk_size;  // bounding size markers so that we can traverse the chunks in either direction even when in_use
};

struct s_tiny_chunk {
    size_t size;    // TODO: figure out how to incorporate an in_use flag into the size bytes ?
    t_chunk* next;
    void* data;     // TODO: figure out how to write over the pointers while in use
};

// TODO: create a system for only allowing certain hash bins (standard sizes from min_chunk_size to tbd max)
typedef struct hash_table_s t_hash;
typedef unsigned int (hash_function)(size_t input, uint32_t);

struct hash_table_s {
    size_t size;
    hash_function* hash;
    t_chunk** elements;
};

// Macros that return the address of datatypes within the heap
// based on the start of the heap
// #define PAGEMAP_SHIFT(start)((u_int8_t *)start + sizeof(t_pagemap))
#define SPAN_SHIFT(start)((u_int8_t *)start + sizeof(t_span))
#define PAGE_SHIFT(start)((u_int8_t *)start + sizeof(t_page))
#define CHUNK_SHIFT(start)((u_int8_t *)start + sizeof(t_chunk))
#define HASH_SHIFT(start)((u_int8_t *)start + sizeof(t_hash))
#define MEMORY_SHIFT(start, data_size)((u_int8_t *)start + data_size)

#define PAGE_SIZE getpagesize()
#define BASE_HEAP_SIZE (48 * PAGE_SIZE)
#define FAST_PAGE_ALLOCATION_SIZE (8 * PAGE_SIZE)
#define FAST_PAGE_MAX_CHUNK_SIZE 64
#define SMALL_HEAP_ALLOCATION_SIZE (20 * PAGE_SIZE)
#define SMALL_PAGE_MAX_CHUNK_SIZE 512
#define LARGE_HEAP_ALLOCATION_SIZE (20 * PAGE_SIZE)
#define LARGE_PAGE_MAX_CHUNK_SIZE (LARGE_HEAP_ALLOCATION_SIZE / 12)

extern size_t min_chunk_size;

void* my_malloc(size_t size);
void my_free(void* ptr);
void* my_realloc(void* ptr, size_t size);
void* my_calloc(size_t nitems, size_t size);
// void* create_block(t_heap* heap, size_t size);
// void print_blocks(t_heap* heap);

#endif
