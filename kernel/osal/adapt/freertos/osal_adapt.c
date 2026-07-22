/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: OAL OS
 * Author:
 * Create:
 */
#include <pthread.h>
#include "stdint.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#if defined(TEST_SUITE)
#include "test_suite_uart.h"
#endif
#if defined(SW_RTT_DEBUG)
#include "SEGGER_RTT.h"
#endif
#include "debug_print.h"
#include "arch_barrier.h"
#include "common_def.h"
#include "osal_adapt.h"

print_cb_t g_osal_adapt_print = NULL;

void vAssertCalled(const char *pcFile, uint32_t ulLine)
{
    static volatile uint8_t ulSetTo1ToExitFunction = 0;

    PRINT("[Assert failed][%s][%u]\n", pcFile, ulLine);

    taskDISABLE_INTERRUPTS();

    while (ulSetTo1ToExitFunction != 1) {
        uapi_reg_write(0x0, 0x5A6A);
        __asm volatile("NOP");
    }
}

int pthread_rwlock_unlock(pthread_rwlock_t *rw)
{
    return 0;
}

int pthread_rwlock_wrlock(pthread_rwlock_t *rw)
{
    return 0;
}

int pthread_rwlock_rdlock(pthread_rwlock_t *rw)
{
    return 0;
}

locale_t newlocale(int mask, const char *name, locale_t loc)
{
    return 0;
}

void freelocale(locale_t l)
{
    return;
}

locale_t uselocale(locale_t new)
{
    return 0;
}

float wcstof(const wchar_t *restrict s, wchar_t **restrict p)
{
    return 0;
}

double wcstod(const wchar_t *restrict s, wchar_t **restrict p)
{
    return 0;
}

long double wcstold(const wchar_t *restrict s, wchar_t **restrict p)
{
    return 0;
}

struct lconv *localeconv(void)
{
    return 0;
}

void osal_dsb(void)
{
    dsb();
    return;
}

void osal_isb(void)
{
    isb();
    return;
}

void osal_dcache_flush_all(void)
{
    ArchDCacheFlush();
}

void osal_register_print(print_cb_t print)
{
    g_osal_adapt_print = print;
}

unsigned int osal_uart_printk(const char *fmt, va_list ap)
{
    #define PRINT_BUF_SIZE 256
    static char buff[PRINT_BUF_SIZE] = {0};
    const char *p = "Output illegal string! vsnprintf_s failed!\n";

    (void)memset_s(buff, PRINT_BUF_SIZE, 0, PRINT_BUF_SIZE);
    int len = vsnprintf_s(buff, PRINT_BUF_SIZE, PRINT_BUF_SIZE - 1, fmt, ap);
    if (len < 0 && buff[0] == 0) {
        (void)memcpy_s(buff, PRINT_BUF_SIZE, p, (size_t)strlen(p));
    }

#if defined(SW_UART_DEBUG)
    uapi_uart_write(SW_DEBUG_UART_BUS, (const uint8_t *)buff, (uint32_t)strlen(buff), 0);
#elif defined(TEST_SUITE)
    test_suite_uart_send(buff);
#elif defined(SW_RTT_DEBUG)
    SEGGER_RTT_Write(0, (const char *)buff, (uint32_t)strlen(buff));
#else
    if (g_osal_adapt_print != NULL) {
        g_osal_adapt_print(buff);
    }
#endif
    return len;
}