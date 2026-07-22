/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides hal mem_monitor \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-23, Create file. \n
 */
#include "common_def.h"
#include "hal_mem_monitor.h"

uintptr_t g_base_addr = NULL;
hal_mem_monitor_funcs_t *g_hal_mem_monitor_funcs = NULL;

errcode_t hal_mem_monitor_regs_init(void)
{
    if (mem_monitor_port_base_addr_get() == 0) {
        return ERRCODE_MEM_MONITOR_REG_ADDR_INVALID;
    }

    g_base_addr = mem_monitor_port_base_addr_get();

    return ERRCODE_SUCC;
}

void hal_mem_monitor_regs_deinit(void)
{
    g_base_addr = 0;
}

errcode_t hal_mem_monitor_register_funcs(hal_mem_monitor_funcs_t *funcs)
{
    if (funcs == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    g_hal_mem_monitor_funcs = funcs;

    return ERRCODE_SUCC;
}

errcode_t hal_mem_monitor_unregister_funcs(void)
{
    g_hal_mem_monitor_funcs = NULL;
    return ERRCODE_SUCC;
}

hal_mem_monitor_funcs_t *hal_mem_monitor_get_funcs(void)
{
    return g_hal_mem_monitor_funcs;
}