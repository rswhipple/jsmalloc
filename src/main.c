#include "../inc/hash_table.h" 
#include "../inc/pagemap.h" 
#include "../inc/my_malloc.h"

int main() {

    // initiate memory span and add to pagemap
    t_pagemap *pagemap = NULL;
    create_pagemap(&pagemap);

    log_info("allocating");
    void *ptr1 = my_malloc(10);
    void *ptr5 = my_malloc(80);
    void *ptr2 = my_malloc(40);
    void *ptr3 = my_malloc(30);
    
    const void *ptr4 = my_malloc(900);
    UNUSED(ptr4);

    log_info("freeing");
    my_free(ptr1);
    my_free(ptr2);
    my_free(ptr3);
    my_free(ptr4);

    destroy_pageheap(pagemap);

    return 0;
}