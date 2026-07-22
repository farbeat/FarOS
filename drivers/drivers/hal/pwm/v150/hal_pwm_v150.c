/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V150 HAL pwm \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */
#include "common_def.h"
#include "hal_pwm.h"
#include "hal_pwm_v150.h"

#define PWM_LEN_HIGH_16BIT_RIGHT_SHIFT 16

static hal_pwm_callback_t g_hal_pwm_callbacks[PWM_MAX_NUMBER] = { NULL };

static errcode_t hal_pwm_v150_init(void)
{
    if (hal_pwm_regs_init() != ERRCODE_SUCC) {
        return ERRCODE_PWM_REG_ADDR_INVALID;
    }

    return ERRCODE_SUCC;
}

static void hal_pwm_v150_deinit(void)
{
    hal_pwm_regs_deinit();
}

static void hal_pwm_v150_set_low_time(hal_pwm_set_time_id_t id, pwm_channel_t channel, uint32_t time)
{
    unused(id);
    uint32_t low_time = 0;
    if (time != 0) {
        low_time = time - 1;
    }
    ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].length0_l = low_time;
    ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].length0_h = low_time >> PWM_LEN_HIGH_16BIT_RIGHT_SHIFT;
}

static void hal_pwm_v150_set_high_time(hal_pwm_set_time_id_t id, pwm_channel_t channel, uint32_t time)
{
    unused(id);
    uint32_t high_time = 0;
    if (time != 0) {
        high_time = time - 1;
    }
    ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].length1_l = high_time;
    ((pwm_regs_t *)g_pwm_regs)->pwm_cfg[channel].length1_h = high_time >> PWM_LEN_HIGH_16BIT_RIGHT_SHIFT;
}

static hal_pwm_set_time_t g_hal_pwm_set_func_array[PWM_SET_TIEM_MAX] = {
    hal_pwm_v150_set_low_time,
    hal_pwm_v150_set_high_time,
};

static void hal_pwm_v150_set_time(hal_pwm_set_time_id_t id, pwm_channel_t channel, uint32_t time)
{
    if (id >= PWM_SET_OFFSET_TIME) {
        return ;
    }
    g_hal_pwm_set_func_array[id](id, channel, time);
}

static void hal_pwm_v150_set_sycles(pwm_channel_t channel, uint16_t cycles)
{
    hal_pwm_reg_cfg1_set_sycles(channel, (uint32_t)cycles);
}

static void hal_pwm_v150_set_action(uint8_t channel, pwm_action_t action)
{
    switch (action) {
        case PWM_ACTION_START:
            hal_pwm_reg_cfg0_set_start(channel);
            hal_pwm_reg_int_en_set(channel, (uint32_t)1);
            break;
        case PWM_ACTION_STOP:
            hal_pwm_reg_cfg0_set_stop(channel);
            hal_pwm_reg_int_en_set(channel, (uint32_t)0);
            break;
        case PWM_ACTION_REFRESH:
            hal_pwm_reg_cfg0_set_refresh(channel);
            break;
        case PWM_ACTION_CONTINUE_SET:
            hal_pwm_reg_cfg1_set_continue(channel, (uint32_t)1);
            break;
        case PWM_ACTION_CONTINUE_CLR:
            hal_pwm_reg_cfg1_set_continue(channel, (uint32_t)0);
            break;
    }
}

static void hal_pwm_v150_intr_clear(pwm_channel_t channel)
{
    hal_pwm_reg_int_clr_set(channel);
}

static void hal_pwm_v150_register_callback(pwm_channel_t channel, hal_pwm_callback_t callback)
{
    g_hal_pwm_callbacks[channel] = callback;
}

static void hal_pwm_v150_unregister_callback(pwm_channel_t channel)
{
    g_hal_pwm_callbacks[channel] = NULL;
}

void hal_pwm_irq_handler(pwm_channel_t channel)
{
    if (g_hal_pwm_callbacks[channel]) {
        g_hal_pwm_callbacks[channel](channel);
    }
}

static hal_pwm_funcs_t g_hal_pwm_v150_funcs = {
    .init = hal_pwm_v150_init,
    .deinit = hal_pwm_v150_deinit,
    .set_time = hal_pwm_v150_set_time,
    .set_sycles = hal_pwm_v150_set_sycles,
    .set_action = hal_pwm_v150_set_action,
    .isrclear = hal_pwm_v150_intr_clear,
    .registerfunc = hal_pwm_v150_register_callback,
    .unregisterfunc = hal_pwm_v150_unregister_callback,
};

hal_pwm_funcs_t *hal_pwm_v150_funcs_get(void)
{
    return &g_hal_pwm_v150_funcs;
}
