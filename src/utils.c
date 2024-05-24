#include "../inc/main.h"

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