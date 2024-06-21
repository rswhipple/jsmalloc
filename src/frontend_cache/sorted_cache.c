#include "../../inc/main.h"

/*
search_unsorted_cache() iterates through the unsorted linked list of t_chunks.
If the t_chunk is not large enough for allocation, it is sorted into the
cache_table (aka sorted_cache). If a suitable t_chunk is found, it is either
split or a void* to the data field is returned.
*/
void* search_unsorted_cache(size_t size) {
  t_chunk* unsorted_chunk = g_pagemap->frontend_cache->unsorted_cache;
  t_chunk* new = NULL;

  while (unsorted_chunk) {
    if (unsorted_chunk->size >= size) {
      // printf("found unsorted_chunk %p, size: %zu\n", unsorted_chunk, unsorted_chunk->size);
      if (unsorted_chunk->size > size + 72) {
        g_pagemap->frontend_cache->unsorted_cache = unsorted_chunk->fd;
        new = split_chunk(unsorted_chunk, size);
      }
      // printf("here\n");
      return (void*)MEMORY_SHIFT(new, CHUNK_OVERHEAD);
    }
    else {
      cache_table_set(unsorted_chunk);
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
  void* ptr = NULL;

  if ((ptr = cache_table_get(size)) != NULL) {
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