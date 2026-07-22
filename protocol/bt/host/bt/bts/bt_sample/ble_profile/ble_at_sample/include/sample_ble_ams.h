/*
 * Copyright (c) CompanyNameMagicTag 2024. All rights reserved.
 *
 * Description:  AMS Client Test module.
 */
/**
 * @defgroup bluetooth_sample_ble_ams AMS TEST API
 * @ingroup  bluetooth
 * @{
 */

#ifndef SAMPLE_BLE_AMS_H
#define SAMPLE_BLE_AMS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void ble_ams_client_at_cmd(int32_t cmd_id, char **params, uint8_t params_cnt);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif