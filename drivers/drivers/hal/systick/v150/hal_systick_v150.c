/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Provides V150 HAL systick \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-01, Create file. \n
 */
#include <stdint.h>
#include "hal_systick_v150_regs_op.h"
#include "hal_systick_v150.h"

STATIC void hal_systick_v150_init(void)
{
    hal_systick_v150_regs_init();
}

STATIC void hal_systick_v150_deinit(void)
{
    hal_systick_v150_regs_deinit();
}

STATIC errcode_t hal_systick_v150_ctrl_count_clear(hal_systick_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    unused(param);

    hal_systick_reg_cfg_set_stz(1);
    return ERRCODE_SUCC;
}

STATIC errcode_t hal_systick_v150_ctrl_count_cleared(hal_systick_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    unused(param);

    if (hal_systick_reg_cfg_get_clr() == 1) {
        return ERRCODE_SUCC;
    }
    return ERRCODE_SYSTICK_NOT_CLEARED;
}

STATIC errcode_t hal_systick_v150_ctrl_count_get(hal_systick_ctrl_id_t id, uintptr_t param)
{
    unused(id);

    uint64_t *count = (uint64_t *)param;
    *count = hal_systick_reg_count_get();
    return ERRCODE_SUCC;
}

STATIC hal_systick_ctrl_t g_hal_systick_ctrl_func_array[SYSTICK_CTRL_MAX] = {
    hal_systick_v150_ctrl_count_clear,
    hal_systick_v150_ctrl_count_cleared,
    hal_systick_v150_ctrl_count_get,
};

STATIC errcode_t hal_systick_v150_ctrl(hal_systick_ctrl_id_t id, uintptr_t param)
{
    return g_hal_systick_ctrl_func_array[id](id, param);
}

STATIC hal_systick_funcs_t g_hal_systick_v150_funcs = {
    .init = hal_systick_v150_init,
    .deinit = hal_systick_v150_deinit,
    .ctrl = hal_systick_v150_ctrl
};

hal_systick_funcs_t *hal_systick_v150_funcs_get(void)
{
    return &g_hal_systick_v150_funcs;
}