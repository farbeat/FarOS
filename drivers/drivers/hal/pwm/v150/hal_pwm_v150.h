/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V150 HAL PWM \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */
#ifndef HAL_PWM_V150_H
#define HAL_PWM_V150_H

#include "hal_pwm.h"
#include "hal_pwm_v150_regs_op.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_pwm_v150 PWM V150
 * @ingroup  drivers_hal_pwm
 * @{
 */

/**
 * @if Eng
 * @brief  Get the instance of v150.
 * @return The instance of v150.
 * @else
 * @brief  获取v150实例。
 * @return v150实例。
 * @endif
 */
hal_pwm_funcs_t *hal_pwm_v150_funcs_get(void);

/**
 * @if Eng
 * @brief  Handler of the PWM interrupt request.
 * @param  [in]  channel PWM device
 * @else
 * @brief  PWM中断处理函数。
 * @param  [in]  channel PWM设备。
 * @endif
 */
void hal_pwm_irq_handler(pwm_channel_t channel);


/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif