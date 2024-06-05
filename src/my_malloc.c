#include "../../inc/main.h" 

void* my_malloc(size_t size) {
    if (g_pagemap == NULL) {
        create_pagemap(&g_pagemap);
    }
    

    printf("returning NULL\n");
    // return NULL;
    return NULL;
};



