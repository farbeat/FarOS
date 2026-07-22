/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides v150 HAL MEM_Monitor \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-23, Create file. \n
 */
#include "common_def.h"
#include "hal_mem_monitor.h"
#include "hal_mem_monitor_v150.h"
#if defined(CONFIG_MEM_MONITOR_DEBUG)
#include "debug/osal_debug.h"
#endif /* CONFIG_MEM_MONITOR_DEBUG */

static hal_mem_monitor_callback_t g_hal_mem_monitor_callbacks[MEM_MONITOR_NUM_MAX] = { NULL };

static errcode_t hal_mem_monitor_v150_init(void)
{
    if (hal_mem_monitor_regs_init() != ERRCODE_SUCC) {
        return ERRCODE_MEM_MONITOR_REG_ADDR_INVALID;
    }

    return ERRCODE_SUCC;
}

static void hal_mem_monitor_v150_deinit(void)
{
    hal_mem_monitor_regs_deinit();
}

static void hal_mem_monitor_v150_enable(mem_monitor_t monitor)
{
    hal_mem_monitor_reg_set_en(monitor, 1);
}

static void hal_mem_monitor_v150_disable(mem_monitor_t monitor)
{
    hal_mem_monitor_reg_clr_interrupt(monitor);

    hal_mem_monitor_reg_addr_en_clr(monitor);
    hal_mem_monitor_reg_set_en(monitor, 0);
}

static void hal_mem_monitor_v150_clock_enable(mem_monitor_t monitor)
{
    hal_mem_monitor_reg_set_clock_en(monitor, 1);
}

static void hal_mem_monitor_v150_clock_disable(mem_monitor_t monitor)
{
    hal_mem_monitor_reg_set_clock_en(monitor, 0);
}

static bool hal_mem_monitor_v150_set_section(mem_monitor_t monitor,
                                             mem_monitor_section_t section,
                                             uint32_t addr_start,
                                             uint32_t addr_end)
{
    if ((monitor >= MEM_MONITOR_NUM_MAX) ||
        (section >= MEM_MONITOR_SECTION_MAX)) {
        return false;
    }

    if (!mem_monitor_port_check_addr(addr_start, addr_end)) {
        return false;
    }

    uint16_t addr_high_low_16;   // addr_end  16~31 bits
    uint16_t addr_high_high_16;  // addr_end  0~15 bits
    uint16_t addr_low_low_16;    // addr_start 0~15 bits
    uint16_t addr_low_high_16;   // addr_start 16~31 bits

    addr_high_low_16 = lou16(addr_end);
    addr_high_high_16 = hiu16(addr_end);
    addr_low_low_16 = lou16(addr_start);
    addr_low_high_16 = hiu16(addr_start);

    hal_mem_monitor_reg_mem_addr_set_l_l(monitor, section, addr_low_low_16);
    hal_mem_monitor_reg_mem_addr_set_l_h(monitor, section, addr_low_high_16);
    hal_mem_monitor_reg_mem_addr_set_h_l(monitor, section, addr_high_low_16);
    hal_mem_monitor_reg_mem_addr_set_h_h(monitor, section, addr_high_high_16);

    return true;
}

static bool hal_mem_monitor_v150_set_section_mode(mem_monitor_t monitor,
                                                  mem_monitor_section_t section,
                                                  hal_mem_monitor_mode_t mode)
{
    if ((monitor >= MEM_MONITOR_NUM_MAX) ||
        (section >= MEM_MONITOR_SECTION_MAX)) {
        return false;
    }

    if (((uint32_t)mode & HAL_MEM_MONITOR_MODE_READ) != 0) {
        hal_mem_monitor_reg_addr_en_r(monitor, section);
    }

    if (((uint32_t)mode & HAL_MEM_MONITOR_MODE_WRITE) != 0) {
        hal_mem_monitor_reg_addr_en_w(monitor, section);
    }

    return true;
}

static void hal_mem_monitor_v150_set_interrupt_report_delay(mem_monitor_t monitor, uint16_t delay)
{
    hal_mem_monitor_reg_delay_interrupt(monitor, delay);
}

static void hal_mem_monitor_v150_set_callback(mem_monitor_t monitor,
                                              hal_mem_monitor_callback_t mem_monitor_callback)
{
    g_hal_mem_monitor_callbacks[monitor] = mem_monitor_callback;
}

static hal_mem_monitor_funcs_t g_hal_mem_monitor_v150_funcs = {
    .init = hal_mem_monitor_v150_init,
    .deinit = hal_mem_monitor_v150_deinit,
    .enable = hal_mem_monitor_v150_enable,
    .disable = hal_mem_monitor_v150_disable,
    .clock_enable = hal_mem_monitor_v150_clock_enable,
    .clock_disable = hal_mem_monitor_v150_clock_disable,
    .set_section = hal_mem_monitor_v150_set_section,
    .set_section_mode = hal_mem_monitor_v150_set_section_mode,
    .set_interrupt_report_delay = hal_mem_monitor_v150_set_interrupt_report_delay,
    .set_callback = hal_mem_monitor_v150_set_callback,
};

hal_mem_monitor_funcs_t *hal_mem_monitor_v150_funcs_get(void)
{
    return &g_hal_mem_monitor_v150_funcs;
}

void hal_mem_monitor_irq_handler(void)
{
    uint16_t interrupt_status[MEM_MONITOR_NUM_MAX] = {0};
    uint32_t addr = 0;
    uint32_t mode = 0;

    for (uint32_t i = 0; i < MEM_MONITOR_NUM_MAX; i++) {
        interrupt_status[i] = hal_mem_monitor_reg_int_sts_get(i);
        uint16_t capture_master_id = hal_mem_monitor_reg_id_capture(i);
        uint16_t capture_addr_low  = hal_mem_monitor_reg_addr_l_capture(i);
        uint16_t capture_addr_high = hal_mem_monitor_reg_addr_h_capture(i);
        addr = makeu32(capture_addr_low, capture_addr_high);
        mode = 1 << ((uint32_t)capture_master_id & 1);
#if defined(CONFIG_MEM_MONITOR_DEBUG)
        osal_printk("Int %d Status:%#x\n", i, interrupt_status[i]);
        osal_printk("ID: 0%#x\n", capture_master_id);
        osal_printk("ADDR: %#x\n", addr);
#endif /* CONFIG_MEM_MONITOR_DEBUG */

        if ((g_hal_mem_monitor_callbacks[i]) && (interrupt_status[i] != 0)) {
            g_hal_mem_monitor_callbacks[i](addr, mode);
        }
        hal_mem_monitor_reg_clr_interrupt(i);
    }
}