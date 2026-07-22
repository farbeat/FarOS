/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: Application core kernel init function for standard
 * Author:
 * Create:
 */

#include "kernel_init.h"
#include "stdint.h"
#include "chip_io.h"
#include "securec.h"
#include "panic.h"
#include "pmp_config.h"
#if defined(__LITEOS__)
#include "los_task_pri.h"
#include "los_printf_pri.h"
#include "arch/exception.h"
#include "arch/canary.h"
#elif defined(__FREERTOS__)
#include "FreeRTOS.h"
#include "task.h"
#include "cache.h"
#endif
#include "exception.h"
#include "cmsis_os2.h"
#include "soc_osal.h"
#include "vectors.h"
#include "debug_print.h"
#include "watchdog.h"
#include "watchdog_porting.h"
#include "pmu_pg.h"
#if defined(XLTCFG_SUPPORT_MEMMNG)
#include "xlt_memory.h"
#endif

extern uint32_t __l2m_gpu_text_start;
extern uint32_t __l2m_gpu_text_load;
extern uint32_t __l2m_gpu_text_end;
extern uint32_t __l2m_gpu_bss_start;
extern uint32_t __l2m_gpu_bss_end;
extern uint32_t __l2m_gpu_data_start;
extern uint32_t __l2m_gpu_data_load;
extern uint32_t __l2m_gpu_data_end;
extern uint32_t __l2m_dsp_text_start;
extern uint32_t __l2m_dsp_text_load;
extern uint32_t __l2m_dsp_text_end;
extern uint32_t __l2m_dsp_bss_start;
extern uint32_t __l2m_dsp_bss_end;
extern uint32_t __l2m_dsp_data_start;
extern uint32_t __l2m_dsp_data_load;
extern uint32_t __l2m_dsp_data_end;

#ifdef CONFIG_PSRAM_SUPPORT
extern uint32_t __psram_data_start;
extern uint32_t __psram_data_load;
extern uint32_t __psram_data_end;
extern uint32_t __psram_bss_start;
extern uint32_t __psram_bss_load;
extern uint32_t __psram_bss_end;
#endif

os_class g_os_init_select = OS_CLASS_LITEOS;

os_class get_os_init_select(void)
{
    return g_os_init_select;
}

extern char __heap_start;
extern void *g_intheap_size;
extern char __extend_heap_start;
extern void *g_extend_heap_size;

#ifdef SW_RTT_DEBUG
extern uint32_t __rtt_bss_end;
extern uint32_t __rtt_bss_start;
#endif

#if (defined SUPPORT_CXX)
typedef void (*SystemInitFunc)(void);
extern char __init_array_start;
extern char __init_array_end;

static void system_init_array(uintptr_t start, uintptr_t end)
{
    if (start >= end) {
        return;
    }

    SystemInitFunc func;
    uintptr_t *it = (uintptr_t *)start;
    for (; it != (uintptr_t *)end; ++it) {
        func = (SystemInitFunc)(*it);
        func();
    }
}
#endif

static void copy_from_flash_to_l2ram(void)
{
    errno_t ret;
    uint32_t size = (uint32_t)(&__l2m_gpu_text_end) - (uint32_t)(&__l2m_gpu_text_start);
    ret = memcpy_s((void *)((uint32_t)(&__l2m_gpu_text_start)), size, (void *)(uint32_t)(&__l2m_gpu_text_load), size);
    if (ret != EOK) {
        PRINT("l2m gpu text memcpy fail."NEWLINE);
    }

    size = (uint32_t)(&__l2m_gpu_data_end) - (uint32_t)(&__l2m_gpu_data_start);
    ret = memcpy_s((void *)((uint32_t)(&__l2m_gpu_data_start)), size, (void *)(uint32_t)(&__l2m_gpu_data_load), size);
    if (ret != EOK) {
        PRINT("l2m gpu data memcpy fail."NEWLINE);
    }

    size = (uint32_t)(&__l2m_gpu_bss_end) - (uint32_t)(&__l2m_gpu_bss_start);
    ret = memset_s((void *)((uint32_t)(&__l2m_gpu_bss_start)), size, 0, size);
    if (ret != EOK) {
        PRINT("l2m gpu bss memset fail."NEWLINE);
    }
#if DSP_L2RAM_AREA_SIZE < DSP_L2RAM_AREA_TOTAL
    size = (uint32_t)(&__l2m_dsp_text_end) - (uint32_t)(&__l2m_dsp_text_start);
    ret = memcpy_s((void *)((uint32_t)(&__l2m_dsp_text_start)), size, (void *)(uint32_t)(&__l2m_dsp_text_load), size);
    if (ret != EOK) {
        PRINT("l2m dsp text memcpy fail."NEWLINE);
    }
    size = (uint32_t)(&__l2m_dsp_data_end) - (uint32_t)(&__l2m_dsp_data_start);
    ret = memcpy_s((void *)((uint32_t)(&__l2m_dsp_data_start)), size, (void *)(uint32_t)(&__l2m_dsp_data_load), size);
    if (ret != EOK) {
        PRINT("l2m dsp data memcpy fail."NEWLINE);
    }
    size = (uint32_t)(&__l2m_dsp_bss_end) - (uint32_t)(&__l2m_dsp_bss_start);
    ret = memset_s((void *)((uint32_t)(&__l2m_dsp_bss_start)), size, 0, size);
    if (ret != EOK) {
        PRINT("l2m dsp bss memset fail."NEWLINE);
    }
#endif
#ifdef SW_RTT_DEBUG
    size = (uint32_t)(&__rtt_bss_end) - (uint32_t)(&__rtt_bss_start);
    ret = memset_s((void *)((uint32_t)(&__rtt_bss_start)), size, 0, size);
    if (ret != EOK) {
        PRINT("l2m dsp bss memset fail."NEWLINE);
    }
#endif
}

#ifdef CONFIG_PSRAM_SUPPORT
static void copy_from_flash_to_psram(void)
{
    errno_t ret;
    uint32_t size = (uint32_t)(&__psram_data_end) - (uint32_t)(&__psram_data_start);
    ret = memcpy_s((void *)((uint32_t)(&__psram_data_start)), size, (void *)(uint32_t)(&__psram_data_load), size);
    if (ret != EOK) {
        PRINT("psram data memcpy fail."NEWLINE);
    }

    size = (uint32_t)(&__psram_bss_end) - (uint32_t)(&__psram_bss_start);
    ret = memset_s((void *)((uint32_t)(&__psram_bss_start)), size, 0, size);
    if (ret != EOK) {
        PRINT("psram bss memset fail."NEWLINE);
    }
}
#endif

#if defined(__FREERTOS__)
static void OsCacheInit(void)
{
    dsb();
    ArchICacheInvAll();
    ArchDCacheInvAll();
    dsb();

    (void)ArchICacheEnable(CACHE_32KB);
    (void)ArchICachePrefetchEnable(CACHE_PREF_1_LINES);

    (void)ArchDCacheEnable(CACHE_32KB);
    (void)ArchDCachePrefetchEnable(CACHE_PREF_1_LINES, CACHE_PREF_1_LINES);
}
#endif

static void chip_watchdog_init(void)
{
    watchdog_turnon_clk();
#ifdef CONFIG_SYSTEM_VIEW
    watchdog_func_adapt(CHIP_WDT_TIMEOUT_128S);
    uapi_watchdog_init(CHIP_WDT_TIMEOUT_128S);
#else
    watchdog_func_adapt(CHIP_WDT_TIMEOUT_32S);
    uapi_watchdog_init(CHIP_WDT_TIMEOUT_32S);
#endif
    uapi_watchdog_enable(WDT_MODE_INTERRUPT);
}

void kernel_init(void)
{
    chip_watchdog_init();

    pmu1_dsp_l2ram_config(DSP_L2RAM_AREA_SIZE);
    copy_from_flash_to_l2ram();

#ifdef CONFIG_PSRAM_SUPPORT
    copy_from_flash_to_psram();
#endif

    panic_init();

#if defined(__FREERTOS__)
    /* os kernel adapt config */
    vPortHwiInit();

    /* enable icache and dcahce */
    pmp_init();
    OsCacheInit();

    /* memory pool init */
#if defined(XLTCFG_SUPPORT_MEMMNG)
    g_heap = (void *)&__heap_start;
    if (xlt_mem_init(g_heap, (uint32_t)&g_intheap_size)) {
        panic(PANIC_MEMORY, __LINE__);
    }

    g_extend_heap = (void *)&__extend_heap_start;
    if (xlt_mem_init(g_extend_heap, (uint32_t)&g_extend_heap_size)) {
        panic(PANIC_MEMORY, __LINE__);
    }
#endif
    osal_irq_request(NON_MASKABLE_INT_IRQN, (osal_irq_handler)do_fault_handler_freertos, NULL, NULL, NULL);
    osal_irq_enable(NON_MASKABLE_INT_IRQN);
    g_os_init_select = OS_CLASS_FREERTOS;
    (void)osKernelInitialize();
    vTimerTickInit();

#elif defined(__LITEOS__)
    /* enable icache and dcahce */
    pmp_init();
    OsCacheInit();

    /* os kernel adapt config */
    ArchSetExcHook((EXC_PROC_FUNC)do_fault_handler);
    ArchSetNMIHook((NMI_PROC_FUNC)nmi_fault_handler);
    OsSetMainTask();
    OsCurrTaskSet((LosTaskCB*)OsGetMainTask());
    g_os_init_select = OS_CLASS_LITEOS;

    (void)osKernelInitialize();
#endif

#if defined SUPPORT_CXX
    system_init_array((uintptr_t)&__init_array_start, (uintptr_t)&__init_array_end);
#endif
}
