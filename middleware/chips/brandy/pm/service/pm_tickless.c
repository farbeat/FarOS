/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm tickless manage code
 */

#include "pm_sleep.h"
#include "pm_core.h"
#include "pm_dev.h"
#include "pm_os.h"
#include "clocks_config.h"

#define PM_SYS_TICK_CONFIG    0x570003e4
#define PM_SYS_TICK_NUM_BIT48 0x570003f4
#define PM_SYS_TICK_NUM_BIT32 0x570003f0
#define PM_SYS_TICK_NUM_BIT16 0x570003ec
#define PM_SYS_TICK_NUM_BIT0  0x570003e8
#define PM_SHIFT_48           48
#define PM_SHIFT_32           32
#define PM_SHIFT_16           16

typedef struct {
    uint32_t           slp_ms;
    uint32_t           handle;
    void               (*cb)(void);
} pm_wkup_timer_s;

static uint64_t g_entry_sleep_time = 0;
static uint64_t g_exit_sleep_time = 0;
static uint64_t g_32k_time_calibrate = 0;

uint32_t *g_pm_sleep_timer_handle = NULL;
static void pm_sleep_timer_callback(void)
{
    // Only wkup, Nothing to do
}

static void pm_sleep_start_wakeup_timer(uint32_t sleep_ms)
{
    pm_wkup_timer_s timer = {
        .slp_ms = sleep_ms,
        .handle = (uint32_t)&g_pm_sleep_timer_handle,
        .cb = pm_sleep_timer_callback,
    };

    (void)pm_dev_control(PM_DEV_M_RTC, DEV_RTC_START, (uint8_t *)&timer, sizeof(pm_wkup_timer_s));
}

static uint64_t pm_core_get_count(void)
{
    reg16_setbit(PM_SYS_TICK_CONFIG, 0);
    uint64_t tmp = ((uint64_t)readl(PM_SYS_TICK_NUM_BIT48) << PM_SHIFT_48);
    tmp |= ((uint64_t)readl(PM_SYS_TICK_NUM_BIT32) << PM_SHIFT_32);
    tmp |= ((uint64_t)readl(PM_SYS_TICK_NUM_BIT16) << PM_SHIFT_16);
    tmp |= ((uint64_t)readl(PM_SYS_TICK_NUM_BIT0));
    reg16_clrbit(PM_SYS_TICK_CONFIG, 0);

    return tmp;
}

static void pm_tickless_compensate_os_ticks(uint32_t ticks)
{
    uint32_t status = pm_os_irq_lock();
    g_exit_sleep_time = pm_core_get_count(); // freq:32768
    uint64_t sleep_time = ((g_exit_sleep_time - g_entry_sleep_time) * PM_OS_TICKS_PER_S) + g_32k_time_calibrate;
    uint32_t actual_sleep_time = (uint32_t)(sleep_time / CLOCK_32K_CALI_FREQ);
    g_32k_time_calibrate = actual_sleep_time % CLOCK_32K_CALI_FREQ;

    if (actual_sleep_time > ticks) {
#ifdef __FREERTOS__
        pm_os_compensate_sleep_ticks(ticks);
#else
        pm_os_compensate_sleep_ticks(actual_sleep_time);
#endif
    } else {
        pm_os_compensate_sleep_ticks(actual_sleep_time);
    }
    pm_os_irq_restore(status);
}

void pm_tickless_start(uint32_t sleep_ms)
{
    g_entry_sleep_time = pm_core_get_count();
    pm_sleep_start_wakeup_timer(sleep_ms);
    pm_dev_control(PM_DEV_M_TIMER, DEV_OSTIMER_DISABLE, NULL, 0);
}

void pm_tickless_stop(uint32_t sleep_ms)
{
    uint32_t ticks = 0;
    if (PM_OS_MS_PER_TICKS == 1) {
        ticks = (sleep_ms - 1) / PM_OS_MS_PER_TICKS;
    } else {
        ticks = (sleep_ms + (PM_OS_MS_PER_TICKS >> 1)) / PM_OS_MS_PER_TICKS;
    }

    pm_dev_control(PM_DEV_M_TIMER, DEV_OSTIMER_ENABLE, NULL, 0);
    pm_tickless_compensate_os_ticks(ticks);
}
