/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides cipher driver header \n
 * Author: @CompanyNameTag \n
 */

#include "cipher_api.h"
#include "drv_hash.h"
#include "common_def.h"


errcode_t uapi_drv_cipher_hash_init(void)
{
    return ERRCODE_SUCC;
}

errcode_t uapi_drv_cipher_hash_deinit(void)
{
    return ERRCODE_SUCC;
}

errcode_t uapi_drv_cipher_hash_start(uint32_t *hash_handle, const uapi_drv_cipher_hash_attr_t *hash_attr)
{
    if (hash_attr == NULL) {
        return ERRCODE_HASH_INVALID_PARAMETER;
    }

    drv_cipher_hash_attr_t attr;
    attr.hash_type = (drv_cipher_hash_type_t)hash_attr->hash_type;

    return drv_hash_start((uintptr_t *)(uintptr_t)hash_handle, &attr);
}

errcode_t uapi_drv_cipher_hash_update(uint32_t hash_handle, const uapi_drv_cipher_buf_attr_t *src_buf,
    const uint32_t len)
{
    drv_cipher_buf_attr_t buf;
    if (src_buf != NULL) {
        buf.address = (uint8_t *)src_buf->phys_addr;
        buf.buf_sec = (drv_cipher_buffer_secure_t)src_buf->buf_sec;
    } else {
        buf.address = NULL;
        buf.buf_sec = 0;
    }

    return drv_hash_update((uintptr_t)hash_handle, &buf, len, 0);
}

errcode_t uapi_drv_cipher_hash_finish(uint32_t hash_handle, uint8_t *out, uint32_t *out_len)
{
    return drv_hash_final((uintptr_t)hash_handle, out, out_len);
}
