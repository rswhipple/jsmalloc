#include "../inc/main.h"

void create_pages(t_pagemap* pagemap, t_span* span) {
  int pages_left = SMALL_HEAP_ALLOCATION_SIZE;
  t_page* current = NULL;
  span->page_head = create_base_page(pagemap, span);
  pages_left -= 1;
  current = span->page_head;

  while (pages_left > 0) {
    current = create_page(current, span, small);
    pages_left -= 1;
  }

  pages_left = LARGE_HEAP_ALLOCATION_SIZE;
  while (pages_left > 0) {
    current = create_page(current, span, large);
    pages_left -= 1;
  }

}

t_page* create_base_page(t_pagemap* pagemap, t_span* span) {
  UNUSED(pagemap);
  t_page* page = (t_page*)SPAN_SHIFT(span);
  page->chunk_count = 1;
  page->prev = NULL;
  page->next = NULL;
  if (span == pagemap->span_head) {
    // available memory in base_page must account for t_pagemap, t_cache, t_span and t_page space
    page->memory = PAGE_SIZE - sizeof(t_pagemap) - sizeof(t_cache) -
        sizeof(t_span) - sizeof(t_fpage);
  }
  else {
    page->memory = PAGE_SIZE - sizeof(t_span) - sizeof(t_fpage);
  }
  page->pagetype = small;
  log_info("creating base page");
  printf("page pointer: %p\n", page);
  // printf("page->memory: %zu\n", page->memory);
  create_top_chunk(page);
  return page;
}

t_page* create_page(t_page* prev_page, t_span* span, int pagetype) {
  UNUSED(span);
  log_info("in create page");
  printf("prev_page: %p\n", prev_page);
  printf("prev_page->memory: %zu\n", prev_page->memory);
  printf("prev_page->pagetype = %zu\n", prev_page->pagetype);
  printf("sizeof(t_page): %zu\n", sizeof(t_fpage));
  t_page* page = (t_page*)MEMORY_SHIFT(PAGE_SHIFT(prev_page), prev_page->memory);
  page->chunk_count = 1;
  page->prev = prev_page;
  page->next = NULL;
  page->memory = PAGE_SIZE - sizeof(t_page);
  page->pagetype = pagetype;
  create_top_chunk(page); // TODO figure out logic
  return page;
}