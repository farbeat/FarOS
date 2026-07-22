/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides uart driver source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-09, Create file. \n
 */
#include "test_rtc_unified.h"
#include "test_suite.h"
#include "test_suite_errors.h"

#include "arch_port.h"
#include "tcxo.h"
#include "securec.h"
#include "oal_interface.h"
#include "errcode.h"
#include "rtc.h"
#include "chip_io.h"
#include "test_suite_uart.h"

#define RTC_TEST_DOUBULE      2
#define RTC_MAX_NUMBER        3
#define TEST_RTC_DELAY        1000
#define TEST_RTC_AUCINTPRI    6
#define RTC_MAX_TIMERS        16
#define RTC_2_BASE_ADDR                       (RTC_BASE_ADDR + 0x28)

typedef struct rtcs {
    uint64_t start_time;
    uint64_t end_time;
} rtcs_t;

static volatile rtcs_t rtc[RTC_MAX_TIMERS];

static uint32_t g_rtc_max_num = CONFIG_RTC_MAX_NUM;
static uint32_t g_test_rtc_index = DEFAULT_TEST_RTC;
static uint32_t g_test_rtc_irq_list[CONFIG_RTC_MAX_NUM] = {
    RTC_0_IRQN,
#if defined(CONFIG_RTC_MAX_NUM) && (CONFIG_RTC_MAX_NUM > 1)
    RTC_1_IRQN,
#if defined(CONFIG_RTC_MAX_NUM) && (CONFIG_RTC_MAX_NUM > 2)
    RTC_2_IRQN,
#if defined(CONFIG_RTC_MAX_NUM) && (CONFIG_RTC_MAX_NUM > 3)
    RTC_3_IRQN,
#endif
#endif
#endif
};

#define RTC_TEST_CALLBACK_FLAG_INIT \
    do {                              \
        g_testsuit_rtc_success = 0; \
        g_testsuit_rtc_int = 0;     \
    } while (0)

/* Use Case FLAG Variable Definition Declaration. */
static volatile uint32_t g_testsuit_rtc_success = 0;
static volatile uint32_t g_testsuit_rtc_int = 0;

/* Timed task callback function list. */
static void testsuite_rtc2_irq(uintptr_t data)
{
    test_suite_uart_sendf("[INFO]testsuite_rtc2_irq,task=%u.\r\n", data);
    uint32_t index_tmp = (uint32_t)data;
    rtc[index_tmp].end_time = uapi_tcxo_get_us();
    g_testsuit_rtc_success++;
    g_testsuit_rtc_int++;
}

static void testsuite_rtc2_reset_irq(uintptr_t data)
{
    test_suite_uart_sendf("[INFO]testsuite_rtc2_reset_irq,task=%u.\r\n", data);
    g_testsuit_rtc_success++;
    g_testsuit_rtc_int += RTC_TEST_DOUBULE;
}

static int test_rtc_start_rtcs(uint32_t rtc_num, rtc_handle_t *rtc_handles, uint32_t delay)
{
    errcode_t retcode;
    for (uint32_t i = 0; i < rtc_num; i++) {
        retcode = uapi_rtc_create(g_test_rtc_index, &rtc_handles[i]);
        if (retcode != ERRCODE_SUCC) {
            test_suite_uart_sendf("RTC CREATE FAULT, hw_rtc[%u], sw_rtc[%u], retcode[0x%x].\r\n",
                g_test_rtc_index, i, retcode);
            return TEST_SUITE_TEST_FAILED;
        }
        test_suite_uart_sendf("RTC CREATE SUCCESS, hw_rtc[%u], sw_rtc[%u], rtc_handler[%d].\r\n",
            g_test_rtc_index, i, (int)rtc_handles[i]);
        rtc[i].start_time = uapi_tcxo_get_us();
        retcode = uapi_rtc_start(rtc_handles[i], delay, testsuite_rtc2_irq, i);
        if (retcode != ERRCODE_SUCC) {
            test_suite_uart_sendf("RTC START FAULT, hw_rtc[%u], sw_rtc[%u], retcode[0x%x].\r\n",
                g_test_rtc_index, i, retcode);
            return TEST_SUITE_TEST_FAILED;
        }
    }
    return TEST_SUITE_OK;
}

static int test_rtc_delete_rtcs(uint32_t rtc_num, rtc_handle_t *rtc_handles)
{
    errcode_t retcode;
    for (uint32_t i = 0; i < rtc_num; i++) {
        retcode = uapi_rtc_stop(rtc_handles[i]);
        if (retcode != ERRCODE_SUCC) {
            test_suite_uart_sendf("RTC STOP FAULT, sw_rtc[%u], retcode[0x%x].\r\n", i, retcode);
            return TEST_SUITE_TEST_FAILED;
        }
        retcode = uapi_rtc_delete(rtc_handles[i]);
        if (retcode != ERRCODE_SUCC) {
            test_suite_uart_sendf("RTC DELETE FAULT, sw_rtc[%u], retcode[0x%x].\r\n", i, retcode);
            return TEST_SUITE_TEST_FAILED;
        }
    }
    return TEST_SUITE_OK;
}

/* rtc init for base test. */
int test_rtc_init(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    uapi_rtc_init();
    test_suite_uart_sendf("rtc init succ\r\n");

    uapi_rtc_adapter(g_test_rtc_index, g_test_rtc_irq_list[g_test_rtc_index],
        irq_prio(g_test_rtc_irq_list[g_test_rtc_index]));
    uapi_tcxo_delay_ms(TEST_RTC_DELAY);

    return TEST_SUITE_OK;
}

int test_rtc_start(int argc, char *argv[])
{
    uint32_t rtc_num, delay;
    errcode_t retcode;
    rtc_handle_t rtc_tmp[RTC_MAX_TIMERS] = {0};

    /* Get the parameters */
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    RTC_TEST_CALLBACK_FLAG_INIT;

    rtc_num = (uint32_t)strtol(argv[0], NULL, 0);
    delay = (uint32_t)strtol(argv[1], NULL, 0);

    test_suite_uart_sendf("rtc start succ,num:%d,delay:%d\r\n", rtc_num, delay);
    if (rtc_num > RTC_MAX_TIMERS) {
        test_suite_uart_sendf("Wrong para: rtc num exceed maximum(%d).\r\n", RTC_MAX_TIMERS);
        return TEST_SUITE_CONFIG_FAILED;
    }

    uapi_rtc_init();
    uapi_rtc_adapter(g_test_rtc_index, g_test_rtc_irq_list[g_test_rtc_index],
        irq_prio(g_test_rtc_irq_list[g_test_rtc_index]));

    retcode = test_rtc_start_rtcs(rtc_num, rtc_tmp, delay);
    if (retcode != TEST_SUITE_OK) {
        test_rtc_delete_rtcs(rtc_num, rtc_tmp);
        return TEST_SUITE_TEST_FAILED;
    }
    test_suite_uart_sendf("g_testsuit_rtc_success:   (%d).\r\n", g_testsuit_rtc_success);
    while (g_testsuit_rtc_success != rtc_num) {
    }
    if (g_testsuit_rtc_int == rtc_num) {
        test_suite_uart_sendf("RTC IRQ FINISH.\r\n");
    }

    test_rtc_delete_rtcs(rtc_num, rtc_tmp);

    for (uint32_t i = 0; i < rtc_num; i++) {
        test_suite_uart_sendf("real time[%d] = %dus  ", i, (uint32_t)(rtc[i].end_time -  rtc[i].start_time));
        test_suite_uart_sendf("  delay = %dus\r\n", delay * TEST_RTC_DELAY);
    }

    return TEST_SUITE_OK;
}

int test_rtc_start_diff_times(int argc, char *argv[])
{
    uint32_t rtc_num;
    uint32_t delay[RTC_MAX_TIMERS] = {0};
    errcode_t retcode;
    rtc_handle_t rtc_tmp[RTC_MAX_TIMERS] = {0};

    RTC_TEST_CALLBACK_FLAG_INIT;
    rtc_num = (uint32_t)strtol(argv[0], NULL, 0);
    if (rtc_num > RTC_MAX_TIMERS) {
        test_suite_uart_sendf("Wrong para: timer num exceed maximum(%d).\r\n", RTC_MAX_TIMERS);
        return TEST_SUITE_CONFIG_FAILED;
    }

    if ((argc - 1) != (int)rtc_num) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    for (uint32_t i = 0; i < rtc_num; i++) {
        delay[i] = (uint32_t)strtol(argv[i + 1], NULL, 0);
    }
    uapi_rtc_init();
    uapi_rtc_adapter(g_test_rtc_index, g_test_rtc_irq_list[g_test_rtc_index],
        irq_prio(g_test_rtc_irq_list[g_test_rtc_index]));

    for (uint32_t i = 0; i < rtc_num; i++) {
        retcode = uapi_rtc_create(g_test_rtc_index, &rtc_tmp[i]);
        if (retcode != ERRCODE_SUCC) {
            test_suite_uart_sendf("TIMER CREATE FAULT(%d).\r\n", retcode);
            test_rtc_delete_rtcs(rtc_num, rtc_tmp);
            return TEST_SUITE_TEST_FAILED;
        }
        test_suite_uart_sendf("TIMER CREATE SUCCESS(%d).\r\n", (int)rtc_tmp[i]);
        retcode = uapi_rtc_start(rtc_tmp[i], delay[i], testsuite_rtc2_irq, i);
        if (retcode != ERRCODE_SUCC) {
            test_suite_uart_sendf("TIMER %u START FAULT(%d).\r\n", i, retcode);
            test_rtc_delete_rtcs(rtc_num, rtc_tmp);
            return TEST_SUITE_TEST_FAILED;
        }
        uapi_tcxo_delay_ms(delay[i] / rtc_num);
    }

    while (g_testsuit_rtc_success != rtc_num) {
    }

    if (g_testsuit_rtc_int == rtc_num) {
        test_suite_uart_sendf("TIMER IRQ FINISH.\r\n");
    }
    test_rtc_delete_rtcs(rtc_num, rtc_tmp);

    return TEST_SUITE_OK;
}

/* Before the previous batch of timer tasks time out, reset the timer tasks, can start more than one, \
   control the number through TimerNums.  */
int test_rtc_reset_timerinfo(int argc, char *argv[])
{
    uint32_t rtc_num, firstdelay, resetdelay;
    errcode_t retcode;
    uint32_t para_index = 0;
    rtc_handle_t rtc_tmp[RTC_MAX_TIMERS] = {0};

    /* Get the parameters */
    if (argc != 3) { /* 3: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    RTC_TEST_CALLBACK_FLAG_INIT;

    rtc_num = (uint32_t)strtol(argv[para_index], NULL, 0);
    para_index++;
    firstdelay = (uint32_t)strtol(argv[para_index], NULL, 0);
    para_index++;
    /* indicates the time to reset. */
    resetdelay = (uint32_t)strtol(argv[para_index], NULL, 0);

    if (rtc_num > RTC_MAX_TIMERS) {
        test_suite_uart_sendf("Wrong para: timer num exceed maximum(%d).\r\n", RTC_MAX_TIMERS);
        return TEST_SUITE_CONFIG_FAILED;
    }

    uapi_rtc_init();
    uapi_rtc_adapter(g_test_rtc_index, g_test_rtc_irq_list[g_test_rtc_index],
        irq_prio(g_test_rtc_irq_list[g_test_rtc_index]));
    retcode = test_rtc_start_rtcs(rtc_num, rtc_tmp, firstdelay);
    if (retcode != TEST_SUITE_OK) {
        test_rtc_delete_rtcs(rtc_num, rtc_tmp);
        return TEST_SUITE_TEST_FAILED;
    }
    for (uint32_t i = 0; i < rtc_num; i++) {
        retcode = uapi_rtc_start(rtc_tmp[i], resetdelay, testsuite_rtc2_reset_irq, i);
        if (retcode != ERRCODE_SUCC) {
            test_suite_uart_sendf("TIMER START FAULT(%d).\r\n", retcode);
            test_rtc_delete_rtcs(rtc_num, rtc_tmp);
            return TEST_SUITE_TEST_FAILED;
        }
    }

    while (g_testsuit_rtc_success != rtc_num) {
    }
    test_rtc_delete_rtcs(rtc_num, rtc_tmp);

    if (g_testsuit_rtc_int == RTC_TEST_DOUBULE * rtc_num) {
        test_suite_uart_sendf("RTC IRQ FINISH.\r\n");
    }
    return TEST_SUITE_OK;
}

/* Delay 1s before the previous batch of timer tasks time out, then reset the timer tasks, \
can start more than one, control the number through TimerNums. */
int test_rtc_delay_reset_timerinfo(int argc, char *argv[])
{
    uint32_t rtc_num, firstdelay, resetdelay;
    errcode_t retcode;
    uint32_t para_index = 0;
    rtc_handle_t rtc_tmp[RTC_MAX_TIMERS] = {0};

    /* Get the parameters */
    if (argc != 3) { /* 3: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    RTC_TEST_CALLBACK_FLAG_INIT;

    rtc_num = (uint32_t)strtol(argv[para_index], NULL, 0);
    para_index++;
    firstdelay = (uint32_t)strtol(argv[para_index], NULL, 0);
    para_index++;
    /* indicates the time to reset. */
    resetdelay = (uint32_t)strtol(argv[para_index], NULL, 0);
    if (rtc_num > RTC_MAX_TIMERS) {
        test_suite_uart_sendf("Wrong para: rtc num exceed maximum(%d).\r\n", RTC_MAX_TIMERS);
        return TEST_SUITE_CONFIG_FAILED;
    }

    uapi_rtc_init();
    uapi_rtc_adapter(g_test_rtc_index, g_test_rtc_irq_list[g_test_rtc_index],
        irq_prio(g_test_rtc_irq_list[g_test_rtc_index]));
    retcode = test_rtc_start_rtcs(rtc_num, rtc_tmp, firstdelay);
    if (retcode != TEST_SUITE_OK) {
        test_rtc_delete_rtcs(rtc_num, rtc_tmp);
        return TEST_SUITE_TEST_FAILED;
    }
    uapi_tcxo_delay_ms(TEST_RTC_DELAY);
    test_suite_uart_sendf("delay.\r\n");

    for (uint32_t i = 0, j = 0; i < rtc_num; i++) {
        retcode = uapi_rtc_start(rtc_tmp[j++], resetdelay, testsuite_rtc2_reset_irq, i);
        if (retcode != ERRCODE_SUCC) {
            test_suite_uart_sendf("RTC START FAULT(%d).\r\n", retcode);
            test_rtc_delete_rtcs(rtc_num, rtc_tmp);
            return TEST_SUITE_TEST_FAILED;
        }
    }

    while (g_testsuit_rtc_success != rtc_num) {
    }
    test_rtc_delete_rtcs(rtc_num, rtc_tmp);

    if (g_testsuit_rtc_int == RTC_TEST_DOUBULE * rtc_num) {
        test_suite_uart_sendf("RTC IRQ FINISH.\r\n");
    }
    return TEST_SUITE_OK;
}

/* Wait for the timer task to time out, restart the timer task, can start more than one, \
 control the number through TimerNums. */
int test_rtc_restart(int argc, char *argv[])
{
    uint32_t rtc_num, firstdelay, secdelay;
    errcode_t retcode;
    uint32_t para_index = 0;
    rtc_handle_t rtc_tmp[RTC_MAX_TIMERS] = {0};

    /* Get the parameters */
    if (argc != 3) { /* 3: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    RTC_TEST_CALLBACK_FLAG_INIT;

    rtc_num = (uint32_t)strtol(argv[para_index], NULL, 0);
    para_index++;
    firstdelay = (uint32_t)strtol(argv[para_index], NULL, 0);
    para_index++;
    /* indicates the second time to start. */
    secdelay = (uint32_t)strtol(argv[para_index], NULL, 0);
    test_suite_uart_sendf("rtc input succ,num:%d,firstdelay:%d,secdelay:%d\r\n", rtc_num, firstdelay, secdelay);
    if (rtc_num > RTC_MAX_TIMERS) {
        test_suite_uart_sendf("Wrong para: timer num exceed maximum(%d).\r\n", RTC_MAX_TIMERS);
        return TEST_SUITE_CONFIG_FAILED;
    }

    uapi_rtc_init();
    uapi_rtc_adapter(g_test_rtc_index, g_test_rtc_irq_list[g_test_rtc_index],
        irq_prio(g_test_rtc_irq_list[g_test_rtc_index]));
    retcode = test_rtc_start_rtcs(rtc_num, rtc_tmp, firstdelay);
    if (retcode != TEST_SUITE_OK) {
        test_rtc_delete_rtcs(rtc_num, rtc_tmp);
        return TEST_SUITE_TEST_FAILED;
    }
    while (g_testsuit_rtc_success != rtc_num) {
    }

    if (g_testsuit_rtc_int == rtc_num) {
        test_suite_uart_sendf("RTC IRQ FIRST DONE.\r\n");
    }
    RTC_TEST_CALLBACK_FLAG_INIT;
    for (uint32_t i = 0, j = 0; i < rtc_num; i++) {
        retcode = uapi_rtc_start(rtc_tmp[j++], secdelay, testsuite_rtc2_irq, i);
        if (retcode != ERRCODE_SUCC) {
            test_suite_uart_sendf("RTC %u START FAULT(%d).\r\n", i, retcode);
            test_rtc_delete_rtcs(rtc_num, rtc_tmp);
            return TEST_SUITE_TEST_FAILED;
        }
    }

    while (g_testsuit_rtc_success != rtc_num) {
    }
    test_rtc_delete_rtcs(rtc_num, rtc_tmp);

    if (g_testsuit_rtc_int == rtc_num) {
        test_suite_uart_sendf("RTC IRQ FINISH.\r\n");
    }

    return TEST_SUITE_OK;
}

/* Start timed tasks of different lengths with 1s intervals. */
int test_rtc_start_diff_times_intervals(int argc, char *argv[])
{
    uint32_t rtc_num;
    uint32_t delay[RTC_MAX_TIMERS] = {0};
    errcode_t retcode;
    rtc_handle_t rtc_tmp[RTC_MAX_TIMERS] = {0};

    RTC_TEST_CALLBACK_FLAG_INIT;
    rtc_num = (uint32_t)strtol(argv[0], NULL, 0);
    if (rtc_num > RTC_MAX_TIMERS) {
        test_suite_uart_sendf("Wrong para: rtc num exceed maximum(%d).\r\n", RTC_MAX_TIMERS);
        return TEST_SUITE_CONFIG_FAILED;
    }
    if ((argc - 1) != (int)rtc_num) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    for (uint32_t i = 0; i < rtc_num; i++) {
        delay[i] = (uint32_t)strtol(argv[i + 1], NULL, 0);
    }

    uapi_rtc_init();
    for (uint32_t i = 0; i < rtc_num; i++) {
        retcode = uapi_rtc_create(g_test_rtc_index, &rtc_tmp[i]);
        if (retcode != ERRCODE_SUCC) {
            test_suite_uart_sendf("RTC CREATE FAULT(%d).\r\n", retcode);
            test_rtc_delete_rtcs(rtc_num, rtc_tmp);
            return TEST_SUITE_TEST_FAILED;
        }
        test_suite_uart_sendf("RTC CREATE SUCCESS(%d).\r\n", (int)rtc_tmp[i]);
        retcode = uapi_rtc_start(rtc_tmp[i], delay[i], testsuite_rtc2_irq, i);
        if (retcode != ERRCODE_SUCC) {
            test_suite_uart_sendf("RTC %u START FAULT(%d).\r\n", i, retcode);
            test_rtc_delete_rtcs(rtc_num, rtc_tmp);
            return TEST_SUITE_TEST_FAILED;
        }
        uapi_tcxo_delay_ms(TEST_RTC_DELAY);
        test_suite_uart_sendf("delay.\r\n");
    }

    while (g_testsuit_rtc_success != rtc_num) {
    }

    if (g_testsuit_rtc_int == rtc_num) {
        test_suite_uart_sendf("RTC IRQ FINISH.\r\n");
    }
    test_rtc_delete_rtcs(rtc_num, rtc_tmp);

    return TEST_SUITE_OK;
}

int test_rtc_deinit(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    uapi_rtc_deinit();
    test_suite_uart_sendf("rtc deinit succ\r\n");

    return TEST_SUITE_OK;
}

/* Set test rtc index. */
int test_rtc_set_test_rtc(int argc, char *argv[])
{
    uint32_t test_rtc_index;

    /* Get the parameters */
    if (argc != 1) { /* 1: Indicates the number of input parameters */
        PRINT("Invalid para num [%u]. \r\n", argc);
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    test_rtc_index = (uint32_t)strtol(argv[0], NULL, 0);
    if (test_rtc_index >= g_rtc_max_num) {
        PRINT("Invalid test_rtc_index[%u], rtc_num is [%u]. \r\n", test_rtc_index, g_rtc_max_num);
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    PRINT("Set test_rtc_index[%u] -> [%u]. \r\n", g_test_rtc_index, test_rtc_index);
    g_test_rtc_index = test_rtc_index;
    return TEST_SUITE_OK;
}

/* Add the test_rtc use case. */
void add_rtc_test_case(void)
{
    /* Adapted product rtc. */
    uapi_rtc_init();
    uapi_tcxo_init();
    uapi_rtc_adapter(g_test_rtc_index, g_test_rtc_irq_list[g_test_rtc_index],
        irq_prio(g_test_rtc_irq_list[g_test_rtc_index]));

    /* rtc init for base test. */
    uapi_test_suite_add_function("test_rtc_init", "Params: none", test_rtc_init);

    /* Timer use case validation in the default rtc interrupt, can start more than one, control the number through
     TimerNums. */
    uapi_test_suite_add_function("test_rtc_start",
        "Test API: test_rtc_start. Params: <TimerNums>(1~8), <delay>(ms) (hex or decimal)", test_rtc_start);

    /* Timed tasks of different duration. */
    uapi_test_suite_add_function("test_rtc_start_diff_times",
        "Test API: test_rtc_start_diff_times. Params: <TimerNums>(1~8), <firstdelay>(ms), <secdelay>(ms) "
        "...<ndelay><ms>",
        test_rtc_start_diff_times);

    /* Wait for the timer task to time out, restart the timer task, can start more than one, control the number through
     TimerNums. */
    uapi_test_suite_add_function("test_rtc_restart",
        "Test API: test_rtc_restart. Params: <TimerNums>(1~8), <firstdelay>(ms), <secdelay>(ms)",
        test_rtc_restart);
    /* Before the previous batch of timer tasks time out, reset the timer tasks, can start more than one, control the
     number through TimerNums. */
    uapi_test_suite_add_function("test_rtc_reset_timerinfo",
        "Test API: test_rtc_reset_timerinfo. Params: <TimerNums>(1~8), <firstdelay>(ms), <resetdelay>(ms)",
        test_rtc_reset_timerinfo);

    /* Delay 1s before the previous batch of timer tasks time out, then reset the timer tasks, can start more than one,
     control the number through TimerNums. */
    uapi_test_suite_add_function("test_rtc_delay_reset_timerinfo",
        "Test API: test_rtc_reset_timerinfo. Params: <TimerNums>(1~8), <firstdelay>(ms), <resetdelay>(ms)",
        test_rtc_delay_reset_timerinfo);
    /* Start timed tasks of different lengths with 1s intervals. */
    uapi_test_suite_add_function("test_rtc_start_diff_times_intervals",
        "Test API: test_rtc_start_diff_times_intervals. Params: <TimerNums>(1~8), <firstdelay>(ms), <secdelay>(ms) "
        "...<ndelay><ms>",
        test_rtc_start_diff_times_intervals);

    /* Set test rtc index. */
    uapi_test_suite_add_function("test_rtc_set_test_rtc",
        "Test API: test_rtc_set_test_rtc. Params: <test_rtc_index>(hw_rtc)", test_rtc_set_test_rtc);

    uapi_test_suite_add_function("test_rtc_deinit", "Params: none", test_rtc_deinit);
}
