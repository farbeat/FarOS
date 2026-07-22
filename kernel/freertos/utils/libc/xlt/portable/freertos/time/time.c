#include "time_pri.h"
#include "limits.h"
#include "sys/times.h"
#ifndef CONFIG_LIBC_AARCH64
#include "time64.h"
#endif
#include "errno.h"
#include "unistd.h"
#include "stdio.h"
#include "sys/time.h"
#include "sys/timertc.h"
#include "stdint.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * Do a time package defined return. This requires the error code
 * to be placed in errno, and if it is non-zero, -1 returned as the
 * result of the function. This also gives us a place to put any
 * generic tidyup handling needed for things like signal delivery and
 * cancellation.
 */
#define time_return(err) do { \
    int32_t retVal = 0;         \
    if ((err) != 0) {         \
        retVal = -1;          \
        errno = (err);        \
    }                         \
    return retVal;            \
} while (0)

#ifdef CONFIG_LIBC_AARCH64
/*
 * This two structures originally did't exit,
 * they added by liteos to support 64bit interfaces on 32bit platform,
 * in 64bit platform, timeval64 define to timeval which is platform adaptive.
 */
#ifndef timeval64
#define timeval64 timeval
#endif
#ifndef timespec64
#define timespec64 timespec
#endif
#endif

#define TV_SEC_MAX ((INT_MAX / 1000000L) - 2)
#define TV_SEC_MIN ((INT_MIN / 1000000L) + 2)

#define TIME_REMAINDER(number, unit) (((uint64_t)(number) * KERNEL_TICK_PER_SECOND) % (unit))

static inline bool utils_validate_timeval(const struct timeval *tv)
{
    /* Fail a NULL pointer */
    if (tv == NULL) {
        return false;
    }

    /* Fail illegal microseconds values */
    if ((tv->tv_usec < 0) || (tv->tv_usec >= MICROSECONDS_PER_SECOND) || (tv->tv_sec < 0)) {
        return false;
    }

    return true;
}

#ifndef CONFIG_LIBC_AARCH64
static inline bool utils_validate_timeval64(const struct timeval64 *tv)
{
    /* Fail a NULL pointer */
    if (tv == NULL) {
        return false;
    }

    /* Fail illegal microseconds values */
    if ((tv->tv_usec < 0) || (tv->tv_usec >= MICROSECONDS_PER_SECOND) || (tv->tv_sec < 0)) {
        return false;
    }

    return true;
}
#endif

static SPIN_LOCK_INIT(g_time_spin);
static long long g_adjtime_left; /* absolute value of adjtime */
static int32_t g_adj_direction;    /* 1, speed up; 0, slow down; */

/* Adjust pacement, nanoseconds per SCHED_CLOCK_INTETRVAL_TICKS ticks */
#define TIME_ADJ_PACEMENT (((MICROSECONDS_ADJ_PER_SECOND * SCHED_CLOCK_INTETRVAL_TICKS) / KERNEL_TICK_PER_SECOND) * NANOSECONDS_PER_MICROSECONDS)

/* accumulative time delta from continuous modify, such as adjtime */
static struct timespec64 g_acc_delta_from_adj;
/* accumulative time delta from discontinuous modify, such as settimeofday */
static struct timespec64 g_acc_delta_from_set;

void xlt_adjtime(void)
{
    uint32_t int_save;
    utils_spin_lock_save(&g_time_spin, &int_save);
    if (!g_adjtime_left) {
        utils_spin_unlock_restore(&g_time_spin, int_save);
        return;
    }

    if (g_adjtime_left > TIME_ADJ_PACEMENT) {
        if (g_adj_direction) {
            if ((g_acc_delta_from_adj.tv_nsec + TIME_ADJ_PACEMENT) >= NANOSECONDS_PER_SECOND) {
                g_acc_delta_from_adj.tv_sec++;
                g_acc_delta_from_adj.tv_nsec  = (g_acc_delta_from_adj.tv_nsec + TIME_ADJ_PACEMENT) % NANOSECONDS_PER_SECOND;
            } else {
                g_acc_delta_from_adj.tv_nsec  = g_acc_delta_from_adj.tv_nsec + TIME_ADJ_PACEMENT;
            }
        } else {
            if ((g_acc_delta_from_adj.tv_nsec - TIME_ADJ_PACEMENT) < 0) {
                g_acc_delta_from_adj.tv_sec--;
                g_acc_delta_from_adj.tv_nsec  = g_acc_delta_from_adj.tv_nsec - TIME_ADJ_PACEMENT + NANOSECONDS_PER_SECOND;
            } else {
                g_acc_delta_from_adj.tv_nsec  = g_acc_delta_from_adj.tv_nsec - TIME_ADJ_PACEMENT;
            }
        }

        g_adjtime_left -= TIME_ADJ_PACEMENT;
    } else {
        if (g_adj_direction) {
            if ((g_acc_delta_from_adj.tv_nsec + g_adjtime_left) >= NANOSECONDS_PER_SECOND) {
                g_acc_delta_from_adj.tv_sec++;
                g_acc_delta_from_adj.tv_nsec  = (g_acc_delta_from_adj.tv_nsec + g_adjtime_left) % NANOSECONDS_PER_SECOND;
            } else {
                g_acc_delta_from_adj.tv_nsec  = g_acc_delta_from_adj.tv_nsec + g_adjtime_left;
            }
        } else {
            if ((g_acc_delta_from_adj.tv_nsec - g_adjtime_left) < 0) {
                g_acc_delta_from_adj.tv_sec--;
                g_acc_delta_from_adj.tv_nsec  = g_acc_delta_from_adj.tv_nsec - g_adjtime_left + NANOSECONDS_PER_SECOND;
            } else {
                g_acc_delta_from_adj.tv_nsec  = g_acc_delta_from_adj.tv_nsec - g_adjtime_left;
            }
        }

        g_adjtime_left = 0;
    }
    utils_spin_unlock_restore(&g_time_spin, int_save);
    return;
}

/*
 * Function: adjtime
 * Description:  correct the time to synchronize the system clock.
 * Input:     delta - The amount of time by which the clock is to be adjusted.
 * Output: oldDelta - the amount of time remaining from any previous adjustment that has not yet been completed.
 * Return: On success, returns 0.  On failure, -1 is returned, and errno is set to indicate the error.
 */
int adjtime(const struct timeval *delta, struct timeval *olddelta)
{
    uint32_t int_save;
    utils_spin_lock_save(&g_time_spin, &int_save);
    /* return the amount of time remaining from any previous adjustment that has not yet been completed. */
    if (olddelta != NULL) {
        if (g_adj_direction == 1) {
            olddelta->tv_sec = g_adjtime_left / NANOSECONDS_PER_SECOND;
            olddelta->tv_usec = (g_adjtime_left % NANOSECONDS_PER_SECOND) / NANOSECONDS_PER_MICROSECONDS;
        } else {
            olddelta->tv_sec = -(g_adjtime_left / NANOSECONDS_PER_SECOND);
            olddelta->tv_usec = -((g_adjtime_left % NANOSECONDS_PER_SECOND) / NANOSECONDS_PER_MICROSECONDS);
        }
    }

    if ((delta == NULL) || ((delta->tv_sec == 0) && (delta->tv_usec == 0))) {
        utils_spin_unlock_restore(&g_time_spin, int_save);
        return 0;
    }

    if ((delta->tv_usec > MICROSECONDS_PER_SECOND) || (delta->tv_usec < -MICROSECONDS_PER_SECOND)) {
        utils_spin_unlock_restore(&g_time_spin, int_save);
        time_return(EINVAL);
    }

    if ((delta->tv_sec < TV_SEC_MIN) ||
        (delta->tv_sec > TV_SEC_MAX)) {
        utils_spin_unlock_restore(&g_time_spin, int_save);
        time_return(EINVAL);
    }

    g_adjtime_left = ((int64_t)delta->tv_sec * NANOSECONDS_PER_SECOND) + (delta->tv_usec * NANOSECONDS_PER_MICROSECONDS);
    if (g_adjtime_left > 0) {
        g_adj_direction = 1;
    } else {
        g_adj_direction = 0;
        g_adjtime_left = -g_adjtime_left;
    }

    utils_spin_unlock_restore(&g_time_spin, int_save);
    return 0;
}

static inline struct timespec64 utils_timespec_add(const struct timespec64 t1, const struct timespec64 t2)
{
    struct timespec64 ret = {0};

    ret.tv_sec = t1.tv_sec + t2.tv_sec;
    ret.tv_nsec = t1.tv_nsec + t2.tv_nsec;
    if (ret.tv_nsec >= NANOSECONDS_PER_SECOND) {
        ret.tv_sec += 1;
        ret.tv_nsec -= NANOSECONDS_PER_SECOND;
    } else if (ret.tv_nsec < 0L) {
        ret.tv_sec -= 1;
        ret.tv_nsec += NANOSECONDS_PER_SECOND;
    }

    return ret;
}

static inline struct timespec64 utils_timespec_sub(const struct timespec64 t1, const struct timespec64 t2)
{
    struct timespec64 ret = {0};

    ret.tv_sec = t1.tv_sec - t2.tv_sec;
    ret.tv_nsec = t1.tv_nsec - t2.tv_nsec;
    if (ret.tv_nsec < 0) {
        ret.tv_sec -= 1;
        ret.tv_nsec += NANOSECONDS_PER_SECOND;
    }

    return ret;
}

static int32_t utils_settimeofday(const struct timeval64 *tv, const struct timezone *tz)
{
    uint64_t nanosec;
    uint32_t int_save;
    struct timespec64 set_time = {0};
    struct timespec64 hw_time = {0};
    struct timespec64 real_time = {0};
    struct timespec64 tmp = {0};

    if (tz != NULL) {
        if ((tz->tz_minuteswest >= TIME_ZONE_MIN) &&
            (tz->tz_minuteswest <= TIME_ZONE_MAX)) {
            timezone = tz->tz_minuteswest * SECSPERMIN;
        } else {
            time_return(EINVAL);
        }
    }

    nanosec = utils_curr_nanosec();
    set_time.tv_sec = tv->tv_sec;
    set_time.tv_nsec = tv->tv_usec * NANOSECONDS_PER_MICROSECONDS;
    utils_nanoseconds2timespec(nanosec, &hw_time);

    utils_spin_lock_save(&g_time_spin, &int_save);
    /* stop on-going continuous adjusement */
    if (g_adjtime_left) {
        g_adjtime_left = 0;
    }
    real_time = utils_timespec_add(hw_time, g_acc_delta_from_adj);
    real_time = utils_timespec_add(real_time, g_acc_delta_from_set);

    tmp = utils_timespec_sub(set_time, real_time);
    g_acc_delta_from_set = utils_timespec_add(g_acc_delta_from_set, tmp);

    utils_spin_unlock_restore(&g_time_spin, int_save);

    return 0;
}

int settimeofday(const struct timeval *tv, const struct timezone *tz)
{
    struct timeval64 tv64 = {0};
    const xlt_time_rtc_ops *rtc_ops = xlt_time_rtc_ops_get();

    if (!utils_validate_timeval(tv)) {
        time_return(EINVAL);
    }

    tv64.tv_sec = tv->tv_sec;
    tv64.tv_usec = tv->tv_usec;

    if (rtc_ops->enable) {
        (void)rtc_settimeofday(&tv64, tz);
    }

    return utils_settimeofday(&tv64, tz);
}

#ifndef CONFIG_LIBC_AARCH64
int settimeofday64(const struct timeval64 *tv, const struct timezone *tz)
{
    if (!utils_validate_timeval64(tv)) {
        time_return(EINVAL);
    }

    return utils_settimeofday(tv, tz);
}
#endif

int setlocalseconds(int seconds)
{
    struct timeval tv = {0};

    tv.tv_sec = seconds;
    tv.tv_usec = 0;

    return settimeofday(&tv, NULL);
}

static int32_t utils_gettimeofday(struct timeval64 *tv, struct timezone *tz)
{
    uint64_t nanosec;
    uint32_t int_save;

    struct timespec64 hw_time = {0};
    struct timespec64 real_time = {0};

    nanosec = utils_curr_nanosec();
    utils_nanoseconds2timespec(nanosec, &hw_time);

    utils_spin_lock_save(&g_time_spin, &int_save);
    real_time = utils_timespec_add(hw_time, g_acc_delta_from_adj);
    real_time = utils_timespec_add(real_time, g_acc_delta_from_set);
    utils_spin_unlock_restore(&g_time_spin, int_save);

    tv->tv_sec = real_time.tv_sec;
    tv->tv_usec = real_time.tv_nsec / NANOSECONDS_PER_MICROSECONDS;

    if (tv->tv_sec < 0) {
        time_return(EINVAL);
    }

    return 0;
}

#ifndef CONFIG_LIBC_AARCH64
int gettimeofday64(struct timeval64 *tv, struct timezone *tz)
{
    if (tv == NULL) {
        time_return(EINVAL);
    }

    return utils_gettimeofday(tv, tz);
}
#endif

int gettimeofday(struct timeval *tv, void *tz)
{
    struct timeval64 tv64 = {0};
    const xlt_time_rtc_ops *rtc_ops = xlt_time_rtc_ops_get();
    int ret;

    if (tv == NULL) {
        time_return(EINVAL);
    }

    if (rtc_ops->enable) {
        ret = rtc_gettimeofday(&tv64, tz);
    } else {
        ret = utils_gettimeofday(&tv64, tz);
    }

    if (ret != 0) {
        return -1;
    }

#ifdef CONFIG_LIBC_AARCH64
    tv->tv_sec = tv64.tv_sec;
    tv->tv_usec = tv64.tv_usec;
#else
    if (tv64.tv_sec > (long long)LONG_MAX) {
        return -1;
    }
    tv->tv_sec = (time_t)tv64.tv_sec;
    tv->tv_usec = (suseconds_t)tv64.tv_usec;
#endif

    return 0;
}

/*-----------------------------------------------------------*/

int clock_settime(clockid_t clk_id, const struct timespec * tp)
{
    struct timeval tv = {0};

    if (clk_id != CLOCK_REALTIME) {
        time_return(EINVAL);
    }

    if (!utils_validate_timespec(tp)) {
        time_return(EINVAL);
    }

    tv.tv_sec = tp->tv_sec;
    tv.tv_usec = tp->tv_nsec / NANOSECONDS_PER_MICROSECONDS;
    return settimeofday(&tv, NULL);
}


/*-----------------------------------------------------------*/

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
    uint32_t int_save;
    uint64_t nanosec;
    struct timespec64 tmp = {0};
    struct timespec64 hw_time = {0};

    if ((clk_id > MAX_CLOCKS) || (clk_id < CLOCK_REALTIME)) {
        goto err_out;
    }

    if (tp == NULL) {
        goto err_out;
    }

    nanosec = utils_curr_nanosec();
    utils_nanoseconds2timespec(nanosec, &hw_time);

    switch (clk_id) {
        case CLOCK_MONOTONIC_RAW:
            tp->tv_sec = (time_t)hw_time.tv_sec;
            tp->tv_nsec = (long)hw_time.tv_nsec;
            break;
        case CLOCK_MONOTONIC:
            utils_spin_lock_save(&g_time_spin, &int_save);
            tmp = utils_timespec_add(hw_time, g_acc_delta_from_adj);
            utils_spin_unlock_restore(&g_time_spin, int_save);
            tp->tv_sec = (time_t)tmp.tv_sec;
            tp->tv_nsec = (long)tmp.tv_nsec;
            break;
        case CLOCK_REALTIME:
            utils_spin_lock_save(&g_time_spin, &int_save);
            tmp = utils_timespec_add(hw_time, g_acc_delta_from_adj);
            tmp = utils_timespec_add(tmp, g_acc_delta_from_set);
            utils_spin_unlock_restore(&g_time_spin, int_save);
            tp->tv_sec = (time_t)tmp.tv_sec;
            tp->tv_nsec = (long)tmp.tv_nsec;
            break;
        default:
            goto err_out;
    }

    return 0;

err_out:
        time_return(EINVAL);
}


/*-----------------------------------------------------------*/

int clock_getres(clockid_t clk_id, struct timespec *res)
{
    /* Silence warnings about unused parameters. */
    (void)clk_id;

    /* Convert FreeRTOS tick resolution as timespec. */
    if (res != NULL) {
        res->tv_sec = 0;
        res->tv_nsec = NANOSECONDS_PER_TICK;
    }

    return 0;
}

/*-----------------------------------------------------------*/

int usleep(useconds_t usec)
{
    /* To avoid delaying for less than usec, always round up. */
    vTaskDelay(pdMS_TO_TICKS(usec / MICROSECONDS_PER_MILLISECOND + (usec % MICROSECONDS_PER_MILLISECOND != 0)));

    return 0;
}

/*-----------------------------------------------------------*/

int nanosleep(const struct timespec *req, struct timespec *rem)
{
    int status = 0;
    TickType_t sleep_time = 0;

    /* Silence warnings about unused parameters. */
    (void)rem;

    /* Check rqtp. */
    if (utils_validate_timespec(req) == false) {
        errno = EINVAL;
        status = -1;
    }

    if (status == 0) {
        /* Convert rqtp to ticks and delay. */
        if (utils_timespec2ticks(req, &sleep_time) == 0) {
            vTaskDelay(sleep_time);
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

unsigned sleep(unsigned seconds)
{
    vTaskDelay(pdMS_TO_TICKS(seconds * MILLISECONDS_PER_SECOND));

    return 0;
}

/*-----------------------------------------------------------*/

clock_t clock(void)
{
    clock_t clock_millisec;
    uint64_t nanosec;

    nanosec = utils_curr_nanosec();
    clock_millisec = (clock_t)(nanosec / NANOSECONDS_PER_MILLISECOND);

    return clock_millisec;
}

/*-----------------------------------------------------------*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
