/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: button application header file \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-08, Create file. \n
 */
#ifndef INPUT_APP_H
#define INPUT_APP_H

#include "stdint.h"
#include "product.h"
#ifdef SUPPORT_POWER_MANAGER
#include "power_display_service.h"
#endif

/**
 * @defgroup BUTTON_APP_
 * @ingroup  BUTTON_APP
 * @{
 */

#if (CONFIG_SUPPORT_BUTTON_DOUBLE_CLICK == YES)
#define BUTTON_CLICK_THRESHOLD 400
#else
#define BUTTON_CLICK_THRESHOLD 0
#endif

#define BUTTON_APP_TIMER_INTERVAL 20
#define BUTTON_INVALID_CLICK_TIMES 8
#if (CONFIG_SUPPORT_BUTTON_DOUBLE_CLICK == YES)
#define BUTTON_INVILID_CLICK_TIMES 8
#else
#define BUTTON_INVILID_CLICK_TIMES 0
#endif
#define BUTTON_DEFAULT_MAX_CLICK_TIMES 2
#define BUTTON_PRESS_TIME_1S 1000
#define BUTTON_PRESS_TIME_9S 9000
#define BUTTON_INVALID_PRESS_TIME (BUTTON_PRESS_TIME_9S + BUTTON_APP_TIMER_INTERVAL)
#define APP_BOARD_MSG_COUNT       16
#define APP_BOARD_TASK_SIZE       0x800
#define APP_BOARD_TASK_PRIO       5
#define BUTTON_TASK_SIZE          0x800
#define BUTTON_TASK_PRIO          3

#define TOUCH_VETO_SLEEP_MS       16

typedef enum {
    /* power key */
    BUTTON_PWR_START = 0,
    BUTTON_PWR_PRESS,
    BUTTON_PWR_CLICKUP,
    BUTTON_PWR_CLICKUP_2,
    BUTTON_PWR_LONGPRESS_1S,
    BUTTON_PWR_LONGPRESS_2S,
    BUTTON_PWR_LONGPRESS_3S,
    BUTTON_PWR_LONGPRESS_4S,
    BUTTON_PWR_LONGPRESS_5S,
    BUTTON_PWR_LONGPRESS_6S,
    BUTTON_PWR_LONGPRESS_7S,
    BUTTON_PWR_LONGPRESS_8S,
    BUTTON_PWR_LONGPRESS_9S,
    BUTTON_PWR_LONGPRESS_1S_OVER,
    BUTTON_PWR_LONGPRESS_2S_OVER,
    BUTTON_PWR_LONGPRESS_3S_OVER,
    BUTTON_PWR_LONGPRESS_4S_OVER,
    BUTTON_PWR_LONGPRESS_5S_OVER,
    BUTTON_PWR_LONGPRESS_6S_OVER,
    BUTTON_PWR_LONGPRESS_7S_OVER,
    BUTTON_PWR_LONGPRESS_8S_OVER,
    BUTTON_PWR_LONGPRESS_9S_OVER,
    BUTTON_PWR_INVALID,

    /* function key */
    BUTTON1_START,
    BUTTON1_PRESS,
    BUTTON1_CLICKUP,
    BUTTON1_CLICKUP_2,
    BUTTON1_LONGPRESS_1S,
    BUTTON1_LONGPRESS_2S,
    BUTTON1_LONGPRESS_3S,
    BUTTON1_LONGPRESS_4S,
    BUTTON1_LONGPRESS_5S,
    BUTTON1_LONGPRESS_6S,
    BUTTON1_LONGPRESS_7S,
    BUTTON1_LONGPRESS_8S,
    BUTTON1_LONGPRESS_9S,
    BUTTON1_LONGPRESS_1S_OVER,
    BUTTON1_LONGPRESS_2S_OVER,
    BUTTON1_LONGPRESS_3S_OVER,
    BUTTON1_LONGPRESS_4S_OVER,
    BUTTON1_LONGPRESS_5S_OVER,
    BUTTON1_LONGPRESS_6S_OVER,
    BUTTON1_LONGPRESS_7S_OVER,
    BUTTON1_LONGPRESS_8S_OVER,
    BUTTON1_LONGPRESS_9S_OVER,
    BUTTON1_INVALID,

    BUTTON_BUTT
} event_button_t;

typedef enum {
    APP_BUTTON,
    APP_TOUCH,
} app_type;

typedef struct {
    uint8_t event;
    uint8_t sub_event;   /* 发给ux时需要填写 */
    uint8_t res[2];
    app_type app;
} app_comm_mail_t;

typedef struct {
    app_comm_mail_t comm;
    uint32_t data[6];
    void* pri;
    void* config_buff;
    uint16_t config_len;
} common_notify_mail_t;

typedef struct {
    common_notify_mail_t mail;
} cmd_msg_t;

/**
 * @if Eng
 * @brief  Button task message.
 * @else
 * @brief  按键任务消息。
 * @endif
 */
typedef struct {
    common_notify_mail_t notify;
} button_task_mail_t;

typedef void (*report_common_event_cb)(void);

/**
 * @if Eng
 * @brief  Button driver event.
 * @else
 * @brief  按键驱动事件枚举
 * @endif
 */
enum {
    BUTTON_DRIVER_EVENT_PRESS,
    BUTTON_DRIVER_EVENT_RELEASE,
};

uint32_t button_app_init(void);
uint64_t get_button_queue_id(void);
uint64_t get_app_queue_id(void);
#ifdef SUPPORT_POWER_MANAGER
uint32_t button_register_power_display_cb(event_callback_func callback);
#endif
uint32_t uapi_button_register_event_cb(report_common_event_cb callback, event_button_t event);
uint32_t uapi_button_unregister_event_cb(event_button_t event);
/**
 * @}
 */

#endif
