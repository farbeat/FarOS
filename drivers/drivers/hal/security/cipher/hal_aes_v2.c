/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hal aes v2 api\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-09, Create file. \n
 */
#include "hal_aes_v2.h"
#include <stdio.h>
#include "hal_aes_v2_regs_op.h"
#include "systick.h"
#include "aes_porting.h"

#define AES_TIMEOUT_MS       500ULL

static hal_aes_callback_t g_hal_aes_callback = NULL;

static errcode_t hal_aes_v2_init(hal_aes_callback_t callback)
{
    if (hal_aes_v2_regs_init() != 0) {
        return ERRCODE_CIPHER_REG_ADDR_INVALID;
    }

    hal_set_aes_ctrl_aes_en(0x1);
    g_hal_aes_callback = callback;
    return ERRCODE_SUCC;
}

static void hal_aes_v2_deinit(void)
{
    hal_set_aes_ctrl_aes_en(0x0);
    hal_aes_v2_regs_deinit();
    g_hal_aes_callback = NULL;
}

void hal_aes_v2_cfg(aes_attr_t aes_attr)
{
    hal_aes_cfg_reg_t aes_cfg = aes_attr.input_cfg;
    hal_set_aes_cfg_aes_decrypt(aes_cfg.aes_cfg_para.aes_operation);
    hal_set_aes_cfg_aes_mode(aes_cfg.aes_cfg_para.aes_mode);
    hal_set_aes_cfg_aes_key_length(aes_cfg.aes_cfg_para.aes_key_length);
    hal_set_aes_cfg_aes_key_sel(aes_cfg.aes_cfg_para.aes_key_sel);
    hal_set_aes_cfg_aes_key_id(aes_cfg.aes_cfg_para.aes_key_id);
    hal_set_aes_data_len_aes_data_len(aes_attr.data_len);

    if (aes_cfg.aes_cfg_para.aes_key_sel == AES_KEY_SOFT) {
        hal_set_aes_key_start_addr(aes_attr.key_start_addr);
    }

    hal_set_aes_iv_start_addr(aes_attr.iv_start_addr);
    hal_set_aes_data_in_start_addr(aes_attr.data_in_start_addr);
    hal_set_aes_data_out_start_addr(aes_attr.data_out_start_addr);

    if (aes_cfg.aes_cfg_para.aes_mode == AES_CCM) {
        hal_set_aes_auth_iv_start_addr(aes_attr.iv_start_addr);
    }

    if ((aes_cfg.aes_cfg_para.aes_mode == AES_CCM) || (aes_cfg.aes_cfg_para.aes_mode == AES_GCM)) {
        hal_set_aes_aad_start_addr(aes_attr.aad_start_addr);
        hal_set_aes_tag_start_addr(aes_attr.tag_start_addr);
    }
}

static void hal_aes_v2_start(aes_attr_t aes_attr)
{
    hal_aes_v2_cfg(aes_attr);
    hal_set_aes_ctrl_aes_start(0x1);
}

static uint32_t hal_aes_v2_is_end(void)
{
    return g_aes_v2_regs->aes_ctrl.b.aes_end;
}

static errcode_t hal_aes_v2_get_result(aes_mode_t aes_mode, bool *auth_result)
{
    if (aes_mode > AES_GCM) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    uint64_t start_time = uapi_systick_get_ms();
    while (hal_aes_v2_is_end() == 0) {
        if ((uapi_systick_get_ms() - start_time) >= AES_TIMEOUT_MS) {
            return ERRCODE_CIPHER_TIMEOUT;
        }
    }

    if ((aes_mode == AES_CCM) || (aes_mode == AES_GCM)) {
        if (g_aes_v2_regs->aes_auth_result.b.aes_auth_success != 0) {
            *auth_result = true;
        } else {
            *auth_result = false;
        }
    }

    return ERRCODE_SUCC;
}

static bool hal_aes_v2_get_int_status(void)
{
    return g_aes_v2_regs->aes_int_sts.b.aes_int_sts;
}

void hal_aes_int_clear(void)
{
    hal_set_aes_int_clr_aes_int_clr(0x1);
}

void hal_aes_int_enable(void)
{
    hal_set_aes_int_en_aes_int_en(0x1);
}

void hal_aes_int_disable(void)
{
    hal_set_aes_int_en_aes_int_en(0x0);
}

static void hal_aes_trigger_irq_callback(void)
{
    if (g_hal_aes_callback != NULL) {
        g_hal_aes_callback();
    }
    hal_aes_int_clear();
}

void hal_aes_irq_handler(void)
{
    if (hal_aes_v2_get_int_status() != 0) {
        hal_aes_trigger_irq_callback();
    }
}

static hal_aes_funcs_t g_hal_aes_v2_funcs = {
    .init = hal_aes_v2_init,
    .deinit = hal_aes_v2_deinit,
    .start = hal_aes_v2_start,
    .get_result = hal_aes_v2_get_result
};

hal_aes_funcs_t *hal_aes_v2_get_funcs(void)
{
    return &g_hal_aes_v2_funcs;
}
