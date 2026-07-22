#include <string.h>

#if defined(LOSCFG_LMS_LIBC_HIGH_FREQ_FUNC_CHECK)
__attribute__((no_sanitize_address)) char *__strcpy(char *restrict dest, const char *restrict src)
#else
char *strcpy(char *restrict dest, const char *restrict src)
#endif
{
	__stpcpy(dest, src);
	return dest;
}
