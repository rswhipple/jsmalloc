#include "../../inc/main.h"

int get_fpage_index(size_t nbr) {
    int num_pages = g_pagemap->frontend_cache->fcache_size;
    int i;
    size_t list[] = { 8, 16, 24, 32, 40, 48, 56, 64 };
    int list_len = 8;

    // Iterate through the list
    for (i = 0; i < list_len; i++) {
        // If the current list element is greater than or equal to the number
        if (list[i] >= nbr) {
            break;
        }
    }

    // TODO: If no larger or equal number is found, throw error

    // Logic if min_chunk_size is 16
    if (num_pages == 7 && i > 0) return i - 1;
    else return i;
}

void create_fpages(t_pagemap* pagemap) {
  int count = 0;
  pagemap->span_head->fastpages = create_base_fpage(pagemap);
  count += 1;

  t_cache* cache = pagemap->frontend_cache;
  t_fpage* current = pagemap->span_head->fastpages;
  t_fpage* temp;
  int chunk_size = min_chunk_size;
  if (chunk_size == 8) chunk_size += 8;

  while (count < FAST_PAGE_ALLOCATION_SIZE && chunk_size <= 64) {
    // log_info("creating fpage");
    // printf("page_count: %d\n", count);
    // printf("chunk_size: %d\n", chunk_size);
    temp = create_fpage(current, count, chunk_size, cache);
    current->next = temp;
    current = temp;
    chunk_size += 8;
    count += 1;
  }
}

t_fpage* create_base_fpage(t_pagemap* pagemap) {
  // log_info("creating base fpage");
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

  // printf("fpage pointer: %p\n", fpage);
  // printf("sizeof(t_fpage): %zu\n", sizeof(t_fpage));
  // printf("available memory: %zu\n", fpage->memory);
  // printf("chunk size: %zu\n", fpage->chunk_size);
  // printf("maximum number of chunks: %zu\n", fpage->max_chunks);
  // void* last_byte = (void*)MEMORY_SHIFT(fpage, fpage->memory + sizeof(t_fpage));
  // printf("fpage end = %p\n", last_byte);

  fpage->last_chunk = create_top_tiny_chunk(fpage);
  print_tiny_chunk(fpage->last_chunk);
  /* The tiny_chunk is immediately added to the fast_cache. */
  pagemap->frontend_cache->fast_cache[0] = fpage->last_chunk;

  return fpage;
}

t_fpage* create_fpage(t_fpage* prev_page, int count,
        size_t chunk_size, t_cache* cache) {
  // log_info("in create fpage");
  t_fpage* fpage = (t_fpage*)MEMORY_SHIFT(FASTPAGE_SHIFT(prev_page), prev_page->memory);
  fpage->chunk_count = 1;
  fpage->next = NULL;
  fpage->memory = PAGE_SIZE - sizeof(t_fpage);
  fpage->chunk_size = chunk_size;
  fpage->max_chunks = fpage->memory / fpage->chunk_size;
  // printf("fpage pointer: %p\n", fpage);
  // printf("chunk size: %zu\n", fpage->chunk_size);
  // printf("maximum number of chunks: %zu\n", fpage->max_chunks);
  // void* last_byte = (void*)MEMORY_SHIFT(fpage, fpage->memory + sizeof(t_fpage));
  // printf("fpage end = %p\n", last_byte);
  fpage->last_chunk = create_top_tiny_chunk(fpage);
  print_tiny_chunk(fpage->last_chunk);

  /* The tiny_chunk is immediately added to the fast_cache.
  Must take into account 2 pages for size 16 bytes when the
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