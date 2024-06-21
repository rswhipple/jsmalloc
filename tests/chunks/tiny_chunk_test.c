#include "../../inc/tests.h"

void tiny_chunk_top_create_test(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_fpage* fpage = pagemap->span_head->fastpages;  fpage->chunk_size = 16;
  t_tiny_chunk* top_tiny_chunk = tiny_chunk_top_create(fpage);
  assert_int_equal(top_tiny_chunk->size, fpage->chunk_size);
  assert_null(top_tiny_chunk->next);
}

void tiny_chunk_create_test(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_fpage* fpage = pagemap->span_head->fastpages;
  fpage->chunk_size = 16;
  fpage->chunk_count = 0;
  t_tiny_chunk* tiny_chunk = tiny_chunk_create(fpage);

  assert_null(tiny_chunk->next);
  assert_int_equal(fpage->chunk_count, 1);
  assert_ptr_equal(fpage->last_chunk, tiny_chunk);
}

// TODO: waiting on completion of tiny_chunk_free?
void tiny_chunk_free_test(void** state) {
  UNUSED(state);
  // t_pagemap* pagemap = (t_pagemap*)*state;
  // t_fpage* fpage = pagemap->span_head->fastpages;
  // fpage->chunk_size = 16;
  // fpage->chunk_count = 0;
  // t_tiny_chunk* tiny_chunk = tiny_chunk_create(fpage);
  // tiny_chunk_free(tiny_chunk, tiny_chunk->size);
  // assert_null(tiny_chunk);
}