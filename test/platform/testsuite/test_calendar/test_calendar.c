/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test calendar source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "debug_print.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "securec.h"
#include "calendar.h"
#include "hal_calendar.h"
#include "calendar_porting.h"
#include "test_calendar.h"

#define TEST_CALENDAR_SET_PARAM_NUM 6

static int test_calendar_set_date(int argc, char *argv[])
{
    if (argc < TEST_CALENDAR_SET_PARAM_NUM) {
        printf("Para Error\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t param_index = 0;
    calendar_t date;
    date.year = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    date.mon = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    date.day = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    date.hour = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    date.min = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    date.sec = (uint32_t)strtol(argv[param_index], NULL, 0);
    param_index++;
    errcode_t ret = uapi_calendar_set_datetime(&date);
    if (ret == ERRCODE_SUCC) {
        printf("Set datetime success: %d-%d-%d %d:%d:%d\r\n", date.year, date.mon, date.day, date.hour, date.min,
            date.sec);
        return TEST_SUITE_OK;
    }
    printf("Set datetime failed, error code = %d \r\n", ret);
    return TEST_SUITE_TEST_FAILED;
}

static int test_calendar_get_date(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    calendar_t date;
    errcode_t ret = uapi_calendar_get_datetime(&date);
    if (ret == ERRCODE_SUCC) {
        printf("Get datetime success: %d-%d-%d %d:%d:%d\r\n", date.year, date.mon, date.day, date.hour, date.min,
            date.sec);
        return TEST_SUITE_OK;
    }
    printf("Get datetime failed, error code = %d \r\n", ret);
    return TEST_SUITE_TEST_FAILED;
}

static int test_calendar_set_timestamp(int argc, char *argv[])
{
    if (argv == NULL) {
        return 0;
    }
    if (argc < 1) {
        printf("Para Error\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint64_t timestamp = strtoull(argv[0], NULL, 0);
    errcode_t ret = uapi_calendar_set_timestamp(timestamp);
    if (ret == ERRCODE_SUCC) {
        printf("Set timestamp success: %llu\r\n", timestamp);
        return TEST_SUITE_OK;
    }
    printf("Set timestamp failed, error code = %d \r\n", ret);
    return TEST_SUITE_TEST_FAILED;
}

static int test_calendar_get_timestamp(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    uint64_t timestamp;
    errcode_t ret = uapi_calendar_get_timestamp(&timestamp);
    if (ret == ERRCODE_SUCC) {
        printf("Get timestamp success: %llu\r\n", timestamp);
        return TEST_SUITE_OK;
    }
    printf("Get timestamp failed, error code = %d \r\n", (uint32_t)ret);
    return TEST_SUITE_TEST_FAILED;
}

void add_calendar_test_case(void)
{
    uapi_calendar_init();
    uapi_test_suite_add_function("calendar_set_date_test",
        "Test API: set date. Params: <year>, <mon>, <day>, <hour>, <min>, <sec>", test_calendar_set_date);
    uapi_test_suite_add_function("calendar_get_date_test",
        "Test API: get date. Params: <year>, <mon>, <day>, <hour>, <min>, <sec>", test_calendar_get_date);
    uapi_test_suite_add_function("calendar_set_timestamp_test", "Test API: set timestamp. Params: <timestamp>",
        test_calendar_set_timestamp);
    uapi_test_suite_add_function("calendar_get_timestamp_test", "Test API: get timestamp. Params: <timestamp>",
        test_calendar_get_timestamp);
}
