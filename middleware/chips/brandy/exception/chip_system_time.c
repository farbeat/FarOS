/**
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: get time
 * Author:
 * Create:
 */
#ifndef BUILD_APPLICATION_SSB

#include "chip_system_time.h"

#define SECONDS_PER_MINUTE      60 /* second per minute */
#define MINUTES_PER_HOUR        60   /* second per minute */
#define MSEC_PER_SEC            1000
#define USEC_PER_MSEC           1000
#define TZ_ADDRESS              0x5702C014
#define MAX_TIMEZONE_HOURS      24
#define TIME_MAX                64

void print_time(calendar_t *date)
{
    printf("CALLENDAR:year = %d, month = %d, day = %d, hour = %d, minute = %d, second = %d \r\n",
        date->year,
        date->mon,
        date->day,
        date->hour,
        date->min,
        date->sec);
}

void print_time_2(struct tm *date)
{
    printf("TM:year = %d, month = %d, day = %d, hour = %d, minute = %d, second = %d \r\n",
        date->tm_year + 1900,
        date->tm_mon,
        date->tm_mday,
        date->tm_hour,
        date->tm_min,
        date->tm_sec);
}

static uint16_t get_timezone(void)
{
    return readw(TZ_ADDRESS) & 0x00FF;
}

static void set_timezone(uint16_t timezone)
{
    uint16_t tz_zone = readw(TZ_ADDRESS) & 0xFF00;
    tz_zone = tz_zone | timezone;
    writew(TZ_ADDRESS, tz_zone);
}

void save_time(void)
{
    uint64_t time_sec = get_local_timestamp();
    time_sec = time_sec * MSEC_PER_SEC;
    uapi_calendar_set_timestamp(time_sec);
}

uint32_t get_current_timestamp(void)
{
    uint32_t timestamp = 0;
    struct timeval current_time = {0};
    if (gettimeofday(&current_time, NULL)) {
        return 0;
    }
    struct tm tm;
    localtime_r(&current_time.tv_sec, &tm);
    timestamp = (uint32_t)current_time.tv_sec;
    return timestamp;
}

uint32_t get_local_timestamp(void)
{
    uint32_t timestamp = get_current_timestamp() + get_timezone() * MINUTES_PER_HOUR * SECONDS_PER_MINUTE;
    return timestamp;
}

void get_current_time(struct timeval *tv, struct tm *tm)
{
    char timebuf[TIME_MAX] = {0};
    if (gettimeofday(tv, NULL)) {
        return;
    }
    localtime_r(&tv->tv_sec, tm);
    tm->tm_mon = tm->tm_mon + 1;
    printf("get_current_time tv_tv_sec = %d \r\n", tv->tv_sec);
    print_time_2(tm);
}

void restore_time(void)
{
    calendar_t date;
    uapi_calendar_get_datetime(&date);
    print_time(&date);
    uint64_t sec;
    uapi_calendar_get_timestamp(&sec);
    printf("callendar timestamp = %lld \r\n", sec);
    sec = sec / MSEC_PER_SEC;
    sync_time(sec, get_timezone());
}

uint32_t sync_time(uint32_t timestamp, uint32_t timezone)
{
    printf("sync time timestamp %d, timezone %d \r\n", timestamp, timezone);
    uint32_t seconds;
    if (timezone > MAX_TIMEZONE_HOURS) {
        return ERRCODE_FAIL;
    }
    if (timestamp == 0) {
        return ERRCODE_FAIL;
    } else if (timestamp <= (timezone * MINUTES_PER_HOUR * SECONDS_PER_MINUTE)) {
        seconds = 0;
    } else {
        seconds = timestamp - (timezone * MINUTES_PER_HOUR * SECONDS_PER_MINUTE);
    }
    set_timezone(timezone);
    struct timeval tv = {0};
    struct timezone tzone = {0};
    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    tzone.tz_dsttime = 0;
    tzone.tz_minuteswest = timezone * MINUTES_PER_HOUR;
    return settimeofday(&tv, &tzone);
}

void test_bandy_time(void)
{
    struct tm tm;
    struct timeval tv;
    uint32_t time = 1716044461;  // 2024-5-18 15:1:1
    printf("--------SYNC TIME----------\r\n");
    get_current_timestamp();
    get_current_time(&tv, &tm);
    sync_time(time, 8);
    get_current_timestamp();
    get_current_time(&tv, &tm);
    printf("--------SAVE TIME----------\r\n");
    get_current_timestamp();
    get_current_time(&tv, &tm);
    save_time();
    get_current_timestamp();
    get_current_time(&tv, &tm);
    printf("--------RESTORE TIME----------\r\n");
    get_current_timestamp();
    get_current_time(&tv, &tm);
    restore_time();
    get_current_timestamp();
    get_current_time(&tv, &tm);
}
#endif