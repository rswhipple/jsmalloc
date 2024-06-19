#include "../../inc/main.h"


t_cache_table* cache_table_create(t_cache* cache) {
  // log_info("creating true cache table");
  /* The next line shifts the starting point of the t_cache_table struct 
  beyond fastcache. */
  t_cache_table* table = (t_cache_table*)MEMORY_SHIFT(cache, 
      (cache->fcache_size * sizeof(t_tiny_chunk)));    
  // printf("cache_table pointer: %p\n", table);
  table->capacity = NUM_BINS;
  table->entries = (cache_table_entry*)MEMORY_SHIFT(table, sizeof(t_cache_table));
  // printf("cache_table->entries pointer: %p\n", table->entries);
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
static size_t cache_table_index(t_cache_table *ct, uint64_t hash) {
  return (size_t)(hash & (uint64_t)(ct->capacity - 1));
}

/* Removes first entry in linked list and update head. If linked list is empty, 
return NULL. */
t_chunk* cache_table_get(t_cache_table* ct, const char* key) {
  uint64_t hash = hash_key(key);
  size_t index = cache_table_index(ct, hash);

  if (!ct->entries[index].value) {
    return NULL;
  }

  t_chunk* value = ct->entries[index].value;
  ct->entries[index].value = ct->entries[index].value->fd;
  return value;
}

/* Adds t_chunk to the tail of the linked list. Returns key. */
static const char* cache_table_set_entry(t_cache_table* ct,
        const char* key, t_chunk* value) {
  uint64_t hash = hash_key(key);
  size_t index = cache_table_index(ct, hash);

  // Loop till we find an empty entry.
  while (ct->entries[index].key != NULL) {
    if (strcmp(key, ct->entries[index].key) == 0) {
      // Found key (it already exists), add value to head, update pointers.
      value->fd = ct->entries[index].value;
      ct->entries[index].value->bk = value;
      ct->entries[index].value = value;
      return ct->entries[index].key;
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
  return key;
}

const char* cache_table_set(t_cache_table* ct, const char* key, t_chunk* value) {
  assert(value != NULL);
  if (value == NULL) {
    return NULL;
  }

  return cache_table_set_entry(ct, key, value);
}
