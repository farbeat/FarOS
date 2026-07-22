/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides aes ccm alg \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#include "common_def.h"
#include "securec.h"
#include "aes_porting.h"
#include "symc_common.h"
#include "aes_ccm.h"

errcode_t set_ccm_iv(symc_hard_context_t *ctx, const uint8_t *iv, const uint32_t iv_len)
{
    unused(ctx);

    if (memcpy_s((uint8_t*)AES_IV_ADDR, iv_len, iv, iv_len) != EOK) {
        return ERRCODE_MEMCPY;
    }

    return ERRCODE_SUCC;
}

errcode_t set_ccm_aad(symc_hard_context_t *ctx, const uint8_t *aad, const uint32_t aad_len)
{
    unused(ctx);

    if (memcpy_s((uint8_t*)AES_AAD_ADDR, aad_len, aad, aad_len) != EOK) {
        return ERRCODE_MEMCPY;
    }

    ctx->node_context.extend.ccm.aad_len = aad_len;
    return ERRCODE_SUCC;
}

errcode_t set_ccm_param(symc_hard_context_t *ctx, const void *param)
{
    errcode_t ret;
    drv_cipher_config_aes_ccm_t *ccm_param = (drv_cipher_config_aes_ccm_t*)param;

    if (ccm_param == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    set_common_params(ctx, ccm_param->key_len, DRV_CIPHER_BIT_WIDTH_128BIT, DRV_CIPHER_AES_IV_LEN_IN_BYTES,
        ccm_param->key_parity);

    ret = set_ccm_iv(ctx, ccm_param->iv_ctr, ccm_param->iv_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = set_ccm_aad(ctx, ccm_param->aad, ccm_param->aad_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

errcode_t get_ccm_param(const symc_hard_context_t *ctx, void *param)
{
    drv_cipher_config_aes_ccm_t *ccm_param = (drv_cipher_config_aes_ccm_t*)param;

    ccm_param->key_len = ctx->config.klen;
    ccm_param->key_parity = ctx->node_context.is_odd_key;
    ccm_param->tag_len = (uint8_t)ctx->node_context.extend.ccm.tag_len;
    ccm_param->total_len = ctx->node_context.extend.ccm.total_len;

    return ERRCODE_SUCC;
}

errcode_t ccm_start(symc_hard_context_t *ctx)
{
    return symc_start(ctx);
}

errcode_t ccm_get_tag(const symc_hard_context_t *ctx, uint8_t *tag, uint32_t *tag_len)
{
    unused(ctx);
    if (tag == NULL || tag_len == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    if (memcpy_s(tag, AES_TAG_SIZE, (uint32_t*)(uintptr_t)AES_TAG_ADDR, AES_TAG_SIZE) != EOK) {
        return ERRCODE_MEMCPY;
    }

    *tag_len = AES_TAG_SIZE;

    return ERRCODE_SUCC;
}
