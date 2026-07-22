#include "libc_types.h"
#include "sys/timertc.h"
#include "time64.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static xlt_time_rtc_ops g_rtc_ops;

const xlt_time_rtc_ops *xlt_time_rtc_ops_get(void)
{
    return &g_rtc_ops;
}

int32_t xlt_time_rtc_ops_set(const xlt_time_rtc_ops *ops)
{
    if (ops == NULL) {
        return -1;
    }

    g_rtc_ops.enable = ops->enable;
    g_rtc_ops.time_us_set = ops->time_us_set;
    g_rtc_ops.time_us_get = ops->time_us_get;
    g_rtc_ops.time_zone_set = ops->time_zone_set;
    g_rtc_ops.time_zone_get = ops->time_zone_get;

    return 0;
}

static inline uint64_t utils_time_us_get(void)
{
    if (g_rtc_ops.time_us_get != NULL) {
        return g_rtc_ops.time_us_get();
    }

    return 0;
}

static inline uint32_t utils_time_us_set(uint64_t time)
{
    if (g_rtc_ops.time_us_set != NULL) {
        return g_rtc_ops.time_us_set(time);
    }

    return 0;
}

static inline int32_t utils_time_zone_set(const struct timezone *tz)
{
    if (tz == NULL) {
        return 0;
    }

    if (g_rtc_ops.time_zone_set != NULL) {
        return g_rtc_ops.time_zone_set(tz);
    }

    return 0;
}

static inline int32_t utils_time_zone_get(struct timezone *tz)
{
    if (tz == NULL) {
        return 0;
    }

    if (g_rtc_ops.time_zone_get != NULL) {
        return g_rtc_ops.time_zone_get(tz);
    }

    return 0;
}

int32_t rtc_gettimeofday(struct timeval64 *tv, struct timezone *tz)
{
    uint64_t nowUsec;

    nowUsec = utils_time_us_get();
    if (nowUsec == 0) {
        tv->tv_sec = 0;
        tv->tv_usec = 0;
    } else {
        tv->tv_sec = (int64_t)(nowUsec / MICROSECONDS_PER_SECOND);
        tv->tv_usec = (int64_t)(nowUsec - tv->tv_sec * MICROSECONDS_PER_SECOND);
    }

    if (utils_time_zone_get(tz) != 0) {
        return -1;
    }

    return 0;
}

int32_t rtc_settimeofday(const struct timeval64 *tv, const struct timezone *tz)
{
    uint64_t usec;
    int32_t ret;

    usec = (tv->tv_sec * MICROSECONDS_PER_SECOND) + tv->tv_usec;
    ret = utils_time_us_set(usec);
    if (ret != 0) {
        return -1;
    }

    ret = utils_time_zone_set(tz);
    if (ret != 0) {
        return -1;
    }

    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
