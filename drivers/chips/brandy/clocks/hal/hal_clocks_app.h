/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:   HAL APP CLOCK DRIVER HEADER FILE
 * Author: @CompanyNameTag
 * Create: 2020-01-13
 */

#ifndef HAL_CLOCKS_APPLICATION_H
#define HAL_CLOCKS_APPLICATION_H

#include "hal_clocks.h"
#include "hal_clocks_app_priv.h"

/** @addtogroup connectivity_drivers_hal_clock
  * @{
  */
typedef enum {
    HAL_CLOCKS_TCXO_1X,
    HAL_CLOCKS_TCXO_2X,
} hal_clocks_tcxo_clk_type_t;

typedef enum {
    HAL_MCPU_SOFT_RST_IR            = 0,
    HAL_MCPU_SOFT_RST_IMG_DEC       = 1,
    HAL_MCPU_SOFT_RST_AUX_ADC       = 2,
    HAL_MCPU_SOFT_RST_SEC           = 3,
    HAL_MCPU_SOFT_RST_MDIAG         = 4,
    HAL_MCPU_SOFT_RST_MEM_BUS       = 8,
    HAL_MCPU_SOFT_RST_XIP_BUS       = 9,
    HAL_MCPU_SOFT_RST_DISPLAY_BUS   = 10,
    HAL_MCPU_SOFT_RST_PWM           = 11,
    HAL_MCPU_SOFT_RST_N1_OFFSET     = HAL_MCPU_SOFT_RST_DISPLAY_BUS,
} hal_mcpu_soft_rst_type_t;

typedef enum {
    HAL_MCPU_JPGD_CLK_EN                          = 1,
    HAL_MCPU_MIPITX_CLK_EN                        = 2,
    HAL_MCPU_TDE_APB_CLK_EN                       = 3,
    HAL_MCPU_VDP_APB_CLK_EN                       = 4,
    HAL_MCPU_VIDEO_PLL2CBB_160M_CLK_EN            = 5,
    HAL_MCPU_AON_CTRL_MIPITX_CLK_EN               = 6,
    HAL_MCPU_AON_CTRL_VIDEO_CLK_EN                = 7,
} hal_mcpu_video_clk_t;

typedef enum {
    HAL_MCPU_VIDEO_SRST_MIPITX_REQ                = 1,
    HAL_MCPU_VIDEO_SRST_TDE_REQ                   = 2,
    HAL_MCPU_VIDEO_SRST_VDP_REQ                   = 3,
    HAL_MCPU_VIDEO_SRST_JPGD_REQ                  = 4,
} hal_mcpu_video_srst_t;

/**
 * @brief  Config mcpu reset pulse width.
 * @param  pulse_width  Reset pulse width.
 */
void hal_clocks_set_mcpu_soft_trg_pulse_width(uint8_t pulse_width);

/**
 * @brief  Control mcpu soft reset module.
 * @param  hal_mcpu_soft_rst Control which module.
 * @param  rst_control  Reset or dereset.
 */
void hal_clocks_mcpu_soft_reset_control(hal_mcpu_soft_rst_type_t hal_mcpu_soft_rst,
                                        hal_soft_rst_control_type_t rst_control);

/**
 * @brief  Control mem peripheral clock enable or disable.
 * @param  mem_clk Config which perip.
 * @param  on  Clock enable or disable.
 */
void hal_clocks_mem_perips_config(hal_clocks_mem_perips_clk_type_t mem_clk, switch_type_t on);

/**
 * @brief  Control mem peripheral reset/dereset.
 * @param  hal_mem_soft_rst Config which perip.
 * @param  rst_control  Reset or dereset.
 */
void hal_clocks_xip_soft_reset_control(hal_mem_soft_rst_type_t hal_mem_soft_rst,
                                       hal_soft_rst_control_type_t rst_control);

/**
 * @brief  Module auto cg control.
 * @param  auto_cg_module Config which module.
 * @param  auto_en  Auto cg or not.
 */
void hal_clocks_module_auto_cg_control(hal_clocks_module_auto_cg_t auto_cg_module, switch_type_t auto_en);

/**
 * @brief  Clocks bridging enter/exit low power config.
 * @param  on  TURN_OFF:enter lp, TURN_ON:exit lp.
 */
void hal_clocks_bridging_exit_lp_config(switch_type_t on);

/**
 * @brief  Config aux adc clock div in mcrg.
 * @param  div Config clk div.
 */
void hal_clocks_mcrg_aux_adc_div_set(uint8_t div);

/**
 * @brief  Get aux adc clock div in mcrg.
 * @return Divider number to be set.
 */
uint8_t hal_clocks_mcrg_aux_adc_div_get(void);

/**
 * @brief  Enable or disable the bt bus clock normal status.
 * @param  clk_en  Clock open or close.
 */
void hal_clocks_bcrg_clk_en(switch_type_t clk_en);

/**
 * @brief  Enable or disable the com bus xo dll2.
 * @param  clk_en xo dll2 open or close.
 */
void hal_clocks_ccrg_xo_dll2_clk_en(switch_type_t clk_en);

/**
 * @brief  Config mcu bus clock div.
 * @param  div Config clk div.
 */
void hal_clocks_ccrg_mcu_bus_div_set(uint8_t div);

/**
 * @brief  Enable or disable the common crg channel associating to the  module.
 * @param  module The module that need to enable or disable the channel.
 * @param  ch The channel to be set, If there's only one channel,
 *         this param should be set to HAL_CLOCKS_CRG_CH_INVALID.
 * @param  ch_en TURN_ON indicates to enable, otherwise disable.
 * @note   For the <code>ch</code> parameter,
 *         it's useful only when the number of the channels in the current module isn't singnal.
 */
void hal_clocks_ccrg_module_ch_enable(hal_clocks_ccrg_module_t module,
                                      hal_clocks_crg_ch_t ch, switch_type_t ch_en);

/**
 * @brief  Config the clock source and frequency divider.
 * @param  module The module that need to config clock parameters.
 * @param  ch The channel to be set, If there's only one channel,
 *         this param should be set to HAL_CLOCKS_CRG_CH_INVALID.
 * @param  clk_src The clock source.
 * @param  clk_div Divide value.
 */
void hal_clocks_ccrg_module_config(hal_clocks_ccrg_module_t module, hal_clocks_crg_ch_t ch, \
                                   uint8_t clk_src, uint8_t clk_div);

/**
 * @brief  Control glb module clock enable/disable.
 * @param  module The module that need to config clock parameters.
 * @param  on  Clock open or close.
 */
void hal_clocks_glb_clken_config(hal_clocks_ccrg_module_t module, switch_type_t on);

/**
 * @brief  Control aperp 32k clock manual value.
 * @param  manual_en  Manual value.
 */
void hal_clocks_aperp_32k_sel_manual_config(switch_type_t manual_en);

/**
 * @brief  Select aperp 32k clock manual/hw auto control.
 * @param  sel_en  Manual/hw auto control.
 */
void hal_clocks_aperp_32k_sel_control(switch_type_t sel_en);

/**
 * @brief  Control PLL clock enable/disable.
 * @param  pll_module  Which pll module.
 * @param  clk_en  Clock open or close.
 */
void hal_clocks_pll_module_clken(switch_type_t clk_en);

/**
 * @brief  Config MCU group clock source and clock div.
 * @param  module Which MCU group module.
 * @param  clk_source MCU normal clock source.
 * @param  div Config clk div.
 */
void hal_mcu_group_clock_switch(hal_clocks_ccrg_module_t module,
                                hal_clocks_ccrg_mcu_group_clk_src_t clk_source,
                                uint8_t div);

/**
 * @brief  Get mcpu_bus clock divider.
 * @return uint8_t The clock divider.
 */
uint8_t hal_clocks_ccrg_mcu_bus_div_get(void);

/**
 * @brief  Check whether the clock module is dual-channel.
 * @param module Which MCU group module.
 * @return true Means dual channel, otherwise, single channel.
 */
bool hal_clocks_is_dual_channel_module(hal_clocks_ccrg_module_t module);

/**
 * @brief  Clock channel selection configuration.
 * @param module Which MCU group module.
 * @param ch Which channel.
 */
void hal_clocks_sub_normal_sel_cfg(hal_clocks_ccrg_module_t module, hal_clocks_crg_ch_t ch);

/**
 * @brief  Clock channel selection configuration.
 * @param module Which MCU group module.
 * @param ch Which channel.
 */
void hal_clocks_video_clk_en(hal_mcpu_video_clk_t pos, switch_type_t clk_en);

/**
 * @brief  Clock channel selection configuration.
 * @param module Which MCU group module.
 * @param ch Which channel.
 */
void hal_clocks_video_srst_en(hal_mcpu_video_srst_t pos, switch_type_t clk_en);

/**
 * @brief  Clock channel selection configuration.
 * @param module Which MCU group module.
 * @param ch Which channel.
 */
uint16_t hal_clocks_video_clk_en_get(void);

/**
  * @}
  */
#endif
