#include "../inc/main.h" 

void* my_realloc(void* ptr, size_t size) {
    if (ptr == NULL && size != 0) {
        return my_malloc(size);
    }

    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    void* new_ptr = my_malloc(size);

    if (new_ptr != NULL) {
        // copy data from ptr to new_ptr
        // TODO: make sure cast works
        my_strcpy((char*)new_ptr, (char*)ptr);
        my_free(ptr);
        return new_ptr;
    }

    return NULL;
};
