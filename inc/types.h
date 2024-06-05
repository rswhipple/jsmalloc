#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <stdint.h>

// =================== Chunk ===================
typedef struct s_tiny_chunk t_tiny_chunk;
typedef struct s_chunk t_chunk;

struct s_tiny_chunk {
    size_t size;
    t_tiny_chunk* next;
};

struct s_chunk {
    size_t size;    // bounding size marker
    t_chunk* fd;
    t_chunk* bk;
};

// =================== Cache ===================
typedef struct cache_table cache_table;

struct s_cache_tablei {
    const char* key;  // current key
    void* value;      // current value

    // Don't use these fields directly.
    cache_table* _table;       // reference to hash table being iterated
    size_t _index;    // current index into cache_table._entries
};

typedef struct s_cache_tablei cache_tablei;

typedef struct s_cache t_cache;
typedef struct cache_table_s t_cache_table;
struct s_cache {
    t_tiny_chunk** fast_cache;
    size_t fcache_size;
    cache_table* cache_table;
    t_chunk* unsorted_cache;
};

// Hash table structure
typedef unsigned int (hash_function)(size_t input, uint32_t);

struct cache_table_s {
    size_t size;
    hash_function* hash;
    t_chunk** elements;
};

// =================== Pages ===================

typedef struct s_page t_page;
typedef struct s_fpage t_fpage;

struct s_page {
    t_page* next;
    t_page* prev;
    size_t pagetype;
    size_t memory;
    size_t chunk_count;
    t_chunk* top_chunk;
};

struct s_fpage {
    t_fpage* next;
    size_t memory;
    size_t chunk_size;
    size_t chunk_count;
    size_t max_chunks;
    t_tiny_chunk* last_chunk;
};

// =================== Span ===================

typedef struct s_span t_span;

struct s_span {
    t_span* next;
    t_fpage* fastpages;
    t_page* page_head;
    t_chunk* top_chunk;
    t_chunk* last_chunk;
    size_t num_pages;
    bool pages_returned;
};

// =================== PageMap ===================

typedef struct s_pagemap t_pagemap;

struct s_pagemap {
    t_cache* frontend_cache;
    t_span* span_head;
    size_t total_pages;
    // t_chunk* top_chunk;      // Holds the top chunk in the heap
    // t_chunk* last_chunk;     // Holds the chunk where memory was allocated last. Purpose is to help create objects in proximity to one another.
};


// =================== Enums ===================

enum page_types {
    small,
    large
};

// =================== V1 Heap ===================

typedef struct s_heap t_heap;
typedef struct s_block t_block;

struct s_heap {
    struct s_heap* prev;
    struct s_heap* next;
    size_t total_size;
    size_t free_size;
    int block_count;
    t_cache_table* cache_table;
};

struct s_block {
    struct s_block* prev;
    struct s_block* next;
    size_t data_size;
    bool freed;
    void* object;
};

#endif  // TYPES_H
