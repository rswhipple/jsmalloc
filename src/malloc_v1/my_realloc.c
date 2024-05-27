#include "../../inc/main.h" 

void* my_realloc(void* ptr, size_t size) {
    // to compile the code with unused params, we need to use the UNUSED macro
    // UNUSED(ptr);
    // UNUSED(size);

    if (ptr == NULL && size != 0) {
        return my_malloc(size);
    }

    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    void* new_ptr = my_malloc(size);

    if (new_ptr != NULL) {
        my_free(ptr);
        return new_ptr;
    }

    return NULL;
};
