/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: RISCV70 Machine timer config for LiteOS
 * Author: @CompanyNameTag
 * Create: 2022-5-25
 */
#include "hal_hwi.h"
#include "los_hwi.h"
#include "los_tick_pri.h"
#include "chip_io.h"
#include "arch_barrier.h"
#include "platform_core.h"
#include "idle_config.h"
#include "soc_osal.h"
#include "clocks_switch.h"
#include "systick.h"
#include "los_tick.h"
#include "clocks_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MACHINE_INT_NUM                 7

#define MACHINE_TIMER_BASE              TICK_TIMER_BASE_ADDR
#define MACHINE_TIMER_LOAD_COUNT        0x00
#define MACHINE_TIMER_LOAD_COUNT_L      0x00
#define MACHINE_TIMER_LOAD_COUNT_H      0x02
#define MACHINE_TIMER_CURRENT_VALUE     0x04
#define MACHINE_TIMER_CONTROL_REG       0x08
#define MACHINE_TIMER_EOI               0x0C
#define MACHINE_TIMER_INT_STATUS        0x10
#define MACHINE_TIMER_ENABLE_USRMODE    0x3

#ifndef OS_CYCLE_PER_TICK
#define OS_CYCLE_PER_TICK ((OS_SYS_CLOCK) / (LOSCFG_BASE_CORE_TICK_PER_SECOND))
#endif

void int_clear_pending_irq(int32_t irq_id);

static inline void machine_timer_init(void)
{
    // before config timer, disable it
    writel((MACHINE_TIMER_BASE + MACHINE_TIMER_CONTROL_REG), 0);

    writel((MACHINE_TIMER_BASE + MACHINE_TIMER_LOAD_COUNT), OS_CYCLE_PER_TICK);
    // user define mode and enable it
    writel((MACHINE_TIMER_BASE + MACHINE_TIMER_CONTROL_REG), MACHINE_TIMER_ENABLE_USRMODE);
}

static inline uint32_t machine_timer_get_cur_count(void)
{
    return readl(MACHINE_TIMER_BASE + MACHINE_TIMER_CURRENT_VALUE);
}

static inline uint32_t machine_timer_get_load_count(void)
{
    return readl(MACHINE_TIMER_BASE + MACHINE_TIMER_LOAD_COUNT);
}

static inline uint32_t machine_timer_get_int_status(void)
{
    return readl(MACHINE_TIMER_BASE + MACHINE_TIMER_INT_STATUS);
}

void HalClockDisable(void)
{
    writel((MACHINE_TIMER_BASE + MACHINE_TIMER_CONTROL_REG), 0);
    int_clear_pending_irq(MACHINE_INT_NUM);
}

void HalClockEnable(void)
{
    writel((MACHINE_TIMER_BASE + MACHINE_TIMER_LOAD_COUNT), OS_CYCLE_PER_TICK);
    // user define mode and enable it
    writel((MACHINE_TIMER_BASE + MACHINE_TIMER_CONTROL_REG), MACHINE_TIMER_ENABLE_USRMODE);
}

void HalTickEntry(void)
{
    // clear the timer interrupt
    int_clear_pending_irq(MACHINE_INT_NUM);
    readl(MACHINE_TIMER_BASE + MACHINE_TIMER_EOI);
    OsTickHandler();
}

LITE_OS_SEC_TEXT_INIT void HalClockStart(void)
{
    uint32_t ret;
    uint32_t lock_int_save;

    if ((OS_SYS_CLOCK == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND > OS_SYS_CLOCK)) {
        return;
    }

    lock_int_save = LOS_IntLock();
    osal_irq_request(MACHINE_INT_NUM, (osal_irq_handler)HalTickEntry, NULL, NULL, NULL);
    osal_irq_enable(MACHINE_INT_NUM);
    machine_timer_init();
    LOS_IntRestore(lock_int_save);
    idle_task_config();
}

void HalClockInit(void)
{
    SET_SYS_CLOCK(OS_SYS_CLOCK);
}

void HalDelayUs(unsigned usecs)
{
    uint64_t tmo = LOS_CurrNanosec() + (uint64_t)usecs * OS_SYS_NS_PER_US;

    while (LOS_CurrNanosec() < tmo) {
        nop();
    }
}

#ifdef SUPPORT_RC_CALIBRATION
volatile uint64_t g_pre_count = 0;
volatile double g_pre_msec = 0;
#endif
 
uint64_t HalClockGetCycles(void)
{
#ifdef SUPPORT_RC_CALIBRATION
    volatile double tick_count;
#else
    uint64_t tick_count;
#endif
    uint64_t cycle;
    uint32_t lock_int_save;
    uint32_t load_tick;
#ifdef SUPPORT_RC_CALIBRATION
    volatile double msec;
    volatile uint64_t now_count;
#endif
 
    lock_int_save = LOS_IntLock();
 
#ifdef SUPPORT_RC_CALIBRATION
    if (g_pre_count == 0) {
        tick_count = (uapi_systick_get_count() * OS_SYS_MS_PER_SECOND) / CLOCK_32K_CALI_FREQ;
        g_pre_count = uapi_systick_get_count();
        g_pre_msec = tick_count;
    } else {
        now_count = uapi_systick_get_count();
        msec = (double)((now_count - g_pre_count) * OS_SYS_MS_PER_SECOND) / CLOCK_32K_CALI_FREQ;
        tick_count = g_pre_msec + msec;
 
        g_pre_count = now_count;
        g_pre_msec = tick_count;
    }
#else
    tick_count = uapi_systick_get_ms();
#endif
 
    load_tick = machine_timer_get_load_count();
    cycle = (uint64_t)(tick_count * load_tick);
 
    LOS_IntRestore(lock_int_save);
    return cycle;
}

uint32_t HalGetRemainingCycles(uint32_t *ticks)
{
    uint32_t cycles;

    machine_timer_disable();
    cycles = machine_timer_get_cur_count();
    /* If the tick interrupt has arrived, update tick count and sleep ticks */
    if (machine_timer_get_int_status() != 0) {
        if (ticks != NULL) {
            (*ticks)--;
        }
        cycles = machine_timer_get_cur_count();
    }
    return cycles;
}

VOID HalTickCheckAndAdjust(uint32_t ticks)
{
    uint32_t period = OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    uint32_t remainingCycles = HalGetRemainingCycles(&ticks);
    uint32_t sleepCycles = (ticks - 1) * period;
    HalClockTickTimerReload(sleepCycles + remainingCycles);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
