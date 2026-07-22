/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: XLT Memory Module Private HeadFile
 * --------------------------------------------------------------------------- */

#ifndef XLT_MEMSTAT_PRI_H
#define XLT_MEMSTAT_PRI_H

#include "xlt_typedef.h"
#include "xlt_memory.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* extra 1 blocks is for extra temparary task */
#define TASK_NUM        (LOSCFG_BASE_CORE_TSK_LIMIT + 1)
#define MIN_TASK_ID(x, y)               ((x) > (y) ? (y) : (x))
#define MAX_MEM_USE(x, y)               ((x) > (y) ? (x) : (y))

#ifdef XLTCFG_MEM_DEBUG
typedef struct {
    UINT32 mem_used;
    UINT32 mem_peak;
} task_mem_used_info;

typedef struct {
    UINT32 mem_total_used;
    UINT32 mem_total_peak;
#ifdef LOSCFG_TASK_MEM_USAGE
    task_mem_used_info task_memstats[TASK_NUM];
#endif
} memstat;

extern void os_memstat_task_used_inc(memstat *stat, UINT32 used_size, UINT32 task_id);
extern void os_memstat_task_used_dec(memstat *stat, UINT32 used_size, UINT32 task_id);
extern void os_memstat_task_clear(memstat *stat, UINT32 task_id);
extern UINT32 os_memstat_task_usage(const memstat *stat, UINT32 task_id);
#endif

extern void os_mem_task_clear(UINT32 task_id);
extern UINT32 os_mem_task_usage(UINT32 task_id);

#ifdef LOSCFG_MEM_TASK_STAT
#define os_mem_add_used(stat, used_size, task_id)         os_memstat_task_used_inc(stat, used_size, task_id)
#define os_mem_reduce_used(stat, used_size, task_id)      os_memstat_task_used_dec(stat, used_size, task_id)
#ifdef LOSCFG_TASK_MEM_USAGE
#define os_mem_clear(task_id)                            os_memstat_task_clear(task_id)
#endif
#define os_mem_usage(task_id)                            os_memstat_task_usage(task_id)
#else
#define os_mem_add_used(stat, used_size, task_id)
#define os_mem_reduce_used(stat, used_size, task_id)
#define os_mem_clear(task_id)
#define os_mem_usage(task_id)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* XLT_MEMSTAT_PRI_H */
