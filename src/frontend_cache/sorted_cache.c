#include "../../inc/main.h"

/*
search_unsorted_cache() iterates through the unsorted linked list of t_chunks.
If the t_chunk is not large enough for allocation, it is sorted into the 
cache_table (aka sorted_cache). If a suitable t_chunk is found, it is either
split or a void* to the data field is returned.
*/
void* search_unsorted_cache(size_t size) {
  t_chunk* unsorted_chunk = g_pagemap->frontend_cache->unsorted_cache;

  while (unsorted_chunk) {
    if (unsorted_chunk->size >= size) {
      if (unsorted_chunk->size > size + 72) {
        unsorted_chunk = split_chunk(unsorted_chunk, size);
      }
      g_pagemap->frontend_cache->unsorted_cache = unsorted_chunk->fd;
      return (void*)MEMORY_SHIFT(unsorted_chunk, CHUNK_OVERHEAD);
    }
    else {
      char key[32];
      snprintf(key, sizeof(key), "%zu", size);
      cache_table_set(g_pagemap->frontend_cache->cache_table, key, unsorted_chunk);
    }
    unsorted_chunk = unsorted_chunk->fd;
  }

  return NULL;
}

/*
search_sorted_cache() checks to see if there are any t_chunks in the 
corresponding cache_table bin by calling cache_table_get(). If the t_chunk is 
found a void* to the data field is returned.
If no t_chunk is found, split_chunk is called and creates a new t_chunk from 
t_pagemap* g_pagemap->top_chunk.
*/
void* search_sorted_cache(size_t size, int page_type) {
  UNUSED(page_type);
  char key[32];
  snprintf(key, sizeof(key), "%zu", size);
  t_cache_table* cache_table = g_pagemap->frontend_cache->cache_table;
  void* ptr = NULL;

  if ((ptr = cache_table_get(cache_table, key)) != NULL) {
    return (void*)MEMORY_SHIFT(ptr, sizeof(t_chunk));
  }

  // TODO: add last_chunk logic
  if ((ptr = split_chunk(g_pagemap->top_chunk, size)) != NULL) {
    printf("split top_chunk\n");
    return (void*)MEMORY_SHIFT(ptr, sizeof(t_chunk));
  }

  printf("Returning NULL\n");
  return NULL;
}