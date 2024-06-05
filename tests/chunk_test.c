#include "../inc/tests.h"

void chunk_test_success(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;

  t_chunk* split = split_chunk(pagemap->span_head->page_head->top_chunk, 100);
  assert_int_equal(pagemap->span_head->page_head->top_chunk->size, 100);
  printf("split->size: %zu\n", split->size);
  printf("split->fd->size: %zu\n", split->fd->size);
  assert_int_equal(pagemap->span_head->page_head->top_chunk->fd, split->fd);
  assert_int_equal(split->fd->fd, NULL);
  assert_int_equal(pagemap->span_head->page_head->top_chunk->bk, NULL);
  assert_int_equal(split, pagemap->span_head->page_head->top_chunk);
}