/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 * Description: Test trng source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-13, Create file. \n
 */
#include "debug_print.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "test_trng.h"
#include "securec.h"
#include "trng.h"

#define MAX_TEST_TRNG_SIZE 1000
uint8_t g_trng_data[MAX_TEST_TRNG_SIZE] = {0};

#define DUMP_BYTES_ONCE_LEN  16
#define DUMP_BYTES_TIMES     3

static char get_high_hex(uint8_t buffer_byte)
{
    char r = 0;
    buffer_byte >>= 4; // 右移4bit,取高4bit

    if (buffer_byte <= 9) { // 小于等于9,'0'~'9'表示
        r = buffer_byte + '0';
    } else {
        r = buffer_byte - 10 + 'A'; // 大于等于10,'A'~'F'表示
    }
    return r;
}

static char get_low_hex(uint8_t buffer_byte)
{
    char r = 0;
    buffer_byte &= 0x0f;

    if (buffer_byte <= 9) { // 小于等于9,'0'~'9'表示
        r = buffer_byte + '0';
    } else {
        r = buffer_byte - 10 + 'A'; // 大于等于10,'A'~'F'表示
    }
    return r;
}

static void dump_buf(const char* buffer_name, const uint8_t* buffer, const uint32_t len)
{
    uint32_t i;
    uint8_t dump_byte[DUMP_BYTES_ONCE_LEN * DUMP_BYTES_TIMES + 1] = {0};
    uint8_t *ptr = dump_byte;

    if (buffer_name == NULL || buffer == NULL) {
        PRINT("the buffer is null\r\n");
        return;
    }

    PRINT("name:%s, addr:0x%x, len:0x%x\r\n", buffer_name, buffer, len);

    for (i = 0; i < len; i++) {
        *(ptr++) = get_high_hex(buffer[i]);
        *(ptr++) = get_low_hex(buffer[i]);
        *(ptr++) = ' ';
        if ((ptr - dump_byte) == (sizeof(dump_byte) - 1)) {
            *ptr = '\0';
            ptr = dump_byte;
            PRINT("%s\r\n", dump_byte);
        }
    }

    if (ptr != dump_byte) {
        *ptr = '\0';
        PRINT("%s\r\n", dump_byte);
    }
}

static int test_trng_get_random(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uint32_t first_data, second_data;
    errcode_t ret = uapi_drv_cipher_trng_get_random(&first_data);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    ret = uapi_drv_cipher_trng_get_random(&second_data);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    PRINT("first_trng_data = 0x%x\n", first_data);
    PRINT("second_trng_data = 0x%x\n", second_data);
    if (first_data == second_data) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

static int test_trng_get_random_bytes(int argc, char *argv[])
{
    UNUSED(argc);
    uint32_t trng_size = (uint32_t)strtol(argv[0], NULL, 0);
    errcode_t ret = uapi_drv_cipher_trng_get_random_bytes(g_trng_data, trng_size);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    dump_buf("trng_result", g_trng_data, trng_size);

    return TEST_SUITE_OK;
}

void add_trng_test_case(void)
{
    uapi_test_suite_add_function("test_trng_get_random", "function trng get random test.", test_trng_get_random);
    uapi_test_suite_add_function(
        "test_trng_get_random_bytes", "function trng get random bytes test.Params: <len>", test_trng_get_random_bytes);
}