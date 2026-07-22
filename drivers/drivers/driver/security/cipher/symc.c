/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides cipher driver \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#include <stdio.h>
#include <stdbool.h>
#include "soc_osal.h"
#include "securec.h"
#include "common_def.h"
#include "systick.h"
#include "keyslot.h"
#include "hal_aes.h"
#include "aes_porting.h"
#include "symc_def.h"
#include "aes_gcm.h"
#include "aes_ccm.h"
#include "symc_crypto.h"

typedef errcode_t (*get_alg_param_t)(const symc_hard_context_t *ctx, drv_cipher_work_mode_t mode, void *param);
typedef errcode_t (*set_alg_param_t)(symc_hard_context_t *ctx, drv_cipher_work_mode_t mode, const void *param);

typedef struct {
    drv_cipher_alg_t alg;
    get_alg_param_t get_func;
    set_alg_param_t set_func;
} cipher_alg_func_t;

static uint8_t g_cipher_key_sel = DRV_DIPHER_KEY_HARDWARE;

static errcode_t set_normal_aes_param(symc_hard_context_t *ctx, const drv_cipher_work_mode_t mode, const void *param);
static errcode_t get_normal_aes_param(const symc_hard_context_t *ctx, void *param);
static errcode_t set_aes_param(symc_hard_context_t *ctx, const drv_cipher_work_mode_t mode, const void *param);
static errcode_t get_aes_param(const symc_hard_context_t *ctx, drv_cipher_work_mode_t mode, void *param);
static errcode_t set_cipher_ctrl_params(symc_hard_context_t *ctx, const drv_cipher_config_t *ctrl);
static errcode_t get_cipher_ctrl_params(const symc_hard_context_t *ctx, drv_cipher_config_t *ctrl);

const cipher_alg_func_t g_cipher_param_handle_funcs[] = {
    {DRV_CIPHER_ALG_AES, get_aes_param, set_aes_param},
};

void drv_cipher_set_key_sel(drv_cipher_key_sel_t key_sel)
{
    g_cipher_key_sel = key_sel;
}

static errcode_t set_normal_aes_param(symc_hard_context_t *ctx, const drv_cipher_work_mode_t mode, const void *param)
{
    errcode_t ret;
    drv_cipher_config_aes_t *aes_param = (drv_cipher_config_aes_t*)param;

    if (aes_param->key_len < 0 || aes_param->key_len >= DRV_CIPHER_KEY_LENGTH_MAX) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }
    if (aes_param->key_parity < 0 || aes_param->key_parity >= DRV_CIPHER_KEY_PARITY_MAX) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }
    if (aes_param->key_len != DRV_CIPHER_KEY_128BIT && aes_param->key_len != DRV_CIPHER_KEY_256BIT) {
        return ERRCODE_CIPHER_UNSUPPORTED;
    }
    if (aes_param->bit_width < 0 || aes_param->bit_width >= DRV_CIPHER_BIT_WIDTH_MAX) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }
    if (mode == DRV_CIPHER_WORK_MODE_CFB && aes_param->bit_width != DRV_CIPHER_BIT_WIDTH_8BIT &&
        aes_param->bit_width != DRV_CIPHER_BIT_WIDTH_128BIT) {
        return ERRCODE_CIPHER_UNSUPPORTED;
    }
    if (mode == DRV_CIPHER_WORK_MODE_OFB && aes_param->bit_width != DRV_CIPHER_BIT_WIDTH_128BIT) {
        return ERRCODE_CIPHER_UNSUPPORTED;
    }

    set_common_params(ctx, aes_param->key_len, aes_param->bit_width, DRV_CIPHER_AES_BLOCK_SIZE,
                      aes_param->key_parity);

    ret = set_iv(ctx, aes_param->iv, sizeof(aes_param->iv));
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

static errcode_t set_aes_param(symc_hard_context_t *ctx, const drv_cipher_work_mode_t mode, const void *param)
{
    if (mode == DRV_CIPHER_WORK_MODE_GCM) {
        return set_gcm_param(ctx, param);
    }

    if (mode == DRV_CIPHER_WORK_MODE_CCM) {
        return set_ccm_param(ctx, param);
    }

    return set_normal_aes_param(ctx, mode, param);
}

static errcode_t set_cipher_ctrl_params(symc_hard_context_t *ctx, const drv_cipher_config_t *ctrl)
{
    if (ctrl->alg < 0 || ctrl->alg >= DRV_CIPHER_ALG_MAX) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (ctrl->work_mode < 0 || ctrl->work_mode >= DRV_CIPHER_WORK_MODE_MAX) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    ctx->config.alg = ctrl->alg;
    ctx->config.mode = ctrl->work_mode;
    ctx->config.key_sel = ctrl->key_sel;
    ctx->node_context.work_alg = (uint32_t)ctrl->alg;
    ctx->node_context.work_mode = (uint32_t)ctrl->work_mode;

    uint32_t i;
    uint32_t table_len = (uint32_t)(sizeof(g_cipher_param_handle_funcs) / sizeof(cipher_alg_func_t));
    for (i = 0; i < table_len; ++i) {
        if (ctrl->alg == g_cipher_param_handle_funcs[i].alg) {
            return g_cipher_param_handle_funcs[i].set_func(ctx, ctrl->work_mode, ctrl->param);
        }
    }

    return ERRCODE_CIPHER_UNSUPPORTED;
}

static errcode_t get_cipher_ctrl_params(const symc_hard_context_t *ctx, drv_cipher_config_t *ctrl)
{
    uint32_t i = 0;
    uint32_t table_len = (uint32_t)(sizeof(g_cipher_param_handle_funcs) / sizeof(cipher_alg_func_t));

    ctrl->alg = ctx->config.alg;
    ctrl->work_mode = ctx->config.mode;
    ctrl->dfa = (drv_cipher_dfa_t)(ctx->config.dfa_enable);
    for (i = 0; i < table_len; ++i) {
        if (ctrl->alg == g_cipher_param_handle_funcs[i].alg) {
            return g_cipher_param_handle_funcs[i].get_func(ctx, ctrl->work_mode, ctrl->param);
        }
    }
    return ERRCODE_CIPHER_UNSUPPORTED;
}

errcode_t get_normal_aes_param(const symc_hard_context_t *ctx, void *param)
{
    errcode_t ret;
    drv_cipher_config_aes_t *aes_param = (drv_cipher_config_aes_t*)param;

    aes_param->bit_width = ctx->config.width;
    aes_param->key_len = ctx->config.klen;
    aes_param->key_parity = ctx->node_context.is_odd_key;
    ret = get_iv(ctx, aes_param->iv, sizeof(aes_param->iv));
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

errcode_t get_aes_param(const symc_hard_context_t *ctx, drv_cipher_work_mode_t mode, void *param)
{
    if (mode < 0 || mode >= DRV_CIPHER_WORK_MODE_MAX) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (mode == DRV_CIPHER_WORK_MODE_GCM) {
        return get_gcm_param(ctx, param);
    }

    if (mode == DRV_CIPHER_WORK_MODE_CCM) {
        return get_ccm_param(ctx, param);
    }

    return get_normal_aes_param(ctx, param);
}

errcode_t uapi_drv_cipher_create(uintptr_t *hcipher, const drv_cipher_attr_t *cipher_attr)
{
    if (hcipher == NULL || cipher_attr == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (cipher_attr->cipher_type < 0 || cipher_attr->cipher_type >= DRV_CIPHER_TYPE_MAX) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    errcode_t ret = symc_create_handle(hcipher);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    symc_hard_context_t *ctx = (symc_hard_context_t*)(*hcipher);
    ctx->keyslot_id = SYMC_KEY_SLOT_INVALID;

    return ERRCODE_SUCC;
}

errcode_t uapi_drv_cipher_destroy(const uintptr_t hcipher)
{
    errcode_t final_ret = ERRCODE_FAIL;
    bool is_final_ret_change = false;
    errcode_t ret = ERRCODE_FAIL;
    symc_hard_context_t *ctx = (symc_hard_context_t*)hcipher;

    if (ctx == NULL || hcipher == CIPHER_INVALID_HANDLE) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    ret = check_cipher_context(ctx);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (ctx->keyslot_id != SYMC_KEY_SLOT_INVALID) {
        final_ret = ERRCODE_CIPHER_ENV_NOT_READY;
        is_final_ret_change = true;
    }

    ret = symc_destroy_handle(ctx);
    if (ret != ERRCODE_SUCC && is_final_ret_change == false) {
        final_ret = ret;
        is_final_ret_change = true;
    }

    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return is_final_ret_change == false ? ret : final_ret;
}

errcode_t uapi_drv_cipher_attach(const uintptr_t hcipher, const uintptr_t hkeyslot)
{
    errcode_t ret;
    symc_hard_context_t *ctx = (symc_hard_context_t*)hcipher;
    if (ctx == NULL || hcipher == CIPHER_INVALID_HANDLE) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    ret = cipher_keyslot_param_check(hkeyslot);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = check_cipher_context(ctx);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = symc_attach(ctx, handle_2_id(hkeyslot));
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_drv_cipher_detach(const uintptr_t hcipher, const uintptr_t hkeyslot)
{
    errcode_t ret;
    symc_hard_context_t *ctx = (symc_hard_context_t*)hcipher;
    if (ctx == NULL || hcipher == CIPHER_INVALID_HANDLE) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    ret = cipher_keyslot_param_check(hkeyslot);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = check_cipher_context(ctx);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (ctx->keyslot_id == SYMC_KEY_SLOT_INVALID || ctx->keyslot_id != handle_2_id(hkeyslot)) {
        return ERRCODE_CIPHER_ENV_NOT_READY;
    }

    ret = symc_detach(ctx);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_drv_cipher_set_config(const uintptr_t hcipher, const drv_cipher_config_t *ctrl)
{
    errcode_t ret;
    symc_hard_context_t *ctx = (symc_hard_context_t*)hcipher;
    if (ctx == NULL || hcipher == CIPHER_INVALID_HANDLE) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (ctrl == NULL || ctrl->param == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    ret = check_cipher_context(ctx);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = set_cipher_ctrl_params(ctx, ctrl);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_drv_cipher_get_config(const uintptr_t hcipher, drv_cipher_config_t *ctrl)
{
    errcode_t ret;
    symc_hard_context_t *ctx = (symc_hard_context_t*)hcipher;

    if (ctx == NULL || hcipher == CIPHER_INVALID_HANDLE) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (ctrl == NULL || ctrl->param == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    ret = check_cipher_context(ctx);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = get_cipher_ctrl_params(ctx, ctrl);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ret;
}

errcode_t uapi_drv_cipher_encrypt(const uintptr_t hcipher, const drv_cipher_buff_attr_t *src_buf,
    const drv_cipher_buff_attr_t *dest_buf, const uint32_t length, const uint32_t timeout_ms)
{
    errcode_t ret;
    symc_hard_context_t *ctx = (symc_hard_context_t*)hcipher;
    if (ctx == NULL || hcipher == CIPHER_INVALID_HANDLE) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (ctx->config.mode == DRV_CIPHER_WORK_MODE_CMAC ||
        ctx->config.mode == DRV_CIPHER_WORK_MODE_CBC_MAC) {
        return ERRCODE_CIPHER_UNSUPPORTED;
    }

    ctx->timeout_ms = timeout_ms;
    ctx->config.key_sel = g_cipher_key_sel;

    ret = symc_crypto(ctx, src_buf, dest_buf, length, ENCRYPT);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_drv_cipher_decrypt(const uintptr_t hcipher, const drv_cipher_buff_attr_t *src_buf,
    const drv_cipher_buff_attr_t *dest_buf, const uint32_t length, const uint32_t timeout_ms)
{
    errcode_t ret;
    symc_hard_context_t *ctx = (symc_hard_context_t *)hcipher;
    if (ctx == NULL || hcipher == CIPHER_INVALID_HANDLE) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (ctx->config.mode == DRV_CIPHER_WORK_MODE_CMAC ||
        ctx->config.mode == DRV_CIPHER_WORK_MODE_CBC_MAC) {
        return ERRCODE_CIPHER_UNSUPPORTED;
    }

    ctx->timeout_ms = timeout_ms;
    ctx->config.key_sel = g_cipher_key_sel;

    ret = symc_crypto(ctx, src_buf, dest_buf, length, DECRYPT);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_drv_cipher_get_tag(uintptr_t hcipher, uint8_t *tag, uint32_t *tag_len)
{
    errcode_t ret;
    symc_hard_context_t *ctx = (symc_hard_context_t*)hcipher;

    if (ctx == NULL || hcipher == CIPHER_INVALID_HANDLE) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (tag == NULL || tag_len == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    ret = check_cipher_context(ctx);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (ctx->config.mode != DRV_CIPHER_WORK_MODE_GCM && ctx->config.mode != DRV_CIPHER_WORK_MODE_CCM) {
        return ERRCODE_CIPHER_UNSUPPORTED;
    }

    if (ctx->config.mode == DRV_CIPHER_WORK_MODE_CCM) {
        ret = ccm_get_tag(ctx, tag, tag_len);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
    }

    if (ctx->config.mode == DRV_CIPHER_WORK_MODE_GCM) {
        ret = gcm_get_tag(ctx, tag, tag_len);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
    }

    return ERRCODE_SUCC;
}
