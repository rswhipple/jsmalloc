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
t_chunk* cache_table_get(size_t size);
int cache_table_set(t_chunk* value);
bool cache_table_is_bin_head(t_chunk* value);
int cache_table_remove_head(t_chunk* value);

#endif