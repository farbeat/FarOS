/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:   HAL DISPLAY CLOCK DRIVER HEADER FILE
 * Author: @CompanyNameTag
 * Create: 2020-01-13
 */

#ifndef HAL_CLOCKS_DISPLAY_H
#define HAL_CLOCKS_DISPLAY_H

#include "hal_clocks.h"

/** @addtogroup connectivity_drivers_hal_clock
  * @{
  */
typedef enum {
    HAL_CLOCKS_DISPLAY_DSS            = 0,
    HAL_CLOCKS_DISPLAY_DSI_CFG        = 1,
    HAL_CLOCKS_DISPLAY_RGB_AXI        = 2,
    HAL_CLOCKS_DISPLAY_RGB_PPC_HD0    = 3,
    HAL_CLOCKS_DISPLAY_RGB_CFG        = 4,
    HAL_CLOCKS_DISPLAY_RGB_VO_HD0     = 5,
    HAL_CLOCKS_DISPLAY_RGB_VO_BT      = 6,
    HAL_CLOCKS_DISPLAY_RGB_VO_BT_BP   = 7,
    HAL_CLOCKS_DISPLAY_MIPI_PHY_REF   = 10,
    HAL_CLOCKS_DISPLAY_GPU_CLKEN      = 11,
    HAL_CLOCKS_DISPLAY_MIPI_PHY_CLKEN = 12,
} hal_clocks_display_clk_type_t;

typedef enum {
    HAL_DISPLAY_SOFT_RESET_GPU_CORE,
    HAL_DISPLAY_SOFT_RESET_GPU_AXI,
    HAL_DISPLAY_SOFT_RESET_GPU_AHB,
} hal_display_soft_reset_type_t;

typedef enum {
    HAL_DISPLAY_SOFT_TRG_RGB,
    HAL_DISPLAY_SOFT_TRG_DSS_MWDT,
    HAL_DISPLAY_SOFT_TRG_DSI_MWDT,
    HAL_DISPLAY_SOFT_TRG_DISPLAY_BUS_MWDT,
} hal_display_soft_trg_type_t;

typedef enum {
    HAL_DISPLAY_RST_MASK_RGB,
    HAL_DISPLAY_RST_MASK_DSS,
    HAL_DISPLAY_RST_MASK_DSI,
    HAL_DISPLAY_RST_MASK_DISPLAY_BUS,
} hal_display_rst_mask_type_t;

typedef enum {
    HAL_DISPLAY_DSS_M0_CSYSREQ,
    HAL_DISPLAY_DSS_M1_CSYSREQ,
    HAL_DISPLAY_DIS_AXI_CSYSREQ,
} hal_display_bus_lp_ctrl_t;

typedef enum {
    HAL_GPU_ENTER_LOW_POWER,
    HAL_GPU_EXIT_LOW_POWER,
} hal_gpu_lp_mode_type_t;

/**
 * @brief  Control display clock enable or disable.
 * @param  display_clk Config which sdio bus.
 * @param  clk_en  Clock enable or disable
 */
void hal_clocks_display_config(hal_clocks_display_clk_type_t display_clk, switch_type_t clk_en);

/**
 * @brief  Control rgb clock div enable or disable.
 * @param  div_en  Division factor enable or disable
 */
void hal_clocks_display_rgb_div_en(switch_type_t div_en);

/**
 * @brief  Config vo_bt clock div.
 * @param  vo_bt_div Config clk div value.
 */
void hal_clocks_config_display_vo_bt_div(uint8_t vo_bt_div);

/**
 * @brief  Get vo_bt clock div.
 * @return vo_bt clock div
 */
uint8_t hal_clocks_get_display_vo_bt_div(void);

/**
 * @brief  Display bus request enter or exit low power mode.
 * @param  display_bus_lp which display bus.
 * @param  req_en Request enter or exit low power mode.
 */
void hal_clocks_display_bus_low_power_ctrl(hal_display_bus_lp_ctrl_t display_bus_lp, switch_type_t req_en);

/**
 * @brief  Gpu request enter or exit low power mode.
 * @param  hal_gpu_lp_mode Request enter or exit low power mode.
 */
void hal_clocks_gpu_low_power_control(hal_gpu_lp_mode_type_t hal_gpu_lp_mode);

/**
 * @brief  Config rgb clock div.
 * @param  div_clken Config clk div value.
 */
void hal_clocks_display_rgb_div_clken(switch_type_t div_clken);

/**
 * @brief  Config mipi phy clock div.
 * @param  div Config clk div value.
 */
void hal_clocks_display_mipi_phy_div(uint8_t div);

/**
 * @brief  Get mipi phy clock div num.
 * @return Return mipi phy clock div num.
 */
uint8_t hal_clocks_display_mipi_phy_div_get(void);

/**
 * @brief  Config display module reset mask.
 * @param  display_rst_mask Which display module.
 * @param  rst_mask_type reset mask enable/disable.
 */
void hal_clocks_display_reset_mask(hal_display_rst_mask_type_t display_rst_mask,
                                   hal_rst_mask_type_t rst_mask_type);

/**
 * @brief Enable gpu auto cg function.
 */
void hal_clocks_gpu_auto_cg_en(void);

/**
 * @brief Enable dss auto cg function.
 */
void hal_clocks_dss_auto_cg_en(void);

/**
 * @brief Config dss clock enable or disable.
 * @param clk_en Clock enable or disable.
 */
void hal_clocks_config_dss_clock(switch_type_t clk_en);

/**
  * @}
  */
#endif
