/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides Calendar hardware definition \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-05, Create file. \n
 */
#ifndef HAL_CALENDAR_HW_DEF_H
#define HAL_CALENDAR_HW_DEF_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_calendar_hw_def Hal Calendar Hardware Definition
 * @ingroup  drivers_hal_calendar
 * @{
 */

/**
 * @brief  Registers associated with Calendar.
 */
typedef struct calendar_regs {
    volatile uint32_t alarm_l;           /*!< @if Eng Low 32 bits of the alarm @else 定时器低32位 @endif */
    volatile uint32_t alarm_h;           /*!< @if Eng High 32 bits of the alarm @else 定时器高32位 @endif */
    volatile uint32_t data_l;           /*!< @if Eng Low 32 bits of the timestamp @else 时间戳低32位 @endif */
    volatile uint32_t data_h;           /*!< @if Eng High 32 bits of the timestamp @else 时间戳高32位 @endif */
    volatile uint32_t cnt_l;            /*!< @if Eng Low 32 bits of the counter @else 计数器低32位 @endif */
    volatile uint32_t cnt_h;            /*!< @if Eng High 32 bits of the timestamp @else 计数器高32位 @endif */
} calendar_regs_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
