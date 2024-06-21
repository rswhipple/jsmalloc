#include "../../inc/main.h"

/*
search_unsorted_cache() iterates through the unsorted linked list of t_chunks.
If the t_chunk is not large enough for allocation, it is sorted into the
cache_table (aka sorted_cache). If a suitable t_chunk is found, it is either
split or a void* to the data field is returned.
*/
void* search_unsorted_cache(size_t size) {
  t_chunk* unsorted_chunk = g_pagemap->frontend_cache->unsorted_cache;
  t_chunk* split_chunk = NULL;

  while (unsorted_chunk) {
    if (unsorted_chunk->size >= size) {
      if (unsorted_chunk->size > size + 72) {
        g_pagemap->frontend_cache->unsorted_cache = unsorted_chunk->fd;
        split_chunk = chunk_split(unsorted_chunk, size);
        return (void*)MEMORY_SHIFT(split_chunk, pointer_size);
      } else {
        g_pagemap->frontend_cache->unsorted_cache = unsorted_chunk->fd;
        return (void*)MEMORY_SHIFT(unsorted_chunk, pointer_size);
      }
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
If no t_chunk is found, chunk_split is called and creates a new t_chunk from
t_pagemap* g_pagemap->top_chunk.
*/
void* search_sorted_cache(size_t size, int page_type) {
  UNUSED(page_type);
  void* ptr = NULL;

  if ((ptr = cache_table_get(size)) != NULL) {
    return (void*)MEMORY_SHIFT(ptr, pointer_size);
  }

  /* NEXT VERSION TODO: 
      add last_chunk logic
      add error handling if we get near the end of the heap
  */
  if (g_pagemap->top_chunk->size > size + 72) {
    if ((ptr = chunk_split(g_pagemap->top_chunk, size)) != NULL) 
      return (void*)MEMORY_SHIFT(ptr, pointer_size);
    else custom_exit("search_sorted_cache(): first error creating chunk.\n");
  } 
  else if (g_pagemap->top_chunk->size > size) {
    ptr = (void*)MEMORY_SHIFT(g_pagemap->top_chunk, pointer_size);
    g_pagemap->top_chunk = NEXT_CHUNK(g_pagemap->top_chunk);
    return ptr;
  }
  else {
    g_pagemap->top_chunk = NEXT_CHUNK(g_pagemap->top_chunk);
    if ((ptr = chunk_split(g_pagemap->top_chunk, size)) != NULL) 
      return (void*)MEMORY_SHIFT(ptr, pointer_size);
    else custom_exit("search_sorted_cache(): second error creating chunk.\n");
  }

  printf("Returning NULL\n");
  return NULL;
}