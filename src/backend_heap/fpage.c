#include "../../inc/main.h"


void fpages_create(t_pagemap* pagemap) {
  int count = 0;
  pagemap->span_head->fastpages = fpage_base_create(pagemap);
  count += 1;

  t_cache* cache = pagemap->frontend_cache;
  t_fpage* current = pagemap->span_head->fastpages;
  t_fpage* temp;
  int chunk_size = min_chunk_size;
  if (chunk_size == 8) chunk_size += 8;

  while (count < FAST_PAGE_ALLOCATION_SIZE && chunk_size <= 64) {
    temp = fpage_create(current, count, chunk_size, cache);
    current->next = temp;
    current = temp;
    chunk_size += 8;
    count += 1;
  }
}

t_fpage* fpage_base_create(t_pagemap* pagemap) {
  /* Beginning pointer for fastpages is found by starting at the start of
  the pageheap and using MEMORY_SHIFT to add the standard pages total
  allocation size. */
  size_t pages_area = PAGE_SIZE *
    (SMALL_PAGE_ALLOCATION_SIZE + LARGE_PAGE_ALLOCATION_SIZE);

  t_fpage* fpage = (t_fpage*)MEMORY_SHIFT(pagemap, pages_area);
  fpage->chunk_count = 1;
  fpage->next = NULL;
  fpage->memory = PAGE_SIZE - sizeof(t_fpage);
  fpage->chunk_size = min_chunk_size;
  fpage->max_chunks = fpage->memory / fpage->chunk_size;
  fpage->last_chunk = tiny_chunk_top_create(fpage);

  /* The tiny_chunk is immediately added to the fast_cache. */
  pagemap->frontend_cache->fast_cache[0] = fpage->last_chunk;

  return fpage;
}

t_fpage* fpage_create(t_fpage* prev_page, int count,
        size_t chunk_size, t_cache* cache) {
  t_fpage* fpage = (t_fpage*)MEMORY_SHIFT(FASTPAGE_SHIFT(prev_page), prev_page->memory);
  fpage->chunk_count = 1;
  fpage->next = NULL;
  fpage->memory = PAGE_SIZE - sizeof(t_fpage);
  fpage->chunk_size = chunk_size;
  fpage->max_chunks = fpage->memory / fpage->chunk_size;
  fpage->last_chunk = tiny_chunk_top_create(fpage);

  /* The tiny_chunk is immediately added to the fast_cache.
  Logic takes into account 2 pages for size 16 bytes when the
  min_chunk_size is 16. */
  if (min_chunk_size != 8) { count -= 1; }

  if (min_chunk_size != 8 && count == 0) {
    fpage->last_chunk->next = cache->fast_cache[0];
    cache->fast_cache[0] = fpage->last_chunk;
  }
  else {
    cache->fast_cache[count] = fpage->last_chunk;
  }
  
  return fpage;
}