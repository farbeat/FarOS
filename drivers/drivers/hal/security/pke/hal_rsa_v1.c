/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides rsa v1 hal api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-06, Create file. \n
 */
#include "hal_rsa_v1.h"
#include "hal_rsa_v1_regs_op.h"

static errcode_t hal_rsa_v1_init(hal_rsa_callback_t callback)
{
    unused(callback);
    if (hal_rsa_v1_regs_init() != 0) {
        return ERRCODE_PKE_REG_ADDR_INVALID;
    }

    return ERRCODE_SUCC;
}

static bool hal_rsa_v1_clear_ram(void)
{
    hal_rsa_v1_copy_result();
    hal_rsav1_is_busy();
    hal_set_rsav1_cfg_attribute_key_mode(0x2);
    hal_set_rsav1_cfg_attribute_key_width(0x2);
    hal_set_rsav1_cfg_attribute_data0_clr(0x1);
    hal_set_rsav1_cfg_attribute_data1_clr(0x1);
    hal_set_rsav1_cfg_attribute_data2_clr(0x1);
    hal_set_rsav1_cfg_start();
    hal_rsav1_is_busy();
    return hal_rsav1_ram_is_cleard();
}

static void hal_rsa_v1_deinit(void)
{
    hal_rsa_v1_clear_ram();
    hal_rsa_v1_regs_deinit();
}

static void hal_rsa_v1_cfg(rsa_attr_t *rsa_attr)
{
    uint8_t key_len;
    if (rsa_attr->key_len == RSAV1_KEY_LEN_1024) {
        key_len = 0x0;
    } else if (rsa_attr->key_len == RSAV1_KEY_LEN_2048) {
        key_len = 0x1;
    } else {
        key_len = 0x2;
    }
    hal_rsav1_is_busy();
    hal_set_rsav1_cfg_attribute_key_mode(0x0);
    hal_set_rsav1_cfg_attribute_key_width(key_len);
    hal_set_rsav1_cfg_attribute_data0_clr(0x0);
    hal_set_rsav1_cfg_attribute_data1_clr(0x0);
    hal_set_rsav1_cfg_attribute_data2_clr(0x0);
    hal_set_rsav1_cfg_plaintext_addr(rsa_attr->ptext_addr);
    hal_set_rsav1_cfg_key_addr(rsa_attr->key_addr);
    hal_set_rsav1_cfg_result_addr(rsa_attr->output_addr);
}

static void hal_rsa_v1_start(rsa_attr_t *rsa_attr)
{
    hal_rsa_v1_cfg(rsa_attr);
    hal_set_rsav1_cfg_start();
}

bool hal_rsa_v1_clear_key(void)
{
    hal_rsav1_is_busy();
    hal_set_rsav1_cfg_attribute_key_mode(0x3);
    hal_set_rsav1_cfg_attribute_key_width(0x2);
    hal_set_rsav1_cfg_attribute_data0_clr(0x0);
    hal_set_rsav1_cfg_attribute_data1_clr(0x0);
    hal_set_rsav1_cfg_attribute_data2_clr(0x0);
    hal_set_rsav1_cfg_start();
    hal_rsav1_is_busy();
    return hal_rsav1_key_is_cleard();
}

static bool hal_rsa_v1_is_finish(void)
{
    return (g_rsa_v1_regs->rsa_busy == 0);
}

static hal_rsa_funcs_t g_hal_rsa_v1_funcs = {
    .init = hal_rsa_v1_init,
    .deinit = hal_rsa_v1_deinit,
    .start = hal_rsa_v1_start,
    .finish = hal_rsa_v1_is_finish
};

hal_rsa_funcs_t *hal_rsa_v1_get_funcs(void)
{
    return &g_hal_rsa_v1_funcs;
}