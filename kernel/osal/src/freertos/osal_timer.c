/*
* Copyright (c) CompanyNameMagicTag 2019-2022. All rights reserved.
* Description:osal_timer
* Author: AuthorNameMagicTag
* Create: 2019-10-11
*/
#include <FreeRTOS.h>
#include <timers.h>
#include <time.h>
#include <hwi.h>
#include "hal_freertos_timer.h"
#include "soc_osal.h"
#include "osal_inner.h"

#define US_TO_NSEC  1000
#define NS_PER_S    (1000000000LL)
#define NS_PER_MS   (1000000LL)

unsigned long osal_timer_get_private_data(const void *sys_data)
{
    if (sys_data == NULL) {
        return 0;
    }
    return (unsigned long)pcTimerGetTimerName((TimerHandle_t)sys_data);
}

int osal_timer_init(osal_timer *timer)
{
    if (timer == NULL || timer->timer != NULL || timer->handler == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    if (pdMS_TO_TICKS(timer->interval) <= 0) {
        osal_log("interval:%d invalid!\n", timer->interval);
        return OSAL_FAILURE;
    }
    TimerHandle_t xtimer = xTimerCreate((const char * const)timer->data, pdMS_TO_TICKS(timer->interval), pdFALSE,
        0, (TimerCallbackFunction_t)timer->handler);
    if (xtimer == NULL) {
        osal_log("xTimerCreate fail ms = %d!\n", timer->interval);
        return OSAL_FAILURE;
    }
    timer->timer = xtimer;
    return OSAL_SUCCESS;
}

/*
 * timer will restart after modify.
 */
int osal_timer_mod(osal_timer *timer, unsigned int interval)
{
    unsigned int ret;

    if (timer == NULL || timer->timer == NULL || pdMS_TO_TICKS(interval) <= 0) {
        osal_log("interval:%d parameter invalid!\n", interval);
        return OSAL_FAILURE;
    }
    ret = xTimerChangePeriod(timer->timer, pdMS_TO_TICKS(interval), 0);
    if (ret != pdPASS) {
        osal_log("xTimerChangePeriod failed ret= %d! interval = %d\n", ret, interval);
        return OSAL_FAILURE;
    }

    return OSAL_SUCCESS;
}

/*
 * if the timer is timing, this function will restart the timer
 */
int osal_timer_start(osal_timer *timer)
{
    if (timer == NULL || timer->timer == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    int ret = xTimerStart(timer->timer, 0);
    if (ret != pdPASS) {
        osal_log("xTimerStart failed! ret = %d\n", ret);
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

int osal_timer_stop(osal_timer *timer)
{
    if (timer == NULL || timer->timer == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    int ret = xTimerStop(timer->timer, 0);
    if (ret != pdPASS) {
        osal_log("xTimerStop failed! ret = %d.\n", ret);
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

int osal_timer_destroy(osal_timer *timer)
{
    if (timer == NULL || timer->timer == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    int ret = xTimerDelete(timer->timer, 0);
    if (ret != pdPASS) {
        osal_log("xTimerDelete failed! ret = %d.\n", ret);
        return OSAL_FAILURE;
    }
    timer->timer = NULL;
    return OSAL_SUCCESS;
}

void osal_gettimeofday(osal_timeval *tv)
{
    if (tv == NULL) {
        return;
    }

    struct timespec tp;
    if (clock_gettime(CLOCK_MONOTONIC, &tp) == 0) {
        tv->tv_usec = (unsigned int)(tp.tv_nsec / US_TO_NSEC);
        tv->tv_sec = tp.tv_sec;
    } else {
        osal_log("clock_gettime failed!\n");
    }
}

unsigned long long osal_get_jiffies(void)
{
    if (xHwiGetInterruptId() == configHWI_INVALID_IRQ_ID) {
        return (unsigned long long)xTaskGetTickCount();
    }

    return (unsigned long long)xTaskGetTickCountFromISR();
}

unsigned long osal_msecs_to_jiffies(const unsigned int m)
{
    return (unsigned long)pdMS_TO_TICKS(m);
}

unsigned int osal_jiffies_to_msecs(const unsigned int n)
{
    return (unsigned int)(((uint64_t)n * (TickType_t)1000U) / (TickType_t)configTICK_RATE_HZ);
}

unsigned long long osal_sched_clock(void)
{
    return ullTickGetCycles() * (NS_PER_S / NS_PER_MS) / (configTICK_CLOCK_HZ / NS_PER_MS);
}