#include <stdio.h>
#include "my_malloc.h"

t_heap *global_heap = NULL;

void *my_malloc(size_t size) {
    return NULL;
};

void my_free(size_t nitems, size_t size) {
};

void *my_realloc(void *ptr, size_t size) {
    return NULL;
};

void *calloc(size_t nitems, size_t size) {
    return NULL;
};

int main() {
    // char *ptr = my_malloc(10);

    // printf("pointer to malloced node: %s\n", ptr);
    return 0;
}