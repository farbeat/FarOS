/*
 * Copyright (c) @CompanyNameMagicTag 2023. All rights reserved.
 *
 * Description:  FMP Test module.
 */

/**
 * @defgroup bluetooth_sample_ble_fmp FMP TEST API
 * @ingroup  bluetooth
 * @{
 */

#ifndef SAMPLE_BLE_FMP_H
#define SAMPLE_BLE_FMP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @if Eng
 * @brief  Find Me Profile at test command.
 * @par Description:Find Me Profile at test command.
 * @attention  NULL
 * @param  [in]  cmd_id     command id
 * @param  [in]  params     parameter
 * @param  [in]  params_cnt parameter number
 * @retval NULL
 * @par Dependency:
 * @li sample_ble_fmp.h
 * @else
 * @brief  FMP 对外接口测试命令。
 * @par 说明:FMP 对外接口测试命令。
 * @attention  NULL
 * @param  [in]  cmd_id     命令编号
 * @param  [in]  params     参数
 * @param  [in]  params_cnt 参数个数
 * @retval NULL
 * @par 依赖:
 * @li sample_ble_fmp.h
 * @endif
 */
void ble_at_fmp_srv_cmd(int cmd_id, char **params, int params_cnt);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif /* SAMPLE_BLE_FMP_H */
