/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  related to device management.
 * Author: @CompanyNameTag
 * Create:
 */

#ifndef _DEVICE_MANAGER_H_
#define _DEVICE_MANAGER_H_

#include "mpc_dispatch.h"
/*
 * UI and the open of BT protocol stack have relationship with this enum,
 * please modify these two module synchronously when modified this enum.
 */
typedef enum {
    START_UP_NORMAL = 0,
    START_UP_CHARGE, /* last shutdown reason is Button Press */
    START_UP_CHARGE_NORMAL, /* last shutdown reason is BAT_ULOW */
    START_UP_BAT_ULOW, /* Start Up with ULow Bat and PS Offline */
    START_UP_ABNORMAL,
    START_UP_TOUCH_UPGRADE_START, /* TP upgrade start */
    START_UP_TOUCH_UPGRADE_STOP, /* TP upgrade stop */
    START_UP_TYPE_MAX,
} mc_start_up_status;

void mc_bt_dev_info(const mpc_message *param);

uint8_t bt_get_charging_status(void);
uint8_t bt_get_charging_value(void);
void bt_set_ota_status(uint8_t status);
uint8_t bt_get_ota_status(void);
void bt_acore_recover_smp_key(void);
#endif
