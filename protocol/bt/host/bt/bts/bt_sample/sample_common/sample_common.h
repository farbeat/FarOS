/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include common test functions of bluetooth.
 */

#ifndef SAMPLE_BT_COMMON_H
#define SAMPLE_BT_COMMON_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "securec.h"
#include "osal_addr.h"
#include "bts_def.h"
#include "diag_log.h"

/* BT_NUM */
#define BT_NUM0 0
#define BT_NUM1 1
#define BT_NUM2 2
#define BT_NUM3 3
#define BT_NUM4 4
#define BT_NUM5 5
#define BT_NUM6 6
#define BT_NUM7 7
#define BT_NUM8 8
#define BT_NUM9 9

/* 日志打印类型 */
#define BTSRV_LOG_TYPE_HSO          0x01
#define BTSRV_LOG_TYPE_UART         0x02

/* 日志打印级别定义选项：（用于业务逻辑中的日志打印） */
#define BTSRV_SAMPLE_LOG_LEVEL_ERROR       0x100
#define BTSRV_SAMPLE_LOG_LEVEL_WARNING     0x200
#define BTSRV_SAMPLE_LOG_LEVEL_INFO        0x400
#define BTSRV_SAMPLE_LOG_LEVEL_DEBUG       0x800

extern uint32_t g_bth_log_type_level;

#define bts_sample_log(fmt, ...) \
    do { \
        if ((g_bth_log_type_level & BTSRV_SAMPLE_LOG_LEVEL_INFO) != 0) { \
            if ((g_bth_log_type_level & BTSRV_LOG_TYPE_HSO) != 0) { \
                uapi_diag_info_log(0, fmt, ##__VA_ARGS__); \
            } else { \
                printf(fmt, ##__VA_ARGS__); \
            } \
        } \
    } while (0)

#define bts_sample_info_log(fmt, ...) \
    do { \
        if ((g_bth_log_type_level & BTSRV_SAMPLE_LOG_LEVEL_INFO) != 0) { \
            if ((g_bth_log_type_level & BTSRV_LOG_TYPE_HSO) != 0) { \
                uapi_diag_info_log(0, fmt, ##__VA_ARGS__); \
            } else { \
                printf("[bts][info] " fmt, ##__VA_ARGS__); \
            } \
        } \
    } while (0)

#define bts_sample_warning_log(fmt, ...) \
    do { \
        if ((g_bth_log_type_level & BTSRV_SAMPLE_LOG_LEVEL_WARNING) != 0) { \
            if ((g_bth_log_type_level & BTSRV_LOG_TYPE_HSO) != 0) { \
                uapi_diag_warning_log(0, fmt, ##__VA_ARGS__); \
            } else { \
                printf("[bts][warning] " fmt, ##__VA_ARGS__); \
            } \
        } \
    } while (0)

#define bts_sample_error_log(fmt, ...) \
    do { \
        if ((g_bth_log_type_level & BTSRV_SAMPLE_LOG_LEVEL_ERROR) != 0) { \
            if ((g_bth_log_type_level & BTSRV_LOG_TYPE_HSO) != 0) { \
                uapi_diag_error_log(0, fmt, ##__VA_ARGS__); \
            } else { \
                printf("[bts][error] " fmt, ##__VA_ARGS__); \
            } \
        } \
    } while (0)

#define bts_sample_debug_log(fmt, ...) \
    do { \
        if ((g_bth_log_type_level & BTSRV_SAMPLE_LOG_LEVEL_DEBUG) != 0) { \
            if ((g_bth_log_type_level & BTSRV_LOG_TYPE_HSO) != 0) { \
                uapi_diag_debug_log(0, fmt, ##__VA_ARGS__); \
            } else { \
                printf("[bts][debug] " fmt, ##__VA_ARGS__); \
            } \
        } \
    } while (0)

#define bts_unused(var) \
    do { \
        (void)(var); \
    } while (0)

typedef void (*btsrv_sample_cmd_handle_func)(const char **params, int32_t params_cnt);

typedef struct {
    uint32_t id;
    btsrv_sample_cmd_handle_func func;
} btsrv_sample_cmd_hdl_info;

/* 从字符串中解析设备地址，示例： "c0a012345678" -> [0xc0, 0xa0, 0x12, 0x34, 0x56, 0x78] */
uint32_t decode_bd_addr_from_str(const char *str,  uint8_t *device_addr, int32_t len);

/* 将字符串转换为 bt_uuid_t 结构表示。  */
uint32_t generate_uuid_from_string(const char *str, uint8_t len, bt_uuid_t *uuid_out);

uint32_t sample_get_sec(void);
uint64_t sample_get_us(void);

uint32_t time_sub_duration(uint32_t time, uint32_t duration);
uint64_t time_sub_duration_u64(uint64_t time, uint64_t duration);

/* 获取文件大小 */
int get_file_size(const char *file_name);

/* 比较文件内容 */
bool cmp_file_content(const char *file1_name, const char *file2_name);

uint32_t decode_uuid_from_str(const char *str,  uint8_t *uuid);
uint32_t decode_hex_from_str(const char *str,  uint8_t *nums);

void *bts_sample_mem_new(uint32_t size);
void bts_sample_mem_free(void* buf);
void bts_sample_data_to_uuid_len2(uint16_t uuid_data, bt_uuid_t *out_uuid);
uint8_t *bts_sample_generate_full_uuid(const bt_uuid_t *uuid);
bool bts_sample_bt_gatts_cmp_uuid(const bt_uuid_t *tgt_uuid, const bt_uuid_t *src_uuid);
#endif /* SAMPLE_BT_COMMON_H */
