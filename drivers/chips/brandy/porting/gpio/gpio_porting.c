/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides gpio port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-26， Create file. \n
 */
#include "gpio_porting.h"
#include "hal_gpio_v100.h"
#include "platform_types.h"
#include "platform_core.h"
#include "chip_core_irq.h"
#include "chip_io.h"
#include "arch_port.h"
#include "soc_osal.h"

#define GPIO_CHANNEL_0_BASE_ADDR GPIO0_BASE_ADDR
#define GPIO_CHANNEL_1_BASE_ADDR GPIO1_BASE_ADDR
#define GPIO_CHANNEL_2_BASE_ADDR GPIO2_BASE_ADDR
#define GPIO_CHANNEL_3_BASE_ADDR GPIO3_BASE_ADDR
#define GPIO_CHANNEL_4_BASE_ADDR GPIO4_BASE_ADDR
#define GPIO_CHANNEL_ULP_BASE_ADDR ULP_GPIO_BASE_ADDR
#define CHANNEL_MAX_NUM GPIO_MAX_NUMBER

uintptr_t g_gpio_base_addrs[GPIO_CHANNEL_MAX_NUM] = {
    (uintptr_t)GPIO_CHANNEL_0_BASE_ADDR,
    (uintptr_t)GPIO_CHANNEL_1_BASE_ADDR,
    (uintptr_t)GPIO_CHANNEL_2_BASE_ADDR,
    (uintptr_t)GPIO_CHANNEL_3_BASE_ADDR,
#if (GPIO_WITH_ULP == YES)
    (uintptr_t)GPIO_CHANNEL_ULP_BASE_ADDR,
#endif
};

uint32_t irq_num[GPIO_CHANNEL_MAX_NUM] = {
    GPIO_0_IRQN,
    GPIO_1_IRQN,
    GPIO_2_IRQn,
    GPIO_3_IRQn,
#if (GPIO_WITH_ULP == YES)
    ULP_GPIO_IRQN,
#endif
};

uintptr_t gpio_porting_base_addr_get(gpio_channel_t channel)
{
    return g_gpio_base_addrs[channel];
}

void gpio_port_register_irq(uint32_t int_id)
{
    osal_irq_request(int_id, (osal_irq_handler)irq_gpio_handler, NULL, NULL, NULL);
    osal_irq_set_priority(int_id, irq_prio(int_id));
}

int irq_gpio_handler(int a, const void *tmp)
{
    unused(a);
    unused(tmp);
    hal_gpio_irq_handler();
    return 0;
}

void gpio_port_register_hal_funcs(void)
{
    hal_gpio_register_funcs(hal_gpio_v100_funcs_get());
}

void gpio_port_unregister_hal_funcs(void)
{
    hal_gpio_unregister_funcs();
}

void gpio_context_adapter(void)
{
    gpio_context_t *ctxt = gpio_context_get();

    ctxt->pin_max_num = PIN_MAX_NUMBER;
#if (GPIO_WITH_ULP == YES)
    ctxt->is_ulp_enable = true;
    ctxt->ulp_pin_start = ULP_GPIO0;
#if (AON_SPECIAL_PIO == YES)
    ctxt->ulp_pin_end = ULP_GPIO1;
#else
    ctxt->ulp_pin_end = PMUIC_IRQ;
#endif
#else
    ctxt->is_ulp_enable = false;
#endif
#if ENABLE_GPIO_INTERRUPT == YES
    ctxt->is_irq_enable = true;
#else
    ctxt->is_irq_enable = false;
#endif
    for (int i = 0; i < CHANNEL_MAX_NUM; i++) {
        ctxt->irq_list[i] = irq_num[i];
    }
}

void gpio_hal_context_adapter(void)
{
    hal_gpio_context_t *hal_ctxt = hal_gpio_context_get();

    hal_ctxt->pin_max_num = PIN_MAX_NUMBER;
#if (GPIO_WITH_ULP == YES)
    hal_ctxt->is_ulp_enable = true;
    hal_ctxt->ulp_pin_start = ULP_GPIO0;
    hal_ctxt->ulp_pin_end = PMUIC_IRQ;
    hal_ctxt->ulp_pin_channel = GPIO_CHANNEL_5;
#else
    hal_ctxt->is_ulp_enable = false;
#endif
}

void gpio_ulp_int_en(bool on)
{
#if (GPIO_WITH_ULP == YES)
    if (!hal_gpio_context_get()->is_ulp_enable) {
        return;
    }
    uint8_t clk_en = BIT(HAL_GPIO_ULP_AON_PCLK_INT_EN_BIT) | BIT(HAL_GPIO_ULP_AON_PCLK_INT_CLK_SEL_BIT);
    if (on) {
        do {
            // enable ulp gpio interrupt
            reg16_setbit(HAL_GPIO_ULP_AON_GP_REG, HAL_GPIO_ULP_AON_PCLK_INT_EN_BIT);
            // set ulp gpio clk 32K
            reg16_setbit(HAL_GPIO_ULP_AON_GP_REG, HAL_GPIO_ULP_AON_PCLK_INT_CLK_SEL_BIT);
        } while ((reg16(HAL_GPIO_ULP_AON_GP_REG) & HAL_GPIO_ULP_PCLK_INTR_STATUS_BITS) != clk_en);
    } else {
        // disable ulp gpio interrupt
        do {
            reg16_clrbit(HAL_GPIO_ULP_AON_GP_REG, HAL_GPIO_ULP_AON_PCLK_INT_EN_BIT);
        } while (reg16_getbit(HAL_GPIO_ULP_AON_GP_REG, HAL_GPIO_ULP_AON_PCLK_INT_EN_BIT) != 0);
    }
#else
    UNUSED(on);
#endif
}

void gpio_ulp_clock_switch_to_pclk(bool on)
{
#if (GPIO_WITH_ULP == YES)
    if (!hal_gpio_context_get()->is_ulp_enable) {
        return;
    }
    if (on) {
        // set ulp gpio clk 32M
        reg16_clrbit(HAL_GPIO_ULP_AON_GP_REG, HAL_GPIO_ULP_AON_PCLK_INT_CLK_SEL_BIT);
    } else {
        // set ulp gpio clk 32K
        reg16_setbit(HAL_GPIO_ULP_AON_GP_REG, HAL_GPIO_ULP_AON_PCLK_INT_CLK_SEL_BIT);
    }
#else
    UNUSED(on);
#endif
}

static gpio_channel_t port_gpio_get_channel(pin_t pin)
{
    if (hal_gpio_context_get()->is_ulp_enable &&
        pin >= hal_gpio_context_get()->ulp_pin_start && pin <= hal_gpio_context_get()->ulp_pin_end) {
        return hal_gpio_context_get()->ulp_pin_channel;
    }
    return (gpio_channel_t)(pin / HAL_GPIO_CHANNEL_OFFSET);
}

static uint32_t port_gpio_get_channel_pin(pin_t pin)
{
    if (hal_gpio_context_get()->is_ulp_enable &&
        pin >= hal_gpio_context_get()->ulp_pin_start && pin <= hal_gpio_context_get()->ulp_pin_end) {
        return pin - hal_gpio_context_get()->ulp_pin_start;//lint !e732
    }
    return pin % HAL_GPIO_CHANNEL_OFFSET;//lint !e732
}

void gpio_select_core(pin_t pin, cores_t core)
{
    if (pin >= ULP_PIN) {
        return;
    }
    uint32_t channel = (uint32_t)port_gpio_get_channel(pin);
    uint32_t channel_pin = port_gpio_get_channel_pin(pin);
    uint32_t addr_dsp = (uint32_t)(HAL_GPIO_D_CORE_SET_REG + channel * HAL_GPIO_CORE_SET_CHANNEL_OFFSET);
    uint32_t addr_non_dsp = (uint32_t)(HAL_GPIO_NON_D_CORE_SET_REG + channel * HAL_GPIO_CORE_SET_CHANNEL_OFFSET);
    uint32_t offset = ((uint32_t)(channel_pin / HAL_GPIO_CORE_SET_GPIOS)) << HAL_GPIO_CORE_SET_REG_OFFSET;
    channel_pin = (pin_t)(channel_pin % HAL_GPIO_CORE_SET_GPIOS);
    switch (core) {
        case CORES_PROTOCOL_CORE:
            reg16_setbit((uintptr_t)addr_dsp + offset, channel_pin);
            break;
        case CORES_APPS_CORE:
            reg16_clrbit((uintptr_t)addr_dsp + offset, channel_pin);
            reg16_clrbit((uintptr_t)addr_non_dsp + offset, channel_pin);
            break;
        case CORES_BT_CORE:
            reg16_clrbit((uintptr_t)addr_dsp + offset, channel_pin);
            reg16_setbit((uintptr_t)addr_non_dsp + offset, channel_pin);
            break;
        default:
            break;
    }
}