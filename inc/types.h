// types.h
#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <stdint.h>

typedef struct s_span t_span;
typedef struct hash_table_s t_hash;
typedef struct s_pagemap t_pagemap;
typedef struct s_page t_page;
typedef struct s_fpage t_fpage;
typedef struct s_heap t_heap;
typedef struct s_block t_block;

struct s_pagemap {
    t_span* span_head;
    size_t total_pages;
};

enum page_types {
    fast,
    small,
    large
};

struct s_base_chunk {
    void* data;
    struct s_base_chunk* next;
};

struct s_chunk {
    size_t size;
    struct s_base_chunk base;
    struct s_base_chunk* prev;
    size_t chunk_size;
};

struct s_tiny_chunk {
    struct s_base_chunk base;
    struct s_page* parent;
};

typedef struct s_base_chunk t_base_chunk;
typedef struct s_chunk t_chunk;
typedef struct s_tiny_chunk t_tiny_chunk;

struct s_span {
    t_span* next;
    t_fpage* fastpages;
    t_page* page_head;
    t_chunk* top_chunk;
    t_chunk* last_chunk;
    size_t num_pages;
    bool pages_returned;
};

struct s_base_page {
    struct s_base_page* next;
    struct s_base_page* prev;
    size_t memory;
    size_t chunk_count;
    t_base_chunk* top_chunk;
};

struct s_page {
    struct s_base_page base;
    size_t pagetype;
};

struct s_fpage {
    struct s_base_page base;
    size_t chunk_size;
};

typedef struct s_base_page t_base_page;
typedef struct s_page t_page;
typedef struct s_fpage t_fpage;

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
