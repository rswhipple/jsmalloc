#include "../inc/main.h"

t_pagemap* g_pagemap = NULL;

#ifndef TESTING
int main() {
    system_settings();
    create_pagemap(&g_pagemap);


    void* ptr = my_malloc(100);
    printf("ptr: %p\n", ptr);
    destroy_pagemap(g_pagemap);

    return 0;
}
#endif