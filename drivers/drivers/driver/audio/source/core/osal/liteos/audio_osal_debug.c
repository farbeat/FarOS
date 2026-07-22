/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: API of audio debug
 * Author: audio
 */

#include <stdarg.h>
#include "audio_debug.h"
#include "securec.h"
#include "soc_osal.h"

#if (SAP_CHIP_TYPE == socmn1)
#include "ha_usart.h"
#define audio_uart_printf UART_PRINTF
#else
#ifdef BRANDY_PROT
#define UART_PRINTF
#define audio_uart_printf
#else
#define audio_uart_printf osal_printk
#endif /* BRANDY_PROT */
#endif /* SAP_CHIP_TYPE */

#ifdef BRANDY_PROT
#elif (SAP_CHIP_TYPE == socmn1)
#endif /* BRANDY_PROT */

#define MAX_PRINT_SIZE 128

td_void audio_osal_print(const td_char *fmt, ...)
{
#if (AUDIO_LOG_SWITCH_A != 0) /* log enable */
    int ret;
    va_list args;
    char str[MAX_PRINT_SIZE];

    va_start(args, fmt);
    ret = vsnprintf_s(str, sizeof(str), sizeof(str) - 1, fmt, args);
    va_end(args);
    if (ret < 0) {
        return;
    }

    audio_uart_printf("%s", str);
#else
    audio_unused(fmt);
#endif /* AUDIO_LOG_SWITCH_A */
}

td_void audio_osal_print_fun(const td_char *core, td_u8 level, const td_char *fn_name, td_u32 line_num,
    const td_char *format, ...)
{
#if (AUDIO_LOG_SWITCH_A != 0) // log enable
    td_s32 ret;
    va_list args;
    td_char buf[MAX_PRINT_SIZE + 1] = {'\0'}; /* add '\n' */
    td_u32 msg_len;

    char *level_str[] = {
        "ALERT",
        "FATAL",
        "ERROR",
        "WARN ",
        "INFO ",
        "DEBUG",
        "TRACE",
    };

    if (core == TD_NULL || level > AUDIO_LOG_LEVEL_TRACE || fn_name == TD_NULL) {
        return;
    }

    ret = snprintf_s(buf, MAX_PRINT_SIZE, MAX_PRINT_SIZE - 1,
        "[%s][%s][%s:%u] ", core, level_str[level], fn_name, line_num);
    if (ret < 0) {
        audio_osal_print("func head Call snprintf_s failed with %d\n", ret);
        return;
    }

    msg_len = (td_u32)ret;

    va_start(args, format);
    ret = vsnprintf_s(buf + msg_len, MAX_PRINT_SIZE - msg_len,
        MAX_PRINT_SIZE - msg_len - 1, format, args);
    va_end(args);
    if (ret < 0) {
        audio_osal_print("func log Call vsnprintf_s failed with %d\n", ret);
        return;
    }

    msg_len += (td_u32)ret;
    buf[msg_len] = '\n';

    audio_uart_printf("%s", buf);
#else
    audio_unused(core);
    audio_unused(level);
    audio_unused(fn_name);
    audio_unused(line_num);
    audio_unused(format);
#endif /* AUDIO_LOG_SWITCH_A */
}
