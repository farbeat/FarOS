/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  PMU DRIVER Header
 */

#ifndef HAL_PMU_PG_H
#define HAL_PMU_PG_H

/** @defgroup connectivity_drivers_hal_pmu_pg PMU PAGE GATING
  * @ingroup  connectivity_drivers_hal_pmu
  * @{
  */
#include <stdbool.h>
#include "chip_definitions.h"

/**
 * @brief  Pmu1 digital gating sub system list.
 */
typedef enum {
    /* Common region. */
    PMU1_DIG_PG_B_SOC,          /*!< B_SOC. */
    PMU1_DIG_PG_BT_CBB,         /*!< BT_CBB. */
    PMU1_DIG_PG_B_MEM,          /*!< B_MEM. */
    PMU1_DIG_PG_BT_EM,          /*!< BT_EM. */
    PMU1_DIG_PG_MCU_HS,         /*!< MCU_HS. */
    PMU1_DIG_PG_DSUB,           /*!< DSUB. */
    PMU1_DIG_PG_ASUB,           /*!< ASUB. */
    PMU1_DIG_PG_S_EGPIO,        /*!< S_EGPIO. */
    PMU1_DIG_PG_BT_RF,          /*!< BT_RF. */
    PMU1_DIG_PG_MCU,            /*!< MCU. */
    PMU1_DIG_PG_MEM,            /*!< MEM. */
    PMU1_DIG_PG_COM,            /*!< COM. */
    /* Auto PG region. */
    PMU1_DIG_AUTO_PG_MCU,       /*!< AUTO MCU. */
    PMU1_DIG_AUTO_PG_BSOC,      /*!< AUTO BSOC. */
    PMU1_DIG_AUTO_PG_COM,       /*!< AUTO COM. */
    PMU1_DIG_AUTO_PG_MEM,       /*!< AUTO MEM. */
    PMU1_DIG_AUTO_PG_BMEM,      /*!< AUTO BMEM. */
    PMU1_DIG_PG_MAX
} pmu1_dig_pg_sub_sys_t;

/**
 * @brief  L2ram allocated to dsp space.
 */
typedef enum {
    L2MEM_SHARE_0K_FOR_DSP,       //!< L2RAM 0KB FOR DSP, 2048KB FOR MCU
    L2MEM_SHARE_128K_FOR_DSP,     //!< L2RAM 128KB FOR DSP, 1920KB FOR MCU
    L2MEM_SHARE_256K_FOR_DSP,     //!< L2RAM 256KB FOR DSP, 1792KB FOR MCU
    L2MEM_SHARE_384K_FOR_DSP,     //!< L2RAM 384KB FOR DSP, 1664KB FOR MCU
    L2MEM_SHARE_512K_FOR_DSP,     //!< L2RAM 512KB FOR DSP, 1536KB FOR MCU
    L2MEM_SHARE_640K_FOR_DSP,     //!< L2RAM 640KB FOR DSP, 1408KB FOR MCU
    L2MEM_SHARE_768K_FOR_DSP,     //!< L2RAM 768KB FOR DSP, 1280KB FOR MCU
    L2MEM_SHARE_896K_FOR_DSP,     //!< L2RAM 896KB FOR DSP, 1152KB FOR MCU
    L2MEM_SHARE_1024K_FOR_DSP,    //!< L2RAM 1024KB FOR DSP, 1024KB FOR MCU
    L2MEM_SHARE_1152K_FOR_DSP,    //!< L2RAM 1152KB FOR DSP, 896KB FOR MCU
    L2MEM_SHARE_1280K_FOR_DSP,    //!< L2RAM 1280KB FOR DSP, 768KB FOR MCU
    L2MEM_SHARE_MAX_FOR_DSP,      //!< L2RAM SHARE BETWEEN MCU & DSP TOTAL MEMORY SIZE: 1280KB
} l2m_dsp_memory_share_mode_t;

/**
 * @brief  L2ram dsp power ctrl.
 */
typedef enum {
    HAL_U0_1_L2M_DSP_RAM,
    HAL_U2_3_L2M_DSP_RAM,
    HAL_U4_5_L2M_DSP_RAM,
    HAL_U6_7_L2M_DSP_RAM,
    HAL_U8_9_L2M_DSP_RAM,
    HAL_U10_11_L2M_DSP_RAM,
    HAL_U12_13_L2M_DSP_RAM,
    HAL_U14_15_L2M_DSP_RAM,
    HAL_U16_17_L2M_DSP_RAM,
    HAL_U18_19_L2M_DSP_RAM,
} hal_l2m_dsp_power_on_t;

typedef enum {
    BRAM_SHARE_0K_FOR_BT_DIAG       = 0,       //!< BTRAM 0KB FOR DIAG
    BRAM_SHARE_8_AND_9_FOR_BT_DIAG  = 1,       //!< BTRAM 64KB FOR DIAG
    BRAM_SHARE_6_AND_7_FOR_BT_DIAG  = 2,       //!< BTRAM 128KB FOR DIAG
    BRAM_SHARE_ALL_IN_FOR_BT_DIAG   = 3,       //!< BTRAM 192KB FOR DIAG
    BRAM_SHARE_MAX_FOR_BT_DIAG,
} bt_memory_share_mode_t;

/**
 * @brief  Enable or disable the ISO of sub system.
 * @note   This function can enable or disable the following sub-system's iso: <b>B_SOC, BT_CBB, B_MEM, BT_EM, MCU_HS,
 *         DSUB, ASUB, S_EGPIO, BT_RF, MEM, COM</b>.
 * @param  sys The sub-system which to enable or disable the iso.
 * @param  en Enable or disable.
 */
void hal_pmu1_dig_pg_common_iso_en(pmu1_dig_pg_sub_sys_t sys, bool en);

/**
 * @brief  Enable or disable the power of sub system.
 * @note   This function can enable or disable the following sub-system's power: <b>B_SOC, BT_CBB, B_MEM, BT_EM, MCU_HS,
 *         DSUB, ASUB, S_EGPIO, BT_RF, MEM, COM</b>.
 * @param  sys The sub-system which to enable or disable the power.
 * @param  en Enable or disable.
 */
void hal_pmu1_dig_pg_common_pwr_en(pmu1_dig_pg_sub_sys_t sys, bool en);

/**
 * @brief  Get the result of enable or disable the power of sub system.
 * @param  sys The sub-system which to get the status.
 * @return The status of enable the power.
 */
bool hal_pmu1_dig_pg_common_pwr_ack_get(pmu1_dig_pg_sub_sys_t sys);

/**
 * @brief  Auto enable or disable the power of sub system in sleep.
 * @note   This function can enable or disable the following sub-system's iso: <b>MCU, BSOC, COM, MEM, BMEM</b>.
 * @param  sys The sub-system which to auto enable or disable the iso.
 * @param  en Enable or disable.
 */
void hal_pmu1_dig_pg_auto_pwr_en(pmu1_dig_pg_sub_sys_t sys, bool en);

/**
 * @brief  Enable or disable the power of L2M gmmu.
 * @param  en Enable or disable.
 */
void hal_pmu1_dig_pg_l2m_gmmu_pwr_en(bool en);

/**
 * @brief  Enable or disable the retention1 status of L2M gmmu.
 * @param  en Enable or disable.
 */
void hal_pmu1_dig_pg_l2m_gmmu_ret1_en(bool en);

/**
 * @brief  Enable or disable the retention2 status of L2M gmmu.
 * @param  en Enable or disable.
 */
void hal_pmu1_dig_pg_l2m_gmmu_ret2_en(bool en);

/**
 * @brief  Get the result of enable or disable the power of L2M gmmu.
 * @return The status of enable the power.
 */
bool hal_pmu1_dig_pg_l2m_gmmu_pwr_ack_get(void);

/**
 * @brief  Enable or disable the power of L2M dsp.
 * @param  en Enable or disable.
 */
void hal_pmu1_dig_pg_l2m_dsp_pwr_en(bool en);

/**
 * @brief  Enable or disable the retention1 status of L2M dsp.
 * @param  en Enable or disable.
 */
void hal_pmu1_dig_pg_l2m_dsp_ret1_en(bool en);

/**
 * @brief  Enable or disable the retention2 status of L2M dsp.
 * @param  en Enable or disable.
 */
void hal_pmu1_dig_pg_l2m_dsp_ret2_en(bool en);

/**
 * @brief  Get the result of enable or disable the power of L2M dsp.
 * @return The status of enable the power.
 */
bool hal_pmu1_dig_pg_l2m_dsp_pwr_ack_get(void);

/**
 * @brief  Enable or disable the power of xip mem.
 * @param  en Enable or disable.
 */
void hal_pmu1_dig_pg_xip_mem_pwr_en(bool en);

/**
 * @brief  Enable or disable the retention of xip mem.
 * @param  en Enable or disable.
 */
void hal_pmu1_dig_pg_xip_mem_retention_en(bool en);

/**
 * @brief  Get the result of enable or disable the power of xip mem.
 * @return The status of enable the power.
 */
bool hal_pmu1_dig_pg_xip_mem_pwr_ack_get(void);

/**
 * @brief  Enable or disable the power of bram0-4.
 * @param  en Enable or disable.
 */
void hal_pmu1_dig_pg_bram_pwr_en(bool en);

/**
 * @brief  config dsp l2m share mode
 * @param  hal_memory_share_config Share mode config value.
 */
void hal_pmu1_dsp_l2ram_config(l2m_dsp_memory_share_mode_t mode);

/**
  * @brief  config bt share mode
  * @param  bt_memory_share_mode_t Share mode config value
  */
void hal_pmu_bt_config(bt_memory_share_mode_t mode);

/**
  * @brief  Power on l2m dsp.
  * @param  pos l2m dsp power on position.
  * @param  en Enable or disable.
  */
void hal_pmu1_dig_pg_sub_l2m_dsp_pwr_en(hal_l2m_dsp_power_on_t pos, bool en);

/**
  * @brief  Ret on l2m dsp.
  * @param  pos l2m dsp power on position.
  * @param  en Enable or disable.
  */
void hal_pmu1_dig_pg_sub_l2m_dsp_ret1_en(hal_l2m_dsp_power_on_t pos, bool en);

/**
  * @brief  Power on l2m dsp.
  * @param  pos l2m dsp power on position.
  */
bool hal_pmu1_dig_pg_sub_l2m_dsp_pwr_ack_get(hal_l2m_dsp_power_on_t pos);

#endif