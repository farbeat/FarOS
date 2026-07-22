#ifndef __LP64__
#include "time64.h"
#include "time_impl.h"
#include "errno.h"
#include "limits.h"
#include "tzdst_pri.h"
#include "lock.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TM tm
#define RETURN_STRING_LEN 35
#define MAX_SEC_PER_YEAR (366LL * 24LL * 60LL * 60LL)
#define TM_YEAR_BASE 1900
#define TM_YEAR_MAX  9999
typedef time64_t Time64_T;
static struct TM g_returnDate;
static char g_returnString[RETURN_STRING_LEN];

static int valid_tm_wday(const struct TM *date)
{
    if ((date->tm_wday >= 0) && (date->tm_wday <= 6)) {
        return 1;
    } else {
        return 0;
    }
}

static int valid_tm_mon(const struct TM *date)
{
    if ((date->tm_mon >= 0) && (date->tm_mon <= 11)) {
        return 1;
    } else {
        return 0;
    }
}

struct TM *localtime64_r(const Time64_T *time, struct TM *local_tm)
{
    Time64_T timeoff;
    int dstsec = 0;

    (void)LIBC_LOCK(g_tzdst_lock);
    if (dst_period_check(NULL, *time) == true) {
        dstsec = dst_forward_sec_get();
    }
    timeoff = *time + timezone + dstsec;
    (void)LIBC_UNLOCK(g_tzdst_lock);

    /* Reject time_t values whose year would overflow int because
     * __secs_to_zone cannot safely handle them. */
    if ((timeoff < (INT_MIN * MAX_SEC_PER_YEAR)) || (timeoff > (INT_MAX * MAX_SEC_PER_YEAR))) {
        errno = EOVERFLOW;
        return 0;
    }

    if (__secs_to_tm(timeoff, local_tm) < 0) {
        errno = EOVERFLOW;
        return 0;
    }
    return local_tm;
}

struct TM *localtime64(const Time64_T *time)
{
    return localtime64_r(time, &g_returnDate);
}

char *asctime64_r(const struct TM *date, char *result)
{
    if (!valid_tm_wday(date) || !valid_tm_mon(date)) {
        return NULL;
    }

    if ((TM_YEAR_BASE + date->tm_year) > TM_YEAR_MAX) {
        return NULL;
    }

    return asctime_r(date, result);
}

char *asctime64(const struct TM *date)
{
    return asctime64_r(date, g_returnString);
}

char *ctime64(const Time64_T *time)
{
    struct TM *ptm = localtime64(time);
    if (ptm != NULL) {
        return asctime64(ptm);
    }
    return NULL;
}

Time64_T mktime64(struct TM *tm)
{
    long long t = __tm_to_secs(tm);
    int dstsec = 0;

    (void)LIBC_LOCK(g_tzdst_lock);
    if (tm->tm_isdst != 0) {
        if (dst_period_check(tm, 0) == true) {
            dstsec = dst_forward_sec_get();
        }
        tm->tm_isdst = 0;
    }
    t = t - timezone - dstsec;
    (void)LIBC_UNLOCK(g_tzdst_lock);

    if ((Time64_T)t != t) {
        goto error;
    }

    return t;

error:
    (void)LIBC_UNLOCK(g_tzdst_lock);
    errno = EOVERFLOW;
    return -1;
}

struct TM *gmtime64_r(const Time64_T *in_time, struct TM *tm)
{
    if (__secs_to_tm(*in_time, tm) < 0) {
        errno = EOVERFLOW;
        return 0;
    }
    tm->tm_isdst = 0;
    tm->__tm_gmtoff = 0;
    tm->__tm_zone = __utc;
    return tm;
}

struct TM *gmtime64(const Time64_T *time)
{
    return gmtime64_r(time, &g_returnDate);
}

char *ctime64_r(const Time64_T *time, char *result)
{
    struct TM date;

    (void)localtime64_r(time, &date);
    return asctime64_r(&date, result);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
