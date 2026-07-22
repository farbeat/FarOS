/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides cipher driver \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#include <stdio.h>
#include "common_def.h"
#include "systick.h"
#include "keyslot.h"
#include "hal_aes.h"
#include "symc_def.h"
#include "aes_porting.h"
#include "aes_ccm.h"
#include "aes_gcm.h"
#include "symc_crypto.h"

typedef errcode_t (*crypto_start_t)(symc_hard_context_t *ctx);

typedef struct {
    drv_cipher_work_mode_t work_mode;
    crypto_start_t start;
} crypto_start_func_t;

static const crypto_start_func_t g_symc_crypto_start_func_t[] = {
    {DRV_CIPHER_WORK_MODE_GCM, gcm_start},
    {DRV_CIPHER_WORK_MODE_CCM, ccm_start},
};

static errcode_t symc_crypto_start(symc_hard_context_t *ctx)
{
    uint32_t i;
    uint32_t table_len = (uint32_t)(sizeof(g_symc_crypto_start_func_t) / sizeof(crypto_start_func_t));

    for (i = 0; i < table_len; ++i) {
        if (ctx->config.mode == g_symc_crypto_start_func_t[i].work_mode) {
            return g_symc_crypto_start_func_t[i].start(ctx);
        }
    }

    return symc_start(ctx);
}

static errcode_t check_buffer_align(const symc_hard_context_t *ctx, const uint8_t *src_addr,
                                    const uint8_t *dest_addr, const uint32_t length)
{
    if (src_addr == NULL && ctx->config.mode != DRV_CIPHER_WORK_MODE_GCM &&
        ctx->config.mode != DRV_CIPHER_WORK_MODE_CCM) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (dest_addr == NULL && ctx->config.mode != DRV_CIPHER_WORK_MODE_GCM &&
        ctx->config.mode != DRV_CIPHER_WORK_MODE_CCM) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (length == 0 && ctx->config.mode != DRV_CIPHER_WORK_MODE_GCM &&
        ctx->config.mode != DRV_CIPHER_WORK_MODE_CCM) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (ctx->config.alg == DRV_CIPHER_ALG_AES &&
        (ctx->config.mode == DRV_CIPHER_WORK_MODE_GCM || ctx->config.mode == DRV_CIPHER_WORK_MODE_CCM)) {
        if (src_addr == NULL && length != 0) {
            return ERRCODE_CIPHER_INVALID_PARAMETER;
        }
        if (src_addr != NULL && length == 0) {
            return ERRCODE_CIPHER_INVALID_PARAMETER;
        }
        if (src_addr != NULL && length != 0 && dest_addr == NULL) {
            return ERRCODE_CIPHER_INVALID_PARAMETER;
        }
    }

    if ((ctx->config.alg == DRV_CIPHER_ALG_AES) &&
        (ctx->config.mode == DRV_CIPHER_WORK_MODE_ECB || ctx->config.mode == DRV_CIPHER_WORK_MODE_CBC ||
        ctx->config.mode == DRV_CIPHER_WORK_MODE_CBC_MAC)) {
        if ((length % AES_BLOCK_SIZE != 0)) {
            return ERRCODE_CIPHER_SIZE_NOT_ALIGNED;
        }
    }

    return ERRCODE_SUCC;
}

errcode_t symc_crypto(symc_hard_context_t *ctx, const drv_cipher_buff_attr_t *src_buf,
    const drv_cipher_buff_attr_t *dest_buf, const uint32_t length, uint8_t operation)
{
    if (ctx == NULL || src_buf == NULL || dest_buf == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (operation != ENCRYPT && operation != DECRYPT) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    errcode_t ret = check_cipher_context(ctx);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (ctx->keyslot_id == SYMC_KEY_SLOT_INVALID) {
        return ERRCODE_CIPHER_ENV_NOT_READY;
    }

    ret = check_buffer_align(ctx, src_buf->address, dest_buf->address, length);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ctx->operation = operation;
    ctx->node_context.src_addr = (uintptr_t)(src_buf->address);
    ctx->node_context.dest_addr = (uintptr_t)(dest_buf->address);
    ctx->node_context.buffer_length = length;

    ret = symc_crypto_start(ctx);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}