/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides SHA256 source code \n
 * Author: @CompanyNameTag \n
 */

#include "sha256.h"
#include "cipher_api.h"


errcode_t uapi_drv_cipher_sha256_start(uint32_t *hash_handle)
{
    uapi_drv_cipher_hash_attr_t hash_attr = { .hash_type = UAPI_DRV_CIPHER_HASH_TYPE_SHA256, };
    return uapi_drv_cipher_hash_start(hash_handle, &hash_attr);
}

errcode_t uapi_drv_cipher_sha256_update(uint32_t hash_handle, const uint8_t *buf, uint32_t len)
{
    uapi_drv_cipher_buf_attr_t src_buf = {
        .phys_addr = (uintptr_t)buf,
        .buf_sec = UAPI_DRV_CIPHER_BUF_NONSECURE,
    };

    return uapi_drv_cipher_hash_update(hash_handle, &src_buf, len);
}

errcode_t uapi_drv_cipher_sha256_finish(uint32_t hash_handle, uint8_t *out, uint32_t *out_len)
{
    return uapi_drv_cipher_hash_finish(hash_handle, out, out_len);
}

errcode_t uapi_drv_cipher_sha256(const uint8_t *buf, uint32_t len, uint8_t *out, uint32_t out_len)
{
    uint32_t hash_handle;
    errcode_t ret = uapi_drv_cipher_sha256_start(&hash_handle);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = uapi_drv_cipher_sha256_update(hash_handle, buf, len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    uint32_t tmp_len = out_len;
    return uapi_drv_cipher_sha256_finish(hash_handle, out, &tmp_len);
}
