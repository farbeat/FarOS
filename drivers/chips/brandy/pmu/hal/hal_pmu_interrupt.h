/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: PMU INTERRUPT FUNCTIONS
 */

#ifndef SRC_DRIVERS_HAL_APPLICATION_HAL_PMU_INTERRUPT_H
#define SRC_DRIVERS_HAL_APPLICATION_HAL_PMU_INTERRUPT_H

#include <stdint.h>
#include "chip_io.h"
/** @defgroup connectivity_drivers_hal_pmu_interrupt PMU INTERRUPT
  * @ingroup  connectivity_drivers_hal_pmu
  * @{
  */
/** PMU_LPM wakeup types */
typedef enum {
    LPM_EVT_ACTION,
    LPM_INT_ACTION,
} lpm_action_type_t;

typedef enum {
    LPM_TYPE_WAKEUP,
    LPM_TYPE_SLEEP,
} lpm_type_t;

/* MCPU LPM_XXX_WAKEUP BIT position */
#define PMU_LPM_MCPU_DSP_GPIO_INT_WAKEUP    15
#define PMU_LPM_MCPU_CWDT_INT_WAKEUP        14
#define PMU_LPM_MCPU_ULP_WAKEUP             13
#define PMU_LPM_MCPU_CMU_OCP_WAKEUP         12
#define PMU_LPM_MCPU_DAP_WAKEUP             11
#define PMU_LPM_MCPU_SSI_WAKEUP             10
#define PMU_LPM_MCPU_GPIO_WAKEUP            9
#define PMU_LPM_MCPU_M_RTC_WAKEUP           8
#define PMU_LPM_MCPU_D_RTC_WAKEUP           7
#define PMU_LPM_MCPU_UART_H1_RX_WAKEUP      6
#define PMU_LPM_MCPU_UART_H0_RX_WAKEUP      5
#define PMU_LPM_MCPU_UART_L0_RX_WAKEUP      4
#define PMU_LPM_MCPU_B2D_IPC_WAKEUP         3
#define PMU_LPM_MCPU_D2M_IPC_WAKEUP         2
#define PMU_LPM_MCPU_B2M_IPC_WAKEUP         1
#define PMU_LPM_MCPU_AON_CAPSENS_WAKEUP     0

#define PMU_LPM_MCPU_WKAEUP_MASK_LEN        (PMU_LPM_MCPU_DSP_GPIO_INT_WAKEUP - PMU_LPM_MCPU_AON_CAPSENS_WAKEUP + 1)


#define PMU_LPM_MCPU_WKUP_ALL_MASK          0xFFFF
#define PMU_LPM_MCPU_WKUP_MASK              (BIT(PMU_LPM_MCPU_CWDT_INT_WAKEUP)  | \
                                            BIT(PMU_LPM_MCPU_DAP_WAKEUP)        | \
                                            BIT(PMU_LPM_MCPU_SSI_WAKEUP)        | \
                                            BIT(PMU_LPM_MCPU_GPIO_WAKEUP)       | \
                                            BIT(PMU_LPM_MCPU_M_RTC_WAKEUP)      | \
                                            BIT(PMU_LPM_MCPU_UART_H1_RX_WAKEUP) | \
                                            BIT(PMU_LPM_MCPU_UART_H0_RX_WAKEUP) | \
                                            BIT(PMU_LPM_MCPU_UART_L0_RX_WAKEUP) | \
                                            BIT(PMU_LPM_MCPU_D2M_IPC_WAKEUP)    | \
                                            BIT(PMU_LPM_MCPU_B2M_IPC_WAKEUP)    | \
                                            BIT(PMU_LPM_MCPU_AON_CAPSENS_WAKEUP))
/**
 * @brief  Mask audio vpu wakup interrupt.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_audio_vpu_interrupt_mask(switch_type_t on);

/**
 * @brief  Mask audio mad wakup interrupt.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_audio_mad_interrupt_mask(switch_type_t on);

/**
 * @brief  Mask mcpu rtc wakup evt register bit, bit(x) == 0:turn off, bit(x) == 1:turn on by manual.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_mrtc_wakeup_interrupt_mask(switch_type_t on);

/**
 * @brief  Mask dsp rtc wakup evt register bit, bit(x) == 0:turn off, bit(x) == 1:turn on by manual.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_drtc_wakeup_interrupt_mask(switch_type_t on);

/**
 * @brief  Mask dsp0 to mcpu ipc wakup evt register bit, bit(x) == 0:turn off, bit(x) == 1:turn on by manual.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_d2m_0_ipc_wakeup_interrupt_mask(switch_type_t on);

/**
 * @brief  Mask dsp1 to mcpu wakup evt register bit, bit(x) == 0:turn off, bit(x) == 1:turn on by manual.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_d2m_1_ipc_wakeup_interrupt_mask(switch_type_t on);

/**
 * @brief  Mask bt to dsp0 wakup evt register bit, bit(x) == 0:turn off, bit(x) == 1:turn on by manual.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_b2d_0_ipc_wakeup_interrupt_mask(switch_type_t on);

/**
 * @brief  Mask bt to dsp1 wakup evt register bit, bit(x) == 0:turn off, bit(x) == 1:turn on by manual.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_b2d_1_ipc_wakeup_interrupt_mask(switch_type_t on);

/**
 * @brief  Mask b2m_ipc wakup evt register bit, bit(x) == 0:turn off, bit(x) == 1:turn on by manual.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_b2m_ipc_wakeup_interrupt_mask(switch_type_t on);

/**
 * @brief  Mask gpio wakup evt register bit, bit(x) == 0:turn off, bit(x) == 1:turn on by manual.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_gpio_wakeup_interrupt_mask(switch_type_t on);

/**
 * @brief  System allow mcpu to sleep.
 * @param  on           turn on/turn off
 */
void hal_pmu_lpm_mcpu_allow_to_sleep(switch_type_t on);

/**
 * @brief  Clear PMU_LPM_WKUP_XXX_CLR_REG, for clear status flag on action type and position.
 * @param  position     loaction of bit(x), position form bit(0).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 */
void hal_pmu_lpm_wakeup_status_clear(lpm_action_type_t type, uint16_t position);

/**
 * @brief  Clear PMU_LPM_WKUP_XXX_CLR_REG, for clear all status flag on action type.
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 */
void hal_pmu_lpm_wakeup_all_status_clear(lpm_action_type_t type);

/**
 * @brief  Set PMU_LPM_WKUP_XXX_EN_REG, for enable/disable interrupt on bit(x).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @param  on           turn on/turn off.
 * @param  position     loaction of bit(x), position form bit(0).
 */
void hal_pmu_lpm_wakeup_action_enable(lpm_action_type_t type, switch_type_t on, uint16_t position);

/**
 * @brief  Set PMU_LPM_WKUP_XXX_EN_REG, for enable/disable all interrupts on bit(x).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_wakeup_action_all_enable(lpm_action_type_t type, switch_type_t on);

/**
 * @brief  Init PMU_LPM_WKUP_XXX_EN_REG, for enable/disable all interrupts on bit(x).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 */
void hal_pmu_lpm_wakeup_action_all_init(lpm_action_type_t type);

/**
 * @brief  Read PMU_LPM_WKUP_XXX_STS_REG, Get the status form bit(x) indecate that.
 * @param  position     loaction of bit(x), position form bit(0).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @return Event or interrupt wakeup status, bit(x) ture: on, bit(x)false: off.
 */
uint16_t hal_pmu_lpm_wakeup_get_status(lpm_action_type_t type, uint16_t position);

/**
 * @brief  Read PMU_LPM_WKUP_XXX_STS_REG,Get all the status form bit(x) indecate that.
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @return Event or interrupt all wakeup status, bit(x) ture: on,bit(x)false: off.
 */
uint16_t hal_pmu_lpm_wakeup_get_all_status(lpm_action_type_t type);

/**
 * @brief  Read back reg value, makesure status reg is 0.
 * @param  lpm_type         Select Wake_up/Sleep register.
 * @param  action_type      Select event/interrupt status.
 */
void hal_pmu_lpm_make_sure_all_status_clear(lpm_type_t lpm_type, lpm_action_type_t action_type);

/**
 * @brief  Clear PMU_LPM_SLP_INT_CLR_REG, for clear status flag on action type and position.
 * @param  position     loaction of bit(x), position form bit(0).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 */
void hal_pmu_lpm_sleep_status_clear(lpm_action_type_t type, uint16_t position);

/**
 * @brief  Clear PMU_LPM_SLP_INT_CLR_REG, for clear all status flag on action type.
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 */
void hal_pmu_lpm_sleep_all_status_clear(lpm_action_type_t type);

/**
 * @brief  Read PMU_LPM_SLP_XXX_STS_REG, Get the status form bit(x) indecate that.
 * @param  position     loaction of bit(x), position form bit(0).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @return Event or interrupt sleep status, bit(x) ture: on, bit(x)false: off.
 */
uint16_t hal_pmu_lpm_sleep_get_status(lpm_action_type_t type, uint16_t position);

/**
 * @brief  Read PMU_LPM_SLP_XXX_STS_REG,Get all the status form bit(x) indecate that.
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @return Event or interrupt all sleep status, bit(x) ture: on,bit(x)false: off.
 */
uint16_t hal_pmu_lpm_sleep_get_all_status(lpm_action_type_t type);

/**
 * @brief  Set PMU_LPM_SLP_XXX_EN_REG, for enable/disable interrupt on bit(x).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @param  on           turn on/turn off.
 * @param  position     loaction of bit(x), position form bit(0).
 */
void hal_pmu_lpm_sleep_action_enable(lpm_action_type_t type, switch_type_t on, uint16_t position);

/**
 * @brief  Set PMU_LPM_SLP_XXX_EN_REG,for enable/disable all interrupts on bit(x).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_sleep_action_all_enable(lpm_action_type_t type, switch_type_t on);

/**
 * @brief  Init PMU_LPM_SLP_XXX_EN_REG,for enable/disable all interrupts on bit(x).
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 */
void hal_pmu_lpm_sleep_action_all_init(lpm_action_type_t type);

/**
 * @brief  Get msystem sleep history.
 * @return true indicates that sleep has occurred, and false indicates that sleep has not occurred.
 */
bool hal_pmu_lpm_msystem_sleep_history(void);

/**
 * @brief  Clear msystem sleep history.
 */
void hal_pmu_lpm_msystem_sleep_history_clear(void);

/**
 * @brief  System bypass BT core status, only judge the M core state.
 * @param  on           Bypass bt core / Do not bypass bt.
 */
void hal_pmu_lpm_system_bypass_bt_status(switch_type_t on);

/**
 * @brief  Audio allow system to sleep.
 * @param  on   turn on/turn off.
 */
void hal_pmu_lpm_audio_allow_to_sleep(switch_type_t on);

/**
 * @brief  Get allow system to sleep.
 * @param  on   turn on/turn off.
 */
switch_type_t hal_pmu_lpm_get_mcpu_allow_to_sleep(void);

/**
 * @brief  Clear audio wakeup interrupt status.
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 */
void hal_pmu_lpm_audio_wakeup_status_clear(lpm_action_type_t type);

/**
 * @brief  Set audio enable/disable wakeup interrupt.
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @param  on           turn on/turn off.
 */
void hal_pmu_lpm_audio_wakeup_enable(lpm_action_type_t type, switch_type_t on);

/**
 * @brief  Get audio wakeup interrupt status.
 * @param  type         LPM_EVT_ACTION/LPM_INT_ACTION.
 * @return Whether the wakeup event/interrupt occurs.
 */
uint16_t hal_pmu_lpm_audio_wakeup_get_status(lpm_action_type_t type);

/**
  * @}
  */
#endif
