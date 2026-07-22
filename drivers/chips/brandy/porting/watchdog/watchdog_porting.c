/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides watchdog port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "watchdog_porting.h"
#include "hal_watchdog_v100.h"
#include "watchdog.h"
#include "tcxo.h"
#include "platform_core.h"
#include "cpu_utils.h"
#include "debug_print.h"
#include "non_os.h"
#include "chip_io.h"
#include "osal_interrupt.h"

#define PARAM       0
#define WAIT_APPS_DUMP_DSP_DELAY_MS  15000
#define WDT_PORTING_ENABLE_DELAY     60uLL
#define DEFAULT_WDT_TIME             8

uintptr_t g_watchdog_base_addr = (uintptr_t)CHIP_WDT_BASE_ADDRESS;
hal_chip_watchdog_dump_callback_t g_hal_dsp_chip_watchdog_callback = NULL;
static uint32_t g_watchdog_config = DEFAULT_WDT_TIME ;

void watchdog_port_register_hal_funcs(void)
{
    hal_watchdog_register_funcs(hal_watchdog_v100_funcs_get());
}

void watchdog_port_unregister_hal_funcs(void)
{
    hal_watchdog_unregister_funcs();
}

void watchdog_port_register_irq(void)
{
    return;
}

void irq_wdt_handler(void)
{
    hal_watchdog_irq_handler((uintptr_t)PARAM);
}

/* ------------------------------------------------------------------------------*/
/* The following is the product adaptation section. */
/* ------------------------------------------------------------------------------*/
__IRQ void irq_chip_wdt_dsp_handler(void)
{
#if MASTER_ONLY
    uapi_watchdog_kick();
    PRINT("[DSP->BT] irq_chip_wdt_handler !\r\n");
    if (g_hal_dsp_chip_watchdog_callback != NULL) {
        g_hal_dsp_chip_watchdog_callback();
    }
    uapi_tcxo_delay_ms((uint64_t)WAIT_APPS_DUMP_DSP_DELAY_MS);
#endif
#ifndef NO_TIMEOUT
#if CORE == MASTER_BY_ALL
    cpu_utils_reset_chip_with_log((cores_t)HIFI, REBOOT_CAUSE_PROTOCOL_WATCHDOG);
#endif
#else
    volatile uint8_t i = 1;
    while (i != 0) {
    }
#endif
}

#if MCU_ONLY
__IRQ void irq_chip_wdt_dsp1_handler(void)
{
    uapi_watchdog_kick();
#ifndef NO_TIMEOUT
    cpu_utils_reset_chip_with_log((cores_t)HIFI, REBOOT_CAUSE_PROTOCOL1_WATCHDOG);
#endif
}

__IRQ void irq_dsp_addr_mon_handler(void)
{
    uapi_watchdog_kick();
#ifndef NO_TIMEOUT
    cpu_utils_reset_chip_with_log((cores_t)HIFI, REBOOT_CAUSE_PROTOCOL_ADDR_MON);
#endif
}

__IRQ void irq_dsp1_addr_mon_handler(void)
{
    uapi_watchdog_kick();
#ifndef NO_TIMEOUT
    cpu_utils_reset_chip_with_log((cores_t)HIFI, REBOOT_CAUSE_PROTOCOL1_ADDR_MON);
#endif
}
#endif

void watchdog_dsp_inform_mcpu_enable(void)
{
#if DSP_EXIST == YES
    osal_irq_request(DSP_WDT_IRQn, (osal_irq_handler)irq_chip_wdt_dsp_handler, NULL, NULL, NULL);
    osal_irq_request(DSP0_ADDR_MON_IRQn, (osal_irq_handler)irq_dsp_addr_mon_handler, NULL, NULL, NULL);
    (void)osal_irq_set_priority(DSP_WDT_IRQn, irq_prio(DSP_WDT_IRQn));
    (void)osal_irq_set_priority(DSP0_ADDR_MON_IRQn, irq_prio(DSP0_ADDR_MON_IRQn));
    osal_irq_enable(DSP_WDT_IRQn);
    osal_irq_enable(DSP0_ADDR_MON_IRQn);
#endif
}

void watchdog_func_adapt(uint32_t time)
{
    watchdog_dsp_inform_mcpu_enable();
    non_os_nmi_config(NMI_CWDT, true);
    watchdog_set_conifgtime(time);
}

void watchdog_turnon_clk(void)
{
    reg_setbit(HAL_SOFT_RST_CTL_BASE, HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET, HAL_CHIP_WDT_ATOP1_RST_BIT);
    uapi_tcxo_delay_us(WDT_PORTING_ENABLE_DELAY);
}

void watchdog_turnoff_clk(void)
{
    reg_clrbit(HAL_SOFT_RST_CTL_BASE, HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET, HAL_CHIP_WDT_ATOP1_RST_BIT);
}

void watchdog_set_conifgtime(uint32_t time)
{
    g_watchdog_config = time;
}

uint32_t watchdog_get_conifgtime(void)
{
    return g_watchdog_config;
}

void hal_dsp_chip_wdt_register_dump_callback(hal_chip_watchdog_dump_callback_t callback)
{
    if (callback != NULL) {
        g_hal_dsp_chip_watchdog_callback = callback;
    }
}
