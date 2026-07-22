/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: API of audio debug
 * Author: audio
 */

#ifndef __AUDIO_DEBUG_IPC_H__
#define __AUDIO_DEBUG_IPC_H__

#include <stdarg.h>

#include "audio_type.h"
#include "log_dsp.h"

#define AUDIO_LOG_LEVEL_ALERT   LOG_LEVEL_ERROR
#define AUDIO_LOG_LEVEL_FATAL   LOG_LEVEL_ERROR
#define AUDIO_LOG_LEVEL_ERROR   LOG_LEVEL_ERROR
#define AUDIO_LOG_LEVEL_WARN    LOG_LEVEL_WARNING
#define AUDIO_LOG_LEVEL_INFO    LOG_LEVEL_INFO
#define AUDIO_LOG_LEVEL_DBG     LOG_LEVEL_INFO
#define AUDIO_LOG_LEVEL_TRACE   LOG_LEVEL_INFO

#define SAP_CORE "X"

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

#define audio_log0 oml_dsp_log_print0
#define audio_log1 oml_dsp_log_print1
#define audio_log2 oml_dsp_log_print2
#define audio_log3 oml_dsp_log_print3
#define audio_log4 oml_dsp_log_print4

#define audio_func_enter() oml_dsp_log_print0(LOG_LEVEL_INFO, "enter...")
#define audio_func_exit()  oml_dsp_log_print0(LOG_LEVEL_INFO, "exit...")

/* function trace log, strictly prohibited to expand */
#define sap_fatal_log_ret(err_code)                                               \
    do {                                                                               \
        oml_dsp_log_print1(LOG_LEVEL_ERROR, "Error Code: [0x%08X]\n", err_code); \
    } while (0)
/* function trace log, print the called function name when function is error */
#define sap_fatal_log_fun(func, err_code)                                                            \
    do {                                                                                                 \
        oml_dsp_log_print1(LOG_LEVEL_ERROR, "Call func Failed, Error Code: [0x%08X]\n", err_code); \
    } while (0)
/* Function trace log, print the pointer name when pointer is null */
#define sap_fatal_log_null_pointer(val)                                               \
    do {                                                                              \
        oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %p, Null Pointer!\n",  val); \
    } while (0)

/* function trace log, strictly prohibited to expand */
#define sap_err_log_ret(err_code)                                                   \
    do {                                                                                 \
        oml_dsp_log_print1(LOG_LEVEL_ERROR, "Error Code: [0x%08X]\n", err_code);   \
    } while (0)
/* function trace log, print the called function name when function is error */
#define sap_err_log_fun(func, err_code)                                                                \
    do {                                                                                                   \
        oml_dsp_log_print1(LOG_LEVEL_ERROR, "Call func Failed, Error Code: [0x%08X]\n", err_code);   \
    } while (0)
/* Function trace log, print the pointer name when pointer is null */
#define sap_err_log_null_pointer(val)                                                   \
    do {                                                                                \
        oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %p, Null Pointer!\n",  val);   \
    } while (0)

/* function trace log, strictly prohibited to expand */
#define sap_warn_log_ret(err_code)                                                     \
    do {                                                                                    \
        oml_dsp_log_print1(LOG_LEVEL_WARNING, "Error Code: [0x%08X]\n", err_code);    \
    } while (0)
/* function trace log, print the called function name when function is error */
#define sap_warn_log_fun(func, err_code)                                                                  \
    do {                                                                                                      \
        oml_dsp_log_print1(LOG_LEVEL_WARNING, "Call func Failed, Error Code: [0x%08X]\n", err_code);    \
    } while (0)
/* Function trace log, print the pointer name when pointer is null */
#define sap_warn_log_null_pointer(val)                                                  \
    do {                                                                                \
        oml_dsp_log_print1(LOG_LEVEL_WARNING, "val = %p, Null Pointer!\n",  val); \
    } while (0)

/* function trace log, strictly prohibited to expand */
#define sap_trace_log_ret(err_code)                                                     \
    do {                                                                                    \
        oml_dsp_log_print1(LOG_LEVEL_INFO, "Error Code: [0x%08X]\n", err_code);    \
    } while (0)
/* function trace log, print the called function name when function is error */
#define sap_trace_log_fun(func, err_code)                                                                  \
    do {                                                                                                      \
        oml_dsp_log_print1(LOG_LEVEL_INFO, "Call func Failed, Error Code: [0x%08X]\n",   \
            (td_u32)(err_code));   \
    } while (0)
/* Function trace log, print the pointer name when pointer is null */
#define sap_trace_log_null_pointer(val)                                                  \
    do {                                                                                \
        oml_dsp_log_print1(LOG_LEVEL_INFO, "val = %p, Null Pointer!\n",  val); \
    } while (0)

/* Used for displaying more detailed alert information */
#define sap_alert_log_s32(val)   oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %d\n", val)
#define sap_alert_log_u32(val)   oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %u\n", val)
#define sap_alert_log_s64(val)
#define sap_alert_log_u64(val)
#define sap_alert_log_h32(val)   oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = 0x%08X\n", val)
#define sap_alert_log_h64(val)
#define sap_alert_log_str(val)
#define sap_alert_log_void(val)  oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %p\n", val)
#define sap_alert_log_float(val) oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %f\n", val)
#define sap_alert_log_bool(val)  oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %d\n", val)
#define sap_alert_log_info(val)  oml_dsp_log_print0(LOG_LEVEL_ERROR, "val")

/* Used for displaying more detailed fatal information */
#define sap_fatal_log_s32(val)   oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %d\n", val)
#define sap_fatal_log_u32(val)   oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %u\n", val)
#define sap_fatal_log_s64(val)
#define sap_fatal_log_u64(val)
#define sap_fatal_log_h32(val)   oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = 0x%08X\n", val)
#define sap_fatal_log_h64(val)
#define sap_fatal_log_str(val)
#define sap_fatal_log_void(val)  oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %p\n", val)
#define sap_fatal_log_float(val) oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %f\n", val)
#define sap_fatal_log_bool(val)  oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %d\n", val)
#define sap_fatal_log_info(val)  oml_dsp_log_print0(LOG_LEVEL_ERROR, "val")

/* Used for displaying more detailed error information */
#define sap_err_log_s32(val)   oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %d\n", val)
#define sap_err_log_u32(val)   oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %u\n", val)
#define sap_err_log_s64(val)
#define sap_err_log_u64(val)
#define sap_err_log_h32(val)   oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = 0x%08X\n", val)
#define sap_err_log_h64(val)
#define sap_err_log_str(val)
#define sap_err_log_void(val)  oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %p\n", val)
#define sap_err_log_float(val) oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %f\n", val)
#define sap_err_log_bool(val)  oml_dsp_log_print1(LOG_LEVEL_ERROR, "val = %d\n", val)
#define sap_err_log_info(val)  oml_dsp_log_print0(LOG_LEVEL_ERROR, "val")

/* Used for displaying more detailed warning information */
#define sap_warn_log_s32(val)   oml_dsp_log_print1(LOG_LEVEL_WARNING, "val = %d\n", val)
#define sap_warn_log_u32(val)   oml_dsp_log_print1(LOG_LEVEL_WARNING, "val = %u\n", val)
#define sap_warn_log_s64(val)
#define sap_warn_log_u64(val)
#define sap_warn_log_h32(val)   oml_dsp_log_print1(LOG_LEVEL_WARNING, "val = 0x%08X\n", val)
#define sap_warn_log_h64(val)
#define sap_warn_log_str(val)
#define sap_warn_log_void(val)  oml_dsp_log_print1(LOG_LEVEL_WARNING, "val = %p\n", val)
#define sap_warn_log_float(val) oml_dsp_log_print1(LOG_LEVEL_WARNING, "val = %f\n", val)
#define sap_warn_log_bool(val)  oml_dsp_log_print1(LOG_LEVEL_WARNING, "val = %d\n", val)
#define sap_warn_log_info(val)  oml_dsp_log_print0(LOG_LEVEL_WARNING, "val")

/* Only used for self debug, Can be expanded as needed */
#define sap_dbg_log_s32(val)   oml_dsp_log_print1(LOG_LEVEL_INFO, "val = %d\n", val)
#define sap_dbg_log_u32(val)   oml_dsp_log_print1(LOG_LEVEL_INFO, "val = %u\n", val)
#define sap_dbg_log_s64(val)
#define sap_dbg_log_u64(val)
#define sap_dbg_log_h32(val)   oml_dsp_log_print1(LOG_LEVEL_INFO, "val = 0x%08X\n", val)
#define sap_dbg_log_h64(val)
#define sap_dbg_log_str(val)
#define sap_dbg_log_void(val)  oml_dsp_log_print1(LOG_LEVEL_INFO, "val = %p\n", val)
#define sap_dbg_log_float(val) oml_dsp_log_print1(LOG_LEVEL_INFO, "val = %f\n", val)
#define sap_dbg_log_bool(val)  oml_dsp_log_print1(LOG_LEVEL_INFO, "val = %d\n", val)
#define sap_dbg_log_info(val)  oml_dsp_log_print0(LOG_LEVEL_INFO, "val")

/* Only used for trace, Can be expanded as needed */
#define sap_trace_log_s32(val)   oml_dsp_log_print1(LOG_LEVEL_INFO, "val = %d\n", val)
#define sap_trace_log_u32(val)   oml_dsp_log_print1(LOG_LEVEL_INFO, "val = %u\n", val)
#define sap_trace_log_s64(val)
#define sap_trace_log_u64(val)
#define sap_trace_log_h32(val)   oml_dsp_log_print1(LOG_LEVEL_INFO, "val = 0x%08X\n", val)
#define sap_trace_log_h64(val)
#define sap_trace_log_str(val)
#define sap_trace_log_void(val)  oml_dsp_log_print1(LOG_LEVEL_INFO, "val = %p\n", val)
#define sap_trace_log_float(val) oml_dsp_log_print1(LOG_LEVEL_INFO, "val = %f\n", val)
#define sap_trace_log_bool(val)  oml_dsp_log_print1(LOG_LEVEL_INFO, "val = %d\n", val)
#define sap_trace_log_info(val)  oml_dsp_log_print0(LOG_LEVEL_INFO, "val")
#define sap_trace_log_reg(val)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __AUDIO_DEBUG_IPC_H__ */
