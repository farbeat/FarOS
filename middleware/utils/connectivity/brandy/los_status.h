/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: los status header
 * Author:
 * Create:
 */
#ifndef LIB_PUBLIC_LOS_STATUS_H
#define LIB_PUBLIC_LOS_STATUS_H

#include <stdint.h>
#include "chip_core_definition.h"
#include "lpc.h"
#ifdef BUILD_APPLICATION_STANDARD
#ifdef __LITEOS__
#include "los_cpup.h"
#endif
#endif

/**
 * @defgroup connectivity_libs_connectivity_los_status CONNECTIVITY Los Status
 * @ingroup  connectivity_libs_connectivity
 * @{
 */
typedef struct {
    uint32_t system_wakeup_m2b_ipc;
    uint32_t system_wakeup_d2b_ipc;
    uint32_t system_wakeup_b_gpio;
    uint32_t system_wakeup_bt_osc_en;
    uint32_t system_wakeup_uart_l1;
    uint32_t system_wakeup_b_rtc;
    uint32_t system_wakeup_b_ssi;
    uint32_t system_wakeup_b_dap;
} system_wakeup_source_t;

#if CORE == BT
typedef enum {
    BCPU_M2B_IPC_WAKEUP     = 0,
    BCPU_D2B_IPC_WAKEUP     = 1,
    BCPU_GPIO_WAKEUP        = 2,
    BCPU_OSC_EN_WAKEUP      = 3,
    BCPU_UART_L1_RX_WAKEUP  = 4,
    BCPU_RTC_WAKEUP         = 5,
    BCPU_SSI_WAKEUP         = 6,
    BCPU_DAP_WAKEUP         = 7,
    WAKEUP_SOURCE_MAX,
} wakeup_source_e;
#else
typedef enum {
    MCPU_AON_CAPSENS_WAKEUP  = 0,
    MCPU_B2M_IPC_WAKEUP      = 1,
    MCPU_D2M_IPC_WAKEUP      = 2,
    MCPU_B2D_IPC_WAKEUP      = 3,
    MCPU_UART_L0_RX_WAKEUP   = 4,
    MCPU_UART_H0_RX_WAKEUP   = 5,
    MCPU_UART_H1_RX_WAKEUP   = 6,
    MCPU_D_RTC_WAKEUP        = 7,
    MCPU_M_RTC_WAKEUP        = 8,
    MCPU_GPIO_WAKEUP         = 9,
    MCPU_SSI_WAKEUP          = 10,
    MCPU_DAP_WAKEUP          = 11,
    MCPU_SDIO_WAKEUP         = 12,
    MCPU_ULP_GPIO_WAKEUP     = 13,
    MCPU_CWDT_INT_WAKEUP     = 14,
    MCPU_DSP_GPIO_INT_WAKEUP = 15,
    MCPU_AUDIO_WAKEUP        = 16,
    WAKEUP_SOURCE_MAX,
} wakeup_source_e;

typedef struct {
    uint32_t m_wakeup_cap;
    uint32_t m_wakeup_b2m_ipc;
    uint32_t m_wakeup_d2m_ipc;
    uint32_t m_wakeup_b2d_ipc;
    uint32_t m_wakeup_uart_l0;
    uint32_t m_wakeup_uart_h0;
    uint32_t m_wakeup_uart_h1;
    uint32_t m_wakeup_d_rtc;
    uint32_t m_wakeup_rtc;
    uint32_t m_wakeup_gpio;
    uint32_t m_wakeup_ssi;
    uint32_t m_wakeup_dap;
    uint32_t m_wakeup_sdio;
    uint32_t m_wakeup_ulp_gpio;
    uint32_t m_wakeup_cwdt;
    uint32_t m_wakeup_dsp_gpio;
    uint32_t m_wakeup_audio;
} m_wakeup_source;
#endif

/**
 * @brief  Record system start time
 */
void los_system_start_up_init(void);

/**
 * @brief  Return system runtime
 */
uint64_t los_system_system_run_time(void);

/**
 * @brief  Record the time the system enters deep sleep
 */
void los_status_enter_sleep_time(void);

/**
 * @brief  Get system deep sleep flag
 * @return true is system entered deep sleep.
 */
bool los_get_enter_deepsleep_flag(void);

/**
 * @brief  Plt get system deep sleep flag.
 */
bool los_plt_enter_deepsleep_flag(void);

/**
 * @brief  Record the time the system enters light sleep
 * @param  sleep_mode current sleep mode
 */
void los_status_exit_sleep_time(lpc_sleep_mode_e sleep_mode);

/**
 * @brief  Clear previous sleep time
 */
void los_status_sleep_time_clear(void);

/**
 * @brief  Statistics number of wakeups for each wakeup source
 * @param  interrupt_id ID of the interrupt
 */
void los_status_wakeup_source_statistics(uint16_t interrupt_id);

#if CORE == BT
/**
 * @brief  Return bt core to deep sleep time
 */
uint32_t los_bt_core_deep_sleep_time(void);

/**
 * @brief  Return bt core to light sleep time
 */
uint32_t los_bt_core_light_sleep_time(void);

/**
 * @brief  Get current core statistics number of wakeups for each wakeup source
 * @return current core statistics number of wakeups for each wakeup source
 */
system_wakeup_source_t* los_status_get_wakeup_source_statistics(void);

#endif

#if CORE == APPS
/**
 * @brief  Return m core to deep sleep time
 */
uint32_t los_m_core_deep_sleep_time(void);

/**
 * @brief  Return m core to light sleep time
 */
uint32_t los_m_core_light_sleep_time(void);

/**
 * @brief  Get system and bt core deep sleep time
 * @param  system_sleep_time system sleep time
 * @param  bt_core_sleep_time bt core sleep time
 * @return Get count result success or fail
 */
bool los_get_system_sleep_count(uint32_t *system_sleep_time, uint32_t *bt_core_sleep_time);

/**
 * @brief  Get system wakeup source statistics
 * @param  system_wakeup_source wakeup source statistics
 * @return Get success or fail
 */
bool get_system_wakeup_source(system_wakeup_source_t* system_wakeup_source);

/**
 * @brief  Get current core statistics number of wakeups for each wakeup source
 * @param  m_core_wakeup_source current core statistics number of wakeups for each wakeup source
 * @return Get success or fail
 */
bool los_status_get_wakeup_source_statistics(m_wakeup_source* m_core_wakeup_source);
#endif

/**
 * @}
 */
#endif
