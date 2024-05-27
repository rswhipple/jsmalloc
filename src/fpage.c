#include "../inc/main.h"

void create_fpages(t_pagemap* pagemap, t_span* span) {
    int page_count = FAST_PAGE_ALLOCATION_SIZE;
    span->fastpages = create_base_fpage(pagemap, span);
    page_count -= 1;

    t_fpage* current;
    UNUSED(current);
    current = span->fastpages;
    int chunk_size = min_chunk_size;
    while (page_count > 0 && chunk_size <= 64) {
        log_info("creating fpage");
        printf("page_count: %d\n", page_count);
        chunk_size += 8;
        create_fpage(current, span, chunk_size);
        current = (t_fpage*)current->next;
        page_count -= 1;
    }
}

t_fpage* create_base_fpage(t_pagemap* pagemap, t_span* span) {
    t_fpage* fpage = (t_fpage*)SPAN_SHIFT(span);
    fpage->chunk_count = 1;
    fpage->prev = NULL;
    fpage->next = NULL;
    if (span == pagemap->span_head) {
        // available memory accounts for t_pagemap, t_span and t_page space
        fpage->memory = PAGE_SIZE - sizeof(t_pagemap) -
            sizeof(t_span) - sizeof(t_fpage);
    }
    else {
        fpage->memory = PAGE_SIZE - sizeof(t_span) - sizeof(t_fpage);
    }
    fpage->chunk_size = min_chunk_size;

    create_top_tiny_chunk(fpage);
    return fpage;
}

t_fpage* create_fpage(t_fpage* prev_page, t_span* span, int size) {
    UNUSED(span);
    UNUSED(size);
    log_info("in create fpage");
    t_fpage* page = (t_fpage*)MEMORY_SHIFT(FASTPAGE_SHIFT(prev_page), prev_page->memory);
    page->chunk_count = 1;
    page->prev = prev_page;
    page->next = NULL;
    page->memory = PAGE_SIZE - sizeof(t_fpage);
    prev_page = page;
    log_info("creating tiny chunk");
    create_top_tiny_chunk(page);
    return page;
}