/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides calendar register operation api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-17, Create file. \n
 */
#include <stdio.h>
#include "calendar_porting.h"
#include "hal_calendar_hw_op.h"

calendar_regs_t *g_calendar_reg = NULL;

errcode_t hal_calendar_reg_init(void)
{
    if (hal_calendar_hw_base_addr_get() == 0) {
        return ERRCODE_CALENDAR_FAILURE;
    }
    g_calendar_reg = (calendar_regs_t *)hal_calendar_hw_base_addr_get();
    return ERRCODE_SUCC;
}

void hal_calendar_reg_deinit(void)
{
    g_calendar_reg = NULL;
}
