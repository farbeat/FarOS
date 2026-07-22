/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides keyslot port template\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "keyslot_porting.h"

#define DRV_KEYSLOT_MCIPHER_MAX         0x1

keyslot_lock_stat_t g_keyslot_lock_stat[DRV_KEYSLOT_MCIPHER_MAX] = {{0, KEYSLOT_STAT_UNLOCK}};

void keyslot_lock_config(const uint32_t slot_num, keyslot_stat_t lock_flag)
{
    int32_t index;
    for (index = 0; index < DRV_KEYSLOT_MCIPHER_MAX; index++) {
        if (slot_num == g_keyslot_lock_stat[index].slot_num) {
            g_keyslot_lock_stat[index].lock_flag = lock_flag;
        }
    }
}

keyslot_stat_t get_keyslot_status(uint32_t slot_num)
{
    int32_t index;
    for (index = 0; index < DRV_KEYSLOT_MCIPHER_MAX; index++) {
        if (slot_num == g_keyslot_lock_stat[index].slot_num) {
            return g_keyslot_lock_stat[index].lock_flag;
        }
    }

    return KEYSLOT_STAT_MAX;
}