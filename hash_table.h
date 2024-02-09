#ifndef HASH_TABLE_H  
#define HASH_TABLE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint64_t (hash_function)(const char *input, size_t);
typedef struct hash_table_s hash_table;
typedef struct entry entry;

// function prototypes
hash_table *hash_table_create(uint32_t size, hash_function *hf);
void hash_table_destroy(hash_table *ht);
void hash_table_print(hash_table *ht);
bool hash_table_insert(hash_table *ht, const char *key, void *value);
void *hash_table_allocate(hash_table *ht, const char *key);
void *hash_table_deallocate(hash_table *ht, const char *key);

#endif