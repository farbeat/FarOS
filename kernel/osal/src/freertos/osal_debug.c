/*
 * Copyright (c) CompanyNameMagicTag 2019-2022. All rights reserved.
 * Description: osal debug source file.
 * Create: 2019-11-11
 */
#include <stdarg.h>
#include <FreeRTOS.h>
#include "print.h"

void osal_printk(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vUartVprintk(ePrintkErr, fmt, ap);
    va_end(ap);
}

void osal_flush_cache(void)
{
    ArchDCacheFlush();
}