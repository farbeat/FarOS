/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides v150 mem_monitor register operation api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-23, Create file. \n
 */
#include "common_def.h"
#include "hal_mem_monitor_v150_regs_op.h"

static void hal_mem_monitor_reg_addr_en_r_set(uint32_t section, mem_monitor_addr_en_data_t *data)
{
    switch (section) {
        case MEM_MONITOR_SECTION_0:
            data->b.r0 = 1;
            break;
        case MEM_MONITOR_SECTION_1:
            data->b.r1 = 1;
            break;
        case MEM_MONITOR_SECTION_2:
            data->b.r2 = 1;
            break;
        case MEM_MONITOR_SECTION_3:
            data->b.r3 = 1;
            break;
        default:
            break;
    }
}

static void hal_mem_monitor_reg_addr_en_w_set(uint32_t section, mem_monitor_addr_en_data_t *data)
{
    switch (section) {
        case MEM_MONITOR_SECTION_0:
            data->b.w0 = 1;
            break;
        case MEM_MONITOR_SECTION_1:
            data->b.w1 = 1;
            break;
        case MEM_MONITOR_SECTION_2:
            data->b.w2 = 1;
            break;
        case MEM_MONITOR_SECTION_3:
            data->b.w3 = 1;
            break;
        default:
            break;
    }
}

void hal_mem_monitor_reg_addr_en_r(uint32_t monitor, uint32_t section)
{
    mem_monitor_addr_en_data_t addr_en;
    addr_en.d32 = MEM_MONITOR_REGS[monitor].addr_en;
    hal_mem_monitor_reg_addr_en_r_set(section, &addr_en);
    MEM_MONITOR_REGS[monitor].addr_en = addr_en.d32;
}

void hal_mem_monitor_reg_addr_en_w(uint32_t monitor, uint32_t section)
{
    mem_monitor_addr_en_data_t addr_en;
    addr_en.d32 = MEM_MONITOR_REGS[monitor].addr_en;
    hal_mem_monitor_reg_addr_en_w_set(section, &addr_en);
    MEM_MONITOR_REGS[monitor].addr_en = addr_en.d32;
}

uint16_t hal_mem_monitor_reg_int_sts_get(uint32_t monitor)
{
    return *(uint16_t *)(uintptr_t)(&MEM_MONITOR_REGS[monitor].int_sts);
}