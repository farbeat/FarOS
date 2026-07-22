/*
 * Copyright (c) @CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: L2ram Memory Map configuration
 * Author: @CompanyNameTag
 * Create: 2020-06-25
 */

#ifndef HAL_L2RAM_PRIVATE_H
#define HAL_L2RAM_PRIVATE_H

#include "chip_io.h"

/**
 * @defgroup connectivity_drivers_hal_l2ram L2RAM
 * @ingroup  connectivity_drivers_hal
 * @{
 */
/**
 * @brief  L2ram allocated to bt space.
 */
typedef enum {
    HAL_MEM_SHARE_0K_FOR_BT,      //!< L2RAM 0KB FOR BT, 384KB FOR MCU
    HAL_MEM_SHARE_128K_FOR_BT,    //!< L2RAM 128KB FOR BT, 256KB FOR MCU
    HAL_MEM_SHARE_256K_FOR_BT,    //!< L2RAM 256KB FOR BT, 128KB FOR MCU
} hal_b_memory_share_config_t;

/**
 * @brief  L2ram allocated to dsp space.
 */
typedef enum {
    HAL_MEM_SHARE_0K_FOR_DSP,       //!< L2RAM 0KB FOR DSP, 2048KB FOR MCU
    HAL_MEM_SHARE_128K_FOR_DSP,     //!< L2RAM 128KB FOR DSP, 1920KB FOR MCU
    HAL_MEM_SHARE_256K_FOR_DSP,     //!< L2RAM 256KB FOR DSP, 1792KB FOR MCU
    HAL_MEM_SHARE_384K_FOR_DSP,     //!< L2RAM 384KB FOR DSP, 1664KB FOR MCU
    HAL_MEM_SHARE_512K_FOR_DSP,     //!< L2RAM 512KB FOR DSP, 1536KB FOR MCU
    HAL_MEM_SHARE_640K_FOR_DSP,     //!< L2RAM 640KB FOR DSP, 1408KB FOR MCU
    HAL_MEM_SHARE_768K_FOR_DSP,     //!< L2RAM 768KB FOR DSP, 1280KB FOR MCU
    HAL_MEM_SHARE_896K_FOR_DSP,     //!< L2RAM 896KB FOR DSP, 1152KB FOR MCU
    HAL_MEM_SHARE_1024K_FOR_DSP,    //!< L2RAM 1024KB FOR DSP, 1024KB FOR MCU
    HAL_MEM_SHARE_1152K_FOR_DSP,    //!< L2RAM 1152KB FOR DSP, 896KB FOR MCU
    HAL_MEM_SHARE_1280K_FOR_DSP,    //!< L2RAM 1280KB FOR DSP, 768KB FOR MCU
    HAL_MEM_SHARE_1408K_FOR_DSP,    //!< L2RAM 1408KB FOR DSP, 640KB FOR MCU
    HAL_MEM_SHARE_1536K_FOR_DSP,    //!< L2RAM 1536KB FOR DSP, 512KB FOR MCU
    HAL_MEM_SHARE_1664K_FOR_DSP,    //!< L2RAM 1664KB FOR DSP, 384KB FOR MCU
    HAL_MEM_SHARE_1684K_FOR_DSP,    //!< L2RAM 1684KB FOR DSP, 256KB FOR MCU, notice 108KB cannot be used by all
    HAL_MEM_SHARE_MAX,              //!< L2RAM SHARE BETWEEN MCU & DSP TOTAL MEMORY SIZE: 2048KB for CS
} hal_dsp_memory_share_config_t;

/**
 * @brief  Enum defining the l2ram memory regions.
 */
typedef enum {
    HAL_MEMORY_L2RAM_DSP_REGION_0_1,
    HAL_MEMORY_L2RAM_DSP_REGION_2_3,
    HAL_MEMORY_L2RAM_DSP_REGION_4_5,
    HAL_MEMORY_L2RAM_DSP_REGION_6_7,
    HAL_MEMORY_L2RAM_DSP_REGION_8_9,
    HAL_MEMORY_L2RAM_DSP_REGION_10_11,
    HAL_MEMORY_L2RAM_DSP_REGION_12_13,
    HAL_MEMORY_L2RAM_DSP_REGION_14_15,
    HAL_MEMORY_L2RAM_DSP_REGION_16_17,
    HAL_MEMORY_L2RAM_DSP_REGION_18_19,
    HAL_MEMORY_L2RAM_DSP_REGION_20_21,
    HAL_MEMORY_L2RAM_DSP_REGION_22_23,
    HAL_MEMORY_L2RAM_DSP_REGION_24_25,
    HAL_MEMORY_L2RAM_DSP_REGION_26_27,
    HAL_MEMORY_L2RAM_DSP_REGION_28_29,
    HAL_MEMORY_L2RAM_DSP_REGION_30_31,
    HAL_MEMORY_L2RAM_BT_REGION_0_1,
    HAL_MEMORY_L2RAM_BT_REGION_2_3,
    HAL_MEMORY_L2RAM_BT_REGION_4_5,
    HAL_MEMORY_L2RAM_REGION_MAX,
} hal_memory_l2ram_region_type_t;

/**
 * @brief  Enum defining the l2ram memory regions power control sequence.
 */
typedef enum {
    HAL_PMU_L2RAM_RAM0_1            = 0,
    HAL_PMU_L2RAM_RAM2_3            = 1,
    HAL_PMU_L2RAM_RAM4_5            = 2,
    HAL_PMU_L2RAM_RAM6_7            = 11,
    HAL_PMU_L2RAM_RAM8_9            = 10,
    HAL_PMU_L2RAM_RAM10_11_12_13    = 9,
    HAL_PMU_L2RAM_RAM14_15_16_17    = 8,
    HAL_PMU_L2RAM_RAM18_19_20_21    = 7,
    HAL_PMU_L2RAM_RAM22_23_24_25    = 6,
    HAL_PMU_L2RAM_RAM26_27_28_29    = 5,
    HAL_PMU_L2RAM_RAM30_31_32_33    = 4,
    HAL_PMU_L2RAM_RAM34_35_36_37    = 3,
} hal_pmu_l2ram_mem_type_t;

/**
 * @brief  Allocate l2ram size for bcpu
 * @param  hal_memory_share_config  Config bt l2ram size
 */
void hal_b_memory_l2ram_config(hal_b_memory_share_config_t hal_memory_share_config);

/**
 * @brief  Allocate l2ram size
 * @param  hal_memory_share_config  Config dsp l2ram size
 */
void hal_dsp_memory_l2ram_config(hal_dsp_memory_share_config_t hal_memory_share_config);

/**
 * @brief  Get l2ram power control status.
 * @param  hal_l2ram_type  which l2ram region.
 * @return power_switch (power on:1 power off:0)
 */
switch_type_t hal_memory_l2ram_get_power_mode(hal_memory_l2ram_region_type_t region);

/**
 * @brief  l2ram mode control.
 * @param  region  which l2ram region.
 * @param  power_switch  ram power on / off.
 */
void hal_memory_l2ram_set_power_mode(hal_memory_l2ram_region_type_t region,
                                     switch_type_t power_switch);

/**
 * @brief  rf dig ram power control.
 * @param  power_switch  power on/off.
 */
void hal_pmu_rf_dig_power_enable(switch_type_t power_switch);

/**
 * @}
 */
#endif