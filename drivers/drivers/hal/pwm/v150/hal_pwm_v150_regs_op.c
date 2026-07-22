/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V150 pwm register operation api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */
#include "common_def.h"
#include "hal_pwm_v150_regs_op.h"

static void hal_pwm_reg_de_set(pwm_channel_t channel, pwm_int_data_t *data, uint32_t value)
{
    switch (channel) {
        case PWM_0:
            data->b.pwm0 = value;
            break;
        case PWM_1:
            data->b.pwm1 = value;
            break;
        case PWM_2:
            data->b.pwm2 = value;
            break;
        case PWM_3:
            data->b.pwm3 = value;
            break;
        case PWM_4:
            data->b.pwm4 = value;
            break;
        case PWM_5:
            data->b.pwm5 = value;
            break;
        default:
            break;
    }
}

void hal_pwm_reg_int_clr_set(pwm_channel_t channel)
{
    pwm_int_data_t clr_data;
    clr_data.d32 = ((pwm_regs_t *)g_pwm_regs)->int_clr;
    hal_pwm_reg_de_set(channel, &clr_data, (uint32_t)1);
    ((pwm_regs_t *)g_pwm_regs)->int_clr = clr_data.d32;
}

void hal_pwm_reg_int_en_set(pwm_channel_t channel, uint32_t value)
{
    pwm_int_data_t en_data;
    en_data.d32 = ((pwm_regs_t *)g_pwm_regs)->int_en;
    hal_pwm_reg_de_set(channel, &en_data, value);
    ((pwm_regs_t *)g_pwm_regs)->int_en = en_data.d32;
}
