#ifndef FRONTEND_CACHE_H
#define FRONTEND_CACHE_H

#include "types.h"

void print_fast_cache(t_tiny_chunk** fast_cache);
t_cache* create_frontend_cache(t_pagemap* pagemap);
t_tiny_chunk** create_fast_cache(t_cache* cache);

size_t round_up_to_next(size_t number);
t_chunk** create_cache_table(t_cache* cache);

// unsigned int my_hash_function(size_t data_size, uint32_t table_size);
size_t round_up_to_next(size_t number);
t_chunk** create_cache_table(t_cache* cache);
// void cache_table_destroy(t_cache_table* ht);
// void cache_table_print(t_cache_table* ht);
// bool cache_table_insert(t_heap **heap, t_cache_table *ht, const char *key, size_t value);
// void* cache_table_allocate(t_cache_table* ht, size_t key);
// void *cache_table_deallocate(t_block *block);
// void *cache_table_deallocate(t_cache_table *ht, const char *key);

#endif