/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef GDK_LIST_H
#define GDK_LIST_H
#pragma once
#include "custdefs.h"
#include "stdint.h"

#if defined __cplusplus
extern "C" {
#endif

typedef struct gdk_list_link_s {
    struct gdk_list_link_s *p_next; // <-- next element in list, or NULL
} gdk_list_link_t;

typedef gdk_list_link_t* gdk_list_t;

typedef struct {
    int step_on_next;
    gdk_list_link_t* p_prev;    // points to the item before the current one
    gdk_list_link_t* p_curr;    // points to the current item (to detect item removal)
} gdk_list_itor_t;


/**
 * @brief tests if list is empty
 * @param p_list the opearation list.
 */
boolean  gdk_list_is_empty(const gdk_list_t* p_list);
/**
 * @brief find the last item in the list
 * @param p_list the opearation list.
 */
gdk_list_link_t* gdk_list_get_last(const gdk_list_t* p_list);
/**
 * @brief add item to list
 * @param p_list the opearation list.
 */
void gdk_list_add(gdk_list_t* p_list, gdk_list_link_t* p_node);
/**
 * @brief add item to list as last element
 * @param p_list the opearation list.
 */
void gdk_list_add_tail(gdk_list_t* p_list, gdk_list_link_t* p_node);
/**
 * @brief remove item from list
 * @param p_list the opearation list.
 */
int32_t  gdk_list_remove(gdk_list_t* p_list, gdk_list_link_t* p_node);

/**
 * @brief flush list.
 * @param p_list the opearation list.
 */
void gdk_list_flush(gdk_list_t* p_list);

/**
 * @brief remove item from list
 * @param p_list the opearation list.
 */
int32_t gdk_list_count(gdk_list_t* p_list);
/**
 * @brief get first element
 * @param p_list the opearation list.
 */
gdk_list_link_t* gdk_list_peek(const gdk_list_t* p_list);
/**
 * @brief get first element
 * @param p_list the opearation list.
 */
gdk_list_link_t* gdk_list_next(gdk_list_link_t* p_link);

/**
 * @brief get and remove first element
 * @param p_list the opearation list.
 */
gdk_list_link_t* gdk_list_pop(gdk_list_t* p_list);

void gdk_list_itor_init(gdk_list_itor_t *p_itor, gdk_list_t* p_list);
int32_t gdk_list_itor_has_next(gdk_list_itor_t *p_itor);
gdk_list_link_t* gdk_list_itor_next(gdk_list_itor_t *p_itor);
void gdk_list_itor_remove(gdk_list_itor_t* p_itor);

void gdk_list_itor_insert(gdk_list_itor_t* p_itor, gdk_list_link_t* p_node);

/**
 * @brief check if specify link is in the list.
 * @param p_list the opearation list.
 * @param p_link compare link.
 */
boolean gdk_list_search(
    gdk_list_t* p_list,
    gdk_list_link_t* p_link
);

#if defined __cplusplus
}
#endif
#endif

