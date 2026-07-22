/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: API of audio debug
 * Author: audio
 */

#ifndef __AUDIO_DEBUG_UART_H__
#define __AUDIO_DEBUG_UART_H__

#include <stdarg.h>

#include "audio_type.h"

#define AUDIO_LOG_LEVEL_ALERT 0
#define AUDIO_LOG_LEVEL_FATAL 1
#define AUDIO_LOG_LEVEL_ERROR 2
#define AUDIO_LOG_LEVEL_WARN  3
#define AUDIO_LOG_LEVEL_INFO  4
#define AUDIO_LOG_LEVEL_DBG   5
#define AUDIO_LOG_LEVEL_TRACE 6

#if (SAP_OS_TYPE == os_type_xtensa)
#define SAP_CORE "X"
/* Switch for log print in xtensa */
#ifndef AUDIO_LOG_SWITCH_X
#define AUDIO_LOG_SWITCH_X 1 /* 0:disable, 1:enable */
/* Default log level in xtensa */
#ifndef AUDIO_LOG_LEVEL
#define AUDIO_LOG_LEVEL AUDIO_LOG_LEVEL_ERROR
#endif /* AUDIO_LOG_LEVEL */
#endif /* AUDIO_LOG_SWITCH_X */

#else /* SAP_OS_TYPE */
#define SAP_CORE "A"
/* Switch for log print in arm */
#ifndef AUDIO_LOG_SWITCH_A
#ifdef BRANDY_PROT /* bt core id */
#define AUDIO_LOG_SWITCH_A 0 /* 0:disable, 1:enable */
#else
#define AUDIO_LOG_SWITCH_A 1 /* 0:disable, 1:enable */
#endif /* BRANDY_PROT */
/* Default log level in arm */
#ifndef AUDIO_LOG_LEVEL
#ifdef BRANDY_PROT /* bt core id */
#define AUDIO_LOG_LEVEL AUDIO_LOG_LEVEL_ALERT
#else
#define AUDIO_LOG_LEVEL AUDIO_LOG_LEVEL_ERROR
#endif /* BRANDY_PROT */
#endif /* AUDIO_LOG_LEVEL */
#endif /* AUDIO_LOG_SWITCH_A */

#endif /* SAP_OS_TYPE */

#ifndef AUDIO_LOG_LEVEL
#define AUDIO_LOG_LEVEL AUDIO_LOG_LEVEL_TRACE
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define NONE "\033[m"
#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"

td_void audio_osal_print(const td_char *fmt, ...);

td_void audio_osal_print_fun(const td_char *core, td_u8 level, const td_char *fn_name, td_u32 line_num,
    const td_char *format, ...);

#define audio_log_print(level, fmt...)                                      \
    do {                                                                    \
        audio_osal_print_fun(SAP_CORE, level, __FUNCTION__, __LINE__, fmt); \
    } while (0)

#if (AUDIO_LOG_LEVEL == AUDIO_LOG_LEVEL_ALERT)
#define audio_log_alert(fmt...) audio_log_print(AUDIO_LOG_LEVEL_ALERT, fmt)
#define audio_log_fatal(fmt...)
#define audio_log_err(fmt...)
#define audio_log_warn(fmt...)
#define audio_log_info(fmt...)
#define audio_log_dbg(fmt...)
#define audio_log_trace(fmt...)
#elif (AUDIO_LOG_LEVEL == AUDIO_LOG_LEVEL_FATAL)
#define audio_log_alert(fmt...) audio_log_print(AUDIO_LOG_LEVEL_ALERT, fmt)
#define audio_log_fatal(fmt...) audio_log_print(AUDIO_LOG_LEVEL_FATAL, fmt)
#define audio_log_err(fmt...)
#define audio_log_warn(fmt...)
#define audio_log_info(fmt...)
#define audio_log_dbg(fmt...)
#define audio_log_trace(fmt...)
#elif (AUDIO_LOG_LEVEL == AUDIO_LOG_LEVEL_ERROR)
#define audio_log_alert(fmt...) audio_log_print(AUDIO_LOG_LEVEL_ALERT, fmt)
#define audio_log_fatal(fmt...) audio_log_print(AUDIO_LOG_LEVEL_FATAL, fmt)
#define audio_log_err(fmt...)   audio_log_print(AUDIO_LOG_LEVEL_ERROR, fmt)
#define audio_log_warn(fmt...)
#define audio_log_info(fmt...)
#define audio_log_dbg(fmt...)
#define audio_log_trace(fmt...)
#elif (AUDIO_LOG_LEVEL == AUDIO_LOG_LEVEL_WARN)
#define audio_log_alert(fmt...) audio_log_print(AUDIO_LOG_LEVEL_ALERT, fmt)
#define audio_log_fatal(fmt...) audio_log_print(AUDIO_LOG_LEVEL_FATAL, fmt)
#define audio_log_err(fmt...)   audio_log_print(AUDIO_LOG_LEVEL_ERROR, fmt)
#define audio_log_warn(fmt...)  audio_log_print(AUDIO_LOG_LEVEL_WARN, fmt)
#define audio_log_info(fmt...)
#define audio_log_dbg(fmt...)
#define audio_log_trace(fmt...)
#elif (AUDIO_LOG_LEVEL == AUDIO_LOG_LEVEL_INFO)
#define audio_log_alert(fmt...) audio_log_print(AUDIO_LOG_LEVEL_ALERT, fmt)
#define audio_log_fatal(fmt...) audio_log_print(AUDIO_LOG_LEVEL_FATAL, fmt)
#define audio_log_err(fmt...)   audio_log_print(AUDIO_LOG_LEVEL_ERROR, fmt)
#define audio_log_warn(fmt...)  audio_log_print(AUDIO_LOG_LEVEL_WARN, fmt)
#define audio_log_info(fmt...)  audio_log_print(AUDIO_LOG_LEVEL_INFO, fmt)
#define audio_log_dbg(fmt...)
#define audio_log_trace(fmt...)
#elif (AUDIO_LOG_LEVEL == AUDIO_LOG_LEVEL_DBG)
#define audio_log_alert(fmt...) audio_log_print(AUDIO_LOG_LEVEL_ALERT, fmt)
#define audio_log_fatal(fmt...) audio_log_print(AUDIO_LOG_LEVEL_FATAL, fmt)
#define audio_log_err(fmt...)   audio_log_print(AUDIO_LOG_LEVEL_ERROR, fmt)
#define audio_log_warn(fmt...)  audio_log_print(AUDIO_LOG_LEVEL_WARN, fmt)
#define audio_log_info(fmt...)  audio_log_print(AUDIO_LOG_LEVEL_INFO, fmt)
#define audio_log_dbg(fmt...)   audio_log_print(AUDIO_LOG_LEVEL_DBG, fmt)
#define audio_log_trace(fmt...)
#elif (AUDIO_LOG_LEVEL == AUDIO_LOG_LEVEL_TRACE)
#define audio_log_alert(fmt...) audio_log_print(AUDIO_LOG_LEVEL_ALERT, fmt)
#define audio_log_fatal(fmt...) audio_log_print(AUDIO_LOG_LEVEL_FATAL, fmt)
#define audio_log_err(fmt...)   audio_log_print(AUDIO_LOG_LEVEL_ERROR, fmt)
#define audio_log_warn(fmt...)  audio_log_print(AUDIO_LOG_LEVEL_WARN, fmt)
#define audio_log_info(fmt...)  audio_log_print(AUDIO_LOG_LEVEL_INFO, fmt)
#define audio_log_dbg(fmt...)   audio_log_print(AUDIO_LOG_LEVEL_DBG, fmt)
#define audio_log_trace(fmt...) audio_log_print(AUDIO_LOG_LEVEL_TRACE, fmt)
#else
#error "audio_debug.h: unsupport AUDIO_LOG_LEVEL"
#endif

#define audio_log0 audio_log_print
#define audio_log1 audio_log_print
#define audio_log2 audio_log_print
#define audio_log3 audio_log_print
#define audio_log4 audio_log_print

#define audio_func_enter() audio_log_trace("enter...")
#define audio_func_exit()  audio_log_trace("exit...")

/* function trace log, strictly prohibited to expand */
#define sap_fatal_log_ret(err_code) audio_log_fatal("Error Code: [0x%08X]\n", err_code)
/* function trace log, print the called function name when function is error */
#define sap_fatal_log_fun(func, err_code) audio_log_fatal("Call %s Failed, Error Code: [0x%08X]\n", #func, err_code)
/* Function trace log, print the pointer name when pointer is null */
#define sap_fatal_log_null_pointer(val) audio_log_fatal("%s = %p,  Null Pointer!\n", #val, val)

/* function trace log, strictly prohibited to expand */
#define sap_err_log_ret(err_code) audio_log_err("Error Code: [0x%08X]\n", err_code)
/* function trace log, print the called function name when function is error */
#define sap_err_log_fun(func, err_code) audio_log_err("Call %s Failed, Error Code: [0x%08X]\n", #func, err_code)
/* Function trace log, print the pointer name when pointer is null */
#define sap_err_log_null_pointer(val) audio_log_err("%s = %p,  Null Pointer!\n", #val, val)

/* function trace log, strictly prohibited to expand */
#define sap_warn_log_ret(err_code) audio_log_warn("Error Code: [0x%08X]\n", err_code)
/* function trace log, print the called function name when function is error */
#define sap_warn_log_fun(func, err_code) audio_log_warn("Call %s Failed, Error Code: [0x%08X]\n", #func, err_code)
/* Function trace log, print the pointer name when pointer is null */
#define sap_warn_log_null_pointer(val) audio_log_warn("%s = %p,  Null Pointer!\n", #val, val)

/* function trace log, strictly prohibited to expand */
#define sap_trace_log_ret(err_code) audio_log_trace("Error Code: [0x%08X]\n", err_code)
/* function trace log, print the called function name when function is error */
#define sap_trace_log_fun(func, err_code) audio_log_trace("Call %s Failed, Error Code: [0x%08X]\n", #func, \
    (td_u32)(err_code))
/* Function trace log, print the pointer name when pointer is null */
#define sap_trace_log_null_pointer(val) audio_log_trace("%s = %p,  Null Pointer!\n", #val, val)

/* Used for displaying more detailed alert information */
#define sap_alert_log_s32(val)   audio_log_alert("%s = %d",        #val, val)
#define sap_alert_log_u32(val)   audio_log_alert("%s = %u",        #val, val)
#define sap_alert_log_s64(val)   audio_log_alert("%s = %lld",      #val, val)
#define sap_alert_log_u64(val)   audio_log_alert("%s = %llu",      #val, val)
#define sap_alert_log_h32(val)   audio_log_alert("%s = 0x%08X",    #val, val)
#define sap_alert_log_h64(val)   audio_log_alert("%s = 0x%016llX", #val, val)
#define sap_alert_log_str(val)   audio_log_alert("%s = %s",        #val, val)
#define sap_alert_log_void(val)  audio_log_alert("%s = %p",        #val, val)
#define sap_alert_log_float(val) audio_log_alert("%s = %f",        #val, val)
#define sap_alert_log_bool(val)  audio_log_alert("%s = %s",        #val, (val) ? "True" : "False")
#define sap_alert_log_info(val)  audio_log_alert("<%s>",            val)

/* Used for displaying more detailed fatal information */
#define sap_fatal_log_s32(val) audio_log_fatal("%s = %d", #val, val)
#define sap_fatal_log_u32(val) audio_log_fatal("%s = %u", #val, val)
#define sap_fatal_log_s64(val) audio_log_fatal("%s = %lld", #val, val)
#define sap_fatal_log_u64(val) audio_log_fatal("%s = %llu", #val, val)
#define sap_fatal_log_h32(val) audio_log_fatal("%s = 0x%08X", #val, val)
#define sap_fatal_log_h64(val) audio_log_fatal("%s = 0x%016llX", #val, val)
#define sap_fatal_log_str(val) audio_log_fatal("%s = %s", #val, val)
#define sap_fatal_log_void(val) audio_log_fatal("%s = %p", #val, val)
#define sap_fatal_log_float(val) audio_log_fatal("%s = %f", #val, val)
#define sap_fatal_log_bool(val) audio_log_fatal("%s = %s", #val, (val) ? "True" : "False")
#define sap_fatal_log_info(val) audio_log_fatal("<%s>", val)

/* Used for displaying more detailed error information */
#define sap_err_log_s32(val) audio_log_err("%s = %d", #val, val)
#define sap_err_log_u32(val) audio_log_err("%s = %u", #val, val)
#define sap_err_log_s64(val) audio_log_err("%s = %lld", #val, val)
#define sap_err_log_u64(val) audio_log_err("%s = %llu", #val, val)
#define sap_err_log_h32(val) audio_log_err("%s = 0x%08X", #val, val)
#define sap_err_log_h64(val) audio_log_err("%s = 0x%016llX", #val, val)
#define sap_err_log_str(val) audio_log_err("%s = %s", #val, val)
#define sap_err_log_void(val) audio_log_err("%s = %p", #val, val)
#define sap_err_log_float(val) audio_log_err("%s = %f", #val, val)
#define sap_err_log_bool(val) audio_log_err("%s = %s", #val, (val) ? "True" : "False")
#define sap_err_log_info(val) audio_log_err("<%s>", val)

/* Used for displaying more detailed warning information */
#define sap_warn_log_s32(val) audio_log_warn("%s = %d\n", #val, val)
#define sap_warn_log_u32(val) audio_log_warn("%s = %u\n", #val, val)
#define sap_warn_log_s64(val) audio_log_warn("%s = %lld\n", #val, val)
#define sap_warn_log_u64(val) audio_log_warn("%s = %llu\n", #val, val)
#define sap_warn_log_h32(val) audio_log_warn("%s = 0x%08X\n", #val, val)
#define sap_warn_log_h64(val) audio_log_warn("%s = 0x%016llX\n", #val, val)
#define sap_warn_log_str(val) audio_log_warn("%s = %s\n", #val, val)
#define sap_warn_log_void(val) audio_log_warn("%s = %p\n", #val, val)
#define sap_warn_log_float(val) audio_log_warn("%s = %f\n", #val, val)
#define sap_warn_log_bool(val) audio_log_warn("%s = %s\n", #val, (val) ? "True" : "False")
#define sap_warn_log_info(val) audio_log_warn("<%s>\n", val)

/* Only used for self debug, Can be expanded as needed */
#define sap_dbg_log_s32(val) audio_log_dbg("%s = %d", #val, val)
#define sap_dbg_log_u32(val) audio_log_dbg("%s = %u", #val, val)
#define sap_dbg_log_s64(val) audio_log_dbg("%s = %lld", #val, val)
#define sap_dbg_log_u64(val) audio_log_dbg("%s = %llu", #val, val)
#define sap_dbg_log_h32(val) audio_log_dbg("%s = 0x%08X", #val, val)
#define sap_dbg_log_h64(val) audio_log_dbg("%s = 0x%016llX", #val, val)
#define sap_dbg_log_str(val) audio_log_dbg("%s = %s", #val, val)
#define sap_dbg_log_void(val) audio_log_dbg("%s = %p", #val, val)
#define sap_dbg_log_float(val) audio_log_dbg("%s = %f", #val, val)
#define sap_dbg_log_bool(val) audio_log_dbg("%s = %s", #val, (val) ? "True" : "False")
#define sap_dbg_log_info(val) audio_log_dbg("<%s>", val)

/* Only used for trace, Can be expanded as needed */
#define sap_trace_log_s32(val) audio_log_trace("%s = %d", #val, val)
#define sap_trace_log_u32(val) audio_log_trace("%s = %u", #val, val)
#define sap_trace_log_s64(val) audio_log_trace("%s = %lld", #val, val)
#define sap_trace_log_u64(val) audio_log_trace("%s = %llu", #val, val)
#define sap_trace_log_h32(val) audio_log_trace("%s = 0x%08X", #val, val)
#define sap_trace_log_h64(val) audio_log_trace("%s = 0x%016llX", #val, val)
#define sap_trace_log_str(val) audio_log_trace("%s = %s", #val, val)
#define sap_trace_log_void(val) audio_log_trace("%s = %p", #val, val)
#define sap_trace_log_float(val) audio_log_trace("%s = %f", #val, val)
#define sap_trace_log_bool(val) audio_log_trace("%s = %s", #val, (val) ? "True" : "False")
#define sap_trace_log_info(val) audio_log_trace("<%s>", val)
#define sap_trace_log_reg(val) audio_log_trace("[0x%08X = 0x%08X (%s)]", &(val), (val), #val)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __AUDIO_DEBUG_UART_H__ */
