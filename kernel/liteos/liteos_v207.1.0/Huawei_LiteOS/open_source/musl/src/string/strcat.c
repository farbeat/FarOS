#include <string.h>

#if defined(LOSCFG_LMS_LIBC_HIGH_FREQ_FUNC_CHECK)
__attribute__((no_sanitize_address)) char *__strcat(char *restrict dest, const char *restrict src)
#else
char *strcat(char *restrict dest, const char *restrict src)
#endif
{
	strcpy(dest + strlen(dest), src);
	return dest;
}
