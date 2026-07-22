/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides pwm port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */

#ifndef PWM_PORTING_H
#define PWM_PORTING_H

#include <stdint.h>
#include <stdbool.h>


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_pwm PWM
 * @ingroup  drivers_port
 * @{
 */

extern uintptr_t g_pwm_base_addr;

#define PWM_MAX_NUMBER      6  /* < Max number of PWM available */


typedef enum {
    PWM_0,                  /* < PWM Peripheral 0 */
    PWM_1,                  /* < PWM Peripheral 1 */
    PWM_2,                  /* < PWM Peripheral 2 */
#if PWM_MAX_NUMBER > 5
    PWM_3,                  /* < PWM Peripheral 3 */
    PWM_4,                  /* < PWM Peripheral 4 */
    PWM_5,                  /* < PWM Peripheral 4 */
#endif
    PWM_NONE = PWM_MAX_NUMBER
} pwm_channel_t;

/**
 * @brief  Get the base address of a specified PWM.
 * @return The base address of specified PWM.
 */
uintptr_t pwm_porting_base_addr_get(void);

/**
 * @brief  Register hal funcs objects into hal_pwm module.
 */
void pwm_port_register_hal_funcs(void);

/**
 * @brief  Unregister hal funcs objects from hal_pwm module.
 */
void pwm_port_unregister_hal_funcs(void);

/**
 * @brief  Register the interrupt of pwm.
 * @param  [in]  channel PWM device
 */
void pwm_port_register_irq(pwm_channel_t channel);

/**
 * @brief  Unregister the interrupt of pwm.
 * @param  [in]  channel PWM device
 */
void pwm_port_unregister_irq(pwm_channel_t channel);

/**
 * @brief  Set the divider number of the peripheral device clock.
 * @param  [in]  on Enable or disable.
 */
void pwm_port_clock_enable(bool on);

/**
 * @brief  Get pwm clock value.
 * @param  [in]  channel PWM device.
 */
uint32_t pwm_port_get_clock_value(pwm_channel_t channel);

/**
 * @brief  Lock of pwm interrupt.
 * @param  [in]  channel The pwm channel.
 */
uint32_t pwm_irq_lock(uint8_t channel);

/**
 * @brief  Unlock of pwm interrupt.
 * @param  [in]  channel The pwm channel.
 */
void pwm_irq_unlock(uint8_t channel);

/**
 * @brief  Restore of pwm interrupt.
 * @param  [in]  channel The pwm channel.
 */
void pwm_irq_restore(uint8_t channel, uint32_t irq);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif