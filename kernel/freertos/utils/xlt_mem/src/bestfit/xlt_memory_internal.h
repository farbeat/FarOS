/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * Description:  memory private struct defines headfile
 */
#ifndef _XLT_MEMORY_INTERNAL_H
#define _XLT_MEMORY_INTERNAL_H

#include "xlt_typedef.h"
#include "xlt_memory_pri.h"
#include "xlt_multipledlinkhead_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define XLT_MEM_NODE_HEAD_SIZE sizeof(xlt_mem_dyn_node)
#define XLT_MEM_MIN_POOL_SIZE (XLT_DLNK_HEAD_SIZE + (2 * XLT_MEM_NODE_HEAD_SIZE) + sizeof(xlt_mem_pool_info))
#define is_pow_two(value) ((((UINTPTR)(value)) & ((UINTPTR)(value)-1)) == 0)
#define POOL_ADDR_ALIGNSIZE 64
#define OS_MEM_NODE_USED_FLAG 0x80000000U
#define OS_MEM_NODE_ALIGNED_FLAG 0x40000000U
#define OS_MEM_NODE_ALIGNED_AND_USED_FLAG (OS_MEM_NODE_USED_FLAG | OS_MEM_NODE_ALIGNED_FLAG)

#define xlt_mem_node_get_aligned_flag(size_and_flag) ((size_and_flag) & OS_MEM_NODE_ALIGNED_FLAG)

#define xlt_mem_node_set_aligned_flag(size_and_flag) ((size_and_flag) = ((size_and_flag) | OS_MEM_NODE_ALIGNED_FLAG))

#define xlt_mem_node_get_aligned_gapsize(size_and_flag) ((size_and_flag) & ~OS_MEM_NODE_ALIGNED_FLAG)

#define xlt_mem_node_get_used_flag(size_and_flag) ((size_and_flag) & OS_MEM_NODE_USED_FLAG)

#define xlt_mem_node_set_used_flag(size_and_flag) ((size_and_flag) = ((size_and_flag) | OS_MEM_NODE_USED_FLAG))

#define xlt_mem_node_get_size(size_and_flag) ((size_and_flag) & ~OS_MEM_NODE_ALIGNED_AND_USED_FLAG)

#define xlt_mem_head(pool, size) os_dlnk_multi_head(xlt_mem_head_addr(pool), size)

#define xlt_mem_head_addr(pool) ((void *)((UINTPTR)(pool) + sizeof(xlt_mem_pool_info)))

#define xlt_mem_next_node(node) \
    ((xlt_mem_dyn_node *)(void *)((unsigned char *)(node) + xlt_mem_node_get_size((node)->self_node.size_and_flag)))

#define xlt_mem_first_node(pool) \
    ((xlt_mem_dyn_node *)(void *)((unsigned char *)xlt_mem_head_addr(pool) + XLT_DLNK_HEAD_SIZE))

#define xlt_mem_end_node(pool, size) \
    ((xlt_mem_dyn_node *)(void *)(((unsigned char *)(pool) + (size)) - XLT_MEM_NODE_HEAD_SIZE))

#define xlt_mem_middle_addr_open_end(start_addr, middle_addr, end_addr)   \
    (((unsigned char *)(start_addr) <= (unsigned char *)(middle_addr)) && \
        ((unsigned char *)(middle_addr) < (unsigned char *)(end_addr)))

#define xlt_mem_middle_addr(start_addr, middle_addr, end_addr)            \
    (((unsigned char *)(start_addr) <= (unsigned char *)(middle_addr)) && \
        ((unsigned char *)(middle_addr) <= (unsigned char *)(end_addr)))

#define xlt_mem_set_magic(value) ((value) = (UINTPTR) & (value) ^ (UINTPTR)(-1))

#define xlt_mem_magic_valid(value) (((UINTPTR)(value) ^ (UINTPTR) & (value)) == (UINTPTR)(-1))

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _XLT_MEMORY_INTERNAL_H */
