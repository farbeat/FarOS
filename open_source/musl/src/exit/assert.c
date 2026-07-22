#include <stdio.h>
#include <stdlib.h>
#ifdef __XLT_OS__
#include "libc_portable.h"
#endif

_Noreturn void __assert_fail(const char *expr, const char *file, int line, const char *func)
{
#if defined(__XLT_OS__) && !defined(CONFIG_LIBC_STDIO)
	PRINT_ERR("Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line);
#else
	fprintf(stderr, "Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line);
	fflush(NULL);
#endif
	abort();
}
