/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides efuse driver source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-21, Create file. \n
 */
#include <stdint.h>
#include <stdio.h>
#include "errcode.h"
#include "efuse_porting.h"
#include "hal_efuse_v100_reg_op.h"

efuse_regs_t *g_efuse_v100_regs[EFUSE_REGION_NUM] = { 0 };

static inline uintptr_t hal_efuse_v100_base_addr_get(int32_t i)
{
    return g_efuse_base_addr[i];
}

int32_t hal_efuse_v100_regs_init(void)
{
    for (uint32_t i = 0; i < EFUSE_REGION_NUM; i++) {
        g_efuse_v100_regs[i] = (efuse_regs_t *)hal_efuse_v100_base_addr_get(i);
    }
    return ERRCODE_SUCC;
}

void hal_efuse_v100_regs_deinit(void)
{
    for (uint32_t i = 0; i < EFUSE_REGION_NUM; i++) {
        g_efuse_v100_regs[i] = NULL;
    }
}