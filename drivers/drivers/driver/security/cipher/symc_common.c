/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides cipher driver api\n
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
#include "symc_def.h"
#include "aes_porting.h"
#include "klad_porting.h"
#include "symc_common.h"

static uint32_t g_cipher_context_count = 0;
static symc_hard_context_t *g_cipher_context_table[CRYPTO_HARD_CHANNEL_MAX] = {NULL};

void *osal_kmalloc(unsigned long size, unsigned int osal_gfp_flag);
void osal_kfree(void *addr);

static void aes_pre_process(void)
{
    aes_port_set_soft_reset(0x1);
    aes_port_set_clk_en(0x1);
    aes_set_endian(AES_BIG_ENDIAN_32BIT);
}

static void aes_post_process(void)
{
    aes_port_set_soft_reset(0x0);
    aes_port_set_clk_en(0x0);
}

static errcode_t aes_init(void)
{
    errcode_t ret;
    aes_pre_process();
    aes_port_register_hal_funcs();
    hal_aes_funcs_t *hal_funcs = aes_port_get_funcs();
    if (hal_funcs == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    ret = hal_funcs->init(NULL);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    return ERRCODE_SUCC;
}

static void aes_deinit(void)
{
    hal_aes_funcs_t *hal_funcs = aes_port_get_funcs();
    hal_funcs->deinit();
    aes_port_unregister_hal_funcs();
    aes_post_process();
}

static void aes_start(aes_attr_t aes_attr)
{
    hal_aes_funcs_t *hal_funcs = aes_port_get_funcs();
    hal_funcs->start(aes_attr);
}

static errcode_t aes_get_result(aes_mode_t work_mode, bool *auth_result)
{
    hal_aes_funcs_t *hal_funcs = aes_port_get_funcs();
    return hal_funcs->get_result(work_mode, auth_result);
}

errcode_t aes_process(aes_attr_t aes_attr, aes_mode_t work_mode)
{
    errcode_t ret;
    bool auth_result;

    aes_init();
    aes_start(aes_attr);

    ret = aes_get_result(work_mode, &auth_result);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    aes_deinit();
    return ret;
}

errcode_t handle_num_check(void)
{
    if (g_cipher_context_count >= CRYPTO_HARD_CHANNEL_MAX) {
        return ERRCODE_CIPHER_BUSY;
    }

    return ERRCODE_SUCC;
}

errcode_t symc_create_handle(uintptr_t *symc_handle)
{
    if (symc_handle == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    errcode_t ret;
    int32_t index;
    symc_hard_context_t *ctx = NULL;

    ret = handle_num_check();
    if (ret != ERRCODE_SUCC) {
        goto exit;
    }

    ctx = (symc_hard_context_t*)osal_kmalloc(sizeof(symc_hard_context_t), 0);
    if (ctx == NULL) {
        ret = ERRCODE_CIPHER_FAILED_MEM;
        goto exit;
    }

    if (memset_s(ctx, sizeof(symc_hard_context_t), 0, sizeof(symc_hard_context_t)) != EOK) {
        ret = ERRCODE_MEMSET;
        goto exit;
    }

    for (index = 0; index < CRYPTO_HARD_CHANNEL_MAX; index++) {
        if (g_cipher_context_table[index] == NULL) {
            g_cipher_context_table[index] = ctx;
            break;
        }
    }

    g_cipher_context_count++;
    *symc_handle = (uintptr_t)ctx;

    return ret;

exit:
    if (ctx != NULL) {
        osal_kfree(ctx);
        ctx = NULL;
    }
    *symc_handle = (uintptr_t)NULL;
    return ret;
}

errcode_t clear_cipher_context(symc_hard_context_t *ctx)
{
    int32_t i;
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if (ctx == g_cipher_context_table[i]) {
            g_cipher_context_count--;
            osal_kfree(ctx);
            g_cipher_context_table[i] = NULL;
            return ERRCODE_SUCC;
        }
    }

    return ERRCODE_CIPHER_INVALID_PARAMETER;
}

errcode_t symc_destroy_handle(symc_hard_context_t *ctx)
{
    errcode_t ret = clear_cipher_context(ctx);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

errcode_t check_cipher_context(const symc_hard_context_t *ctx)
{
    int32_t i;
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if (ctx == g_cipher_context_table[i]) {
            return ERRCODE_SUCC;
        }
    }

    return ERRCODE_CIPHER_INVALID_PARAMETER;
}

errcode_t symc_attach(symc_hard_context_t *ctx, const uint32_t keyslot_id)
{
    ctx->keyslot_id = keyslot_id;

    return ERRCODE_SUCC;
}

errcode_t symc_detach(symc_hard_context_t *ctx)
{
    ctx->keyslot_id = SYMC_KEY_SLOT_INVALID;

    return ERRCODE_SUCC;
}

errcode_t cipher_keyslot_param_check(const uintptr_t hkeyslot)
{
    if (hkeyslot == CIPHER_INVALID_HANDLE) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (handle_2_modid(hkeyslot) != KM_ID) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (handle_get_type(hkeyslot) != DRV_KEYSLOT_TYPE_MCIPHER) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (handle_2_id(hkeyslot) >= DRV_KEYSLOT_MCIPHER_MAX) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    return ERRCODE_SUCC;
}

errcode_t set_iv(symc_hard_context_t *ctx, const uint8_t *iv, const uint32_t iv_len)
{
    if (ctx == NULL || iv == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (ctx->config.alg == DRV_CIPHER_ALG_AES && iv_len != AES_BLOCK_SIZE) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    uint32_t i = 0;
    uint32_t *p = (uint32_t*)iv;
    for (i = 0; i < iv_len / DRV_WORD_WIDTH; i++) {
        ctx->node_context.iv[i] = p[i];
    }

    return ERRCODE_SUCC;
}

void set_common_params(symc_hard_context_t *ctx, const drv_cipher_key_length_t key_len,
    const drv_cipher_bit_width_t bit_width, const uint8_t block_size, const drv_cipher_key_parity_t key_parity)
{
    ctx->config.klen = key_len;
    ctx->config.width = bit_width;
    ctx->config.block_size = block_size;
    ctx->node_context.is_odd_key = (uint8_t)key_parity;
}

errcode_t get_iv(const symc_hard_context_t *ctx, uint8_t *iv, const uint32_t iv_len)
{
    if (ctx == NULL || iv == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (ctx->config.alg == DRV_CIPHER_ALG_AES && iv_len != AES_BLOCK_SIZE) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    uint32_t *p = (uint32_t*)iv;
    if (memcpy_s(p, iv_len / DRV_WORD_WIDTH, ctx->node_context.iv, iv_len / DRV_WORD_WIDTH) != EOK) {
        return ERRCODE_MEMCPY;
    }

    return ERRCODE_SUCC;
}

aes_key_length_t get_aes_key_length(drv_cipher_key_length_t key_length)
{
    if (key_length == DRV_CIPHER_KEY_128BIT) {
        return AES_LENGTH_128;
    } else if (key_length == DRV_CIPHER_KEY_192BIT) {
        return AES_LENGTH_192;
    } else {
        return AES_LENGTH_256;
    }
}

static errcode_t set_aes_common_attr(symc_hard_context_t *ctx, aes_attr_t *aes_attr)
{
    uint32_t len = ctx->node_context.buffer_length;

    aes_attr->data_len = len;
    aes_attr->input_cfg.aes_cfg_para.aes_operation = ctx->operation;
    aes_attr->input_cfg.aes_cfg_para.aes_mode = ctx->node_context.work_mode;
    aes_attr->input_cfg.aes_cfg_para.aes_key_length = get_aes_key_length(ctx->config.klen);
    aes_attr->input_cfg.aes_cfg_para.aes_key_sel = ctx->config.key_sel;

    aes_attr->iv_start_addr = (uint32_t)(uintptr_t)AES_IV_ADDR;
    aes_attr->key_start_addr = (uint32_t)(uintptr_t)KLAD_KEY_ADDR;
    aes_attr->data_in_start_addr = (uint32_t)(uintptr_t)AES_INPUT_ADDR;
    aes_attr->data_out_start_addr = (uint32_t)(uintptr_t)AES_OUTPUT_ADDR;

    if (ctx->node_context.work_mode == DRV_CIPHER_WORK_MODE_CCM ||
        ctx->node_context.work_mode == DRV_CIPHER_WORK_MODE_GCM) {
        aes_attr->aad_len = ctx->node_context.extend.ccm.aad_len;
        aes_attr->aad_start_addr = (uint32_t)(uintptr_t)AES_AAD_ADDR;
        aes_attr->tag_start_addr = (uint32_t)(uintptr_t)AES_TAG_ADDR;
    }

    if (memcpy_s((uint32_t*)(uintptr_t)AES_INPUT_ADDR, len, (uint32_t*)ctx->node_context.src_addr, len) != EOK) {
        return ERRCODE_MEMCPY;
    }

    if (ctx->node_context.work_mode != DRV_CIPHER_WORK_MODE_CCM &&
        ctx->node_context.work_mode != DRV_CIPHER_WORK_MODE_GCM) {
        if (memcpy_s((uint32_t*)(uintptr_t)AES_IV_ADDR, AES_IV_SIZE, ctx->node_context.iv, AES_IV_SIZE) != EOK) {
            return ERRCODE_MEMCPY;
        }
    }

    return ERRCODE_SUCC;
}

errcode_t symc_start(symc_hard_context_t *ctx)
{
    errcode_t ret;
    aes_attr_t aes_attr;
    uint32_t len = ctx->node_context.buffer_length;

    if (memset_s(&aes_attr, sizeof(aes_attr), 0, sizeof(aes_attr)) != EOK) {
        return ERRCODE_MEMSET;
    }

    ret = set_aes_common_attr(ctx, &aes_attr);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = aes_process(aes_attr, (aes_mode_t)ctx->node_context.work_mode);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (memcpy_s((uint32_t*)ctx->node_context.dest_addr, len, (uint32_t*)(uintptr_t)AES_OUTPUT_ADDR, len) != EOK) {
        return ERRCODE_MEMCPY;
    }

    return ERRCODE_SUCC;
}
