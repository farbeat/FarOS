/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides calendar port template. \n
 * The Systick module needs to be used for simulation, calendar port file is stored in this path.
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "hal_calendar.h"
#include "hal_calendar_hw.h"
#include "hal_calendar_hw_op.h"
#include "systick.h"
#include "memory_config.h"
#include "calendar_porting.h"

#define ULP_CALIBRATION_INTERVAL 900000
#define VERIFICATIN_TIME 0xFFFFFFFFFFFFFFFFUL
#define SELFHEAL_TIME 500UL
#define CALENDAR_BASE_ADDR ULP_RAM_CALENDAR
uintptr_t g_calendar_base_addr = (uintptr_t)CALENDAR_BASE_ADDR;

uintptr_t hal_calendar_hw_base_addr_get(void)
{
    return g_calendar_base_addr;
}

errcode_t calendar_port_register_hal_funcs(void)
{
    errcode_t ret = hal_calendar_reg_init();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    return hal_calendar_register_funcs(hal_calendar_hw_funcs_get());
}

errcode_t calendar_port_unregister_hal_funcs(void)
{
    hal_calendar_reg_deinit();
    return hal_calendar_unregister_funcs();
}

static errcode_t calendar_set_alarm(const uint64_t timeout)
{
    hal_calendar_funcs_t *hal_funcs = hal_calendar_get_funcs();
    if (hal_funcs == NULL || timeout <= 0) {
        return ERRCODE_CALENDAR_FAILURE;
    }

    uint64_t time = hal_funcs->get_timestamp();
    if (timeout > ULP_CALIBRATION_INTERVAL) {
        uapi_enable_alarm(ULP_CALIBRATION_INTERVAL);
    } else {
        uapi_enable_alarm(timeout);
    }

    return hal_funcs->set_alarm(timeout);
}

static errcode_t calendar_get_alarm(uint64_t *alarm)
{
    hal_calendar_funcs_t *hal_funcs = hal_calendar_get_funcs();
    if (hal_funcs == NULL) {
        return ERRCODE_CALENDAR_FAILURE;
    }

    uint64_t time = hal_funcs->get_alarm();
    *alarm = time;
    return ERRCODE_SUCC;
}

void uapi_calendar_calibration_enable(void)
{
    calendar_set_alarm(VERIFICATIN_TIME);
}

void uapi_self_heal_timer_enable(void)
{
    calendar_set_alarm(SELFHEAL_TIME);
}