/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides HAL systick \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-01, Create file. \n
 */
#include "common_def.h"
#include "hal_systick.h"

hal_systick_funcs_t *g_hal_systicks_funcs;

errcode_t hal_systick_register_funcs(hal_systick_funcs_t *funcs)
{
    if (funcs == NULL) {
        return ERRCODE_INVALID_PARAM;
    }
    g_hal_systicks_funcs = funcs;
    return ERRCODE_SUCC;
}

errcode_t hal_systick_unregister_funcs(void)
{
    g_hal_systicks_funcs = NULL;
    return ERRCODE_SUCC;
}

hal_systick_funcs_t *hal_systick_get_funcs(void)
{
    return g_hal_systicks_funcs;
}
