/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: sdk list header
 * Author: @CompanyNameTag
 * Create: 2022-03-03
 */

#ifndef SDK_LIST_H
#define SDK_LIST_H

#include "sdk_private.h"

struct list_stru {
    void *head;
    void *tail;
};

#define lget(n)            ((void **)(n) + (1 * 2))
#define lstart(n)          ((void **)(n) - (1 * 2))
#define lnext(n)           (*((void **)(n) - (1 * 2)))
#define lprev(n)           (*((void **)(n) - 1))

void *bt_sdk_list_node_new(uint32_t len);
void *bt_sdk_list_node_create(uint32_t len);
void *bt_sdk_list_remove(struct list_stru *list, void *node);
void bt_sdk_list_add_tail(struct list_stru *list, void *node);
void *bt_sdk_list_remove_head(struct list_stru *list);
void bt_sdk_list_insert_before(struct list_stru *list, void *next, void *node);

#define sdk_list_node_free(n) (bt_os_free(lstart(n)))
#define sdk_list_node_new(size) (bt_sdk_list_node_create(size))
#endif

