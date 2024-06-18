#include "../../inc/tests.h"

void create_top_tiny_chunk_test(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_fpage* fpage = pagemap->span_head->fastpages;  fpage->chunk_size = 16;
  t_tiny_chunk* top_tiny_chunk = create_top_tiny_chunk(fpage);
  assert_int_equal(top_tiny_chunk->size, fpage->chunk_size);
  assert_null(top_tiny_chunk->next);
}

void create_tiny_chunk_test(void** state) {
  t_pagemap* pagemap = (t_pagemap*)*state;
  t_fpage* fpage = pagemap->span_head->fastpages;
  fpage->chunk_size = 16;
  fpage->chunk_count = 0;
  t_tiny_chunk* tiny_chunk = create_tiny_chunk(fpage);

  assert_null(tiny_chunk->next);
  assert_int_equal(fpage->chunk_count, 1);
  assert_ptr_equal(fpage->last_chunk, tiny_chunk);
}

// TODO: waiting on completion of free_tiny_chunk?
void free_tiny_chunk_test(void** state) {
  UNUSED(state);
  // t_pagemap* pagemap = (t_pagemap*)*state;
  // t_fpage* fpage = pagemap->span_head->fastpages;
  // fpage->chunk_size = 16;
  // fpage->chunk_count = 0;
  // t_tiny_chunk* tiny_chunk = create_tiny_chunk(fpage);
  // free_tiny_chunk(tiny_chunk, tiny_chunk->size);
  // assert_null(tiny_chunk);
}