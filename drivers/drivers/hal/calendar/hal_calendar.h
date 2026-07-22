/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hal Calendar \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-03, Create file. \n
 */
#ifndef HAL_CALENDAR_H
#define HAL_CALENDAR_H

#include <stdint.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_calendar_api Hal Calendar API
 * @ingroup  drivers_hal_calendar
 * @{
 */

/**
 * @if Eng
 * @brief  Set timestamp interface for hal Calendar.
 * @param  timestamp timestamp value.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  HAL层设置定时器接口
 * @param  timestamp 时间戳值
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
typedef errcode_t (*hal_calendar_set_alarm_t)(uint64_t timestamp);

/**
 * @if Eng
 * @brief  Get timestamp interface for hal Calendar.
 * @retval timestamp value.
 * @else
 * @brief  HAL层获取定时器接口
 * @retval 时间戳值
 * @endif
 */
typedef uint64_t (*hal_calendar_get_alarm_t)(void);

/**
 * @if Eng
 * @brief  Set timestamp interface for hal Calendar.
 * @param  timestamp timestamp value.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  HAL层设置时间戳接口
 * @param  timestamp 时间戳值
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
typedef errcode_t (*hal_calendar_set_timestamp_t)(uint64_t timestamp);

/**
 * @if Eng
 * @brief  Get timestamp interface for hal Calendar.
 * @retval timestamp value.
 * @else
 * @brief  HAL层获取时间戳接口
 * @retval 时间戳值
 * @endif
 */
typedef uint64_t (*hal_calendar_get_timestamp_t)(void);

/**
 * @if Eng
 * @brief  Interface between Calendar driver and Calendar hal.
 * @else
 * @brief  Driver层Calendar和HAL层Calendar的接口
 * @endif
 */
typedef struct {
    hal_calendar_set_alarm_t       set_alarm;            /*!< @if Eng Set timestamp device interface.
                                                            @else   HAL层设置定时器接口 @endif */
    hal_calendar_get_alarm_t       get_alarm;            /*!< @if Eng Get timestamp device interface.
                                                            @else   HAL层获取定时器接口 @endif */
    hal_calendar_set_timestamp_t   set_timestamp;        /*!< @if Eng Set timestamp device interface.
                                                            @else   HAL层设置时间戳接口 @endif */
    hal_calendar_get_timestamp_t   get_timestamp;        /*!< @if Eng Get timestamp device interface.
                                                            @else   HAL层获取时间戳接口 @endif */
} hal_calendar_funcs_t;

/**
 * @if Eng
 * @brief  Register @ref hal_calendar_funcs_t into the g_hal_calendar_funcs.
 * @param  funs Interface between uart driver and Calendar hal.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  注册 @ref hal_calendar_funcs_t 到 g_hal_calendar_funcs
 * @param  funs Driver层Calendar和HAL层Calendar的接口实例
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t hal_calendar_register_funcs(hal_calendar_funcs_t *funs);

/**
 * @if Eng
 * @brief  Unregister @ref hal_calendar_funcs_t from the g_hal_calendar_funcs.
 * @return ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  从g_hal_calendar_funcs注销 @ref hal_calendar_funcs_t
 * @return ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t hal_calendar_unregister_funcs(void);

/**
 * @if Eng
 * @brief  Get interface between Calendar driver and Calendar hal, see @ref hal_calendar_funcs_t.
 * @return Interface between Calendar driver and Calendar hal, see @ref hal_calendar_funcs_t.
 * @else
 * @brief  获取Driver层Calendar和HAL层Calendar的接口实例，参考 @ref hal_calendar_funcs_t.
 * @return Driver层Calendar和HAL层Calendar的接口实例，参考 @ref hal_calendar_funcs_t.
 * @endif
 */
hal_calendar_funcs_t *hal_calendar_get_funcs(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
