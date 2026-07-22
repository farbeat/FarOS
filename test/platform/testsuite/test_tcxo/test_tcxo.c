/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test tcxo source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */

#include "test_tcxo.h"
#include "tcxo.h"
#include "debug_print.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "systick_porting.h"
#include "systick.h"
#include "irmalloc.h"
#include "errcode.h"
#include "securec.h"
#define TCXO_SYSTICK_COUNT_ERROR 30
#define TCXO_SOFT_COUNT_ERROR 30
#define TCXO_US_ERROR_PPM 20000
#define TCXO_MS_ERROR_PPM 20
#define TCXO_US_TO_MS 1000
#define SYSTICK_DELAY_MS 1000
#define SYSTICK_DELAY_US 10000
#define SYSTICK_DELAY_US_ERROR 300
#define TEST_LOOP_MAX 128

/* 软件延迟误差 */
static uint32_t g_tcxo_soft_count_error = TCXO_SOFT_COUNT_ERROR;

static int test_tcxo_get_ms(int argc, char *argv[])
{
    uint32_t loop_times;
    uint64_t count_before_get_ms;
    uint64_t count_after_get_ms;
    uint64_t count = 0;

    // Get the parameters
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    loop_times = (uint32_t)strtol(argv[0], NULL, 0);
    loop_times = loop_times > TEST_LOOP_MAX ? TEST_LOOP_MAX : loop_times;
    if (loop_times <= 0) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uapi_tcxo_init();

    for (uint8_t i = 0; i < loop_times; i++) {
        PRINT("uapi_tcxo_get_ms. \r\n");
        count_before_get_ms = uapi_tcxo_get_ms();
        (void)uapi_systick_delay_ms(SYSTICK_DELAY_MS);
        count_after_get_ms = uapi_tcxo_get_ms();
        if (count_after_get_ms > count_before_get_ms) {
            PRINT("tcxo work normall. \r\n");
        } else {
            PRINT("tcxo work error. \r\n");
        }
        count += (count_after_get_ms - count_before_get_ms);
    }
    PRINT("delay(ms): %d. \r\n", SYSTICK_DELAY_MS);
    PRINT("count: %d. \r\n", (uint32_t)(count / loop_times));

    if ((count / loop_times) == SYSTICK_DELAY_MS) {
        return TEST_SUITE_OK;
    } else {
        return TEST_SUITE_TEST_FAILED;
    }

    uapi_tcxo_deinit();
}

static int test_tcxo_get_us(int argc, char *argv[])
{
    uint32_t loop_times;
    uint64_t count_before_get_us;
    uint64_t count_after_get_us;
    uint64_t count = 0;
    int32_t delay_error_cur;
    int32_t delay_error_max;
    int32_t delay_error_min;

    // Get the parameters
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    loop_times = (uint32_t)strtol(argv[0], NULL, 0);
    loop_times = loop_times > TEST_LOOP_MAX ? TEST_LOOP_MAX : loop_times;
    if (loop_times <= 0) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uapi_tcxo_init();

    for (uint8_t i = 0; i < loop_times; i++) {
        PRINT("uapi_tcxo_get_us. \r\n");
        count_before_get_us = uapi_tcxo_get_us();
        (void)uapi_systick_delay_us(SYSTICK_DELAY_US);
        count_after_get_us = uapi_tcxo_get_us();
        if (count_after_get_us > count_before_get_us) {
            PRINT("tcxo work normall. \r\n");
        } else {
            PRINT("tcxo work error. \r\n");
        }
        count += (count_after_get_us - count_before_get_us);
    }
    PRINT("delay(us): %d. \r\n", SYSTICK_DELAY_US);
    PRINT("count: %d. \r\n", (uint32_t)(count / loop_times));

    delay_error_cur = count / loop_times - SYSTICK_DELAY_US;
    delay_error_max = g_tcxo_soft_count_error + SYSTICK_DELAY_US_ERROR;
    delay_error_min = -(SYSTICK_DELAY_US / TCXO_US_ERROR_PPM + TCXO_SYSTICK_COUNT_ERROR);
    if ((delay_error_cur <= delay_error_max) && (delay_error_cur >= delay_error_min)) {
        return TEST_SUITE_OK;
    } else {
        return TEST_SUITE_TEST_FAILED;
    }

    uapi_tcxo_deinit();
}

static int test_tcxo_delay_ms(int argc, char *argv[])
{
    uint32_t delay_ms;
    uint32_t loop_times;
    uint64_t count_before_delay_ms;
    uint64_t count_after_delay_ms;
    uint64_t count = 0;
    int32_t delay_error_cur;
    int32_t delay_error_max;
    int32_t delay_error_min;
    errcode_t ret = ERRCODE_FAIL;

    // Get the parameters
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    delay_ms = (uint32_t)strtol(argv[0], NULL, 0);
    loop_times = (uint32_t)strtol(argv[1], NULL, 0);
    loop_times = loop_times > TEST_LOOP_MAX ? TEST_LOOP_MAX : loop_times;
    if (loop_times <= 0) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uapi_tcxo_init();

    for (uint8_t i = 0; i < loop_times; i++) {
        PRINT("uapi_tcxo_delay_ms. \r\n");
        count_before_delay_ms = uapi_systick_get_ms();
        ret = uapi_tcxo_delay_ms(delay_ms);
        count_after_delay_ms = uapi_systick_get_ms();
        PRINT("uapi_tcxo_delay_ms %s. \r\n", ret == ERRCODE_SUCC ? "successful" : "Failed");
        if (count_after_delay_ms > count_before_delay_ms) {
            PRINT("tcxo work normall. \r\n");
        } else {
            PRINT("tcxo work error. \r\n");
        }
        count += (count_after_delay_ms - count_before_delay_ms);
    }
    PRINT("delay(ms): %d. \r\n", delay_ms);
    PRINT("count: %d. \r\n", (uint32_t)(count / loop_times));

    delay_error_cur = count / loop_times - delay_ms;
    delay_error_max = (TCXO_SYSTICK_COUNT_ERROR + g_tcxo_soft_count_error +
                       (delay_ms / TCXO_MS_ERROR_PPM)) / TCXO_US_TO_MS + 1;
    delay_error_min = -((delay_ms / TCXO_MS_ERROR_PPM + TCXO_SYSTICK_COUNT_ERROR) / TCXO_US_TO_MS + 1);
    if ((delay_error_cur <= delay_error_max) && (delay_error_cur >= delay_error_min)) {
        return TEST_SUITE_OK;
    } else {
        return TEST_SUITE_TEST_FAILED;
    }

    uapi_tcxo_deinit();
}

static int test_tcxo_delay_us(int argc, char *argv[])
{
    uint32_t delay_us;
    uint32_t loop_times;
    uint64_t count_before_delay_us;
    uint64_t count_after_delay_us;
    uint64_t count = 0;
    int32_t delay_error_cur;
    int32_t delay_error_max;
    int32_t delay_error_min;
    errcode_t ret = ERRCODE_FAIL;

    // Get the parameters
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    delay_us = (uint32_t)strtol(argv[0], NULL, 0);
    loop_times = (uint32_t)strtol(argv[1], NULL, 0);
    loop_times = loop_times > TEST_LOOP_MAX ? TEST_LOOP_MAX : loop_times;
    if (loop_times <= 0) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uapi_tcxo_init();

    for (uint8_t i = 0; i < loop_times; i++) {
        PRINT("uapi_tcxo_delay_us. \r\n");
        count_before_delay_us = uapi_systick_get_us();
        ret = uapi_tcxo_delay_us(delay_us);
        count_after_delay_us = uapi_systick_get_us();
        PRINT("uapi_tcxo_delay_us %s. \r\n", ret == ERRCODE_SUCC ? "successful" : "Failed");
        if (count_after_delay_us > count_before_delay_us) {
            PRINT("tcxo work normall. \r\n");
        } else {
            PRINT("tcxo work error. \r\n");
        }
        count += (count_after_delay_us - count_before_delay_us);
    }
    PRINT("delay(us): %d. \r\n", delay_us);
    PRINT("count: %d. \r\n", (uint32_t)(count / loop_times));

    delay_error_cur = count / loop_times - delay_us;
    delay_error_max = TCXO_SYSTICK_COUNT_ERROR + g_tcxo_soft_count_error + (delay_us / TCXO_US_ERROR_PPM);
    delay_error_min = -(delay_us / TCXO_US_ERROR_PPM + TCXO_SYSTICK_COUNT_ERROR);
    if ((delay_error_cur <= delay_error_max) && (delay_error_cur >= delay_error_min)) {
        return TEST_SUITE_OK;
    } else {
        return TEST_SUITE_TEST_FAILED;
    }

    uapi_tcxo_deinit();
}

static int test_tcxo_soft_count_error_set(int argc, char *argv[])
{
    if (argc != 1) { /* 1: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t tcxo_soft_count_error = (uint32_t)strtol(argv[0], NULL, 0);
    PRINT("Set tcxo_soft_count_error[%u] -> [%u] \r\n", g_tcxo_soft_count_error, tcxo_soft_count_error);
    g_tcxo_soft_count_error = tcxo_soft_count_error;

    return TEST_SUITE_OK;
}

void add_tcxo_test_case(void)
{
    systick_port_cali_xclk();
    uapi_test_suite_add_function("test_cfbb_tcxo_get_ms", "Params: <Delaytimes(1~128)>", test_tcxo_get_ms);
    uapi_test_suite_add_function("test_cfbb_tcxo_get_us", "Params: <Delaytimes(1~128)>", test_tcxo_get_us);
    uapi_test_suite_add_function(
        "test_cfbb_tcxo_delay_ms", "Params: <DelayTime(ms)>, <Delaytimes(1~128)>", test_tcxo_delay_ms);
    uapi_test_suite_add_function(
        "test_cfbb_tcxo_delay_us", "Params: <DelayTime(us)>, <Delaytimes(1~128)>", test_tcxo_delay_us);
    uapi_test_suite_add_function("test_tcxo_soft_count_error_set", "Params: <tcxo_soft_count_error(us, default 30)>",
        test_tcxo_soft_count_error_set);
}
