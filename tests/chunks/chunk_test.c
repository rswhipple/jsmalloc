#include "../../inc/tests.h"


void chunk_top_create_test(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_page* page = pagemap->span_head->page_head;
  t_chunk* top_chunk = chunk_top_create(page);
  assert_int_equal(top_chunk->size, page->memory);
  assert_false(IS_IN_USE(top_chunk));
  assert_null(top_chunk->fd);
  assert_null(top_chunk->bk);
  assert_ptr_equal(page->base_chunk, top_chunk);
}

void chunk_split_test_success(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;

  size_t new_top_size = pagemap->top_chunk->size - 100;
  t_chunk* split = chunk_split(pagemap->top_chunk, 100);
  assert_int_equal(CHUNK_SIZE(split), 100);
  assert_int_equal(pagemap->top_chunk->fd, split->fd);
  assert_int_equal(split->fd, NULL);
  assert_int_equal(split->bk, NULL);
  assert_int_equal(new_top_size, pagemap->top_chunk->size);
}


void huge_chunk_allocate_test_success(void** state) {
  UNUSED(state);

  size_t size = 4096;
  t_chunk* huge_chunk = huge_chunk_allocate(size);
  assert_int_equal(huge_chunk->size, size);
}

void huge_chunk_allocate_test_failure(void** state) {
  UNUSED(state);

  size_t size = 0;
  t_chunk* huge_chunk = huge_chunk_allocate(size);
  assert_null(huge_chunk);
}

// TODO: Implement this test
void chunk_free_test(void** state) {
  UNUSED(state);

  // t_pagemap* pagemap = (t_pagemap*)*state;
  // t_chunk* chunk = pagemap->top_chunk;
  // size_t size = CHUNK_SIZE(chunk);
  // chunk_free(chunk, size);
}

void huge_chunk_free_test(void** state) {
  UNUSED(state);
  size_t size = 4096;
  t_chunk* huge_chunk = huge_chunk_allocate(size);
  huge_chunk_free(huge_chunk, size);
}


void chunk_merge_test(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_chunk* top_chunk = pagemap->top_chunk;
  t_chunk* chunk1 = chunk_split(top_chunk, 72);
  top_chunk = pagemap->top_chunk;
  t_chunk* chunk2 = chunk_split(top_chunk, 100);
  SET_FREE(chunk1);
  SET_FREE(chunk2);
  size_t merge_size = chunk1->size + chunk2->size;
  t_chunk* merged_chunk = chunk_merge(chunk1, chunk2);

  assert_int_equal(merged_chunk->size, merge_size);
  assert_int_equal(merged_chunk->size, chunk1->size);
  assert_null(merged_chunk->fd);
  assert_null(merged_chunk->bk);
  assert_false(cache_table_is_bin_head(chunk1));
  assert_false(cache_table_is_bin_head(chunk2));
  assert_null(chunk1->bk);
  assert_null(chunk1->fd);
  assert_null(chunk2->bk);
  assert_null(chunk2->fd);
}

void try_merge_is_in_use_test(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_chunk* top_chunk = pagemap->top_chunk;
  t_chunk* chunk1 = chunk_split(top_chunk, 72);
  top_chunk = pagemap->top_chunk;
  t_chunk* chunk2 = chunk_split(top_chunk, 100);
  top_chunk = pagemap->top_chunk;
  t_chunk* chunk3 = chunk_split(top_chunk, 72);
  top_chunk = pagemap->top_chunk;
  t_chunk* chunk4 = chunk_split(top_chunk, 72);
  SET_FREE(chunk1);
  SET_FREE(chunk2);
  SET_FREE(chunk3);
  SET_FREE(chunk4);

  t_chunk* merged_chunk = try_merge(chunk3);
  assert_ptr_equal(merged_chunk, chunk1);
}
