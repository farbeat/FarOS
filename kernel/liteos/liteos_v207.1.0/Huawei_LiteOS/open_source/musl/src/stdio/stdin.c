#include "stdio_impl.h"

#undef stdin

/*
 * LiteOS stdin is _IONBF, to fix the bug that twice read of
 * console will block os, when there is no data in console,
 * musl stdin is _IOFBF, and buff size is 'sizeof buf-UNGET',
 * if stdin reopened as file, setbuf() can enable stdin buff.
 */
static unsigned char buf[BUFSIZ+UNGET];
hidden FILE __stdin_FILE = {
	.buf = buf+UNGET,
#ifdef __LITEOS__
	.buf_size = 0,
#else
	.buf_size = sizeof buf-UNGET,
#endif
	.fd = 0,
	.flags = F_PERM | F_NOWR,
	.read = __stdio_read,
	.seek = __stdio_seek,
	.close = __stdio_close,
#ifndef __LITEOS__
	.lock = -1,
#else
	/* lock always check */
	.lock = 0,
	__INIT_LOCK_ATTR(._lock),
#endif
};
FILE *const stdin = &__stdin_FILE;
FILE *volatile __stdin_used = &__stdin_FILE;
