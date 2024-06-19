#include "../inc/main.h" 

/*
Upon successful completion with both nitems and itsize non-zero,
calloc() shall return a pointer to the allocated space. If either
nitem or itsize is 0, then:

    *  A null pointer shall be returned and errno may be set to an
        implementation-defined value
*/

void* my_calloc(size_t nitems, size_t itsize) {
    if (nitems == 0 || itsize == 0) {
        printf("returning NULL\n");
        return NULL;
    }

    size_t size = nitems * itsize;
    void* ptr = my_malloc(size);
    if (ptr) return ptr;

    return NULL;
};