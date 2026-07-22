#include "time_impl.h"
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include "tzdst_pri.h"
#include "lock.h"

struct tm *__localtime_r(const time_t *restrict t, struct tm *restrict tm)
{
#ifndef __XLT_OS__
	/* Reject time_t values whose year would overflow int because
	 * __secs_to_zone cannot safely handle them. */
	if (*t < INT_MIN * 31622400LL || *t > INT_MAX * 31622400LL) {
		errno = EOVERFLOW;
		return 0;
	}
	__secs_to_zone(*t, 0, &tm->tm_isdst, &tm->__tm_gmtoff, 0, &tm->__tm_zone);
	if (__secs_to_tm((long long)*t + tm->__tm_gmtoff, tm) < 0) {
		errno = EOVERFLOW;
		return 0;
	}
	return tm;
#else
	(void)LIBC_LOCK(g_tzdst_lock);
	time_t timeoff;
	int dstsec = 0;
	if (dst_period_check(NULL, *t) == true)
	 	dstsec = dst_forward_sec_get();
	timeoff = *t + timezone + dstsec;
	(void)LIBC_UNLOCK(g_tzdst_lock);

	/* Reject time_t values whose year would overflow int because
	 * __secs_to_zone cannot safely handle them. */
	if (timeoff < INT_MIN * 31622400LL || timeoff > INT_MAX * 31622400LL) {
		errno = EOVERFLOW;
		return 0;
	}

	if (__secs_to_tm((long long)timeoff, tm) < 0) {
		errno = EOVERFLOW;
		return 0;
	}
	return tm;
#endif
}

weak_alias(__localtime_r, localtime_r);
