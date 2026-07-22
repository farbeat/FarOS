/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * Description: multipledlinkhead private headFile
 */
#ifndef _XLT_MULTIPLE_DLINK_HEAD_PRI_H
#define _XLT_MULTIPLE_DLINK_HEAD_PRI_H

#include "xlt_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_MAX_MULTI_DLNK_LOG2 29
#define OS_MIN_MULTI_DLNK_LOG2 4
#define OS_MULTI_DLNK_NUM ((OS_MAX_MULTI_DLNK_LOG2 - OS_MIN_MULTI_DLNK_LOG2) + 1)
#define XLT_DLNK_HEAD_SIZE XLT_MULTI_DLNK_HEAD_SIZE
#define XLT_MULTI_DLNK_HEAD_SIZE sizeof(xlt_multiple_dlink_head)

typedef struct {
    xlt_dl_list list_head[OS_MULTI_DLNK_NUM];
} xlt_multiple_dlink_head;

static inline xlt_dl_list *os_dlnk_next_multi_head(void *head_addr, xlt_dl_list *list_node_head)
{
    xlt_multiple_dlink_head *head = (xlt_multiple_dlink_head *)head_addr;

    return (&head->list_head[OS_MULTI_DLNK_NUM - 1] == list_node_head) ? NULL : (list_node_head + 1);
}

extern void os_dlnk_init_multi_head(void *head_addr);
extern xlt_dl_list *os_dlnk_multi_head(void *head_addr, unsigned int size);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _XLT_MULTIPLE_DLINK_HEAD_PRI_H */
