/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:   HAL SDIO CLOCK DRIVER HEADER FILE
 * Author: @CompanyNameTag
 * Create: 2020-01-13
 */

#ifndef HAL_CLOCKS_SDIO_H
#define HAL_CLOCKS_SDIO_H
#include "chip_io.h"
#include "hal_clocks.h"

/** @addtogroup connectivity_drivers_hal_clock
  * @{
  */
typedef enum {
    HAL_CLOCKS_SDIO_EMMC_TX_RX,
    HAL_CLOCKS_SDIO_EMMC_H,
    HAL_CLOCKS_SDIO_EMMC_M_B_TM,
    HAL_CLOCKS_SDIO_HOST_TX_RX,
    HAL_CLOCKS_SDIO_HOST_H,
    HAL_CLOCKS_SDIO_HOST_M_B_TM,
    HAL_CLOCKS_SDIO_DEVICE_BUS,
    HAL_CLOCKS_SDIO_DEVICE_PAD,
} hal_clocks_sdio_clk_type_t;

typedef enum {
    HAL_SDIO_SOFT_RST_EMMC_TX_RX,
    HAL_SDIO_SOFT_RST_EMMC_H,
    HAL_SDIO_SOFT_RST_EMMC_M_B_TM,
    HAL_SDIO_SOFT_RST_HOST_TX_RX,
    HAL_SDIO_SOFT_RST_HOST_H,
    HAL_SDIO_SOFT_RST_HOST_M_B_TM,
    HAL_SDIO_SOFT_RST_DEVICE_BUS,
    HAL_SDIO_SOFT_RST_DEVICE_SD,
} hal_sdio_soft_rst_type_t;

typedef enum {
    HAL_CLOCKS_SDIO_DIV_BYPASS      = 0,
    HAL_CLOCKS_SDIO_DIV_2X          = 2,
    HAL_CLOCKS_SDIO_DIV_4X          = 4,
} hal_clocks_sdio_div_type_t;

typedef enum {
    HAL_CLOCKS_SDIO_FREQ_IP,
    HAL_CLOCKS_SDIO_FREQ_REG,
} hal_sdio_freq_mode_type_t;

typedef enum {
    HAL_SDIO_32M_SEL_PLL,
    HAL_SDIO_32M_SEL_32M,
} hal_sdio_32m_select_type_t;

/**
 * @brief  Enable/Disable emmc host clock.
 * @param  clk_en Clock enable or disable
 * @return None
 */
void hal_clocks_emmc_host_clken(switch_type_t clk_en);

/**
 * @brief  Set emmc cclk div.
 * @param  div set cclk div
 * @return None
 */
void hal_clocks_set_emmc_cclk_div(uint8_t div);

/**
 * @brief  Control emmc tuning coarse mode.
 * @param  tuning_coarse_mode emmc tuning coarse.
 * @return None
 */
void hal_clocks_emmc_tuning_coarse_mode_control(switch_type_t tuning_coarse_mode);

/**
 * @brief  Config emmc tuning cclk bm0.
 * @param  tuning_cclk_bm emmc tuning cclk bm0.
 * @return None
 */
void hal_clocks_set_emmc_tuning_cclk_bm0(uint16_t tuning_cclk_bm);

/**
 * @brief  Config emmc tuning cclk bm1.
 * @param  tuning_cclk_bm emmc tuning cclk bm1.
 * @return None
 */
void hal_clocks_set_emmc_tuning_cclk_bm1(uint16_t tuning_cclk_bm);

/**
 * @brief  Enable/Disable sdio host clock.
 * @param  clk_en Clock enable or disable
 * @return None
 */
void hal_clocks_sdio_host_clken(switch_type_t clk_en);

/**
 * @brief  Set sdio cclk div.
 * @param  div set cclk div
 * @return None
 */
void hal_clocks_set_sdio_cclk_div(uint8_t div);

/**
 * @brief  Control sdio tuning coarse mode.
 * @param  tuning_coarse_mode emmc tuning coarse.
 * @return None
 */
void hal_clocks_sdio_tuning_coarse_mode_control(switch_type_t tuning_coarse_mode);

/**
 * @brief  Config sdio tuning cclk bm0.
 * @param  tuning_cclk_bm emmc tuning cclk bm0.
 * @return None
 */
void hal_clocks_set_sdio_tuning_cclk_bm0(uint16_t tuning_cclk_bm);

/**
 * @brief  Config sdio tuning cclk bm1.
 * @param  tuning_cclk_bm emmc tuning cclk bm1.
 * @return None
 */
void hal_clocks_set_sdio_tuning_cclk_bm1(uint16_t tuning_cclk_bm);

/**
 * @brief  Set sdio bus div.
 * @param  div set cclk div
 * @return None
 */
void hal_clocks_set_sdio_bus_div(uint8_t div);

/**
 * @brief  Set sdio tm div.
 * @param  div set tm div
 * @return None
 */
void hal_clocks_set_sdio_tm_div(uint8_t div);

/**
 * @brief  Control sdio crg reset.
 * @param  rst_ctl sdio crg reset/dereset.
 * @return None
 */
void hal_clocks_sdio_crg_rst_control(switch_type_t rst_ctl);

/**
  * @}
  */
#endif
