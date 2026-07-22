/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description: PMU INTERRUPT HEADER
 */

#ifndef NON_OS_APPLICATION_CORE_PMU_INTERRUPT_H
#define NON_OS_APPLICATION_CORE_PMU_INTERRUPT_H

#include <stdint.h>
#include "chip_definitions.h"
#include "platform_core.h"
#include "hal_pmu_interrupt.h"

/** @defgroup connectivity_drivers_non_os_pmu_interrupt PMU Interrupt
  * @ingroup  connectivity_drivers_non_os_pmu
  * @{
  */
/**
 * @brief  System allow mcpu to sleep.
 * @param  on           turn on/turn off.
 */
void pmu_mcpu_allow_to_sleep(switch_type_t on);

/**
 * @brief  System bypass BT core status, only judge the M core state.
 * @param  on           Bypass bt core/Do not bypass bt.
 */
void pmu_lpm_system_bypass_bt_status(switch_type_t on);

/**
 * @brief  System bypass GNSS core status, only judge the M core state.
 * @param  on           Bypass gnss core/Do not bypass gnss.
 */
void pmu_lpm_system_bypass_gnss_status(switch_type_t on);

/**
 * @brief  Set PMU_LPM_SLP_XXX_EN_REG, for enable/disable all interrupts on bit(x).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @param  on           turn on/turn off.
 */
void pmu_lpm_sleep_action_all_enable(lpm_action_type_t type, switch_type_t on);

/**
 * @brief  Set PMU_LPM_WKUP_XXX_EN_REG, for enable/disable all interrupts on bit(x).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @param  on           turn on/turn off.
 */
void pmu_lpm_wakeup_action_all_enable(lpm_action_type_t type, switch_type_t on);

/**
 * @brief  Clear PMU_LPM_WKUP_XXX_CLR_REG, for clear all status flag on action type.
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 */
void pmu_lpm_wakeup_all_status_clear(lpm_action_type_t type);

/**
 * @brief  Read PMU_LPM_WKUP_XXX_STS_REG, get the status form bit(x) indecate that.
 * @param  position     loaction of bit(x), position form bit(0).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @return Wakeup status, bit(x) ture: on, bit(x)false: off.
 */
uint16_t pmu_lpm_wakeup_get_status(lpm_action_type_t type, uint16_t position);

/**
 * @brief  Read PMU_LPM_WKUP_XXX_STS_REG, get all the status form bit(x) indecate that.
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @return All wakeup status, bit(x) ture: on, bit(x)false: off.
 */
uint16_t pmu_lpm_wakeup_get_all_status(lpm_action_type_t type);

/**
 * @brief  Clear PMU_LPM_SLP_INT_CLR_REG, for clear all status flag on action type.
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 */
void pmu_lpm_sleep_all_status_clear(lpm_action_type_t type);

/**
 * @brief  Read PMU_LPM_SLP_XXX_STS_REG, get all the status form bit(x) indecate that.
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @return Sleep status, bit(x) ture: on, bit(x)false: off.
 */
uint16_t pmu_lpm_sleep_get_all_status(lpm_action_type_t type);

/**
 * @brief  Read back reg value, makesure status reg is 0.
 * @param  lpm_type         Select Wake_up/Sleep register.
 * @param  action_type      Select event/interrupt status.
 */
void pmu_lpm_make_sure_all_status_clear(lpm_type_t lpm_type, lpm_action_type_t action_type);

/**
 * @brief  Enable SDIO slave sleep/wakeup event.
 */
void pmu_lpm_sdio_evt_enable_init(void);

/**
 * @brief  Enable ulp gpio sleep/wakeup event.
 */
void pmu_lpm_ulp_gpio_evt_enable_init(void);

/**
 * @brief  Clear wake_up/sleep evt/int status.
 */
void pmu_lpm_before_enter_wfi_handle(void);

/**
 * @brief  Enable wake_up/sleep evt/int.
 */
void pmu_lpm_before_enter_deep_sleep_handle(void);

/**
 * @brief  Get msystem sleep history.
 * @return Sleep history, true indicates that sleep has occurred, false indicates that sleep has not occurred.
 */
bool pmu_msystem_sleep_history(void);

/**
 * @brief  Clear msystem sleep history.
 */
void pmu_msystem_sleep_history_clear(void);

/**
 * @brief  Wakeup interrupt handler, deal with that restore clk,vol,register,glb value.
 */
void pmu_wakeup_handler(void);

/**
 * @brief  Sleep interrupt handler, deal with tha clk gating,vol buck,register/glb value preserve.
 */
void pmu_sleep_handler(void);

/**
 * @brief  Pmu interrupts init.
 */
void pmu_cmu_interrupt_config(void);

/**
 * @brief  Pmu interrupts init, register interrupt,set priority, enable interrupt,and so on.
 */
void pmu_init_interrupts(void);

/**
 * @brief  Pmu interrupts deinit, delete interrupt register, disable interrupt,and so on.
 */
void pmu_deinit_interrupts(void);

/**
 * @brief  Waiting time, make sure reg value set success.
 */
void pmu_lpm_reg_config_delay(void);

/**
 * @brief  Get audio wakeup interrupt status.
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @return Event/Interruption wakeup status.
 */
uint16_t pmu_lpm_audio_wakeup_get_status(lpm_action_type_t type);

/**
  * @}
  */
#endif
