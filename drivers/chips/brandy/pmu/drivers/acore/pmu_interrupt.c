/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:  PMU interrupt FUNCTIONS
 */

#include "pmu_interrupt.h"
#include "lpc_core.h"
#include "tcxo.h"
#include "pmu_cmu.h"
#include "debug_print.h"
#include "hal_pmu_cmu.h"
#ifdef BUILD_APPLICATION_STANDARD
#include "los_status.h"
#include "log_oam_logger.h"
#include "log_def.h"
#include "log_printf.h"
#endif
#ifdef __LITEOS__
#include "los_hwi.h"
#endif
#include "panic.h"
#include "hal_reg_config.h"
#include "oal_interface.h"
#include "osal_interrupt.h"

#define CHECK_FREQUENCY 1000
#define PMU_LDO_TIMEOUT 1000
#define PMU_LDO_UNLOCK_CLEAR_PANIC 1UL

#define PMU_LPM_MCPU_ULP_GPIO_WAKEUP        13
#define PMU_LPM_MCPU_SDIO_WAKEUP            12

#define PMU_LPM_REG_DELAY                       35ULL

#define SHIFT_8           8UL
#define SHIFT_12          12UL
#define SHIFT_16          16UL

#define PMU_SLEEP_SEL_TRACE            0x5700041c
#define PMU_SLEEP_TRACE_SEL_TAG(val)   (writew(PMU_SLEEP_SEL_TRACE, val))
#define PMU_SLEEP_SEL_INT               0x666

#ifdef USE_CMSIS_OS
static volatile uint16_t g_ana_status = 0;
#endif

void pmu_mcpu_allow_to_sleep(switch_type_t on)
{
    hal_pmu_lpm_mcpu_allow_to_sleep(on);
}

switch_type_t pmu_mcpu_allow_to_sleep_status(void)
{
    return hal_pmu_lpm_get_mcpu_allow_to_sleep();
}

void pmu_lpm_system_bypass_bt_status(switch_type_t on)
{
    hal_pmu_lpm_system_bypass_bt_status(on);
}

void pmu_lpm_system_bypass_gnss_status(switch_type_t on)
{
    hal_pmu_lpm_system_bypass_gnss_status(on);
}

uint16_t pmu_lpm_audio_wakeup_get_status(lpm_action_type_t type)
{
    return hal_pmu_lpm_audio_wakeup_get_status(type);
}

void pmu_lpm_wakeup_action_all_enable(lpm_action_type_t type, switch_type_t on)
{
    hal_pmu_lpm_wakeup_action_all_enable(type, on);
}

void pmu_lpm_sleep_action_all_enable(lpm_action_type_t type, switch_type_t on)
{
    hal_pmu_lpm_sleep_action_all_enable(type, on);
}

void pmu_lpm_wakeup_all_status_clear(lpm_action_type_t type)
{
    hal_pmu_lpm_wakeup_all_status_clear(type);
}

uint16_t pmu_lpm_wakeup_get_all_status(lpm_action_type_t type)
{
    return hal_pmu_lpm_wakeup_get_all_status(type);
}

uint16_t pmu_lpm_wakeup_get_status(lpm_action_type_t type, uint16_t position)
{
    return hal_pmu_lpm_wakeup_get_status(type, position);
}

void pmu_lpm_sleep_all_status_clear(lpm_action_type_t type)
{
    hal_pmu_lpm_sleep_all_status_clear(type);
}

uint16_t pmu_lpm_sleep_get_all_status(lpm_action_type_t type)
{
    return hal_pmu_lpm_sleep_get_all_status(type);
}

void pmu_lpm_make_sure_all_status_clear(lpm_type_t lpm_type, lpm_action_type_t action_type)
{
    hal_pmu_lpm_make_sure_all_status_clear(lpm_type, action_type);
}

void pmu_lpm_sdio_evt_enable_init(void)
{
    hal_pmu_lpm_wakeup_action_enable(LPM_EVT_ACTION, TURN_ON, PMU_LPM_MCPU_SDIO_WAKEUP);
}

void pmu_lpm_ulp_gpio_evt_enable_init(void)
{
    hal_pmu_lpm_wakeup_action_enable(LPM_EVT_ACTION, TURN_ON, PMU_LPM_MCPU_ULP_GPIO_WAKEUP);
}

void pmu_lpm_before_enter_wfi_handle(void)
{
    /* disable and clear wakeup events and interrupts. */
    pmu_lpm_wakeup_action_all_enable(LPM_EVT_ACTION, TURN_OFF);
    pmu_lpm_wakeup_all_status_clear(LPM_EVT_ACTION);
    pmu_lpm_wakeup_action_all_enable(LPM_INT_ACTION, TURN_OFF);
    pmu_lpm_wakeup_all_status_clear(LPM_INT_ACTION);

    /* disable and clear sleep events and interrupts. */
    pmu_lpm_sleep_action_all_enable(LPM_EVT_ACTION, TURN_OFF);
    pmu_lpm_sleep_all_status_clear(LPM_EVT_ACTION);
    pmu_lpm_sleep_action_all_enable(LPM_INT_ACTION, TURN_OFF);
    pmu_lpm_sleep_all_status_clear(LPM_INT_ACTION);

    hal_pmu_lpm_audio_wakeup_enable(LPM_EVT_ACTION, TURN_OFF);

    /* Make sure status clear */
    pmu_lpm_make_sure_all_status_clear(LPM_TYPE_WAKEUP, LPM_EVT_ACTION);
    pmu_lpm_make_sure_all_status_clear(LPM_TYPE_SLEEP, LPM_EVT_ACTION);

    hal_pmu_lpm_audio_wakeup_status_clear(LPM_EVT_ACTION);
    pmu_lpm_make_sure_all_status_clear(LPM_TYPE_WAKEUP, LPM_INT_ACTION);
#ifdef ENABLE_LPC_INT
    pmu_lpm_make_sure_all_status_clear(LPM_TYPE_SLEEP, LPM_INT_ACTION);
#endif
}

void pmu_lpm_before_enter_deep_sleep_handle(void)
{
    pmu_lpm_before_enter_wfi_handle();

    /* enable all sleep events,for can be enter sleep after WFI. */
    pmu_lpm_sleep_action_all_enable(LPM_EVT_ACTION, TURN_ON);
    /* enable all wakeup events,for can be wakeup. */
    pmu_lpm_wakeup_action_all_enable(LPM_EVT_ACTION, TURN_ON);
    hal_pmu_lpm_audio_wakeup_enable(LPM_EVT_ACTION, TURN_ON);

#ifdef ENABLE_LPC_INT
    /* enable all sleep int, for can be enter sleep after WFI. */
    pmu_lpm_sleep_action_all_enable(LPM_INT_ACTION, TURN_ON);
#endif
    /* enable all wakeup int, for can be wakeup. */
    pmu_lpm_wakeup_action_all_enable(LPM_INT_ACTION, TURN_ON);
}


bool pmu_msystem_sleep_history(void)
{
    return hal_pmu_lpm_msystem_sleep_history();
}

void pmu_msystem_sleep_history_clear(void)
{
    hal_pmu_lpm_msystem_sleep_history_clear();
}

static void pmu_platform_wakeup_handler(void)
{
    uint32_t irq_sts = osal_irq_lock();

    /* disable allow system to sleep */
    pmu_mcpu_allow_to_sleep(TURN_OFF);

    /* disable and clear wakeup evt/int */
    pmu_lpm_wakeup_action_all_enable(LPM_INT_ACTION, TURN_OFF);
    pmu_lpm_wakeup_all_status_clear(LPM_INT_ACTION);
    pmu_lpm_wakeup_action_all_enable(LPM_EVT_ACTION, TURN_OFF);
    pmu_lpm_wakeup_all_status_clear(LPM_EVT_ACTION);

    /* disable and clear sleep events and interrupts,for protcting wakeup don't interrupt be sleep */
    pmu_lpm_sleep_action_all_enable(LPM_INT_ACTION, TURN_OFF);
    pmu_lpm_sleep_all_status_clear(LPM_INT_ACTION);
    pmu_lpm_sleep_action_all_enable(LPM_EVT_ACTION, TURN_OFF);
    pmu_lpm_sleep_all_status_clear(LPM_EVT_ACTION);

    lpc_work_state_callback(LPC_ID_A2B, LPC_CONFIGURATION_WORK);
    osal_irq_restore(irq_sts);
}

static void pmu_platform_sleep_handler(void)
{
    /* disable sleep intrrupt and clear interrupt status */
    pmu_lpm_sleep_action_all_enable(LPM_INT_ACTION, TURN_OFF);
    pmu_lpm_sleep_all_status_clear(LPM_INT_ACTION);

    lpc_work_state_callback(LPC_ID_A2B, LPC_CONFIGURATION_DEEP_SLEEP);
    lpc_work_state_callback(LPC_ID_PLT, LPC_CONFIGURATION_DEEP_SLEEP);

    /* allow system to sleep */
    pmu_mcpu_allow_to_sleep(TURN_ON);
    pmu_lpm_sleep_all_status_clear(LPM_INT_ACTION); // Clear mcpu slp int
    while (pmu_lpm_sleep_get_all_status(LPM_INT_ACTION) != 0) { }
}
#ifdef BUILD_APPLICATION_SSB
__IRQ void pmu_wakeup_handler(void)
{
    uint16_t interrupt_id;
    uint32_t irq_sts = osal_irq_lock();
    interrupt_id = pmu_lpm_wakeup_get_all_status(LPM_INT_ACTION);
    if (interrupt_id == 0) {
        osal_irq_restore(irq_sts);
        return;
    }
#if defined(BUILD_APPLICATION_STANDARD)
    oml_pf_log_print1(LOG_BCORE_PLT_DRIVER_PMU, LOG_NUM_DRIVER_LPC, LOG_LEVEL_INFO,
                      "M core wakeup interrupt is: 0x%X", interrupt_id);
#endif
    pmu_platform_wakeup_handler();
    osal_irq_restore(irq_sts);
}
#else
__IRQ void pmu_wakeup_handler(void)
{
    uint16_t interrupt_id;
    lpc_forbid_deepsleep();
    PMU_SLEEP_TRACE_SEL_TAG(PMU_SLEEP_SEL_INT);
    interrupt_id = pmu_lpm_wakeup_get_all_status(LPM_INT_ACTION);
    if (interrupt_id == 0) {
        return;
    }
    pm_core_resume();
    osal_irq_disable(M_WAKEUP_IRQN);
    PRINT("M core wakeup interrupt is 0x%x"NEWLINE, interrupt_id);
#if defined(BUILD_APPLICATION_STANDARD)
    oml_pf_log_print1(LOG_BCORE_PLT_DRIVER_PMU, LOG_NUM_DRIVER_LPC, LOG_LEVEL_INFO,
                      "M core wakeup interrupt is: 0x%X", interrupt_id);
#endif
    pmu_platform_wakeup_handler();
}
#endif
__IRQ void pmu_sleep_handler(void)
{
    uint16_t interrupt_id;
    uint32_t irq_sts = osal_irq_lock();
    if (lpc_get_sleep_veto() > 0) {
        osal_irq_restore(irq_sts);
        return;
    }

    interrupt_id = pmu_lpm_sleep_get_all_status(LPM_INT_ACTION);
    if (interrupt_id == 0) {
        osal_irq_restore(irq_sts);
        return;
    }

    pmu_platform_sleep_handler();

    osal_irq_restore(irq_sts);
}

void pmu_cmu_interrupt_config(void)
{
    if (pmu_cmu_get_pll_status()) {
        pmu_pll_unlock_interrupt_enable();
    }
}

#ifdef USE_CMSIS_OS
static void pmu_cmu_interrupt_handler(void)
{
    hal_pmu_cmu_clear_ana_int_status();

    while (hal_pmu_cmu_get_ana_status() != 0) {
        hal_pmu_cmu_clear_ana_int_status();
    }

    uint64_t time = uapi_tcxo_get_ms();
    uint64_t check_frq = CHECK_FREQUENCY;
    while (1) { //lint !e716
        if (hal_pmu_cmu_get_ana_status() != 0) {
            // Clear interrupt status
            hal_pmu_cmu_clear_ana_int_status();
        } else {
            return;
        }

        check_frq--;
        if (check_frq == 0) {
            if ((time + PMU_LDO_TIMEOUT) < uapi_tcxo_get_ms()) {
                uint16_t ana_status = hal_pmu_cmu_get_ana_status();
                PRINT("[LOW_POWER], ana_status:0x%x\r\n", ana_status);
#ifdef BUILD_APPLICATION_STANDARD
                oml_pf_log_print1(LOG_BCORE_PLT_LPM_SWITCH_CLK, LOG_NUM_LPM_SWITCH_CLK, LOG_LEVEL_ERROR, \
                    "[LOW_POWER], ana_status:0x%x\r\n", ana_status);
#endif
                uint32_t error_code = ((ana_status << SHIFT_16) | PMU_LDO_UNLOCK_CLEAR_PANIC);
                panic(PANIC_PMU_LDO, error_code);
            } else {
                check_frq = CHECK_FREQUENCY;
            }
        }
    }
}
#endif

void pmu_init_interrupts(void)
{
    // Register and enable mcpu core sleep interrupts only
    osal_irq_request(M_SLEEP_IRQN, (osal_irq_handler)pmu_sleep_handler, NULL, NULL, NULL);
    osal_irq_disable(M_SLEEP_IRQN);

    // Register and enable mcpu core wakeup interrupts only
    osal_irq_request(M_WAKEUP_IRQN, (osal_irq_handler)pmu_wakeup_handler, NULL, NULL, NULL);
    osal_irq_disable(M_WAKEUP_IRQN);

#ifdef USE_CMSIS_OS
    // Register and enable pmu pll unlock status interrupts
    osal_irq_request(PMU_CMU_ERR_IRQN, (osal_irq_handler)pmu_cmu_interrupt_handler, NULL, NULL, NULL);
    osal_irq_enable(PMU_CMU_ERR_IRQN);
#endif
}

void pmu_deinit_interrupts(void)
{
    osal_irq_disable(M_SLEEP_IRQN);
    osal_irq_free(M_SLEEP_IRQN, NULL);

    osal_irq_disable(M_WAKEUP_IRQN);
    osal_irq_free(M_WAKEUP_IRQN, NULL);
#ifdef USE_CMSIS_OS
    osal_irq_disable(PMU_CMU_ERR_IRQN);
    osal_irq_free(PMU_CMU_ERR_IRQN, NULL);
#endif
}

void pmu_lpm_reg_config_delay(void)
{
    uapi_tcxo_delay_us(PMU_LPM_REG_DELAY);
}
