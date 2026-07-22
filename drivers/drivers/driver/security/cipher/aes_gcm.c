/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides aes gcm alg \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#include "common_def.h"
#include "securec.h"
#include "symc_common.h"
#include "aes_porting.h"
#include "aes_gcm.h"

errcode_t set_gcm_iv(symc_hard_context_t *ctx, const uint8_t *iv, const uint32_t iv_len)
{
    unused(ctx);
#if IV_UPDATE_SUPPORT == NO
    if (memcpy_s((uint32_t*)AES_IV_ADDR, iv_len, iv, iv_len) != EOK) {
        return ERRCODE_MEMCPY;
    }
#endif

    return ERRCODE_SUCC;
}

errcode_t set_gcm_aad(symc_hard_context_t *ctx, const uint8_t *aad, const uint32_t aad_len)
{
    unused(ctx);
#if IV_UPDATE_SUPPORT == NO
    if (memcpy_s((uint32_t*)AES_AAD_ADDR, aad_len, aad, aad_len) != EOK) {
        return ERRCODE_MEMCPY;
    }
#endif

    ctx->node_context.extend.gcm.aad_len = aad_len;
    return ERRCODE_SUCC;
}

errcode_t set_gcm_param(symc_hard_context_t *ctx, const void *param)
{
    errcode_t ret;
    drv_cipher_config_aes_gcm_t *gcm_param = (drv_cipher_config_aes_gcm_t*)param;

    set_common_params(ctx, gcm_param->key_len, DRV_CIPHER_BIT_WIDTH_128BIT, DRV_CIPHER_AES_IV_LEN_IN_BYTES,
        gcm_param->key_parity);

#if IV_UPDATE_SUPPORT == NO
    ret = set_gcm_iv(ctx, gcm_param->iv, gcm_param->iv_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = set_gcm_aad(ctx, gcm_param->aad, gcm_param->aad_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
#endif

    return ERRCODE_SUCC;
}

errcode_t get_gcm_param(const symc_hard_context_t *ctx, void *param)
{
    drv_cipher_config_aes_gcm_t *gcm_param = (drv_cipher_config_aes_gcm_t *)param;

    gcm_param->key_len = ctx->config.klen;
    gcm_param->key_parity = ctx->node_context.is_odd_key;
    gcm_param->data_len = ctx->node_context.extend.gcm.updated_len;

    return ERRCODE_SUCC;
}

errcode_t gcm_start(symc_hard_context_t *ctx)
{
    return symc_start(ctx);
}

errcode_t gcm_get_tag(const symc_hard_context_t *ctx, uint8_t *tag, uint32_t *tag_len)
{
    unused(ctx);
    if (tag == NULL || tag_len == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

#if HARDWARE_OUTPUT_TAG == YES
    if (memcpy_s(tag, AES_TAG_SIZE, (uint32_t*)(uintptr_t)AES_TAG_ADDR, AES_TAG_SIZE) != EOK) {
        return ERRCODE_MEMCPY;
    }

    *tag_len = AES_TAG_SIZE;
#endif

    return ERRCODE_SUCC;
}
