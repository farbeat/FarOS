#include "stdio_impl.h"
#include <fcntl.h>
#include <unistd.h>

/* The basic idea of this implementation is to open a new FILE,
 * hack the necessary parts of the new FILE into the old one, then
 * close the new FILE. */

/* Locking IS necessary because another thread may provably hold the
 * lock, via flockfile or otherwise, when freopen is called, and in that
 * case, freopen cannot act until the lock is released. */

#ifdef __LITEOS__
#define IS_STANDARD_STREAM(f) ((f) == stdin || (f) == stdout || (f) == stderr)
#endif

FILE *freopen(const char *restrict filename, const char *restrict mode, FILE *restrict f)
{
	int fl = __fmodeflags(mode);
	FILE *f2;
#ifdef __LITEOS__
	/* if dup2() success, use original file fd and close new file */
	int needclose_newfile = 1;
#endif

	FLOCK(f);

	fflush(f);

	if (!filename) {
		if (fl&O_CLOEXEC)
			fcntl(f->fd, F_SETFD, FD_CLOEXEC);
		fl &= ~(O_CREAT|O_EXCL|O_CLOEXEC);
		if (fcntl(f->fd, F_SETFL, fl) < 0)
			goto fail;
	} else {
		f2 = fopen(filename, mode);
		if (!f2) goto fail;
		if (f2->fd == f->fd) f2->fd = -1; /* avoid closing in fclose */
#ifdef __LITEOS__
		else if (IS_STANDARD_STREAM(f) || dup2(f2->fd, f->fd) < 0) {
			fclose(f); /* close original file, use new file fd */
			f->fd = f2->fd;
			needclose_newfile = 0;
		}
#else
		else if (dup2(f2->fd, f->fd)<0) goto fail2;
#endif

		f->flags = (f->flags & F_PERM) | f2->flags;
		f->read = f2->read;
		f->write = f2->write;
		f->seek = f2->seek;
#ifdef __LITEOS__
		f->seek64 = f2->seek64;
#endif
		f->close = f2->close;

#ifdef __LITEOS__
		if (needclose_newfile) {
			fclose(f2);
		}
#else
		fclose(f2);
#endif
	}

	FUNLOCK(f);
	return f;

#ifndef __LITEOS__
fail2:
	fclose(f2);
#endif
fail:
	fclose(f);
	return NULL;
}

weak_alias(freopen, freopen64);
