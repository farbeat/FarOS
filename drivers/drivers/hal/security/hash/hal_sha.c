/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hash hal drivers file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-05, Create file. \n
 */
#include <stdio.h>
#include "hal_sha.h"

static hal_sha_funcs_t *g_hal_sha_funcs = NULL;

errcode_t hal_sha_register_funcs(hal_sha_funcs_t *funcs)
{
    if (funcs == NULL) {
        return ERRCODE_INVALID_PARAM;
    }
    g_hal_sha_funcs = funcs;
    return ERRCODE_SUCC;
}

void hal_sha_unregister_funcs(void)
{
    g_hal_sha_funcs = NULL;
}

hal_sha_funcs_t *hal_sha_get_funcs(void)
{
    return g_hal_sha_funcs;
}
