/*
 * Copyright (c) CompanyNameMagicTag 2024. All rights reserved.
 *
 * Description:  ANCS Client Test module.
 */
/**
 * @defgroup bluetooth_sample_ble_ancs ANCS TEST API
 * @ingroup  bluetooth
 * @{
 */

#ifndef SAMPLE_BLE_ANCS_H
#define SAMPLE_BLE_ANCS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void ble_ancs_client_at_cmd(int32_t cmd_id, char **params, uint8_t params_cnt);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif