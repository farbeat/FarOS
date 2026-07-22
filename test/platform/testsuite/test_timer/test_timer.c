/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test timer source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-09, Create file. \n
 */
#include "timer.h"
#include "debug_print.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "arch_port.h"
#include "tcxo.h"
#include "securec.h"
#include "oal_interface.h"
#include "errcode.h"
#include "timer_porting.h"
#include "chip_io.h"
#include "test_timer.h"

#define TIMER_MAX_TIMERS 8
#define TIMER_TEST_DOUBULE 2
#define TIMER_MAX_NUMBER 3
#define TEST_TIMER_DELAY 1000
#define TIMER_IRQ_OFFSET 0xc

typedef struct timers {
    uint64_t start_time;
    uint64_t end_time;
} timers_t;

static volatile timers_t timer[TIMER_MAX_TIMERS];

static uint32_t g_timer_max_num = CONFIG_TIMER_MAX_NUM;
static uint32_t g_test_timer_index = DEFAULT_TEST_TIMER;
static uint32_t g_test_timer_irq_list[CONFIG_TIMER_MAX_NUM] = {
    TIMER_0_IRQN,
#if defined(CONFIG_TIMER_MAX_NUM) && (CONFIG_TIMER_MAX_NUM > 1)
    TIMER_1_IRQN,
#if defined(CONFIG_TIMER_MAX_NUM) && (CONFIG_TIMER_MAX_NUM > 2)
    TIMER_2_IRQN,
#if defined(CONFIG_TIMER_MAX_NUM) && (CONFIG_TIMER_MAX_NUM > 3)
    TIMER_3_IRQN,
#endif
#endif
#endif
};

/* Initialization Use case FLAG variable. */
#define TIMER_TEST_CALLBACK_FLAG_INIT \
    do {                              \
        g_testsuit_timer_success = 0; \
        g_testsuit_timer_int = 0;     \
    } while (0)

/* Use Case FLAG Variable Definition Declaration. */
static volatile uint32_t g_testsuit_timer_success = 0;
static volatile uint32_t g_testsuit_timer_int = 0;

/* Timed task callback function list. */
static void testsuite_timer0_irq(uintptr_t data)
{
    uint32_t index_tmp = (uint32_t)data;
    timer[index_tmp].end_time = uapi_tcxo_get_us();
    PRINT("[INFO]testsuite_timer0_irq,task=%u.\r\n", data);
    g_testsuit_timer_success++;
    g_testsuit_timer_int++;
}

static void testsuite_timer0_reset_irq(uintptr_t data)
{
    PRINT("[INFO]testsuite_timer0_reset_irq,task=%u.\r\n", data);
    g_testsuit_timer_success++;
    g_testsuit_timer_int += TIMER_TEST_DOUBULE;
}

/* Create and start multiple timed tasks. */
static int test_timer_start_timers(uint32_t timer_num, timer_handle_t *timer_handles, uint32_t delay)
{
    errcode_t retcode;
    for (uint32_t i = 0; i < timer_num; i++) {
        retcode = uapi_timer_create(g_test_timer_index, &timer_handles[i]);
        if (retcode != ERRCODE_SUCC) {
            PRINT("TIMER CREATE FAULT, hw_timer[%u], sw_timer[%u], retcode[0x%x].\r\n",
                g_test_timer_index, i, retcode);
            return TEST_SUITE_TEST_FAILED;
        }
        PRINT("TIMER CREATE SUCCESS, hw_timer[%u], sw_timer[%u], timer_handler[%d].\r\n",
            g_test_timer_index, i, (uint32_t)timer_handles[i]);
        timer[i].start_time = uapi_tcxo_get_us();
        retcode = uapi_timer_start(timer_handles[i], delay, testsuite_timer0_irq, i);
        if (retcode != ERRCODE_SUCC) {
            PRINT("TIMER START FAULT, hw_timer[%u], sw_timer[%u], retcode[0x%x].\r\n",
                g_test_timer_index, i, retcode);
            return TEST_SUITE_TEST_FAILED;
        }
    }
    return TEST_SUITE_OK;
}

/* Delete multiple timed tasks. */
static int test_timer_delete_timers(uint32_t timer_num, timer_handle_t *timer_handles)
{
    errcode_t retcode;
    for (uint32_t i = 0; i < timer_num; i++) {
        retcode = uapi_timer_stop(timer_handles[i]);
        if (retcode != ERRCODE_SUCC) {
            PRINT("TIMER STOP FAULT, sw_timer[%u], retcode[0x%x].\r\n", i, retcode);
            return TEST_SUITE_TEST_FAILED;
        }
        retcode = uapi_timer_delete(timer_handles[i]);
        if (retcode != ERRCODE_SUCC) {
            PRINT("TIMER DELETE FAULT, sw_timer[%u], retcode[0x%x].\r\n", i, retcode);
            return TEST_SUITE_TEST_FAILED;
        }
    }
    return TEST_SUITE_OK;
}

/* Before the previous batch of timer tasks time out, reset the timer tasks, can start more than one, \
   control the number through TimerNums.  */
int test_timer_reset_timerinfo(int argc, char *argv[])
{
    uint32_t timer_num, firstdelay, resetdelay;
    errcode_t retcode;
    uint32_t para_index = 0;
    timer_handle_t timer_tmp[TIMER_MAX_TIMERS] = {0};

    /* Get the parameters */
    if (argc != 3) { /* 3: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    TIMER_TEST_CALLBACK_FLAG_INIT;

    timer_num = (uint32_t)strtol(argv[para_index], NULL, 0);
    para_index++;
    firstdelay = (uint32_t)strtol(argv[para_index], NULL, 0);
    para_index++;
    /* indicates the time to reset. */
    resetdelay = (uint32_t)strtol(argv[para_index], NULL, 0);

    if (timer_num > TIMER_MAX_TIMERS) {
        PRINT("Wrong para: timer num exceed maximum(%d).\r\n", TIMER_MAX_TIMERS);
        return TEST_SUITE_CONFIG_FAILED;
    }

    uapi_timer_init();
    uapi_timer_adapter(g_test_timer_index, g_test_timer_irq_list[g_test_timer_index],
        g_aucIntPri[g_test_timer_irq_list[g_test_timer_index]]);
    retcode = test_timer_start_timers(timer_num, timer_tmp, firstdelay);
    if (retcode != TEST_SUITE_OK) {
        test_timer_delete_timers(timer_num, timer_tmp);
        return TEST_SUITE_TEST_FAILED;
    }
    for (uint32_t i = 0; i < timer_num; i++) {
        retcode = uapi_timer_start(timer_tmp[i], resetdelay, testsuite_timer0_reset_irq, i);
        if (retcode != ERRCODE_SUCC) {
            PRINT("TIMER START FAULT(%d).\r\n", retcode);
            return TEST_SUITE_TEST_FAILED;
        }
    }

    while (g_testsuit_timer_success != timer_num) {
    }
    test_timer_delete_timers(timer_num, timer_tmp);

    if (g_testsuit_timer_int == TIMER_TEST_DOUBULE * timer_num) {
        PRINT("TIMER IRQ FINISH.\r\n");
    }
    return TEST_SUITE_OK;
}

/* Delay 1s before the previous batch of timer tasks time out, then reset the timer tasks, \
can start more than one, control the number through TimerNums. */
int test_timer_delay_reset_timerinfo(int argc, char *argv[])
{
    uint32_t timer_num, firstdelay, resetdelay;
    errcode_t retcode;
    uint32_t para_index = 0;
    timer_handle_t timer_tmp[TIMER_MAX_TIMERS] = {0};

    /* Get the parameters */
    if (argc != 3) { /* 3: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    TIMER_TEST_CALLBACK_FLAG_INIT;

    timer_num = (uint32_t)strtol(argv[para_index], NULL, 0);
    para_index++;
    firstdelay = (uint32_t)strtol(argv[para_index], NULL, 0);
    para_index++;
    /* indicates the time to reset. */
    resetdelay = (uint32_t)strtol(argv[para_index], NULL, 0);
    if (timer_num > TIMER_MAX_TIMERS) {
        PRINT("Wrong para: timer num exceed maximum(%d).\r\n", TIMER_MAX_TIMERS);
        return TEST_SUITE_CONFIG_FAILED;
    }

    uapi_timer_init();
    uapi_timer_adapter(g_test_timer_index, g_test_timer_irq_list[g_test_timer_index],
        g_aucIntPri[g_test_timer_irq_list[g_test_timer_index]]);
    retcode = test_timer_start_timers(timer_num, timer_tmp, firstdelay);
    if (retcode != TEST_SUITE_OK) {
        test_timer_delete_timers(timer_num, timer_tmp);
        return TEST_SUITE_TEST_FAILED;
    }
    uapi_tcxo_delay_ms(TEST_TIMER_DELAY);
    PRINT("delay.\r\n");

    for (uint32_t i = 0, j = 0; i < timer_num; i++) {
        retcode = uapi_timer_start(timer_tmp[j++], resetdelay, testsuite_timer0_reset_irq, i);
        if (retcode != ERRCODE_SUCC) {
            PRINT("TIMER START FAULT(%d).\r\n", retcode);
            return TEST_SUITE_TEST_FAILED;
        }
    }

    while (g_testsuit_timer_success != timer_num) {
    }
    test_timer_delete_timers(timer_num, timer_tmp);

    if (g_testsuit_timer_int == TIMER_TEST_DOUBULE * timer_num) {
        PRINT("TIMER IRQ FINISH.\r\n");
    }
    return TEST_SUITE_OK;
}

/* Wait for the timer task to time out, restart the timer task, can start more than one, \
 control the number through TimerNums. */
int test_timer_restart(int argc, char *argv[])
{
    uint32_t timer_num, firstdelay, secdelay;
    errcode_t retcode;
    uint32_t para_index = 0;
    timer_handle_t timer_tmp[TIMER_MAX_TIMERS] = {0};

    /* Get the parameters */
    if (argc != 3) { /* 3: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    TIMER_TEST_CALLBACK_FLAG_INIT;

    timer_num = (uint32_t)strtol(argv[para_index], NULL, 0);
    para_index++;
    firstdelay = (uint32_t)strtol(argv[para_index], NULL, 0);
    para_index++;
    /* indicates the second time to start. */
    secdelay = (uint32_t)strtol(argv[para_index], NULL, 0);
    PRINT("timer input succ,num:%d,firstdelay:%d,secdelay:%d\r\n", timer_num, firstdelay, secdelay);
    if (timer_num > TIMER_MAX_TIMERS) {
        PRINT("Wrong para: timer num exceed maximum(%d).\r\n", TIMER_MAX_TIMERS);
        return TEST_SUITE_CONFIG_FAILED;
    }

    uapi_timer_init();
    uapi_timer_adapter(g_test_timer_index, g_test_timer_irq_list[g_test_timer_index],
        g_aucIntPri[g_test_timer_irq_list[g_test_timer_index]]);
    retcode = test_timer_start_timers(timer_num, timer_tmp, firstdelay);
    if (retcode != TEST_SUITE_OK) {
        test_timer_delete_timers(timer_num, timer_tmp);
        return TEST_SUITE_TEST_FAILED;
    }
    while (g_testsuit_timer_success != timer_num) {
    }

    if (g_testsuit_timer_int == timer_num) {
        PRINT("TIMER IRQ FIRST DONE.\r\n");
    }
    TIMER_TEST_CALLBACK_FLAG_INIT;
    for (uint32_t i = 0, j = 0; i < timer_num; i++) {
        retcode = uapi_timer_start(timer_tmp[j++], secdelay, testsuite_timer0_irq, i);
        if (retcode != ERRCODE_SUCC) {
            PRINT("TIMER %u START FAULT(%d).\r\n", i, retcode);
            return TEST_SUITE_TEST_FAILED;
        }
    }

    while (g_testsuit_timer_success != timer_num) {
    }
    test_timer_delete_timers(timer_num, timer_tmp);

    if (g_testsuit_timer_int == timer_num) {
        PRINT("TIMER IRQ FINISH.\r\n");
    }

    return TEST_SUITE_OK;
}

/* Timer use case validation in the default timer interrupt, can start more than one, \
  control the number through TimerNums. */
int test_timer_start(int argc, char *argv[])
{
    uint32_t timer_num, delay;
    errcode_t retcode;
    timer_handle_t timer_tmp[TIMER_MAX_TIMERS] = {0};

    /* Get the parameters */
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    TIMER_TEST_CALLBACK_FLAG_INIT;

    timer_num = (uint32_t)strtol(argv[0], NULL, 0);
    delay = (uint32_t)strtol(argv[1], NULL, 0);

    PRINT("timer start succ,num:%d,delay:%d\r\n", timer_num, delay);
    if (timer_num > TIMER_MAX_TIMERS) {
        PRINT("Wrong para: timer num exceed maximum(%d).\r\n", TIMER_MAX_TIMERS);
        return TEST_SUITE_CONFIG_FAILED;
    }

    uapi_timer_init();
    uapi_timer_adapter(g_test_timer_index, g_test_timer_irq_list[g_test_timer_index],
        g_aucIntPri[g_test_timer_irq_list[g_test_timer_index]]);

    retcode = test_timer_start_timers(timer_num, timer_tmp, delay);
    if (retcode != TEST_SUITE_OK) {
        test_timer_delete_timers(timer_num, timer_tmp);
        return TEST_SUITE_TEST_FAILED;
    }

    while (g_testsuit_timer_success != timer_num) {
    }
    if (g_testsuit_timer_int == timer_num) {
        PRINT("TIMER IRQ FINISH.\r\n");
    }

    test_timer_delete_timers(timer_num, timer_tmp);

    for (uint32_t i = 0; i < timer_num; i++) {
        PRINT("real time[%d] = %d, delay = %d\r\n", i, (uint32_t)(timer[i].end_time -  timer[i].start_time), delay);
        PRINT("TIMER[%d] irq response overtime!\r\n", i);
    }

    return TEST_SUITE_OK;
}

/* Timed tasks of different duration. */
int test_timer_start_diff_times(int argc, char *argv[])
{
    uint32_t timer_num;
    uint32_t delay[TIMER_MAX_TIMERS] = {0};
    errcode_t retcode;
    timer_handle_t timer_tmp[TIMER_MAX_TIMERS] = {0};

    TIMER_TEST_CALLBACK_FLAG_INIT;
    timer_num = (uint32_t)strtol(argv[0], NULL, 0);
    if (timer_num > TIMER_MAX_TIMERS) {
        PRINT("Wrong para: timer num exceed maximum(%d).\r\n", TIMER_MAX_TIMERS);
        return TEST_SUITE_CONFIG_FAILED;
    }

    if ((argc - 1) != (int)timer_num) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    for (uint32_t i = 0; i < timer_num; i++) {
        delay[i] = (uint32_t)strtol(argv[i + 1], NULL, 0);
    }
    uapi_timer_init();
    uapi_timer_adapter(g_test_timer_index, g_test_timer_irq_list[g_test_timer_index],
        g_aucIntPri[g_test_timer_irq_list[g_test_timer_index]]);

    for (uint32_t i = 0; i < timer_num; i++) {
        retcode = uapi_timer_create(g_test_timer_index, &timer_tmp[i]);
        if (retcode != ERRCODE_SUCC) {
            PRINT("TIMER CREATE FAULT(%d).\r\n", retcode);
            return TEST_SUITE_TEST_FAILED;
        }
        PRINT("TIMER CREATE SUCCESS(%d).\r\n", (int)timer_tmp[i]);
        retcode = uapi_timer_start(timer_tmp[i], delay[i], testsuite_timer0_irq, i);
        if (retcode != ERRCODE_SUCC) {
            PRINT("TIMER %u START FAULT(%d).\r\n", i, retcode);
            return TEST_SUITE_TEST_FAILED;
        }
        uapi_tcxo_delay_us(delay[i] / timer_num);
    }

    while (g_testsuit_timer_success != timer_num) {
    }

    if (g_testsuit_timer_int == timer_num) {
        PRINT("TIMER IRQ FINISH.\r\n");
    }
    test_timer_delete_timers(timer_num, timer_tmp);

    return TEST_SUITE_OK;
}

/* Start timed tasks of different lengths with 1s intervals. */
int test_timer_start_diff_times_intervals(int argc, char *argv[])
{
    uint32_t timer_num;
    uint32_t delay[TIMER_MAX_TIMERS] = {0};
    errcode_t retcode;
    timer_handle_t timer_tmp[TIMER_MAX_TIMERS] = {0};

    TIMER_TEST_CALLBACK_FLAG_INIT;
    timer_num = (uint32_t)strtol(argv[0], NULL, 0);
    if (timer_num > TIMER_MAX_TIMERS) {
        PRINT("Wrong para: timer num exceed maximum(%d).\r\n", TIMER_MAX_TIMERS);
        return TEST_SUITE_CONFIG_FAILED;
    }
    if ((argc - 1) != (int)timer_num) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    for (uint32_t i = 0; i < timer_num; i++) {
        delay[i] = (uint32_t)strtol(argv[i + 1], NULL, 0);
    }

    uapi_timer_init();
    for (uint32_t i = 0; i < timer_num; i++) {
        retcode = uapi_timer_create(g_test_timer_index, &timer_tmp[i]);
        if (retcode != ERRCODE_SUCC) {
            PRINT("TIMER CREATE FAULT(%d).\r\n", retcode);
            return TEST_SUITE_TEST_FAILED;
        }
        PRINT("TIMER CREATE SUCCESS(%d).\r\n", (uint32_t)timer_tmp[i]);
        retcode = uapi_timer_start(timer_tmp[i], delay[i], testsuite_timer0_irq, i);
        if (retcode != ERRCODE_SUCC) {
            PRINT("TIMER %u START FAULT(%d).\r\n", i, retcode);
            return TEST_SUITE_TEST_FAILED;
        }
        uapi_tcxo_delay_ms(TEST_TIMER_DELAY);
        PRINT("delay.\r\n");
    }

    while (g_testsuit_timer_success != timer_num) {
    }

    if (g_testsuit_timer_int == timer_num) {
        PRINT("TIMER IRQ FINISH.\r\n");
    }
    test_timer_delete_timers(timer_num, timer_tmp);

    return TEST_SUITE_OK;
}

/* timer init for base test. */
int test_timer_init(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    uapi_timer_init();
    PRINT("timer init succ\r\n");

    uapi_timer_adapter(g_test_timer_index, g_test_timer_irq_list[g_test_timer_index],
        g_aucIntPri[g_test_timer_irq_list[g_test_timer_index]]);
    uapi_tcxo_delay_ms(TEST_TIMER_DELAY);

    return TEST_SUITE_OK;
}

/* Set test timer index. */
int test_timer_set_test_timer(int argc, char *argv[])
{
    uint32_t test_timer_index;

    /* Get the parameters */
    if (argc != 1) { /* 1: Indicates the number of input parameters */
        PRINT("Invalid para num [%u]. \r\n", argc);
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    test_timer_index = (uint32_t)strtol(argv[0], NULL, 0);
    if (test_timer_index >= g_timer_max_num) {
        PRINT("Invalid test_timer_index[%u], timer_num is [%u]. \r\n", test_timer_index, g_timer_max_num);
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    PRINT("Set test_timer_index[%u] -> [%u]. \r\n", g_test_timer_index, test_timer_index);
    g_test_timer_index = test_timer_index;
    return TEST_SUITE_OK;
}

/* Add the test_timer use case. */
void add_timer_test_case(void)
{
    /* Adapted product timer. */
    uapi_tcxo_init();

    /* timer init for base test. */
    uapi_test_suite_add_function("timer_init", "base test. No Params", test_timer_init);

    /* Timer use case validation in the default timer interrupt, can start more than one, control the number through
     TimerNums. */
    uapi_test_suite_add_function("test_timer_start",
        "Test API: test_timer_start. Params: <TimerNums>(1~8), <delay>(us) (hex or decimal)", test_timer_start);
    /* Wait for the timer task to time out, restart the timer task, can start more than one, control the number through
     TimerNums. */
    uapi_test_suite_add_function("test_timer_restart",
        "Test API: test_timer_restart. Params: <TimerNums>(1~8), <firstdelay>(us), <secdelay>(us)",
        test_timer_restart);
    /* Before the previous batch of timer tasks time out, reset the timer tasks, can start more than one, control the
     number through TimerNums. */
    uapi_test_suite_add_function("test_timer_reset_timerinfo",
        "Test API: test_timer_reset_timerinfo. Params: <TimerNums>(1~8), <firstdelay>(us), <resetdelay>(us)",
        test_timer_reset_timerinfo);
    /* Timed tasks of different duration. */
    uapi_test_suite_add_function("test_timer_start_diff_times",
        "Test API: test_timer_start_diff_times. Params: <TimerNums>(1~8), <firstdelay>(us), <secdelay>(us) "
        "...<ndelay><us>",
        test_timer_start_diff_times);
    /* Delay 1s before the previous batch of timer tasks time out, then reset the timer tasks, can start more than one,
     control the number through TimerNums. */
    uapi_test_suite_add_function("test_timer_delay_reset_timerinfo",
        "Test API: test_timer_reset_timerinfo. Params: <TimerNums>(1~8), <firstdelay>(us), <resetdelay>(us)",
        test_timer_delay_reset_timerinfo);
    /* Start timed tasks of different lengths with 1s intervals. */
    uapi_test_suite_add_function("test_timer_start_diff_times_intervals",
        "Test API: test_timer_start_diff_times_intervals. Params: <TimerNums>(1~8), <firstdelay>(us), <secdelay>(us) "
        "...<ndelay><us>",
        test_timer_start_diff_times_intervals);
    /* Set test timer index. */
    uapi_test_suite_add_function("test_timer_set_test_timer",
        "Test API: test_timer_set_test_timer. Params: <test_timer_index>(hw_timer)", test_timer_set_test_timer);
}