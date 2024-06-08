#include "../../inc/main.h"

#include <string.h>

typedef struct {
  const char* key;  // key is NULL if this slot is empty
  t_chunk* value;
} cache_table_entry;

struct cache_table {
  cache_table_entry* entries;
  size_t capacity;
  size_t length;
};

cache_table* cache_table_create(t_cache* cache) {
  // log_info("creating true cache table");

  cache_table* table = (cache_table*)MEMORY_SHIFT(cache, (cache->fcache_size * sizeof(t_tiny_chunk)));
  // printf("cache_table pointer: %p\n", table);
  table->length = 0;
  table->capacity = NUM_BINS;

  table->entries = (cache_table_entry*)MEMORY_SHIFT(cache, sizeof(cache_table) + (sizeof(cache_table_entry*) * NUM_BINS));
  // printf("cache_table->entries pointer: %p\n", table->entries);
  table->entries = (cache_table_entry*)mmap(NULL, table->capacity * sizeof(cache_table_entry),
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (table->entries == MAP_FAILED) {
    perror("mmap failed");
    munmap(table, sizeof(cache_table));
    return NULL;
  }
  return table;
}

void cache_table_destroy(cache_table* table) {
  for (size_t i = 0; i < table->capacity; i++) {
    munmap((void*)table->entries[i].key, strlen(table->entries[i].key));
  }

  munmap(table->entries, table->capacity * sizeof(cache_table_entry));
  munmap(table, sizeof(cache_table));
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

void* cache_table_get(cache_table* table, const char* key) {
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
        const char* key, t_chunk* value, size_t* plength) {
  // AND hash with capacity-1 to ensure it's within entries array.
  uint64_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

  // Loop till we find an empty entry.
  while (entries[index].key != NULL) {
    if (strcmp(key, entries[index].key) == 0) {
      // Found key (it already exists), update value.
      entries[index].value = value;
      return entries[index].key;
    }
    // Key wasn't in this slot, move to next (linear probing).
    index++;
    if (index >= capacity) {
      // At end of entries array, wrap around.
      index = 0;
    }
  }

  // Didn't find key, allocate+copy if needed, then insert it.
  if (plength != NULL) {
    key = strdup(key);
    if (key == NULL) {
      return NULL;
    }
    (*plength)++;
  }
  entries[index].key = (char*)key;
  entries[index].value = value;
  return key;
}

// Expand hash table to twice its current size. Return true on success,
// false if out of memory.
static bool cache_table_expand(cache_table* table) {
  // Allocate new entries array.
  size_t new_capacity = table->capacity * 2;
  if (new_capacity < table->capacity) {
    return false;  // overflow (capacity would be too big)
  }
  cache_table_entry* new_entries = (cache_table_entry*)mmap(NULL, new_capacity * sizeof(cache_table_entry),
                               PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (new_entries == NULL) {
    return false;
  }

  // Iterate entries, move all non-empty ones to new table's entries.
  for (size_t i = 0; i < table->capacity; i++) {
    cache_table_entry entry = table->entries[i];
    if (entry.key != NULL) {
      cache_table_set_entry(new_entries, new_capacity, entry.key,
                   entry.value, NULL);
    }
  }

  // Free old entries array and update this table's details.
  munmap(table->entries, table->capacity * sizeof(cache_table_entry));
  table->entries = new_entries;
  table->capacity = new_capacity;
  return true;
}

const char* cache_table_set(cache_table* table, const char* key, t_chunk* value) {
  assert(value != NULL);
  if (value == NULL) {
    return NULL;
  }

  // If length will exceed half of current capacity, expand it.
  if (table->length >= table->capacity / 2) {
    if (!cache_table_expand(table)) {
      return NULL;
    }
  }

  // Set entry and update length.
  return cache_table_set_entry(table->entries, table->capacity, key, value,
                      &table->length);
}

size_t cache_table_length(cache_table* table) {
  return table->length;
}

cache_tablei cache_table_iterator(cache_table* table) {
  cache_tablei it;
  it._table = table;
  it._index = 0;
  return it;
}

bool cache_table_next(cache_tablei* it) {
  // Loop till we've hit end of entries array.
  cache_table* table = it->_table;
  while (it->_index < table->capacity) {
    size_t i = it->_index;
    it->_index++;
    if (table->entries[i].key != NULL) {
      // Found next non-empty item, update iterator key and value.
      cache_table_entry entry = table->entries[i];
      it->key = entry.key;
      it->value = entry.value;
      return true;
    }
  }
  return false;
}