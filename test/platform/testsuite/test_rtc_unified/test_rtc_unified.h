/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Test rtc header \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-14, Create file. \n
 */
#ifndef TEST_RTC_H
#define TEST_RTC_H


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup testcase_rtc RTC
 * @ingroup  testcase
 * @{
 */

#define RTC_LOAD_COUNT_REG_OFFSET (0x00)
#define RTC_CURRENT_COUNT_REG_OFFSET (0x04)
#define RTC_CONTROL_REG_OFFSET (0x08)
#define RTC_IRQ_OFFSET        (0x0c)

#if defined(TEST_RTC) && (TEST_RTC == 3)
#define DEFAULT_TEST_RTC 3
#elif defined(TEST_RTC) && (TEST_RTC == 2)
#define DEFAULT_TEST_RTC 2
#elif defined(TEST_RTC) && (TEST_RTC == 1)
#define DEFAULT_TEST_RTC 1
#else
#define DEFAULT_TEST_RTC 0
#endif

/**
 * @if Eng
 * @brief  Add the test_rtc use case.
 * @param  None.
 * @else
 * @brief  添加test_rtc 用例.
 * @param  无.
 * @endif
 */
void add_rtc_test_case(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif