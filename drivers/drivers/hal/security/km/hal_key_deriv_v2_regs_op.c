/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hal key deriv v2 regs operation \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#include <stdio.h>
#include "key_deriv_porting.h"
#include "hal_key_deriv_v2_regs_op.h"

key_deriv_regs_t *g_key_deriv_v2_regs = NULL;

int32_t hal_key_deriv_v2_regs_init(void)
{
    if (key_deriv_get_base_addr() == 0) {
        return -1;
    }
    g_key_deriv_v2_regs = (key_deriv_regs_t*)key_deriv_get_base_addr();
    return 0;
}

void hal_key_deriv_v2_regs_deinit(void)
{
    g_key_deriv_v2_regs = NULL;
}
