/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides Calendar hardware api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01， Create file. \n
 */
#ifndef HAL_CALENDAR_HW_OP_H
#define HAL_CALENDAR_HW_OP_H

#include <stdint.h>
#include "errcode.h"
#include "hal_calendar_hw_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_calendar_hw_op Hal Calendar Hardware Operation
 * @ingroup  drivers_hal_calendar
 * @{
 */

extern calendar_regs_t *g_calendar_reg;

 /**
 * @if Eng
 * @brief  Init register address
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  初始化寄存器地址
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t hal_calendar_reg_init(void);

 /**
 * @if Eng
 * @brief  Deinit register address
 * @else
 * @brief  注销寄存器地址
 * @endif
 */
void hal_calendar_reg_deinit(void);

/**
 * @if Eng
 * @brief  Set alarm_l data
 * @param  val  Low 32 bits of the timestamp
 * @else
 * @brief  设置alarm_l数据
 * @param  val  定时器低32位数据
 * @endif
 */
static inline void hal_calendar_set_alarm_l(uint32_t val)
{
    g_calendar_reg->alarm_l = val;
}

/**
 * @if Eng
 * @brief  Get alarm_l data
 * @retval Low 32 bits of the timestamp
 * @else
 * @brief  获取alarm_l数据
 * @retval 定时器低32位数据
 * @endif
 */
static inline uint32_t hal_calendar_get_alarm_l(void)
{
    return g_calendar_reg->alarm_l;
}

/**
 * @if Eng
 * @brief  Set alarm_h data
 * @param  val  High 32 bits of the timestamp
 * @else
 * @brief  设置alarm_h数据
 * @param  val  定时器高32位数据
 * @endif
 */
static inline void hal_calendar_set_alarm_h(uint32_t val)
{
    g_calendar_reg->alarm_h = val;
}

/**
 * @if Eng
 * @brief  Get alarm_h data
 * @retval High 32 bits of the timestamp
 * @else
 * @brief  获取alarm_h数据
 * @retval 定时器高32位数据
 * @endif
 */
static inline uint32_t hal_calendar_get_alarm_h(void)
{
    return g_calendar_reg->alarm_h;
}

/**
 * @if Eng
 * @brief  Set data_l data
 * @param  val  Low 32 bits of the timestamp
 * @else
 * @brief  设置data_l数据
 * @param  val  时间戳低32位数据
 * @endif
 */
static inline void hal_calendar_set_data_l(uint32_t val)
{
    g_calendar_reg->data_l = val;
}

/**
 * @if Eng
 * @brief  Get data_l data
 * @retval Low 32 bits of the timestamp
 * @else
 * @brief  获取data_l数据
 * @retval 时间戳低32位数据
 * @endif
 */
static inline uint32_t hal_calendar_get_data_l(void)
{
    return g_calendar_reg->data_l;
}

/**
 * @if Eng
 * @brief  Set data_h data
 * @param  val  High 32 bits of the timestamp
 * @else
 * @brief  设置data_h数据
 * @param  val  时间戳高32位数据
 * @endif
 */
static inline void hal_calendar_set_data_h(uint32_t val)
{
    g_calendar_reg->data_h = val;
}

/**
 * @if Eng
 * @brief  Get data_h data
 * @retval High 32 bits of the timestamp
 * @else
 * @brief  获取data_h数据
 * @retval 时间戳高32位数据
 * @endif
 */
static inline uint32_t hal_calendar_get_data_h(void)
{
    return g_calendar_reg->data_h;
}

/**
 * @if Eng
 * @brief  Set cnt_l data
 * @param  val  Low 32 bits of the counter
 * @else
 * @brief  设置cnt_l数据
 * @param  val  计数器低32位数据
 * @endif
 */
static inline void hal_calendar_set_cnt_l(uint32_t val)
{
    g_calendar_reg->cnt_l = val;
}

/**
 * @if Eng
 * @brief  Get cnt_l data
 * @retval Low 32 bits of the counter
 * @else
 * @brief  获取cnt_l数据
 * @retval 计数器低32位数据
 * @endif
 */
static inline uint32_t hal_calendar_get_cnt_l(void)
{
    return g_calendar_reg->cnt_l;
}

/**
 * @if Eng
 * @brief  Set cnt_h data
 * @param  val  High 32 bits of the counter
 * @else
 * @brief  设置cnt_h数据
 * @param  val  计数器高32位数据
 * @endif
 */
static inline void hal_calendar_set_cnt_h(uint32_t val)
{
    g_calendar_reg->cnt_h = val;
}

/**
 * @if Eng
 * @brief  Get cnt_h data
 * @retval High 32 bits of the counter
 * @else
 * @brief  获取cnt_h数据
 * @retval 计数器高32位数据
 * @endif
 */
static inline uint32_t hal_calendar_get_cnt_h(void)
{
    return g_calendar_reg->cnt_h;
}

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
