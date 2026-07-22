/*
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description:   BT PMU DRIVER HEADER FILE
 * Author: @CompanyNameTag
 * Create: 2019-12-19
 */

#ifndef HAL_CLOCKS_GLB_H
#define HAL_CLOCKS_GLB_H
#include "hal_clocks.h"

/** @defgroup connectivity_drivers_hal_clocks_glb Clocks Global
  * @ingroup  connectivity_drivers_hal
  * @{
  */
/**
 * @brief  Describe always on pheriphrals clock.
 */
typedef enum {
    HAL_CLOCK_AON_PERIP_EFUSE         = 3,
    HAL_CLOCK_AON_PERIP_ULP_AON_BUS   = 8,
    HAL_CLOCK_AON_PERIP_GLB_32K       = 9,
    HAL_CLOCK_AON_PERIP_SDIOTM        = 10,
    HAL_CLOCK_AON_PERIP_MCU_CORE      = 11,
    HAL_CLOCK_AON_PERIP_XO_RC_32M     = 12,
    HAL_CLOCK_AON_PERIP_HPM           = 13,
    HAL_CLOCK_AON_PERIP_ULP_RTC       = 14,
    HAL_CLOCK_AON_PERIP_MCPU_CLK_TEST = 15,
} hal_clock_aon_perips_type_t;

typedef enum {
    HAL_CLOCKS_COM_AON_BUS_CLKEN = 3,
} hal_clocks_com_clk_type_t;

typedef enum {
    HAL_CLOCK_AON_TCXO  = 0,
    HAL_CLOCK_AON_RC    = 1,
} hal_clocks_aon_clk_t;

typedef enum {
    HAL_CLOCKS_GLB_FON_COM_BUS,
    HAL_CLOCKS_GLB_FON_MCU_BUS,
} hal_clocks_glb_fon_bus_t;

typedef enum {
    HAL_SOFT_RST0_EFUSE,
    HAL_SOFT_RST0_EFUSE_BOOT
} hal_aon_soft_rst_type_t;

typedef enum {
    HAL_SOFT_RST1_GLB_COM_CRG,
    HAL_SOFT_RST1_GLB_COM_LGC,
    HAL_SOFT_RST2_B_CRG,
    HAL_SOFT_RST2_B_LGC,
    HAL_SOFT_RST2_B_CPU,
} hal_glb_bt_soft_rst_type_t;

typedef enum {
    HAL_SOFT_RST4_DISPLAY_CRG,
    HAL_SOFT_RST4_DISPLAY_LGC,
} hal_glb_display_soft_rst_type_t;

typedef enum {
    HAL_SOFT_RST5_MCU_CRG,
    HAL_SOFT_RST5_MCU_LGC,
    HAL_SOFT_RST5_MCU_CPU,
} hal_glb_mcu_soft_rst_type_t;

typedef enum {
    HAL_SOFT_RST6_SDIO_CRG,
    HAL_SOFT_RST6_SDIO_LGC,
} hal_glb_sdio_soft_rst_type_t;

typedef enum {
    HAL_SOFT_RST7_XIP_CRG,
    HAL_SOFT_RST7_XIP_LGC,
} hal_glb_xip_soft_rst_type_t;

/**
 * @brief  Turn on/off always on peripheral clock.
 * @param  perip The peripheral clock to config.
 * @param  clken Turn on/off.
 */
void hal_clocks_aon_config(hal_clock_aon_perips_type_t perip, switch_type_t clken);

/**
 * @brief  Control aon soft reset module.
 * @param  hal_aon_soft_rst Control which module.
 * @param  rst_control  reset or dereset.
 */
void hal_clocks_aon_soft_reset_control(hal_aon_soft_rst_type_t hal_aon_soft_rst,
                                       hal_soft_rst_control_type_t rst_control);

/**
 * @brief  Control glb mcu soft reset module.
 * @param  hal_glb_mcu_soft_rst Control which module.
 * @param  rst_control  reset or dereset.
 */
void hal_clocks_glb_mcu_soft_reset_control(hal_glb_mcu_soft_rst_type_t hal_glb_mcu_soft_rst,
                                           hal_soft_rst_control_type_t rst_control);

/**
 * @brief  Control glb bt soft reset module.
 * @param  hal_glb_bt_soft_rst Control which module.
 * @param  rst_control  reset or dereset.
 */
void hal_clocks_glb_bt_soft_reset_control(hal_glb_bt_soft_rst_type_t hal_glb_bt_soft_rst,
                                          hal_soft_rst_control_type_t rst_control);

/**
 * @brief  Control glb dsp soft reset module.
 * @param  rst_control  reset or dereset.
 */
void hal_clocks_glb_dsp_soft_reset_control(hal_soft_rst_control_type_t rst_control);

/**
 * @brief  Control glb display soft reset module.
 * @param  hal_glb_display_soft_rst Control which module.
 * @param  rst_control  reset or dereset.
 */
void hal_clocks_glb_display_soft_reset_control(hal_glb_display_soft_rst_type_t hal_glb_display_soft_rst,
                                               hal_soft_rst_control_type_t rst_control);

/**
 * @brief  Control mtop bus clock enable or disable.
 * @param  com_clk Choose which bus
 * @param  clken   Clock open or close
 */
void hal_clocks_com_bus_enable(hal_clocks_com_clk_type_t com_clk, switch_type_t clken);

/**
 * @brief  Control glb sdio soft reset module.
 * @param  hal_glb_sdio_soft_rst Control which module.
 * @param  rst_control  reset or dereset.
 */
void hal_clocks_glb_sdio_soft_reset_control(hal_glb_sdio_soft_rst_type_t hal_glb_sdio_soft_rst,
                                            hal_soft_rst_control_type_t rst_control);

/**
 * @brief  AON 32M clock enable control.
 * @param  clken   Clock open or close
 */
void hal_clocks_a32m_common_clk_en(switch_type_t clken);

/**
 * @brief  Set AON 32M clock div.
 * @param  div_num   clock div.
 */
void hal_clocks_set_a32m_common_clk_div(uint8_t div_num);

/**
 * @brief  hf 32k clock enable control.
 * @param  clken   Clock open or close
 */
void hal_clocks_hf_32k_clk_en(switch_type_t clken);

/**
 * @brief  Set hf 32k clock div.
 * @param  div_num   clock div.
 */
void hal_clocks_set_hf_32k_div(uint8_t div_num);

/**
 * @brief  AON 32M common clock enable control.
 * @param  clken   Clock open or close
 */
void hal_clocks_a32m_common_clken(switch_type_t clken);

/**
 * @brief  AON 32M common clock source select.
 * @param  clksel   Clock source
 */
void hal_clocks_a32m_common_sel(hal_clocks_aon_clk_t clksel);

/**
 * @brief  xo_rc clock source select.
 * @param  clksel   Clock source
 */
void hal_clocks_xo_rc_clk_sel(hal_clocks_aon_clk_t clksel);

/**
 * @brief  xo_32K clock source select.
 * @param  clksel   Clock source
 */
void hal_clocks_xo_32k_clk_sel(switch_type_t clksel);

/**
 * @brief  rcxo_32K clock source select.
 * @param  clksel   Clock source
 */
void hal_clocks_rcxo_32k_clk_sel(switch_type_t clksel);

/**
 * @brief  Control glb bus fon enable.
 * @param  fon_bus   which bus
 * @param  clken   fon en or reg control.
 */
void hal_clocks_glb_fon_bus_control(hal_clocks_glb_fon_bus_t fon_bus, switch_type_t clken);

/**
 * @brief  Set aon bus clock div.
 * @param  div_num   clock div.
 */
void hal_clocks_set_aon_bus_div(uint8_t div_num);

/**
 * @brief  AON bus clock enable control.
 * @param  clken   Clock open or close
 */
void hal_clocks_aon_bus_clken(switch_type_t clken);

/**
 * @brief  Control glb xip soft reset or dereset.
 * @param  hal_glb_xip_soft_rst   reset module
 * @param  rst_control   reset or dereset
 */
void hal_clocks_glb_xip_soft_reset_control(hal_glb_xip_soft_rst_type_t hal_glb_xip_soft_rst,
                                           hal_soft_rst_control_type_t rst_control);

/**
 * @brief  Control mbus clock enable.
 * @param  clken  Clock open or close.
 */
void hal_clocks_mbus_clken_nor_control(switch_type_t clken);

/**
 * @brief  Release bcore wait.
 */
void hal_clocks_bt_core_wait_release(void);

/**
 * @brief  Codec clock enable.
 */
void hal_clocks_codec_enable(void);

/**
 * @brief  Codec clock disable.
 */
void hal_clocks_codec_disable(void);
/**
  * @}
  */
#endif
