#include "../../inc/main.h"

/*
search_unsorted_cache() iterates through the unsorted linked list of t_chunks.
If the t_chunk is not large enough for allocation, it is sorted into the 
cache_table (aka sorted_cache). When a suitable t_chunk is found, the 
*/
void* search_unsorted_cache(size_t size) {
  // t_chunk* chunk = NULL;
  size = size + CHUNK_OVERHEAD;
  t_chunk* unsorted_chunk = g_pagemap->frontend_cache->unsorted_cache;
  while (unsorted_chunk) {
    printf("unsorted_cache->size: %zu\n", unsorted_chunk->size);
    if (unsorted_chunk->size >= size) {
      if (unsorted_chunk->size > size + 72) {
        unsorted_chunk = split_chunk(unsorted_chunk, size);
      }
      g_pagemap->frontend_cache->unsorted_cache = unsorted_chunk->fd;
      return (void*)MEMORY_SHIFT(unsorted_chunk, CHUNK_OVERHEAD);
    }
    else {
      // sort chunk if not correct size
      char key[32];
      snprintf(key, sizeof(key), "%zu", size);

      cache_table_set(g_pagemap->frontend_cache->cache_table, key, unsorted_chunk);
    }
    unsorted_chunk = unsorted_chunk->fd;
  }

  return NULL;
}

t_page* get_page_head(int page_type) {
  t_page* page_head = g_pagemap->span_head->page_head;
  if (page_type == 3) {
    while ((int)page_head->pagetype != page_type) {
      page_head = page_head->next;
    }
    page_head = g_pagemap->span_head->page_head;
  }
  return page_head;
}

t_chunk* get_top_chunk(t_page* page) {
  t_chunk* top_chunk = page->top_chunk;
  while (IS_IN_USE(top_chunk)) {
    top_chunk = top_chunk->fd;
  }

  if (top_chunk == NULL) {
    t_chunk* new_chunk = create_top_chunk(page);
    top_chunk->fd = new_chunk;
    new_chunk->bk = top_chunk;
    top_chunk = new_chunk;
  }

  return top_chunk;
}

void* search_sorted_cache(size_t size, int page_type) {
  size = size + CHUNK_OVERHEAD;
  char key[32];
  snprintf(key, sizeof(key), "%zu", size);

  t_cache_table* cache_table = g_pagemap->frontend_cache->cache_table;

  void* ptr = NULL;

  if ((ptr = cache_table_get(cache_table, key)) != NULL) {
    return (void*)MEMORY_SHIFT(ptr, sizeof(t_chunk));
  }

  t_page* page_head = get_page_head(page_type);

  t_chunk* top_chunk = get_top_chunk(page_head);

  if ((ptr = split_chunk(top_chunk, size)) != NULL) {
    printf("split top_chunk\n");
    return (void*)MEMORY_SHIFT(ptr, sizeof(t_chunk));
  }

  printf("Returning NULL\n");
  return NULL;
}