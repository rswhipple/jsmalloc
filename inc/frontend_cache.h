#ifndef FRONTEND_CACHE_H
#define FRONTEND_CACHE_H

#include "types.h"

void* search_fast_cache(size_t size);
void* search_cache(size_t size, int page_type);
void print_fast_cache(t_tiny_chunk** fast_cache);
t_cache* create_frontend_cache(t_pagemap* pagemap);
t_tiny_chunk** create_fast_cache(t_cache* cache);

size_t round_up_to_next(size_t number);

cache_table* cache_table_create(t_cache* cache);
void cache_table_destroy(cache_table* table);
void* cache_table_get(cache_table* table, const char* key);
const char* cache_table_set(cache_table* table, const char* key, t_chunk* value);
size_t cache_table_length(cache_table* table);
cache_tablei cache_table_iterator(cache_table* table);
bool cache_table_next(cache_tablei* it);

#endif