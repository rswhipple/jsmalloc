#include "../inc/main.h"

size_t min_chunk_size = 0;
size_t pointer_size = 0;
t_heap* global_heap = NULL;

void log_info(const char* message) {
    printf("\n=====%s=====\n", message);
}

void log_span(t_span* span) {
    UNUSED(span);
    // while (current_heap != NULL) {
    //     log_info("pointers allocated");
    //     printf("heap total: %zu\n", current_heap->total_size);
    //     printf("remaining size: %zu\n", current_heap->free_size);
    //     print_blocks(current_heap);
    //     current_heap = current_heap->next;
    // }
}


void system_settings() {
    check_system_pointer();
    if (pointer_size == 4) {
        min_chunk_size = 16;
        log_info("Minimum chunk size is 8 bytes (4 free).");
    }
    else {
        min_chunk_size = 16;
        log_info("Minimum chunk size is 16 bytes (8 free).");
    }
}

void check_system_pointer() {
    pointer_size = sizeof(void*);
    if (pointer_size == 4) {
        log_info("System has 4-byte pointers.");
    }
    else if (pointer_size == 8) {
        log_info("System has 8-byte pointers.");
    }
    else {
        printf("Unexpected pointer size: %zu bytes\n", pointer_size);
        // add error exit
    }
}

size_t check_system_size_t() {
    size_t size_t_size = sizeof(size_t);

    if (size_t_size == 4) {
        log_info("System has 4-byte size_t.");
    }
    else if (size_t_size == 8) {
        log_info("System has 8-byte size_t.");
    }
    else {
        printf("Unexpected size_t size: %zu bytes\n", size_t_size);
        // add error exit
    }

    return size_t_size;
}
