/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description: HAL CLOCKS MODULE HEADER
 */

#ifndef HAL_CLOCKS_H
#define HAL_CLOCKS_H

#include "core.h"
#include "chip_io.h"

/** @defgroup connectivity_drivers_hal_clocks CLOCKS
  * @ingroup  connectivity_drivers_hal
  * @{
  */
#define SYSTICK_CLK     32768ULL            // System 32k clock.
#define TCXO_1X_CLK     32000000ULL         // System TCXO 1X 32M clock.
#define TCXO_2X_CLK     64000000ULL         // System TCXO 2X 64M clock.
#define RC_CLK          49152000ULL         // System RC 49.152M clock.
#define TCXO_16M_CLK    16000000ULL         // System 16M clock.

#define CMU_CLOCK_GATING_MASK 0xFFFF

/**
 * @brief  Enum defining the available power mode.
 * Represents
 */
typedef enum {
    HAL_CLOCKS_POWER_NORMAL,  // !< Represents normal power mode.
    HAL_CLOCKS_POWER_SLEEP    // !< Represents sleep power mode.
} hal_clocks_power_mode_type_t;

#if CORE == BT
typedef enum {
    HAL_CLOCKS_BCPU_HCLKEN_SLP      = 0,
    HAL_CLOCKS_BBUS_CLKEN_SLP       = 1,
    HAL_CLOCKS_BDMA_LP_REQ_SLP      = 2,
    HAL_CLOCKS_B_32M_2X_CLKSEL_SLP  = 3,
} hal_clocks_sub_slp_cfg_t;

typedef enum {
    HAL_CLOCKS_BCPU_HCLKEN_NOR      = 4,
    HAL_CLOCKS_BDMA_CLKEN_NOR       = 6,
    HAL_CLOCKS_B_32M_2X_CLKSEL_NOR  = 7,
} hal_clocks_sub_nor_cfg_t;
#else // CORE != BT
typedef enum {
    HAL_CLOCKS_MCPU_CLKEN_SLP       = 0,
    HAL_CLOCKS_MCPU_HCLKEN_SLP      = 1,
    HAL_CLOCKS_MBUS_CLKEN_SLP       = 2,
    HAL_CLOCKS_MDMA_CLKEN_SLP       = 3,
    HAL_CLOCKS_SDMA_CLKEN_SLP       = 4,
    HAL_CLOCKS_COM_BUS_CLKEN_SLP    = 5,
    HAL_CLOCKS_MCU_CORE_CH_SEL_SLP  = 6,
} hal_clocks_sub_slp_cfg_t;

typedef enum {
    HAL_CLOCKS_MCPU_CLKEN_NOR       = 0,
    HAL_CLOCKS_MCPU_HCLKEN_NOR      = 1,
    HAL_CLOCKS_MDMA_CLKEN_NOR       = 3,
    HAL_CLOCKS_SDMA_CLKEN_NOR       = 4,
    HAL_CLOCKS_COM_BUS_CLKEN_NOR    = 5,
    HAL_CLOCKS_MCU_CORE_CH_SEL_NOR  = 6,
} hal_clocks_sub_nor_cfg_t;

typedef enum {
    HAL_CLOCKS_MCPU_COM_BUS_CH_SEL    = 0,
    HAL_CLOCKS_MCPU_HIFI_CH_SEL       = 1,
    HAL_CLOCKS_MCPU_CODEC_CH_SEL      = 2,
} hal_clocks_sub_com_ctrl_ch_sel_t;
#endif

/**
 * @brief  Enum defining the available cpu clocks gating mode.
 * Represents
 */
typedef enum {
    HAL_CLOCKS_CPU_GATING_MAN,  // !< Represents CPU auto gating clock.
    HAL_CLOCKS_CPU_GATING_AUTO, // !< Represents CPU manual gating clock.
} hal_clocks_cpu_gating_mode_type_t;

typedef enum {
    HAL_SOFT_RST_RESET,
    HAL_SOFT_RST_DERESET,
} hal_soft_rst_control_type_t;

typedef enum {
    HAL_RST_MASK_DISABLE,
    HAL_RST_MASK_ENABLE,
} hal_rst_mask_type_t;

/**
 * @brief  Set or clear XXX register bit.
 * @param  reg_addr     addr of register
 * @param  on           Set or clear
 * @param  position     loaction of bit(x),position form bit(0)
 * @return None
 */
void hal_clocks_config_bit(uint32_t reg_addr, switch_type_t on, REG16_POS position);

/**
 * @brief  Set XXX register bit,for clock select.
 * @param  reg_addr     addr of register
 * @param  on           Set or clear.
 * @param  position     The bit to set or clear.
 * @return None
 */
void hal_clocks_select_config(uint32_t reg_addr, switch_type_t on, REG16_POS position);

/**
 * @brief  Read CMU_CLOCK_GATING_CLKSEL_x REG status bit.
 * @param  reg_addr     The addr to read
 * @param  position     The status bit to read
 * @return bool         ture: the status bit has been set; false: not been set.
 */
bool hal_clocks_select_get_status(uint32_t reg_addr, uint16_t position);

/**
 * @brief  Check clocks status.
 * @param  addr     addr of register
 * @param  offset       check bit offset
 * @param  on           Set or clear
 * @return None
 */
void hal_clocks_status_check(uint32_t addr, uint16_t offset, switch_type_t on);

/**
 * @brief  Check clocks status. If check timeout, print log and enter panic.
 * @param  addr     addr of register
 * @param  offset       check bit offset
 * @param  on           Set or clear
 * @param  timeout      timeout value
 * @return None
 */
void hal_clocks_status_check_timeout(uint32_t addr, uint16_t offset, switch_type_t on, uint32_t timeout);

/**
 * @brief  Config cpu auto/manual gating hclk atfer entering WFI.
 * @param  mode         auto/manual gating cpu clock
 * @return None
 */
void hal_clocks_cpu_gating_mode(hal_clocks_cpu_gating_mode_type_t mode);

/**
 * @brief  Enable dma power mode switch.
 * @param  mode         power mode
 * @param  on           turn on/off
 * @return None
 */
void hal_clocks_dma_power_mode(hal_clocks_power_mode_type_t mode, switch_type_t on);

/**
 * @brief  Msub/bsub normal status switch.
 * @param  sub_normal_cfg  msub/bsub normal status config
 * @param  on   turn on/off
 * @return None
 */
void hal_clocks_sub_normal_cfg(hal_clocks_sub_nor_cfg_t sub_normal_cfg, switch_type_t on);

/**
 * @brief  Msub/bsub sleep status switch.
 * @param  sub_sleep_cfg  msub/bsub sleep status config
 * @param  on   turn on/off
 * @return None
 */
void hal_clocks_sub_sleep_cfg(hal_clocks_sub_slp_cfg_t sub_sleep_cfg, switch_type_t on);

/**
 * @brief  Enable bus power mode switch.
 * @param  mode         power mode
 * @param  on           turn on/off
 * @return None
 */
void hal_clocks_bus_power_mode(hal_clocks_power_mode_type_t mode, switch_type_t on);

/**
 * @brief  Return the value of the uart clock.
 * @return value of the uart clock in Hz.
 */
uint32_t hal_clocks_get_uart_clock_value(uart_bus_t uart);


/**
 * @brief  Return the value of the spi clock.
 * @return value of the spi clock in Hz.
 */
uint32_t hal_clocks_get_spi_clock_value(void);

/**
 * @brief  Return the value of the xip clock.
 * @return value of the xip clock in Hz.
 */
uint32_t hal_clocks_get_xip_clock_value(void);

#if CORE == APPS
/**
 * @brief  Return the value of the i2c clock.
 * @return value of the i2c clock in Hz.
 */
uint32_t hal_clocks_get_i2c_clock_value(void);
#endif

/**
 * @brief  Control bsub channel.
 * @param  mode  bsub sleep/normal status config
 * @param  on   turn on/off
 * @return None
 */
void hal_clocks_bsub_ch_config(hal_clocks_power_mode_type_t mode, switch_type_t on);

/**
  * @}
  */
#endif
