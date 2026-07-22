/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  CLOCKS CONFIG PRIVATE SOURCE.
 */
#include "clocks_config_priv.h"
#include "product.h"

#if !defined(BUILD_APPLICATION_ROM)
// FVCO : FREF *(FBDIV+FRAC/(2^24))/REFDIV ;CLK_PLL:FVCO/POSTDIV1/POSTDIV2
static cmu_pll_config_t g_pll_cfg = {
    PLL_FBDIV_NUM,
    PLL_FRAC_H_NUM,
    PLL_FRAC_L_NUM,
    PLL_POSTDIV1_NUM,
    PLL_POSTDIV2_NUM,
};

#ifdef BUILD_APPLICATION_SSB
static clocks_clk_cfg_t g_ssb_clk_cfg[CLOCKS_CCRG_MODULE_MAX] = {
    { CLOCKS_CLK_SRC_PLL, CLOCK_DIV_4 },            // CLOCKS_CCRG_MODULE_MCU_CORE
    { CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1 },           // CLOCKS_CCRG_MODULE_MCU_PERP_LS
    { CLOCKS_CLK_SRC_PLL, CLOCK_DIV_8 },           // CLOCKS_CCRG_MODULE_MCU_PERP_UART
    { CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1 },        // CLOCKS_CCRG_MODULE_MCU_PERP_SPI
    { CLOCKS_CLK_SRC_PLL, CLOCK_DIV_9 },            // CLOCKS_CCRG_MODULE_COM_BUS
    { CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3 },            // CLOCKS_CCRG_MODULE_SDIOM
    { CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5 },            // CLOCKS_CCRG_MODULE_MEM_BUS
    { CLOCKS_CLK_SRC_PLL, CLOCK_DIV_4 },            // CLOCKS_CCRG_MODULE_XIP_OPI
    { CLOCKS_CLK_SRC_PLL, CLOCK_DIV_8 },            // CLOCKS_CCRG_MODULE_XIP_QSPI
    { CLOCKS_CLK_SRC_PLL, CLOCK_DIV_6 },            // CLOCKS_CCRG_MODULE_GPU
    { CLOCKS_CLK_SRC_PLL, CLOCK_DIV_6 },            // CLOCKS_CCRG_MODULE_HIFI
    { CLOCKS_CLK_SRC_PLL, CLOCK_DIV_36 },           // CLOCKS_CCRG_MODULE_CODEC
    { CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1 },            // CLOCKS_CCRG_MODULE_PAD_OUT0
    { CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1 },            // CLOCKS_CCRG_MODULE_PAD_OUT1
    { CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1 },            // CLOCKS_CCRG_MODULE_PAD_OUT2
};
#else
/************************** System clock Config ***********************/
static const clocks_clk_cfg_t g_system_all_core_clocks[CLOCKS_CONFIG_MAX][CLOCKS_CCRG_MODULE_MAX] = {
    /* All 32M */
    {
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_MCU_CORE
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_MCU_PERP_LS
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_MCU_PERP_UART
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_MCU_PERP_SPI
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_COM_BUS
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},          // CLOCKS_CCRG_MODULE_SDIOM
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_MEM_BUS
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_XIP_OPI
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_XIP_QSPI
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},          // CLOCKS_CCRG_MODULE_GPU
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_HIFI
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},          // CLOCKS_CCRG_MODULE_CODEC
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},          // CLOCKS_CCRG_MODULE_PAD_OUT0
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},          // CLOCKS_CCRG_MODULE_PAD_OUT1
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},          // CLOCKS_CCRG_MODULE_PAD_OUT1
    },
    /* All 64M */
    {
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},      // CLOCKS_CCRG_MODULE_MCU_CORE
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_MCU_PERP_LS
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_MCU_PERP_UART
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_MCU_PERP_SPI
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_COM_BUS
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_SDIOM
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},      // CLOCKS_CCRG_MODULE_MEM_BUS
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},      // CLOCKS_CCRG_MODULE_XIP_OPI
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},      // CLOCKS_CCRG_MODULE_XIP_QSPI
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},      // CLOCKS_CCRG_MODULE_GPU
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},      // CLOCKS_CCRG_MODULE_HIFI
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},      // CLOCKS_CCRG_MODULE_CODEC
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},          // CLOCKS_CCRG_MODULE_PAD_OUT0
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},          // CLOCKS_CCRG_MODULE_PAD_OUT1
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},          // CLOCKS_CCRG_MODULE_PAD_OUT2
    },
    /* PLL */
    {
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_8},         // CLOCKS_CCRG_MODULE_MCU_CORE
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},        // CLOCKS_CCRG_MODULE_MCU_PERP_LS
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},        // CLOCKS_CCRG_MODULE_MCU_PERP_UART
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_MCU_PERP_SPI
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_COM_BUS
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},         // CLOCKS_CCRG_MODULE_SDIOM
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_8},         // CLOCKS_CCRG_MODULE_MEM_BUS
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_4},         // CLOCKS_CCRG_MODULE_XIP_OPI
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},         // CLOCKS_CCRG_MODULE_XIP_QSPI
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_6},         // CLOCKS_CCRG_MODULE_GPU
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_6},         // CLOCKS_CCRG_MODULE_HIFI
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_36},        // CLOCKS_CCRG_MODULE_CODEC
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_PAD_OUT0
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_PAD_OUT1
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_PAD_OUT2
    },
    /* PLL 0.9v */
    {
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_4},         // CLOCKS_CCRG_MODULE_MCU_CORE
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},        // CLOCKS_CCRG_MODULE_MCU_PERP_LS
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},        // CLOCKS_CCRG_MODULE_MCU_PERP_UART
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_10},         // CLOCKS_CCRG_MODULE_MCU_PERP_SPI
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_COM_BUS
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},         // CLOCKS_CCRG_MODULE_SDIOM
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},         // CLOCKS_CCRG_MODULE_MEM_BUS
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},         // CLOCKS_CCRG_MODULE_XIP_OPI
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},         // CLOCKS_CCRG_MODULE_XIP_QSPI
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_6},         // CLOCKS_CCRG_MODULE_GPU
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},         // CLOCKS_CCRG_MODULE_HIFI
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_36},        // CLOCKS_CCRG_MODULE_CODEC
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_PAD_OUT0
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_PAD_OUT1
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_PAD_OUT2
    },
    /* test */
    {
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},      // CLOCKS_CCRG_MODULE_MCU_CORE
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_MCU_PERP_LS
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_MCU_PERP_UART
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_MCU_PERP_SPI
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_4},         // CLOCKS_CCRG_MODULE_COM_BUS
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},         // CLOCKS_CCRG_MODULE_SDIOM
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},      // CLOCKS_CCRG_MODULE_MEM_BUS
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},      // CLOCKS_CCRG_MODULE_XIP_OPI
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},      // CLOCKS_CCRG_MODULE_XIP_QSPI
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},      // CLOCKS_CCRG_MODULE_GPU
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_6},          // CLOCKS_CCRG_MODULE_HIFI
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_36},         // CLOCKS_CCRG_MODULE_CODEC
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},          // CLOCKS_CCRG_MODULE_PAD_OUT0
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},          // CLOCKS_CCRG_MODULE_PAD_OUT1
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},          // CLOCKS_CCRG_MODULE_PAD_OUT2
    },
};

/************************** System core clock Config ******************/
static const system_clocks_freq_config_t g_system_clocks_freq_config[CLOCKS_FREQ_MAX] = {
    {   // CLOCKS_M32_B32_D_0
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // mcu:32M
        CLOCKS_CLK_SRC_TCXO,                         // bt:32M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // dsp:32M
    },
    {   // CLOCKS_M32_B32_D_1
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // mcu:32M
        CLOCKS_CLK_SRC_TCXO,                         // bt:32M
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // dsp:64M
    },
    {   // CLOCKS_M32_B32_D_2
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // mcu:32M
        CLOCKS_CLK_SRC_TCXO,                         // bt:32M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_9},           // dsp:98M
    },
    {   // CLOCKS_M32_B32_D_3
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // mcu:32M
        CLOCKS_CLK_SRC_TCXO,                         // bt:32M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_7},           // dsp:126M
    },
    {   // CLOCKS_M32_B32_D_4
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // mcu:32M
        CLOCKS_CLK_SRC_TCXO,                         // bt:32M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},           // dsp:176.9M
    },
    {   // CLOCKS_M32_B32_D_5
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // mcu:32M
        CLOCKS_CLK_SRC_TCXO,                         // bt:32M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},           // dsp:294.9M
    },
    {   // CLOCKS_M32_B64_D_0
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // mcu:32M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:64M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // dsp:32M
    },
    {   // CLOCKS_M32_B64_D_1
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // mcu:32M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:64M
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // dsp:64M
    },
    {   // CLOCKS_M32_B64_D_2
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // mcu:32M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:64M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_9},           // dsp:98M
    },
    {   // CLOCKS_M32_B64_D_3
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // mcu:32M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:64M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_7},           // dsp:126M
    },
    {   // CLOCKS_M32_B64_D_4
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // mcu:32M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:32M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},           // dsp:176.9M
    },
    {   // CLOCKS_M32_B64_D_5
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // mcu:32M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:32M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},           // dsp:294.9M
    },
    {   // CLOCKS_M64_B32_D_0
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // mcu:64M
        CLOCKS_CLK_SRC_TCXO,                         // bt:32M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // dsp:32M
    },
    {   // CLOCKS_M64_B32_D_1
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // mcu:64M
        CLOCKS_CLK_SRC_TCXO,                         // bt:32M
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // dsp:64M
    },
    {   // CLOCKS_M64_B32_D_2
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // mcu:64M
        CLOCKS_CLK_SRC_TCXO,                         // bt:32M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_9},           // dsp:96.7M
    },
    {   // CLOCKS_M64_B32_D_3
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // mcu:64M
        CLOCKS_CLK_SRC_TCXO,                         // bt:32M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_7},           // dsp:126M
    },
    {   // CLOCKS_M64_B32_D_4
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // mcu:64M
        CLOCKS_CLK_SRC_TCXO,                         // bt:32M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},           // dsp:176.9M
    },
    {   // CLOCKS_M64_B32_D_5
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // mcu:64M
        CLOCKS_CLK_SRC_TCXO,                         // bt:32M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},           // dsp:294.9M
    },
    {   // CLOCKS_M64_B64_D_0
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // mcu:64M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:64M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},          // dsp:32M
    },
    {   // CLOCKS_M64_B64_D_1
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // mcu:64M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:64M
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // dsp:64M
    },
    {   // CLOCKS_M64_B64_D_2
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // mcu:64M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:64M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_9},           // dsp:96.7M
    },
    {   // CLOCKS_M64_B64_D_3
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // mcu:64M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:64M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_7},           // dsp:126M
    },
    {   // CLOCKS_M64_B64_D_4
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // mcu:64M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:64M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},           // dsp:176.9M
    },
    {   // CLOCKS_M64_B64_D_5
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},       // mcu:64M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:64M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},           // dsp:294.9M
    },
    {   // CLOCKS_M_H_B64_D_H
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_4},           // mcu:221M
        CLOCKS_CLK_SRC_TCXO_2X,                      // bt:64M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},           // dsp:294.9M
    },
};

static const system_display_freq_config_t g_system_display_freq_config[CLOCKS_DISPLAY_FREQ_MAX] = {
    {   // CLOCKS_DISPLAY_FREQ_0
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},        // opi:32M -> psram:16M
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},         // gpu:closed
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},         // membus:follow mcu_clk/2
    },
    {   // CLOCKS_DISPLAY_FREQ_1
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},     // opi:64M -> psram:32M
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},         // gpu:closed
        {CLOCKS_CLK_SRC_MAX, CLOCK_DIV_1},         // membus:follow mcu_clk/2
    },
    {   // CLOCKS_DISPLAY_FREQ_2
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_6},   // psram:147M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_8},   // gpu:110M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_8},   // membus:110M
    },
    {   // CLOCKS_DISPLAY_FREQ_3
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},   // psram:133M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},   // gpu:159.6M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_4},   // membus:199.5M
    },
    {   // CLOCKS_DISPLAY_FREQ_4
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},   // psram:159M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},   // gpu:190.8M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},   // membus:190.8M
    },
};

/************************** MCU core clock Config *********************/
static const system_mcu_core_clk_cfg_t g_system_mcu_freq_config[CLOCKS_MCU_FREQ_MAX] = {
    {   // CLOCKS_M_H16
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_2},   // mcu_clk:16M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},   // psram_clk:32M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_2},   // membus:16M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},   // flash: norflash & nandflash:32M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},   // sdiom:32M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},   // gpu:32M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},   // spi:32M
    },
    {   // CLOCKS_M32
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},   // mcu_clk:32M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},   // psram_clk:32M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},   // membus:32M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},   // flash: norflash & nandflash 32M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},   // sdiom: 32M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},   // gpu: 32M
        {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},   // spi: 32M
    },
    {   // CLOCKS_RC
        {CLOCKS_CLK_SRC_RC, CLOCK_DIV_1},   // mcu_clk:49.152M
        {CLOCKS_CLK_SRC_RC, CLOCK_DIV_1},   // psram_clk:49.152M
        {CLOCKS_CLK_SRC_RC, CLOCK_DIV_1},   // membus:49.152M
        {CLOCKS_CLK_SRC_RC, CLOCK_DIV_1},   // flash: norflash & nandflash 49.152M
        {CLOCKS_CLK_SRC_RC, CLOCK_DIV_1},   // sdiom: 49.152M
        {CLOCKS_CLK_SRC_RC, CLOCK_DIV_1},   // gpu: 49.152M
        {CLOCKS_CLK_SRC_RC, CLOCK_DIV_1},   // spi: 49.152M
    },
    {   // CLOCKS_M64
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},   // mcu_clk:64M
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},   // psram_clk:64M
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},   // membus:64M
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},   // flash: norflash & nandflash 64M
#if defined (CFG_DRIVERS_MMC)
        {CLOCKS_CLK_SRC_RC, CLOCK_DIV_1},        // sdiom: 64M
#else
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},   // sdiom: 64M
#endif
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},   // gpu: 64M
        {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},   // spi: 64M
    },
    {   // CLOCKS_M_H0
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_8},   // mcu_clk:110M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_4},   // psram_clk:225M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_8},   // membus:110M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},   // flash: norflash & nandflash: 176M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},   // sdiom:295M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_6},   // gpu:147M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_9},   // spi:98M
    },
    {   // CLOCKS_M_H1
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_4},   // mcu_clk:221M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},   // psram_clk:295M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},   // membus:177M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_5},   // flash: norflash & nandflash:177M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},   // sdiom:295M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_6},   // gpu:147M
        {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_10},  // spi:88M
    },
};

static const clocks_clk_cfg_t g_system_sdio_freq_config[CLOCKS_SDIO_FREQ_MAX] = {
    {CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1},        // 32M
    {CLOCKS_CLK_SRC_RC, CLOCK_DIV_1},          // 50M
    {CLOCKS_CLK_SRC_TCXO_2X, CLOCK_DIV_1},     // 64M
    {CLOCKS_CLK_SRC_PLL, CLOCK_DIV_3},         // 294.912M
};

/************************** MCU core all perips clock Config *********************/
static const system_mcu_perip_clk_t g_system_mcu_perp_clk_set_config[] = {
    /* Clocks M_CLKEN0 region */
    {HAL_CLOCKS_APP_SEC_CLK, TURN_ON},         // sec clock
    {HAL_CLOCKS_APP_DIAG_CLK, TURN_ON},        // diag clock
    {HAL_CLOCKS_APP_PWM_CLK, TURN_OFF},        // pwm clock
    {HAL_CLOCKS_APP_X2H_MEM_BUS_CLK, TURN_ON}, // x2h_mem_bus clock

    /* Clocks M_CLKEN1 region */
    {HAL_CLOCKS_APP_TIMER_CLK, TURN_ON},       // timer clock
    {HAL_CLOCKS_APP_SPI0_M_CLK, TURN_OFF},     // spi0 clock
    {HAL_CLOCKS_APP_SPI1_MS_CLK, TURN_OFF},    // spi1 clock
    {HAL_CLOCKS_APP_SPI2_MS_CLK, TURN_OFF},    // spi2 clock
    {HAL_CLOCKS_APP_UART_H0_CLK, TURN_ON},     // uart h0 clock
    {HAL_CLOCKS_APP_UART_H1_CLK, TURN_ON},     // uart h1 clock
    {HAL_CLOCKS_APP_I2C0_CLK, TURN_OFF},       // i2c0 clock
    {HAL_CLOCKS_APP_I2C1_CLK, TURN_ON},        // i2c1 clock
    {HAL_CLOCKS_APP_I2C2_CLK, TURN_OFF},       // i2c2 clock
    {HAL_CLOCKS_APP_I2C3_CLK, TURN_ON},        // i2c3 clock
    {HAL_CLOCKS_APP_QSPI_1CS_CLK, TURN_ON},    // qspi clock
    {HAL_CLOCKS_APP_XIP_BUS_CLK, TURN_ON},     // xip bus clock
    {HAL_CLOCKS_APP_COM_BUS_DSP_CLK, TURN_ON}, // com bus dsp clock
    {HAL_CLOCKS_APP_BT_TGTWS_CLK, TURN_ON},    // bt tgws clock  no closed will affect bt call

    /* Clocks M_CLKEN2 region */
    {HAL_CLOCKS_APP_PULSE_CAPTURE_CLK, TURN_OFF}, // pulse_capture_clken clock
    {HAL_CLOCKS_APP_M_DAP_CLK, TURN_ON},          // m_dap_clken clock
    {HAL_CLOCKS_APP_RC_CLK, TURN_ON},             // rc clock
    {HAL_CLOCKS_APP_XO_CLK, TURN_ON},             // xo clock
};
#endif

/*********************** clk cg config **********************************/
#if (CLK_AUTO_CG_ENABLE == YES)
static const system_clk_cg_t g_system_clkcg_config[] = {
    {HAL_CLOCKS_MODULE_AUTO_CG_COM_BUS,                  TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_CACHE,                    TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_MEM_AHB,                  TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_AON,                      TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_BT,                       TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_MCU_MAIN,                 TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_MCU_SDIO_M,               TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_SDIO_CFG,                 TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_DISPLAY,                  TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_MCU_CFG,                  TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_MAIN_AXI,                 TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_DISPLAY_AXI,              TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_MCU_PERP_AXI,             TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_EH2H_OPI,                 TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_EH2H_FCOM,                TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_EH2H_TCOM,                TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_EH2H_SDIO,                TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_EH2H_BSUB_FCOM,           TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_EH2H_BSUB_TCOM,           TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_MCU_APB,                  TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_AH2H_MCU_SUB_TAON,        TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_AH2H_MCU_SUB_SEC,         TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_AH2H_SH2H_MCU_SUB_MFSH2H, TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_SH2H_MCU_SUB_SFH2H,       TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_MCU_PERP_AXI_GLB,         TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_SDMA_MTOP_SDMA,           TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_DMA_MTOP_SDMA,            TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_BTOP_DMA_SDMA,            TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_OPI,                      TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_QSPI2,                    TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_QSPI1,                    TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_QSPI0,                    TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_SPI2_2CS,                 TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_SPI1_3CS,                 TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_SPI0_3CS,                 TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_CACHE_H2H,                TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_CACHE,                    TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_MAIN_AXI_GLB,             TURN_OFF},
    {HAL_CLOCKS_MODULE_AUTO_CG_DSP_SHARE,                TURN_OFF},
};
#endif

cmu_pll_config_t clocks_pll_cfg_get(void)
{
    return g_pll_cfg;
}

#ifdef BUILD_APPLICATION_SSB
clocks_clk_cfg_t const *clocks_ssb_clk_cfg_get(void)
{
    return g_ssb_clk_cfg;
}
#else
clocks_clk_cfg_t const *clocks_system_all_clocks_get(system_clocks_config_t clk_level)
{
    return g_system_all_core_clocks[clk_level];
}

system_clocks_freq_config_t clocks_system_freq_cfg_get(system_clocks_freq_t clk_level)
{
    return g_system_clocks_freq_config[clk_level];
}

system_display_freq_config_t clocks_system_display_freq_get(system_display_freq_t clk_level)
{
    return g_system_display_freq_config[clk_level];
}

system_mcu_core_clk_cfg_t clocks_system_mcu_freq_get(system_mcu_freq_t clk_level)
{
    return g_system_mcu_freq_config[clk_level];
}

clocks_clk_cfg_t clocks_system_sdio_freq_get(system_sdio_freq_t clk_level)
{
    return g_system_sdio_freq_config[clk_level];
}

system_mcu_perip_clk_t *clocks_system_mcu_perip_clock_config_get(uint32_t *length)
{
    *length = sizeof(g_system_mcu_perp_clk_set_config) / sizeof(system_mcu_perip_clk_t);
    return g_system_mcu_perp_clk_set_config;
}

#if (CLK_AUTO_CG_ENABLE == YES)
system_clk_cg_t const *clock_system_cg_config_get(uint32_t *length)
{
    *length = sizeof(g_system_clkcg_config) / sizeof(system_clk_cg_t);
    return g_system_clkcg_config;
}
#endif
#endif
#endif