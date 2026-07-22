/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 * Description: Provides aes v2 regs operation \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#include "hal_aes_v2_regs_op.h"
#include <stdio.h>
#include "aes_porting.h"

aes_regs_t *g_aes_v2_regs = NULL;

int32_t hal_aes_v2_regs_init(void)
{
    if (aes_get_base_addr() == 0) {
        return -1;
    }

    g_aes_v2_regs = (aes_regs_t*)aes_get_base_addr();
    return 0;
}

void hal_aes_v2_regs_deinit(void)
{
    g_aes_v2_regs = NULL;
}
