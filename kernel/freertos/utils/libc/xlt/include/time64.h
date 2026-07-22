#ifndef _HW_TIME64_H_
#define _HW_TIME64_H_

#if defined(__LP64__)
#error Your time_t is already 64-bit.
#else
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

#if defined(__XLT_OS__) && !defined(__LP64___)
#include <bits/alltypes.h>
#endif /* __XLT_OS__ && !__LP64___ */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if defined(__XLT_OS__) && !defined(__LP64___)
#ifndef __NEED_int64_t
#define __NEED_int64_t
#endif

struct timeval64 {
    int64_t tv_sec;
    int64_t tv_usec;
};

struct timespec64 {
    int64_t tv_sec;
    int64_t tv_nsec;
};

/**
 * @ingroup  time
 * @par Description:
 * This function  sets the time as well as a timezone in 32-bit system.
 *
 * @attention
 * <ul>
 * <li>The function is not supported to set timezone,So the second parameter is unused</li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid Input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time64.h</li></ul>
 *
 * @see  clock_gettime | time | ctime
 */
int settimeofday64(const struct timeval64 *, const struct timezone *);

/**
 * @ingroup  time
 * @par Description:
 * This function  gets the time as well as a timezone in 32-bit system.
 *
 * @attention
 * <ul>
 * <li>The function is not supported to get timezone,So the second parameter is unused</li>
 * </ul>
 *
 * @retval #0  The function is executed successfully.
 * @retval #-1 The function failed to execute, and corresponding error code is set.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: An invalid Input.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>time64.h</li></ul>
 *
 * @see  clock_gettime | time | ctime
 */
int gettimeofday64(struct timeval64 *, struct timezone *);
#endif

typedef int64_t time64_t;

char *asctime64(const struct tm *);
char *asctime64_r(const struct tm *, char *);
char *ctime64(const time64_t *);
char *ctime64_r(const time64_t *, char *);
struct tm *gmtime64(const time64_t *);
struct tm *gmtime64_r(const time64_t *, struct tm *);
struct tm *localtime64(const time64_t *);
struct tm *localtime64_r(const time64_t *, struct tm *);
time64_t mktime64(struct tm *);
time64_t timegm64(const struct tm *);
time64_t timelocal64(const struct tm *);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __LP64__ */
#endif /* _HW_TIME64_H_ */
