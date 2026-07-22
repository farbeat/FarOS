#include "stddef.h"
#include "errno.h"
#include "sys/types.h"
#include "sys/random.h"
#include "sys/param.h"

#ifdef CONFIG_LIBC_HW_RANDOM_ENABLE
#include "random.h"
#endif

static int getentropy_urandom(void *buf, size_t len) {
#ifdef CONFIG_LIBC_HW_RANDOM_ENABLE
	if (HiRandomHwSupport() == 0) {
		errno = EIO;
		return -1;
	}
	HiRandomHwInit();

	ssize_t ret = HiRandomHwGetNumber((char *)buf, len);
	if (ret != 0) {
		errno = EIO;
		return -1;
	}
	return 0;
#else
	(void)buf;
	(void)len;
	errno = EIO;
	return -1;
#endif
}

int getentropy(void *buf, size_t len) {
	int ret;

	if (len > 256) {
		errno = EIO;
		return -1;
	}

	ret = getentropy_urandom(buf, len);
	if (ret == -1) {
		errno = EIO;
	}
	return ret;
}

