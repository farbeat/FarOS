/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Provides button port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-15， Create file. \n
 */

#include "button_porting.h"
#include "chip_core_irq.h"
#include "chip_io.h"
#include "gpio.h"
#include "pm_veto.h"
#include "debug_print.h"
#include "tcxo.h"
#include "clock_calibration.h"

bool  g_startup_finish_flag = false;
uint16_t g_ulp_wkup_flag = 0;
static uint32_t g_clock_ulp = 3276800;

void set_startup_finish_flag(void)
{
    g_startup_finish_flag = true;
}

void uapi_ulp_wkup_type_record(void)
{
    if (g_startup_finish_flag == false) {
        g_ulp_wkup_flag = 0;
        g_ulp_wkup_flag = readw(ULP_WKUP_INT_STS);
        g_startup_finish_flag = true;
        PRINT("startup reason flag is 0x%x\r\n", g_ulp_wkup_flag);
    }
}

startup_reason_t uapi_get_startup_reason(void)
{
    startup_reason_t startup_type = STARTUP_OTHER;

    if (((g_ulp_wkup_flag >> WKUP_SYSTICK_MASK) & 1) == 1) {
        startup_type = STARTUP_SYSTICK;
    }
    if (((g_ulp_wkup_flag >> WKUP_PWRON_MASK) & 1) == 1) {
        startup_type = STARTUP_PWRON;
    }
    if (((g_ulp_wkup_flag >> WKUP_VBUS_EN_MASK) & 1) == 1) {
        startup_type = STARTUP_VBUS_EN;
    }
    return startup_type;
}

errcode_t uapi_power_set_mode(power_mode_t pdmode)
{
    if (pdmode >= PDMODE_MAX) {
        return ERRCODE_INVALID_PARAM;
    }

    if (pdmode == PDMODE_RE_UP) {
        reg16_clrbit(PWRON_PD_MODE_REG, PWRON_PD_MODE_POS4);
    } else if (pdmode == PDMODE_DOWN) {
        reg16_setbit(PWRON_PD_MODE_REG, PWRON_PD_MODE_POS4);
    }
    return ERRCODE_SUCC;
}

uint64_t uapi_ulp_systick_get_count(void)
{
    uint64_t count = 0;

    count = (uint64_t)readw(ULP_SYSTICK_VAL3) << 48U;
    count |= (uint64_t)readw(ULP_SYSTICK_VAL2) << 32U;
    count |= (uint64_t)readw(ULP_SYSTICK_VAL1) << 16U;
    count |= (uint64_t)readw(ULP_SYSTICK_VAL0);

    return count;
}

uint32_t uapi_ulp_systick_soc_calibration(void)
{
    unsigned int irq_sts = osal_irq_lock();
    uint16_t val = readw(CLK_32K_32M_EN_SEL_REG);
    reg16_clrbit(CLK_32K_32M_EN_SEL_REG, PMU_RC_512K_XO_SEL_MASK);
    reg16_clrbit(CLK_32K_32M_EN_SEL_REG, RCXO_32K_SEL_MASK);
    g_clock_ulp = calibration_get_clock_frq(CALIBRATION_CYCLE_MUL);
    writew(CLK_32K_32M_EN_SEL_REG, val);
    osal_irq_restore(irq_sts);
    return g_clock_ulp;
}

uint32_t uapi_ulp_systick_get_frequency(void)
{
    return g_clock_ulp;
}

uint64_t uapi_ulp_systick_get_ms(void)
{
    uint64_t count = 0;
    count = uapi_ulp_systick_get_count();
    uint64_t ms = count * MS_PER_S * CALIBRATION_CLOCK_MUL / g_clock_ulp;

    return ms;
}

errcode_t uapi_enable_alarm(uint64_t timeout_ms)
{
    uint64_t count = 0;
    count = uapi_ulp_systick_get_count();
    count += (timeout_ms * g_clock_ulp / (MS_PER_S * CALIBRATION_CLOCK_MUL));

    // alarm enable
    reg16_setbit(ULP_WKUP_EVT_EN, POS_2);
    reg16_setbit(ULP_WKUP_INT_EN, POS_2);

    writew(ULP_SYSTICK_SET3, (uint16_t)(count >> 48U));
    writew(ULP_SYSTICK_SET2, (uint16_t)(count >> 32U));
    writew(ULP_SYSTICK_SET1, (uint16_t)(count >> 16U));
    writew(ULP_SYSTICK_SET0, (uint16_t)count);

    return ERRCODE_SUCC;
}

errcode_t uapi_disable_alarm(void)
{
    reg16_clrbit(ULP_WKUP_EVT_EN, POS_2);
    reg16_clrbit(ULP_WKUP_INT_EN, POS_2);

    writew(ULP_SYSTICK_SET3, 0);
    writew(ULP_SYSTICK_SET2, 0);
    writew(ULP_SYSTICK_SET1, 0);
    writew(ULP_SYSTICK_SET0, 0);

    return ERRCODE_SUCC;
}

errcode_t uapi_power_time_mode(power_time_t time_mode)
{
    if (time_mode >= PDTIME_LEVELMAX) {
        return ERRCODE_INVALID_PARAM;
    }

    if (time_mode == PDTIME_LEVEL0) {
        reg16_setbit(PWRON_PD_MODE_REG, PWRON_TM_MODE_POS0);
        reg16_clrbit(PWRON_PD_MODE_REG, PWRON_TM_MODE_POS1);
    }
    if (time_mode == PDTIME_LEVEL1) {
        reg16_clrbit(PWRON_PD_MODE_REG, PWRON_TM_MODE_POS0);
        reg16_clrbit(PWRON_PD_MODE_REG, PWRON_TM_MODE_POS1);
    }
    if (time_mode == PDTIME_LEVEL2) {
        reg16_clrbit(PWRON_PD_MODE_REG, PWRON_TM_MODE_POS0);
        reg16_setbit(PWRON_PD_MODE_REG, PWRON_TM_MODE_POS1);
    }
    if (time_mode == PDTIME_LEVEL3) {
        reg16_setbit(PWRON_PD_MODE_REG, PWRON_TM_MODE_POS0);
        reg16_setbit(PWRON_PD_MODE_REG, PWRON_TM_MODE_POS1);
    }
    return ERRCODE_SUCC;
}

static button_press_state_t porting_button_get_state(pin_t pin)
{
    if (pin == PIN_NONE) {
        if ((readl(ANA_STS) & 0x3) == 0) {
            return PRESSED;
        } else {
            return RELEASED;
        }
    } else {
        if (uapi_gpio_get_val(pin) == GPIO_LEVEL_LOW) {
            return PRESSED;
        } else {
            return RELEASED;
        }
    }
}

static errcode_t porting_button_irq_register(pin_t pin, osal_irq_handler thread_handler)
{
    unused(pin);
    osal_irq_request(ULP_TO_MCU_IRQn, thread_handler, NULL, NULL, NULL);
    osal_irq_enable(ULP_TO_MCU_IRQn);

    writel(ULP_WKUP_INT_EN, ULP_WKUP_INT_EN_MASK);
    return ERRCODE_SUCC;
}

static errcode_t porting_button_unregister(pin_t pin)
{
    if (pin == PIN_NONE) {
        osal_irq_free(ULP_TO_MCU_IRQn, NULL);
    } else {
        uapi_gpio_unregister_isr_func(pin);
    }
    return ERRCODE_SUCC;
}

static bool porting_button_vbus_event_check(void)
{
    if (reg32_getbit(ULP_WKUP_INT_STS, VBUS_INSERT_INTERRUPT_POS) == 1 ||
        reg32_getbit(ULP_WKUP_INT_STS, VBUS_UNPLUG_INNTERRUPT_POS) == 1) {
        return true;
    }

    return false;
}

void insert_vbus(void)
{
    PRINT("insert_vbus");  // add user insert usb notify function here to change ui
}

void unplug_vbus(void)
{
    PRINT("unplug_vbus");  // add user unplug usb notify function here to change ui
}

static void deal_with_vbus_event(void)
{
    if (reg32_getbit(ULP_WKUP_INT_STS, VBUS_INSERT_INTERRUPT_POS) == 1) {
        insert_vbus();
        return;
    }

    if (reg32_getbit(ULP_WKUP_INT_STS, VBUS_UNPLUG_INNTERRUPT_POS) == 1) {
        unplug_vbus();
        return;
    }
    return;
}

static bool porting_button_check_int(pin_t pin)
{
    unused(pin);
    if (reg32_getbit(ULP_WKUP_INT_STS, WKUP_PWRON_MASK) == 1 ||
        reg32_getbit(ULP_WKUP_INT_STS, VBUS_INSERT_INTERRUPT_POS) == 1 ||
        reg32_getbit(ULP_WKUP_INT_STS, VBUS_UNPLUG_INNTERRUPT_POS) == 1) {
        return true;
    }

    return false;
}

static bool porting_tick_check_int(pin_t pin)
{
    unused(pin);
    if (reg32_getbit(ULP_WKUP_INT_STS, 0x2) == 1) {
        return true;
    }

    return false;
}

static errcode_t porting_button_clear_int(pin_t pin)
{
    unused(pin);
    osal_irq_clear(ULP_TO_MCU_IRQn);
    writel(ULP_WKUP_INT_CLR, ULP_WKUP_INT_CLR_MASK);

    while (porting_button_check_int(pin) == true) {
        uapi_tcxo_delay_us(ULP_INT_CLEAR_DELAY);
        continue;
    }
    while (porting_button_vbus_event_check() == true) {
        uapi_tcxo_delay_us(ULP_INT_CLEAR_DELAY);
        continue;
    }
    while (porting_tick_check_int(pin) == true) {
        uapi_tcxo_delay_us(ULP_INT_CLEAR_DELAY);
        continue;
    }
    return ERRCODE_SUCC;
}

static errcode_t porting_button_pmu_pwr_cfg(pin_t pin)
{
    unused(pin);
    return ERRCODE_SUCC;
}

static void porting_button_add_veto(void)
{
    uapi_pm_add_sleep_veto(PM_ID_BUTTON);
}

static void porting_button_remove_veto(void)
{
    uapi_pm_remove_sleep_veto(PM_ID_BUTTON);
}

button_peripheral_api g_brandy_button_api = {
    .button_pmu_pwr_cfg = porting_button_pmu_pwr_cfg,
    .button_check_interrupt = porting_button_check_int,
    .button_check_vbus_interrupt = porting_button_vbus_event_check,
    .button_clear_interrupt = porting_button_clear_int,
    .button_deal_vbus_event = deal_with_vbus_event,
    .button_get_state = porting_button_get_state,
    .button_register_callback = porting_button_irq_register,
    .button_unregister_callback = porting_button_unregister,
    .button_add_veto = porting_button_add_veto,
    .button_remove_veto = porting_button_remove_veto,
};

button_peripheral_api *button_port_get_api(void)
{
    return &g_brandy_button_api;
}
