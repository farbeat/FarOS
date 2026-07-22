/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides symc crypto driver api\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#ifndef DRV_SYMC_H
#define DRV_SYMC_H

#include <stdint.h>
#include "errcode.h"
#include "symc_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define KM_ID                           0x55
#define DRV_KEYSLOT_MCIPHER_MAX         0x1

#define make_handle(mod, privatedata, chnid) \
    (uint32_t)((((mod) & 0xff) << 24) | ((((privatedata) & 0xff)<< 16)) | (((chnid) & 0xffff)))
#define id_2_handle(id, key)    make_handle(KM_ID, (key), (id))
#define handle_2_modid(handle)  (((handle) >> 24) & 0xff)
#define handle_2_id(handle)     (((handle)) & 0xffff)
#define handle_get_type(handle) (((handle) >> 16) & 0xff)

errcode_t handle_num_check(void);
errcode_t symc_create_handle(uintptr_t *symc_handle);
errcode_t clear_cipher_context(symc_hard_context_t *ctx);
errcode_t symc_destroy_handle(symc_hard_context_t *ctx);
errcode_t check_cipher_context(const symc_hard_context_t *ctx);
errcode_t symc_attach(symc_hard_context_t *ctx, const uint32_t keyslot_id);
errcode_t symc_detach(symc_hard_context_t *ctx);
errcode_t cipher_keyslot_param_check(const uintptr_t hkeyslot);
errcode_t set_iv(symc_hard_context_t *ctx, const uint8_t *iv, const uint32_t iv_len);
void set_common_params(symc_hard_context_t *ctx, const drv_cipher_key_length_t key_len,
    const drv_cipher_bit_width_t bit_width, const uint8_t block_size, const drv_cipher_key_parity_t key_parity);
errcode_t get_iv(const symc_hard_context_t *ctx, uint8_t *iv, const uint32_t iv_len);
errcode_t symc_start(symc_hard_context_t *ctx);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
