/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides mem_monitor driver source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-23, Create file. \n
 */
#include "common_def.h"
#include "hal_mem_monitor.h"
#include "mem_monitor.h"

static hal_mem_monitor_funcs_t *g_hal_funcs = NULL;

errcode_t uapi_mem_monitor_init(void)
{
    mem_monitor_port_register_hal_funcs();
    g_hal_funcs = hal_mem_monitor_get_funcs();
    errcode_t ret = g_hal_funcs->init();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    mem_monitor_port_register_irq();
    return ERRCODE_SUCC;
}

void uapi_mem_monitor_deinit(void)
{
    mem_monitor_port_unregister_irq();
    g_hal_funcs->deinit();
    mem_monitor_port_unregister_hal_funcs();
}

void uapi_mem_monitor_enable(mem_monitor_t monitor, uapi_mem_monitor_callback_t callback)
{
    if (monitor >= MEM_MONITOR_NUM_MAX) {
        return;
    }
    g_hal_funcs->clock_enable(monitor);
    g_hal_funcs->enable(monitor);
    g_hal_funcs->set_callback(monitor, (hal_mem_monitor_callback_t)callback);
}

void uapi_mem_monitor_disable(mem_monitor_t monitor)
{
    if (monitor >= MEM_MONITOR_NUM_MAX) {
        return;
    }
    g_hal_funcs->disable(monitor);
    g_hal_funcs->clock_disable(monitor);
}

errcode_t uapi_mem_monitor_set_section(mem_monitor_t monitor, mem_monitor_config_t *config)
{
    if (!g_hal_funcs->set_section(monitor, config->section, config->addr_start, config->addr_end)) {
        return ERRCODE_MEM_MONITOR_INVALID_PARAMETER;
    }
    (void)g_hal_funcs->set_section_mode(monitor, config->section, (hal_mem_monitor_mode_t)config->mode);

    return ERRCODE_SUCC;
}