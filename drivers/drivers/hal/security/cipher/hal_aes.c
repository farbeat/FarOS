/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hal aes \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-09, Create file. \n
 */
#include <stdio.h>
#include "hal_aes.h"

static hal_aes_funcs_t *g_hal_aes_funcs = NULL;

errcode_t hal_aes_register_funcs(hal_aes_funcs_t *funcs)
{
    if (funcs == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    g_hal_aes_funcs = funcs;
    return ERRCODE_SUCC;
}

void hal_aes_unregister_funcs(void)
{
    g_hal_aes_funcs = NULL;
}

hal_aes_funcs_t *hal_aes_get_funcs(void)
{
    return g_hal_aes_funcs;
}