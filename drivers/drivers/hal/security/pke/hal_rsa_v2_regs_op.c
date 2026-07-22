/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides rsa v2 regs operation \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#include <stdio.h>
#include "rsa_porting.h"
#include "hal_rsa_v2_regs_op.h"

rsa_regs_t *g_rsa_v2_regs = NULL;

int32_t hal_rsa_v2_regs_init(void)
{
    if (rsa_get_base_addr() == 0) {
        return -1;
    }

    g_rsa_v2_regs = (rsa_regs_t*)rsa_get_base_addr();
    return 0;
}

void hal_rsa_v2_regs_deinit(void)
{
    g_rsa_v2_regs = NULL;
}