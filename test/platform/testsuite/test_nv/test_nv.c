/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test nv source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */
#include "test_nv.h"
#include <stdlib.h>
#include <securec.h>
#include "test_suite.h"
#include "test_suite_log.h"
#include "test_suite_errors.h"
#include "tcxo.h"
#include "nv.h"
#include "soc_osal.h"
#include "nv_reset.h"
#include "common_def.h"
#include "nv_porting.h"
#include "partition.h"

/*
 *  Configuration parameters
 */
#define MAX_TEST_VALUE_LENGTH 4060
#define MAX_TEST_RESTORE_ENABLE_PARAM_LENGTH (KEY_ID_REGION_MAX_NUM + 1)

uint8_t g_arr_kvalue_test[MAX_TEST_VALUE_LENGTH];

static void kv_set_done_callback_test(errcode_t result)
{
    test_suite_log_stringf("kv set done result = %x\r\n", result);
}

static int test_nv_init(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    errcode_t ret = uapi_partition_init();
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("partition init failed\n");
        return TEST_SUITE_TEST_FAILED;
    }
    uapi_nv_init();
    return TEST_SUITE_OK;
}

#if (defined(CONFIG_OTA_UPDATE_SUPPORT) && (CONFIG_NV_SUPPORT_OTA_UPDATE == NV_YES))
static int test_nv_upg(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    errcode_t kv_ret = kv_upg_upgrade_task_process();
    if (kv_ret == ERRCODE_SUCC)
    return TEST_SUITE_OK;
    else
    return TEST_SUITE_TEST_FAILED;
}
#endif

static int test_nv_write(int argc, char *argv[])
{
    test_suite_error_e test_ret_value = TEST_SUITE_OK;
    uint16_t key;
    uint16_t length;

    if (argc != 2) {
        test_ret_value = TEST_SUITE_ERROR_BAD_PARAMS;
    }

    /* get parameters */
    if (test_ret_value == TEST_SUITE_OK) {
        key = (uint16_t)strtol(*argv++, NULL, 0);
        length = (uint16_t)strtol(*argv++, NULL, 0);
        if (length > MAX_TEST_VALUE_LENGTH) {
            test_ret_value = TEST_SUITE_ERROR_BAD_PARAMS;
        }
    }

    /* process it */
    if (test_ret_value == TEST_SUITE_OK) {
        test_suite_log_stringf("About to store a key %d, value length %d\r\n", key, length);
        uint16_t i;
        uint8_t start_value = (uint8_t)(0xFF & (uapi_tcxo_get_us() / 1000000));  /* just take a random first value */

        uint8_t *kvalue = osal_kmalloc(sizeof(uint8_t) * length, OSAL_GFP_KERNEL);

        if (kvalue == NULL) {
            return TEST_SUITE_ALLOC_FAILED;
        }
        for (i = 0; i < length; i++) {
            kvalue[i] = (uint8_t)(start_value + i); /* make the values consecutive */
            test_suite_log_stringf("key[%d] = 0x%x\r\n", i, kvalue[i]);
        }

        errcode_t kv_ret = uapi_nv_write(key, kvalue, length);
        if (kv_ret != ERRCODE_SUCC) {
            test_suite_log_stringf("error returned 0x%x\r\n", kv_ret);
            test_ret_value = TEST_SUITE_TEST_FAILED;
        }
        osal_kfree(kvalue);
    }
    return test_ret_value;
}

/* set a key and associate it with a value <key> <length> <permanent = 1 | erasable = 0 > */
static int test_nv_write_with_attr(int argc, char *argv[])
{
    test_suite_error_e test_ret_value = TEST_SUITE_OK;
    uint16_t key;
    uint16_t length;
    nv_key_attr_t attr = {0};
    if (argc != 5) { // 5为参数个数
        test_ret_value = TEST_SUITE_ERROR_BAD_PARAMS;
    }

    /* get parameters */
    if (test_ret_value == TEST_SUITE_OK) {
        key = (uint16_t)strtol(*argv++, NULL, 0);
        length = (uint16_t)strtol(*argv++, NULL, 0);
        attr.permanent = (bool)strtol(*argv++, NULL, 0);
        attr.encrypted = (bool)strtol(*argv++, NULL, 0);
        attr.non_upgrade = (bool)strtol(*argv++, NULL, 0);
        if (length > MAX_TEST_VALUE_LENGTH) {
            test_ret_value = TEST_SUITE_ERROR_BAD_PARAMS;
        }
    }

    /* process it */
    if (test_ret_value == TEST_SUITE_OK) {
        test_suite_log_stringf("About to store a key %d, value length %d\r\n", key, length);
        uint16_t i;
        uint8_t start_value = (uint8_t)(0xFF & (uapi_tcxo_get_us() / 1000000));  // just take a random first value
        uint8_t *kvalue = osal_kmalloc(sizeof(uint8_t) * length, OSAL_GFP_KERNEL);
        if (kvalue == NULL) {
            return TEST_SUITE_ALLOC_FAILED;
        }
        for (i = 0; i < length; i++) {
            kvalue[i] = start_value + i; // make the values consecutive
            test_suite_log_stringf("key[%d] = 0x%x\r\n", i, kvalue[i]);
        }
        errcode_t kv_ret = uapi_nv_write_with_attr(key, kvalue, length, &attr, kv_set_done_callback_test);
        if (kv_ret != ERRCODE_SUCC) {
            test_suite_log_stringf("error returned 0x%x\r\n", kv_ret);
            test_ret_value = TEST_SUITE_TEST_FAILED;
        }
        osal_kfree(kvalue);
    }

    return test_ret_value;
}

/* get the value of a previously stored key <key> */
static int test_nv_read(int argc, char *argv[])
{
    test_suite_error_e test_ret_value = TEST_SUITE_OK;
    uint16_t key;

    if (argc != 1) {
        test_ret_value = TEST_SUITE_ERROR_BAD_PARAMS;
    }

    /* get parameters */
    if (test_ret_value == TEST_SUITE_OK) {
        key = (uint16_t)strtol(*argv++, NULL, 0);
    }

    /* process it */
    if (test_ret_value == TEST_SUITE_OK) {
        uint16_t klength = 0;
        test_suite_log_stringf("Requesting key 0x%x\r\n", key);
        errcode_t kv_ret = uapi_nv_read(key, MAX_TEST_VALUE_LENGTH, &klength, g_arr_kvalue_test);
        if (kv_ret == ERRCODE_SUCC && klength <= MAX_TEST_VALUE_LENGTH) {
            test_suite_log_stringf("key len:%d\r\n", klength);
            for (uint16_t i = 0; i < klength; i++) {
                test_suite_log_stringf("key[%d] = 0x%x\r\n", i, g_arr_kvalue_test[i]);
            }
        } else {
            test_suite_log_stringf("error returned 0x%x\r\n", kv_ret);
            test_ret_value = TEST_SUITE_TEST_FAILED;
        }
    }
    return test_ret_value;
}

static int test_nv_read_with_attr(int argc, char *argv[])
{
    test_suite_error_e test_ret_value = TEST_SUITE_OK;
    uint16_t key;

    if (argc != 1) {
        test_ret_value = TEST_SUITE_ERROR_BAD_PARAMS;
    }

    /* get parameters */
    if (test_ret_value == TEST_SUITE_OK) {
        key = (uint16_t) strtol(*argv++, NULL, 0);
    }

    /* process it */
    if (test_ret_value == TEST_SUITE_OK) {
        uint16_t klength = 0;
        nv_key_attr_t attr = {0};
        test_suite_log_stringf("Requesting key 0x%x\r\n", key);
        errcode_t kv_ret = uapi_nv_read_with_attr(key, MAX_TEST_VALUE_LENGTH, &klength, g_arr_kvalue_test, &attr);
        if (kv_ret == ERRCODE_SUCC && klength <= MAX_TEST_VALUE_LENGTH) {
            test_suite_log_stringf("key len:%d\r\n", klength);
            for (uint16_t i = 0; i < klength; i++) {
                test_suite_log_stringf("key[%d] = 0x%x\r\n", i, g_arr_kvalue_test[i]);
            }
            test_suite_log_stringf("permanent:   %u\r\n", attr.permanent);
            test_suite_log_stringf("encrypted:   %u\r\n", attr.encrypted);
            test_suite_log_stringf("non_upgrade: %u\r\n", attr.non_upgrade);
        } else {
            test_suite_log_stringf("error returned 0x%x\r\n", kv_ret);
            test_ret_value = TEST_SUITE_TEST_FAILED;
        }
    }
    return test_ret_value;
}

static int test_nv_get_store_status(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    nv_store_status_t status;
    errcode_t kv_ret = uapi_nv_get_store_status(&status);
    if (kv_ret == ERRCODE_SUCC) {
        test_suite_log_stringf("Total:      %d Bytes\r\n", status.total_space);
        test_suite_log_stringf("used:       %d Bytes\r\n", status.used_space);
        test_suite_log_stringf("reclaimable:%d Bytes\r\n", status.reclaimable_space);
        test_suite_log_stringf("corrupted:  %d Bytes\r\n", status.corrupted_space);
        test_suite_log_stringf("max_key:    %d Bytes\r\n", status.max_key_space);
    } else {
        test_suite_log_stringf("Read fail, ret: 0x%x\r\n", kv_ret);
    }
    return TEST_SUITE_OK;
}

static int test_nv_backup_all_keys(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    if (argc != KEY_ID_REGION_MAX_NUM) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    nv_backup_mode_t backup_flag = {0};
    for (uint32_t i = 0; i < KEY_ID_REGION_MAX_NUM; i++) {
        backup_flag.region_mode[i] = (uint8_t)strtol(*argv++, NULL, 0);
    }
    errcode_t nv_ret = uapi_nv_backup((const nv_backup_mode_t *)&backup_flag);
    if (nv_ret == ERRCODE_SUCC) {
        return TEST_SUITE_OK;
    } else {
        test_suite_log_stringf("nv error ret = 0x%x\r\n", nv_ret);
        return TEST_SUITE_TEST_FAILED;
    }
}

static int test_nv_restore_all_keys(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
#if (CONFIG_NV_SUPPORT_BACKUP_RESTORE == NV_YES)
    errcode_t nv_ret = kv_restore_all_keys();
    if (nv_ret == ERRCODE_SUCC) {
        return TEST_SUITE_OK;
    } else {
        test_suite_log_stringf("nv error ret = 0x%x\r\n", nv_ret);
        return TEST_SUITE_TEST_FAILED;
    }
#else
    return TEST_SUITE_OK;
#endif
}

static int test_nv_enable_restore_flag(int argc, char *argv[])
{
    nv_restore_mode_t nv_reset_mode = {0};
    errcode_t ret;

    if (argc < MAX_TEST_RESTORE_ENABLE_PARAM_LENGTH) {
        test_suite_log_stringf("parameters error!\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    /* get parameters */
    uint8_t mode = (uint8_t)strtol(*argv++, NULL, 0);
    if (mode == 1) {
        ret = uapi_nv_set_restore_mode_all();
    } else if (mode == 2) { /* 2: mode of partitial restore */
        for (int i = 0; i < KEY_ID_REGION_MAX_NUM; i++) {
            nv_reset_mode.region_mode[i]= (bool)strtol(*argv++, NULL, 0);
        }
        ret = uapi_nv_set_restore_mode_partitial((const nv_restore_mode_t *)&nv_reset_mode);
    } else {
        test_suite_log_stringf("parameters error!\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    if (ret == ERRCODE_SUCC) {
        return TEST_SUITE_OK;
    } else {
        test_suite_log_stringf("nv error ret = %d\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }
}

void add_kv_functions(void)
{
    uapi_test_suite_add_function("nv_init", "NV init", test_nv_init);
    uapi_test_suite_add_function("nv_get_store_status",
        "Reads the state of the flash NV size",  test_nv_get_store_status);
    uapi_test_suite_add_function("nv_write",
        "set a key and associate it with a value. <key> <length>", test_nv_write);
    uapi_test_suite_add_function("nv_write_with_attr",
        "set a key with attribute. <key> <length> <attr>", test_nv_write_with_attr);
    uapi_test_suite_add_function("nv_read",
        "get the value of a previously stored key. <key>", test_nv_read);
    uapi_test_suite_add_function("nv_read_with_attr",
        "get the value of a previously stored key. <key>", test_nv_read_with_attr);
    uapi_test_suite_add_function("nv_backup_all_keys",
        "backup all keys in working_region", test_nv_backup_all_keys);
    uapi_test_suite_add_function("nv_restore_all_keys",
        "restore all keys in backup_region", test_nv_restore_all_keys);
    uapi_test_suite_add_function("nv_enable_restore_flag",
        "enable restore flag. <mode> <flag>", test_nv_enable_restore_flag);
#if (defined(CONFIG_OTA_UPDATE_SUPPORT) && (CONFIG_NV_SUPPORT_OTA_UPDATE == NV_YES))
    uapi_test_suite_add_function("nv_upg", "NV upgrade", test_nv_upg);
#endif
}