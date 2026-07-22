/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides efuse driver source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-20, Create file. \n
 */
#include "test_efuse.h"
#include "efuse.h"

#include "securec.h"
#include "osal_addr.h"
#include "osal_debug.h"
#include "common_def.h"
#include "test_suite_errors.h"
#include "test_suite.h"
#include "tcxo.h"
#include "debug_print.h"

#define TEST_PARAM_ARGC_1 1
#define TEST_PARAM_ARGC_2 2
#define BUF_MAX_LEN         384

#ifdef EFUSE_BIT_OPERATION
static int test_efuse_read_bit(int argc, char *argv[])
{
    uint8_t value;
    uint32_t addr;
    uint8_t pos;

    if (argc >= TEST_PARAM_ARGC_2) {
        addr = (uint32_t)strtol(argv[0], NULL, 0);
        pos = (uint8_t)strtol(argv[1], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    if (uapi_efuse_read_bit(&value, addr, pos) == ERRCODE_FAIL) {
        PRINT("[efuse_read_bit] Invalid address. \r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    PRINT("[efuse_read_bit] addr:%d, pos_of_bit:%d, value: %d. \r\n", addr, pos, value);

    return TEST_SUITE_OK;
}
#endif

static int test_efuse_read_buffer(int argc, char *argv[])
{
    uint32_t addr;
    uint16_t buf_len;
    uint8_t *buffer = NULL;

    if (argc >= TEST_PARAM_ARGC_2) {
        addr = (uint32_t)strtol(argv[0], NULL, 0);
        buf_len = (uint16_t)strtol(argv[1], NULL, 0);
        if ((buf_len == 0) || (addr + buf_len > BUF_MAX_LEN)) {
            PRINT("[efuse_read_buffer] Invalid params. \r\n");
            return TEST_SUITE_ERROR_BAD_PARAMS;
        }
        buffer = (uint8_t *)malloc(buf_len * sizeof(uint8_t));
        if (buffer == NULL) {
            PRINT("[efuse_read_buffer] malloc failed. \r\n");
            return TEST_SUITE_ALLOC_FAILED;
        }
    } else {
        PRINT("[efuse_read_buffer] Invalid params. \r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    if (uapi_efuse_read_buffer(buffer, addr, buf_len) == ERRCODE_FAIL) {
        PRINT("[efuse_read_buffer] Invalid address. \r\n");
        free(buffer);
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    PRINT("[efuse_read_buffer] addr:%d, buffer[%d]:", addr, buf_len);
    for (int i = 0; i < buf_len; i++) {
        PRINT(" %d", buffer[i]);
    }
    PRINT(". \r\n");

    free(buffer);
    return TEST_SUITE_OK;
}

#ifdef EFUSE_BIT_OPERATION
static int test_efuse_write_bit(int argc, char *argv[])
{
    uint32_t addr;
    uint8_t pos;

    if (argc >= TEST_PARAM_ARGC_2) {
        addr = (uint32_t)strtol(argv[0], NULL, 0);
        pos = (uint8_t)strtol(argv[1], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    if (uapi_efuse_write_bit(addr, pos) == ERRCODE_SUCC) {
        PRINT("[efuse_write_bit] Write success. \r\n");
    } else {
        PRINT("[efuse_write_bit] Write fail. \r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}
#endif

static int test_efuse_check_buffer(uint32_t addr, uint16_t buf_len, uint8_t *buffer)
{
    uint8_t *read_buffer = NULL;

    read_buffer = (uint8_t *)malloc(buf_len * sizeof(uint8_t));
    if (read_buffer == NULL) {
        PRINT("[test_efuse_check_buffer] malloc failed. \r\n");
        return TEST_SUITE_ALLOC_FAILED;
    }
    if (uapi_efuse_read_buffer(read_buffer, addr, buf_len) == ERRCODE_FAIL) {
        PRINT("[test_efuse_check_buffer] Invalid address. \r\n");
        free(read_buffer);
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    for (int index = 0; index < buf_len; index++) {
        if ((buffer[index] & read_buffer[index]) != 0) {
            PRINT("[test_efuse_check_buffer] addr write already. \r\n");
            free(read_buffer);
            return TEST_SUITE_TEST_FAILED;
        }
    }
    free(read_buffer);
    return TEST_SUITE_OK;
}

static int test_efuse_write_buffer(int argc, char *argv[])
{
    uint32_t addr;
    uint16_t buf_len;
    uint8_t *buffer = NULL;
    int32_t ret;

    if (argc >= TEST_PARAM_ARGC_2) {
        addr = (uint32_t)strtol(argv[0], NULL, 0);
        buf_len = (uint16_t)strtol(argv[1], NULL, 0);
        if ((argc - buf_len < TEST_PARAM_ARGC_2) || (addr + buf_len > BUF_MAX_LEN)) {
            PRINT("[efuse_write_buffer] Invalid params. \r\n");
            return TEST_SUITE_ERROR_BAD_PARAMS;
        }

        buffer = (uint8_t *)malloc(buf_len * sizeof(uint8_t));
        if (buffer == NULL) {
            PRINT("[efuse_write_buffer] malloc failed. \r\n");
            return TEST_SUITE_ALLOC_FAILED;
        }
        for (int i = 0; i < buf_len; i++) {
            buffer[i] = (uint8_t)strtol(argv[i + TEST_PARAM_ARGC_2], NULL, 0);
        }
        ret = test_efuse_check_buffer(addr, buf_len, buffer);
        if (ret != TEST_SUITE_OK) {
            PRINT("[efuse_write_buffer] Write fail. \r\n");
            free(buffer);
            return TEST_SUITE_TEST_FAILED;
        }
    } else {
        PRINT("[efuse_write_buffer] Invalid params. \r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    if (uapi_efuse_write_buffer(addr, buffer, buf_len) == ERRCODE_SUCC) {
        PRINT("[efuse_write_buffer] Write success. \r\n");
    } else {
        PRINT("[efuse_write_buffer] Write fail. \r\n");
        free(buffer);
        return TEST_SUITE_TEST_FAILED;
    }

    free(buffer);
    return TEST_SUITE_OK;
}

static int test_efuse_init(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    uapi_efuse_init();
    PRINT("[efuse_init] efuse init success. \r\n");
    return TEST_SUITE_OK;
}

static int test_efuse_deinit(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    uapi_efuse_deinit();
    PRINT("[efuse_deinit] efuse deinit success. \r\n");
    return TEST_SUITE_OK;
}

static int test_efuse_get_die_id(int argc, char *argv[])
{
    uint8_t *buffer = NULL;
    if (argc != TEST_PARAM_ARGC_1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint16_t len = (uint16_t)strtol(argv[0], NULL, 0);
    buffer = (uint8_t *)malloc(len * sizeof(uint8_t));
    if (uapi_efuse_get_die_id(buffer, len) == ERRCODE_SUCC) {
        PRINT("[efuse_get_die_id] get success. \r\n");
    } else {
        PRINT("[efuse_get_die_id] get failed. \r\n");
    }
    PRINT("buffer[%d]:", len);
    for (int i = 0; i < len; i++) {
        PRINT(" %d", buffer[i]);
    }
    PRINT("\n");

    free(buffer);
    return TEST_SUITE_OK;
}

static int test_efuse_get_chip_id(int argc, char *argv[])
{
    uint8_t *buffer = NULL;
    if (argc != TEST_PARAM_ARGC_1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint16_t len = (uint16_t)strtol(argv[0], NULL, 0);
    buffer = (uint8_t *)malloc(len * sizeof(uint8_t));
    if (uapi_efuse_get_chip_id(buffer, len) == ERRCODE_SUCC) {
        PRINT("[efuse_get_chip_id] get success. \r\n");
    } else {
        PRINT("[efuse_get_chip_id] get failed. \r\n");
    }
    PRINT("buffer[%d]:", len);
    for (int i = 0; i < len; i++) {
        PRINT(" %d", buffer[i]);
    }
    PRINT("\n");

    free(buffer);
    return TEST_SUITE_OK;
}

void add_efuse_test_case(void)
{
    uapi_test_suite_add_function("test_cfbb_efuse_init", "efuse init.", test_efuse_init);
#ifdef EFUSE_BIT_OPERATION
    uapi_test_suite_add_function("test_cfbb_efuse_read_bit", "efuse read bit. <addr> <pos>", test_efuse_read_bit);
    uapi_test_suite_add_function("test_cfbb_efuse_write_bit", "efuse write bit. <addr> <pos>", test_efuse_write_bit);
#endif
    uapi_test_suite_add_function(
        "test_cfbb_efuse_read_buffer", "efuse read buffer. <addr> <buf_len>", test_efuse_read_buffer);

    uapi_test_suite_add_function(
        "test_cfbb_efuse_write_buffer", "efuse write buffer. <addr> <buf_len> <buffer>", test_efuse_write_buffer);
    uapi_test_suite_add_function("test_cfbb_efuse_deinit", "efuse deinit.", test_efuse_deinit);
    uapi_test_suite_add_function("test_cfbb_efuse_get_die_id", "param:<buf_len>", test_efuse_get_die_id);
    uapi_test_suite_add_function("test_cfbb_efuse_get_chip_id", "param:<buf_len>", test_efuse_get_chip_id);
}
