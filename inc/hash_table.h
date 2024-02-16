#ifndef HASH_TABLE_H  
#define HASH_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "my_malloc.h"

typedef uint64_t (hash_function)(const char *input, size_t);
typedef struct hash_table_s t_hash;

struct hash_table_s {
    uint32_t size;
    hash_function *hash;
    t_block **elements;
};

// function prototypes
unsigned int my_hash_function(int key, int table_size);
t_hash *hash_table_create(t_heap *heap, uint32_t size, hash_function *hf) ;
void hash_table_destroy(t_hash *ht);
void hash_table_print(t_hash *ht);
// bool hash_table_insert(t_heap **heap, t_hash *ht, const char *key, size_t value);
void *hash_table_allocate(t_hash *ht, const char *key);
void *hash_table_deallocate(t_block *block);
// void *hash_table_deallocate(t_hash *ht, const char *key);

#endif