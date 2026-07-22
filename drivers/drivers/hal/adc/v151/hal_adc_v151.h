/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V151 HAL ADC \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */
#ifndef HAL_ADC_V151_H
#define HAL_ADC_V151_H

#include "hal_adc.h"
#include "hal_adc_v151_regs_op.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_adc_v151 ADC V151
 * @ingroup  drivers_hal_adc
 * @{
 */

/**
 * @if Eng
 * @brief  ADC type infomation.
 * @else
 * @brief  ADC类型信息。
 * @endif
 */
typedef struct {
    adc_channel_t channel;
    hal_adc_channel_type_t channel_type;
    uint32_t scan_fifo_start_addr;
} hal_adc_type_info_t;

/**
 * @if Eng
 * @brief  Get the instance of v151.
 * @return The instance of v151.
 * @else
 * @brief  获取v151实例。
 * @return v151实例。
 * @endif
 */
hal_adc_funcs_t *hal_adc_v151_funcs_get(void);

/**
 * @if Eng
 * @brief  ADC calibration channel set.
 * @param  [in]  ch The ADC channel.
 * @else
 * @brief  ADC通道的校准设置。
 * @param  [in]  ch ADC通道。
 * @endif
 */
void hal_adc_calibration_channel(adc_channel_t ch);

/**
 * @if Eng
 * @brief  Handler of the ADC interrupt request.
 * @else
 * @brief  ADC中断处理函数。
 * @endif
 */
void hal_adc_irq_handler(void);

/**
 * @if Eng
 * @brief  ADC get channel config.
 * @retval ADC type infomation. For details, see @ref hal_adc_type_info_t.
 * @else
 * @brief  ADC获取通道配置。
 * @retval ADC类型信息 参考 @ref hal_adc_type_info_t 。
 * @endif
 */
hal_adc_type_info_t *adc_port_get_cfg(void);

/**
 * @if Eng
 * @brief  ADC get channel config.
 * @param  [in]  channel The ADC channel.
 * @param  [out] reg_val Pointer to save select reg value.
 * @param  [out] ch_buffer Pointer to save ch_buffer.
 * @else
 * @param  [in]  channel ADC通道。
 * @param  [out] reg_val 保存选择寄存器值的指针。
 * @param  [out] ch_buffer 保留缓存的指针。
 * @endif
 */
void adc_port_get_channel_config(adc_channel_t channel, uint32_t *reg_val, hal_adc_buffer_t *ch_buffer);

/**
 * @if Eng
 * @brief  ADC get channel config.
 * @param  [in]  ch The ADC channel.
 * @param  [in]  on Set or clear of ADC channel.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @param  [in]  ch ADC通道。
 * @param  [in]  on 设置或清除通道。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败 参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_adc_v151_channel_set(adc_channel_t ch, bool on);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif