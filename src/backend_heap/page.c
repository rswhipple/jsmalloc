#include "../../inc/main.h"


void pages_create(t_pagemap* pagemap, t_span* span) {
  int pages_left = SMALL_PAGE_ALLOCATION_SIZE;
  t_page* current = NULL;
  t_page* temp = NULL;
  span->page_head = page_base_create(pagemap, span);
  pages_left -= 1;
  current = span->page_head;

  while (pages_left > 0) {
    temp = current;
    current = page_create(current, small);
    temp->next = current;
    pages_left -= 1;
  }

  pages_left = LARGE_PAGE_ALLOCATION_SIZE;
  while (pages_left > 0) {
    temp = current;
    current = page_create(current, large);
    temp->next = current;
    pages_left -= 1;
  }
}

t_page* page_base_create(t_pagemap* pagemap, t_span* span) {
  t_cache* cache = pagemap->frontend_cache;
  t_page* page = (t_page*)SPAN_SHIFT(span);
  page->next = NULL;
  page->memory = (void*)PAGE_SHIFT(page);
  page->pagetype = small;

  if (span == pagemap->span_head) {
    page->memory_size = PAGE_SIZE - sizeof(t_pagemap) - sizeof(t_cache) -
      (cache->fcache_size * sizeof(t_tiny_chunk*)) -
      sizeof(t_cache_table) - (NUM_BINS * sizeof(cache_table_entry))- 
      sizeof(t_span) - sizeof(t_page);
  }
  else {
    page->memory_size = PAGE_SIZE - sizeof(t_span) - sizeof(t_page);
  }

  page->pagetype = small;
  chunk_top_create(page);

  return page;
}

t_page* page_create(t_page* prev_page, int pagetype) {
  t_page* page = (t_page*)PAGE_SHIFT(prev_page);
  page->memory = (void*)MEMORY_SHIFT(prev_page->memory, prev_page->memory_size);
  // prev_page->memory
  page->next = NULL;
  page->memory_size = PAGE_SIZE - sizeof(t_page);
  page->pagetype = pagetype;
  chunk_top_create(page);

  return page;
}