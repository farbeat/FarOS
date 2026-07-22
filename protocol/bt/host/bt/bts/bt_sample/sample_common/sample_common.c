/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include common test functions of bluetooth.
 */

#include "sample_common.h"

#include <stdlib.h>
#include <string.h>
#include "td_base.h"
#include "soc_osal.h"
#include "bts_def.h"

#define TEN 10

static uint8_t char_2_num(char ch)
{
    uint8_t c = (uint8_t)ch;
    if (c >= '0' && c <= '9') {
        return (uint8_t)(c - '0');
    } else if (c >= 'A' && c <= 'F') {
        return (uint8_t)(c - 'A' + TEN);
    } else if (c >= 'a' && c <= 'f') {
        return (uint8_t)(c - 'a' + TEN);
    } else {
        return UINT8_MAX;
    }
}

#define BD_ADDR_STR_LEN 12
#define GATT_UUID_OFFSET 12

/* 从字符串中解析设备地址，示例： "c0a012345678" -> [0x78, 0x56, 0x34, 0x12, 0xa0, 0xc0] */
uint32_t decode_bd_addr_from_str(const char *str,  uint8_t *device_addr, int32_t len)
{
    int32_t str_len = (int32_t)strnlen(str, BD_ADDR_STR_LEN + 1);
    if (str_len != BD_ADDR_STR_LEN || len != BD_ADDR_LEN) {
        bts_sample_log("len err! str_len = %d  len = %d\n", str_len, len);
        return EXT_ERR_FAILURE;
    }

    bts_sample_log("str: %s  bd_addr:", str);
    int32_t cnt = 0;
    for (int32_t i = BD_ADDR_STR_LEN - 1; i >= 0; i -= 2) { /* 2: 每个字节包含2个字符 */
        uint8_t num1 = char_2_num(str[i - 1]);
        uint8_t num2 = char_2_num(str[i]);
        if (num1 == UINT8_MAX || num2 == UINT8_MAX) {
            bts_sample_log("invalid input %c%c\n", str[i - 1], str[i]);
            return EXT_ERR_FAILURE;
        }
        device_addr[cnt] = ((num1 << TD_U4_BITS) | num2);
        bts_sample_log("%02x", device_addr[cnt]);
        cnt++;
    }
    bts_sample_log("\n");

    return EXT_ERR_SUCCESS;
}

/* 将字符串转换为 bt_uuid_t 结构表示 */
uint32_t generate_uuid_from_string(const char *str, uint8_t len, bt_uuid_t *uuid_out)
{
    bts_sample_log("str: %s len: %u out: ", str, len);
    int slen = (int)strnlen(str, len);
    bool new_uuid = false;
    if (uuid_out == NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    uuid_out->uuid_len = (uint8_t)slen / 2; /* 2: 2个字符组成一个字节，因此只取偶数个字符 */
    for (int i = 0; i + 1 < slen; i += 2) { /* 2: 同上 */
        uint8_t num1 = char_2_num(str[i]);
        uint8_t num2 = char_2_num(str[i + 1]);
        if (num1 == UINT8_MAX || num2 == UINT8_MAX) {
            bts_sample_log("invalid input %c%c\n", str[i], str[i + 1]);
            return EXT_ERR_INVALID_PARAMETER;
        }

        uuid_out->uuid[i / 2] = (uint8_t)((num1 << TD_U4_BITS) | num2); /* 2: 同上 */
        bts_sample_log("%02x ", (uint8_t)(uuid_out->uuid[i / 2])); /* 2: 同上 */
    }

    bts_sample_log("\n");

    return EXT_ERR_SUCCESS;
}

uint32_t sample_get_sec(void)
{
    osal_timeval cur_time = { 0, 0 };
    osal_gettimeofday(&cur_time);

    return (uint32_t)(cur_time.tv_sec);
}

uint64_t sample_get_us(void)
{
    osal_timeval cur_time = { 0, 0 };
    osal_gettimeofday(&cur_time);
    return (uint64_t)cur_time.tv_sec * 1000000 + (uint64_t)cur_time.tv_usec; /* 1000000: us per sec */
}

uint32_t time_sub_duration(uint32_t time, uint32_t duration)
{
    if (time >= duration) {
        return time - duration;
    } else {
        return UINT8_MAX - (duration - time) + 1;
    }
}

/* u64减法，带翻转处理 */
uint64_t time_sub_duration_u64(uint64_t time, uint64_t duration)
{
    if (time >= duration) {
        return time - duration;
    } else {
        return TD_U64_MAX - (duration - time) + 1;
    }
}

/* 获取文件大小 */
int get_file_size(const char *file_name)
{
    bts_unused(file_name);
    return 0;
}

/* 比较文件内容 */
bool cmp_file_content(const char *file1_name, const char *file2_name)
{
    bts_unused(file1_name);
    bts_unused(file2_name);
    return false;
}

/* 从字符串中解析uuid，示例： "180D" -> [0x0d, 0x18] */
uint32_t decode_uuid_from_str(const char *str,  uint8_t *uuid)
{
    size_t str_len = strlen(str);
    if (str_len > BT_UUID_MAX_LEN * 2) { /* 2: 每个字节包含2个字符 */
        bts_sample_log("uuid len exceeds max limit! str_len = %u \n", str_len);
        return EXT_ERR_FAILURE;
    }
    bts_sample_log("str_len: %u str: %s  uuid:", str_len, str);
    int32_t cnt = 0;
    for (int32_t i = (int32_t)str_len - 1; i >= 0; i -= 2) { /* 2: 每个字节包含2个字符 */
        uuid[cnt] = (char_2_num(str[i - 1]) << TD_U4_BITS) + char_2_num(str[i]);
        bts_sample_log("%02x", (uint8_t)uuid[cnt]);
        cnt++;
    }
    bts_sample_log("\n");
    return EXT_ERR_SUCCESS;
}

/* 从字符串中解析十六进制数字，例： "7d05" -> [0x7d, 0x05] */
uint32_t decode_hex_from_str(const char *str,  uint8_t *nums)
{
    uint32_t len = strlen(str);
    bts_sample_log("str_len: %u str: %s  num:", len, str);
    uint32_t cnt = 0;
    for (uint32_t i = 0; i < len - 1; i += 2) { /* 2: 每个字节包含2个字符 */
        nums[cnt] = (char_2_num(str[i]) << TD_U4_BITS) + char_2_num(str[i + 1]);
        bts_sample_log("%02x", nums[cnt]);
        cnt++;
    }
    bts_sample_log("\n");
    return EXT_ERR_SUCCESS;
}

void bts_sample_data_to_uuid_len2(uint16_t uuid_data, bt_uuid_t *out_uuid)
{
    char uuid[] = {(uint8_t)(uuid_data >> TD_U8_BITS), (uint8_t)uuid_data};
    out_uuid->uuid_len = BT_NUM2;
    if (memcpy_s(out_uuid->uuid, out_uuid->uuid_len, uuid, BT_NUM2) != EOK) {
        bts_sample_log("[btsrv][ERROR] uuid data to len2 memcpy fail.\n");
        return;
    }
}

void *bts_sample_mem_new(uint32_t size)
{
    void *ptr = osal_kmalloc(size, 0);
    if (ptr == NULL) {
        return NULL;
    }
    (void)memset_s(ptr, size, 0, size);
    return ptr;
}

void bts_sample_mem_free(void* buf)
{
    osal_kfree(buf);
}

/* 根据bt uuid新初始化一个完整128bit的uuid, 用于GATT */
uint8_t *bts_sample_generate_full_uuid(const bt_uuid_t *uuid)
{
    uint8_t *full_uuid = (uint8_t *)bts_sample_mem_new(BT_UUID_MAX_LEN);
    if (full_uuid == NULL) {
        bts_sample_log("[btsrv][ERROR] failed to malloc for full uuid.\n");
        return NULL;
    }
    uint8_t base_full[BT_UUID_MAX_LEN] = {0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (memcpy_s(full_uuid, BT_UUID_MAX_LEN, base_full, BT_UUID_MAX_LEN) != EOK) {
        bts_sample_log("[btsrv][ERROR] base uuid memcpy fail.\n");
        bts_sample_mem_free(full_uuid);
        return NULL;
    }
    if (uuid->uuid_len == BT_NUM2) {
        /* 16bit类型的GATT UUID，占用第12、13个byte */
        if (memcpy_s(full_uuid + GATT_UUID_OFFSET, uuid->uuid_len, uuid->uuid, uuid->uuid_len) != EOK) {
            bts_sample_log("[btsrv][ERROR] short uuid memcpy fail.\n");
            bts_sample_mem_free(full_uuid);
            return NULL;
        }
    } else if (uuid->uuid_len == BT_UUID_MAX_LEN) {
        if (memcpy_s(full_uuid, uuid->uuid_len, uuid->uuid, uuid->uuid_len) != EOK) {
            bts_sample_log("[btsrv][ERROR] full uuid memcpy fail.\n");
            bts_sample_mem_free(full_uuid);
            return NULL;
        }
    } else {
        bts_sample_log("[btsrv][ERROR] GATT only supports 16-bit or 128-bit UUIDs.\n");
        bts_sample_mem_free(full_uuid);
        return NULL;
    }
    return full_uuid;
}

bool bts_sample_bt_gatts_cmp_uuid(const bt_uuid_t *tgt_uuid, const bt_uuid_t *src_uuid)
{
    uint8_t *tgt = bts_sample_generate_full_uuid(tgt_uuid);
    bool result = false;
    if (tgt == NULL) {
        return false;
    }
    uint8_t *src = bts_sample_generate_full_uuid(src_uuid);
    if (src == NULL) {
        bts_sample_mem_free(tgt);
        return false;
    }
    if (memcmp(tgt, src, BT_UUID_MAX_LEN) == 0) {
        result = true;
    }
    bts_sample_mem_free(src);
    bts_sample_mem_free(tgt);
    return result;
}