/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hal sha v2 regs operation \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#include <stdio.h>
#include "sha_porting.h"
#include "hal_sha_v2_regs_op.h"

sha_regs_t *g_sha_v2_regs = NULL;

int32_t hal_sha_v2_regs_init(void)
{
    if (sha_get_base_addr() == 0) {
        return -1;
    }
    g_sha_v2_regs = (sha_regs_t*)sha_get_base_addr();
    return 0;
}

void hal_sha_v2_regs_deinit(void)
{
    g_sha_v2_regs = NULL;
}
