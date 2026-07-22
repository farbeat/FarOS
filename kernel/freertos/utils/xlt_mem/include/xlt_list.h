/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * Description:  doubly linked list
 */
/**
 * @defgroup los_list Doubly linked list
 * @ingroup kernel
 */

#ifndef _XLT_LIST_H
#define _XLT_LIST_H

#include "xlt_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup xlt_list
 * Structure of a node in a doubly linked list.
 */
typedef struct xlt_dl_list {
    struct xlt_dl_list *pst_prev; /* *< Current node's pointer to the previous node */
    struct xlt_dl_list *pst_next; /* *< Current node's pointer to the next node */
} xlt_dl_list;

/**
 * @ingroup xlt_list
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
 * @see xlt_dl_list_head
 * @since Huawei LiteOS V100R001C00
 */
static inline void xlt_list_init(xlt_dl_list *list)
{
    list->pst_next = list;
    list->pst_prev = list;
}

/**
 * @ingroup xlt_list
 * @brief Point to the next node of the current node.
 *
 * @par Description:
 * This API is used to point to the next node of the current node.
 * @attention
 * None.
 *
 * @param object  [IN] Type # xlt_dl_list *  The node in the doubly linked list.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_dl_list_last
 * @since Huawei LiteOS V100R001C00
 */
#define xlt_dl_list_first(object) ((object)->pst_next)

/**
 * @ingroup los_list
 * @brief Point to the previous node of the current node.
 *
 * @par Description:
 * This API is used to point to the previous node of the current node.
 * @attention
 * None.
 *
 * @param object  [IN] Type # xlt_dl_list *  The node in the doubly linked list.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_dl_list_first
 * @since Huawei LiteOS V100R001C00
 */
#define xlt_dl_list_last(object) ((object)->pst_prev)

/**
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
 * @see xlt_list_delete | xlt_list_tail_insert | xlt_list_head_insert
 * @since Huawei LiteOS V100R001C00
 */

static inline void xlt_list_add(xlt_dl_list *list, xlt_dl_list *node)
{
    node->pst_next = list->pst_next;
    node->pst_prev = list;
    list->pst_next->pst_prev = node;
    list->pst_next = node;
}

/**
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
 * @see xlt_list_add | xlt_list_head_insert
 * @since Huawei LiteOS V100R001C00
 */

static inline void xlt_list_tail_insert(xlt_dl_list *list, xlt_dl_list *node)
{
    xlt_list_add(list->pst_prev, node);
}

/**
 * @ingroup los_list
 * @brief Insert a node to a doubly linked list.
 *
 * @par Description:
 * This API is used to insert a new node after the list node to a doubly linked list.
 * It is same with #xlt_list_add.
 * @attention
 * The parameters passed in should be legal pointers.
 *
 * @param list     [IN] Doubly linked list which the new node will be inserted in.
 * @param node     [IN] The new node to be inserted.
 *
 * @retval None.
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_list_add | xlt_list_tail_insert
 * @since Huawei LiteOS V100R001C00
 */

static inline void xlt_list_head_insert(xlt_dl_list *list, xlt_dl_list *node)
{
    xlt_list_add(list, node);
}

/**
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
 * @see xlt_list_add
 * @since Huawei LiteOS V100R001C00
 */

static inline void xlt_list_delete(xlt_dl_list *node)
{
    node->pst_next->pst_prev = node->pst_prev;
    node->pst_prev->pst_next = node->pst_next;
    node->pst_next = NULL;
    node->pst_prev = NULL;
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
 * @since Huawei LiteOS V100R001C00
 */

static inline bool xlt_list_empty(xlt_dl_list *list)
{
    return (bool)(list->pst_next == list);
}

/**
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
 * @retval #UINTPTR Offset of the member relative to the structure start address.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
#define xlt_off_set_of(type, member) ((UINTPTR) & ((type *)0)->member)

/* Obsolete API, please use xlt_off_set_of instead */
#define offset_of_field(type, field) xlt_off_set_of(type, field)

/**
 * @ingroup los_list
 * @brief Obtain the pointer to a structure that contains a doubly linked list.
 *
 * @par Description:
 * This API is used to obtain the pointer to a structure that contains the doubly
 * linked list which the first parameter item specified.
 * @attention
 * None.
 *
 * @param item    [IN] Type # xlt_dl_list *  The node of the doubly linked list.
 * @param type    [IN] Structure name.
 * @param member  [IN] The doubly linked list name in the structure.
 *
 * @retval The pointer to the structure that contains the doubly linked list. And
 * the doubly linked list has the node of the first parameter item.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_dl_list_for_each_entry | xlt_dl_list_for_each_entry_safe
 * @since Huawei LiteOS V100R001C00
 */
#define xlt_dl_list_entry(item, type, member) ((type *)(void *)((char *)(item)-xlt_off_set_of(type, member)))

/**
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
 * @param list    [IN] Type # xlt_dl_list *  The start node of the doubly linked list to
 * be traversed.
 * @param type    [IN] Structure name.
 * @param member  [IN] The doubly linked list name in the structure.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_dl_list_entry | xlt_dl_list_for_each_entry_safe | xlt_dl_list_for_each
 * @since Huawei LiteOS V100R001C00
 */
#define xlt_dl_list_for_each_entry(item, list, type, member)                                  \
    for (item = xlt_dl_list_entry((list)->pst_next, type, member); &(item)->member != (list); \
        item = xlt_dl_list_entry((item)->member.pst_next, type, member))

/**
 * @ingroup los_list
 * @brief Traverse a doubly linked list which is included in a given type structure. And
 * it is safe against removal of list entry.
 *
 * @par Description:
 * This API is used to traverse a doubly linked list which is included in a given type
 * structure. The API is a loop. The start node of the doubly linked list is the third
 * parameter list. And in each loop, the obtained pointer to a structure that contains
 * the list is outputted in the first parameter item. And the next node is outputted in
 * the second parameter next.
 * @attention
 * None.
 *
 * @param item    [IN/OUT] The pointer to the structure that contains the doubly linked list.
 * @param next    [IN/OUT] The pointer to the structure that contains the next node of the
 * doubly linked list.
 * @param list    [IN] Type # xlt_dl_list *  The start node of the doubly linked list to
 * be traversed.
 * @param type    [IN] Structure name.
 * @param member  [IN] The doubly linked list name in the structure.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_dl_list_entry | xlt_dl_list_for_each_entry | xlt_dl_list_for_each_safe
 * @since Huawei LiteOS V100R001C00
 */
#define xlt_dl_list_for_each_entry_safe(item, next, list, type, member)  \
    for (item = xlt_dl_list_entry((list)->pst_next, type, member),       \
        next = xlt_dl_list_entry((item)->member.pst_next, type, member); \
        &(item)->member != (list); item = next, next = xlt_dl_list_entry((item)->member.pst_next, type, member))

/**
 * @ingroup los_list
 * @brief Iterate over a doubly linked list of given type, and call hook for any extra procedures every time.
 *
 * @par Description:
 * This API is used to iterate over a doubly linked list of given type,
 * and call hook for any extra procedures every time.
 * @attention
 * None.
 *
 * @param item           [IN/OUT] Pointer to the structure that contains the doubly linked list that is to be traversed.
 * @param list           [IN] Pointer to the doubly linked list to be traversed.
 * @param type           [IN] Structure name.
 * @param member         [IN] Member name of the doubly linked list in the structure.
 * @param hook           [IN] Hook for extra procedures which will be called every time when dev is fetched.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_dl_list_entry | xlt_dl_list_for_each_entry
 * @since Huawei LiteOS V200R005C10
 */
#define xlt_dl_list_for_each_entry_hook(item, list, type, member, hook)                             \
    for (item = xlt_dl_list_entry((list)->pst_next, type, member), hook; &(item)->member != (list); \
        item = xlt_dl_list_entry((item)->member.pst_next, type, member), hook)

/**
 * @ingroup los_list
 * @brief Delete a specified node from a doubly linked list and reinitialize the node.
 *
 * @par Description:
 * This API is used to delete a specified node (the first parameter list) from the doubly
 * linked list. And reinitialize the deleted node to a doubly linked list.
 *
 * @attention
 * The parameter passed in should be a legal pointer.
 *
 * @param list    [IN] Node to be deleted and reinitialize to a doubly linked list.
 *
 * @retval None.
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_list_init | xlt_list_delete
 * @since Huawei LiteOS V100R001C00
 */

static inline void xlt_list_del_init(xlt_dl_list *list)
{
    list->pst_next->pst_prev = list->pst_prev;
    list->pst_prev->pst_next = list->pst_next;
    xlt_list_init(list);
}

/**
 * @ingroup los_list
 * @brief Traverse a doubly linked list.
 *
 * @par Description:
 * This API is used to traverse a doubly linked list. The API is a loop. The start node of the
 * doubly linked list is the second parameter list. And in each loop, the obtained pointer to
 * the next node of the doubly linked list is outputted in the first parameter item.
 * @attention
 * None.
 *
 * @param item        [IN/OUT] Type # xlt_dl_list *  The pointer to the next node in the doubly
 * linked list.
 * @param list        [IN] Type # xlt_dl_list *   The pointer to the node of the doubly linked
 * list to be traversed.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_dl_list_for_each_safe | xlt_dl_list_for_each_entry
 * @since Huawei LiteOS V100R001C00
 */
#define xlt_dl_list_for_each(item, list) for (item = (list)->pst_next; (item) != (list); item = (item)->pst_next)

/**
 * @ingroup los_list
 * @brief Traverse a doubly linked list safe against removal of list entry.
 *
 * @par Description:
 * This API is used to traverse a doubly linked list safe against removal of list entry. The
 * API is a loop. The start node of the doubly linked list is the third parameter list. And
 * in each loop, the obtained pointer to the next node of the doubly linked list is outputted
 * in the first parameter item. And the next node of the the node specified by first parameter
 * item is outputted in the second parameter next.
 * @attention
 * None.
 *
 * @param item        [IN/OUT] Type # xlt_dl_list *  The pointer to the next node in the doubly
 * linked list.
 * @param next        [IN/OUT] Type # xlt_dl_list *  The pointer to the next node of the the node
 * specified by first parameter item.
 * @param list        [IN]     Type # xlt_dl_list *  The pointer to the node of the doubly linked
 * list to be traversed.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_dl_list_for_each | xlt_dl_list_for_each_entry_safe
 * @since Huawei LiteOS V100R001C00
 */
#define xlt_dl_list_for_each_safe(item, next, list) \
    for (item = (list)->pst_next, next = (item)->pst_next; (item) != (list); item = next, next = (item)->pst_next)

/**
 * @ingroup los_list
 * @brief Initialize a double linked list.
 *
 * @par Description:
 * This API is used to initialize the input node (the parameter list) to a double linked
 * list. The difference with xlt_list_init is that the parameter list is not a pointer while
 * in xlt_list_init it is a pointer.
 * @attention
 * None.
 *
 * @param list    [IN] Type # xlt_dl_list  A node to be initialized to a doubly linked list.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_list_init
 * @since Huawei LiteOS V100R001C00
 */
#define xlt_dl_list_head(list) xlt_dl_list list = { &(list), &(list) }

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _XLT_LIST_H */
