/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides Calendar hardware
  template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-03， Create file. \n
 */
#ifndef HAL_CALENDAR_HW_H
#define HAL_CALENDAR_HW_H

#include "hal_calendar.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_calendar_hw Hal Calendar Hardware
 * @ingroup  drivers_hal_calendar
 * @{
 */

 /**
 * @if Eng
 * @brief  Get hardware object
 * @else
 * @brief  获取硬件实例
 * @endif
 */
hal_calendar_funcs_t *hal_calendar_hw_funcs_get(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif