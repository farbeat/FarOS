/*
 * Copyright (c) @CompanyNameMagicTag 2015-2022. All rights reserved.
 */


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    btlist.c
Abstract:
    This file provides private memory management functions.
--------------------------------------------------------------------------- */
#ifndef BTMEM_H
#define BTMEM_H

#ifdef CONFIG_MEMORY_STATIC
#ifdef CONFIG_TYPE_BIT16_ONLY
#define HUGE huge
#else
#define HUGE
#endif

void *mem_malloc (int size);
void MEM_FREE(void *buf);

void *mem_realloc (void *buf, int size);
void *mem_calloc (int num, int size);

void init_static_mem(void);
void done_static_mem(void);

#ifdef CONFIG_MEMORY_LEAK_DETECT
uint32_t dbg_memory_len2space(uint32_t len);
#endif

#endif /* CONFIG_MEMORY_STATIC */

#endif /* BTMEM_H */
