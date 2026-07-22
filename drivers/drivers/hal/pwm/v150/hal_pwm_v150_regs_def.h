/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V150 PWM register \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */

#ifndef HAL_PWM_V150_REGS_DEF_H
#define HAL_PWM_V150_REGS_DEF_H

#include <stdint.h>
#include "pwm_porting.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_pwm_v150_regs_def PWM V150 Regs Definition
 * @ingroup  drivers_hal_pwm
 * @{
 */

/**
 * @brief  This union represents the bit fields in the cfg1 register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union pwm_cfg1_data {
    uint32_t d32;                                   /*!< Raw register data. */
    struct {
        uint32_t continue_go             : 1;       /*!< If this bit is set to 1, indicates continuous output,
                                                         the value of repeat_cycle is not concerned. */
        uint32_t cycles                  : 15;      /*!< PWM repeat cycles. */
    } b;                                            /*!< Register bits. */
} pwm_cfg1_data_t;

/**
 * @brief  This union represents the bit fields in the cfg0 register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union pwm_cfg0_data {
    uint32_t d32;                                   /*!< Raw register data. */
    struct {
        uint32_t start                   : 1;       /*!< If this bit is set to 1, PWM will be start. */
        uint32_t stop                    : 1;       /*!< If this bit is set to 1, PWM will be stop. */
        uint32_t refresh                 : 1;       /*!< If this bit is set to 1, PWM length will be refresh. */
        uint32_t reserved                : 1;       /*!< reserved. */
        uint32_t output                  : 1;       /*!< PWM output value. */
        uint32_t work_sts                : 1;       /*!< PWM working status. */
    } b;                                            /*!< Register bits. */
} pwm_cfg0_data_t;

/**
 * @brief  This union represents the bit fields in the interrupt register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union pwm_int_data {
    uint32_t d32;                                   /*!< Raw register data. */
    struct {
        uint32_t pwm0                    : 1;       /*!< PWM device No.0. */
        uint32_t pwm1                    : 1;       /*!< PWM device No.1. */
        uint32_t pwm2                    : 1;       /*!< PWM device No.2. */
        uint32_t pwm3                    : 1;       /*!< PWM device No.3. */
        uint32_t pwm4                    : 1;       /*!< PWM device No.4. */
        uint32_t pwm5                    : 1;       /*!< PWM device No.5. */
    } b;                                            /*!< Register bits. */
} pwm_int_data_t;

/**
 * @brief  Registers associated with PWM.
 */

typedef struct pwm_cfg {
    volatile uint32_t cfg0;                     /*!< cfg0.       <i>Offset: 00h</i>. */
    volatile uint32_t cfg1;                     /*!< cfg1.       <i>Offset: 04h</i>. */
    volatile uint32_t length0_l;                /*!< length0_l.  <i>Offset: 08h</i>. */
    volatile uint32_t length0_h;                /*!< length0_h.  <i>Offset: 0Ch</i>. */
    volatile uint32_t length1_l;                /*!< length1_l.  <i>Offset: 10h</i>. */
    volatile uint32_t length1_h;                /*!< length1_h.  <i>Offset: 14h</i>. */
    volatile uint64_t reserved;                 /*!< length1_h.  <i>Offset: 14h</i>. */
} pwm_cfg_t;

typedef struct pwm_regs {
    volatile pwm_cfg_t pwm_cfg[PWM_MAX_NUMBER];                     /*!< PWM config.    <i>Offset: 00h</i>. */
    volatile uint32_t int_en;                                       /*!< PWM enable.    <i>Offset: C0h</i>. */
    volatile uint32_t int_clr;                                      /*!< PWM clear.     <i>Offset: C4</i>. */
    volatile uint32_t int_sts;                                      /*!< PWM status.    <i>Offset: C8h</i>. */
} pwm_regs_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif