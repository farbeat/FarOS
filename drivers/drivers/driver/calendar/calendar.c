/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides calendar driver source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-20， Create file. \n
 */
#include <stdio.h>
#include "hal_calendar.h"
#include "calendar_porting.h"
#include "calendar.h"

#define EXT_HAL_MS_SECS_MUL     1000
#define EXT_DAY_OF_NORMAL       365
#define EXT_NUM_OF_MIN_YEAR     1970
#define EXT_NUM_OF_MAX_YEAR     2099
#define EXT_MAX_MON_OF_YEAR     12
#define EXT_MIN_MON_OF_YEAR     1
#define EXT_MAX_HOUR_OF_DAY     23
#define EXT_MAX_MIN_OF_HOUR     59
#define EXT_MAX_SEC_OF_MIN      59

/* Get the number of days (seconds of a day). 86400 on day. */
#define SEC_OF_DAY (24 * 60 * 60)

/* hour. 3600 sec. */
#define SEC_OF_HOUR (60 * 60)


/* Number of days in a specified month. Index 1 for leap year, else 0. */
const uint8_t g_month[2][12] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

static uint32_t is_leap_year(uint32_t year)
{
    /* 4, 100, 400: leap year can be divided by 4 and cannot be divided by 100, or it can be divided by 400 */
    return (uint32_t)(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

static uint32_t leaps_thru_end_of(uint32_t year)
{
    /* 4, 100, 400: leap year can be divided by 4 and cannot be divided by 100, or it can be divided by 400 */
    return ((year) / 4 - (year) / 100 + (year) / 400);
}
static errcode_t check_datetime_day_legality(const calendar_t *date)
{
    /* Make sure time is valid. */
    if ((date->sec > EXT_MAX_SEC_OF_MIN) || (date->min > EXT_MAX_MIN_OF_HOUR) || (date->hour > EXT_MAX_HOUR_OF_DAY)) {
        return ERRCODE_CALENDAR_INVALID_PARAMETER;
    }

    /* Make sure month is valid. */
    if ((date->mon > EXT_MAX_MON_OF_YEAR) || (date->mon < EXT_MIN_MON_OF_YEAR)) {
        return ERRCODE_CALENDAR_INVALID_PARAMETER;
    }

    /* Make sure days in month are not more than it should be. */
    if (date->day > g_month[is_leap_year(date->year)][date->mon - 1]) {
        return ERRCODE_CALENDAR_INVALID_PARAMETER;
    }

    /* Make sure year is not earlier than 1970 and before 2100. */
    if ((date->year < EXT_NUM_OF_MIN_YEAR) || (date->year > EXT_NUM_OF_MAX_YEAR)) {
        return ERRCODE_CALENDAR_INVALID_PARAMETER;
    } else {
        return ERRCODE_SUCC;
    }
}

static int32_t month_to_days(uint32_t month, uint32_t year)
{
    return (int32_t)g_month[is_leap_year(year)][month];
}

static void calendar_timestamp_to_date(uint64_t time, calendar_t *date)
{
    uint32_t month, year;
    int32_t days;
    uint64_t timestamp = time;

    days = (int32_t)(timestamp / SEC_OF_DAY);
    timestamp -= (uint32_t) days * SEC_OF_DAY;

    year = EXT_NUM_OF_MIN_YEAR + days / EXT_DAY_OF_NORMAL;
    days -= (int32_t)((year - EXT_NUM_OF_MIN_YEAR) * EXT_DAY_OF_NORMAL + leaps_thru_end_of(year - 1) -
        leaps_thru_end_of(EXT_NUM_OF_MIN_YEAR - 1));
    if (days < 0) {
        year = year - 1;
        days += (int32_t)(EXT_DAY_OF_NORMAL + is_leap_year(year));
    }
    date->year = year;

    for (month = 0; month < EXT_MAX_MON_OF_YEAR - 1; month++) {
        int32_t new_days = days - month_to_days(month, year);
        if (new_days < 0) {
            break;
        }
        days = new_days;
    }
    date->mon = month + 1; /* Get Month */
    date->day = (uint32_t)(days + 1); /* Get Date */
    date->hour = (uint32_t)(timestamp / SEC_OF_HOUR);
    timestamp -= date->hour * SEC_OF_HOUR;
    date->min = (uint32_t)(timestamp / 60); /* Get Minutes, 60 sec. */
    date->sec = (uint32_t)(timestamp - date->min * 60); /* Get Seconds, 60 sec. */
}

static uint64_t calendar_date_to_timestamp(const calendar_t *date)
{
    int32_t mon = (int32_t)date->mon;
    uint32_t year = date->year;

    /* Puts Feb last since it has leap day */
    if ((mon -= 2) <= 0) { /* 2 : self-decrease 2 */
        mon += 12; /* 12 : self-increasing 12 */
        year -= 1;
    }

    uint64_t secs = (((
        /* Calculate the difference between the year and 1970. */
        (uint64_t)(year / 4 - year / 100 + year / 400 + 367 * mon / 12 + date->day) +
        year * EXT_DAY_OF_NORMAL - 719499) * /* base on 1970, minus days bofore 1970 */
        24 + /* day to hours, 24 hour */
        date->hour) *
        60 + /* hour to min, 60 min */
        date->min) *
        60 + /* min to sec, 60 sec */
        date->sec;
    return secs;
}

errcode_t uapi_calendar_init(void)
{
    return calendar_port_register_hal_funcs();
}

errcode_t uapi_calendar_deinit(void)
{
    (void)uapi_calendar_set_timestamp(0);
    return calendar_port_unregister_hal_funcs();
}

errcode_t uapi_calendar_set_datetime(const calendar_t *date)
{
    if (date == NULL) {
        return ERRCODE_CALENDAR_INVALID_PARAMETER;
    }

    if (check_datetime_day_legality(date) != ERRCODE_SUCC) {
        return ERRCODE_CALENDAR_INVALID_PARAMETER;
    }

    uint64_t timestamp = calendar_date_to_timestamp(date);
    hal_calendar_funcs_t *hal_funcs = hal_calendar_get_funcs();
    if (hal_funcs == NULL) {
        return ERRCODE_CALENDAR_FAILURE;
    }

    return hal_funcs->set_timestamp(timestamp * EXT_HAL_MS_SECS_MUL);
}

errcode_t uapi_calendar_get_datetime(calendar_t *date)
{
    if (date == NULL) {
        return ERRCODE_CALENDAR_INVALID_PARAMETER;
    }

    hal_calendar_funcs_t *hal_funcs = hal_calendar_get_funcs();
    if (hal_funcs == NULL) {
        return ERRCODE_CALENDAR_FAILURE;
    }

    uint64_t timestamp = hal_funcs->get_timestamp();
    calendar_timestamp_to_date(timestamp / EXT_HAL_MS_SECS_MUL, date);

    if (check_datetime_day_legality(date) != ERRCODE_SUCC) {
        calendar_timestamp_to_date(0, date);
        return ERRCODE_CALENDAR_GET_DATETIME_FAILURE;
    }
    return ERRCODE_SUCC;
}

errcode_t uapi_calendar_set_timestamp(const uint64_t timestamp)
{
    calendar_t date;
    calendar_timestamp_to_date(timestamp / EXT_HAL_MS_SECS_MUL, &date);
    if (check_datetime_day_legality(&date) != ERRCODE_SUCC) {
        return ERRCODE_CALENDAR_INVALID_PARAMETER;
    }

    hal_calendar_funcs_t *hal_funcs = hal_calendar_get_funcs();
    if (hal_funcs == NULL) {
        return ERRCODE_CALENDAR_FAILURE;
    }

    return hal_funcs->set_timestamp(timestamp);
}

errcode_t uapi_calendar_get_timestamp(uint64_t *timestamp)
{
    hal_calendar_funcs_t *hal_funcs = hal_calendar_get_funcs();
    if (hal_funcs == NULL) {
        return ERRCODE_CALENDAR_FAILURE;
    }

    uint64_t time = hal_funcs->get_timestamp();

    calendar_t date;
    calendar_timestamp_to_date(time / EXT_HAL_MS_SECS_MUL, &date);

    if (check_datetime_day_legality(&date) != ERRCODE_SUCC) {
        *timestamp = 0;
        return ERRCODE_CALENDAR_GET_TIMESTAMP_FAILURE;
    }

    *timestamp = time;
    return ERRCODE_SUCC;
}