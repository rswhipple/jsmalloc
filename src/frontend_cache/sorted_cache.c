#include "../../inc/main.h"


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
    } else {
      // function to sort chunk
      const char* key = (const char*)size;  // TODO: make sure this works
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
  // TODO: determine how to get correct bin_size
  char key[32];
  snprintf(key, sizeof(key), "%zu", size);

  t_cache_table* cache_table = g_pagemap->frontend_cache->cache_table;

  void* ptr = NULL;

  if ((ptr = cache_table_get(cache_table, key)) != NULL) {
    return (void*)MEMORY_SHIFT(ptr, sizeof(t_chunk));
  }

  t_page* page_head = get_page_head(page_type);

  t_chunk* top_chunk = get_top_chunk(page_head);

  if ((cache_table_set(cache_table, key, top_chunk)) != NULL) {
    SET_IN_USE(top_chunk);
    return (void*)MEMORY_SHIFT(cache_table_get(cache_table, key), sizeof(t_chunk));
  }

  printf("Returning NULL\n");
  return NULL;
}