#include "../inc/pagemap.h" 
#include "../inc/my_malloc.h" 

void create_pageheap(t_pagemap **pagemap) {
    *pagemap = (t_pagemap *)mmap(0, BASE_HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    (*pagemap)->span_head = create_span((void *)*pagemap, BASE_HEAP_SIZE);
    (*pagemap)->total_pages = BASE_HEAP_SIZE / PAGE_SIZE;
    create_pages((*pagemap)->span_head);
}

t_span *create_base_span(void *start, size_t size) {
    t_span *span = (t_span *)PAGEMAP_SHIFT(start);
    span->next = NULL;
    span->num_pages = size / PAGE_SIZE;
    // t_page *page_one = 

    // span->page_head;
    return span;
}

void create_base_page(t_span *span, ) {
    t_page *page_one = (t_page *)SPAN_SHIFT(span);
    span->page_head = span;
    t_page *current = NULL;


}

void destroy_active_page(t_page *page) {
    if (page->next && page->prev) {
        page->next->prev = page->prev;
        page->prev->next = page->next;
    } else if (page->next) {
        page->next->prev = NULL;
    } else {
        page->prev->next = NULL;
    }

    munmap(page, PAGE_SIZE);
}

void destroy_page(t_page *page) {
    munmap(page, PAGE_SIZE);
}

void destroy_pageheap(t_pagemap *pagemap) {
    t_span *span = pagemap->span_head;
    while (span) {
        t_page *cur = span->page_head;
        t_page *next = cur->next;
        if (span->pages_returned) {
            while (cur) {
                destroy_page(cur);
                cur = next;
                next = cur->next;
            }
        } else {
            munmap(span, (span->num_pages * PAGE_SIZE));
        }
        span = span->next;
    }
}