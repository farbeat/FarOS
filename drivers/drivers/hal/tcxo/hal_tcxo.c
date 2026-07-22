/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides HAL tcxo \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */

#include "common_def.h"
#include "hal_tcxo.h"

hal_tcxo_funcs_t *g_hal_tcxo_funcs = NULL;

errcode_t hal_tcxo_register_funcs(hal_tcxo_funcs_t *funcs)
{
    if (funcs == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    g_hal_tcxo_funcs = funcs;

    return ERRCODE_SUCC;
}

errcode_t hal_tcxo_unregister_funcs(void)
{
    g_hal_tcxo_funcs = NULL;
    return ERRCODE_SUCC;
}

hal_tcxo_funcs_t *hal_tcxo_get_funcs(void)
{
    return g_hal_tcxo_funcs;
}