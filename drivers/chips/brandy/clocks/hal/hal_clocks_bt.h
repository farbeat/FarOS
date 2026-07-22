/*
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description:BT PMU DRIVER HEADER FILE
 * Author: @CompanyNameTag
 * Create: 2019-12-19
 */

#ifndef HAL_CLOCKS_BT_H
#define HAL_CLOCKS_BT_H
#include "hal_clocks_glb.h"

/** @addtogroup connectivity_drivers_hal_clocks_glb
  * @{
  */
typedef enum {
    HAL_CLOCKS_BT_FCLK          = 2,
    HAL_CLOCKS_BT_STCLK         = 3,
    HAL_CLOCKS_BT_RF_TEST_CLK   = 4,
    HAL_CLOCKS_BT_MEM_CLK       = 5,
    HAL_CLOCKS_BT_CBB_CLK       = 6,
    HAL_CLOCKS_BT_UART_CLK      = 7,
    HAL_CLOCKS_BT_TRNG_CLK      = 8,
    HAL_CLOCKS_BT_DIAG_CTL_CLK  = 9,
    HAL_CLOCKS_BT_CPU_TRACE_CLK = 10,
    HAL_CLOCKS_BT_TIMER_CLK     = 11,
    HAL_CLOCKS_BT_WDT_CLK       = 12,
} hal_clocks_bt_perips_clk_type_t;

typedef enum {
    HAL_BCPU_SOFT_TRG_BBUS      = 0,
    HAL_BCPU_SOFT_TRG_BCPU_POR  = 2,
    HAL_BCPU_SOFT_TRG_BCPU_SYS  = 3,
} hal_bcrg_soft_trg_type_t;

typedef enum {
    HAL_BCPU_SOFT_RST_BCPU_SYS,
    HAL_BCPU_SOFT_RST_BCPU_POR,
} hal_bcpu_soft_rst_t;

typedef enum {
    HAL_BCPU_SOFT_RST_RF_TEST           = 0,
    HAL_BCPU_SOFT_RST_TWS               = 1,
    HAL_BCPU_SOFT_RST_CRG_BT_SUB        = 2,
    HAL_BCPU_SOFT_RST_BT_SUB            = 3,
    HAL_BCPU_SOFT_RST_B_TRNG            = 4,
    HAL_BCPU_SOFT_RST_TGLP              = 5,
    HAL_BCPU_SOFT_RST_BCBB              = 6,
} hal_bcpu_soft_rst_type_t;

/**
 * @brief  BT CPU clock switch to 16M clock.
 * @return None
 */
void hal_clocks_bcpu_switch_to_16m(void);

/**
 * @brief
 * @return true is tcxo_16m, false is not tcxo_16m.
 */
bool hal_clocks_bcpu_is_switch_to_16m(void);

/**
 * @brief  BT CPU clock switch to 1x tcxo clock.
 * @return None
 */
void hal_clocks_bcpu_switch_tcxo_1x(void);

/**
 * @brief  BT CPU clock switch to 2x tcxo clock.
 * @return None
 */
void hal_clocks_bcpu_switch_tcxo_2x(void);

/**
 * @brief  BT CPU clock switch which clock.
 * @return True is tcxo_2x clk, else tcxo clk
 */
bool hal_clocks_bcpu_is_switch_tcxo_2x(void);

/**
 * @brief  Get the clock configuration of the BT normal status.
 * @return True is tcxo_2x, false is tcxo.
 */
bool hal_clocks_bcpu_get_normal_config(void);

/**
 * @brief  Control BT peripheral clock enable or disable.
 * @param  bt_clk Config which common bus.
 * @param  clken  Clock enable or disable
 * @return None
 */
void hal_clocks_bt_perip_config(hal_clocks_bt_perips_clk_type_t bt_clk, switch_type_t clken);

/**
 * @brief  Control clock pulse width.
 * @param  dll_ph Clock pulse width
 * @return None
 */
void hal_clocks_bcpu_dll_ph_config(uint8_t dll_ph);

/**
 * @brief  Control bcpu clock dll enable or disable.
 * @param  dll_en  Clock open or close
 * @return None
 */
void hal_clocks_bcpu_dll_enable(switch_type_t dll_en);


/**
 * @brief  Config BCPU hclken auto/manual gating after entering WFI.
 * @param  mode          AUTO/MANUAL
 * @return None
 */
void hal_clocks_bcpu_hclken_mode(hal_clocks_cpu_gating_mode_type_t mode);

/**
 * @brief  Config BCPU hclken manual status.
 * @param  clken  Clock open or close
 * @return None
 */
void hal_clocks_bcpu_hclken_manual_config(switch_type_t clken);

/**
 * @brief  Config bcpu reset pulse width.
 * @param  pulse_width  reset pulse width.
 * @return None
 */
void hal_clocks_set_bcpu_soft_trg_pulse_width(uint8_t pulse_width);

/**
 * @brief  Control bcpu soft reset trg module.
 * @param  hal_bcpu_soft_trg Control which module.
 * @param  rst_control  reset or dereset.
 * @return None
 */
void hal_clocks_bcpu_soft_trg_control(hal_bcrg_soft_trg_type_t hal_bcpu_soft_trg,
                                      hal_soft_rst_control_type_t rst_control);

/**
 * @brief  Control bcpu soft reset module.
 * @param  hal_bcpu_soft_rst Control which module.
 * @param  rst_control  reset or dereset.
 * @return None
 */
void hal_clocks_bcpu_soft_reset_control(hal_bcpu_soft_rst_type_t hal_bcpu_soft_rst,
                                        hal_soft_rst_control_type_t rst_control);

/**
 * @brief  Control bcpu soft all reset module.
 * @param  rst_control  reset or dereset.
 * @return None
 */
void hal_clocks_bcpu_soft_all_reset_control(hal_soft_rst_control_type_t rst_control);

/**
 * @brief  Set bcpu 32M clock div.
 * @param  div   clock div.
 * @return None
 */
void hal_clocks_set_bcpu_32m_div(uint8_t div);

/**
 * @brief  Set bperp bus clock div.
 * @param  div   clock div.
 * @return None
 */
void hal_clocks_set_bperp_bus_div(uint8_t div);

/**
 * @brief  Control bcpu soft reset module.
 * @param  rst_t Control which module.
 * @param  rst_control  reset or dereset.
 * @return None
 */
void hal_clocks_bcpu_soft_rst(hal_bcpu_soft_rst_t rst_t, hal_soft_rst_control_type_t rst_control);

/**
  * @}
  */
#endif
