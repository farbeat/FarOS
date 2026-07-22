/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test pwm source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */

#include "pwm.h"
#include "pwm_porting.h"
#include "pinctrl.h"

#include "debug_print.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "securec.h"
#include "pinctrl_porting.h"
#include "tcxo.h"
#include "irmalloc.h"
#include "errcode.h"
#include "common_def.h"
#include "test_pwm.h"

#define TEST_MAX_TIMES          5
#define TEST_TCXO_DELAY_1000MS  1000
#define TEST_PARAM_FOR_PWM_10   10
#define TEST_PARAM_FOR_PWM_200  200

static errcode_t pwm_test_callback(uint8_t channel)
{
    PRINT("PWM number is %d, func of interrupt start. \r\n", channel);
    uapi_pwm_isr(channel);
    return ERRCODE_SUCC;
}

static int test_pwm_repeat(int argc, char *argv[])
{
    pwm_channel_t channel;
    pin_t pin;
    pin_mode_t mode;
    uint32_t dalay_ms;
    pwm_config_t cfg_repeat = { 100, 100, 0xFF, 0xFF, true };

    if (argc == 6) { /* 6: Indicates the number of input parameters */
        uint32_t param_index = 0;
        channel = strtol(argv[param_index++], NULL, 0);
        cfg_repeat.low_time = strtol(argv[param_index++], NULL, 0);
        cfg_repeat.high_time = strtol(argv[param_index++], NULL, 0);
        pin = (pin_t)strtol(argv[param_index++], NULL, 0);
        mode = (pin_mode_t)strtol(argv[param_index++], NULL, 0);
        dalay_ms = (uint32_t)strtol(argv[param_index++], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    if (channel >= PWM_MAX_NUMBER) {
        PRINT("Paremeter Error. PWM max number is 5. \r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uapi_pwm_init();
    uapi_pin_set_mode(pin, mode);
    PRINT("+++++++ pwm init success +++++++ \r\n");

    /* test for repeat, interrupt only be call when stop or close */
    PRINT("+++++++ Start pwm repeat test +++++++ \r\n");
    uapi_tcxo_delay_ms((uint32_t)TEST_TCXO_DELAY_1000MS);
    uapi_pwm_open(channel, &cfg_repeat);
    uapi_pwm_unregister_interrupt(channel);
    uapi_pwm_register_interrupt(channel, pwm_test_callback);
#if defined(CONFIG_PWM_USING_V151)
    uapi_pwm_set_group(0, 1u << channel);
    uapi_pwm_start(0);
#else
    uapi_pwm_start(channel);
#endif
    uapi_tcxo_delay_ms((uint32_t)TEST_TCXO_DELAY_1000MS);

    for (uint32_t test_times = 1; test_times <= TEST_MAX_TIMES; test_times++) {
        PRINT("+++++++ pwm repeat interrupt +++++++ \r\n");
        if (test_times == TEST_MAX_TIMES) {
            PRINT("+++++++ pwm repeat ready to trigger interrupt +++++++ \r\n");
            uapi_pwm_close(channel);
            PRINT("+++++++ pwm repeat exit interrupt +++++++ \r\n");
        }
        uapi_tcxo_delay_ms(dalay_ms);
    }

    uapi_tcxo_delay_ms((uint32_t)TEST_TCXO_DELAY_1000MS);
    PRINT("+++++++ pwm repeat test ok +++++++ \r\n");
    uapi_pwm_deinit();
    return TEST_SUITE_OK;
}

static int test_pwm_no_repeat(int argc, char *argv[])
{
    pwm_channel_t channel;
    uint32_t i;
    pin_t pin;
    pin_mode_t mode;
    uint32_t dalay_ms;
    pwm_config_t cfg_no_repeat = { 100, 100, 0xFF, 0xFF, false };

    if (argc == 6) { /* 6: Indicates the number of input parameters */
        uint32_t param_index = 0;
        channel = strtol(argv[param_index++], NULL, 0);
        cfg_no_repeat.low_time = strtol(argv[param_index++], NULL, 0);
        cfg_no_repeat.high_time = strtol(argv[param_index++], NULL, 0);
        pin = (pin_t)strtol(argv[param_index++], NULL, 0);
        mode = (pin_mode_t)strtol(argv[param_index++], NULL, 0);
        dalay_ms = (uint32_t)strtol(argv[param_index++], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    if (channel >= PWM_MAX_NUMBER) {
        PRINT("Paremeter Error. \r\n");
        PRINT("PWM max number is 5.\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uapi_pwm_init();
    uapi_pin_set_mode(pin, mode);
    PRINT("+++++++ pwm init success +++++++ \r\n");

    /* test for no repeat, interrupt will be call after all repeat cycles done */
    PRINT("+++++++ Start pwm no repeat test +++++++ \r\n");
    uapi_tcxo_delay_ms((uint32_t)TEST_TCXO_DELAY_1000MS);
    uapi_pwm_open(channel, &cfg_no_repeat);
    uapi_pwm_unregister_interrupt(channel);
    uapi_pwm_register_interrupt(channel, pwm_test_callback);
    uapi_pwm_start(channel);

    for (i = 0; i <= TEST_MAX_TIMES; i++) {
        PRINT("+++++++ pwm no repeat no interrupt +++++++ \r\n");
        uapi_tcxo_delay_ms(dalay_ms);
    }

    uapi_pwm_close(channel);
    uapi_tcxo_delay_ms((uint32_t)TEST_TCXO_DELAY_1000MS);
    PRINT("+++++++ pwm no repeat test ok +++++++ \r\n");
    uapi_pwm_deinit();
    return TEST_SUITE_OK;
}

static int test_pwm_duty_cycle(int argc, char *argv[])
{
    pwm_channel_t channel;
    uint32_t low_time;
    uint32_t high_time;
    uint32_t count = 0;
    pin_t pin;
    pin_mode_t mode;
    pwm_config_t cfg_duty_cycle = { 100, 100, 0xFF, 0xFF, true };

    if (argc == 3) { /* 3: Indicates the number of input parameters */
        uint32_t param_index = 0;
        channel = strtol(argv[param_index++], NULL, 0);
        pin = (pin_t)strtol(argv[param_index++], NULL, 0);
        mode = (pin_mode_t)strtol(argv[param_index++], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    if (channel >= PWM_MAX_NUMBER) {
        PRINT("Paremeter Error. \r\n");
        PRINT("PWM max number is 5.\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uapi_pwm_init();
    uapi_pin_set_mode(pin, mode);
    PRINT("+++++++ pwm init success +++++++ \r\n");

    /* test for update duty ratio */
    PRINT("+++++++ Start pwm update duty ratio test +++++++ \r\n");
    uapi_tcxo_delay_ms((uint64_t)TEST_TCXO_DELAY_1000MS);
    uapi_pwm_open(channel, &cfg_duty_cycle);
    uapi_pwm_unregister_interrupt(channel);
    uapi_pwm_register_interrupt(channel, pwm_test_callback);
    uapi_pwm_start(channel);

    for (low_time = TEST_PARAM_FOR_PWM_10; low_time < TEST_PARAM_FOR_PWM_200; low_time += TEST_PARAM_FOR_PWM_10) {
        high_time = TEST_PARAM_FOR_PWM_200 - low_time;
#if defined(CONFIG_PWM_USING_V150)
        uapi_pwm_update_duty_ratio(channel, low_time, high_time);
#endif
        uapi_tcxo_delay_ms((uint64_t)TEST_TCXO_DELAY_1000MS);
        count++;
        PRINT("pwm update duty ratio count: %d low_time: %d high_time: %d \r\n", count, low_time, high_time);
    }

    uapi_pwm_close(channel);
    PRINT("+++++++ pwm update duty ratio test ok +++++++ \r\n");
    uapi_pwm_deinit();
    unused(high_time);
    return TEST_SUITE_OK;
}

void add_pwm_test_case(void)
{
    uapi_pin_init();
    uapi_test_suite_add_function("pwm_repeat_test", "pwm repeat test. Params: <channel>, <low time>, <high time> "
                 "<pin number>, <pin mode>, <delay time(ms)>", test_pwm_repeat);
    uapi_test_suite_add_function("pwm_no_repeat_test", "pwm repeat test. Params: <channel>, <low time>, <high time> "
                 "<pin number>, <pin mode>, <delay time(ms)>", test_pwm_no_repeat);
    uapi_test_suite_add_function("pwm_duty_cycle_test", "pwm duty cycle test. Params: <channel>, "
                 "<pin number>, <pin mode>", test_pwm_duty_cycle);
}
