#ifndef _SYS_TIME_RTC_H
#define _SYS_TIME_RTC_H

#include "sys/time.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t enable;
    uint64_t (*time_us_set)(uint64_t time);
    uint64_t (*time_us_get)(void);
    int32_t (*time_zone_set)(const struct timezone *tz);
    int32_t (*time_zone_get)(struct timezone *tz);
} xlt_time_rtc_ops;

const xlt_time_rtc_ops *xlt_time_rtc_ops_get(void);
int32_t xlt_time_rtc_ops_set(const xlt_time_rtc_ops *ops);

#ifdef __cplusplus
}
#endif

#endif /* END _SYS_TIME_RTC_H */
