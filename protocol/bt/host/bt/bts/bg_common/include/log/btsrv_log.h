/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BT Service LOG Print define.
 */
#ifndef __BTSRV_LOG_H__
#define __BTSRV_LOG_H__

#include <stdio.h>
#include "debug_print.h"
#include "diag_log.h"

#define bts_unused(var) \
    do { \
        (void)(var); \
    } while (0)

/* 日志打印类型 */
#define BTSRV_LOG_TYPE_HSO          0x01
#define BTSRV_LOG_TYPE_UART         0x02
#define BTSRV_LOG_TYPE_UNSAFE          0x04

/* 日志打印级别定义选项：（用于业务逻辑中的日志打印） */
#define BTSRV_LOG_LEVEL_ERROR       0x10
#define BTSRV_LOG_LEVEL_WARNING     0x20
#define BTSRV_LOG_LEVEL_INFO        0x40
#define BTSRV_LOG_LEVEL_DEBUG       0x80

#define BTSRV_LOG_HSO_INFO          0xFF1

extern uint32_t g_bth_log_type_level;

#define bts_info(fmt, ...) \
    do { \
        if ((g_bth_log_type_level & BTSRV_LOG_LEVEL_INFO) != 0) { \
            if ((g_bth_log_type_level & BTSRV_LOG_TYPE_HSO) != 0) { \
                uapi_diag_info_log(0, fmt, ##__VA_ARGS__); \
            } else { \
                printf("[bts][info] %s " fmt, __func__, ##__VA_ARGS__); \
            } \
        } \
    } while (0)

#define bts_warning(fmt, ...) \
    do { \
        if ((g_bth_log_type_level & BTSRV_LOG_LEVEL_WARNING) != 0) { \
            if ((g_bth_log_type_level & BTSRV_LOG_TYPE_HSO) != 0) { \
                uapi_diag_warning_log(0, fmt, ##__VA_ARGS__); \
            } else { \
                printf("[bts][warning] %s " fmt, __func__, ##__VA_ARGS__); \
            } \
        } \
    } while (0)

#define bts_error(fmt, ...) \
    do { \
        if ((g_bth_log_type_level & BTSRV_LOG_LEVEL_ERROR) != 0) { \
            if ((g_bth_log_type_level & BTSRV_LOG_TYPE_HSO) != 0) { \
                uapi_diag_error_log(0, fmt, ##__VA_ARGS__); \
            } else { \
                printf("[bts][error] %s " fmt, __func__, ##__VA_ARGS__); \
            } \
        } \
    } while (0)

#define bts_debug(fmt, ...) \
    do { \
        if ((g_bth_log_type_level & BTSRV_LOG_LEVEL_DEBUG) != 0) { \
            if ((g_bth_log_type_level & BTSRV_LOG_TYPE_HSO) != 0) { \
                uapi_diag_debug_log(0, fmt, ##__VA_ARGS__); \
            } else { \
                printf("[bts][debug] %s " fmt, __func__, ##__VA_ARGS__); \
            } \
        } \
    } while (0)

#define bts_info_no_header(fmt, ...) \
    do { \
        if ((g_bth_log_type_level & BTSRV_LOG_LEVEL_INFO) != 0) { \
            if ((g_bth_log_type_level & BTSRV_LOG_TYPE_HSO) != 0) { \
                uapi_diag_info_log(0, fmt, ##__VA_ARGS__); \
            } else { \
                printf(fmt, ##__VA_ARGS__); \
            } \
        } \
    } while (0)

#define bts_dfx_print(fmt, ...) bts_info(fmt, ##__VA_ARGS__)
#define bts_dfx_print_no_header(fmt, ...) bts_info_no_header(fmt, ##__VA_ARGS__)

#endif /* __BTSRV_LOG_H__ */
