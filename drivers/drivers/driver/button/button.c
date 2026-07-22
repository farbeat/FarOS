/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Button driver source file \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-24, Create file. \n
 */

#include "pinctrl.h"
#include "button_porting.h"
#include "platform_core.h"
#include "gpio.h"
#include "gpio_porting.h"
#include "debug_print.h"
#include "rtc.h"
#include "timer.h"
#include "systick.h"
#include "chip_core_irq.h"

#define BUTTON_PWREN_ID 1
/**
 * @if Eng
 * @brief  Button debounce struct
 * @else
 * @brief  按键去抖动结构体定义
 * @endif
 */
typedef struct {
    uint8_t debounced;         /*!< @if Eng Recording debounced group.
                                    @else   记录产生抖动的按键组  @endif */
    uint32_t debounced_time;   /*!< @if Eng Debounced time.
                                    @else   抖动计时  @endif */
} button_debounced_t;

/**
 * @if Eng
 * @brief  Button information struct
 * @else
 * @brief  按键信息结构体定义
 * @endif
 */
typedef struct {
    uint8_t pressed_group;   /*!< @if Eng Recording button pressed group.
                                  @else   记录被按下的按键组  @endif */
    uint8_t which_pressed;   /*!< @if Eng Pressed button bit turns to 1
                                  @else   被按下的按键置1  @endif */
    uint8_t which_released;  /*!< @if Eng Releaesd button bit turns to 1
                                  @else   被松开的按键置1  @endif */
} button_info_t;

button_debounced_t g_button_debounced = {0};
button_map_t g_button_pin_map[BUTTON_PIN_MAP_MAX] = {0};
timer_handle_t g_button_timer = {0};
timer_handle_t g_gpio_button_timer = {0};
button_info_t g_button_info = {0};
bool g_timer_is_running = false;
bool g_gpio_timer_is_running = false;
msg_send_callback_t g_msg_send_cb = NULL;
pin_t g_gpio_key_record = 0;
uint8_t g_buttom_vetoed = 0;

static const char* g_state_str[PRESSED_MAX] = {
    "BUTTON_PRESSED",
    "BUTTON_RESLASED",
};

static void button_debounced_timer_callback(uint32_t data);
static void button_check_released_timer_callback(uint32_t data);

void button_group_set_bit(uint8_t* group, uint8_t bit)
{
    (*group) |= (1 << bit);
}

static void button_group_clear_bit(uint8_t* group, uint8_t bit)
{
    (*group) &= ~(1 << bit);
}

static button_press_state_t button_get_press_state(pin_t pin)
{
    button_peripheral_api *button_api = button_port_get_api();
    return button_api->button_get_state(pin);
}

static bool button_group_find_bit(uint8_t group, uint8_t* bit)
{
    if (bit == NULL) {
        return false;
    }
    for (uint8_t i = 0; i < BUTTON_PIN_MAP_MAX; i++) {
        if ((group & (1 << i)) != 0) {
            *bit = i;
            return true;
        }
    }
    return false;
}

static void button_send_msg(uint8_t pin_group, button_press_state_t state)
{
    if (g_msg_send_cb == NULL) {
        PRINT("button %d is %s\r\n", pin_group, g_state_str[state]);
    } else {
        g_msg_send_cb(pin_group, state);
    }
}

static void button_timer_start(void)
{
    bool start = false;
    if (!g_gpio_timer_is_running) {
        g_gpio_timer_is_running = true;
        start = true;
    }

    if (start) {
        uapi_rtc_stop(g_gpio_button_timer);
        uapi_rtc_start(g_gpio_button_timer, BUTTON_TIMER_INTERVAL, button_debounced_timer_callback, 0);
    }
}

static void button_timer_start_check_released(void)
{
    bool start = false;
    if (!g_timer_is_running) {
        g_timer_is_running = true;
        start = true;
    }

    if (start) {
        uapi_rtc_stop(g_button_timer);
        uapi_rtc_start(g_button_timer, BUTTON_CHECK_RELEADSED, button_check_released_timer_callback, 0);
    }
}

static void button_irq_callback(pin_t pin)
{
    pin = PIN_NONE;
    uapi_ulp_wkup_type_record();
    button_peripheral_api *button_api = button_port_get_api();
    // if not vbus or pwren interrupt return
    if (button_api->button_check_interrupt(pin) == false) {
        button_api->button_clear_interrupt(pin);
        return;
    }
    // deal with vbus interrupt
    if (button_api->button_check_vbus_interrupt()) {
        button_api->button_deal_vbus_event();
        button_api->button_clear_interrupt(pin);
        return;
    }

    // deal with pwren interrupt
    button_api->button_clear_interrupt(pin);
    button_press_state_t press_state = button_get_press_state(pin);
    button_send_msg(BUTTON_PWREN_ID, press_state);
    if (g_buttom_vetoed == 0) {
        button_api->button_add_veto();
        g_buttom_vetoed = 1;
    }
    button_timer_start_check_released();
}

static void button_gpio_callback(pin_t pin, uintptr_t param)
{
    unused(param);
    uint8_t bit = 0;
    for (uint8_t index = 0; index < BUTTON_PIN_MAP_MAX; index++) {
        if (pin == g_button_pin_map[index].pin) {
            bit = index;
            break;
        }
    }

    uapi_gpio_clear_interrupt(pin);
    g_gpio_key_record = pin;
    uapi_gpio_disable_interrupt(g_gpio_key_record);
    button_press_state_t press_state = button_get_press_state(pin);
    unsigned int irq_sts = osal_irq_lock();
    if (press_state == PRESSED) {
        button_group_set_bit(&g_button_debounced.debounced, bit);
        g_button_debounced.debounced_time = (uint32_t)uapi_systick_get_ms();
    }
    osal_irq_restore(irq_sts);

    button_peripheral_api *button_api = button_port_get_api();
    button_api->button_add_veto();
    button_timer_start();
}

static void button_debounced_handler(void)
{
    pin_t pin = 0;
    uint32_t time = (uint32_t)uapi_systick_get_ms();
    uint8_t pressed_group = 0;
    uint8_t bit = 0;
    uint8_t debounced = g_button_debounced.debounced;

    while (debounced > 0) {
        if (button_group_find_bit(debounced, &bit)) {
            button_group_clear_bit(&debounced, bit);
            pin = g_button_pin_map[bit].pin;
            if ((button_get_press_state(pin) == PRESSED) &&
                (time - g_button_debounced.debounced_time >= DEBOUNCED_INTERVAL)) {
                /* Debounce */
                button_group_set_bit(&pressed_group, bit);
                button_group_clear_bit(&g_button_debounced.debounced, bit);
            }
 
            if ((button_get_press_state(pin) == RELEASED) &&
                (time - g_button_debounced.debounced_time >= INTERFERENCE_INTERVAL)) {
                /* Interference */
                button_group_clear_bit(&g_button_debounced.debounced, bit);
            }
        }
    }

    /* Comparing with the old pressed state to figure out which buttons are pressed or released */
    g_button_info.which_pressed = pressed_group & ~g_button_info.pressed_group;
    g_button_info.which_released = ~pressed_group & g_button_info.pressed_group;

    /* Recording pressed_group */
    g_button_info.pressed_group = pressed_group;
}

static void button_debounced_timer_callback(uint32_t data)
{
    unused(data);
    g_gpio_timer_is_running = false;

    button_debounced_handler();

    if (g_button_info.which_pressed != 0) {
        button_send_msg(g_button_info.which_pressed, PRESSED);
    } else if (g_button_info.which_released != 0) {
        button_send_msg(g_button_info.which_released, RELEASED);
    }
    if (g_button_debounced.debounced != 0) {
        button_timer_start();
    } else {
        button_peripheral_api *button_api = button_port_get_api();
        button_api->button_remove_veto();
        uapi_gpio_enable_interrupt(g_gpio_key_record);
    }
}

static void button_check_released_timer_callback(uint32_t data)
{
    unused(data);

    g_timer_is_running = false;

    button_peripheral_api *button_api = button_port_get_api();
    button_press_state_t press_state;
    press_state = button_get_press_state(PIN_NONE);
    if (press_state == 1) {
        if (g_buttom_vetoed == 1) {
            button_api->button_remove_veto();
            g_buttom_vetoed = 0;
        }
        button_send_msg(1, RELEASED);
    } else {
        button_timer_start_check_released();
    }
}

errcode_t uapi_button_gpio_register(pin_t pin)
{
    errcode_t ret;
    ret = uapi_gpio_unregister_isr_func(pin);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    /* GPIO set input and pull up */
    if ((uapi_pin_set_mode(pin, (pin_mode_t)HAL_PIO_FUNC_GPIO) != ERRCODE_SUCC) ||
        (uapi_pin_set_pull(pin, (pin_pull_t)HAL_PIO_PULL_UP) != ERRCODE_SUCC) ||
        (uapi_gpio_set_dir(pin, GPIO_DIRECTION_INPUT) != ERRCODE_SUCC)) {
        PRINT("gpio config failed\r\n");
        return ERRCODE_FAIL;
    }
    ret = uapi_gpio_register_isr_func(pin, GPIO_INTERRUPT_DEDGE, button_gpio_callback);
    if (ret != ERRCODE_SUCC) {
        PRINT("button gpio register failed\r\n");
        return ret;
    }
    return ERRCODE_SUCC;
}

errcode_t uapi_button_deregister(pin_t pin)
{
    errcode_t ret;
    button_peripheral_api *button_api = button_port_get_api();
    ret = button_api->button_unregister_callback(pin);
    if (ret != ERRCODE_SUCC) {
        PRINT("button deregister failed\r\n");
        return ret;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_button_register(button_map_t* button_map)
{
    errcode_t ret = 0;
    if (button_map == NULL) {
        return ERRCODE_FAIL;
    }

    button_peripheral_api *button_api = button_port_get_api();
    for (uint8_t i = 0; i < BUTTON_PIN_MAP_MAX; i++) {
        g_button_pin_map[i].pin = button_map[i].pin;
        g_button_pin_map[i].button = button_map[i].button;
        if (button_map[i].pin != PIN_NONE && button_map[i].button != BUTTON_MAX) {
            if (button_map[i].button == BUTTON_PWR) {
                button_api->button_pmu_pwr_cfg(true);
            }
            ret |= uapi_button_gpio_register(g_button_pin_map[i].pin);
        } else if (button_map[i].pin == PIN_NONE && button_map[i].button == BUTTON_PWR) {
            ret = button_api->button_register_callback(g_button_pin_map[i].pin, (osal_irq_handler)button_irq_callback);
            if (ret != ERRCODE_SUCC) {
                PRINT("button register failed\r\n");
                return ret;
            }
            continue;
        }
    }

    return ret;
}

void uapi_button_deinit(void)
{
    /* Stop the time and clear the pin map */
    uapi_rtc_stop(g_button_timer);
    uapi_rtc_delete(g_button_timer);
    g_button_timer = NULL;
    uapi_rtc_stop(g_gpio_button_timer);
    uapi_rtc_delete(g_gpio_button_timer);
    g_gpio_button_timer = NULL;
    for (uint8_t i = 0; i < BUTTON_PIN_MAP_MAX; i++) {
        g_button_pin_map[i].pin = PIN_NONE;
        g_button_pin_map[i].button = BUTTON_MAX;
    }
}

errcode_t uapi_button_init(void)
{
    errcode_t ret = 0;
    ret = uapi_rtc_init();

    ret |= uapi_rtc_adapter(0, RTC_0_IRQN, irq_prio(RTC_0_IRQN)); /* 1: using timer1 */
    if (ret != ERRCODE_SUCC) {
        PRINT("uapi_rtc_init FAILED 0x%x", ret);
        return ret;
    }
    ret = uapi_rtc_create(0, &g_button_timer); /* 1: using timer1 */
    if (ret != ERRCODE_SUCC) {
        PRINT("uapi_rtc_create FAILED 0x%x", ret);
        return ret;
    }
    ret = uapi_rtc_create(0, &g_gpio_button_timer); /* 1: using timer1 */
    if (ret != ERRCODE_SUCC) {
        PRINT("uapi_rtc_create FAILED 0x%x", ret);
        return ret;
    }
    return ERRCODE_SUCC;
}

void uapi_button_send_msg_register(msg_send_callback_t cb)
{
    if (cb != NULL) {
        g_msg_send_cb = cb;
    }
}