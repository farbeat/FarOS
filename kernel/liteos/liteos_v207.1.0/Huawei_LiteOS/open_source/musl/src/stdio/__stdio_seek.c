#include "stdio_impl.h"
#include <unistd.h>

off_t __stdio_seek(FILE *f, off_t off, int whence)
{
#ifdef __LITEOS__
	return lseek(f->fd, off, whence);
#else
	return __lseek(f->fd, off, whence);
#endif
}

#ifdef __LITEOS__
off64_t __stdio_seek64(FILE *f, off64_t off, int whence)
{
	return lseek64(f->fd, off, whence);
}
#endif
