/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides symc crypto driver api\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#ifndef SYMC_CRYOTO_H
#define SYMC_CRYOTO_H

#include <stdint.h>
#include "errcode.h"
#include "symc_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

errcode_t symc_crypto(symc_hard_context_t *ctx, const drv_cipher_buff_attr_t *src_buf,
    const drv_cipher_buff_attr_t *dest_buf, const uint32_t length, uint8_t operation);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
