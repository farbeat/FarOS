#include "stdio_impl.h"
#include "aio_impl.h"
#ifdef __LITEOS__
#include "unistd.h"
#endif

static int dummy(int fd)
{
	return fd;
}

weak_alias(dummy, __aio_close);

int __stdio_close(FILE *f)
{
#ifdef __LITEOS__
	return close(__aio_close(f->fd));
#else
	return syscall(SYS_close, __aio_close(f->fd));
#endif
}
