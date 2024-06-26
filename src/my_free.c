#include "../inc/main.h" 


void my_free(void* ptr) {
    if (g_pagemap == NULL || ptr == NULL) {
        printf("Attempting to free NULL pointer or heap not initialized.\n");
        return;
    }

    // get chunk category
    size_t size = (size_t)((char*)ptr - sizeof(size_t));
    int page_type = get_page_type(size);

    switch (page_type) {
    case 1: tiny_chunk_free(ptr, size);
            break;
    case 2:
    case 3: chunk_free(ptr, size);
            break;
    case 4: huge_chunk_free(ptr, size);
            break;
    default: printf("Error reading memory size: Can not free.\n");
            break;
    }

}