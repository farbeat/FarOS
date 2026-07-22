/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Test cap sensor source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-28， Create file. \n
 */

#ifndef CTS_COMMON_H
#define CTS_COMMON_H
#include "td_type.h"
#include "td_base.h"
#include "test_suite.h"
#include "test_suite_log.h"
#include "test_suite_errors.h"
#include "dfx_os_st.h"


/* 测试代码使用的公共接口，实现位于cts_common.c中 */
uint32_t abs_sub(uint64_t a, uint64_t b);
void print_ret(uint32_t ret);
void print_test_result(uint32_t test_result);
void print_buf(uint8_t *buf, uint32_t size);
void print_buf_hex(uint8_t *buf, uint32_t size);
void print_buf_16bit(uint16_t *buf, uint32_t size);
void print_buf_32bit(uint32_t *buf, uint32_t size);
void print_buf_score(uint8_t *buf, uint32_t size);
void set_buf_data(uint8_t *buf, uint32_t size, uint8_t start_value, uint8_t data_step);
void set_buf_data_16bit(uint16_t *buf, uint32_t size, uint16_t start_value, uint16_t data_step);
void set_buf_data_32bit(uint32_t *buf, uint32_t size, uint32_t start_value, uint32_t data_step);
void set_buf_char(char *buf, uint32_t size, char start_value);
uint32_t cts_get_mem_used(void);

typedef enum {
    NORMAL_TEST = 0, // 正常功能测试
    ABNORMAL_TEST = 1, // 异常测试
    NULL_POINTER_TEST = 2, // 指针为空测试
    PRESSURE_TEST = 3, // 压力测试
    MULTI_TASK_TEST = 4, // 多任务测试
    COMMUNICATION_TEST = 5, // 长时间通信测试
    COMBINATION_TEST = 6, // 组合接口测试

    NORMAL_TEST_1 = 21, // 正常功能测试add
    NORMAL_TEST_2 = 22,
    NORMAL_TEST_3 = 23,
    NORMAL_TEST_4 = 24,
    NORMAL_TEST_5 = 25,
    NORMAL_TEST_6 = 26,
    NORMAL_TEST_7 = 27,
    NORMAL_TEST_8 = 28,
    NORMAL_TEST_9 = 29,
    NORMAL_TEST_10 = 30,
    NORMAL_TEST_11 = 101,
    NORMAL_TEST_12 = 102,
    NORMAL_TEST_13 = 103,

    ABNORMAL_TEST_1 = 31, // 异常测试add
    ABNORMAL_TEST_2 = 32,
    ABNORMAL_TEST_3 = 33,
    ABNORMAL_TEST_4 = 34,
    ABNORMAL_TEST_5 = 35,
    ABNORMAL_TEST_6 = 36,
    ABNORMAL_TEST_7 = 37,

    NULL_POINTER_TEST_1 = 41, // 指针为空测试add
    NULL_POINTER_TEST_2 = 42,

    PRESSURE_TEST_1 = 51, // 压力测试add
    PRESSURE_TEST_2 = 52,
	PRESSURE_TEST_3 = 53,
	PRESSURE_TEST_4 = 54,
	PRESSURE_TEST_5 = 55,

    MULTI_TASK_TEST_1 = 61, // 多任务测试add
    MULTI_TASK_TEST_2 = 62,

    COMMUNICATION_TEST_1 = 71, // 长时间通信测试add
    COMMUNICATION_TEST_2 = 72,

    COMBINATION_TEST_1 = 81, // 组合接口测试add
} cts_test_case;

typedef struct {
    uint32_t total;                /* Total space of the memory pool (unit: byte).
                                    CNcomment:内存池总大小（单位：byte）CNend */
    uint32_t used;                 /* Used space of the memory pool (unit: byte).
                                    CNcomment:内存池已经使用大小（单位：byte）CNend */
    uint32_t free;                 /* Free space of the memory pool (unit: byte).
                                    CNcomment:内存池剩余空间（单位：byte）CNend */
    uint32_t free_node_num;        /* Number of free nodes in the memory pool.
                                    CNcomment:内存池剩余空间节点个数 CNend */
    uint32_t used_node_num;        /* Number of used nodes in the memory pool.
                                    CNcomment:内存池已经使用的节点个数 CNend */
    uint32_t max_free_node_size;   /* Maximum size of the node in the free space of the memory pool (unit: byte).
                                    CNcomment:内存池剩余空间节点中最大节点的大小（单位：byte）CNend */
    uint32_t peek_size;            /* Peak memory usage of the memory pool.CNcomment:内存池使用峰值CNend */
} ext_mdm_mem_info;

/**
* @ingroup  iot_mem
* @brief  Obtains the memory information.CNcomment:获取内存信息。CNend
*
* @par 描述:
*           Obtains the memory information.CNcomment:获取内存信息。CNend
*
* @attention None
* @param  mem_inf [OUT] type #ext_mdm_mem_info*，Overall memory information.CNcomment:整体内存信息。CNend
*
* @retval #EXT_ERR_SUCCESS Success.
* @retval #Other Failure, for details, see soc_errno.h
* @par 依赖:
*            @li soc_os_stat.h：Describes memory APIs.CNcomment:文件用于描述内存相关接口。CNend
* @see  None
*/
ext_errno uapi_os_get_mem_sys_info(TD_OUT ext_mdm_mem_info *mem_inf);

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
                                const char *function, int line);

#endif
