/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  os adapt api source.
 * Author:
 * Create: 2023-03-09
 */

#include "los_printf.h"
#include "los_task_pri.h"
#include "asm/memmap_config.h"
#include "los_init.h"
#include "hal_hwi.h"

#ifdef LOSCFG_SHELL
#include "soc/uart.h"
#endif

extern char __heap_start;
extern void *g_intheap_size;
extern char __extend_heap_start;
extern void *g_extend_heap_size;

typedef VOID (*LOWPOWERIDLEHOOK)(VOID);
static LOWPOWERIDLEHOOK g_low_power_hook = NULL;
STATIC VOID OsAppInitHook(VOID) __attribute__((weakref("app_init_ram")));

typedef VOID (*SystemInitFunc)(VOID);
VOID SystemInitArray(UINTPTR start, UINTPTR end)
{
    SystemInitFunc func;
    UINTPTR *it = (UINTPTR *)start;
    UINTPTR *it_end = (UINTPTR *)end;
    while (it < it_end) {
        func = (SystemInitFunc)(*it);
        func();
        it++;
    }
    return;
}

/* brandy need change hwi triggle mode*/
LITE_OS_SEC_TEXT_INIT UINT32 OsAdaptInit(VOID)
{
    uint8_t clicintattr = 0;
    /* clicintattr[2:1] = 1 edge-triggered;
       clicintattr[2:1] = 0 level-triggered;
       clicintattr[7:6] = 3 M-mode */
    clicintattr |= ((0 << 1) | (3 << 6));
    for (uint8_t index = 0; index < LOSCFG_PLATFORM_HWI_LIMIT; index++) {
        HalIrqSetClicIntAttr(index, clicintattr);
    }
    return LOS_OK;
}
LOS_SYS_INIT(OsAdaptInit, SYS_INIT_LEVEL_APP, SYS_INIT_SYNC_1);

/* brandy need two memory pools,this init another mem pool in osMain*/
LITE_OS_SEC_TEXT_INIT UINT32 OsMemAdaptInit(VOID)
{
    if ((LOS_MemInit((void *)&__heap_start, (uint32_t)&g_intheap_size)) != 0) {
        PRINTK("liteos memory init error!\r\n");
    }

    m_aucSysMem1 = (uint8_t*)OS_EXTEND_MEM_ADDR;
    m_aucSysMem0 = m_aucSysMem1;
    return LOS_OK;
}
LOS_SYS_INIT(OsMemAdaptInit, SYS_INIT_LEVEL_KERNEL, SYS_INIT_SYNC_1);


__attribute__((weak)) void app_init(void)
{
    PRINTK("%s start ok\n", __FUNCTION__);
    if (OsAppInitHook != NULL) {
        OsAppInitHook();
    }

    return;
}

#ifndef LOSCFG_KERNEL_LOWPOWER
LITE_OS_SEC_TEXT_MINOR VOID LOS_LowpowerHookReg(LOWPOWERIDLEHOOK hook)
{
    g_low_power_hook = hook;
}
#endif

LITE_OS_SEC_TEXT VOID OsIdleTask(VOID)
{
    while (1) {
        LOS_TaskResRecycle();
        OsIdleHandler();

        if (g_low_power_hook != NULL) {
            g_low_power_hook();
        }
    }
}