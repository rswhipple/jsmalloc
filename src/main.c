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

    // For my_malloc testing
    printf("\n=====mallocing for small size heap=====\n");
    const void *ptr1 = my_malloc(10);
    const void *ptr5 = my_malloc(80);
    const void *ptr2 = my_malloc(40);
    const void *ptr3 = my_malloc(30);

    printf("\n=====mallocing medium?=====\n");
    // For testing big batches
    // for (size_t i = 0; i < 100; i++)
    // {
    //      my_malloc(900);
    //     // /* code */
    // }
    
    const void *ptr4 = my_malloc(900);

    printf("\n=====mallocing big size heap=====\n");
    printf("None yet\n");

    printf("\n======pointers=====\n");
    printf("pointer to malloced node: %p\n", ptr1);
    printf("pointer to malloced node: %p\n", ptr2);
    printf("pointer to malloced node: %p\n", ptr3);
    printf("pointer to malloced node: %p\n", ptr4);
    printf("pointer to malloced node: %p\n", ptr5);

    printf("\n=====pointers allocated=====\n");
    t_heap *current_heap = global_heap;
    while (current_heap != NULL) {
        printf("heap total: %zu\n", current_heap->total_size);
        printf("remaining size: %zu\n", current_heap->free_size);
        print_blocks(current_heap);
        current_heap = current_heap->next;
    }

    // my_free(ptr1);
    return 0;
}