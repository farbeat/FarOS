/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides aes gcm driver\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#ifndef AES_GCM_H
#define AES_GCM_H

#include <stdint.h>
#include "errcode.h"
#include "symc_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

errcode_t set_gcm_iv(symc_hard_context_t *ctx, const uint8_t *iv, const uint32_t iv_len);
errcode_t set_gcm_aad(symc_hard_context_t *ctx, const uint8_t *aad, const uint32_t aad_len);
errcode_t set_gcm_param(symc_hard_context_t *ctx, const void *param);
errcode_t get_gcm_param(const symc_hard_context_t *ctx, void *param);
errcode_t gcm_get_tag(const symc_hard_context_t *ctx, uint8_t *tag, uint32_t *tag_len);
errcode_t gcm_start(symc_hard_context_t *ctx);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
