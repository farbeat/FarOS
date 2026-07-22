/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides rsa v1 regs operation \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-06, Create file. \n
 */
#include <stdio.h>
#include "rsa_porting.h"
#include "hal_rsa_v1_regs_op.h"

rsav1_regs_t *g_rsa_v1_regs = NULL;

int32_t hal_rsa_v1_regs_init(void)
{
    if (rsa_get_base_addr() == 0) {
        return -1;
    }

    g_rsa_v1_regs = (rsav1_regs_t*)rsa_get_base_addr();
    return 0;
}

void hal_rsa_v1_regs_deinit(void)
{
    g_rsa_v1_regs = NULL;
}