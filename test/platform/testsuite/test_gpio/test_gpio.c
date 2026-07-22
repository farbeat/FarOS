/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test gpio source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-11, Create file. \n
 */
#include "test_gpio.h"
#include "cmsis_os2.h"
#include "gpio.h"
#include "securec.h"
#include "pinctrl_porting.h"
#include "chip_io.h"

#include "test_suite.h"
#include "test_suite_errors.h"
#include "debug_print.h"


#include "tcxo.h"
#include "irmalloc.h"
#include "pinctrl.h"


#define TEST_PARAM_ARGC_1               1
#define TEST_PARAM_ARGC_2               2
#define TEST_PARAM_ARGC_3               3


static int test_gpio_output(int argc, char *argv[])
{
    pin_t pin = 0;
    uint32_t level = 0;

    if (argc >= TEST_PARAM_ARGC_2) {
        pin = (pin_t)strtol(argv[0], NULL, 0);
        level = strtol(argv[1], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    PRINT("[Params] PIN:%d, levle: %d. \r\n", pin, (uint32_t)level);

    uapi_pin_set_mode(pin, HAL_PIO_FUNC_GPIO);
    errcode_t ret = ERRCODE_SUCC;
    ret = uapi_gpio_set_val(pin, level);
    if (ret != ERRCODE_SUCC) {
        PRINT("gpio operate failed , ret = 0x%x\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }

    gpio_level_t gpio_level;
    gpio_level = uapi_gpio_get_val(pin);
    if (gpio_level == (gpio_level_t)level) {
        PRINT("gpio operate success\r\n");
        PRINT("read level is %d\r\n", gpio_level);
    } else {
        PRINT("gpio operate failed\r\n");
        PRINT("read level is %d\r\n", gpio_level);
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}


static int test_gpio_input(int argc, char *argv[])
{
    pin_t pin;
    if (argc > TEST_PARAM_ARGC_1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    pin = (pin_t)strtol(argv[0], NULL, 0);

    PRINT("[Params] PIN:%d. \r\n", pin);
    uapi_pin_set_mode(pin, HAL_PIO_FUNC_GPIO);

    gpio_level_t gpio_level;
    gpio_level = uapi_gpio_get_val(pin);
    if (gpio_level == GPIO_LEVEL_HIGH) {
        PRINT("gpio input level is high\r\n");
    } else {
        PRINT("gpio input level is low\r\n");
    }

    return TEST_SUITE_OK;
}

static int test_gpio_setdir(int argc, char *argv[])
{
    int ret = 0;
    pin_t pin = 0;
    int level = 0;

    if (argc >= TEST_PARAM_ARGC_2) {
        pin = (pin_t)strtol(argv[0], NULL, 0);
        level = strtol(argv[1], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    PRINT("[Params] PIN:%d, levle: %d. \r\n", pin, (uint32_t)level);

    uapi_pin_set_mode(pin, HAL_PIO_FUNC_GPIO);
    if (level == 1) {
        ret = uapi_gpio_set_dir(pin, GPIO_DIRECTION_OUTPUT);
    } else if (level == 0) {
        ret = uapi_gpio_set_dir(pin, GPIO_DIRECTION_INPUT);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    if (ret != 0) {
        PRINT("gpio operate failed , ret = 0x%x\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

static int test_gpio_getdir(int argc, char *argv[])
{
    errcode_t ret = ERRCODE_SUCC;
    pin_t pin = 0;
    gpio_direction_t level = 0;
    gpio_direction_t level_tmp = 0;

    if (argc >= TEST_PARAM_ARGC_1) {
        pin = (pin_t)strtol(argv[0], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uapi_pin_set_mode(pin, HAL_PIO_FUNC_GPIO);
    level = uapi_gpio_get_dir(pin);
    level_tmp = level;
    PRINT("[Params] PIN:%d , level: %d\r\n", pin, level);
    if (level == 0) {
        ret = uapi_gpio_set_dir(pin, GPIO_DIRECTION_OUTPUT);
    } else {
        ret = uapi_gpio_set_dir(pin, GPIO_DIRECTION_INPUT);
    }
    if (ret != ERRCODE_SUCC) {
        PRINT("gpio operate failed , ret = 0x%x\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }
    level = uapi_gpio_get_dir(pin);
    if (level == level_tmp) {
        PRINT("gpio operate failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    PRINT("[Params] PIN:%d , level: %d\r\n", pin, level);
    return TEST_SUITE_OK;
}

static int test_gpio_toggle(int argc, char *argv[])
{
    pin_t pin;
    int level = 0;
    errcode_t ret = ERRCODE_SUCC;

    if (argc >= TEST_PARAM_ARGC_2) {
        pin = (pin_t)strtol(argv[0], NULL, 0);
        level = strtol(argv[1], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    PRINT("[Params] PIN:%d, levle: %d. \r\n", pin, (uint32_t)level);
    ret = uapi_pin_set_mode(pin, HAL_PIO_FUNC_GPIO);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    ret = uapi_gpio_set_dir(pin, GPIO_DIRECTION_OUTPUT);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    if (level == 1) {
        uapi_gpio_set_val(pin, GPIO_LEVEL_HIGH);
    } else if (level == 0) {
        uapi_gpio_set_val(pin, GPIO_LEVEL_LOW);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    ret = uapi_gpio_toggle(pin);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    gpio_level_t gpio_level;
    gpio_level = uapi_gpio_get_val(pin);
    if (gpio_level != (gpio_level_t)level) {
        PRINT("gpio toggle success\r\n");
        PRINT("read level is %d\r\n", gpio_level);
    } else {
        PRINT("gpio toggle failed\r\n");
        PRINT("read level is %d\r\n", gpio_level);
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

static void gpio_callback_func(pin_t pin, uintptr_t param)
{
    UNUSED(pin);
    UNUSED(param);
    PRINT("PIN:%d interrupt success. \r\n", pin);
}

static int test_gpio_register(int argc, char *argv[])
{
    pin_t pin;
    uint32_t trigger = 0;
    if (argc >= TEST_PARAM_ARGC_2) {
        pin = (pin_t)strtol(argv[0], NULL, 0);
        trigger = (uint32_t)strtol(argv[1], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    PRINT("[Params] PIN:%d, Trigger: %d. \r\n", pin, trigger);

    if (trigger != GPIO_INTERRUPT_RISING_EDGE && trigger != GPIO_INTERRUPT_FALLING_EDGE &&
        trigger != GPIO_INTERRUPT_LOW && trigger != GPIO_INTERRUPT_HIGH && trigger != GPIO_INTERRUPT_DEDGE) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uapi_pin_set_mode(pin, HAL_PIO_FUNC_GPIO);
    uapi_gpio_set_dir(pin, GPIO_DIRECTION_INPUT);

    if (uapi_gpio_register_isr_func(pin, trigger, gpio_callback_func) != ERRCODE_SUCC) {
        PRINT("PIN: %d int registet failed. \r\n", pin);
        uapi_gpio_unregister_isr_func(pin);
        PRINT("PIN: %d int unregistet success. \r\n", pin);
        return TEST_SUITE_TEST_FAILED;
    }
    PRINT("PIN: %d int registet success. \r\n", pin);

    return TEST_SUITE_OK;
}

static int test_gpio_unregister(int argc, char *argv[])
{
    pin_t pin;
    if (argc > TEST_PARAM_ARGC_1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    pin = (pin_t)strtol(argv[0], NULL, 0);

    PRINT("[Params] PIN:%d. \r\n", pin);

    uapi_gpio_set_dir(pin, GPIO_DIRECTION_INPUT);

    if (uapi_gpio_unregister_isr_func(pin) != 0) {
        PRINT("PIN: %d int unregister failed. \r\n", pin);

        return TEST_SUITE_TEST_FAILED;
    }

    PRINT("PIN: %d int unregister success. \r\n", pin);

    return TEST_SUITE_OK;
}

static int test_gpio_disable_interrupt(int argc, char *argv[])
{
    pin_t pin;
    if (argc > TEST_PARAM_ARGC_1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    pin = (pin_t)strtol(argv[0], NULL, 0);

    PRINT("[Params] PIN:%d. \r\n", pin);

    if (uapi_gpio_disable_interrupt(pin) != 0) {
        PRINT("PIN: %d int disable failed. \r\n", pin);

        return TEST_SUITE_TEST_FAILED;
    }

    PRINT("PIN: %d int disable success. \r\n", pin);

    return TEST_SUITE_OK;
}

static int test_gpio_clear_interrupt(int argc, char *argv[])
{
    pin_t pin;
    if (argc > TEST_PARAM_ARGC_1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    pin = (pin_t)strtol(argv[0], NULL, 0);

    PRINT("[Params] PIN:%d. \r\n", pin);

    if (uapi_gpio_clear_interrupt(pin) != 0) {
        PRINT("PIN: %d int clear failed. \r\n", pin);

        return TEST_SUITE_TEST_FAILED;
    }
    PRINT("PIN: %d int clear success. \r\n", pin);

    return TEST_SUITE_OK;
}

static int test_gpio_enable_interrupt(int argc, char *argv[])
{
    pin_t pin;
    if (argc > TEST_PARAM_ARGC_1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    pin = (pin_t)strtol(argv[0], NULL, 0);

    PRINT("[Params] PIN:%d. \r\n", pin);

    if (uapi_gpio_enable_interrupt(pin) != 0) {
        PRINT("PIN: %d int enable failed. \r\n", pin);

        return TEST_SUITE_TEST_FAILED;
    }
    PRINT("PIN: %d int enable success. \r\n", pin);

    return TEST_SUITE_OK;
}

void add_gpio_test_case(void)
{
    uapi_gpio_init();
    uapi_test_suite_add_function("test_gpio_output", "GPIO output high or low. <PIN> <level>", test_gpio_output);
    uapi_test_suite_add_function("test_gpio_input", "GPIO input high or low. <PIN>", test_gpio_input);
    uapi_test_suite_add_function("test_gpio_setdir", "GPIO setdir output or input. <PIN> <level>", test_gpio_setdir);
    uapi_test_suite_add_function("test_gpio_getdir", "GPIO getdir output or input. <PIN>", test_gpio_getdir);
    uapi_test_suite_add_function("test_gpio_toggle", "GPIO  toggle. <Pin> <level>", test_gpio_toggle);
    uapi_test_suite_add_function("test_gpio_register",  "GPIO input interrupt. <PIN>, <INT>", test_gpio_register);
    uapi_test_suite_add_function("test_gpio_unregister",  "GPIO interrupt unregister. <PIN>", test_gpio_unregister);
    uapi_test_suite_add_function(
        "test_gpio_disable_interrupt", "GPIO interrupt disable. <PIN>", test_gpio_disable_interrupt);
    uapi_test_suite_add_function("test_gpio_clear_interrupt", "GPIO interrupt clear. <PIN>", test_gpio_clear_interrupt);
    uapi_test_suite_add_function(
        "test_gpio_enable_interrupt", "GPIO interrupt enable. <PIN>", test_gpio_enable_interrupt);
}
