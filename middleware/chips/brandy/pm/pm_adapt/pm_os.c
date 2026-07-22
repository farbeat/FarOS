/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm os adapter code
 */

#include "pm_os.h"
#include "chip_io.h"
#include "pm_sleep.h"
#if defined(__LITEOS__)
#include "platform.h"
#include "los_task.h"
#include "los_memory.h"
#elif defined(__FREERTOS__)
#include "hal_freertos_timer.h"
#include "hwi.h"
#include "task.h"
#endif

uint32_t pm_os_get_sleep_ms(void)
{
#if defined(__LITEOS__)
    return oal_get_sleep_ticks() * PM_OS_MS_PER_TICKS;
#elif defined(__FREERTOS__)
    return 0;
#endif
}

void pm_os_compensate_sleep_ticks(uint32_t ticks)
{
#if defined(__LITEOS__)
    oal_ticks_restore(ticks);
#elif defined(__FREERTOS__)
    vTaskStepTick(ticks);
#endif
}

uint32_t pm_os_irq_lock(void)
{
#if defined(__LITEOS__)
    return (uint32_t)LOS_IntLock();
#elif defined(__FREERTOS__)
    return (uint32_t)uxHwiLock();
#endif
}

void pm_os_irq_restore(uint32_t irq_status)
{
#if defined(__LITEOS__)
    LOS_IntRestore(irq_status);
#elif defined(__FREERTOS__)
    vHwiRestore(irq_status);
#endif
}

void *pm_os_malloc(uint32_t size)
{
    UNUSED(size);
#if defined(__LITEOS__)
    return LOS_MemAlloc(m_aucSysMem0, size);
#else
    return pvPortMalloc(size);
#endif
}

void pm_os_free(void *addr)
{
    UNUSED(addr);
#if defined(__LITEOS__)
    LOS_MemFree(m_aucSysMem0, addr);
#elif defined(__FREERTOS__)
    vPortFree(addr);
#endif
}

void pm_os_timer_enable(void)
{
#if (ARCH == RISCV70)
#if defined(__LITEOS__)
    HalClockEnable();
#elif defined(__FREERTOS__)
    vTimerEnable();
#endif
#else /* linx131 */
    os_tick_timer_enable();
#endif
}

void pm_os_timer_disable(void)
{
#if (ARCH == RISCV70)
#if defined(__LITEOS__)
    HalClockDisable();
#elif defined(__FREERTOS__)
    vTimerDisable();
#endif
#else /* linx131 */
    os_tick_timer_disable();
#endif
}