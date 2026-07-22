/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V100 HAL gpio \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-26, Create file. \n
 */
#include <stdint.h>
#include "soc_osal.h"
#include "securec.h"
#include "gpio_porting.h"
#include "common_def.h"
#include "hal_gpio_v100.h"

#define GPIO_TIME_OUT         15

static gpio_context_t g_gpio_context = {
    .claimed = { 0 },
    .direction = { 0 },
};

static hal_gpio_context_t g_hal_gpio_context;

static bool hal_gpio_set_interrupt(pin_t pin, uint32_t gpio_interrupt, gpio_callback_t callback, bool need_callback);

static bool hal_gpio_bitget(uint32_t *loc, pin_t bit);

static void hal_gpio_bitset(uint32_t *loc, pin_t bit);

static uint32_t hal_gpio_read(pin_t pin);

static void hal_gpio_bitclr(uint32_t *loc, pin_t bit);

static errcode_t hal_gpio_v100_ctrl_toggle(pin_t pin, hal_gpio_ctrl_id_t id);

static errcode_t hal_gpio_v100_ctrl_enable_interrupt(pin_t pin, hal_gpio_ctrl_id_t id);

static errcode_t hal_gpio_v100_ctrl_disable_interrupt(pin_t pin, hal_gpio_ctrl_id_t id);

static errcode_t hal_gpio_v100_ctrl_clear_interrupt(pin_t pin, hal_gpio_ctrl_id_t id);

static void hal_gpio_clear_interrupt(pin_t pin);

static void hal_gpio_disable_interrupt(pin_t pin);

static void hal_gpio_toggle(pin_t pin);

static void hal_gpio_clear(pin_t pin);

static void hal_gpio_set(pin_t pin);

static void hal_gpio_config_dir(pin_t pin, gpio_direction_t dir);

static void hal_gpio_enable_interrupt(pin_t pin);

static uint32_t hal_gpio_get_channel_pin(pin_t pin);

static gpio_channel_t hal_gpio_get_channel(pin_t pin);

gpio_context_t *gpio_context_get(void)
{
    return &g_gpio_context;
}

hal_gpio_context_t *hal_gpio_context_get(void)
{
    return &g_hal_gpio_context;
}

static void hal_gpio_v100_init(void)
{
    gpio_context_adapter();
    gpio_hal_context_adapter();

    /* Mark every GPIO as unclaimed and clear GPIO direction */
    if (g_gpio_context.is_irq_enable) {
        for (uint16_t i = 0; i < GPIO_CHANNELS_NUM; i++) {
            g_gpio_context.claimed[i] = 0;
            g_gpio_context.direction[i] = 0;
            gpio_port_register_irq(g_gpio_context.irq_list[i]);
        }
    }

    hal_gpio_regs_init();

    if (g_gpio_context.is_ulp_enable && g_gpio_context.is_irq_enable) {
        gpio_ulp_int_en(true);
    }
}

static void hal_gpio_v100_deinit(void)
{
    if (g_gpio_context.is_ulp_enable) {
        gpio_ulp_int_en(false);
    }

    for (uint16_t i = 0; i < GPIO_CHANNELS_NUM; i++) {
        g_gpio_context.claimed[i] = 0;
        g_gpio_context.direction[i] = 0;
    }

    for (uint32_t i = 0; i < GPIO_CHANNELS_NUM; i++) {
        osal_irq_free(g_gpio_context.irq_list[i], NULL);
    }

    hal_gpio_regs_deinit();
}

static void hal_gpio_bitclr(uint32_t *loc, pin_t bit)
{
    uint32_t *channel = 0;
    uint32_t uint32_bit = (uint32_t)bit;
    channel = loc + uint32_bit / CONFIG_GPIO_PWIDTH_A;
    uint32_bit %= CONFIG_GPIO_PWIDTH_A;

    *channel &= ~bit(uint32_bit);
}

static void hal_gpio_bitset(uint32_t *loc, pin_t bit)
{
    uint32_t *channel = 0;
    uint32_t uint32_bit = (uint32_t)bit;
    channel = loc +  uint32_bit / CONFIG_GPIO_PWIDTH_A;  /* get gpio channel */
    uint32_bit %= CONFIG_GPIO_PWIDTH_A;  /* get gpio bit */

    *channel |= bit(uint32_bit);
}

static bool hal_gpio_bitget(uint32_t *loc, pin_t bit)
{
    uint32_t *channel = 0;
    uint32_t uint32_bit = (uint32_t)bit;
    channel = loc +  uint32_bit / CONFIG_GPIO_PWIDTH_A;  /* get gpio channel */
    uint32_bit %= CONFIG_GPIO_PWIDTH_A;  /* get gpio bit */

    return ((*channel & bit(uint32_bit)) != 0);
}

static errcode_t hal_gpio_v100_setdir(pin_t pin, gpio_direction_t dir)
{
    if (pin >= g_gpio_context.pin_max_num) {
        return ERRCODE_INVALID_PARAM;
    }

    /* Set a known state after successfully claiming the PIN */
    hal_gpio_disable_interrupt(pin);

    hal_gpio_config_dir(pin, dir);

    if (dir == GPIO_DIRECTION_INPUT) {
        hal_gpio_bitclr(g_gpio_context.direction, pin);
        gpio_channel_t channel = hal_gpio_get_channel(pin);
        uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
        hal_gpio_gpio_debounce_set_bit_data(channel, channel_pin, 1);
    } else if (dir == GPIO_DIRECTION_OUTPUT) {
        hal_gpio_bitset(g_gpio_context.direction, pin);
    }
    hal_gpio_bitset(g_gpio_context.claimed, pin);

    return ERRCODE_SUCC;
}

#if defined(CONFIG_GPIO_SUPPORT_LPM)
static gpio_info_stru_t g_gpio_regs_backup[GPIO_CHANNEL_MAX_NUM] = { 0 };

static errcode_t hal_gpio_v100_ctrl_suspend(pin_t pin, hal_gpio_ctrl_id_t id)
{
    unused(pin);
    unused(id);

    for (uint32_t i = 0; i < GPIO_CHANNEL_MAX_NUM; i++) {
        if (memcpy_s(&g_gpio_regs_backup[i], sizeof(gpio_info_stru_t),
                     (void *)hal_gpio_base_addrs_get(i), sizeof(gpio_info_stru_t)) != EOK) {
            return ERRCODE_MEMCPY;
        }
    }

    return ERRCODE_SUCC;
}

static errcode_t hal_gpio_v100_ctrl_resume(pin_t pin, hal_gpio_ctrl_id_t id)
{
    unused(pin);
    unused(id);

    for (uint32_t i = 0; i < GPIO_CHANNEL_MAX_NUM; i++) {
        gpio_info_stru_t *gpio_regs = &g_gpio_regs_backup[i];
        hal_gpio_gpio_swporta_dr_set_data((gpio_channel_t)i, gpio_regs->gpio_swporta_dr);
        hal_gpio_gpio_swporta_ddr_set_data((gpio_channel_t)i, gpio_regs->gpio_swporta_ddr);

        hal_gpio_gpio_inten_set_data((gpio_channel_t)i, gpio_regs->gpio_inten);
        hal_gpio_gpio_intmask_set_data((gpio_channel_t)i, gpio_regs->gpio_intmask);
        hal_gpio_gpio_inttype_level_set_data((gpio_channel_t)i, gpio_regs->gpio_inttype_level);
        hal_gpio_gpio_int_polarity_set_data((gpio_channel_t)i, gpio_regs->gpio_int_polarity);

        hal_gpio_gpio_debounce_set_data((gpio_channel_t)i, gpio_regs->gpio_debounce);

        hal_gpio_gpio_ls_sync_set_data((gpio_channel_t)i, gpio_regs->gpio_ls_sync);
    }

    return ERRCODE_SUCC;
}
#endif

static hal_gpio_ctrl_t g_hal_gpio_ctrl_func_array[GPIO_CTRL_MAX] = {
    hal_gpio_v100_ctrl_toggle,                        /* GPIO_CTRL_TOGGLE */
    hal_gpio_v100_ctrl_enable_interrupt,              /* GPIO_CTRL_ENABLE_INTERRUPT */
    hal_gpio_v100_ctrl_disable_interrupt,             /* GPIO_CTRL_DISABLE_INTERRUPT */
    hal_gpio_v100_ctrl_clear_interrupt,               /* GPIO_CTRL_CLEAR_INTERRUPT */
#if defined(CONFIG_GPIO_SUPPORT_LPM)
    hal_gpio_v100_ctrl_suspend,                       /* GPIO_CTRL_SUSPEND */
    hal_gpio_v100_ctrl_resume                         /* GPIO_CTRL_RESUME */
#endif
};

static gpio_channel_t hal_gpio_get_channel(pin_t pin)
{
    if (g_hal_gpio_context.is_ulp_enable &&
        pin >= g_hal_gpio_context.ulp_pin_start && pin <= g_hal_gpio_context.ulp_pin_end) {
        return g_hal_gpio_context.ulp_pin_channel;
    }
    return (gpio_channel_t)((uint32_t)pin / HAL_GPIO_CHANNEL_OFFSET);
}

static uint32_t hal_gpio_get_channel_pin(pin_t pin)
{
    if (g_hal_gpio_context.is_ulp_enable &&
        pin >= g_hal_gpio_context.ulp_pin_start && pin <= g_hal_gpio_context.ulp_pin_end) {
        return (uint32_t)pin - (uint32_t)g_hal_gpio_context.ulp_pin_start;
    }
    return (uint32_t)pin % HAL_GPIO_CHANNEL_OFFSET;
}

static gpio_direction_t hal_gpio_v100_getdir(pin_t pin)
{
    gpio_direction_t direction;
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
    direction = (gpio_direction_t)hal_gpio_gpio_swporta_ddr_get_data(channel, channel_pin);
    return direction;
}

static errcode_t hal_gpio_v100_ctrl(pin_t pin, hal_gpio_ctrl_id_t id)
{
    return g_hal_gpio_ctrl_func_array[id](pin, id);
}

static errcode_t hal_gpio_v100_ctrl_toggle(pin_t pin, hal_gpio_ctrl_id_t id)
{
    unused(id);
    if (pin >= g_gpio_context.pin_max_num) {
        return ERRCODE_INVALID_PARAM;
    }

    if (!hal_gpio_bitget(g_gpio_context.direction, pin)) {
        return ERRCODE_GPIO_DIR_SET_FAIL;
    }

    hal_gpio_toggle(pin);

    return ERRCODE_SUCC;
}

static void hal_gpio_interrupt_clear_flag(pin_t pin)
{
    if (g_hal_gpio_context.interrupts_flag[pin].callback != NULL) {
        g_hal_gpio_context.interrupts_flag[pin].flag = false;
        g_hal_gpio_context.interrupts_flag[pin].callback = NULL;
    }
}

static errcode_t hal_gpio_v100_unregister(pin_t pin)
{
    if (pin >= g_gpio_context.pin_max_num) {
        return ERRCODE_INVALID_PARAM;
    }

    if (hal_gpio_bitget(g_gpio_context.direction, pin)) {
        return ERRCODE_GPIO_DIR_SET_FAIL;
    }

    hal_gpio_disable_interrupt(pin);
    hal_gpio_clear_interrupt(pin);
    hal_gpio_interrupt_clear_flag(pin);

    return ERRCODE_SUCC;
}

static errcode_t hal_gpio_v100_ctrl_enable_interrupt(pin_t pin, hal_gpio_ctrl_id_t id)
{
    unused(id);
    if (pin >= g_gpio_context.pin_max_num) {
        return ERRCODE_INVALID_PARAM;
    }
    gpio_channel_t channel = hal_gpio_get_channel(pin);

    hal_gpio_bitset(g_gpio_context.claimed, pin);

    osal_irq_enable(g_gpio_context.irq_list[channel]);

    hal_gpio_enable_interrupt(pin);

    return ERRCODE_SUCC;
}

static errcode_t hal_gpio_v100_ctrl_disable_interrupt(pin_t pin, hal_gpio_ctrl_id_t id)
{
    unused(id);
    if (pin >= g_gpio_context.pin_max_num) {
        return ERRCODE_INVALID_PARAM;
    }

    hal_gpio_disable_interrupt(pin);

    hal_gpio_bitclr(g_gpio_context.claimed, pin);

    return ERRCODE_SUCC;
}

static errcode_t hal_gpio_v100_ctrl_clear_interrupt(pin_t pin, hal_gpio_ctrl_id_t id)
{
    unused(id);
    if (pin >= g_gpio_context.pin_max_num) {
        return ERRCODE_INVALID_PARAM;
    }
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    hal_gpio_clear_interrupt(pin);
    osal_irq_clear(g_gpio_context.irq_list[channel]);

    return ERRCODE_SUCC;
}

static errcode_t hal_gpio_v100_output(pin_t pin, gpio_level_t level)
{
    if (pin >= g_gpio_context.pin_max_num || (level != GPIO_LEVEL_LOW && level != GPIO_LEVEL_HIGH)) {
        return ERRCODE_INVALID_PARAM;
    }

    if ((uint32_t)hal_gpio_v100_getdir(pin) == 0) {
        return ERRCODE_GPIO_DIR_SET_FAIL;
    }

    level == GPIO_LEVEL_HIGH ? hal_gpio_set(pin) : hal_gpio_clear(pin);

    return ERRCODE_SUCC;
}

static gpio_level_t hal_gpio_v100_get_output_val(pin_t pin)
{
    gpio_level_t level;
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
    level = (gpio_level_t)hal_gpio_gpio_swporta_dr_get_data(channel, channel_pin);
    return level;
}

static gpio_level_t hal_gpio_v100_input(pin_t pin)
{
    if (pin >= g_gpio_context.pin_max_num) {
        return GPIO_LEVEL_LOW;
    }

    return (gpio_level_t)hal_gpio_read(pin);
}

static errcode_t hal_gpio_v100_register(pin_t pin, uint32_t trigger, gpio_callback_t callback, bool need_callback)
{
    if (pin >= g_gpio_context.pin_max_num || (need_callback == true && callback == NULL) ||
        (need_callback == false && callback != NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

    if (hal_gpio_bitget(g_gpio_context.direction, pin)) {
        return ERRCODE_GPIO_DIR_SET_FAIL;
    }

    /* Configure peripheral interrupts */
    hal_gpio_disable_interrupt(pin);
    hal_gpio_clear_interrupt(pin);

    if (hal_gpio_set_interrupt(pin, trigger, callback, need_callback) == false) {
        return ERRCODE_FAIL;
    }
    hal_gpio_v100_ctrl_enable_interrupt(pin, GPIO_CTRL_ENABLE_INTERRUPT);

    return ERRCODE_SUCC;
}

static bool hal_gpio_interrupt_set_flag(pin_t pin, gpio_callback_t callback)
{
    if (g_hal_gpio_context.interrupts_flag[pin].flag == false) {
        g_hal_gpio_context.interrupts_flag[pin].flag = true;
        g_hal_gpio_context.interrupts_flag[pin].callback = callback;
        return true;
    }
    return false;
}

static void hal_gpio_invoke_callback(uint8_t gpio_irq_number)
{
    if (gpio_irq_number < g_hal_gpio_context.pin_max_num) {
        if ((g_hal_gpio_context.interrupts_flag[gpio_irq_number].flag) &&
            (g_hal_gpio_context.interrupts_flag[gpio_irq_number].callback != NULL)) {
            g_hal_gpio_context.interrupts_flag[gpio_irq_number].callback((pin_t)gpio_irq_number, 0);
        }
    }
}

static void hal_gpio_config_dir(pin_t pin, gpio_direction_t dir)
{
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
    hal_gpio_gpio_swporta_ddr_set_bitdata(channel, channel_pin, (uint32_t)dir);
}

static void hal_gpio_set(pin_t pin)
{
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
    hal_gpio_gpio_swporta_dr_set_bit(channel, channel_pin, GPIO_LEVEL_HIGH);
}

static void hal_gpio_clear(pin_t pin)
{
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
    hal_gpio_gpio_swporta_dr_set_bit(channel, channel_pin, GPIO_LEVEL_LOW);
}

static void hal_gpio_toggle(pin_t pin)
{
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
    hal_gpio_gpio_swporta_dr_toggle_bit(channel, channel_pin);
}

static uint32_t hal_gpio_read(pin_t pin)
{
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
    uint32_t external_port_a = hal_gpio_gpio_ext_porta_get_bit_data(channel, channel_pin);
    return external_port_a;
}

static bool hal_gpio_set_interrupt(pin_t pin, uint32_t gpio_interrupt, gpio_callback_t callback, bool need_callback)
{
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
    if (need_callback) {
        pin_t gpio_pin = pin;
        if (hal_gpio_interrupt_set_flag(gpio_pin, callback) == false) {
            return false;
        }
    }
    switch (gpio_interrupt) {
        case GPIO_INTERRUPT_RISING_EDGE:
            hal_gpio_gpio_inttype_level_set_bit(channel, channel_pin, GPIO_LVL_EDGE_SENSITIVE);
            hal_gpio_gpio_int_polarity_set_bit(channel, channel_pin, GPIO_ACTIVE_HIGH);
            hal_gpio_gpio_int_bothedge_set_bit(channel, channel_pin, GPIO_DEDGE_DISABLED);
            break;

        case GPIO_INTERRUPT_FALLING_EDGE:
            hal_gpio_gpio_inttype_level_set_bit(channel, channel_pin, GPIO_LVL_EDGE_SENSITIVE);
            hal_gpio_gpio_int_polarity_set_bit(channel, channel_pin, GPIO_ACTIVE_LOW);
            hal_gpio_gpio_int_bothedge_set_bit(channel, channel_pin, GPIO_DEDGE_DISABLED);
            break;

        case GPIO_INTERRUPT_LOW:
            hal_gpio_gpio_inttype_level_set_bit(channel, channel_pin, GPIO_LVL_LEVEL_SENSITIVE);
            hal_gpio_gpio_int_polarity_set_bit(channel, channel_pin, GPIO_ACTIVE_LOW);
            hal_gpio_gpio_int_bothedge_set_bit(channel, channel_pin, GPIO_DEDGE_DISABLED);
            break;

        case GPIO_INTERRUPT_HIGH:
            hal_gpio_gpio_inttype_level_set_bit(channel, channel_pin, GPIO_LVL_LEVEL_SENSITIVE);
            hal_gpio_gpio_int_polarity_set_bit(channel, channel_pin, GPIO_ACTIVE_HIGH);
            hal_gpio_gpio_int_bothedge_set_bit(channel, channel_pin, GPIO_DEDGE_DISABLED);
            break;

        case GPIO_INTERRUPT_DEDGE:
            hal_gpio_gpio_inttype_level_set_bit(channel, channel_pin, GPIO_LVL_EDGE_SENSITIVE);
            hal_gpio_gpio_int_bothedge_set_bit(channel, channel_pin, GPIO_DEDGE_ENABLED);
            break;
        default :
            return false;
    }

    return true;
}

static void hal_gpio_enable_interrupt(pin_t pin)
{
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
    hal_gpio_gpio_inten_set_bit(channel, channel_pin, true);
}

static void hal_gpio_disable_interrupt(pin_t pin)
{
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
    hal_gpio_gpio_inten_set_bit(channel, channel_pin, false);
}

static void hal_gpio_clear_interrupt(pin_t pin)
{
    uint32_t timeout = GPIO_TIME_OUT;
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
    if (hal_gpio_gpio_inttype_level_get_data(channel, channel_pin) != 0) {
        do {
            timeout--;
            hal_gpio_gpio_porta_eoi_set_bit(channel, channel_pin, 1);
        } while ((hal_gpio_gpio_intstatus_get_data(channel, channel_pin) != 0) && timeout > 0);
    }
}

static uint32_t hal_gpio_is_interrupt_asserted(pin_t pin)
{
    gpio_channel_t channel = hal_gpio_get_channel(pin);
    uint32_t channel_pin = hal_gpio_get_channel_pin(pin);
    return hal_gpio_gpio_intstatus_get_data(channel, channel_pin);
}

static bool gpio_any_pins_claimed(void)
{
    for (uint16_t i = 0; i < GPIO_CHANNELS_NUM; i++) {
        if (g_gpio_context.claimed[i] != 0) {
            return true;
        }
    }

    return false;
}

static bool gpio_any_pins_claimed_as_input(gpio_direction_t dir)
{
    for (uint16_t i = 0; i < GPIO_CHANNELS_NUM; i++) {
        if ((dir == GPIO_DIRECTION_INPUT) && ((g_gpio_context.claimed[i] & ~g_gpio_context.direction[i]) != 0)) {
            return true;
        }
    }

    return false;
}

void hal_gpio_irq_handler(void)
{
    uint8_t pin_num = g_gpio_context.pin_max_num;

    if (!gpio_any_pins_claimed() || !gpio_any_pins_claimed_as_input(GPIO_DIRECTION_INPUT)) {
        return;
    }

    for (pin_num = 0; pin_num < g_gpio_context.pin_max_num; pin_num++) {
        if (hal_gpio_is_interrupt_asserted((pin_t)pin_num) != 0) {
            break;
        }
    }

    if (g_gpio_context.is_ulp_enable &&
        (g_gpio_context.ulp_pin_start <= pin_num && pin_num <= g_gpio_context.ulp_pin_end)) {
        gpio_ulp_int_en(true);
        gpio_ulp_clock_switch_to_pclk(true);
    }

    if (pin_num != g_gpio_context.pin_max_num) {
        hal_gpio_v100_ctrl_clear_interrupt(pin_num, GPIO_CTRL_CLEAR_INTERRUPT);
        hal_gpio_invoke_callback(pin_num);
        hal_gpio_v100_ctrl_clear_interrupt(pin_num, GPIO_CTRL_CLEAR_INTERRUPT);
    }

    if (g_gpio_context.is_ulp_enable &&
        (g_gpio_context.ulp_pin_start <= pin_num && pin_num <= g_gpio_context.ulp_pin_end)) {
        gpio_ulp_clock_switch_to_pclk(false);
    }
}

static hal_gpio_funcs_t g_hal_gpio_v100_funcs = {
    .init = hal_gpio_v100_init,
    .deinit = hal_gpio_v100_deinit,
    .setdir = hal_gpio_v100_setdir,
    .getdir = hal_gpio_v100_getdir,
    .ctrl = hal_gpio_v100_ctrl,
    .output = hal_gpio_v100_output,
    .get_output_val = hal_gpio_v100_get_output_val,
    .input = hal_gpio_v100_input,
    .registerfunc = hal_gpio_v100_register,
    .unregisterfunc = hal_gpio_v100_unregister
};

hal_gpio_funcs_t *hal_gpio_v100_funcs_get(void)
{
    return &g_hal_gpio_v100_funcs;
}
