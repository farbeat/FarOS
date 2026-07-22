/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides keyslot port template header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#ifndef KEYSLOT_PORTING_H
#define KEYSLOT_PORTING_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_port_km Keyslot
 * @ingroup  drivers_port
 * @{
 */

typedef enum {
    KEYSLOT_STAT_UNLOCK   = 0x0,
    KEYSLOT_STAT_LOCK     = 0x1,
    KEYSLOT_STAT_MAX
} keyslot_stat_t;

typedef struct keyslot_lock_stat {
    uint32_t slot_num;
    keyslot_stat_t lock_flag;
} keyslot_lock_stat_t;

void keyslot_lock_config(const uint32_t slot_num, keyslot_stat_t lock_flag);

keyslot_stat_t get_keyslot_status(uint32_t slot_num);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif