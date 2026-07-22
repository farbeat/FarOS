/*
 * Copyright (c) @CompanyNameMagicTag. 2021-2022. All rights reserved.
 * Description: osal debug adapt
 * Author:
 * Create: 2022-05-16
 */


#include <los_printf.h>
#include <los_config.h>
#include <los_printf_pri.h>
#include "soc_osal.h"
#include "los_exc.h"
#include "std_def.h"
#include "platform_core.h"

#ifdef TEST_SUITE
#include "test_suite_uart.h"
#endif
#if defined(SW_RTT_DEBUG)
#include "SEGGER_RTT.h"
#endif

void UartPuts(const CHAR *s, uint32_t len, BOOL isLock)
{
    if ((s == NULL) || (strlen(s) == 0)) {
        return;
    }

    UNUSED(isLock);
#ifdef SW_UART_DEBUG
    uapi_uart_write(SW_DEBUG_UART_BUS, (const void *)s, len, 0);
#elif defined(TEST_SUITE)
    test_suite_uart_sendf(s);
#elif defined(SW_RTT_DEBUG)
    SEGGER_RTT_printf(0, "%s\n", s);
#else
    UNUSED(s);
    UNUSED(len);
#endif
}

void osal_dcache_flush_all(void)
{
    ArchDCacheFlush();
}
