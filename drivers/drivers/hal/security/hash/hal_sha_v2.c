/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hash hal sha drivers. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-05, Create file. \n
 */
#include "errcode.h"
#include "hal_sha_v2_regs_op.h"
#include "hal_sha_v2.h"

#define SHA256_BLOCK_SIZE           64
#define SHA512_BLOCK_SIZE           128

static hal_sha_callback_t g_hal_sha_callback = NULL;

static errcode_t hal_sha_init(hal_sha_callback_t callback)
{
    if (hal_sha_v2_regs_init() != 0) {
        return ERRCODE_HASH_REG_ADDR_INVALID;
    }

    hal_set_sha_ctrl_sha_en(0x1);
    g_hal_sha_callback = callback;
    return ERRCODE_SUCC;
}

static void hal_sha_deinit(void)
{
    hal_set_sha_ivin_start_addr_sha_ivin_start_addr(0x0);
    hal_set_sha_ctrl_sha_en(0x0);
    hal_sha_v2_regs_deinit();
    g_hal_sha_callback = NULL;
}

static void hal_sha_cfg_padding(sha_mode_t sha_mode, uint32_t data_length, bool is_last_package)
{
    uint32_t block_size = (sha_mode == SHA512) ? SHA512_BLOCK_SIZE : SHA256_BLOCK_SIZE;
    if (data_length % block_size != 0 || is_last_package) {
        hal_set_sha_cfg_sha_padding_dis(0x0);
    } else {
        hal_set_sha_cfg_sha_padding_dis(0x1);
    }
}

static void hal_sha_cfg_ivin(bool is_iv_exist, uint32_t iv_addr)
{
    if (is_iv_exist) {
        hal_set_sha_cfg_sha_ivin_sel(0x1);
        hal_set_sha_ivin_start_addr_sha_ivin_start_addr(iv_addr);
    } else {
        hal_set_sha_cfg_sha_ivin_sel(0x0);
    }
}

static void hal_sha_cfg_data(uint32_t input_addr, uint32_t output_addr, uint32_t data_length,
    uint32_t total_length)
{
    hal_set_sha_data_len_sha_data_len(data_length);
    hal_set_sha_data_len_all_sha_data_len_all(total_length);
    hal_set_sha_data_in_start_addr_sha_data_in_start_addr(input_addr);
    hal_set_sha_data_out_start_addr_sha_data_out_start_addr(output_addr);
}

static void hal_sha_cfg_compute_mode(sha_mode_t sha_mode)
{
#if SHA512_SUPPORT == YES
    if (sha_mode == SHA512) {
        hal_set_sha_cfg_sha_compute_mode(0x0);
    } else {
        hal_set_sha_cfg_sha_compute_mode(0x1);
    }
#else
    UNUSED(sha_mode);
#endif
}

static void hal_sha_cfg(sha_attr_t sha_attr)
{
    hal_sha_cfg_padding(sha_attr.sha_mode, sha_attr.data_length, sha_attr.is_last_package);
    hal_sha_cfg_ivin(sha_attr.is_iv_exist, sha_attr.output_addr);
    hal_sha_cfg_data(sha_attr.input_addr, sha_attr.output_addr, sha_attr.data_length, sha_attr.total_length);
    hal_sha_cfg_compute_mode(sha_attr.sha_mode);
}

static void hal_sha_start(sha_attr_t sha_attr)
{
    hal_sha_cfg(sha_attr);
    hal_set_sha_ctrl_sha_start(0x1);
}

static bool hal_sha_finish(void)
{
    return g_sha_v2_regs->sha_ctrl.b.sha_end;
}

static bool hal_sha_get_int_status(void)
{
    return g_sha_v2_regs->sha_int_sts.b.sha_int_sts;
}

void hal_sha_int_clear(void)
{
    hal_set_sha_int_clr(0x1);
}

void hal_sha_int_enable(void)
{
    hal_set_sha_int_en(0x1);
}

void hal_sha_int_disable(void)
{
    hal_set_sha_int_en(0x0);
}

static void hal_sha_trigger_irq_callback(void)
{
    if (g_hal_sha_callback != NULL) {
        g_hal_sha_callback();
    }
    hal_sha_int_clear();
}

void hal_sha_irq_handler(void)
{
    if (hal_sha_get_int_status() != 0) {
        hal_sha_trigger_irq_callback();
    }
}

static hal_sha_funcs_t g_hal_sha_v2_funcs = {
    .init = hal_sha_init,
    .deinit = hal_sha_deinit,
    .start = hal_sha_start,
    .finish = hal_sha_finish
};

hal_sha_funcs_t *hal_sha_v2_get_funcs(void)
{
    return &g_hal_sha_v2_funcs;
}
