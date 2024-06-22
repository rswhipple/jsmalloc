#include "../../inc/main.h"


t_cache_table* cache_table_create(t_cache* cache) {
  /* Shift starting point of t_cache_table struct beyond fastcache. */
  t_cache_table* table = (t_cache_table*)MEMORY_SHIFT(cache, 
                        (cache->fcache_size * sizeof(t_tiny_chunk)));
  table->capacity = NUM_BINS;
  table->entries = (cache_table_entry*)MEMORY_SHIFT(table, 
                  sizeof(t_cache_table));

  return table;
}

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// cache_tabletps://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char* key) {
  uint64_t hash = FNV_OFFSET;
  for (const char* p = key; *p; p++) {
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
  }
  return hash;
}

/* AND hash with capacity-1 to ensure it's within entries array. */
static size_t cache_table_index(t_cache_table *ct, size_t size) {
  char key[32];
  snprintf(key, sizeof(key), "%zu", size);
  uint64_t hash = hash_key(key);
  return (size_t)(hash & (uint64_t)(ct->capacity - 1));
}

/* Get first entry in bin, update head. If empty, return NULL. */
t_chunk* cache_table_get(size_t size) {
  t_cache_table* ct = g_pagemap->frontend_cache->cache_table;
  size_t index = cache_table_index(ct, size);

  if (!ct->entries[index].value) {
    return NULL;
  }

  t_chunk* value = ct->entries[index].value;
  ct->entries[index].value = ct->entries[index].value->fd;
  return value;
}

/* Add t_chunk to tail of bin linked list. */
static int cache_table_set_entry(t_chunk* value) {
  t_cache_table* ct = g_pagemap->frontend_cache->cache_table;
  char key[32];
  snprintf(key, sizeof(key), "%zu", value->size);
  size_t index = cache_table_index(ct, value->size);

  // Loop till we find an empty entry.
  while (ct->entries[index].key != NULL) {
    if (strcmp(key, ct->entries[index].key) == 0) {
      // Found key (it already exists), add value to head, update pointers.
      value->fd = ct->entries[index].value;
      ct->entries[index].value->bk = value;
      ct->entries[index].value = value;
      return EXIT_SUCCESS;
    }
    // Key wasn't in this slot, move to next (linear probing).
    index++;
    if (index >= ct->capacity) {
      // At end of entries array, wrap around.
      index = 0;
    }
  }

  // Didn't find key, insert new key and value.
  ct->entries[index].key = (char*)key;
  ct->entries[index].value = value;
  return EXIT_SUCCESS;
}

int cache_table_set(t_chunk* value) {
  assert(value != NULL);
  if (value == NULL) {
    return EXIT_FAILURE;
  }

  return cache_table_set_entry(value);
}

bool cache_table_is_bin_head(t_chunk* value) {
  t_cache_table* ct = g_pagemap->frontend_cache->cache_table;
  size_t index = cache_table_index(ct, value->size);

  return ct->entries[index].value == value;
}

int cache_table_remove_head(t_chunk* value) {
  t_cache_table* ct = g_pagemap->frontend_cache->cache_table;
  size_t index = cache_table_index(ct, value->size);

  ct->entries[index].value = value->fd;

  return EXIT_SUCCESS;
}
