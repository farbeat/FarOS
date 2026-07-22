#include <stdio.h>
#include <stdarg.h>
#ifdef __LITEOS__
#include "los_printf_pri.h"
#endif

int printf(const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
#if defined(LOSCFG_LIB_STDIO) && defined(LOSCFG_KERNEL_CONSOLE)
	ret = vfprintf(stdout, fmt, ap);
#elif defined(LOSCFG_DRIVERS_UART) || defined(LOSCFG_DRIVERS_SIMPLE_UART)
	UartVprintf(fmt, ap);
	ret = 0;
#else
	ret = -1;
#endif
	va_end(ap);
	return ret;
}
