#include "../inc/pagemap.h" 
#include "../inc/main.h"

void create_pageheap(t_pagemap** pagemap) {
    *pagemap = (t_pagemap*)mmap(0, BASE_HEAP_SIZE, PROT_READ |
                PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    (*pagemap)->span_head = create_base_span(*pagemap);
    (*pagemap)->total_pages = BASE_HEAP_SIZE / PAGE_SIZE;
    create_pages(*pagemap, (*pagemap)->span_head);
}

t_span* create_base_span(t_pagemap* pagemap) {
    t_span* span = (t_span*)SPAN_SHIFT(pagemap);
    span->next = NULL;
    span->page_head = NULL;
    span->top_chunk = NULL;
    span->last_chunk = NULL;
    span->num_pages = BASE_HEAP_SIZE / PAGE_SIZE;
    span->pages_returned = false;
    return span;
}

t_span* add_span(t_pagemap* pagemap, void* start, size_t size) {
    // map memory to span
    t_span* span = (t_span*)mmap(0, size, PROT_READ |
                PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    span->next = NULL;
    span->num_pages = size / PAGE_SIZE;

    // add span to pagemap
    t_span* head = pagemap->span_head;
    while (head) {
        head = head->next;
    }
    head->next = span;

    return span;
}

void create_pages(t_pagemap* pagemap, t_span* span) {
    // TODO: create and assign pages to types (fast, small, large)
    int page_count = span->num_pages;
    t_page* current = NULL;
    if (span == pagemap->span_head)
        span->page_head = create_base_page((void*)span);
    else span->page_head = create_page(current, (void*)span);
    page_count -= 1;
    current = span->page_head;
    while (page_count > 0) {
        void* next = (void*)((void*)current +
                (sizeof(char) * current->memory + sizeof(t_page))); // double check this calculation
        current = create_page(current, next);
        page_count -= 1;
    }
}

t_page* create_base_page(void* start) {
    // TODO: create "base" page with space for pagemap header;
    t_page* base_page = (t_page*)PAGE_SHIFT(start);
    base_page->chunk_count = 1;
    base_page->prev = NULL;
    base_page->next = NULL;
    base_page->memory = PAGE_SIZE -
        sizeof(t_pagemap) - sizeof(t_span) - sizeof(t_page);
    base_page->type = "F";
    base_page->top_chunk = create_chunk(base_page);
    return base_page;
}

t_page* create_page(t_page* prev_page, t_pagemap* pagemap) {
    t_page* page = (t_page*)PAGE_SHIFT(pagemap);
    page->chunk_count = 1;
    page->prev = NULL;
    page->next = NULL;
    page->memory = PAGE_SIZE -
        sizeof(t_pagemap) - sizeof(t_span) - sizeof(t_page);
    page->type = "F";
    page->top_chunk = create_chunk(page);
    return page;
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

void destroy_pageheap(t_pagemap* pagemap) {
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