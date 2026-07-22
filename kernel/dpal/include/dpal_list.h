/*
 * Copyright (C) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: Flash FMC MTD header file.
 * Create: 2021-02-01
 */

#ifndef DPAL_LIST_H
#define DPAL_LIST_H

#include "dpal_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup los_list
 * Structure of a node in a doubly linked list.
 */
typedef struct dpal_dl_list {
    struct dpal_dl_list *prev; /**< Current node's pointer to the previous node */
    struct dpal_dl_list *next; /**< Current node's pointer to the next node */
} dpal_dl_list_t;

/*
 * @ingroup los_list
 * @brief Initialize the input node to a doubly linked list.
 *
 * @par Description:
 * This API is used to initialize the input node (the first parameter list) to
 * a doubly linked list.
 * @attention
 * The parameter passed in should be a legal pointer.
 *
 * @param list    [IN] A node in a doubly linked list.
 *
 * @retval None.
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_DL_LIST_HEAD
 */
static inline void dpal_list_init(dpal_dl_list_t *list)
{
    list->next = list;
    list->prev = list;
}

/*
 * @ingroup los_list
 * @brief Insert a new node to a doubly linked list.
 *
 * @par Description:
 * This API is used to insert a new node after the list node to a doubly linked list.
 * @attention
 * The parameters passed in should be legal pointers.
 *
 * @param list    [IN] Doubly linked list which the new node will be inserted in.
 * @param node    [IN] The new node to be inserted.
 *
 * @retval None
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see dpal_list_delete | dpal_list_tail_insert | LOS_ListHeadInsert
 */
static inline void dpal_list_add(dpal_dl_list_t *list, dpal_dl_list_t *node)
{
    node->next = list->next;
    node->prev = list;
    list->next->prev = node;
    list->next = node;
}

/*
 * @ingroup los_list
 * @brief Insert a node to a doubly linked list.
 *
 * @par Description:
 * This API is used to insert a new node before the list node to a doubly linked list.
 * @attention
 * The parameters passed in should be legal pointers.
 *
 * @param list     [IN] Doubly linked list which the new node will be inserted in.
 * @param node     [IN] The new node to be inserted.
 *
 * @retval None.
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see boot_list_add | LOS_ListHeadInsert
 */
static inline void dpal_list_tail_insert(dpal_dl_list_t *list, dpal_dl_list_t *node)
{
    dpal_list_add(list->prev, node);
}

/*
 * @ingroup los_list
 * @brief Delete a specified node from a doubly linked list.
 *
 * @par Description:
 * This API is used to delete a specified node from a doubly linked list.
 * @attention
 * The parameter passed in should be a legal pointer.
 *
 * @param node    [IN] Node to be deleted.
 *
 * @retval None.
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see dpal_list_add
 */
static inline void dpal_list_delete(dpal_dl_list_t *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->next = NULL;
    node->prev = NULL;
}

/**
 * @ingroup los_list
 * @brief Identify whether a specified doubly linked list is empty or not.
 *
 * @par Description:
 * This API is used to judge whether a doubly linked list is empty or not. It
 * returns a Boolean value.
 * @attention
 * The parameter passed in should be a legal pointer.
 *
 * @param list  [IN] Doubly linked list.
 *
 * @retval #TRUE  The doubly linked list is empty.
 * @retval #FALSE The doubly linked list is not empty.
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 */
static inline bool dpal_list_empty(dpal_dl_list_t *list)
{
    return (bool)(list->next == list);
}

static inline void dpal_list_deinit(dpal_dl_list_t *list)
{
    list->next->prev = list->prev;
    list->prev->next = list->next;
    dpal_list_init(list);
}

/*
 * @ingroup los_list
 * @brief Obtain the offset of a structure member relative to the structure start address.
 *
 * @par  Description:
 * This API is used to obtain the offset of the structure member (member) relative to
 * the start address of the structure (type). And return the offset of #UINTPTR type.
 * @attention
 * None.
 *
 * @param type   [IN] Structure name.
 * @param member [IN] The structure member name which needs to measure the offset.
 *
 * @retval #uintptr_t Offset of the member relative to the structure start address.
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 */
#define BOOT_OFF_SET_OF(type, member) ((uintptr_t)&((type *)0)->member)

/*
 * @ingroup los_list
 * @brief Obtain the pointer to a structure that contains a doubly linked list.
 *
 * @par Description:
 * This API is used to obtain the pointer to a structure that contains the doubly
 * linked list which the first parameter item specified.
 * @attention
 * None.
 *
 * @param item    [IN] Type #dpal_dl_list *  The node of the doubly linked list.
 * @param type    [IN] Structure name.
 * @param member  [IN] The doubly linked list name in the structure.
 *
 * @retval The pointer to the structure that contains the doubly linked list. And
 * the doubly linked list has the node of the first parameter item.
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see BOOT_DL_LIST_FOR_EACH_ENTRY | LOS_DL_LIST_FOR_EACH_ENTRY_SAFE
 */
#define DPAL_DL_LIST_ENTRY(item, type, member) \
    ((type *)(void *)((char *)(item) - BOOT_OFF_SET_OF(type, member)))

/*
 * @ingroup los_list
 * @brief Traverse a doubly linked list which is included in a given type structure.
 *
 * @par Description:
 * This API is used to traverse a doubly linked list which is included in a given type
 * structure. The API is a loop. The start node of the doubly linked list is the second
 * parameter list. And in each loop, the obtained pointer to a structure that contains
 * the list is outputted in the first parameter item.
 * @attention
 * None.
 *
 * @param item    [IN/OUT] The pointer to the structure that contains the doubly linked list.
 * @param list    [IN] Type #dpal_dl_list *  The start node of the doubly linked list to
 *                                          be traversed.
 * @param type    [IN] Structure name.
 * @param member  [IN] The doubly linked list name in the structure.
 *
 * @retval None.
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see BOOT_DL_LIST_ENTRY | LOS_DL_LIST_FOR_EACH_ENTRY_SAFE | LOS_DL_LIST_FOR_EACH
 */
#define DPAL_DL_LIST_FOR_EACH_ENTRY(item, list, type, member)             \
    for ((item) = DPAL_DL_LIST_ENTRY((list)->next, type, member);        \
         &(item)->member != (list);                                      \
         (item) = DPAL_DL_LIST_ENTRY((item)->member.next, type, member))

#define dpal_dl_list_for_each(item, list) \
    for ((item) = (list)->next;         \
         (item) != (list);               \
         (item) = (item)->next)

#define dpal_dl_list_for_each_safe(item, n, list)      \
    for (item = (list)->next, n = (item)->next; \
         (item) != (list);                               \
         item = n, n = (item)->next)

/*
 * @ingroup los_list
 * @brief Initialize a double linked list.
 *
 * @par Description:
 * This API is used to initialize the input node (the parameter list) to a double linked
 * list. The difference with LOS_ListInit is that the parameter list is not a pointer while
 * in LOS_ListInit it is a pointer.
 * @attention
 * None.
 *
 * @param list    [IN] Type #LOS_DL_LIST  A node to be initialized to a doubly linked list.
 *
 * @retval None.
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_ListInit
 */
#define DPAL_DL_LIST_HEAD(list) dpal_dl_list_t list = { &(list), &(list) }

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DPAL_LIST_H__ */
