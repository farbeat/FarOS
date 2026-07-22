#include <string.h>

#if defined(LOSCFG_LMS_LIBC_HIGH_FREQ_FUNC_CHECK)
__attribute__((no_sanitize_address)) char *__strncpy(char *restrict d, const char *restrict s, size_t n)
#else
char *strncpy(char *restrict d, const char *restrict s, size_t n)
#endif
{
	__stpncpy(d, s, n);
	return d;
}
