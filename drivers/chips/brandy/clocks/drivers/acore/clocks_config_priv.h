/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  CLOCKS CONFIG PRIVATE HEADER.
 */
#ifndef CLOCKS_CONFIG_PRIVATE_H
#define CLOCKS_CONFIG_PRIVATE_H

#if !defined(BUILD_APPLICATION_ROM)
#include "clocks_types.h"
#include "hal_clocks_app.h"
#include "clocks_switch_priv.h"

/**
 * @addtogroup connectivity_drivers_non_os_clocks
 * @{
 */
// PLL config: 884.736M.
#define PLL_FBDIV_NUM                 0x37
#define PLL_FRAC_H_NUM                0x4b
#define PLL_FRAC_L_NUM                0xc6ab
#define PLL_POSTDIV1_NUM              0x2
#define PLL_POSTDIV2_NUM              0x1

typedef enum {
    CLOCKS_M32_B32_D_0,      // mcu:32M, bt:32M, dsp:32M
    CLOCKS_M32_B32_D_1,      // mcu:32M, bt:32M, dsp:64M
    CLOCKS_M32_B32_D_2,      // mcu:32M, bt:32M, dsp:98M
    CLOCKS_M32_B32_D_3,      // mcu:32M, bt:32M, dsp:126M
    CLOCKS_M32_B32_D_4,      // mcu:32M, bt:32M, dsp:176.9M
    CLOCKS_M32_B32_D_5,      // mcu:32M, bt:32M, dsp:294.9M
    CLOCKS_M32_B64_D_0,      // mcu:32M, bt:64M, dsp:32M
    CLOCKS_M32_B64_D_1,      // mcu:32M, bt:64M, dsp:64M
    CLOCKS_M32_B64_D_2,      // mcu:32M, bt:64M, dsp:98M
    CLOCKS_M32_B64_D_3,      // mcu:32M, bt:64M, dsp:126M
    CLOCKS_M32_B64_D_4,      // mcu:32M, bt:64M, dsp:176.9M
    CLOCKS_M32_B64_D_5,      // mcu:32M, bt:64M, dsp:294.9M
    CLOCKS_M64_B32_D_0,      // mcu:64M, bt:32M, dsp:32M
    CLOCKS_M64_B32_D_1,      // mcu:64M, bt:32M, dsp:64M
    CLOCKS_M64_B32_D_2,      // mcu:64M, bt:32M, dsp:98M
    CLOCKS_M64_B32_D_3,      // mcu:64M, bt:32M, dsp:126M
    CLOCKS_M64_B32_D_4,      // mcu:64M, bt:32M, dsp:176.9M
    CLOCKS_M64_B32_D_5,      // mcu:64M, bt:32M, dsp:294.9M
    CLOCKS_M64_B64_D_0,      // mcu:64M, bt:64M, dsp:32M
    CLOCKS_M64_B64_D_1,      // mcu:64M, bt:64M, dsp:64M
    CLOCKS_M64_B64_D_2,      // mcu:64M, bt:64M, dsp:98M
    CLOCKS_M64_B64_D_3,      // mcu:64M, bt:64M, dsp:126M
    CLOCKS_M64_B64_D_4,      // mcu:64M, bt:64M, dsp:176.9M
    CLOCKS_M64_B64_D_5,      // mcu:64M, bt:64M, dsp:294.9M
    CLOCKS_M_H_B64_D_H,      // mcu:221, bt:64M, dsp:294.9M
    CLOCKS_FREQ_MAX,
} system_clocks_freq_t;

typedef enum {
    CLOCKS_M16,     //  mcu:16M
    CLOCKS_M32,     //  mcu:32M
    CLOCKS_RC,      //  mcu:49.152M
    CLOCKS_M64,     //  mcu:64M
    CLOCKS_M_H0,    //  mcu:110M
    CLOCKS_M_H1,    //  mcu:220M
    CLOCKS_MCU_FREQ_MAX,
} system_mcu_freq_t;

/**
 * @brief  The clock level of display sub system.
 */
typedef enum {
    CLOCKS_DISPLAY_FREQ_0,      /*!< psram:16M, gpu:close, membus:mcu_clk/2. */
    CLOCKS_DISPLAY_FREQ_1,      /*!< psram:32M, gpu:close, membus:mcu_clk/2. */
    CLOCKS_DISPLAY_FREQ_2,      /*!< psram:100M, gpu:100M, membus:100M. */
    CLOCKS_DISPLAY_FREQ_3,      /*!< psram:133M, gpu:159.6M, membus:199.5M. */
    CLOCKS_DISPLAY_FREQ_4,      /*!< psram:159M, gpu:190.8M, membus:190.8M. */
    CLOCKS_DISPLAY_FREQ_MAX,
} system_display_freq_t;

typedef enum {
    CLOCKS_SDIO_FREQ_32M,      // sdio:32M
    CLOCKS_SDIO_FREQ_50M,      // rc clock, real value 49.512MHz
    CLOCKS_SDIO_FREQ_64M,      // sdio:64M
    CLOCKS_SDIO_FREQ_H,        // 294M
    CLOCKS_SDIO_FREQ_MAX,
} system_sdio_freq_t;

typedef struct {
    clocks_clk_cfg_t mcu_core_clk;
    clocks_clk_src_t bt_core_clk;
    clocks_clk_cfg_t dsp_core_clk;
} system_clocks_freq_config_t;

typedef struct {
    clocks_clk_cfg_t psram_clk;
    clocks_clk_cfg_t gpu_clk;
    clocks_clk_cfg_t mem_clk;
} system_display_freq_config_t;


typedef struct {
    clocks_clk_cfg_t mcu_clk;
    clocks_clk_cfg_t psram_clk;
    clocks_clk_cfg_t mem_clk;
    clocks_clk_cfg_t flash_clk;
    clocks_clk_cfg_t sdiom_clk;
    clocks_clk_cfg_t gpu_clk;
    clocks_clk_cfg_t spi_clk;
} system_mcu_core_clk_cfg_t;

/**
 * @brief  Dsp clock configuration.
 */
typedef struct {
    clocks_clk_src_t dsp_clk;             /*!< The source of the dsp clock. */
    uint8_t dsp0_div;                         /*!< The div of dsp0 clock. */
    uint8_t dsp1_div;                         /*!< The div of dsp1 clock. */
} dsp_clock_freq_t;

/**
 * @brief  System frequency.
 */
typedef struct {
    system_clocks_freq_t system_all_freq;   /*!< Indicates system clocks frequency. */
    uint8_t dsp0_div;                         /*!< Indicates the div of dsp0 clock. */
    uint8_t dsp1_div;                         /*!< Indicates the div of dsp1 clock. */
    bool mcu_only_flag;                     /*!< Indicates that use mcu only. */
    system_mcu_freq_t system_mcu_freq;      /*!< Indicates the frequency of MCU. */
} system_freq_t;

/**
 * @brief  clock gating configuration.
 */
typedef struct {
    hal_clocks_module_auto_cg_t moudle;   /*!< Indicates system clocks moudule. */
    switch_type_t on;                     /*!< Indicates the clock gating enable or not. */
} system_clk_cg_t;

/**
 * @brief  clock gating configuration.
 */
typedef struct {
    hal_clocks_app_perips_clk_type_t perip;   /*!< Indicates system clocks moudule. */
    switch_type_t on;                     /*!< Indicates the clock gating enable or not. */
} system_mcu_perip_clk_t;

/**
 * @brief Get the scpll_mcu configuration.
 * @return The scpll_mcu configuration.
 */
cmu_pll_config_t clocks_pll_cfg_get(void);

/**
 * @brief Get SSB clocks configuration.
 * @return SSB clocks configuration.
 */
clocks_clk_cfg_t const *clocks_ssb_clk_cfg_get(void);

/**
 * @brief  Get the system clocks configuration for the specified clock level.
 * @param  clk_level The clock level.
 * @return The system clocks configuration.
 */
clocks_clk_cfg_t const *clocks_system_all_clocks_get(system_clocks_config_t clk_level);

/**
 * @brief  Get the system clocks configuration of all cores for the specified clock level.
 * @param  clk_level The clock level.
 * @return The system clocks configuration of all cores.
 */
system_clocks_freq_config_t clocks_system_freq_cfg_get(system_clocks_freq_t clk_level);

/**
 * @brief  Get the system clocks configuration of display for the specified clock level.
 * @param  clk_level The clock level.
 * @return The system clocks configuration of display.
 */
system_display_freq_config_t clocks_system_display_freq_get(system_display_freq_t clk_level);

/**
 * @brief  Get the clocks configuration of mcu for the specified clock level.
 * @param  clk_level The clock level.
 * @return The clocks configuration of mcu.
 */
system_mcu_core_clk_cfg_t clocks_system_mcu_freq_get(system_mcu_freq_t clk_level);

/**
 * @brief  Get the clocks configuration of sdio for the specified clock level.
 * @param  clk_level The clock level.
 * @return The clocks configuration of sdio.
 */
clocks_clk_cfg_t clocks_system_sdio_freq_get(system_sdio_freq_t clk_level);

/**
 * @brief Get the clocks gating configuration of system.
 * @param length return the size of configuration.
 * @return The system clocks gating configuration.
 */
system_clk_cg_t const *clock_system_cg_config_get(uint32_t *length);

/**
 * @brief Get the mcu all perips clocks status.
 * @param length return the size of configuration.
 * @return The system mcu all perips clocks configuration.
 */
system_mcu_perip_clk_t *clocks_system_mcu_perip_clock_config_get(uint32_t *length);

/**
 * @}
 */
#endif

#endif