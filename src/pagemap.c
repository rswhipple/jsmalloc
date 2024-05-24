#include "../inc/main.h"

void create_pageheap(t_pagemap** pagemap) {
    *pagemap = (t_pagemap*)mmap(0, BASE_HEAP_SIZE, PROT_READ |
                PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    (*pagemap)->span_head = create_base_span(*pagemap);
    (*pagemap)->total_pages = BASE_HEAP_SIZE / PAGE_SIZE;
    create_pages(*pagemap, (*pagemap)->span_head);
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

void create_fpages(t_pagemap* pagemap, t_span* span) {
    int page_count = FAST_PAGE_ALLOCATION_SIZE;
    span->fastpages = create_base_fpage(pagemap, span);
    page_count -= 1;

    t_fpage* current;
    UNUSED(current);
    current = span->fastpages;
    int chunk_size = min_chunk_size;
    while (page_count > 0 && chunk_size <= 64) {
        chunk_size += 8;
        // how to align fpage with page?
        // current->next = create_fpage(current, span, chunk_size);
        // how to align fpage with page?
        // current = current->next;
        page_count -= 1;
    }
}

t_fpage* create_base_fpage(t_pagemap* pagemap, t_span* span) {
    t_fpage* fpage = (t_fpage*)SPAN_SHIFT(span);
    fpage->base.chunk_count = 1;
    fpage->base.prev = NULL;
    fpage->base.next = NULL;
    if (span == pagemap->span_head) {
        // available memory accounts for t_pagemap, t_span and t_page space
        fpage->base.memory = PAGE_SIZE - sizeof(t_pagemap) -
            sizeof(t_span) - sizeof(t_fpage);
    }
    else {
        fpage->base.memory = PAGE_SIZE - sizeof(t_span) - sizeof(t_fpage);
    }
    fpage->chunk_size = min_chunk_size;
    // can we use create_top_tiny_chunk here?
    // fpage->top_chunk = create_top_tiny_chunk(fpage);
    return fpage;
}

t_fpage* create_fpage(t_fpage* prev_page, t_span* span, int size) {
    UNUSED(span);
    UNUSED(size);
    t_fpage* page = (t_fpage*)MEMORY_SHIFT(FASTPAGE_SHIFT(prev_page), prev_page->base.memory);
    page->base.chunk_count = 1;
    // how to align fpage with page?
    // page->base.prev = prev_page;
    page->base.next = NULL;
    page->base.memory = PAGE_SIZE - sizeof(t_fpage);
    // can we use create_top_tiny_chunk here?
    // page->base.top_chunk = create_top_tiny_chunk(page);
    return page;
}

void create_pages(t_pagemap* pagemap, t_span* span) {
    // TODO: create and assign pages to types (fast, small, large)
    int page_count = span->num_pages;
    t_page* current = NULL;
    span->page_head = create_base_page(pagemap, span);
    page_count -= 1;
    current = span->page_head;
    while (page_count < TINY_HEAP_ALLOCATION_SIZE) {
        current = create_page(current, span, fast);
        page_count -= 1;
    }
}

t_page* create_base_page(t_pagemap* pagemap, t_span* span) {
    UNUSED(pagemap);
    t_page* page = (t_page*)SPAN_SHIFT(span);
    page->base.chunk_count = 1;
    page->base.prev = NULL;
    page->base.next = NULL;
    page->base.memory = PAGE_SIZE - sizeof(t_span) - sizeof(t_page);
    page->pagetype = small;
    create_top_chunk(page);
    return page;
}

t_page* create_page(t_page* prev_page, t_span* span, int pagetype) {
    UNUSED(span);
    t_page* page = (t_page*)MEMORY_SHIFT(PAGE_SHIFT(prev_page), prev_page->base.memory);
    log_info("creating page");
    printf("prev_page: %p\n", prev_page);
    printf("page: %p\n", page);
    page->base.chunk_count = 1;
    page->base.prev = (t_base_page*)prev_page;
    page->base.next = NULL;
    page->base.memory = PAGE_SIZE - sizeof(t_page);
    page->pagetype = pagetype;
    if (pagetype == fast) {
        log_info("creating top tiny chunk");
        printf("page: %p\n", page);
        // TODO: can we use create_top_tiny_chunk here?
        create_top_tiny_chunk((t_fpage*)page);
    }
    else {
        create_top_chunk(page); // TODO figure out logic
    }
    return page;
}

void destroy_active_page(t_page* page) {
    if (page->base.next && page->base.prev) {
        page->base.next->prev = page->base.prev;
        page->base.prev->next = page->base.next;
    }
    else if (page->base.next) {
        page->base.next->prev = NULL;
    }
    else {
        page->base.prev->next = NULL;
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
        t_page* next = (t_page*)cur->base.next;
        if (span->pages_returned) {
            while (cur) {
                destroy_page(cur);
                cur = next;
                next = (t_page*)cur->base.next;
            }
        }
        else {
            munmap(span, (span->num_pages * PAGE_SIZE));
        }
        span = span->next;
    }
}