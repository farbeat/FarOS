/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides KM driver header \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-06-28, Create file. \n
 */

#include "km.h"
#include "keyslot.h"
#include "klad.h"
#include "common_def.h"


errcode_t uapi_drv_km_init(void)
{
    return ERRCODE_SUCC;
}

errcode_t uapi_drv_km_deinit(void)
{
    return ERRCODE_SUCC;
}

errcode_t uapi_drv_keyslot_create(uint32_t *keyslot_handle, uapi_drv_keyslot_type_t keyslot_type)
{
    drv_keyslot_type_t keyslot;
    switch (keyslot_type) {
        case UAPI_DRV_KEYSLOT_TYPE_MCIPHER:
            keyslot = DRV_KEYSLOT_TYPE_MCIPHER;
            break;

        case UAPI_DRV_KEYSLOT_TYPE_HMAC:
            keyslot = DRV_KEYSLOT_TYPE_HMAC;
            break;

        default:
            keyslot = DRV_KEYSLOT_TYPE_MAX;
            break;
    }

    return drv_keyslot_create((uintptr_t *)(uintptr_t)keyslot_handle, keyslot);
}

errcode_t uapi_drv_keyslot_destroy(uint32_t keyslot_handle)
{
    return drv_keyslot_destroy((uintptr_t)keyslot_handle);
}

errcode_t uapi_drv_klad_create(uint32_t *klad_handle)
{
    return drv_klad_create((uintptr_t *)(uintptr_t)klad_handle);
}

errcode_t uapi_drv_klad_destroy(uint32_t klad_handle)
{
    return drv_klad_destroy((uintptr_t)klad_handle);
}

static drv_klad_dest_type_t covert_drv_klad_dest_type(uapi_drv_klad_dest_t klad_type)
{
    drv_klad_dest_type_t klad_dest_type;

    switch (klad_type) {
        case UAPI_DRV_KLAD_DEST_MCIPHER:
            klad_dest_type = DRV_KLAD_DEST_TYPE_MCIPHER;
            break;

        case UAPI_DRV_KLAD_DEST_HMAC:
            klad_dest_type = DRV_KLAD_DEST_TYPE_HMAC;
            break;

        case UAPI_DRV_KLAD_DEST_FLASH:
            klad_dest_type = DRV_KLAD_DEST_TYPE_FLASH;
            break;

        default:
            klad_dest_type = DRV_KLAD_DEST_TYPE_MAX;
            break;
    }

    return klad_dest_type;
}

errcode_t uapi_drv_klad_attach(uint32_t klad_handle, uapi_drv_klad_dest_t klad_type, uint32_t keyslot_handle)
{
    drv_klad_dest_type_t klad_dest_type = covert_drv_klad_dest_type(klad_type);
    return drv_klad_attach((uintptr_t)klad_handle, klad_dest_type, (uintptr_t)keyslot_handle);
}

errcode_t uapi_drv_klad_detach(uint32_t klad_handle, uapi_drv_klad_dest_t klad_type, uint32_t keyslot_handle)
{
    drv_klad_dest_type_t klad_dest_type = covert_drv_klad_dest_type(klad_type);
    return drv_klad_detach((uintptr_t)klad_handle, klad_dest_type, (uintptr_t)keyslot_handle);
}

errcode_t uapi_drv_klad_set_attr(uint32_t klad_handle, const uapi_drv_klad_attr_t *attr)
{
    if (attr == NULL) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    drv_klad_attr_t new_attr;

    new_attr.klad_cfg.rootkey_type = (attr->klad_cfg.rootkey_type == UAPI_DRV_KDF_HARD_KEY_TYPE_ABRK_REE) ? 1 : 0;

    new_attr.key_cfg.engine =
        (attr->key_cfg.engine == UAPI_DRV_KLAD_ENGINE_AES) ? DRV_CRYPTO_ALG_AES : DRV_CRYPTO_ALG_MAX;
    new_attr.key_cfg.decrypt_support = attr->key_cfg.decrypt_support;
    new_attr.key_cfg.encrypt_support = attr->key_cfg.encrypt_support;

    new_attr.key_sec_cfg.key_sec = (attr->key_sec_cfg.key_sec) ? DRV_KLAD_SEC_ENABLE : DRV_KLAD_SEC_DISABLE;
    new_attr.key_sec_cfg.master_only_enable       = attr->key_sec_cfg.master_only_enable;
    new_attr.key_sec_cfg.dest_buf_sec_support     = attr->key_sec_cfg.dest_buf_sec_support;
    new_attr.key_sec_cfg.dest_buf_non_sec_support = attr->key_sec_cfg.dest_buf_non_sec_support;
    new_attr.key_sec_cfg.src_buf_sec_support      = attr->key_sec_cfg.src_buf_sec_support;
    new_attr.key_sec_cfg.src_buf_non_sec_support  = attr->key_sec_cfg.src_buf_non_sec_support;

    return drv_klad_set_attr((uintptr_t)klad_handle, &new_attr);
}

errcode_t uapi_drv_klad_get_attr(uint32_t klad_handle, uapi_drv_klad_attr_t *attr)
{
    drv_klad_attr_t new_attr;
    errcode_t ret = drv_klad_get_attr((uintptr_t)klad_handle, &new_attr);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    attr->klad_cfg.rootkey_type = (new_attr.klad_cfg.rootkey_type == 0) ?
        UAPI_DRV_KDF_HARD_KEY_TYPE_ABRK_REE : UAPI_DRV_KDF_HARD_KEY_TYPE_RDRK_REE;

    attr->key_cfg.engine = (new_attr.key_cfg.engine == DRV_CRYPTO_ALG_AES) ?
        UAPI_DRV_KLAD_ENGINE_AES : UAPI_DRV_KLAD_ENGINE_MAX;
    attr->key_cfg.decrypt_support = new_attr.key_cfg.decrypt_support;
    attr->key_cfg.encrypt_support = new_attr.key_cfg.encrypt_support;

    attr->key_sec_cfg.key_sec = (bool)(new_attr.key_sec_cfg.key_sec == DRV_KLAD_SEC_ENABLE);
    attr->key_sec_cfg.master_only_enable       = new_attr.key_sec_cfg.master_only_enable;
    attr->key_sec_cfg.dest_buf_sec_support     = new_attr.key_sec_cfg.dest_buf_sec_support;
    attr->key_sec_cfg.dest_buf_non_sec_support = new_attr.key_sec_cfg.dest_buf_non_sec_support;
    attr->key_sec_cfg.src_buf_sec_support      = new_attr.key_sec_cfg.src_buf_sec_support;
    attr->key_sec_cfg.src_buf_non_sec_support  = new_attr.key_sec_cfg.src_buf_non_sec_support;

    attr->rkp_sw_cfg = 0;

    return ERRCODE_SUCC;
}

errcode_t uapi_drv_klad_set_effective_key(uint32_t klad_handle, const uapi_drv_klad_effective_key_t *key)
{
    if (key == NULL) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    drv_klad_effective_key_t new_key;
    new_key.deriv_param = key->salt;
    new_key.param_len = key->salt_length;
    new_key.key_size = key->key_size;
    return drv_klad_set_effective_key((uintptr_t)klad_handle, &new_key);
}

errcode_t uapi_drv_klad_set_clear_key(uint32_t klad_handle, const uapi_drv_klad_clear_key_t *key)
{
    if (key == NULL) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    drv_klad_clear_key_t new_key;
    new_key.key = key->key;
    new_key.key_size = key->key_length;
    new_key.key_parity = (key->key_parity) ? DRV_KLAD_KEY_ODD : DRV_KLAD_KEY_EVEN;
    return drv_klad_set_clear_key((uintptr_t)klad_handle, &new_key);
}

errcode_t uapi_drv_kdf_update(uapi_drv_kdf_otp_key_t otp_key, uapi_drv_kdf_update_alg_t alg)
{
    unused(otp_key);
    unused(alg);
    return ERRCODE_KM_UNSUPPORTED;
}
