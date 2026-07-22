/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Test cap sensor source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-28， Create file. \n
 */

#include "cts_common.h"
#include "cmsis_os2.h"
#include "test_suite_log.h"
#include "test_suite_errors.h"


/**
 * @brief 对接口返回值等进行判断
 * @param a - ret - uint32_t - 接口返回值
 * @param b - expected_code - uint32_t - 期望的返回值
 * @param c - is_equal - td_bool - 预期ret和expected_code是相等还是不等
 * @param d - test_result - uint32_t * - 判断结果
 * @return 无
 * @author huangbo 00428201
 */
td_void check_test_result_print(td_u32 ret, td_u32 expected_code, td_bool is_equal, uint32_t *test_result,
                                const char *function, int line)
{
    if (is_equal) {
        if (ret != expected_code) {
            test_suite_log_stringf("---test fail, function:%s, line:%d---\r\n", function, line);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
    } else {
        if (ret == expected_code) {
            test_suite_log_stringf("---test fail, function:%s, line:%d---\r\n", function, line);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
    }
}