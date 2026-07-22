#include <stdio.h>
#include "getc.h"

int getchar(void)
{
#if defined(LOSCFG_FS_VFS)
	return do_getc(stdin);
#else
	return EOF;
#endif
}
