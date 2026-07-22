/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides systick porting template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-01， Create file. \n
 */
#include "systick_porting.h"
#include "systick.h"
#include "hal_systick_v150.h"
#include "platform_core.h"

#define writel(addr, d)      (*(volatile uint32_t*)(uintptr_t)(addr) = (uint32_t)(d))
static uintptr_t g_systick_base_addr =  (uintptr_t)SYSTICK_BASE_ADDR;

uintptr_t systick_porting_base_addr_get(void)
{
    return g_systick_base_addr;
}

void systick_port_register_hal_funcs(void)
{
    hal_systick_register_funcs(hal_systick_v150_funcs_get());
}

void systick_port_unregister_hal_funcs(void)
{
    hal_systick_unregister_funcs();
}

void systick_port_cali_xclk(void)
{
    writel(0x5702c028, 1); // XO: 32Khz, XCLK
}

#ifdef SUPPORT_RC_CALIBRATION
volatile uint64_t g_pre_systick_count = 0;
volatile uint64_t g_pre_systick_us = 0;
uint64_t uapi_systick_get_time(systick_time_t time_type)
{
    volatile uint64_t now_count = 0;
    volatile uint64_t ret_time = 0;
    volatile uint64_t cost_us = 0;

    uint32_t irq_sts = osal_irq_lock();
    uint64_t pre_count = g_pre_systick_count;
    uint64_t pre_us = g_pre_systick_us;
    uint64_t rc_clk = CLOCK_32K_CALI_FREQ;
    if (g_pre_systick_count == 0) {
        pre_count = uapi_systick_get_count();
        g_pre_systick_us = (pre_count * (OS_SYS_US_PER_MS * OS_SYS_MS_PER_SECOND)) / rc_clk;
        g_pre_systick_count = pre_count;
    } else {
        now_count = uapi_systick_get_count();
        cost_us = ((now_count - pre_count) * (OS_SYS_US_PER_MS * OS_SYS_MS_PER_SECOND)) / rc_clk;
        pre_us = pre_us + cost_us;
        g_pre_systick_us = pre_us;
        g_pre_systick_count = now_count;
    }
    osal_irq_restore(irq_sts);

    switch (time_type) {
        case SYSTICK_TIME_S:
            ret_time = (uint64_t)(pre_us / (OS_SYS_US_PER_MS * OS_SYS_MS_PER_SECOND));
            break;
        case SYSTICK_TIME_MS:
            ret_time = (uint64_t)(pre_us / OS_SYS_US_PER_MS);
            break;
        default:
            ret_time = (uint64_t)(pre_us);
            break;
    }

    return ret_time;
}
#endif