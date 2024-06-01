#include "../inc/tests.h"

void chunk_test_success(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;

  t_chunk* split = split_chunk(pagemap->span_head->page_head->top_chunk, 100);
  assert_int_equal(pagemap->span_head->page_head->top_chunk->size, 100);
  printf("split->size: %zu\n", split->size);
  assert_int_equal((int*)split->next->size, 3836);
  assert_int_equal(pagemap->span_head->page_head->top_chunk->next, split->next);
  assert_int_equal(split->next->next, NULL);
  assert_int_equal(pagemap->span_head->page_head->top_chunk->prev, NULL);
  assert_int_equal(split, pagemap->span_head->page_head->top_chunk);
}