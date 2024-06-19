#include "../../inc/tests.h"

void create_top_chunk_test(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_page* page = pagemap->span_head->page_head;
  t_chunk* top_chunk = create_top_chunk(page);
  assert_int_equal(top_chunk->size, page->memory);
  assert_false(IS_IN_USE(top_chunk));
  assert_null(top_chunk->fd);
  assert_null(top_chunk->bk);
  assert_ptr_equal(page->top_chunk, top_chunk);
}

void split_chunk_test_success(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;

  t_chunk* split = split_chunk(pagemap->span_head->page_head->top_chunk, 100);
  assert_int_equal(CHUNK_SIZE(split), 100);
  assert_int_equal(CHUNK_SIZE(pagemap->span_head->page_head->top_chunk), CHUNK_SIZE(split));
  assert_int_equal(pagemap->span_head->page_head->top_chunk->fd, split->fd);
  assert_int_equal(split->fd, NULL);
  assert_int_equal(pagemap->span_head->page_head->top_chunk->bk, NULL);
  assert_int_equal(split, pagemap->span_head->page_head->top_chunk);
}

void split_chunk_test_failure(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_chunk* top_chunk = pagemap->span_head->page_head->top_chunk;
  size_t size = CHUNK_SIZE(top_chunk);
  t_chunk* split = split_chunk(top_chunk, size + 1);
  assert_null(split);
}


void allocate_huge_chunk_test_success(void** state) {
  UNUSED(state);

  size_t size = 4096;
  t_chunk* huge_chunk = allocate_huge_chunk(size);
  assert_int_equal(huge_chunk->size, size);
}

void allocate_huge_chunk_test_failure(void** state) {
  UNUSED(state);

  size_t size = 0;
  t_chunk* huge_chunk = allocate_huge_chunk(size);
  assert_null(huge_chunk);
}

// TODO: Implement this test
void free_chunk_test(void** state) {
  UNUSED(state);

  //   t_pagemap* pagemap = (t_pagemap*)*state;
  //   t_chunk* chunk = pagemap->span_head->page_head->top_chunk;
  //   size_t size = CHUNK_SIZE(chunk);
  //   free_chunk(chunk, size);
}

void free_huge_chunk_test(void** state) {
  UNUSED(state);
  size_t size = 4096;
  t_chunk* huge_chunk = allocate_huge_chunk(size);
  free_huge_chunk(huge_chunk, size);
}
