
#ifndef CACHE_H
#define CACHE_H

#include "types.h"

void print_fast_cache(t_tiny_chunk** fast_cache);
t_cache* create_frontend_cache(t_pagemap* pagemap);
t_tiny_chunk** create_fast_cache(t_cache *cache);

#endif