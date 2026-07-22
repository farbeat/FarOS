#ifndef __LINUX_TZDST_H__
#define __LINUX_TZDST_H__

#include "time.h"
#include "sys/cdefs.h"

__BEGIN_DECLS

#ifdef __XLT_OS__

/**
 * @ingroup localtime
 * @brief Set Time Zone.
 *
 * @par Description:
 * This API is used to set the time zone.
 *
 * @attention
 * <ul>
 * <li>When setting time zones, format is required to be "tzn[+/-]hh[:mm[:ss]][dzn]" .</li>
 * </ul>
 *
 * @param buff The string point to the time zone going to be setting.
 *
 * @retval   None
 */
void settimezone(const char *);

/**
 * @ingroup localtime
 * @brief Disable daylight saving time.
 *
 * @par Description:
 * This API is used to make daylight saving time useless.
 *
 * @param None.
 *
 * @retval   None
 */
int dst_disable(void);

/**
 * @ingroup localtime
 * @brief Enable daylight saving time..
 *
 * @par Description:
 * This API is used to configure daylight saving time.
 *
 * @attention
 * <ul>
 * <li>When config dst, The configuration format has strict requirements.</li>
 * <li>The first configuration format for example "Feb-03 03:00:00"</li>
 * <li>The second configuration format for example "Oct-1st-Fri 03:00:00"</li>
 * <li>The abbreviations for the months are "Jan",  "Feb", "Mar",  "Apr", "May", "Jun", "Jul", "Aug",
 * "Sep", "Oct", "Nov", and "Dec".</li>
 * <li>The abbreviations for the days of the week are "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", and "Sat".</li>
 * <li>The abbreviations for the weeks of the month are "1st", "2nd", "3rd", "4th", "5th"</li>
 * <li>When it's only four weekdays this month, the effect of "5th" is the same as "Last"</li>
 * </ul>
 *
 * @param dst_start_time   The string point to the DST start going to be setting.
 * @param dst_end_tme      The string point to the DST end going to be setting.
 * @param forward_seconds  Indicates the number of seconds that time is allocated when entering DST interval.
 *
 * @retval   0 Succeed.
 * @retval  -1 Failed.
 */
int dst_enable(const char *dst_start_time, const char *dst_end_tme, int forward_seconds);

/**
 * @ingroup localtime
 * @brief Inquire daylight saving time.
 *
 * @par Description:
 * This API is used to inquire daylight saving time.
 *
 * @param year       Represents the year to query start from 1900.
 * @param dst_start  Used to store daylight savings time start time
 * @param dst_end    Used to store daylight savings time end time
 *
 * @retval   0 Succeed.
 * @retval  -1 Failed.
 */
int dst_inquire(int year, struct tm *dst_start, struct tm *dst_end);

#endif /* __XLT_OS__ */
__END_DECLS

#endif /* __LINUX_TZDST_H__ */
