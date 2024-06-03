
#ifndef CACHE_TABLE_H  
#define CACHE_TABLE_H

#include "types.h"

// function prototypes
unsigned int my_hash_function(size_t data_size, uint32_t table_size);
t_cache_table *create_cache_table(t_heap *heap, uint32_t size, hash_function *hf) ;
void cache_table_destroy(t_cache_table* ht);
void cache_table_print(t_cache_table* ht);
// bool cache_table_insert(t_heap **heap, t_cache_table *ht, const char *key, size_t value);
void* cache_table_allocate(t_cache_table* ht, size_t key);
// void *cache_table_deallocate(t_block *block);
// void *cache_table_deallocate(t_cache_table *ht, const char *key);

#endif