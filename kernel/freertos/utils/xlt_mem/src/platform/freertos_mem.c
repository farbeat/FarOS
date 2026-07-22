/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: FreeRTOS memory implementation
 */

#include "FreeRTOS.h"
#include "task.h"
#include "xlt_memory.h"

void *pvPortMalloc(size_t xWantedSize)
{
    return xlt_mem_alloc(OS_SYS_MEM_POOL, xWantedSize);
}

void vPortFree(void *pv)
{
    xlt_mem_free(OS_SYS_MEM_POOL, pv);
}

size_t xPortGetFreeHeapSize(void)
{
    xlt_mem_pool_status memstatus;
    xlt_mem_info_get(OS_SYS_MEM_POOL, &memstatus);
    return memstatus.uw_total_free_size;
}

void *pvPortMallocExtendMem(size_t xWantedSize)
{
    return xlt_mem_alloc(OS_EXTEND_MEM_ADDR, xWantedSize);
}

void vPortFreeExtendMem(void *pv)
{
    xlt_mem_free(OS_EXTEND_MEM_ADDR, pv);
}

size_t xPortGetFreeHeapSizeExtendMem(void)
{
    xlt_mem_pool_status memstatus;
    xlt_mem_info_get(OS_EXTEND_MEM_ADDR, &memstatus);
    return memstatus.uw_total_free_size;
}
/* ----------------------------------------------------------- */

size_t xPortGetMinimumEverFreeHeapSize(void)
{
    return 0;
}