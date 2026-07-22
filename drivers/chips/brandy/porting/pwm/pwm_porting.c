/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides pwm port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */

#include "pwm_porting.h"
#include "soc_osal.h"
#include "common_def.h"
#include "hal_pwm_v150.h"
#include "chip_core_irq.h"
#include "hal_clocks_app_priv.h"
#include "platform_core.h"

#define BUS_CLOCK_TIME_32M      32000000UL

uintptr_t g_pwm_base_addr =  (uintptr_t)PWM_0_BASE;

static bool g_pwm_irq_created[PWM_MAX_NUMBER] = { false };

uintptr_t pwm_porting_base_addr_get(void)
{
    return g_pwm_base_addr;
}

static int pwm_0_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_pwm_irq_handler(PWM_0);
    return 0;
}

static int pwm_1_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_pwm_irq_handler(PWM_1);
    return 0;
}

static int pwm_2_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_pwm_irq_handler(PWM_2);
    return 0;
}
#if PWM_MAX_NUMBER > 5
static int pwm_3_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_pwm_irq_handler(PWM_3);
    return 0;
}

static int pwm_4_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_pwm_irq_handler(PWM_4);
    return 0;
}

static int pwm_5_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_pwm_irq_handler(PWM_5);
    return 0;
}
#endif
static osal_irq_handler const g_pwm_interrupt_handlers[PWM_MAX_NUMBER] = {
    pwm_0_handler,
    pwm_1_handler,
    pwm_2_handler,
#if PWM_MAX_NUMBER > 5
    pwm_3_handler,
    pwm_4_handler,
    pwm_5_handler
#endif
};

uint32_t pwm_irq_num[PWM_MAX_NUMBER] = {
    (uintptr_t)PWM_0_IRQN,
    (uintptr_t)PWM_1_IRQN,
    (uintptr_t)PWM_2_IRQN,
#if PWM_MAX_NUMBER > 5
    (uintptr_t)PWM_3_IRQN,
    (uintptr_t)PWM_4_IRQN,
    (uintptr_t)PWM_5_IRQN
#endif
};

void pwm_port_register_hal_funcs(void)
{
    hal_pwm_register_funcs(hal_pwm_v150_funcs_get());
}

void pwm_port_unregister_hal_funcs(void)
{
    hal_pwm_unregister_funcs();
}

void pwm_port_register_irq(pwm_channel_t channel)
{
    if (!g_pwm_irq_created[channel]) {
        osal_irq_request(pwm_irq_num[channel], g_pwm_interrupt_handlers[channel], NULL, NULL, NULL);
        osal_irq_enable(pwm_irq_num[channel]);
        g_pwm_irq_created[channel] = true;
    }
}

void pwm_port_unregister_irq(pwm_channel_t channel)
{
    if (g_pwm_irq_created[channel]) {
        osal_irq_disable(pwm_irq_num[channel]);
        osal_irq_free(pwm_irq_num[channel], NULL);
        g_pwm_irq_created[channel] = false;
    }
}

void pwm_port_clock_enable(bool on)
{
    if (on) {
        hal_clocks_app_perips_config(HAL_CLOCKS_APP_PWM_CLK, TURN_ON);
    } else {
        hal_clocks_app_perips_config(HAL_CLOCKS_APP_PWM_CLK, TURN_OFF);
    }
}

uint32_t pwm_port_get_clock_value(pwm_channel_t channel)
{
    if (channel >= PWM_MAX_NUMBER) {
        return 0;
    }
    return BUS_CLOCK_TIME_32M;
}

static unsigned int g_irq;

uint32_t pwm_irq_lock(uint8_t channel)
{
    unused(channel);
    g_irq = osal_irq_lock();
    return osal_irq_lock();
}

void pwm_irq_unlock(uint8_t channel)
{
    unused(channel);
    osal_irq_restore(g_irq);
}

void pwm_irq_restore(uint8_t channel, uint32_t irq)
{
    unused(channel);
    osal_irq_restore(irq);
}
