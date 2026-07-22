/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test pmp header \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-31, Create file. \n
 */
#ifndef TEST_PMP_H
#define TEST_PMP_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define PMP_TEST_RO_ADDR 0x0
#define PMP_TEST_RW_ADDR 0x10000
#define PMP_TEST_NACCESS_ADDR 0x20000

/**
 * @defgroup testcase_pmp pmp
 * @ingroup  testcase
 * @{
 */

/**
 * @if Eng
 * @brief  Add the testcase of pmp.
 * @else
 * @brief  添加pmp测试用例
 * @endif
 */
void add_pmp_test_case(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif