/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides calendar port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-03， Create file. \n
 */
#ifndef CALENDAR_PORTING_H
#define CALENDAR_PORTING_H

#include <stdint.h>
#include "errcode.h"
#include "button_porting.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_calendar Calendar
 * @ingroup  drivers_port
 * @{
 */

/**
 * @brief  Register hal funcs objects into hal_calendar module.
 */
errcode_t calendar_port_register_hal_funcs(void);

/**
 * @brief  Unregister hal funcs objects from hal_calendar module.
 */
errcode_t calendar_port_unregister_hal_funcs(void);

/**
 * @brief  Get hardware register address.
 */
uintptr_t hal_calendar_hw_base_addr_get(void);

/**
 * @brief  Calendar clock calibration enable.
 */
void uapi_calendar_calibration_enable(void);

/**
 * @brief  Calendar Self Heal enable.
 */
void uapi_self_heal_timer_enable(void);
/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif