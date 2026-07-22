/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V150 PWM register operation api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */

#ifndef HAL_PWM_V150_REGS_OP_H
#define HAL_PWM_V150_REGS_OP_H

#include <stdint.h>
#include "errcode.h"
#include "hal_pwm_v150_regs_def.h"
#include "pwm_porting.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_pwm_v150_regs_op PWM V150 Regs Operation
 * @ingroup  drivers_hal_pwm
 * @{
 */

extern uintptr_t g_pwm_regs;

/**
 * @brief  Set the value of @ref pwm_cfg1_data_t.cycles.
 */
static inline void hal_pwm_reg_cfg1_set_sycles(pwm_channel_t channel, uint32_t value)
{
    pwm_cfg1_data_t cfg1;
    cfg1.d32 = ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].cfg1;
    cfg1.b.cycles = value;
    ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].cfg1 = cfg1.d32;
}

/**
 * @brief  Set the value of @ref pwm_cfg1_data_t.continue_go.
 */
static inline void hal_pwm_reg_cfg1_set_continue(pwm_channel_t channel, uint32_t value)
{
    pwm_cfg1_data_t cfg1;
    cfg1.d32 = ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].cfg1;
    cfg1.b.continue_go = value;
    ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].cfg1 = cfg1.d32;
}

/**
 * @brief  Set the value of @ref pwm_cfg0_data_t.start.
 */
static inline void hal_pwm_reg_cfg0_set_start(pwm_channel_t channel)
{
    pwm_cfg0_data_t cfg0;
    cfg0.d32 = ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].cfg0;
    cfg0.b.start = 1;
    ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].cfg0 = cfg0.d32;
}

/**
 * @brief  Set the value of @ref pwm_cfg0_data_t.stop.
 */
static inline void hal_pwm_reg_cfg0_set_stop(pwm_channel_t channel)
{
    pwm_cfg0_data_t cfg0;
    cfg0.d32 = ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].cfg0;
    cfg0.b.stop = 1;
    ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].cfg0 = cfg0.d32;
}

/**
 * @brief  Set the value of @ref pwm_cfg0_data_t.refresh.
 */
static inline void hal_pwm_reg_cfg0_set_refresh(pwm_channel_t channel)
{
    pwm_cfg0_data_t cfg0;
    cfg0.d32 = ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].cfg0;
    cfg0.b.refresh = 1;
    ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].cfg0 = cfg0.d32;
}

/**
 * @brief  Get the value of count.
 * @return  The value of count.
 */
uint64_t hal_pwm_reg_count_get(void);

/**
 * @brief  Set the interrupt register of PWM.
 * @param  [in]  channel PWM device
 */
void hal_pwm_reg_int_clr_set(pwm_channel_t channel);

/**
 * @brief  Set the interrupt register enable of PWM.
 * @param  [in]  channel PWM device
 * @param  [in]  value The value want to set.
 */
void hal_pwm_reg_int_en_set(pwm_channel_t channel, uint32_t value);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif