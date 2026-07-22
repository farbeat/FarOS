#ifdef __XLT_OS__
#include "libc_portable.h"
#endif
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>

int tcsetattr(int fd, int act, const struct termios *tio)
{
#ifndef __XLT_OS__
	if (act < 0 || act > 2) {
		errno = EINVAL;
		return -1;
	}
	return ioctl(fd, TCSETS+act, tio);
#else
	(void)fd;
	(void)act;
	(void)tio;
	PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
	errno = ENOSYS;
	return -1;
#endif
}
