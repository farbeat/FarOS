#include <stdlib.h>
#include <stdint.h>
#ifdef __XLT_OS__
#include <errno.h>
#include "libc_portable.h"
#endif

#ifndef __XLT_OS__
#include "libc.h"

static void dummy()
{
}

/* atexit.c and __stdio_exit.c override these. the latter is linked
 * as a consequence of linking either __toread.c or __towrite.c. */
weak_alias(dummy, __funcs_on_exit);
weak_alias(dummy, __stdio_exit);
weak_alias(dummy, _fini);

extern weak hidden void (*const __fini_array_start)(void), (*const __fini_array_end)(void);

static void libc_exit_fini(void)
{
	uintptr_t a = (uintptr_t)&__fini_array_end;
	for (; a>(uintptr_t)&__fini_array_start; a-=sizeof(void(*)()))
		(*(void (**)())(a-sizeof(void(*)())))();
	_fini();
}

weak_alias(libc_exit_fini, __libc_exit_fini);
#endif

_Noreturn void exit(int code)
{
#ifndef __XLT_OS__
	__funcs_on_exit();
	__libc_exit_fini();
	__stdio_exit();
	_Exit(code);
#else
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    errno = ENOSYS;
    while (1);
#endif
}
