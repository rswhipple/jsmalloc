#include "../../inc/main.h"

#include <string.h>

t_cache_table* cache_table_create(t_cache* cache) {
  // log_info("creating true cache table");

  t_cache_table* table = (t_cache_table*)MEMORY_SHIFT(cache, (cache->fcache_size * sizeof(t_tiny_chunk)));
  // printf("cache_table pointer: %p\n", table);
  table->capacity = NUM_BINS;

  table->entries = (cache_table_entry*)mmap(NULL, table->capacity * sizeof(cache_table_entry),
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (table->entries == MAP_FAILED) {
    perror("mmap failed");
    munmap(table, sizeof(t_cache_table));
    return NULL;
  }
  // printf("cache_table->entries pointer: %p\n", table->entries);
  return table;
}

void cache_table_destroy(t_cache_table* table) {
  munmap(table->entries, table->capacity * sizeof(cache_table_entry));
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

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

void* cache_table_get(t_cache_table* table, const char* key) {
  // TODO: make sure cache_table() removes t_chunk from list when it returns
  // AND hash with capacity-1 to ensure it's within entries array.
  uint64_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

  // Loop till we find an empty entry.
  while (table->entries[index].key != NULL) {
    if (strcmp(key, table->entries[index].key) == 0) {
      // Found key, return value.
      return table->entries[index].value;
    }
    // Key wasn't in this slot, move to next (linear probing).
    index++;
    if (index >= table->capacity) {
      // At end of entries array, wrap around.
      index = 0;
    }
  }
  return NULL;
}

// Internal function to set an entry (without expanding table).
static const char* cache_table_set_entry(cache_table_entry* entries, size_t capacity,
        const char* key, t_chunk* value) {
  // TODO: how is the list sorted? We may have to implement old hash format
  // AND hash with capacity-1 to ensure it's within entries array.
  uint64_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

  // Loop till we find an empty entry.
  while (entries[index].key != NULL) {
    if (strcmp(key, entries[index].key) == 0) {
      entries[index].value = value;
      return entries[index].key;
    }
    index++;
    if (index >= capacity) {
      index = 0;
    }
  }

  key = strdup(key);
  if (key == NULL) {
    return NULL;
  }
  entries[index].key = (char*)key;
  entries[index].value = value;
  return key;
}

const char* cache_table_set(t_cache_table* table, const char* key, t_chunk* value) {
  assert(value != NULL);
  if (value == NULL) {
    return NULL;
  }

  return cache_table_set_entry(table->entries, table->capacity, key, value);
}
