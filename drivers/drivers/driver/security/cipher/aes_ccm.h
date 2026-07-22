/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides aes ccm driver\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#ifndef AES_CCM_H
#define AES_CCM_H

#include <stdint.h>
#include "errcode.h"
#include "symc_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

errcode_t set_ccm_iv(symc_hard_context_t *ctx, const uint8_t *iv, const uint32_t iv_len);
errcode_t set_ccm_aad(symc_hard_context_t *ctx, const uint8_t *aad, const uint32_t aad_len);
errcode_t set_ccm_param(symc_hard_context_t *ctx, const void *param);
errcode_t get_ccm_param(const symc_hard_context_t *ctx, void *param);
errcode_t ccm_get_tag(const symc_hard_context_t *ctx, uint8_t *tag, uint32_t *tag_len);
errcode_t ccm_start(symc_hard_context_t *ctx);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
