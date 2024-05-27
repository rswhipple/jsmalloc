
#include "../../inc/main.h" 

void create_heap(t_heap** heap, size_t size) {
  *heap = (t_heap*)mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  // (*heap)->prev = NULL;
  (*heap)->next = NULL;
  (*heap)->total_size = size;
  (*heap)->free_size = size;
  (*heap)->block_count = 0;
  (*heap)->ht = NULL;
}

void destroy_heap(t_heap* heap) {
  munmap(heap, heap->total_size);
}