
#include "../../inc/main.h"


/*
create_pagemap(t_pagemap** pagemap):
Uses a double pointer to update the global t_pagemap* g_pagemap variable.
The system call mmap() is used to retrieve memory for the dynamic heap.
The frontend_cache field is initialized with a pointer to a t_cache struct
created by create_frontend_cache().
*/
void create_pagemap(t_pagemap** pagemap) {
    *pagemap = (t_pagemap*)mmap(0, BASE_HEAP_SIZE, PROT_READ |
                PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    (*pagemap)->frontend_cache = create_frontend_cache(*pagemap);
    (*pagemap)->span_head = create_base_span((*pagemap)->frontend_cache);
    (*pagemap)->total_pages = BASE_HEAP_SIZE / PAGE_SIZE;
    create_pages(*pagemap, (*pagemap)->span_head);
    (*pagemap)->top_chunk = (*pagemap)->span_head->page_head->top_chunk;
    (*pagemap)->last_chunk = NULL;
    create_fpages(*pagemap);
}

t_span* create_base_span(t_cache* cache) {
    t_span* span = (t_span*)MEMORY_SHIFT(CACHE_SHIFT(cache),
            ((cache->fcache_size * sizeof(t_tiny_chunk*)) + sizeof(t_cache_table)
            + (NUM_BINS * sizeof(cache_table_entry))));
    span->next = NULL;
    span->fastpages = NULL;
    span->page_head = NULL;
    span->top_chunk = NULL;
    span->last_chunk = NULL;
    span->num_pages = BASE_HEAP_SIZE / PAGE_SIZE;
    span->pages_returned = false;

    return span;
}

t_span* add_span(t_pagemap* pagemap, size_t size) {
    // map memory to span
    t_span* span = (t_span*)mmap(0, size, PROT_READ |
                PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    span->next = NULL;
    span->fastpages = NULL;
    span->page_head = NULL;
    span->top_chunk = NULL;
    span->last_chunk = NULL;
    span->num_pages = size / PAGE_SIZE;
    span->pages_returned = false;

    // add span to pagemap
    t_span* head = pagemap->span_head;
    while (head) {
        head = head->next;
    }
    head->next = span;

    return span;
}

/*
destroy_active_page() will be used in a future optimization. It will allow us
to return individual pages to the OS if the program running my_malloc is using
lower amounts of memory space.
*/
void destroy_active_page(t_page* page) {
    if (page->next && page->prev) {
        page->next->prev = page->prev;
        page->prev->next = page->next;
    }
    else if (page->next) {
        page->next->prev = NULL;
    }
    else {
        page->prev->next = NULL;
    }
    munmap(page, PAGE_SIZE);
}

void destroy_page(t_page* page) {
    munmap(page, PAGE_SIZE);
}

void destroy_pagemap(t_pagemap* pagemap) {
    t_span* span = pagemap->span_head;
    while (span) {
        t_page* cur = span->page_head;
        t_page* next = cur->next;
        if (span->pages_returned) {
            while (cur) {
                destroy_page(cur);
                cur = next;
                next = cur->next;
            }
        }
        else {
            munmap(span, (span->num_pages * PAGE_SIZE));
        }
        span = span->next;
    }
}