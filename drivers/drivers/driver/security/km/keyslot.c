/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides keyslot driver api\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#include <stdio.h>
#include "errcode.h"
#include "keyslot_porting.h"
#include "keyslot.h"

#define KM_ID                           0x55
#define DRV_KEYSLOT_MCIPHER_MAX         0x1

#define make_handle(mod, privatedata, chnid) \
    (uint32_t)((((mod) & 0xff) << 24) | ((((privatedata) & 0xff)<< 16)) | (((chnid) & 0xffff)))
#define id_2_handle(id, key)    make_handle(KM_ID, (key), (id))
#define handle_2_modid(handle)  (((handle) >> 24) & 0xff)
#define handle_2_id(handle)     (((handle)) & 0xffff)
#define handle_get_type(handle) (((handle) >> 16) & 0xff)

static errcode_t keyslot_lock_process(const uint32_t slot_num)
{
    keyslot_lock_config(slot_num, KEYSLOT_STAT_LOCK);

    keyslot_stat_t state = get_keyslot_status(slot_num);
    if (state == KEYSLOT_STAT_MAX) {
        return ERRCODE_FAIL;
    }

    return ERRCODE_SUCC;
}

static errcode_t keyslot_lock(uint32_t *slot_num)
{
    uint32_t start_slot;
    for (start_slot = 0; start_slot < DRV_KEYSLOT_MCIPHER_MAX; start_slot++) {
        if (get_keyslot_status(start_slot) != KEYSLOT_STAT_UNLOCK) {
            continue;
        }
        if (keyslot_lock_process(start_slot) != ERRCODE_SUCC) {
            continue;
        }
        *slot_num = start_slot;
        return ERRCODE_SUCC;
    }
    return ERRCODE_KM_KEYSLOT_BUSY;
}

static errcode_t keyslot_unlock_process(const uint32_t slot_num)
{
    if (get_keyslot_status(slot_num) == KEYSLOT_STAT_UNLOCK) {
        return ERRCODE_SUCC;
    }

    keyslot_lock_config(slot_num, KEYSLOT_STAT_UNLOCK);

    return ERRCODE_SUCC;
}

errcode_t drv_keyslot_create(uintptr_t *hkeyslot, drv_keyslot_type_t keyslot_type)
{
    if (hkeyslot == NULL) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    if (keyslot_type >= DRV_KEYSLOT_TYPE_MAX || keyslot_type < DRV_KEYSLOT_TYPE_MCIPHER) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    if (keyslot_type != DRV_KEYSLOT_TYPE_MCIPHER) {
        return ERRCODE_KM_UNSUPPORTED;
    }

    errcode_t ret;
    uint32_t slot_num = 0;
    ret = keyslot_lock(&slot_num);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    *hkeyslot = id_2_handle(slot_num, (uint32_t)keyslot_type);
    return ERRCODE_SUCC;
}

errcode_t drv_keyslot_destroy(uintptr_t hkeyslot)
{
    uint32_t keyslot_num = 0;
    drv_keyslot_type_t keyslot_type = DRV_KEYSLOT_TYPE_MAX;

    if (handle_2_modid(hkeyslot) != KM_ID) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    keyslot_type = handle_get_type(hkeyslot);
    if (keyslot_type != DRV_KEYSLOT_TYPE_MCIPHER) {
        return ERRCODE_KM_UNSUPPORTED;
    }

    keyslot_num = handle_2_id(hkeyslot);
    if (keyslot_num >= DRV_KEYSLOT_MCIPHER_MAX) {
        return ERRCODE_KM_INVALID_PARAMETER;
    }

    return keyslot_unlock_process(keyslot_num);
}