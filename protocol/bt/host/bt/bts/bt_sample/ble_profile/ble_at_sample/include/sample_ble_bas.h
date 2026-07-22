/*
 * Copyright (c) CompanyNameMagicTag 2022. All rights reserved.
 *
 * Description:  BAS Service Test module.
 */

/**
 * @defgroup bluetooth_sample_ble_bas BAS TEST API
 * @ingroup  bluetooth
 * @{
 */

#ifndef SAMPLE_BLE_BAS_H
#define SAMPLE_BLE_BAS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @if Eng
 * @brief  Battery service server at test command.
 * @par Description:Battery service server at test command.
 * @attention  NULL
 * @param  [in]  cmd_id     command id
 * @param  [in]  params     parameter
 * @param  [in]  params_cnt parameter number
 * @retval NULL
 * @par Dependency:
 * @li sample_ble_bas.h
 * @else
 * @brief  电池服务服务端测试命令。
 * @par 说明:电池服务服务端测试命令。
 * @attention  NULL
 * @param  [in]  cmd_id     命令编号
 * @param  [in]  params     参数
 * @param  [in]  params_cnt 参数个数
 * @retval NULL
 * @par 依赖:
 * @li sample_ble_bas.h
 * @endif
 */
void ble_at_bas_srv_cmd(uint8_t cmd_id, char **params, uint8_t params_cnt);

/**
 * @if Eng
 * @brief  Battery service client at test command.
 * @par Description:Battery service client at test command.
 * @attention  NULL
 * @param  [in]  cmd_id     command id
 * @param  [in]  params     parameter
 * @param  [in]  params_cnt parameter number
 * @retval NULL
 * @par Dependency:
 * @li sample_ble_bas.h
 * @else
 * @brief  电池服务客户端测试命令。
 * @par 说明:电池服务客户端测试命令。
 * @attention  NULL
 * @param  [in]  cmd_id     命令编号
 * @param  [in]  params     参数
 * @param  [in]  params_cnt 参数个数
 * @retval NULL
 * @par 依赖:
 * @li sample_ble_bas.h
 * @endif
 */
void ble_at_bas_cln_cmd(uint8_t cmd_id, char **params, uint8_t params_cnt);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif