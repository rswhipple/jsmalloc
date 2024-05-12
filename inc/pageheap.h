#ifndef PAGEHEAP_H
#define PAGEHEAP_H

#include "my_malloc.h"

typedef struct s_pagemap t_pagemap;
typedef struct s_span t_span;
typedef struct s_page t_page;

struct s_pagemap {
    t_span* span_head;
    size_t page_size;
    size_t total_pages;
};

struct s_span {
    t_span* next;
    t_page* page_head;
    // variable for the ID of the span
    size_t num_pages;
};

struct s_page {
    t_page* next;
    t_page* prev;
    // variable for the ID of the page
    int block_count;
};

#endif