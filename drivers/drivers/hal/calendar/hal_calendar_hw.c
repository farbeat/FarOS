/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides calendar hardware instance \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-03， Create file. \n
 */
#include "systick.h"
#include "button_porting.h"
#include "hal_calendar_hw_op.h"
#include "hal_calendar_hw.h"

#define HAL_CLC_H_OFFSET 32

static inline uint32_t get_high_32bit(uint64_t val)
{
    return (uint32_t)(val >> HAL_CLC_H_OFFSET);
}

static inline uint32_t get_low_32bit(uint64_t val)
{
    return (uint32_t)(val & 0xFFFFFFFFLL);
}

static errcode_t hal_calendar_set_timestamp(uint64_t timestamp)
{
    uint64_t counter = uapi_ulp_systick_get_count();
    hal_calendar_set_cnt_h(get_high_32bit(counter));
    hal_calendar_set_cnt_l(get_low_32bit(counter));

    hal_calendar_set_data_h(get_high_32bit(timestamp));
    hal_calendar_set_data_l(get_low_32bit(timestamp));
    return ERRCODE_SUCC;
}

static uint64_t hal_calendar_get_timestamp(void)
{
    uint64_t data_l, data_h, timestamp, cnt_l, cnt_h, counter, timeout;

    data_l = hal_calendar_get_data_l();
    data_h = hal_calendar_get_data_h();
    cnt_l = hal_calendar_get_cnt_l();
    cnt_h = hal_calendar_get_cnt_h();
    counter = (cnt_h << HAL_CLC_H_OFFSET) | cnt_l;
    timeout = ((uapi_ulp_systick_get_count() - counter) * (MS_PER_S * CALIBRATION_CLOCK_MUL)) /
               uapi_ulp_systick_get_frequency();
    timestamp = ((data_h << HAL_CLC_H_OFFSET) | data_l) + timeout;

    /* get时补偿刷新 */
    hal_calendar_set_timestamp(timestamp);
    return timestamp;
}

static errcode_t hal_calendar_set_alarm(uint64_t timestamp)
{
    hal_calendar_set_alarm_h(get_high_32bit(timestamp));
    hal_calendar_set_alarm_l(get_low_32bit(timestamp));

    return ERRCODE_SUCC;
}

static uint64_t hal_calendar_get_alarm(void)
{
    uint64_t alarm_l, alarm_h;
    alarm_h = hal_calendar_get_alarm_h();
    alarm_l = hal_calendar_get_alarm_l();
    uint64_t timestamp = ((alarm_h << HAL_CLC_H_OFFSET) | alarm_l);
    return timestamp;
}

hal_calendar_funcs_t g_hal_calendar_hw_funcs = {
    .set_alarm = hal_calendar_set_alarm,
    .get_alarm = hal_calendar_get_alarm,
    .set_timestamp = hal_calendar_set_timestamp,
    .get_timestamp = hal_calendar_get_timestamp
};

hal_calendar_funcs_t *hal_calendar_hw_funcs_get(void)
{
    return &g_hal_calendar_hw_funcs;
}
