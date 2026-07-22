/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides rsa hal funcs \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#include "stdio.h"
#include "hal_rsa.h"

hal_rsa_funcs_t *g_hal_rsa_funcs = NULL;

errcode_t hal_rsa_register_funcs(hal_rsa_funcs_t *funcs)
{
    if (funcs == NULL) {
        return ERRCODE_INVALID_PARAM;
    }
    g_hal_rsa_funcs = funcs;
    return ERRCODE_SUCC;
}

void hal_rsa_unregister_funcs(void)
{
    g_hal_rsa_funcs = NULL;
}

hal_rsa_funcs_t *hal_rsa_get_funcs(void)
{
    return g_hal_rsa_funcs;
}
