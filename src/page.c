#include "../inc/main.h"

void create_pages(t_pagemap* pagemap, t_span* span) {
  // TODO: create and assign pages to types (fast, small, large)
  int pages_left = span->num_pages;
  t_page* current = NULL;
  span->page_head = create_base_page(pagemap, span);
  pages_left -= 1;
  current = span->page_head;
  while (pages_left > 0) {
    // log_info("creating page");
    // printf("pages_left: %d\n", pages_left);
    current = create_page(current, span, fast);
    pages_left -= 1;
  }
}

t_page* create_base_page(t_pagemap* pagemap, t_span* span) {
  UNUSED(pagemap);
  t_page* page = (t_page*)SPAN_SHIFT(span);
  page->chunk_count = 1;
  page->prev = NULL;
  page->next = NULL;
  page->memory = PAGE_SIZE - sizeof(t_span) - sizeof(t_page);
  page->pagetype = small;
  create_top_chunk(page);
  return page;
}

t_page* create_page(t_page* prev_page, t_span* span, int pagetype) {
  UNUSED(span);
  t_page* page = (t_page*)MEMORY_SHIFT(PAGE_SHIFT(prev_page), prev_page->memory);
  // log_info("creating page");
  // printf("prev_page: %p\n", prev_page);
  // printf("page: %p\n", page);
  page->chunk_count = 1;
  page->prev = prev_page;
  page->next = NULL;
  page->memory = PAGE_SIZE - sizeof(t_page);
  page->pagetype = pagetype;
  // if (pagetype == fast) {
  //     // log_info("creating top tiny chunk");
  //     // printf("page: %p\n", page);
  //     // TODO: can we use create_top_tiny_chunk here?
  //     create_top_tiny_chunk((t_fpage*)page);
  // }
  // else {
  create_top_chunk(page); // TODO figure out logic
  // }
  return page;
}