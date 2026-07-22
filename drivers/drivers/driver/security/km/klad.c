/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides klad driver api\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#include "securec.h"
#include "soc_osal.h"
#include "systick.h"
#include "common_def.h"
#include "keyslot.h"
#include "klad_porting.h"
#include "key_deriv.h"
#include "symc.h"
#include "klad.h"

#define KLAD_DEST_TYPE_INVALID_KEYSLOT   0xffffffff
#define ATTACH_MCIPHER                   3
#define KLAD_DEST_TYPE_INVALID           0
#define KLAD_DEST_TYPE_MCIPHER           1

#define KEY_SIZE_64_BIT                  8
#define KEY_SIZE_128_BIT                 16
#define KEY_SIZE_192_BIT                 24
#define KEY_SIZE_256_BIT                 32
#define SBRK_DISABLE_VAL                 0x5
#define ABRK_DISABLE_VAL                 0x5

#define DERIV_PARAM_SIZE                 16
#define DERIV_PARAM_ALIGN                16

#define KLAD_LOCK_REG_CONFIG_VALUE       1
#define KLAD_UNLOCK_REG_CONFIG_VALUE     1
#define KLAD_INVALID_HANDLE_INDEX        (-1)

#define KM_ID                            0x55
#define DRV_KEYSLOT_MCIPHER_MAX          0x1

#define KEY_DERIV_ITER_COUNT             1024

#define make_handle(mod, privatedata, chnid) \
    (uint32_t)((((mod) & 0xff) << 24) | ((((privatedata) & 0xff)<< 16)) | (((chnid) & 0xffff)))
#define id_2_handle(id, key)    make_handle(KM_ID, (key), (id))
#define handle_2_modid(handle)  (((handle) >> 24) & 0xff)
#define handle_2_id(handle)     (((handle)) & 0xffff)
#define handle_get_type(handle) (((handle) >> 16) & 0xff)


typedef struct {
    uint8_t port_sel;
    uint8_t hklad_init;
    uint8_t hklad_attach_init;
    uint8_t hklad_set_attr_init;
    uint32_t klad_dest_type;
    uint32_t hkeyslot;
    drv_klad_attr_t klad_attr;
} klad_handle_info_t;

klad_handle_info_t *g_attach_klad[KLAD_CHANNEL_MAX_SUPPORT] = {NULL};

void *osal_kmalloc(unsigned long size, unsigned int osal_gfp_flag);
void osal_kfree(void *addr);

static int32_t klad_get_handle_index(const klad_handle_info_t *handle)
{
    int32_t i;
    for (i = 0; i < KLAD_CHANNEL_MAX_SUPPORT; ++i) {
        if (handle == g_attach_klad[i]) {
            return i;
        }
    }

    return KLAD_INVALID_HANDLE_INDEX;
}

errcode_t klad_check_handle_is_valid(const klad_handle_info_t *handle)
{
    int32_t index = klad_get_handle_index(handle);
    if (index == KLAD_INVALID_HANDLE_INDEX) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    return ERRCODE_SUCC;
}

errcode_t klad_general_logic_param_check(const klad_handle_info_t *klad_attr)
{
    if (klad_attr->klad_attr.key_cfg.engine >= DRV_CRYPTO_ALG_MAX ||
        klad_attr->klad_attr.key_cfg.engine < DRV_CRYPTO_ALG_AES) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    return ERRCODE_SUCC;
}

errcode_t klad_creat_handle(uintptr_t *hklad)
{
    errcode_t ret;
    int32_t index;
    klad_handle_info_t *klad_info = NULL;

    index = klad_get_handle_index(NULL);
    if (index == KLAD_INVALID_HANDLE_INDEX) {
        *hklad = (uintptr_t)NULL;
        return ERRCODE_FAIL;
    }

    klad_info = (klad_handle_info_t*)osal_kmalloc(sizeof(klad_handle_info_t), 0);
    if (klad_info == NULL) {
        ret = ERRCODE_KM_FAILED_MEM;
        goto exit;
    }

    if (memset_s(klad_info, sizeof(klad_handle_info_t), 0, sizeof(klad_handle_info_t)) != EOK) {
        ret = ERRCODE_MEMSET;
        goto exit;
    }

    klad_info->hklad_init = true;
    klad_info->hklad_attach_init = false;
    klad_info->hklad_set_attr_init = false;

    *hklad = (uintptr_t)klad_info;
    g_attach_klad[index] = klad_info;

    return ERRCODE_SUCC;
exit:
    if (klad_info != NULL) {
        osal_kfree(klad_info);
        klad_info = NULL;
    }
    *hklad = (uint32_t)NULL;

    return ret;
}

static errcode_t klad_attach_keyslot_check_param(drv_klad_dest_type_t dest, uint32_t hkeyslot)
{
    if (dest >= DRV_KLAD_DEST_TYPE_MAX || dest < DRV_KLAD_DEST_TYPE_MCIPHER) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    if (handle_2_modid(hkeyslot) != KM_ID) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    if (handle_get_type(hkeyslot) != DRV_KEYSLOT_TYPE_MCIPHER) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    if (handle_2_id(hkeyslot) >= DRV_KEYSLOT_MCIPHER_MAX) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    return ERRCODE_SUCC;
}

void klad_attach_keyslot(uint32_t hkeyslot, klad_handle_info_t *attach_keyslot)
{
    attach_keyslot->hkeyslot = hkeyslot;
    attach_keyslot->port_sel = KLAD_DEST_TYPE_MCIPHER;
    attach_keyslot->klad_dest_type = DRV_KLAD_DEST_TYPE_MCIPHER;
}

void klad_detach_keyslot(klad_handle_info_t *detach_keyslot)
{
    detach_keyslot->hkeyslot = KLAD_DEST_TYPE_INVALID_KEYSLOT;
    detach_keyslot->klad_dest_type = DRV_KLAD_DEST_TYPE_MAX;
}

errcode_t klad_destroy(klad_handle_info_t *klad_destory)
{
    int32_t index = KLAD_INVALID_HANDLE_INDEX;

    index = klad_get_handle_index(klad_destory);
    if (index == KLAD_INVALID_HANDLE_INDEX) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    if (klad_destory->hklad_init == false) {
        return ERRCODE_KM_ENV_NOT_READY;
    }

    if (memset_s(klad_destory, sizeof(klad_handle_info_t), 0, sizeof(klad_handle_info_t)) != EOK) {
        return ERRCODE_MEMSET;
    }

    klad_destory->hklad_init = false;
    klad_destory->hklad_attach_init = false;
    klad_destory->hklad_set_attr_init = false;

    osal_kfree(klad_destory);
    klad_destory = NULL;
    g_attach_klad[index] = NULL;

    return ERRCODE_SUCC;
}

errcode_t klad_set_clear_key(const drv_klad_clear_key_t *key, const klad_handle_info_t *set_klad_attr)
{
    unused(set_klad_attr);

    if (memcpy_s((uint32_t*)(uintptr_t)KLAD_KEY_ADDR, key->key_size, key->key, key->key_size) != EOK) {
        return ERRCODE_MEMCPY;
    }

    drv_cipher_set_key_sel(DRV_DIPHER_KEY_SOFT);

    return ERRCODE_SUCC;
}

errcode_t drv_klad_create(uintptr_t *hklad)
{
    if (hklad == NULL) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    errcode_t ret = klad_creat_handle(hklad);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

errcode_t drv_klad_destroy(const uintptr_t hklad)
{
    errcode_t ret;
    klad_handle_info_t *klad_destory = (klad_handle_info_t*)hklad;
    if (klad_destory == NULL) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    ret = klad_destroy(klad_destory);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

errcode_t drv_klad_attach(const uintptr_t hklad, const drv_klad_dest_type_t dest, const uintptr_t hkeyslot)
{
    errcode_t ret;
    klad_handle_info_t *klad_attach = (klad_handle_info_t*)hklad;
    if (klad_attach == NULL) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    ret = klad_attach_keyslot_check_param(dest, hkeyslot);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = klad_check_handle_is_valid(klad_attach);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (klad_attach->hklad_init == false) {
        return ERRCODE_KM_ENV_NOT_READY;
    }

    klad_attach_keyslot(hkeyslot, klad_attach);

    if (dest == DRV_KLAD_DEST_TYPE_MCIPHER) {
        klad_attach->hklad_attach_init = ATTACH_MCIPHER;
    }

    return ERRCODE_SUCC;
}

errcode_t drv_klad_detach(const uintptr_t hklad, const drv_klad_dest_type_t dest, const uintptr_t hkeyslot)
{
    errcode_t ret;
    klad_handle_info_t *klad_detach = (klad_handle_info_t*)hklad;
    if (klad_detach == NULL) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    ret = klad_attach_keyslot_check_param(dest, hkeyslot);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = klad_check_handle_is_valid(klad_detach);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (klad_detach->hklad_init == false || klad_detach->hklad_attach_init == false) {
        return ERRCODE_KM_ENV_NOT_READY;
    }

    klad_detach_keyslot(klad_detach);

    klad_detach->hklad_attach_init = false;
    return ERRCODE_SUCC;
}

errcode_t drv_klad_set_attr(const uintptr_t hklad, const drv_klad_attr_t *attr)
{
    errcode_t ret;
    klad_handle_info_t *set_attr = (klad_handle_info_t*)hklad;
    if (attr == NULL || set_attr == NULL) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    if (attr->key_cfg.engine >= DRV_CRYPTO_ALG_MAX || attr->key_cfg.engine < DRV_CRYPTO_ALG_AES) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    if (attr->key_sec_cfg.key_sec >= DRV_KLAD_SEC_MAX || attr->key_sec_cfg.key_sec < DRV_KLAD_SEC_DISABLE) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    ret = klad_check_handle_is_valid(set_attr);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (set_attr->hklad_init == false) {
        return ERRCODE_KM_ENV_NOT_READY;
    }

    if (memcpy_s(&set_attr->klad_attr, sizeof(drv_klad_attr_t), attr, sizeof(drv_klad_attr_t)) != EOK) {
        return ERRCODE_MEMCPY;
    }

    set_attr->hklad_set_attr_init = true;
    return ERRCODE_SUCC;
}

errcode_t drv_klad_get_attr(const uintptr_t hklad, drv_klad_attr_t *attr)
{
    errcode_t ret;
    klad_handle_info_t *get_attr = (klad_handle_info_t*)hklad;
    if (attr == NULL || get_attr == NULL) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    ret = klad_check_handle_is_valid(get_attr);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (get_attr->hklad_init == false) {
        return ERRCODE_KM_ENV_NOT_READY;
    }

    if (memcpy_s(attr, sizeof(drv_klad_attr_t), &get_attr->klad_attr, sizeof(drv_klad_attr_t)) != EOK) {
        return ERRCODE_MEMCPY;
    }

    return ERRCODE_SUCC;
}

errcode_t drv_klad_set_clear_key(const uintptr_t hklad, const drv_klad_clear_key_t *key)
{
    errcode_t ret;
    klad_handle_info_t *set_clear_key = (klad_handle_info_t*)hklad;
    if (key == NULL || set_clear_key == NULL || key->key == NULL) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    ret = klad_check_handle_is_valid(set_clear_key);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = klad_general_logic_param_check(set_clear_key);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (set_clear_key->hklad_init == false || set_clear_key->hklad_attach_init == false ||
        set_clear_key->hklad_set_attr_init == false) {
        return ERRCODE_KM_ENV_NOT_READY;
    }

    if (key->key_parity >= DRV_KLAD_KEY_PARITY_MAX || key->key_parity < DRV_KLAD_KEY_EVEN) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    ret = klad_set_clear_key(key, set_clear_key);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

static uint8_t g_km_deriv_param[DERIV_PARAM_SIZE] __attribute__ ((aligned (DERIV_PARAM_ALIGN)));

errcode_t drv_klad_set_effective_key(const uintptr_t hklad, const drv_klad_effective_key_t *key)
{
    klad_handle_info_t *info = (klad_handle_info_t*)hklad;

    if ((info == NULL) || (key == NULL) || (key->deriv_param == NULL)) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    (void)memcpy_s(g_km_deriv_param, DERIV_PARAM_SIZE, key->deriv_param, min(DERIV_PARAM_SIZE, key->param_len));

    key_deriv_attr_t attr;
    attr.deriv_num = KEY_DERIV_ITER_COUNT;
    attr.key_addr = (uintptr_t)g_km_deriv_param;
    attr.key_sel = (info->klad_attr.klad_cfg.rootkey_type == 1) ? ROOT_KEY_GID : ROOT_KEY_HUK;
    attr.key_id = DERIV_KEY_ID_0;

    key_deriv_init();
    key_deriv_start(&attr);
    (void)key_deriv_get_result(attr.key_id);
    key_deriv_deinit();

    return ERRCODE_SUCC;
}
