/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * Description: mem module implementation
 */

#include "xlt_multipledlinkhead_pri.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define XLT_BITMAP_MASK 0x1FU
#define XLT_INVALID_BIT_INDEX 32
#define XLT_BIT32 32

static unsigned int xlt_clz(unsigned int num)
{
    if (num == 0)
        return XLT_BIT32;

    const unsigned int value = 0x80000000;
    unsigned int i = 0;
    while (value != 0) {
        if ((num & (value >> i)) != 0)
            return i;
        i++;
    }

    return 0;
}

static unsigned short xlt_high_bit_get(unsigned int bitmap)
{
    if (bitmap == 0)
        return XLT_INVALID_BIT_INDEX;

    return (unsigned short)(XLT_BITMAP_MASK - xlt_clz(bitmap));
}
static unsigned int os_log2(unsigned int size)
{
    return (size > 0) ? (unsigned int)xlt_high_bit_get(size) : 0;
}

void os_dlnk_init_multi_head(void *head_addr)
{
    xlt_multiple_dlink_head *dlink_head = (xlt_multiple_dlink_head *)head_addr;
    xlt_dl_list *list_node_head = dlink_head->list_head;
    unsigned int index;

    for (index = 0; index < OS_MULTI_DLNK_NUM; ++index, ++list_node_head)
        xlt_list_init(list_node_head);
}

xlt_dl_list *os_dlnk_multi_head(void *head_addr, unsigned int size)
{
    xlt_multiple_dlink_head *dlink_head = (xlt_multiple_dlink_head *)head_addr;
    unsigned int index = os_log2(size);
    if (index > OS_MAX_MULTI_DLNK_LOG2) {
        return NULL;
    } else if (index <= OS_MIN_MULTI_DLNK_LOG2) {
        index = OS_MIN_MULTI_DLNK_LOG2;
    }

    return dlink_head->list_head + (index - OS_MIN_MULTI_DLNK_LOG2);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
