/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides key_deriv driver api\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */

#include "key_deriv.h"
#include "securec.h"
#include "systick.h"
#include "key_deriv_porting.h"
#include "symc.h"


static void key_deriv_pre_process(void)
{
    key_deriv_port_set_soft_reset(0x1);
    key_deriv_port_set_clk_en(0x1);
    key_deriv_set_endian(KEY_DERIV_BIG_ENDIAN_32BIT);
}

static void key_deriv_post_process(void)
{
    key_deriv_port_set_soft_reset(0x0);
    key_deriv_port_set_clk_en(0x0);
}

errcode_t key_deriv_init(void)
{
    errcode_t ret;
    key_deriv_pre_process();
    key_deriv_port_register_hal_funcs();
    hal_key_deriv_funcs_t *hal_funcs = key_deriv_port_get_funcs();
    if (hal_funcs == NULL) {
        return ERRCODE_HASH_INVALID_PARAMETER;
    }

    ret = hal_funcs->init(NULL);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    return ERRCODE_SUCC;
}

void key_deriv_deinit(void)
{
    hal_key_deriv_funcs_t *hal_funcs = key_deriv_port_get_funcs();
    hal_funcs->deinit();
    key_deriv_port_unregister_hal_funcs();
    key_deriv_post_process();
}

void key_deriv_start(key_deriv_attr_t *key_deriv_attr)
{
    hal_key_deriv_funcs_t *hal_funcs = key_deriv_port_get_funcs();
    hal_funcs->start(key_deriv_attr);
    drv_cipher_set_key_sel(DRV_DIPHER_KEY_HARDWARE);
}

bool key_deriv_get_result(deriv_key_id_t key_id)
{
    hal_key_deriv_funcs_t *hal_funcs = key_deriv_port_get_funcs();
    return hal_funcs->get_result(key_id);
}