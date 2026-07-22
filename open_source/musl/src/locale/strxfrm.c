#include <string.h>
#include <locale.h>
#include "locale_impl.h"
#include "assert.h"

/* collate only by code points */
size_t __strxfrm_l(char *restrict dest, const char *restrict src, size_t n, locale_t loc)
{
	size_t l = strlen(src);
#ifdef __XLT_OS__
	if (n != 0) (void)strlcpy(dest, src, n);
#else
	if (n > l) strcpy(dest, src);
#endif
	return l;

}

size_t strxfrm(char *restrict dest, const char *restrict src, size_t n)
{
	return __strxfrm_l(dest, src, n, CURRENT_LOCALE);
}

weak_alias(__strxfrm_l, strxfrm_l);
