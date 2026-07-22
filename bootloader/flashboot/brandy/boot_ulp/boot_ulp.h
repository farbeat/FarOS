/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Provides button port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-15， Create file. \n
 */
#ifndef BOOT_ULP_H
#define BOOT_ULP_H

#include <stdint.h>
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_button BUTTON
 * @ingroup  drivers_port
 * @{
 */

#define ULP_WKUP_EVT_EN     0x5702C1F0
#define ULP_WKUP_INT_EN     0x5702C1F8
#define ULP_WKUP_INT_STS    0x5702c1E0
#define ULP_WKUP_EVT_STS    0x5702C1E8
#define ULP_WKUP_INT_CLR    0x5702c1E4
#define ULP_WKUP_EVT_CLR    0x5702c1EC
#define ULP_SYSTICK_VAL0    0x5702c514
#define ULP_SYSTICK_VAL1    0x5702c518
#define ULP_SYSTICK_VAL2    0x5702c51C
#define ULP_SYSTICK_VAL3    0x5702c520
#define ULP_SYSTICK_SET0    0x5702c524
#define ULP_SYSTICK_SET1    0x5702c528
#define ULP_SYSTICK_SET2    0x5702c52C
#define ULP_SYSTICK_SET3    0x5702c530
#define MS_PER_S 1000
#define ULP_CALIBRATION_INTERVAL 900000
#define SSB_CALENDAR_BASE_ADDR 0x57038000

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

extern calendar_regs_t *g_calendar_reg;

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

void uapi_ulp_systick_self_verification(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
