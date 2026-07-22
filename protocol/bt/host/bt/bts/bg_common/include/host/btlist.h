/*
 * Copyright (c) @CompanyNameMagicTag 2015-2022. All rights reserved.
 */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    btlist.h
Abstract:
    This file includes the definition for thread safe list.
--------------------------------------------------------------------------- */

#ifndef BT_LIST_H
#define BT_LIST_H

#include <stdint.h>

/* ---------------------------------------------------------------------------
Module Name:
    Pre-Pointer, Variable Preload List management
Abstract:
    This module defines the Bi-Direction and Single-Direction List manage functions.
--------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/*                                 Pre 2 PTR * List                              */
/* --------------------------------------------------------------------------- */
struct bth_list_stru {
    void *head;
    void *tail;
};

/* Bidirection List Node */
#define lget(n)            ((void **)(n) + (1 * 2)) /* From memory start ptr go to userdata */
#define lstart(n)        ((void **)(n) - (1 * 2)) /* From userdata get back to memory start ptr */
#define lnext(n)        (*((void **)(n) - (1 * 2)))
#define lprev(n)        (*((void **)(n) - 1))
#define lfree(n)        (MEM_FREE(lstart(n))) /* Input ListNode userdata Ptr, Free it */

/* Single Direction List Node */
#define sget(n)            ((void **)(n) + 1) /* From memory start ptr go to userdata */
#define sstart(n)        ((void **)(n) - 1) /* From userdata get back to memory start ptr */
#define snext(n)        (*((void **)(n) - 1))
#define sfree(n)        (MEM_FREE(sstart(n))) /* Input ListNode userdata Ptr, Free it */

/* 4 direction List Node */

#define list_stru_off(stru_, mem_)                ((uint32_t)(&(((stru_)(NULL))->mem_)))
#define list_stru_mem_len(stru_, mem_)            sizeof((((stru_)(NULL))->mem_))
void *list_find_(struct bth_list_stru *list, uint32_t off, uint32_t len, uint8_t *val); /* Shall not be used by UI */

#define varlen(stru_, mem_, len_)                (((uint32_t)(&(((stru_)(NULL))->mem_))) + (len_))
#define new_varlen(stru_, mem_, len_)            MEM_NEW(varlen(stru_, mem_, len_))

/* ---------------------------------------------------------------------------
Example:
    list_find(&L2CAP_CID_LIST, struct L2CAP_CIDListStru *, cid, val);
--------------------------------------------------------------------------- */
/* This is UI */
#define list_find(list_, stru_, mem_, val_)                list_find_(list_, list_stru_off(stru_, mem_), \
    list_stru_mem_len(stru_, mem_), (uint8_t *)(&(val_)))
#define list_find_len(list_, stru_, mem_, len_, pval_)    list_find_(list_, list_stru_off(stru_, mem_), (len_), \
    (uint8_t *)(pval_))
#define list_find_len_off0(list_, len_, pval_)            list_find_(list_, 0, (len_), (uint8_t *)(pval_))

uint32_t list_count(struct bth_list_stru *list);
void *list_node_exist(struct bth_list_stru *list, const void *p);
#ifdef CONFIG_HW_MEMORY_DEBUG
void *_list_node_new_ext(uint32_t len, uint32_t pos);
#else
void *_list_node_new(uint32_t len);
#endif
void list_add_tail(struct bth_list_stru *list, void *node);
void list_add_head(struct bth_list_stru *list, void *node);
void list_insert_at(struct bth_list_stru *list, void *pos, void *node);
void *list_remove_head(struct bth_list_stru *list);
void *list_remove_tail(struct bth_list_stru *list);
void *list_remove_at(struct bth_list_stru *list, const void *node);
void list_remove_all(struct bth_list_stru *list);

uint32_t list_s_count(struct bth_list_stru *list);
#ifdef CONFIG_HW_MEMORY_DEBUG
void *_list_s_node_new_ext(uint32_t len, uint32_t pos);
#else
void *_list_s_node_new(uint32_t len);
#endif
void list_s_add_tail(struct bth_list_stru *list, void *node);
void list_s_add_head(struct bth_list_stru *list, void *node);
void *list_s_insert_at(struct bth_list_stru *list, void *pos, void *node);
void *list_s_remove_head(struct bth_list_stru *list);
void *list_s_remove_at_next(struct bth_list_stru *list, void *prev);
void list_s_remove_at(struct bth_list_stru *list, const void *node);
void list_s_remove_all(struct bth_list_stru *list);

#ifdef CONFIG_MEMORY_LEAK_DETECT
/* Not include header */
#define bth_list_node_new(l)     dbg_memory_leak_new(_list_node_new(l), (sizeof(void *) << 1), l, MEMORY_LEAK_POSINFO)
#define bth_list_s_node_new(l)    dbg_memory_leak_new(_list_s_node_new(l), sizeof(void *), l, MEMORY_LEAK_POSINFO)
#elif defined(CONFIG_HW_MEMORY_DEBUG) /* CONFIG_MEMORY_LEAK_DETECT */
#define bth_list_node_new(l)     _list_node_new_ext((l), MEMORY_LEAK_POSINFO)
#define bth_list_s_node_new(l)    _list_s_node_new_ext((l), MEMORY_LEAK_POSINFO)
#else
#define bth_list_node_new(l)        _list_node_new(l)
#define bth_list_s_node_new(l)    _list_s_node_new(l)
#endif
#ifdef CONFIG_MEMORY_LEAK_DETECT
/* Not include header */
#define list_node_new(l)     dbg_memory_leak_new(_list_node_new(l), (sizeof(void *) << 1), l, MEMORY_LEAK_POSINFO)
#define list_snode_new(l)    dbg_memory_leak_new(_list_s_node_new(l), sizeof(void *), l, MEMORY_LEAK_POSINFO)
#elif defined(CONFIG_HW_MEMORY_DEBUG) /* CONFIG_MEMORY_LEAK_DETECT */
#define list_node_new(l)     _list_node_new_ext((l), MEMORY_LEAK_POSINFO)
#define list_snode_new(l)    _list_s_node_new_ext((l), MEMORY_LEAK_POSINFO)
#else
#define list_node_new(l)        _list_node_new(l)
#define list_snode_new(l)    _list_s_node_new(l)
#endif
#endif

