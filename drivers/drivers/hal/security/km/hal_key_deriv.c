/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides key_deriv v2 regs operation \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#include <stdio.h>
#include "hal_key_deriv.h"

static hal_key_deriv_funcs_t *g_hal_key_deriv_funcs = NULL;

errcode_t hal_key_deriv_register_funcs(hal_key_deriv_funcs_t *funcs)
{
    if (funcs == NULL) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }
    g_hal_key_deriv_funcs = funcs;
    return ERRCODE_SUCC;
}

void hal_key_deriv_unregister_funcs(void)
{
    g_hal_key_deriv_funcs = NULL;
}

hal_key_deriv_funcs_t *hal_key_deriv_get_funcs(void)
{
    return g_hal_key_deriv_funcs;
}
