/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: API of audio debug
 * Author: audio
 */

#ifndef __AUDIO_DEBUG_DIAG_H__
#define __AUDIO_DEBUG_DIAG_H__

#include <stdarg.h>

#include "audio_type.h"
#include "soc_diag_util.h"
#include "log_def.h"
#include "log_oam_logger.h"

#define AUDIO_LOG_LEVEL_ALERT 0
#define AUDIO_LOG_LEVEL_FATAL 1
#define AUDIO_LOG_LEVEL_ERROR 2
#define AUDIO_LOG_LEVEL_WARN  3
#define AUDIO_LOG_LEVEL_INFO  4
#define AUDIO_LOG_LEVEL_DBG   5
#define AUDIO_LOG_LEVEL_TRACE 6

#define SAP_CORE "A"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_void audio_osal_print(const td_char *fmt, ...);

td_void audio_osal_print_fun(const td_char *core, td_u8 level, const td_char *fn_name, td_u32 line_num,
    const td_char *format, ...);

#define audio_log_print(level, fmt...)                                      \
    do {                                                                    \
        audio_osal_print_fun(SAP_CORE, level, __FUNCTION__, __LINE__, fmt); \
    } while (0)

#define audio_log_alert(fmt...) audio_log_print(AUDIO_LOG_LEVEL_ALERT, fmt)
#define audio_log_fatal(fmt...) audio_log_print(AUDIO_LOG_LEVEL_FATAL, fmt)
#define audio_log_err(fmt...)   audio_log_print(AUDIO_LOG_LEVEL_ERROR, fmt)
#define audio_log_warn(fmt...)  audio_log_print(AUDIO_LOG_LEVEL_WARN, fmt)
#define audio_log_info(fmt...)  audio_log_print(AUDIO_LOG_LEVEL_INFO, fmt)
#define audio_log_dbg(fmt...)   audio_log_print(AUDIO_LOG_LEVEL_DBG, fmt)
#define audio_log_trace(fmt...) audio_log_print(AUDIO_LOG_LEVEL_TRACE, fmt)

#define audio_func_enter() uapi_diag_debug_log0(0, "enter...")
#define audio_func_exit()  uapi_diag_debug_log0(0, "exit...")

/* function trace log, strictly prohibited to expand */
#define sap_fatal_log_ret(err_code)                                               \
    do {                                                                               \
        uapi_diag_error_log1(0, "Error Code: [0x%08X]\n", err_code); \
    } while (0)
/* function trace log, print the called function name when function is error */
#define sap_fatal_log_fun(func, err_code)                                                            \
    do {                                                                                                 \
        uapi_diag_error_log1(0, "Call func Failed, Error Code: [0x%08X]\n", err_code); \
    } while (0)
/* Function trace log, print the pointer name when pointer is null */
#define sap_fatal_log_null_pointer(val)                                               \
    do {                                                                              \
        uapi_diag_error_log1(0, "val = %p, Null Pointer!\n",  val); \
    } while (0)

/* function trace log, strictly prohibited to expand */
#define sap_err_log_ret(err_code)                                                   \
    do {                                                                                 \
        uapi_diag_error_log1(0, "Error Code: [0x%08X]\n", err_code);   \
    } while (0)
/* function trace log, print the called function name when function is error */
#define sap_err_log_fun(func, err_code)                                                                \
    do {                                                                                                   \
        uapi_diag_error_log1(0, "Call func Failed, Error Code: [0x%08X]\n", err_code);   \
    } while (0)
/* Function trace log, print the pointer name when pointer is null */
#define sap_err_log_null_pointer(val)                                                   \
    do {                                                                                \
        uapi_diag_error_log1(0, "val = %p, Null Pointer!\n",  val);   \
    } while (0)

/* function trace log, strictly prohibited to expand */
#define sap_warn_log_ret(err_code)                                                     \
    do {                                                                                    \
        uapi_diag_warning_log1(0, "Error Code: [0x%08X]\n", err_code);    \
    } while (0)
/* function trace log, print the called function name when function is error */
#define sap_warn_log_fun(func, err_code)                                                                  \
    do {                                                                                                      \
        uapi_diag_warning_log1(0, "Call func Failed, Error Code: [0x%08X]\n", err_code);    \
    } while (0)
/* Function trace log, print the pointer name when pointer is null */
#define sap_warn_log_null_pointer(val)                                                  \
    do {                                                                                \
        uapi_diag_warning_log1(0, "val = %p, Null Pointer!\n",  val); \
    } while (0)

/* function trace log, strictly prohibited to expand */
#define sap_trace_log_ret(err_code)                                                     \
    do {                                                                                    \
        uapi_diag_info_log1(0, "Error Code: [0x%08X]\n", err_code);    \
    } while (0)
/* function trace log, print the called function name when function is error */
#define sap_trace_log_fun(func, err_code)                                                                  \
    do {                                                                                                      \
        uapi_diag_info_log1(0, "Call func Failed, Error Code: [0x%08X]\n", (td_u32)(err_code));    \
    } while (0)
/* Function trace log, print the pointer name when pointer is null */
#define sap_trace_log_null_pointer(val)                                                  \
    do {                                                                                \
        uapi_diag_info_log1(0, "val = %p, Null Pointer!\n",  val); \
    } while (0)

/* Used for displaying more detailed alert information */
#define sap_alert_log_s32(val)   uapi_diag_error_log1(0, "val = %d\n", val)
#define sap_alert_log_u32(val)   uapi_diag_error_log1(0, "val = %u\n", val)
#define sap_alert_log_s64(val)
#define sap_alert_log_u64(val)
#define sap_alert_log_h32(val)   uapi_diag_error_log1(0, "val = 0x%08X\n", val)
#define sap_alert_log_h64(val)
#define sap_alert_log_str(val)
#define sap_alert_log_void(val)  uapi_diag_error_log1(0, "val = %p\n", val)
#define sap_alert_log_float(val) uapi_diag_error_log1(0, "val = %f\n", val)
#define sap_alert_log_bool(val)  uapi_diag_error_log1(0, "val = %d\n", val)
#define sap_alert_log_info(val)  uapi_diag_error_log0(0, "val")

/* Used for displaying more detailed fatal information */
#define sap_fatal_log_s32(val)   uapi_diag_error_log1(0, "val = %d\n", val)
#define sap_fatal_log_u32(val)   uapi_diag_error_log1(0, "val = %u\n", val)
#define sap_fatal_log_s64(val)
#define sap_fatal_log_u64(val)
#define sap_fatal_log_h32(val)   uapi_diag_error_log1(0, "val = 0x%08X\n", val)
#define sap_fatal_log_h64(val)
#define sap_fatal_log_str(val)
#define sap_fatal_log_void(val)  uapi_diag_error_log1(0, "val = %p\n", val)
#define sap_fatal_log_float(val) uapi_diag_error_log1(0, "val = %f\n", val)
#define sap_fatal_log_bool(val)  uapi_diag_error_log1(0, "val = %d\n", val)
#define sap_fatal_log_info(val)  uapi_diag_error_log0(0, "val")

/* Used for displaying more detailed error information */
#define sap_err_log_s32(val)   uapi_diag_error_log1(0, "val = %d\n", val)
#define sap_err_log_u32(val)   uapi_diag_error_log1(0, "val = %u\n", val)
#define sap_err_log_s64(val)
#define sap_err_log_u64(val)
#define sap_err_log_h32(val)   uapi_diag_error_log1(0, "val = 0x%08X\n", val)
#define sap_err_log_h64(val)
#define sap_err_log_str(val)
#define sap_err_log_void(val)  uapi_diag_error_log1(0, "val = %p\n", val)
#define sap_err_log_float(val) uapi_diag_error_log1(0, "val = %f\n", val)
#define sap_err_log_bool(val)  uapi_diag_error_log1(0, "val = %d\n", val)
#define sap_err_log_info(val)  uapi_diag_error_log0(0, "val")

/* Used for displaying more detailed warning information */
#define sap_warn_log_s32(val)   uapi_diag_warning_log1(0, "val = %d\n", val)
#define sap_warn_log_u32(val)   uapi_diag_warning_log1(0, "val = %u\n", val)
#define sap_warn_log_s64(val)
#define sap_warn_log_u64(val)
#define sap_warn_log_h32(val)   uapi_diag_warning_log1(0, "val = 0x%08X\n", val)
#define sap_warn_log_h64(val)
#define sap_warn_log_str(val)
#define sap_warn_log_void(val)  uapi_diag_warning_log1(0, "val = %p\n", val)
#define sap_warn_log_float(val) uapi_diag_warning_log1(0, "val = %f\n", val)
#define sap_warn_log_bool(val)  uapi_diag_warning_log1(0, "val = %d\n", val)
#define sap_warn_log_info(val)  uapi_diag_warning_log0(0, "val")

/* Only used for self debug, Can be expanded as needed */
#define sap_dbg_log_s32(val)   uapi_diag_info_log1(0, "val = %d\n", val)
#define sap_dbg_log_u32(val)   uapi_diag_info_log1(0, "val = %u\n", val)
#define sap_dbg_log_s64(val)
#define sap_dbg_log_u64(val)
#define sap_dbg_log_h32(val)   uapi_diag_info_log1(0, "val = 0x%08X\n", val)
#define sap_dbg_log_h64(val)
#define sap_dbg_log_str(val)
#define sap_dbg_log_void(val)  uapi_diag_info_log1(0, "val = %p\n", val)
#define sap_dbg_log_float(val) uapi_diag_info_log1(0, "val = %f\n", val)
#define sap_dbg_log_bool(val)  uapi_diag_info_log1(0, "val = %d\n", val)
#define sap_dbg_log_info(val)  uapi_diag_info_log0(0, "val")

/* Only used for trace, Can be expanded as needed */
#define sap_trace_log_s32(val)   uapi_diag_info_log1(0, "val = %d\n", val)
#define sap_trace_log_u32(val)   uapi_diag_info_log1(0, "val = %u\n", val)
#define sap_trace_log_s64(val)
#define sap_trace_log_u64(val)
#define sap_trace_log_h32(val)   uapi_diag_info_log1(0, "val = 0x%08X\n", val)
#define sap_trace_log_h64(val)
#define sap_trace_log_str(val)
#define sap_trace_log_void(val)  uapi_diag_info_log1(0, "val = %p\n", val)
#define sap_trace_log_float(val) uapi_diag_info_log1(0, "val = %f\n", val)
#define sap_trace_log_bool(val)  uapi_diag_info_log1(0, "val = %d\n", val)
#define sap_trace_log_info(val)  uapi_diag_info_log0(0, "val")
#define sap_trace_log_reg(val)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __AUDIO_DEBUG_DIAG_H__ */
