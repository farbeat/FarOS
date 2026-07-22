/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: PAL PRINT
 * Author:
 * Create: 2021-05-18
 */

#include "non_os.h"
#include "securec.h"
#ifdef TEST_SUITE
#include "test_suite_uart.h"
#endif
#if defined(SW_RTT_DEBUG)
#include "SEGGER_RTT.h"
#endif
#ifdef SW_UART_DEBUG
#include "uart.h"
#endif

#define UART_TRANS_LEN_MAX 256

void pal_print(const char *str, ...)
{
#ifdef SW_UART_DEBUG
    if (!non_os_is_driver_initialised(DRIVER_INIT_UART)) { return; }

    static uint8_t s[UART_TRANS_LEN_MAX];  // This needs to be large enough to store the string
    va_list args;
    int32_t str_len;

    if ((str == NULL) || (strlen(str) == 0)) {
        return;
    }

    //lint -esym(526, __builtin_va_start) -esym(628, __builtin_va_start)   Lint hopelessly confused about va_args
    va_start(args, str);  //lint !e530 Lint hopelessly confused about va_args
    str_len = vsprintf_s((char *)s, sizeof(s), str, args);
    va_end(args);

    if (str_len < 0) {
        return;
    }
    uapi_uart_write(SW_DEBUG_UART_BUS, (const void *)s, (uint16_t)str_len, 0);
#elif defined(TEST_SUITE)
    static char s[UART_TRANS_LEN_MAX];  // This needs to be large enough to store the string
    int32_t str_len;

    va_list args;
    va_start(args, str); //lint !e530
    str_len = vsprintf_s(s, sizeof(s), str, args);
    va_end(args);

    if (str_len < 0) {
        return;
    }

    test_suite_uart_send(s);
#elif defined(SW_RTT_DEBUG)
    va_list paramList;

    va_start(paramList, str);
    SEGGER_RTT_vprintf(0, str, &paramList);
    va_end(paramList);
    SEGGER_RTT_vprintf(0, str, RTT_CTRL_RESET);
    return;
#else
    UNUSED(str);
#endif
}
