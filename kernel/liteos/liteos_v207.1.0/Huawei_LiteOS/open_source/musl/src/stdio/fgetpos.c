#include "stdio_impl.h"

int fgetpos(FILE *restrict f, fpos_t *restrict pos)
{
	off_t off = __ftello(f);
	if (off < 0) return -1;
#ifdef __LITEOS__
	*(fpos_t *)pos = off;
#else
	*(long long *)pos = off;
#endif
	return 0;
}

weak_alias(fgetpos, fgetpos64);
