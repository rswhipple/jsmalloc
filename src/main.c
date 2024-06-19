/* ************************************************************************** */
/*                   Turn Decoration into a Q for Qwasar                      */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rswhipple  <rswhipple@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 06/11/2024 by rswhipple                  #+#    #+#             */
/*   Updated:                                         ###   ########          */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.h"

// global variable initialization
t_pagemap* g_pagemap = NULL;
size_t min_chunk_size = 0;
size_t pointer_size = 0;

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