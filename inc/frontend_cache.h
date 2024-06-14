#ifndef FRONTEND_CACHE_H
#define FRONTEND_CACHE_H

#include "types.h"

// =================== Search Cache ===================
void* search_fast_cache(size_t size);
void* search_cache(size_t size, int page_type);
void* search_sorted_cache(size_t size, int page_type);
void* search_unsorted_cache(size_t size);

// =================== Print Cache ===================
void print_fast_cache(t_tiny_chunk** fast_cache);

// =================== Create Cache ===================
t_cache* create_frontend_cache(t_pagemap* pagemap);
t_tiny_chunk** create_fast_cache(t_cache* cache);

// =================== Cache Table ===================
t_cache_table* cache_table_create(t_cache* cache);
void cache_table_destroy(t_cache_table* table);
void* cache_table_get(t_cache_table* table, const char* key);
const char* cache_table_set(t_cache_table* table, const char* key, t_chunk* value);

#endif