/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description:  HID Service Test module.
 */

/**
 * @defgroup bluetooth_sample_ble_hid HID TEST API
 * @ingroup  bluetooth
 * @{
 */

#ifndef SAMPLE_BLE_HID_H
#define SAMPLE_BLE_HID_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @if Eng
 * @brief  HID service server at test command.
 * @par Description:HID service server at test command.
 * @attention  NULL
 * @param  [in]  cmd_id     command id
 * @param  [in]  params     parameter
 * @param  [in]  params_cnt parameter number
 * @retval NULL
 * @par Dependency:
 * @li sample_ble_hid.h
 * @else
 * @brief  HID服务服务端测试命令。
 * @par 说明:HID服务服务端测试命令。
 * @attention  NULL
 * @param  [in]  cmd_id     命令编号
 * @param  [in]  params     参数
 * @param  [in]  params_cnt 参数个数
 * @retval NULL
 * @par 依赖:
 * @li sample_ble_hid.h
 * @endif
 */
void ble_at_hid_srv_cmd(uint8_t cmd_id, char **params, uint8_t params_cnt);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif /* SAMPLE_BLE_HID_H */
