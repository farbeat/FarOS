/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test watchdog source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-08, Create file. \n
 */
#include "test_watchdog.h"
#include "watchdog.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "tcxo.h"
#include "irmalloc.h"
#include "securec.h"
#include "chip_io.h"
#include "debug_print.h"

#define TEST_DELAY_100MS        100
#define TEST_PARAM_KICK_TIME    10

static int test_watchdog_init(int argc, char *argv[])
{
    uint32_t mode = 0;
    uint32_t timeout, count1, count2;
    uint32_t delay_time = 100;

    (void)uapi_tcxo_init();

    /* Get the parameters */
    if (argc < 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    timeout = (uint32_t)strtol(argv[0], NULL, 0);
    if (argc >= 2) { /* 2: Indicates the number of input parameters */
        mode = (uint32_t)strtol(argv[1], NULL, 0);
    }

    /* Allocate an external clock */
    reg_setbit(HAL_SOFT_RST_CTL_BASE, HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET, HAL_CHIP_WDT_ATOP1_RST_BIT);

    /* Parameters check */
    errcode_t ret = uapi_watchdog_init(timeout);
    if ((ret == ERRCODE_INVALID_PARAM) || (mode >= WDT_MODE_MAX)) {
        PRINT("Params is error, Timeout is %d, Max Mode is %d. \r\n", timeout, WDT_MODE_MAX - 1);
        reg_clrbit(HAL_SOFT_RST_CTL_BASE, HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET, HAL_CHIP_WDT_ATOP1_RST_BIT);
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    (void)uapi_watchdog_enable((wdt_mode_t)mode);

    uapi_tcxo_delay_ms((uint32_t)TEST_DELAY_100MS);

    while (1) {
        (void)uapi_watchdog_get_left_time(&count1);
        uapi_tcxo_delay_ms((uint32_t)TEST_DELAY_100MS);
        (void)uapi_watchdog_get_left_time(&count2);
        if (count2 < count1) {
            PRINT("Work Normall, current count is %d ms.\r\n", count2);
            continue;
        } else {
            PRINT("Work Error.\r\n");
            PRINT("delay time(os tick): 0x%x.\r\n", delay_time);
            PRINT("count befor delay: %d ms\r\n", count1);
            PRINT("count after delay: %d ms\r\n", count2);
            reg_clrbit(HAL_SOFT_RST_CTL_BASE, HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET, HAL_CHIP_WDT_ATOP1_RST_BIT);
            return TEST_SUITE_TEST_FAILED;
        }
    }

    return TEST_SUITE_OK;
}

static int test_watchdog_kick(int argc, char *argv[])
{
    uint32_t mode = 0, param_index = 0, delay_time = 100, kick_time = 0;
    uint32_t timeout, count1, count2;
    uint32_t kick = TEST_PARAM_KICK_TIME;

    (void)uapi_tcxo_init();

    /* Get the parameters */
    if (argc < 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    timeout = (uint32_t)strtol(argv[param_index++], NULL, 0);
    if (argc >= 2) { /* 2: Indicates the number of input parameters */
        mode = (uint32_t)strtol(argv[param_index++], NULL, 0);
    }
    if (argc >= 3) { /* 3: Indicates the number of input parameters */
        kick = (uint32_t)strtol(argv[param_index++], NULL, 0);
    }

    /* Allocate an external clock */
    reg_setbit(HAL_SOFT_RST_CTL_BASE, HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET, HAL_CHIP_WDT_ATOP1_RST_BIT);

    /* Parameters check */
    errcode_t ret = uapi_watchdog_init(timeout);
    if ((ret == ERRCODE_INVALID_PARAM) || (mode >= WDT_MODE_MAX)) {
        PRINT("Params is error, Timeout is %d, Max Mode is %d. \r\n", timeout, WDT_MODE_MAX - 1);
        reg_clrbit(HAL_SOFT_RST_CTL_BASE, HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET, HAL_CHIP_WDT_ATOP1_RST_BIT);
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    (void)uapi_watchdog_enable((wdt_mode_t)mode);

    while (1) {
        uapi_tcxo_delay_ms(490); /* 490: Indicates delay time, unit ms */
        (void)uapi_watchdog_get_left_time(&count1);

        (void)uapi_watchdog_kick();

        uapi_tcxo_delay_ms(10); /* 10: Indicates delay time, unit ms */
        (void)uapi_watchdog_get_left_time(&count2);

        kick_time++;
        if (kick_time > kick) {
            break;
        }

        if (count2 > count1) {
            PRINT("kick success, count1 is %d ms, count2 is %d ms.\r\n", count1, count2);
            continue;
        } else {
            PRINT("Work Error.\r\n");
            PRINT("delay time(os tick): 0x%x.\r\n", delay_time);
            PRINT("count befor kick: %d ms\r\n", count1);
            PRINT("count after kick: %d ms\r\n", count2);
            reg_clrbit(HAL_SOFT_RST_CTL_BASE, HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET, HAL_CHIP_WDT_ATOP1_RST_BIT);
            return TEST_SUITE_TEST_FAILED;
        }
    }

    (void)uapi_watchdog_deinit();
    reg_clrbit(HAL_SOFT_RST_CTL_BASE, HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET, HAL_CHIP_WDT_ATOP1_RST_BIT);

    return TEST_SUITE_OK;
}

void add_watchdog_test_case(void)
{
    uapi_test_suite_add_function(
        "watchdog_init", "Params: <TimerOut> (2, 4, 8 ... 2 ^ 16), <Mode> (0: reset, 1: interrupt), <nmi|irq>",
        test_watchdog_init);
    uapi_test_suite_add_function(
        "watchdog_kick", "Params: <TimerOut> (2, 4, 8 ... 2 ^ 16), <Mode> (0: reset, 1: interrupt),\
        <kick times:interal 500ms> (default->10)", test_watchdog_kick);
}