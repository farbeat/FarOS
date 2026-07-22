#ifndef _TIME_PRI_H
#define _TIME_PRI_H

#include "time.h"
#include "sys/time.h"
#ifndef CONFIG_LIBC_AARCH64
#include "time64.h"
#endif
#include "limits.h"
#include "stdint.h"
#include "errno.h"
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "hwi.h"
#include "libc_types.h"
#include "hal_freertos_timer.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef CONFIG_LIBC_AARCH64
/*
 * This two structures originally did't exit,
 * they added by liteos to support 64bit interfaces on 32bit platform,
 * in 64bit platform, timeval64 define to timeval which is platform adaptive.
 */
#define timeval64 timeval
#define timespec64 timespec
#endif

#define SECSPERMIN           60     /* second per minute */
#define TIME_ZONE_MAX        840    /* UTC+14:00, the first time zone */
#define TIME_ZONE_MIN        (-720) /* UTC-12:00, the last time zone */

typedef struct {
    size_t lock;
} spin_lock_t;
#define SPIN_LOCK_INIT(lock)  spin_lock_t lock;

/* internal functions */
static inline bool utils_validate_timespec(const struct timespec * const tp)
{
    bool ret = false;

    if (tp != NULL) {
        /* Verify 0 <= tv_nsec < 1000000000. */
        if ((tp->tv_nsec >= 0) && (tp->tv_nsec < NANOSECONDS_PER_SECOND)) {
            ret = true;
        }
    }

    return ret;
}


static inline int utils_timespec2ticks(const struct timespec * const tp,
                           TickType_t * const result)
{
    int status = 0;
    int64_t total_ticks = 0;
    long nanoseconds = 0;

    /* Check parameters. */
    if ((tp == NULL) || (result == NULL)) {
        status = EINVAL;
    } else if ((status == 0) && (utils_validate_timespec(tp) == false)) {
        status = EINVAL;
    }

    if (status == 0) {
        /* Convert timespec.tv_sec to ticks. */
        total_ticks = (int64_t)configTICK_RATE_HZ * (tp->tv_sec);

        /* Convert timespec.tv_nsec to ticks. This value does not have to be checked
         * for overflow because a valid timespec has 0 <= tv_nsec < 1000000000 and
         * NANOSECONDS_PER_TICK > 1. */
        nanoseconds = tp->tv_nsec / (long)NANOSECONDS_PER_TICK +                  /* Whole nanoseconds. */
                      (long)(tp->tv_nsec % (long)NANOSECONDS_PER_TICK != 0); /* Add 1 to round up if needed. */

        /* Add the nanoseconds to the total ticks. */
        total_ticks += (int64_t)nanoseconds;

        /* Check for overflow */
        if (total_ticks < 0) {
            status = EINVAL;
        } else {
            /* check if TickType_t is 32 bit or 64 bit */
            uint32_t tick_type_size = sizeof(TickType_t);

            /* check for downcast overflow */
            if (tick_type_size == sizeof(uint32_t)) {
                if (total_ticks > UINT_MAX) {
                    status = EINVAL;
                }
            }
        }

        /* Write result. */
        *result = (TickType_t)total_ticks;
    }

    return status;
}

static inline void utils_nanoseconds2timespec(int64_t source,
                                  struct timespec64 * const destination)
{
    long carry_sec = 0;

    /* Convert to timespec. */
    destination->tv_sec = (time_t)(source / NANOSECONDS_PER_SECOND);
    destination->tv_nsec = (long)(source % NANOSECONDS_PER_SECOND);

    /* Subtract from tv_sec if tv_nsec < 0. */
    if (destination->tv_nsec < 0L) {
        /* Compute the number of seconds to carry. */
        carry_sec = (destination->tv_nsec / (long)NANOSECONDS_PER_SECOND) + 1L;

        destination->tv_sec -= (time_t)(carry_sec);
        destination->tv_nsec += carry_sec * (long)NANOSECONDS_PER_SECOND;
    }
}

static inline uint64_t utils_curr_nanosec(void)
{
    uint64_t nanosec;

    nanosec = ullTickGetCycles() * (NANOSECONDS_PER_SECOND / NANOSECONDS_PER_MILLISECOND) /
        (configTICK_CLOCK_HZ / NANOSECONDS_PER_MILLISECOND);
    return nanosec;
}

static inline void utils_spin_lock_save(spin_lock_t *lock, uint32_t *int_save)
{
    (void)lock;
    *int_save = uxHwiLock();
}

static inline void utils_spin_unlock_restore(spin_lock_t *lock, uint32_t int_save)
{
    (void)lock;
    vHwiRestore(int_save);
}

extern int32_t rtc_gettimeofday(struct timeval64 *tv, struct timezone *tz);
extern int32_t rtc_settimeofday(const struct timeval64 *tv, const struct timezone *tz);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* _TIME_PRI_H */
