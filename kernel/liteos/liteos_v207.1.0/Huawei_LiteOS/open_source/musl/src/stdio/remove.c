#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include "syscall.h"
#ifdef __LITEOS__
#include "unistd.h"
#include "sys/stat.h"
#endif

int remove(const char *path)
{
#ifdef __LITEOS__
	struct stat sb;

	if (lstat(path, &sb) < 0)
		return -1;
	if (S_ISDIR(sb.st_mode))
		return rmdir(path);
	return unlink(path);
#else
#ifdef SYS_unlink
	int r = __syscall(SYS_unlink, path);
#else
	int r = __syscall(SYS_unlinkat, AT_FDCWD, path, 0);
#endif
#ifdef SYS_rmdir
	if (r==-EISDIR) r = __syscall(SYS_rmdir, path);
#else
	if (r==-EISDIR) r = __syscall(SYS_unlinkat, AT_FDCWD, path, AT_REMOVEDIR);
#endif
	return __syscall_ret(r);
#endif
}
