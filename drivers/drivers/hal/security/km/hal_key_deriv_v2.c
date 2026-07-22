/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides aes v2 regs operation \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#include <stdio.h>
#include "systick.h"
#include "hal_key_deriv.h"
#include "hal_key_deriv_v2_regs_op.h"

#define KEY_DERIV_TIME_OUT 2000
#define BIT(x) (1UL << (uint32_t)(x))

static hal_key_deriv_callback_t g_hal_key_deriv_callback = NULL;

static errcode_t hal_key_deriv_init(hal_key_deriv_callback_t callback)
{
    if (hal_key_deriv_v2_regs_init() != 0) {
        return ERRCODE_KM_REG_ADDR_INVALID;
    }

    hal_set_key_deriv_ctrl_deriv_en(0x1);
    g_hal_key_deriv_callback = callback;
    return ERRCODE_SUCC;
}

static void hal_key_deriv_deinit(void)
{
    hal_set_key_deriv_ctrl_deriv_en(0x0);
    hal_key_deriv_v2_regs_deinit();
    g_hal_key_deriv_callback = NULL;
}

void hal_key_deriv_cfg(key_deriv_attr_t *attr)
{
    if ((attr->key_sel > ROOT_KEY_GID) || (attr->key_id > DERIV_KEY_ID_7)) {
        return;
    }

    hal_set_key_deriv_num_deriv_num(attr->deriv_num);
    if (attr->key_sel == ROOT_KEY_GID) {
        hal_set_key_deriv_cfg_root_key_sel(0x1);
    } else {
        hal_set_key_deriv_cfg_root_key_sel(0x0);
    }

    hal_set_key_deriv_cfg_deriv_key_id(attr->key_id);
    hal_set_deriv_para_start_addr_deriv_para_start_addr(attr->key_addr);
}

void hal_key_deriv_start(key_deriv_attr_t *attr)
{
    hal_key_deriv_cfg(attr);
    hal_set_key_deriv_ctrl_deriv_start(0x1);
}

bool hal_key_deriv_is_finish(void)
{
    return g_key_deriv_v2_regs->key_deriv_ctrl.b.deriv_end;
}

bool hal_key_deriv_get_result(deriv_key_id_t key_id)
{
    if (key_id > DERIV_KEY_ID_7) {
        return false;
    }

    uint64_t start_time = uapi_systick_get_ms();
    while (!hal_key_deriv_is_finish()) {
        if ((uapi_systick_get_ms() - start_time) >= KEY_DERIV_TIME_OUT) {
            return false;
        }
    }

    uint32_t result = g_key_deriv_v2_regs->key_deriv_id_sts.b.deriv_id_sts;
    bool status = (result & BIT(key_id)) > 0 ? true : false;
    return status;
}

static bool hal_key_deriv_get_int_status(void)
{
    return g_key_deriv_v2_regs->key_deriv_int_sts.b.deriv_int_sts;
}

void hal_key_deriv_int_clear(void)
{
    hal_set_key_deriv_int_clr(0x1);
}

void hal_key_deriv_int_enable(void)
{
    hal_set_key_deriv_int_en(0x1);
}

void hal_key_deriv_int_disable(void)
{
    hal_set_key_deriv_int_clr(0x0);
}

static void hal_key_deriv_trigger_irq_callback(void)
{
    if (g_hal_key_deriv_callback != NULL) {
        g_hal_key_deriv_callback();
    }
    hal_key_deriv_int_clear();
}

void hal_key_deriv_irq_handler(void)
{
    if (hal_key_deriv_get_int_status() != 0) {
        hal_key_deriv_trigger_irq_callback();
    }
}

static hal_key_deriv_funcs_t g_hal_key_deriv_v2_funcs = {
    .init = hal_key_deriv_init,
    .deinit = hal_key_deriv_deinit,
    .start = hal_key_deriv_start,
    .get_result = hal_key_deriv_get_result
};

hal_key_deriv_funcs_t *hal_key_deriv_v2_get_funcs(void)
{
    return &g_hal_key_deriv_v2_funcs;
}

