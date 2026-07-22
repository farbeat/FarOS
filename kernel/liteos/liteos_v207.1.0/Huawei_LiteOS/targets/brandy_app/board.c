/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  os board source.
 * Author:
 * Create: 2023-03-09
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include "arch/regs.h"
#include "arch/cache.h"

#ifdef LOSCFG_DRIVERS_BASE
#include "los_driverbase.h"
#endif
#include "asm/platform.h"
#include "asm/delay.h"
#include "soc/uart.h"
#include "los_task_pri.h"
#include "los_tick_pri.h"
#include "los_percpu_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

void dma_cache_clean(UINTPTR start, UINTPTR end)
{
    ArchDCacheCleanByAddr(start, end);
}

void dma_cache_inv(UINTPTR start, UINTPTR end)
{
    ArchDCacheInvByAddr(start, end);
}

LITE_OS_SEC_TEXT VOID ArchICacheInvAll(VOID)
{
    WRITE_CUSTOM_CSR_VAL(ICMAINT, ICIV);
    mb();
}

LITE_OS_SEC_TEXT VOID ArchDCacheInvAll(VOID)
{
    WRITE_CUSTOM_CSR_VAL(DCMAINT, DCIV);
    mb();
}

void OsCacheInit(void)
{
    dsb();
    ArchICacheInvAll();
    ArchDCacheInvAll();
    dsb();

    (void)ArchICacheEnable(CACHE_32KB);
    (void)ArchICachePrefetchEnable(CACHE_PREF_1_LINES);

    (void)ArchDCacheEnable(CACHE_32KB);
    (void)ArchDCachePrefetchEnable(CACHE_PREF_1_LINES, CACHE_PREF_1_LINES);

    return;
}

uint32_t oal_get_sleep_ticks(VOID)
{
    uint32_t intSave;
    intSave = LOS_IntLock();
    uint32_t taskTimeout = OsSortLinkGetNextExpireTime(&(OsPercpuGet()->taskSortLink));
#ifdef LOSCFG_BASE_CORE_SWTMR
    uint32_t swtmrTimeout = OsSortLinkGetNextExpireTime(&(OsPercpuGet()->swtmrSortLink));
    if (swtmrTimeout < taskTimeout) {
        taskTimeout = swtmrTimeout;
    }
#endif
    LOS_IntRestore(intSave);
    return taskTimeout;
}

VOID oal_ticks_restore(uint32_t ticks)
{
    uint32_t intSave;
    intSave = LOS_IntLock();
    uint32_t taskTimeout = OsSortLinkGetNextExpireTime(&(OsPercpuGet()->taskSortLink));
    if (taskTimeout > (ticks - 1)) {
        g_tickCount[ArchCurrCpuid()] += ticks;
        OsSortLinkUpdateExpireTime(ticks, &(OsPercpuGet()->taskSortLink));
    } else {
        g_tickCount[ArchCurrCpuid()] += ticks;
        OsSortLinkUpdateExpireTime(taskTimeout, &(OsPercpuGet()->taskSortLink));
    }
#ifdef LOSCFG_BASE_CORE_SWTMR
    uint32_t swtmrTimeout = OsSortLinkGetNextExpireTime(&(OsPercpuGet()->swtmrSortLink));
    if (swtmrTimeout > (ticks - 1)) {
        OsSortLinkUpdateExpireTime(ticks, &(OsPercpuGet()->swtmrSortLink));
    } else {
        OsSortLinkUpdateExpireTime(swtmrTimeout, &(OsPercpuGet()->swtmrSortLink));
    }
    
#endif
    LOS_IntRestore(intSave);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
