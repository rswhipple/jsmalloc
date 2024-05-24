// types.h
#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>

typedef struct s_span t_span;
typedef struct hash_table_s t_hash;
typedef struct s_pagemap t_pagemap;
typedef struct s_page t_page;
typedef struct s_fpage t_fpage;
typedef struct s_chunk t_chunk;
typedef struct s_tiny_chunk t_tiny_chunk;
typedef struct s_heap t_heap;
typedef struct s_block t_block;

struct s_pagemap {
    t_span* span_head;
    size_t total_pages;
};

struct s_span {
    t_span* next;
    t_fpage* fastpages;
    t_page* page_head;
    t_chunk* top_chunk;
    t_chunk* last_chunk;
    size_t num_pages;
    bool pages_returned;
};

struct s_page {
    t_page* next;
    t_page* prev;
    size_t pagetype;
    size_t memory;
    size_t chunk_count;
    t_chunk* top_chunk;
};

struct s_fpage {
    t_page* next;
    t_page* prev;
    size_t memory;
    size_t chunk_size;
    size_t chunk_count;
    t_chunk* top_chunk;
};

enum page_types
{
    fast,
    small,
    large
};

struct s_chunk {
    size_t size;    // bounding size marker
    t_chunk* prev;
    t_chunk* next;
    void* data;     // TODO: figure out how to write over the pointers while in use
    size_t chunk_size;  // bounding size markers so that we can traverse the chunks in either direction even when in use
};

struct s_tiny_chunk {
    t_page *parent;
    t_chunk* next;
    void* data;     // TODO: figure out how to write over the pointers while in use
};

struct s_heap {
    struct s_heap* prev;
    struct s_heap* next;
    size_t total_size;
    size_t free_size;
    int block_count;
    t_hash* ht;
};

struct s_block {
    struct s_block* prev;
    struct s_block* next;
    size_t data_size;
    bool freed;
    void* object;
};

// Hash table structure
typedef unsigned int (hash_function)(size_t input, uint32_t);
struct hash_table_s {
    size_t size;
    hash_function* hash;
    t_chunk** elements;
};

#define UNUSED(x) (void)(x)

extern size_t min_chunk_size;
extern t_heap* global_heap;

#endif  // TYPES_H
