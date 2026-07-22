/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides pwm driver source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */

#include "common_def.h"
#include "hal_pwm.h"
#include "pwm.h"

#define PWM_CYCLES_MAX_NUMBER      32767  /* Max number of PWM cycles */

static bool g_pwm_inited = false;
static bool g_pwm_opened[CONFIG_PWM_CHANNEL_NUM] = { false };
static hal_pwm_funcs_t *g_hal_funcs = NULL;

static errcode_t uapi_pwm_check(uint8_t channel)
{
    if (unlikely(channel >= CONFIG_PWM_CHANNEL_NUM)) {
        return ERRCODE_PWM_INVALID_PARAMETER;
    }

    if (unlikely(!g_pwm_inited)) {
        return ERRCODE_PWM_NOT_INIT;
    }

    if (unlikely(!g_pwm_opened[channel])) {
        return ERRCODE_PWM_NOT_OPEN;
    }

    return ERRCODE_SUCC;
}

#if defined(CONFIG_PWM_USING_V151)
static errcode_t uapi_pwm_group_check(uint8_t group)
{
    if (unlikely(group >= CONFIG_PWM_GROUP_NUM)) {
        return ERRCODE_PWM_INVALID_PARAMETER;
    }

    return ERRCODE_SUCC;
}
#endif /* CONFIG_PWM_USING_v151 */

errcode_t uapi_pwm_init(void)
{
    if (g_pwm_inited) {
        return ERRCODE_SUCC;
    }

    pwm_port_register_hal_funcs();
    g_hal_funcs = hal_pwm_get_funcs();
    errcode_t ret = g_hal_funcs->init();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    pwm_port_clock_enable(true);

    g_pwm_inited = true;

    return ret;
}

void uapi_pwm_deinit(void)
{
    if (!g_pwm_inited) {
        return;
    }

    for (uint8_t channel_i = (uint8_t)PWM_0; channel_i < (uint8_t)PWM_MAX_NUMBER; channel_i++) {
        if (g_pwm_opened[channel_i]) {
            (void)uapi_pwm_close(channel_i);
        }
    }

    g_hal_funcs->deinit();

    pwm_port_unregister_hal_funcs();

    pwm_port_clock_enable(false);

    g_pwm_inited = false;
}

errcode_t uapi_pwm_open(uint8_t channel, const pwm_config_t *cfg)
{
    if (unlikely(channel >= CONFIG_PWM_CHANNEL_NUM)) {
        return ERRCODE_PWM_INVALID_PARAMETER;
    }

    if (unlikely(!g_pwm_inited)) {
        return ERRCODE_PWM_NOT_INIT;
    }
    if (unlikely(g_pwm_opened[channel])) {
        (void)uapi_pwm_close(channel);
    }

    if (unlikely(cfg->cycles > PWM_CYCLES_MAX_NUMBER)) {
        return ERRCODE_PWM_INVALID_PARAMETER;
    }

    g_hal_funcs->registerfunc((pwm_channel_t)channel, (hal_pwm_callback_t)uapi_pwm_isr);

    g_hal_funcs->set_time(PWM_SET_LOW_TIME, (pwm_channel_t)channel, cfg->low_time);
    g_hal_funcs->set_time(PWM_SET_HIGH_TIME, (pwm_channel_t)channel, cfg->high_time);

#if defined(CONFIG_PWM_USING_V151)
    g_hal_funcs->set_time(PWM_SET_OFFSET_TIME, (pwm_channel_t)channel, cfg->offset_time);
#endif /* CONFIG_PWM_USING_V151 */

    g_hal_funcs->set_sycles((pwm_channel_t)channel, cfg->cycles);

#if defined(CONFIG_PWM_USING_V150)
    if (cfg->repeat) {
        g_hal_funcs->set_action(channel, PWM_ACTION_CONTINUE_SET);
    }
#endif /* CONFIG_PWM_USING_V150 */

    g_pwm_opened[channel] = true;
    return ERRCODE_SUCC;
}


errcode_t uapi_pwm_close(uint8_t channel)
{
    errcode_t ret = uapi_pwm_check(channel);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    g_hal_funcs->set_action(channel, PWM_ACTION_STOP);
    g_hal_funcs->set_action(channel, PWM_ACTION_CONTINUE_CLR);

    pwm_port_unregister_irq((pwm_channel_t)channel);

    uapi_pwm_unregister_interrupt((pwm_channel_t)channel);

    g_pwm_opened[channel] = false;

    return ret;
}

errcode_t uapi_pwm_start(uint8_t channel)
{
    errcode_t ret = ERRCODE_SUCC;
#if defined(CONFIG_PWM_USING_V150)
    ret = uapi_pwm_check(channel);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
#endif /* CONFIG_PWM_USING_V150 */

#if defined(CONFIG_PWM_USING_V151)
    ret = uapi_pwm_group_check(channel);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
#endif /* CONFIG_PWM_USING_V151 */

    pwm_irq_lock(channel);
    g_hal_funcs->set_action(channel, PWM_ACTION_START);
    pwm_irq_unlock(channel);

    pwm_port_register_irq((pwm_channel_t)channel);

    return ret;
}

uint32_t uapi_pwm_get_frequency(uint8_t channel)
{
    return pwm_port_get_clock_value((pwm_channel_t)channel);
}

#if defined(CONFIG_PWM_USING_V150)
errcode_t uapi_pwm_stop(uint8_t channel)
{
    errcode_t ret = uapi_pwm_check(channel);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    g_hal_funcs->set_action(channel, PWM_ACTION_STOP);
    pwm_port_unregister_irq((pwm_channel_t)channel);

    return ret;
}

errcode_t uapi_pwm_update_duty_ratio(uint8_t channel, uint32_t low_time, uint32_t high_time)
{
    errcode_t ret = uapi_pwm_check(channel);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    g_hal_funcs->set_time(PWM_SET_LOW_TIME, (pwm_channel_t)channel, low_time);
    g_hal_funcs->set_time(PWM_SET_HIGH_TIME, (pwm_channel_t)channel, high_time);
    g_hal_funcs->set_action(channel, PWM_ACTION_REFRESH);

    return ret;
}
#endif /* CONFIG_PWM_USING_V150 */

errcode_t uapi_pwm_isr(uint8_t channel)
{
    if (unlikely(channel >= CONFIG_PWM_CHANNEL_NUM)) {
        return ERRCODE_PWM_INVALID_PARAMETER;
    }

    g_hal_funcs->isrclear((pwm_channel_t)channel);

    return ERRCODE_SUCC;
}

errcode_t uapi_pwm_register_interrupt(uint8_t channel, pwm_callback_t callback)
{
    errcode_t ret = uapi_pwm_check(channel);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    g_hal_funcs->registerfunc((pwm_channel_t)channel, (hal_pwm_callback_t)callback);

    return ret;
}

errcode_t uapi_pwm_unregister_interrupt(uint8_t channel)
{
    errcode_t ret = uapi_pwm_check(channel);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    g_hal_funcs->unregisterfunc((pwm_channel_t)channel);

    return ret;
}

#if defined(CONFIG_PWM_USING_V151)
errcode_t uapi_pwm_set_group(uint8_t group, uint16_t channel_id)
{
    if (group >= CONFIG_PWM_GROUP_NUM) {
        return ERRCODE_PWM_INVALID_PARAMETER;
    }

    g_hal_funcs->set_group((pwm_v151_group_t)group, channel_id);

    return ERRCODE_SUCC;
}
#endif /* CONFIG_PWM_USING_V151 */