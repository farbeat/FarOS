/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Button driver header file \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-24, Create file. \n
 */
#ifndef BUTTON_H
#define BUTTON_H

#include "hal_gpio.h"
#include "soc_osal.h"
#include "platform_core.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_driver_button BUTTON
 * @ingroup  drivers_driver
 * @{
 */

#define BUTTON_PIN_MAP_MAX 8
#define BUTTON_CLICK_INTERVAL 10
#define DEBOUNCED_INTERVAL (BUTTON_CLICK_INTERVAL * 2)
#define INTERFERENCE_INTERVAL (BUTTON_CLICK_INTERVAL * 10)
#define BUTTON_TIMER_INTERVAL (BUTTON_CLICK_INTERVAL)
#define BUTTON_CHECK_RELEADSED 20 // pwren的检查时间

/**
 * @if Eng
 * @brief  Button press state enum.
 * @else
 * @brief  按键按压状态枚举
 * @endif
 */
typedef enum {
    PRESSED = 0,
    RELEASED = 1,
    PRESSED_MAX,
} button_press_state_t;

/**
 * @if Eng
 * @brief  Button num enmu.
 * @else
 * @brief  按键序号枚举
 * @endif
 */
typedef enum {
    BUTTON_PWR,
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,
    BUTTON_5,
    BUTTON_6,
    BUTTON_7,
    BUTTON_MAX
} button_id_t;

typedef struct {
    errcode_t (*button_pmu_pwr_cfg)(pin_t ap_cfg);
    bool (*button_check_interrupt)(pin_t pin);
    bool (*button_check_vbus_interrupt)(void);
    void (*button_deal_vbus_event)(void);
    errcode_t (*button_clear_interrupt)(pin_t pin);
    button_press_state_t (*button_get_state)(pin_t pin);
    errcode_t (*button_register_callback)(pin_t pin, osal_irq_handler thread_handler);
    errcode_t (*button_unregister_callback)(pin_t pin);
    void (*button_add_veto)(void);
    void (*button_remove_veto)(void);
} button_peripheral_api;

/**
 * @if Eng
 * @brief  Button map.
 * @else
 * @brief  按键映射
 * @endif
 */
typedef struct {
    pin_t pin;
    button_id_t button;
} button_map_t;

/**
 * @if Eng
 * @brief  Button message send callback.
 * @param  [in]  pin_group Button pin group. Each bit corresponds to a key.
 * @param  [in]  state Button state press or release.
 * @else
 * @brief  定时器回调函数
 * @param  [in]  pin_group 按键组，每一个比特对应一个按键。
 * @param  [in]  state 按键状态是按压还是松开。
 * @endif
 */
typedef void (*msg_send_callback_t)(uint8_t pin_group, button_press_state_t state);

/**
 * @if Eng
 * @brief  Button deregisters func.
 * @param  [in]  pin Index of GPIO to be deregistered.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other          Failure. For details, see @ref errcode_t
 * @else
 * @brief  按键去注册函数
 * @param  [in]  pin 要去注册的GPIO编号
 * @retval ERRCODE_SUCC   成功
 * @retval Other          失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_button_deregister(pin_t pin);

/**
 * @if Eng
 * @brief  Button registers func.
 * @param  [in]  button_map Button map from board
 * @retval ERRCODE_SUCC   Success.
 * @retval Other          Failure. For details, see @ref errcode_t
 * @else
 * @brief  按键注册函数
 * @param  [in]  button_map 来自board的按键映射表
 * @retval ERRCODE_SUCC   成功
 * @retval Other          失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_button_register(button_map_t* button_map);

/**
 * @if Eng
 * @brief  Button driver deinitializes func.
 * @else
 * @brief  按键去初始化函数
 * @endif
 */
void uapi_button_deinit(void);

/**
 * @if Eng
 * @brief  Button driver initializes func.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other          Failure. For details, see @ref errcode_t
 * @else
 * @brief  按键初始化函数
 * @retval ERRCODE_SUCC   成功
 * @retval Other          失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_button_init(void);

/**
 * @if Eng
 * @brief  gpio button registers func.
 * @param  [in]  pin Index of GPIO to be registered.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other          Failure. For details, see @ref errcode_t
 * @else
 * @brief  GPIO按键注册函数
 * @param  [in]  pin 要注册的GPIO编号
 * @retval ERRCODE_SUCC   成功
 * @retval Other          失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_button_gpio_register(pin_t pin);

/**
 * @if Eng
 * @brief  Sending message registers API.
 * @param  [in]  cb Callback func registered by APP.
 * @else
 * @brief  发送函数注册接口
 * @param  [in]  cb APP注册的回调函数.
 * @endif
 */
void uapi_button_send_msg_register(msg_send_callback_t cb);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif