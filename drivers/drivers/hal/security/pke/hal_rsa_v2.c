/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides rsa v2 hal api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#include "hal_rsa_v2.h"
#include <stdio.h>
#include "hal_rsa_v2_regs_op.h"

#define OTASK_LENGTH_1024   0x1f
#define OTASK_LENGTH_2048   0x3f
#define OTASK_LENGTH_4096   0x7f

#define RSA_TRANS_DONE_INT  0x0
#define RSA_NC_FAIL_INT     0x1
#define RSA_AHB_TIMEOUT_INT 0x2
#define RSA_NO_INT          0x3

static hal_rsa_callback_t g_hal_rsa_callback = NULL;

static errcode_t hal_rsa_v2_init(hal_rsa_callback_t callback)
{
    if (hal_rsa_v2_regs_init() != 0) {
        return ERRCODE_PKE_REG_ADDR_INVALID;
    }

    g_hal_rsa_callback = callback;
    return ERRCODE_SUCC;
}

static void hal_rsa_v2_deinit(void)
{
    hal_rsa_v2_regs_deinit();
    g_hal_rsa_callback = NULL;
}

static void hal_rsa_v2_cfg_ahb_timeout(uint16_t ahb_timout_value)
{
    hal_set_rsav2_cfg_ahbm_timeout_waterline_ahbm_timeout_waterline(ahb_timout_value);
}

static void hal_rsa_v2_cfg(rsa_attr_t *rsa_attr)
{
    uint8_t otask_len;
    if (rsa_attr->key_len == RSAV2_KEY_LEN_1024) {
        otask_len = OTASK_LENGTH_1024;
    } else if (rsa_attr->key_len == RSAV2_KEY_LEN_2048) {
        otask_len = OTASK_LENGTH_2048;
    } else {
        otask_len = OTASK_LENGTH_4096;
    }
    hal_rsa_v2_cfg_ahb_timeout(0);
    hal_set_rsav2_otskcfg_attribute_otask_len(otask_len);
    hal_set_rsav2_otskcfg_attribute_plaintext_len_sel(0x0);
    hal_set_rsav2_otskcfg_plaintext_addr_otask_ptext_addr(rsa_attr->ptext_addr);
    hal_set_rsav2_otskcfg_key_addr_otask_key_addr(rsa_attr->key_addr);
    hal_set_rsav2_otskcfg_result_addr_otask_result_addr(rsa_attr->output_addr);
}

static void hal_rsa_v2_start(rsa_attr_t *rsa_attr)
{
    hal_rsa_v2_cfg(rsa_attr);
    hal_set_rsav2_otskcfg_start_otask_start(0x1);
}

bool hal_rsa_v2_is_finish(void)
{
    return (g_rsa_v2_regs->rsav2_rint.b.nc_cipher_done_rint != 0) ||
           (g_rsa_v2_regs->rsav2_rint.b.nc_fail_rint != 0) ||
           (g_rsa_v2_regs->rsav2_rint.b.ahbm_timeout_rint != 0);
}

void hal_rsa_v2_int_clear(void)
{
    hal_set_rsav2_rint_nc_cipher_done_rint(0x1);
    hal_set_rsav2_rint_nc_fail_rint(0x1);
    hal_set_rsav2_rint_ahbm_timeout_rint(0x1);
}

void hal_rsa_v2_int_enable(void)
{
    hal_set_rsav2_intmsk_nc_cipher_done_msk(0x0);
    hal_set_rsav2_intmsk_nc_fail_msk(0x0);
    hal_set_rsav2_intmsk_ahbm_timeout_msk(0x0);
}

void hal_rsa_v2_int_disable(void)
{
    hal_set_rsav2_intmsk_nc_cipher_done_msk(0x1);
    hal_set_rsav2_intmsk_nc_fail_msk(0x1);
    hal_set_rsav2_intmsk_ahbm_timeout_msk(0x1);
}

static void hal_rsa_v2_trigger_irq_callback(void)
{
    if (g_hal_rsa_callback != NULL) {
        g_hal_rsa_callback();
    }
    hal_rsa_v2_int_clear();
}

void hal_rsa_v2_irq_handler(void)
{
    if (hal_rsa_v2_is_finish() != 0) {
        hal_rsa_v2_trigger_irq_callback();
    }
}

static hal_rsa_funcs_t g_hal_rsa_v2_funcs = {
    .init = hal_rsa_v2_init,
    .deinit = hal_rsa_v2_deinit,
    .start = hal_rsa_v2_start,
    .finish = hal_rsa_v2_is_finish
};

hal_rsa_funcs_t *hal_rsa_v2_get_funcs(void)
{
    return &g_hal_rsa_v2_funcs;
}