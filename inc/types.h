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
typedef struct s_cache_table t_cache_table;
typedef struct s_cache t_cache;

struct s_cache {
    t_tiny_chunk** fast_cache;
    t_cache_table* cache_table;
    t_chunk* unsorted_cache;
    size_t fcache_size;
};

typedef struct {
    const char* key;  // key is NULL if this slot is empty
    t_chunk* value;
} cache_table_entry;

struct s_cache_table {
    cache_table_entry* entries;
    size_t capacity;
};

// =================== Pages ===================

typedef struct s_page t_page;
typedef struct s_fpage t_fpage;

struct s_page {
    t_page* next;
    void* memory;
    size_t memory_size;
    size_t pagetype;
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
    t_chunk* top_chunk;      // Holds the top chunk in the heap
    t_chunk* last_chunk;     // Holds the chunk where memory was allocated last. Purpose is to help create objects in proximity to one another.
    size_t total_pages;
};


// =================== Enums ===================

enum page_types {
    fast = 1,
    small,
    large,
    huge
};

#endif  // TYPES_H
