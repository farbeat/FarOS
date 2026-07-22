/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test partion source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */
#include "test_partition.h"
#include <stdlib.h>
#include "test_suite.h"
#include "test_suite_log.h"
#include "test_suite_errors.h"
#include "common_def.h"
#include "partition_info.h"

#define TEST_FAILED_RETURN \
    test_suite_log_stringf("TEST_SUITE_TEST_FAILED\r\n"); \
    return TEST_SUITE_TEST_FAILED

static int test_partition_init(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    /* 初始化测试： 需要手动修改宏 “ PARTITION_MAX_CNT ”  或修改配置文件；
     * 初始化失败： 配置参数区个数 != PARTITION_MAX_CNT
     * 初始化成功： 配置参数区个数 = PARTITION_MAX_CNT
     */
    errcode_t ret = uapi_partition_init();
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("partition init failed\n");
        return ERRCODE_FAIL;
    }

    partition_info_t *tbl = partition_get_global_info()->partition_tbl;
    test_suite_log_stringf("======================== partition print =======================\n");
    for (uint8_t i = 0; i < PARTITION_MAX_CNT; i++) {
        test_suite_log_stringf("[%d] - 0x%x, 0x%x, 0x%x\n", i, tbl[i].addr, tbl[i].size, tbl[i].id);
    }
    test_suite_log_stringf("====================== partition print end =====================\n");
    return ERRCODE_SUCC;
}

static int test_get_info_with_param(int argc, char *argv[])
{
    if (argc != 3) { /* 3代表参数的个数 */
        test_suite_log_stringf("param count must be 3!, argc:%d\r\n", argc);
        TEST_FAILED_RETURN;
    }

    uint8_t partition_id = (uint8_t)strtol(argv[0], NULL, 0);
    uint32_t param = (uint32_t)strtol(argv[1], NULL, 0);
    uint32_t exp_res = (uint32_t)strtol(argv[2], NULL, 0);
    partition_information_t info;
    partition_information_t *pinfo = NULL;
    if (param != 0) {
        pinfo = &info;
    }

    errcode_t ret = uapi_partition_get_info(partition_id, pinfo);
    if ((exp_res == 0 && (ret != ERRCODE_SUCC)) || (exp_res != 0 && ret == ERRCODE_SUCC)) {
        test_suite_log_stringf("partition info get failed\r\n");
        TEST_FAILED_RETURN;
    }

    if (exp_res != 0) {
        test_suite_log_stringf("partition info get test success, abnormal test ok\r\n");
        return ERRCODE_SUCC;
    }

    test_suite_log_stringf("partition info get test success:partition id[0x%x] addr: 0x%x, size: 0x%x\r\n",
        partition_id, info.part_info.addr_info.addr, info.part_info.addr_info.size);
    return ERRCODE_SUCC;
}

void add_test_partition(void)
{
    uapi_test_suite_add_function("part_init", "partition infomation init test", test_partition_init);
    uapi_test_suite_add_function("part_get_info_with_para",
        "part_get_info_with_para <partition id> <param value: 0-NULL, 1-normal value> <exp res: 0-success, 1-fail>",
        test_get_info_with_param);
}
