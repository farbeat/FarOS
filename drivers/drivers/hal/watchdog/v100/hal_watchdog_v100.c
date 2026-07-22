/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V100 HAL watchdog \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-26, Create file. \n
 */
#include <stdint.h>
#include <stdio.h>
#include "securec.h"
#include "common_def.h"
#include "hal_watchdog.h"
#include "hal_watchdog_v100_regs_op.h"
#include "hal_watchdog_v100.h"

/* Watchdog register bit field describe */
#define HAL_WDT_COUNT_RESTART 0x76

#define TOP_MIN          16
#define MS_PER_SECONDS   1000
#define TOP_MIN_CLKS     0x10000
#define TOP_MAX_CLKS     0x80000000

static uint32_t g_timeout = 0;
static uint32_t g_wdt_clock = CONFIG_WDT_CLOCK;
static hal_watchdog_callback_t g_hal_watchdog_callback = NULL;

static void hal_watchdog_v100_clear_interrupt(void)
{
    (void)hal_watchdog_wdt_eoi_get_wdt_eoi();
}

static void hal_watchdog_v100_kick(void)
{
    hal_watchdog_wdt_crr_set_wdt_crr(HAL_WDT_COUNT_RESTART);
}

static void hal_watchdog_v100_disable(void)
{
    hal_watchdog_v100_clear_interrupt();
    hal_watchdog_wdt_cr_set_wdt_en(0);
}

static errcode_t hal_watchdog_v100_init(void)
{
    return hal_watchdog_regs_init();
}

static void hal_watchdog_v100_deinit(void)
{
    hal_watchdog_regs_deinit();
}

static errcode_t hal_watchdog_v100_set_attr(uint32_t timeout)
{
    uint32_t wdt_timeout = 0;
    uint32_t timeout_s = (timeout * g_wdt_clock);

    /* Check whether timeout_s overflow */
    if (((timeout == 0) || (timeout_s / timeout == g_wdt_clock)) == 0) {
        return ERRCODE_INVALID_PARAM;
    }

    if ((timeout_s < TOP_MIN_CLKS) || (timeout_s > TOP_MAX_CLKS) ||
        ((timeout_s & (timeout_s - 1)) != 0)) {
        return ERRCODE_INVALID_PARAM;
    }

    g_timeout = timeout;

    /* Obtaining the timeout that can be written to the register */
    timeout_s = timeout_s >> TOP_MIN;
    while (timeout_s > 1) {
        timeout_s = timeout_s >> 1;
        wdt_timeout++;
    }

    /* Be sure the WATCHDOG is disabled before configuring it. */
    hal_watchdog_v100_disable();

    /* Set wdt timeout range. */
    hal_watchdog_wdt_torr_set_top(wdt_timeout);

    return ERRCODE_SUCC;
}

static uint32_t hal_watchdog_v100_get_attr(void)
{
    return g_timeout;
}

static void hal_watchdog_v100_enable(hal_wdt_mode_t mode)
{
    /* Be sure the WATCHDOG is disabled before configuring it. */
    hal_watchdog_v100_disable();

    /* Set wdt mode. */
    if (mode == HAL_WDT_MODE_RESET) {
        hal_watchdog_wdt_cr_set_rmod(0);
    } else if (mode == HAL_WDT_MODE_INTERRUPT) {
        hal_watchdog_wdt_cr_set_rmod(1);
    }

    hal_watchdog_v100_clear_interrupt();

    /* Enable watchdog. */
    hal_watchdog_wdt_cr_set_wdt_en(1);

    hal_watchdog_v100_kick();
}

static uint32_t hal_watchdog_v100_get_left_time(void)
{
    return (uint32_t)(((uint64_t)hal_watchdog_wdt_ccvr_get_wdt_ccvr() * MS_PER_SECONDS) / g_wdt_clock);
}

static void hal_register_watchdog_v100_callback(hal_watchdog_callback_t callback)
{
    g_hal_watchdog_callback = callback;
}

void hal_watchdog_irq_handler(uintptr_t param)
{
    if (g_hal_watchdog_callback != NULL) {
        g_hal_watchdog_callback(param);
    }

    hal_watchdog_v100_clear_interrupt();
}

static hal_watchdog_funcs_t g_hal_watchdog_v100_funcs = {
    .init = hal_watchdog_v100_init,
    .deinit = hal_watchdog_v100_deinit,
    .set_attr = hal_watchdog_v100_set_attr,
    .get_attr = hal_watchdog_v100_get_attr,
    .enable = hal_watchdog_v100_enable,
    .disable = hal_watchdog_v100_disable,
    .kick = hal_watchdog_v100_kick,
    .get_left_time = hal_watchdog_v100_get_left_time,
    .register_callback = hal_register_watchdog_v100_callback
};

hal_watchdog_funcs_t *hal_watchdog_v100_funcs_get(void)
{
    return &g_hal_watchdog_v100_funcs;
}
