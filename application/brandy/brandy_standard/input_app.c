/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: button application source file \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-08, Create file. \n
 */
#include "input_app.h"
#include "button.h"
#include "soc_diag_util.h"
#include "cmsis_os2.h"
#include "debug_print.h"
#include "pm_definition.h"
#include "tcxo.h"
#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
#include "input_manager.h"
#endif
#ifdef SUPPORT_POWER_MANAGER
#include "soc_lcd_api.h"
#endif

static unsigned long g_app_queue = 0xFFFFFFFF;
uint8_t g_long_press_flag = 0;
/**
 * @if Eng
 * @brief  Button state enum.
 * @else
 * @brief  按键状态机枚举
 * @endif
 */
typedef enum {
    BUTTON_STATE_NONE = 0,          /*!< @if Eng Default state.
                                         @else   默认状态  @endif */
    BUTTON_PRESS_STATUS,            /*!< @if Eng Pressed state.
                                         @else   按下状态  @endif */
    BUTTON_RELEASE_STATUS,          /*!< @if Eng Released state.
                                        @else   松开状态  @endif */
    BUTTON_LONGPRESS_1S_STATUS,
    BUTTON_LONGPRESS_2S_STATUS,
    BUTTON_LONGPRESS_3S_STATUS,
    BUTTON_LONGPRESS_4S_STATUS,
    BUTTON_LONGPRESS_5S_STATUS,
    BUTTON_LONGPRESS_6S_STATUS,
    BUTTON_LONGPRESS_7S_STATUS,
    BUTTON_LONGPRESS_8S_STATUS,
    BUTTON_LONGPRESS_9S_STATUS,
} button_state_t;

/**
 * @if Eng
 * @brief  Button report state enum.
 * @else
 * @brief  按键上报状态机枚举
 * @endif
 */
typedef enum  {
    BUTTON_PRESS,
    BUTTON_CLICK,                   /*!< @if Eng Click one time.
                                         @else   按下一次  @endif */
    BUTTON_DOUBLE_CLICK,            /*!< @if Eng Click double time.
                                         @else   连续按下两次  @endif */
    BUTTON_OTHER_CLICK,             /*!< @if Eng Click other time.
                                         @else   其他连按次数  @endif */
    BUTTON_CLICK_TIMES_INVALID,     /*!< @if Eng Click times invilid.
                                         @else   异常连击次数  @endif */
    BUTTON_LONGPRESS_1S,
    BUTTON_LONGPRESS_2S,
    BUTTON_LONGPRESS_3S,
    BUTTON_LONGPRESS_4S,
    BUTTON_LONGPRESS_5S,
    BUTTON_LONGPRESS_6S,
    BUTTON_LONGPRESS_7S,
    BUTTON_LONGPRESS_8S,
    BUTTON_LONGPRESS_9S,
    BUTTON_LONGPRESS_1S_OVER,
    BUTTON_LONGPRESS_2S_OVER,
    BUTTON_LONGPRESS_3S_OVER,
    BUTTON_LONGPRESS_4S_OVER,
    BUTTON_LONGPRESS_5S_OVER,
    BUTTON_LONGPRESS_6S_OVER,
    BUTTON_LONGPRESS_7S_OVER,
    BUTTON_LONGPRESS_8S_OVER,
    BUTTON_LONGPRESS_9S_OVER,
    BUTTON_LONGPRESS_INVALID,       /*!< @if Eng Click times invilid.
                                         @else   异常长按时间  @endif */
    BUTTON_STATE_MAX
} button_report_state_t;

typedef struct {
    button_state_t state;
    uint32_t lp_start_time;
    uint32_t click_start_time;
    uint64_t end_time;
    uint32_t click_end_time;
    uint8_t click_cnt;
    bool is_clicked;
} button_app_t;

button_app_t g_button[BUTTON_MAX] = {0};
osTimerId_t g_button_app_timer = NULL;
button_id_t g_curr_button_id;

button_state_t g_curr_button_state;
bool g_app_timer_is_running = true;
report_common_event_cb g_button_common_event_cb[BUTTON_BUTT] = {0};
button_report_state_t g_button_report_state_record = 0;
button_state_t g_button_state_record = 0;
uint32_t g_button_press_time = BUTTON_PRESS_TIME_1S;

button_map_t g_brandy_button_pin_map[BUTTON_PIN_MAP_MAX] = {
    { PIN_NONE, BUTTON_PWR },
    { PIN_NONE, BUTTON_1 },
    { PIN_NONE, BUTTON_2 },
    { PIN_NONE, BUTTON_3 },
    { PIN_NONE, BUTTON_4 },
    { PIN_NONE, BUTTON_5 },
    { PIN_NONE, BUTTON_6 },
    { PIN_NONE, BUTTON_7 },
};

#ifdef SUPPORT_POWER_MANAGER
event_callback_func g_button_power_manager_cb = NULL;
uint32_t button_register_power_display_cb(event_callback_func callback)
{
    if (g_button_power_manager_cb != NULL) {
        return ERRCODE_FAIL;
    }
    g_button_power_manager_cb = callback;
    return ERRCODE_SUCC;
}

static void button_report_event_to_pms(void)
{
    if (g_button_power_manager_cb != NULL) {
        g_button_power_manager_cb(0, 0);
    }
}

#endif

#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
report_event_cb g_button_event_cb = NULL;
uint32_t button_register_report_event_cb(report_event_cb callback)
{
    if (g_button_event_cb != NULL) {
        return ERRCODE_FAIL;
    }
    g_button_event_cb = callback;
    return ERRCODE_SUCC;
}

static uint32_t button_unregister_report_event_cb(void)
{
    g_button_event_cb = NULL;
    return ERRCODE_SUCC;
}

static uint32_t button_init(void)
{
    return ERRCODE_SUCC;
}

static uint32_t button_close(void)
{
    return ERRCODE_SUCC;
}

static void button_report_event_to_gui(button_id_t id, uint16_t state)
{
    input_dev_data_t data;

    if (g_button_event_cb != NULL) {
        data.type = INDEV_TYPE_KEY;
        if (id == BUTTON_PWR) {
            data.keyId = INDEV_KEY_POWER;
        } else {
            data.keyId = INDEV_KEY_FUNC;
        }
        data.state = state; // 2:longpress 1:press 0:release
        g_button_event_cb(&data);
    }
}

static void button_report_event(button_id_t id, event_button_t event)
{
    input_dev_data_t data;

    switch (event) {
        case BUTTON_PWR_PRESS:
            #ifdef SUPPORT_POWER_MANAGER
            if (!lcd_get_status()) {
                button_report_event_to_pms();
                return;
            }
            button_report_event_to_pms();
            #endif
            button_report_event_to_gui(id, 1); // 1:press
            break;
        case BUTTON_PWR_CLICKUP:
            button_report_event_to_gui(id, 0); // 0:release
            break;
        case BUTTON_PWR_LONGPRESS_3S:
            button_report_event_to_gui(id, 2); // 2:longpress
            break;
        default:
            break;
    }
}
#endif

uint32_t uapi_button_register_event_cb(report_common_event_cb callback, event_button_t event)
{
    if (g_button_common_event_cb[event] != NULL) {
        return ERRCODE_FAIL;
    }

    if ((callback == NULL) || (event < BUTTON_PWR_PRESS) || (event > BUTTON_BUTT)) {
        return ERRCODE_FAIL;
    }

    g_button_common_event_cb[event] = callback;
    return ERRCODE_SUCC;
}

uint32_t uapi_button_unregister_event_cb(event_button_t event)
{
    if (g_button_common_event_cb[event] == NULL) {
        return ERRCODE_SUCC;
    }

    if ((event < BUTTON_PWR_PRESS) || (event > BUTTON_BUTT)) {
        return ERRCODE_FAIL;
    }

    g_button_common_event_cb[event] = NULL;
    return ERRCODE_SUCC;
}

static void button_send_msg_to_app(uint8_t pin_group, button_press_state_t state)
{
    button_task_mail_t mail = {0};
    button_id_t id = BUTTON_MAX;
    if (state == PRESSED) {
        mail.notify.comm.event = BUTTON_DRIVER_EVENT_PRESS;
    } else if (state == RELEASED) {
        mail.notify.comm.event = BUTTON_DRIVER_EVENT_RELEASE;
    }

    for (uint8_t i = 0; i < BUTTON_PIN_MAP_MAX; i++) {
        if ((pin_group & (1 << i)) != 0) {
            id = (button_id_t)i;
            break;
        }
    }
    mail.notify.data[0] = (uint32_t)id;
    mail.notify.comm.app = APP_BUTTON;
    if (osal_msg_queue_write_copy(get_app_queue_id(), (void *)&mail, sizeof(common_notify_mail_t), 0) != OSAL_SUCCESS) {
        PRINT("[button][driver] button state %d send failed\n", (uint32_t)state);
    }
}

static button_state_t button_get_state(button_id_t id)
{
    return g_button[id].state;
}

static void button_set_state(button_id_t id, button_state_t state)
{
    g_button[id].state = state;
}

static event_button_t button_event_get(button_id_t id, button_report_state_t state)
{
    event_button_t event = BUTTON_PWR_START;
    if (id == BUTTON_1) {
        event = BUTTON1_START;
    }

    if ((state == BUTTON_PRESS) || (state == BUTTON_CLICK) || (state == BUTTON_DOUBLE_CLICK)) {
        return (event + state + 1);
    } else {
        return (event + state - 1);
    }
}

static void button_send_event_to_ui(button_id_t id, button_report_state_t state)
{
    cmd_msg_t msg;
    event_button_t event = BUTTON_BUTT;
    event = button_event_get(id, state);
#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
    button_report_event(id, event);
#endif
    if (g_button_common_event_cb[event] != NULL) {
        g_button_common_event_cb[event]();
    }
}

static void button_report_long_press_over_state(button_id_t id, button_state_t state)
{
    unused(id);
    switch (state) {
        case BUTTON_LONGPRESS_1S_STATUS:
            button_send_event_to_ui(id, BUTTON_LONGPRESS_1S_OVER);
            button_set_state(id, BUTTON_STATE_NONE);
            break;
        case BUTTON_LONGPRESS_2S_STATUS:
            button_send_event_to_ui(id, BUTTON_LONGPRESS_2S_OVER);
            button_set_state(id, BUTTON_STATE_NONE);
            break;
        case BUTTON_LONGPRESS_3S_STATUS:
            button_send_event_to_ui(id, BUTTON_LONGPRESS_3S_OVER);
            button_set_state(id, BUTTON_STATE_NONE);
            break;
        case BUTTON_LONGPRESS_4S_STATUS:
            button_send_event_to_ui(id, BUTTON_LONGPRESS_4S_OVER);
            button_set_state(id, BUTTON_STATE_NONE);
            break;
        case BUTTON_LONGPRESS_5S_STATUS:
            button_send_event_to_ui(id, BUTTON_LONGPRESS_5S_OVER);
            button_set_state(id, BUTTON_STATE_NONE);
            break;
        case BUTTON_LONGPRESS_6S_STATUS:
            button_send_event_to_ui(id, BUTTON_LONGPRESS_6S_OVER);
            button_set_state(id, BUTTON_STATE_NONE);
            break;
        case BUTTON_LONGPRESS_7S_STATUS:
            button_send_event_to_ui(id, BUTTON_LONGPRESS_7S_OVER);
            button_set_state(id, BUTTON_STATE_NONE);
            break;
        case BUTTON_LONGPRESS_8S_STATUS:
            button_send_event_to_ui(id, BUTTON_LONGPRESS_8S_OVER);
            button_set_state(id, BUTTON_STATE_NONE);
            break;
        case BUTTON_LONGPRESS_9S_STATUS:
            button_send_event_to_ui(id, BUTTON_LONGPRESS_9S_OVER);
            button_set_state(id, BUTTON_STATE_NONE);
            break;
        default:
            break;
    }
}

static void button_report_click_state(button_id_t id)
{
    if (g_curr_button_state == BUTTON_RELEASE_STATUS && button_get_state(id) == BUTTON_RELEASE_STATUS) {
        if (g_button[id].is_clicked == false) {
            /* The first click, reset all statistics */
            g_button[id].is_clicked = true;
            g_button[id].click_cnt = 0;
            g_button[id].click_start_time = g_button[id].end_time;
        } else if (g_button[id].is_clicked == true && g_long_press_flag != 1) {
            /* continue click */
            g_button[id].click_cnt++;
            g_button[id].click_start_time = g_button[id].end_time;
        }
        /* click invilid */
        if (g_button[id].click_cnt + 1 > BUTTON_INVILID_CLICK_TIMES) {
#if (CONFIG_SUPPORT_BUTTON_DOUBLE_CLICK == YES)
            button_send_event_to_ui(id, BUTTON_DOUBLE_CLICK);
#else
            button_send_event_to_ui(id, BUTTON_CLICK);
#endif
            osTimerStop(g_button_app_timer);
            g_button[id].is_clicked = 0;
            g_button[id].click_cnt = 0;
            g_long_press_flag = 0;
            button_set_state(id, BUTTON_STATE_NONE);
        }
    }
    /* report click */
    if (button_get_state(id) == BUTTON_RELEASE_STATUS) {
        if (g_button[id].end_time - g_button[id].click_start_time >= BUTTON_CLICK_THRESHOLD) {
            /* click more than two times */
            if (g_button[id].click_cnt + 1 >= BUTTON_DEFAULT_MAX_CLICK_TIMES) {
#if (CONFIG_SUPPORT_BUTTON_DOUBLE_CLICK == YES)
                button_send_event_to_ui(id, BUTTON_DOUBLE_CLICK);
#else
                button_send_event_to_ui(id, BUTTON_CLICK);
#endif
            } else {
                button_send_event_to_ui(id, BUTTON_CLICK);
            }
            g_button[id].is_clicked = 0;
            g_button[id].click_cnt = 0;
            g_long_press_flag = 0;
            button_set_state(id, BUTTON_STATE_NONE);
            osTimerStop(g_button_app_timer);
        }
    }
}

static void button_released_state_handler(button_id_t id)
{
    if (g_curr_button_state == BUTTON_PRESS_STATUS) {
        return;
    }
    if (g_curr_button_state == BUTTON_RELEASE_STATUS) {
        button_report_long_press_over_state(id, button_get_state(id));
        if (button_get_state(id) == BUTTON_STATE_NONE) {
            osTimerStop(g_button_app_timer);
        }
        g_button[id].lp_start_time = g_button[id].end_time;
        if (button_get_state(id) == BUTTON_PRESS_STATUS) {
            button_set_state(id, BUTTON_RELEASE_STATUS);
        }
        g_button_report_state_record = 0;
        g_button_state_record = 0;
        g_button_press_time = 0;
    }
    uint32_t int_sts = osal_irq_lock();
    button_report_click_state(id);
    osal_irq_restore(int_sts);
}

static void button_pressed_state_handler(button_id_t id)
{
    if (g_curr_button_state == BUTTON_RELEASE_STATUS) {
        return;
    }
    if (g_curr_button_state == BUTTON_PRESS_STATUS) {
        g_button[id].lp_start_time = g_button[id].end_time;
        if (button_get_state(id) == BUTTON_STATE_NONE || button_get_state(id) == BUTTON_RELEASE_STATUS) {
            button_set_state(id, BUTTON_PRESS_STATUS);
        }
    }
    if (button_get_state(id) == BUTTON_PRESS_STATUS) {
        /* PRESS after PRESS must be long press */
        if (g_button[id].end_time - g_button[id].lp_start_time >= BUTTON_PRESS_TIME_1S) {
            button_send_event_to_ui(id, BUTTON_LONGPRESS_1S);
            button_set_state(id, BUTTON_LONGPRESS_1S_STATUS);
            g_button_report_state_record = BUTTON_LONGPRESS_1S;
            g_button_state_record = BUTTON_LONGPRESS_1S_STATUS;
            g_button_press_time = BUTTON_PRESS_TIME_1S + BUTTON_PRESS_TIME_1S;
            g_long_press_flag = 1;
        }
    }
    if ((button_get_state(id) == g_button_state_record) && (button_get_state(id) > BUTTON_RELEASE_STATUS)) {
        if ((g_button[id].end_time - g_button[id].lp_start_time >= g_button_press_time) &&
            (g_button[id].end_time - g_button[id].lp_start_time <= BUTTON_INVALID_PRESS_TIME)) {
            g_button_state_record = g_button_state_record + 1;
            g_button_press_time = g_button_press_time + BUTTON_PRESS_TIME_1S;
            button_send_event_to_ui(id, ++g_button_report_state_record);
            button_set_state(id, button_get_state(id) + 1);
        }
    }
    if ((g_button[id].end_time - g_button[id].lp_start_time > BUTTON_INVALID_PRESS_TIME) &&
        (g_curr_button_state == BUTTON_PRESS_STATUS)) {
        osTimerStop(g_button_app_timer);
    }
}

static void button_process(void)
{
    g_button[g_curr_button_id].end_time = uapi_tcxo_get_ms();
    button_released_state_handler(g_curr_button_id);
    button_pressed_state_handler(g_curr_button_id);
    g_curr_button_state = BUTTON_STATE_NONE;
}

static void button_app_timer_func(void)
{
    button_process();
}

static uint32_t button_driver_event_handler(common_notify_mail_t *mptr)
{
    g_curr_button_id = (button_id_t)(mptr->data[0]);
    switch (mptr->comm.event) {
        case BUTTON_DRIVER_EVENT_PRESS:
            button_send_event_to_ui(g_curr_button_id, BUTTON_PRESS);
            g_curr_button_state = BUTTON_PRESS_STATUS;
            break;
        case BUTTON_DRIVER_EVENT_RELEASE:
            g_curr_button_state = BUTTON_RELEASE_STATUS;
            break;
        default:
            break;
    }
    button_process();
    if (mptr->comm.event == BUTTON_DRIVER_EVENT_PRESS) {
        osTimerStart(g_button_app_timer, BUTTON_APP_TIMER_INTERVAL);
    }

    return ERRCODE_SUCC;
}

static void input_device_task(void const *arg)
{
    unused(arg);
    int32_t ret;
    common_notify_mail_t mail;
    uint32_t msgsize = (uint32_t)sizeof(common_notify_mail_t);
    button_id_t id;

    /* button init */
    uapi_button_init();
    uapi_button_register(g_brandy_button_pin_map);
    uapi_button_send_msg_register(button_send_msg_to_app);

    while (1) {
        ret = osal_msg_queue_read_copy(g_app_queue, (void *)&mail, &msgsize,
                                       OSAL_MSGQ_WAIT_FOREVER);
        if (ret != OSAL_SUCCESS) {
            continue;
        }

        if (mail.comm.app == APP_BUTTON) {
            button_driver_event_handler(&mail);
        }

        if (mail.comm.app == APP_TOUCH) {
            uapi_pm_add_sleep_veto(PM_ID_TOUCH);
            touch_info_process();
            uapi_pm_remove_sleep_veto(PM_ID_TOUCH);
            uapi_pm_add_sleep_veto_with_timeout(PM_ID_TOUCH, TOUCH_VETO_SLEEP_MS);
        }
    }
}

uint32_t button_app_init(void)
{
    if (osal_msg_queue_create("app_queue", APP_BOARD_MSG_COUNT, &g_app_queue,
                              0, sizeof(common_notify_mail_t)) != ERRCODE_SUCC) {
        PRINT("[button][app] app create mail fail\n");
        return ERRCODE_FAIL;
    }

    osal_task *task_handle = osal_kthread_create((osal_kthread_handler)input_device_task, NULL,
                                                 "board_app", APP_BOARD_TASK_SIZE);
    if (task_handle == NULL) {
        osal_msg_queue_is_full(g_app_queue);
        PRINT("create button app thread fail!\r\n");
        return ERRCODE_FAIL;
    }

    osal_kthread_set_priority(task_handle, APP_BOARD_TASK_PRIO);

    g_button_app_timer = osTimerNew((osTimerFunc_t)button_app_timer_func, osTimerPeriodic, NULL, NULL);

    return ERRCODE_SUCC;
}

uint64_t get_app_queue_id(void)
{
    return g_app_queue;
}