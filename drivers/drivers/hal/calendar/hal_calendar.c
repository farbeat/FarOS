/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hal calendar \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-03, Create file. \n
 */
#include <stdio.h>
#include "hal_calendar.h"

hal_calendar_funcs_t *g_hal_calendar_funcs = NULL;

errcode_t hal_calendar_register_funcs(hal_calendar_funcs_t *funs)
{
    if (funs == NULL) {
        return ERRCODE_INVALID_PARAM;
    }
    g_hal_calendar_funcs = funs;
    return ERRCODE_SUCC;
}

errcode_t hal_calendar_unregister_funcs(void)
{
    g_hal_calendar_funcs = NULL;
    return ERRCODE_SUCC;
}

hal_calendar_funcs_t *hal_calendar_get_funcs(void)
{
    return g_hal_calendar_funcs;
}
