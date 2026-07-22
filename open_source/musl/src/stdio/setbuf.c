#include <stdio.h>
#if defined(__XLT_OS__) && defined(CONFIG_LIBC_LLTSER)
#include "gcov_ser.h"
#endif

void setbuf(FILE *restrict f, char *restrict buf)
{
#if defined(__XLT_OS__) && defined(CONFIG_LIBC_LLTSER)
	GCOV_SETBUF(f);
#endif
	setvbuf(f, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
}
