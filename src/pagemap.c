#include "../inc/main.h"

void create_pagemap(t_pagemap** pagemap) {
    log_info("creating pageheap");
    printf("BASE_HEAP_SIZE: %d\n", BASE_HEAP_SIZE);
    printf("PAGE_SIZE: %d\n", PAGE_SIZE);
    *pagemap = (t_pagemap*)mmap(0, BASE_HEAP_SIZE, PROT_READ |
                PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    (*pagemap)->span_head = create_base_span(*pagemap);
    (*pagemap)->total_pages = BASE_HEAP_SIZE / PAGE_SIZE;
    create_pages(*pagemap, (*pagemap)->span_head);
    create_fpages(*pagemap, (*pagemap)->span_head);
}

t_span* create_base_span(t_pagemap* pagemap) {
    t_span* span = (t_span*)PAGEMAP_SHIFT(pagemap);
    span->next = NULL;
    span->fastpages = NULL;
    span->page_head = NULL;
    span->top_chunk = NULL;
    span->last_chunk = NULL;
    span->num_pages = BASE_HEAP_SIZE / PAGE_SIZE;
    span->pages_returned = false;
    return span;
}

t_span* add_span(t_pagemap* pagemap, void* start, size_t size) {
    UNUSED(start);
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
        t_page* next = (t_page*)cur->next;
        if (span->pages_returned) {
            while (cur) {
                destroy_page(cur);
                cur = next;
                next = (t_page*)cur->next;
            }
        }
        else {
            munmap(span, (span->num_pages * PAGE_SIZE));
        }
        span = span->next;
    }
}