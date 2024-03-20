#include "../inc/hash_table.h" 
#include "../inc/heap.h" 
#include "../inc/my_malloc.h"

int main() {

    // For hash table testing
    const int tablesize = (1<<12);
    // initiate heap first
    t_heap *heap = NULL;
    int heap_size = 1000000;

    create_heap(&heap, heap_size);
    hash_table_create(heap, tablesize, my_hash_function);


    log_info("allocating");
    void *ptr1 = my_malloc(10);
    void *ptr5 = my_malloc(80);
    void *ptr2 = my_malloc(40);
    void *ptr3 = my_malloc(30);
    
    const void *ptr4 = my_malloc(900);
    UNUSED(ptr4);
    UNUSED(ptr5);
    t_heap *current_heap = global_heap;
    while (current_heap != NULL) {
        log_info("pointers allocated");
        printf("heap total: %zu\n", current_heap->total_size);
        printf("remaining size: %zu\n", current_heap->free_size);
        print_blocks(current_heap);
        current_heap = current_heap->next;
    }

    log_info("freeing");
    my_free(ptr1);
    my_free(ptr2);
    my_free(ptr3);
    return 0;
}