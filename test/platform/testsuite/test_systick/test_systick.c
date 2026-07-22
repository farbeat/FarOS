/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test systick source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-11, Create file. \n
 */
#include "debug_print.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "securec.h"

#include "tcxo.h"
#include "systick.h"
#include "tcxo_porting.h"
#include "systick_porting.h"
#include "test_systick.h"

#define SYSTICK_COUNT_ERROR 10
#define SYSTICK_US_ERROR 50
#define TEST_LOOP_MAX 128

static int test_systick_count(int argc, char *argv[])
{
    uint32_t delay_ms;
    uint32_t test_loop;
    uint64_t count_before_delay;
    uint64_t count_after_delay;
    uint64_t count = 0;

    /* Get the parameters */
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    delay_ms = (uint32_t)strtol(argv[0], NULL, 0);
    test_loop = (uint32_t)strtol(argv[1], NULL, 0);
    if (test_loop == 0) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_loop = test_loop > TEST_LOOP_MAX ? TEST_LOOP_MAX : test_loop;

    for (uint8_t i = 0; i < test_loop; i++) {
        count_before_delay = uapi_systick_get_count();
        uapi_tcxo_delay_ms((uint64_t)delay_ms);
        count_after_delay = uapi_systick_get_count();
        PRINT("systick work normall. \r\n");
        count += (count_after_delay - count_before_delay);
    }
    PRINT("delay(ms): %d. \r\n", delay_ms);
    PRINT("count: %d. \r\n", (uint32_t)(count / test_loop));

    int32_t err = convert_us_2_count((uint64_t)delay_ms * SYSTICK_US_ERROR / (MS_PER_S)) + SYSTICK_COUNT_ERROR;
    int32_t real_err = (count / test_loop) - convert_ms_2_count((uint64_t)delay_ms);
    if (real_err > err || real_err < -err) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

static int test_systick_s(int argc, char *argv[])
{
    uint32_t delay_s;
    uint32_t test_loop;
    uint64_t s_before_delay;
    uint64_t s_after_delay;
    uint64_t s = 0;

    /* Get the parameters */
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    delay_s = (uint32_t)strtol(argv[0], NULL, 0);
    test_loop = (uint32_t)strtol(argv[1], NULL, 0);
    if (test_loop == 0) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_loop = test_loop > TEST_LOOP_MAX ? TEST_LOOP_MAX : test_loop;

    for (uint8_t i = 0; i < test_loop; i++) {
        s_before_delay = uapi_systick_get_s();
        uapi_tcxo_delay_ms((uint64_t)(delay_s * MS_PER_S));
        s_after_delay = uapi_systick_get_s();
        PRINT("systick work normall. \r\n");
        s += (s_after_delay - s_before_delay);
    }
    PRINT("delay(s): %d. \r\n", delay_s);
    PRINT("s: %d. \r\n", (uint32_t)(s / test_loop));

    int32_t err = (uint64_t)delay_s * SYSTICK_US_ERROR / (MS_PER_S * US_PER_MS) +
        convert_count_2_s(SYSTICK_COUNT_ERROR);
    int32_t real_err = (s / test_loop) - delay_s;
    if (real_err > err || real_err < -err) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

static int test_systick_ms(int argc, char *argv[])
{
    uint32_t delay_ms;
    uint32_t test_loop;
    uint64_t ms_before_delay;
    uint64_t ms_after_delay;
    uint64_t ms = 0;

    /* Get the parameters */
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    delay_ms = (uint32_t)strtol(argv[0], NULL, 0);
    test_loop = (uint32_t)strtol(argv[1], NULL, 0);
    if (test_loop == 0) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_loop = test_loop > TEST_LOOP_MAX ? TEST_LOOP_MAX : test_loop;

    for (uint8_t i = 0; i < test_loop; i++) {
        ms_before_delay = uapi_systick_get_ms();
        uapi_tcxo_delay_ms((uint64_t)delay_ms);
        ms_after_delay = uapi_systick_get_ms();
        PRINT("systick work normall. \r\n");
        ms += (ms_after_delay - ms_before_delay);
    }
    PRINT("delay(ms): %d. \r\n", delay_ms);
    PRINT("ms: %d. \r\n", (uint32_t)(ms / test_loop));

    int32_t err = delay_ms * SYSTICK_US_ERROR / (MS_PER_S * US_PER_MS) + 1;
    int32_t real_err = (ms / test_loop) - delay_ms;
    if (real_err > err || real_err < -err) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

static int test_systick_us(int argc, char *argv[])
{
    uint32_t delay_us;
    uint32_t test_loop;
    uint64_t us_before_delay;
    uint64_t us_after_delay;
    uint64_t us = 0;

    /* Get the parameters */
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    delay_us = (uint32_t)strtol(argv[0], NULL, 0);
    test_loop = (uint32_t)strtol(argv[1], NULL, 0);
    if (test_loop == 0) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_loop = test_loop > TEST_LOOP_MAX ? TEST_LOOP_MAX : test_loop;

    for (uint8_t i = 0; i < test_loop; i++) {
        us_before_delay = uapi_systick_get_us();
        uapi_tcxo_delay_us((uint64_t)delay_us);
        us_after_delay = uapi_systick_get_us();
        PRINT("systick work normall. \r\n");
        us += (us_after_delay - us_before_delay);
    }
    PRINT("delay(us): %d. \r\n", delay_us);
    PRINT("us: %d. \r\n", (uint32_t)(us / test_loop));

    int32_t err = (uint64_t)delay_us * SYSTICK_US_ERROR / (MS_PER_S * US_PER_MS) +
        convert_count_2_us(SYSTICK_COUNT_ERROR);
    int32_t real_err = (us / test_loop) - delay_us;
    if (real_err > err || real_err < -err) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

static int test_systick_delay_count(int argc, char *argv[])
{
    uint32_t delay_count;
    uint32_t test_loop;
    uint64_t count_before_delay_count;
    uint64_t count_after_delay_count;
    uint64_t count = 0;

    /* Get the parameters */
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    delay_count = (uint32_t)strtol(argv[0], NULL, 0);
    test_loop = (uint32_t)strtol(argv[1], NULL, 0);
    if (test_loop == 0) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_loop = test_loop > TEST_LOOP_MAX ? TEST_LOOP_MAX : test_loop;

    for (uint8_t i = 0; i < test_loop; i++) {
        count_before_delay_count = convert_us_2_count(uapi_tcxo_get_count() / TCXO_TICKS_PER_U_SECOND);
        uapi_systick_delay_count(delay_count);
        count_after_delay_count = convert_us_2_count(uapi_tcxo_get_count() / TCXO_TICKS_PER_U_SECOND);
        count += (count_after_delay_count - count_before_delay_count);
        PRINT("systick work normall. \r\n");
    }
    PRINT("delay(count): %d. \r\n", delay_count);
    PRINT("count: %d. \r\n", (uint32_t)(count / test_loop));

    int32_t err = (uint64_t)delay_count * SYSTICK_US_ERROR / (MS_PER_S * US_PER_MS) + SYSTICK_COUNT_ERROR;
    int32_t real_err = (count / test_loop) - delay_count;
    if (real_err > err || real_err < -err) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

static int test_systick_delay_s(int argc, char *argv[])
{
    uint32_t delay_s;
    uint32_t test_loop;
    uint64_t count_before_delay_s;
    uint64_t count_after_delay_s;
    uint64_t s = 0;

    /* Get the parameters */
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    delay_s = (uint32_t)strtol(argv[0], NULL, 0);
    test_loop = (uint32_t)strtol(argv[1], NULL, 0);
    if (test_loop == 0) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_loop = test_loop > TEST_LOOP_MAX ? TEST_LOOP_MAX : test_loop;

    for (uint8_t i = 0; i < test_loop; i++) {
        count_before_delay_s = uapi_tcxo_get_ms() / MS_PER_S;
        uapi_systick_delay_s(delay_s);
        count_after_delay_s = uapi_tcxo_get_ms() / MS_PER_S;
        s += (count_after_delay_s - count_before_delay_s);
        PRINT("systick work normall. \r\n");
    }
    PRINT("delay(s): %d. \r\n", delay_s);
    PRINT("s: %d. \r\n", (uint32_t)(s / test_loop));

    int32_t err = (uint64_t)delay_s * SYSTICK_US_ERROR / (MS_PER_S * US_PER_MS) +
        convert_count_2_s(SYSTICK_COUNT_ERROR);
    int32_t real_err = (s / test_loop) - delay_s;
    if (real_err > err || real_err < -err) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

static int test_systick_delay_ms(int argc, char *argv[])
{
    uint32_t delay_ms;
    uint32_t test_loop;
    uint64_t count_before_delay_ms;
    uint64_t count_after_delay_ms;
    uint64_t ms = 0;

    /* Get the parameters */
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    delay_ms = (uint32_t)strtol(argv[0], NULL, 0);
    test_loop = (uint32_t)strtol(argv[1], NULL, 0);
    if (test_loop == 0) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_loop = test_loop > TEST_LOOP_MAX ? TEST_LOOP_MAX : test_loop;

    for (uint8_t i = 0; i < test_loop; i++) {
        count_before_delay_ms = uapi_tcxo_get_ms();
        uapi_systick_delay_ms(delay_ms);
        count_after_delay_ms = uapi_tcxo_get_ms();
        ms += (count_after_delay_ms - count_before_delay_ms);
        PRINT("systick work normall. \r\n");
    }
    PRINT("delay(ms): %d. \r\n", delay_ms);
    PRINT("ms: %d. \r\n", (uint32_t)(ms / test_loop));

    int32_t real_err = (ms / test_loop) - delay_ms;
    int32_t err = (uint64_t)delay_ms * SYSTICK_US_ERROR / (MS_PER_S * US_PER_MS) + 1;

    if (real_err > err || real_err < -err) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

static int test_systick_delay_us(int argc, char *argv[])
{
    uint32_t delay_us;
    uint32_t test_loop;
    uint64_t count_before_delay_us;
    uint64_t count_after_delay_us;
    uint64_t us = 0;

    /* Get the parameters */
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    delay_us = (uint32_t)strtol(argv[0], NULL, 0);
    test_loop = (uint32_t)strtol(argv[1], NULL, 0);
    if (test_loop == 0) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_loop = test_loop > TEST_LOOP_MAX ? TEST_LOOP_MAX : test_loop;

    for (uint8_t i = 0; i < test_loop; i++) {
        count_before_delay_us = uapi_tcxo_get_us();
        uapi_systick_delay_us(delay_us);
        count_after_delay_us = uapi_tcxo_get_us();
        us += (count_after_delay_us - count_before_delay_us);
        PRINT("systick work normall. \r\n");
    }
    PRINT("delay(us): %d. \r\n", delay_us);
    PRINT("us: %d. \r\n", (uint32_t)(us / test_loop));

    int32_t err = (uint64_t)delay_us * SYSTICK_US_ERROR / (MS_PER_S * US_PER_MS) +
        convert_count_2_us(SYSTICK_COUNT_ERROR);
    int32_t real_err = (us / test_loop) - delay_us;

    if (real_err > err || real_err < -err) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

static int test_systick_clear(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    uint64_t count_before_clear;
    uint64_t count_after_clear;
    uapi_systick_init();
    count_before_clear = uapi_systick_get_count();
    PRINT("systick_clear_count. \r\n");
    uapi_systick_count_clear();
    PRINT("systick_clear_count success. \r\n");
    count_after_clear = uapi_systick_get_count();

    PRINT("count befor clear: %d. \r\n", (uint32_t)count_before_clear);
    PRINT("count after clear: %d. \r\n", (uint32_t)count_after_clear);
    if (count_after_clear >= count_before_clear) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

static int test_systick_deinit(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    uapi_systick_deinit();
    uint64_t count;
    count = uapi_systick_get_count();

    PRINT("count: %d. \r\n", (uint32_t)count);
    uapi_systick_init();
    if (count != 0) {
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

void add_systick_test_case(void)
{
    uapi_systick_init();
    uapi_tcxo_init();
    systick_port_cali_xclk();
    uapi_test_suite_add_function(
        "test_systick_count", "Params: <DelayTime(ms)>, <test_loop(10~128)>", test_systick_count);
    uapi_test_suite_add_function("test_systick_s", "Params: <DelayTime(s)>, <test_loop(10~128)>", test_systick_s);
    uapi_test_suite_add_function("test_systick_ms", "Params: <DelayTime(ms)>, <test_loop(10~128)>", test_systick_ms);
    uapi_test_suite_add_function("test_systick_us", "Params: <DelayTime(us)>, <test_loop(10~128)>", test_systick_us);
    uapi_test_suite_add_function(
        "test_systick_delay_count", "Params: <DelayTime(count)>, <test_loop(10~128)>", test_systick_delay_count);
    uapi_test_suite_add_function(
        "test_systick_delay_s", "Params: <DelayTime(s)>, <test_loop(10~128)>", test_systick_delay_s);
    uapi_test_suite_add_function(
        "test_systick_delay_ms", "Params: <DelayTime(ms)>, <test_loop(10~128)>", test_systick_delay_ms);
    uapi_test_suite_add_function(
        "test_systick_delay_us", "Params: <DelayTime(us)>, <test_loop(10~128)>", test_systick_delay_us);
    uapi_test_suite_add_function("test_systick_clear", "Params: NULL", test_systick_clear);
    uapi_test_suite_add_function("test_systick_deinit", "Params: NULL", test_systick_deinit);
}