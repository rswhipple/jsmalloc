
#ifndef HASH_TABLE_H  
#define HASH_TABLE_H

#include "my_malloc.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <math.h>


// function prototypes
unsigned int my_hash_function(size_t data_size, uint32_t table_size);
t_hash *hash_table_create(t_heap *heap, uint32_t size, hash_function *hf) ;
void hash_table_destroy(t_hash *ht);
void hash_table_print(t_hash *ht);
// bool hash_table_insert(t_heap **heap, t_hash *ht, const char *key, size_t value);
void *hash_table_allocate(t_hash *ht, size_t key);
void *hash_table_deallocate(t_block *block);
// void *hash_table_deallocate(t_hash *ht, const char *key);

#endif