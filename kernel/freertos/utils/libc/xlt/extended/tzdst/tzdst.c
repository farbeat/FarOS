#include "tzdst.h"
#include "tzdst_pri.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"
#include "lock.h"
#ifndef CONFIG_LIBC_AARCH64
#include "time64.h"
#endif
#include "securec.h"
#include "libc_portable.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SECS_PER_MIN 60
#define MINS_PER_HOUR 60
#define HOURS_PER_DAY 24
#define DAYS_PER_WEEK 7
#define SECS_PER_HOUR (SECS_PER_MIN * MINS_PER_HOUR)
#define SECS_PER_DAY ((int_fast32_t) SECS_PER_HOUR * HOURS_PER_DAY)
#define MONS_PER_YEAR 12

#define TM_YEAR_BASE 1900
#define isleap(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))

/* 2: leap year or normal year */
static const int32_t g_mon_lens[2][MONS_PER_YEAR] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

/* Time Zone functions */
#define IS_NUM(x) (((x) >= '0') && ((x) <= '9'))

/* used in localtime_r.c */
long timezone;

static void buffer_insert(char *buf, size_t len, size_t positions, char data)
{
    if (len <= positions) {
        return;
    }
    if (memmove_s(&buf[positions + 1], len - positions - 1, &buf[positions], len - positions - 1) != EOK) {
        PRINT_ERR("%s falied \n", __FUNCTION__);
        return;
    }

    buf[positions] = data;
}

#define OPERATE_OFF     3
#define HOUR_HIGH_OFF   4
#define HOUR_LOW_OFF    5
#define MIN_HIGH_OFF    7
#define MIN_LOW_OFF     8
#define SEC_HIGH_OFF    10
#define SEC_LOW_OFF     11

/*
 * tzn[+/-]hh[:mm[:ss]][dzn]
 * tzn  +  11 :30 : 7   dzn
 * tzn  -  9  : 7 :11   dzn
 */
static bool timezone_format(char *stdstr, size_t len)
{
    if ((stdstr[OPERATE_OFF] == '-') || (stdstr[OPERATE_OFF] == '+')) {
        if (!IS_NUM(stdstr[OPERATE_OFF + 1])) {
            return false;
        }
    } else if (IS_NUM(stdstr[OPERATE_OFF])) {
        buffer_insert(stdstr, len, OPERATE_OFF, '+'); /* no operate is default to add */
    } else {
        return false;
    }

    if (!IS_NUM(stdstr[HOUR_LOW_OFF])) {
        buffer_insert(stdstr, len, HOUR_HIGH_OFF, '0'); /* hour only one bit, padding 0 to high bit */
    }

    if (stdstr[HOUR_LOW_OFF + 1] == ':') {
        if (!IS_NUM(stdstr[MIN_HIGH_OFF])) {
            return false;
        } else if (!IS_NUM(stdstr[MIN_LOW_OFF])) {
            buffer_insert(stdstr, len, MIN_HIGH_OFF, '0'); /* minute only one bit, padding 0 to high bit */
        }
    } else {
        /* no minute bits, default is 0 */
        buffer_insert(stdstr, len, HOUR_LOW_OFF + 1, ':');
        buffer_insert(stdstr, len, MIN_HIGH_OFF, '0');
        buffer_insert(stdstr, len, MIN_LOW_OFF, '0');
    }

    if (stdstr[MIN_LOW_OFF + 1] == ':') {
        if (!IS_NUM(stdstr[SEC_HIGH_OFF])) {
            return false;
        } else if (!IS_NUM(stdstr[SEC_LOW_OFF])) {
            buffer_insert(stdstr, len, SEC_HIGH_OFF, '0'); /* second only one bit, padding 0 to high bit */
        }
    } else {
        /* no second bits, default is 0 */
        buffer_insert(stdstr, len, MIN_LOW_OFF + 1, ':');
        buffer_insert(stdstr, len, SEC_HIGH_OFF, '0');
        buffer_insert(stdstr, len, SEC_LOW_OFF, '0');
    }
    return true;
}

static inline int32_t string_to_digital(char high, char low)
{
    /* 10: decimal base number */
    return ((high - '0') * 10) + (low - '0');
}

/*
 * tzn[+/-]hh[:mm[:ss]][dzn]
 * tzn  +  11 :30 : 7   dzn
 * tzn  -  9  : 7 :11   dzn
 */
void settimezone(const char *buf)
{
#define STANDARD_TZ_LEN 15
#define MIN_BUF_LEN     (OPERATE_OFF + 1)
    int32_t hour;
    int32_t minute;
    int32_t second;
    size_t len;
    char stdstr[STANDARD_TZ_LEN] = {0};

    if (buf == NULL) {
        goto ERROR;
    }

    len = strlen(buf);
    if (len < MIN_BUF_LEN) {
        goto ERROR;
    }

    (void)memset(stdstr, '#', STANDARD_TZ_LEN);
    if (memcpy_s(stdstr, STANDARD_TZ_LEN, buf, len) != EOK) {
        goto ERROR;
    }

    if (!timezone_format(stdstr, STANDARD_TZ_LEN)) {
        goto ERROR;
    }

    hour = string_to_digital(stdstr[HOUR_HIGH_OFF], stdstr[HOUR_LOW_OFF]);
    minute = string_to_digital(stdstr[MIN_HIGH_OFF], stdstr[MIN_LOW_OFF]);
    second = string_to_digital(stdstr[SEC_HIGH_OFF], stdstr[SEC_LOW_OFF]);
    /* [-12:00:00, +14:00:00] limits */
    if (((minute > 59) || (second > 59)) ||
        ((stdstr[OPERATE_OFF] == '-') && ((hour > 12) || ((hour == 12) && ((minute != 0) || (second != 0))))) ||
        ((stdstr[OPERATE_OFF] == '+') && ((hour > 14) || ((hour == 14) && ((minute != 0) || (second != 0)))))) {
        goto ERROR;
    }

    if (LIBC_LOCK(g_tzdst_lock)) {
        goto ERROR;
    }

    /* 1h: 3600s, 1min: 60s */
    timezone = hour * 3600 + minute * 60 + second;
    if (stdstr[OPERATE_OFF] == '-') {
        timezone = -timezone;
    }

    (void)LIBC_UNLOCK(g_tzdst_lock);

    return;

ERROR:
    PRINT_ERR("TZ file data error\n");
}

/* DST functions */
#define DST_STR_LEN_FORMAT_MDAY 15 /* for example "Feb-03 03:00:00" */
#define DST_STR_LEN_FORMAT_WDAY 20 /* for example "Oct-1st-Fri 02:59:59" */
#define DST_SET_LENGTH_MAX (DST_STR_LEN_FORMAT_WDAY + 1)

#define MONTH_NAME_LEN 3
static const char *g_mon_str[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
                                    "Aug", "Sep", "Oct", "Nov", "Dec" };
static const char *g_mweek_str[] = { "1st", "2nd", "3rd", "4th", "5th" };
static const char *g_wday_str[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

static bool g_is_dst_work = false;
int32_t g_dst_forward_secs = 0;
static char g_dst_str_start[DST_SET_LENGTH_MAX] = {0};
static char g_dst_str_end[DST_SET_LENGTH_MAX] = {0};

int32_t dst_forward_sec_get(void)
{
    return g_dst_forward_secs;
}

static int32_t get_mon_from_str(const char *mon_str)
{
    uint32_t index;
    for (index = 0; index < sizeof(g_mon_str) / sizeof(char *); index++) {
        if (strncmp(mon_str, g_mon_str[index], MONTH_NAME_LEN) == 0) {
            /* A legal month is from 0 to 11. */
            return (int)index;
        }
    }

    return -1;
}

static int32_t get_wday_from_str(const char *wday_str)
{
    uint32_t index;
    for (index = 0; index < sizeof(g_wday_str) / sizeof(char *); index++) {
        if (strncmp(wday_str, g_wday_str[index], MONTH_NAME_LEN) == 0) {
            /* Day of the week (0-6, Sunday = 0) */
            return (int32_t)index;
        }
    }

    return -1;
}

static int32_t get_mweek_from_str(const char *mweek_str)
{
    uint32_t index;
    for (index = 0; index < sizeof(g_mweek_str) / sizeof(char *); index++) {
        if (strncmp(mweek_str, g_mweek_str[index], MONTH_NAME_LEN) == 0) {
            /* Month of the week (1-5) */
            return (int32_t)(index + 1);
        }
    }

    return -1;
}

static inline int32_t validate_time_str(const char *str)
{
    if (((str[0] < '0') || (str[0] > '9')) ||
        ((str[1] < '0') || (str[1] > '9'))) {
        return -1;
    }
    return 0;
}

/* Day of the month 1 ~ 31 */
static int32_t get_mday_from_str(int32_t month, const char *mday_str)
{
    int32_t mday;

    if (validate_time_str(mday_str)) {
        return -1;
    }

    mday = string_to_digital(mday_str[0], mday_str[1]);
    if (mday > 31) {
        return -1;
    }

    /* Not every year have 29 days in Feb when set DST. */
    if ((mday == 29) && ((month + 1) == 2)) {
        return -1;
    }

    if (mday > g_mon_lens[0][month]) {
        return -1;
    }

    /* Day of the month (1-31) */
    return mday;
}

/*
 * time format HH:MM:SS
 * index       01234567
 * 0~23 for hours per day
 * 0~59 for minutes per hour
 * 0~59 for seconds per minute
 */
static int32_t get_dsecs_from_str(const char *time_str)
{
    int32_t hour, min, sec;

    if (validate_time_str(time_str)) {
        return -1;
    }

    if (validate_time_str(time_str + 3)) {
        return -1;
    }

    if (validate_time_str(time_str + 6)) {
        return -1;
    }

    if ((time_str[2] != ':') || (time_str[5] != ':')) {
        return -1;
    }

    hour = string_to_digital(time_str[0], time_str[1]);
    min = string_to_digital(time_str[3], time_str[4]);
    sec = string_to_digital(time_str[6], time_str[7]);
    /* Hours (0-23) */
    if ((hour < 0) || (hour > 23)) {
        return -1;
    }

    /* Minutes (0-59) */
    if ((min < 0) || (min > 59)) {
        return -1;
    }

    /* Seconds (0-59), not consider of the leap seconds in DST. */
    if ((sec < 0) || (sec > 59)) {
        return -1;
    }

    return hour * 3600 + min * 60 + sec;
}

static int32_t dst_get_mday(int32_t year, int32_t month, int32_t mweek, int32_t wday)
{
#define FIRST_DAY 4   /* the first day of 1970.1.1 is Thursday. */
    int32_t first_wday; /* First week day in this month of the specified year. */
    int32_t first_mday_Of_target_wday; /* First target month day in this month of the specified year. */
    int32_t target_mday_of_target_wday; /* The target month day specified by user. */
    struct tm time = {0};
    int64_t seconds, days;

    time.tm_year = year;
    time.tm_mon = month;
    time.tm_mday = 1;
    /* 14: Hour-value range is [0,23] */
    time.tm_hour = 14;
    time.tm_isdst = 0;
#ifdef CONFIG_LIBC_AARCH64
    seconds = mktime(&time);
#else
    seconds = mktime64(&time);
#endif
    if (seconds == -1) {
        return -1;
    }
    days = seconds / SECS_PER_DAY;
    if (days < 0) {
        days = -days;
        first_wday = DAYS_PER_WEEK - (days - (DAYS_PER_WEEK - FIRST_DAY)) % DAYS_PER_WEEK;
    } else {
        if (days > FIRST_DAY) {
            first_wday = 1 + (days - FIRST_DAY) % DAYS_PER_WEEK;
        } else {
            first_wday = FIRST_DAY;
        }
    }

    first_mday_Of_target_wday = 1 + (DAYS_PER_WEEK + wday - first_wday) % DAYS_PER_WEEK;
    /*
     * Users may use 5th weekday to represent the last week of this month
     * for example "Oct-5th-Fri", but there does not exist the 5th Friday in October, so the last monweek is 4th.
     */
    target_mday_of_target_wday = first_mday_Of_target_wday + (mweek - 1) * DAYS_PER_WEEK;
    if (target_mday_of_target_wday > g_mon_lens[(int32_t)isleap(year + TM_YEAR_BASE)][month]) {
        target_mday_of_target_wday -= 7;
    }

    return target_mday_of_target_wday;
}

/*
 * time format decode
 * 1.  Feb-03 03:00:00
 * idx 012345678901234
 * 2.  Oct-1st-Fri 02:59:59
 * idx 12345678901234567890
 */
static int32_t date_decode(int32_t year, const char *dst_str, int32_t *month, int32_t *mday, int32_t *sec)
{
    int32_t mweek, wday;
    /* For example "Feb-03 03:00:00" */
    if (strlen(dst_str) == DST_STR_LEN_FORMAT_MDAY) {
        if ((dst_str[3] != '-') || (dst_str[6] != ' ')) {
            return -1;
        }

        *month = get_mon_from_str(&dst_str[0]);
        if (*month == -1) {
            return -1;
        }

        *mday = get_mday_from_str(*month, &dst_str[4]);
        if (*mday == -1) {
            return -1;
        }

        *sec = get_dsecs_from_str(&dst_str[7]);
        if (*sec == -1) {
            return -1;
        }
    } else if (strlen(dst_str) == DST_STR_LEN_FORMAT_WDAY) {
        /* For example "Oct-1st-Fri 02:59:59" */
        if ((dst_str[3] != '-') || (dst_str[7] != '-') || (dst_str[11] != ' ')) {
            return -1;
        }

        *month = get_mon_from_str(&dst_str[0]);
        if (*month == -1) {
            return -1;
        }

        mweek = get_mweek_from_str(&dst_str[4]);
        if (mweek == -1) {
            return -1;
        }

        wday = get_wday_from_str(&dst_str[8]);
        if (wday == -1) {
            return -1;
        }

        *sec = get_dsecs_from_str(&dst_str[12]);
        if (*sec == -1) {
            return -1;
        }

        *mday = dst_get_mday(year, *month, mweek, wday);
        if (*mday  == -1) {
            return -1;
        }
    } else {
        return -1;
    }

    return 0;
}

static int64_t dst_config_decode(int32_t year, const char *dst_str)
{
    int32_t month, mday, sec;
    int32_t ret;
    struct tm time = {0};
    int64_t dst_sec;

    ret = date_decode(year, dst_str, &month, &mday, &sec);
    if (ret == -1) {
        return -1;
    }
    /* get the DST period */
    time.tm_year = year;
    time.tm_mon = month;
    time.tm_mday = mday;
    time.tm_isdst = 0;
#ifdef CONFIG_LIBC_AARCH64
    dst_sec = mktime(&time);
#else
    dst_sec = mktime64(&time);
#endif
    if (dst_sec == -1) {
        return -1;
    }

    return dst_sec + sec;
}

static bool dst_config_check(const char *dst_str_start, const char *dst_str_end)
{
    int64_t dst_start, dst_end;
    const int32_t year = 70; /* 70 stands for epoch time */

    if ((dst_str_start == NULL) || (dst_str_end == NULL)) {
        return false;
    }

    dst_start = dst_config_decode(year, dst_str_start);
    dst_end = dst_config_decode(year, dst_str_end);
    if ((dst_start < 0) || (dst_end < 0)) {
        return false;
    }

    if (dst_start >= dst_end) {
        return false;
    }

    return true;
}

static bool dst_period_check_inner(const struct tm * const tm, int64_t seconds, int64_t dst_start, int64_t dts_end)
{
    if (tm != NULL) {
        if (tm->tm_isdst < 0) {
            /* it must to be. */
            if ((seconds >= dst_start) && (seconds < dst_start + g_dst_forward_secs)) {
                return false;
            }

            /* determine the time period of the second pass, out of the DST period. */
            if ((seconds > dts_end) && (seconds <= dts_end + g_dst_forward_secs)) {
                return true;
            }
        } else if (tm->tm_isdst > 0) {
            /* the logical judgment here is the opposite of common sense */
            return true;
        } else {
            /* tm->tm_isdst is zero */
            return false;
        }
    }

    if ((seconds < dst_start) || (seconds >= dts_end)) {
        return false; /* daylight saving time is not effect. */
    }

    return true;
}

bool dst_period_check(const struct tm * const tm, int64_t seconds)
{
    int64_t dst_start, dst_end;
    struct tm time = {0};

    if (LIBC_LOCK(g_tzdst_lock)) {
        return false;
    }
    if (g_is_dst_work == false) {
        (void)LIBC_UNLOCK(g_tzdst_lock);
        return false;
    }

    (void)LIBC_UNLOCK(g_tzdst_lock);
    /* represent a local time. */
    if (tm != NULL) {
        (void)memcpy(&time, tm, sizeof(struct tm));
        time.tm_isdst = 0;
        /* the input-param of seconds is unused in this case. */
#ifdef CONFIG_LIBC_AARCH64
        seconds = mktime(&time);
#else
        seconds = mktime64(&time);
#endif
        if (seconds == -1) {
            return false;
        }
    } else {
        /* represent a standard time, not care TZ. */
#ifdef CONFIG_LIBC_AARCH64
        if (gmtime_r(&seconds, &time) == NULL) {
#else
        if (gmtime64_r(&seconds, &time) == NULL) {
#endif
            return false;
        }
    }

    dst_start = dst_config_decode(time.tm_year, g_dst_str_start);
    dst_end = dst_config_decode(time.tm_year, g_dst_str_end);
    if ((dst_start == -1) || (dst_end == -1)) {
        return false;
    }

    return dst_period_check_inner(tm, seconds, dst_start, dst_end);
}

int dst_disable(void)
{
    if (LIBC_LOCK(g_tzdst_lock)) {
        return -1;
    }

    g_is_dst_work = false;

    (void)LIBC_UNLOCK(g_tzdst_lock);

    return 0;
}

int dst_enable(const char *dst_start_time, const char *dst_end_tme, int forward_seconds)
{
    if (LIBC_LOCK(g_tzdst_lock)) {
        return -1;
    }

    /* Check if the format of dst config is correct. */
    if (dst_config_check(dst_start_time, dst_end_tme) != true) {
        (void)LIBC_UNLOCK(g_tzdst_lock);
        return -1;
    }

    if ((forward_seconds < 0) || (forward_seconds >= 24 * 3600)) { /* seconds per day 24 * 3600 */
        (void)LIBC_UNLOCK(g_tzdst_lock);
        return -1;
    }

    g_is_dst_work = false;
    if (strncpy_s(g_dst_str_start, DST_SET_LENGTH_MAX, dst_start_time, strlen(dst_start_time)) != EOK) {
        (void)LIBC_UNLOCK(g_tzdst_lock);
        return -1;
    }
    if (strncpy_s(g_dst_str_end, DST_SET_LENGTH_MAX, dst_end_tme, strlen(dst_end_tme)) != EOK) {
        (void)LIBC_UNLOCK(g_tzdst_lock);
        return -1;
    }

    g_dst_forward_secs = forward_seconds;
    g_is_dst_work = true;

    (void)LIBC_UNLOCK(g_tzdst_lock);

    return 0;
}

int dst_inquire(int year, struct tm *start, struct tm *end)
{
    int64_t dst_start, dst_end;

    if (LIBC_LOCK(g_tzdst_lock)) {
        return -1;
    }

    if (!g_is_dst_work) {
        (void)LIBC_UNLOCK(g_tzdst_lock);
        return -1;
    }

    if ((start == NULL) || (end == NULL)) {
        (void)LIBC_UNLOCK(g_tzdst_lock);
        return -1;
    }

    dst_start = dst_config_decode(year, g_dst_str_start);
    dst_end = dst_config_decode(year, g_dst_str_end);
    if ((dst_start == -1) || (dst_end == -1)) {
        (void)LIBC_UNLOCK(g_tzdst_lock);
        return -1;
    }

    dst_start += timezone;
    dst_end += timezone;
#ifdef CONFIG_LIBC_AARCH64
    if ((gmtime_r(&dst_start, start) == NULL) || (gmtime_r(&dst_end, end) == NULL)) {
#else
    if ((gmtime64_r(&dst_start, start) == NULL) || (gmtime64_r(&dst_end, end) == NULL)) {
#endif
        (void)LIBC_UNLOCK(g_tzdst_lock);
        return -1;
    }

    (void)LIBC_UNLOCK(g_tzdst_lock);
    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
