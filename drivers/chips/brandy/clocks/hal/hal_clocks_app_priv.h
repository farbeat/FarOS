/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:   HAL APP CLOCK PRIVATE HEADER.
 */
#ifndef HAL_CLOCKS_APPLICATION_PRIVATE_H
#define HAL_CLOCKS_APPLICATION_PRIVATE_H

#include "hal_clocks.h"

/**
 * @addtogroup connectivity_drivers_hal_clocks
 * @{
 */
#define CMU_CLOCKS_M_CLKEN0_REG                 (M_CTL_RB_BASE + 0x40)
#define CMU_CLOCKS_M_CLKEN1_REG                 (M_CTL_RB_BASE + 0x44)
#define CMU_CLOCKS_M_CLKEN2_REG                 (M_CTL_RB_BASE + 0x48)

#define CMU_CLOCKS_M_SOFT_TRG_REG               (M_CTL_RB_BASE + 0x50)
#define CMU_CLOCKS_M_SOFT_RST_PULSE_WIDTH_BIT   8
#define CMU_CLOCKS_M_SOFT_RST_PULSE_WIDTH_LEN   8

#define CMU_CLOCKS_M_SOFT_RST_REG               (M_CTL_RB_BASE + 0x54)
#define CMU_CLOCKS_M_SOFT_RST1_REG              (M_CTL_RB_BASE + 0x58)

#define CMU_CLOCKS_B_CRG_CLKEN1_REG             (COM_CTL_RB_BASE + 0xAE8)
#define CMU_CLOCKS_B_BUS_CLKEN_BIT              0

#define CMU_CLOCKS_COM_DLL2_REG                 (COM_CTL_RB_BASE + 0x600)
#define CMU_CLOCKS_COM_XO_DLL2_EN_BIT           0
#define CMU_CLOCKS_COM_XO_DLL2_PH_SEL_BIT       1
#define CMU_CLOCKS_COM_XO_DLL2_PH_SEL_LEN       2

#define CMU_CLOCKS_MCU_BUS_DIV_REG              (COM_CTL_RB_BASE + 0x604)
#define CMU_CLOCKS_MCU_BUS_LOAD_DIV_EN_BIT      0
#define CMU_CLOCKS_MCU_BUS_DIV_NUM_BIT          1
#define CMU_CLOCKS_MCU_BUS_DIV_NUM_LEN          3

#define CMU_CLOCKS_GLB_CLKEN_REG                (COM_CTL_RB_BASE + 0x608)

#define COM_BUS_LP_CFG_REG                      (COM_CTL_RB_BASE + 0xAC8)

#define CACHE_BUS_LP_CFG_REG                    (COM_CTL_RB_BASE + 0x470)
#define CACHE_AUTO_CG_BYPASS_BIT                0
#define MEM_AHB_AUTO_CG_BYPASS_BIT              8

#define AON_BUS_LP_CFG_REG                      (COM_CTL_RB_BASE + 0x480)
#define B_BUS_LP_CFG_REG                        (COM_CTL_RB_BASE + 0x490)
#define MCU_MAIN_AHB_LP_CLR_REG                 (COM_CTL_RB_BASE + 0x4A0)
#define SDIO_M_BUS_LP_CFG_REG                   (COM_CTL_RB_BASE + 0x4B0)
#define SDIO_CFG_BUS_LP_CFG_REG                 (COM_CTL_RB_BASE + 0x4C0)
#define DIS_BUS_LP_CFG_REG                      (COM_CTL_RB_BASE + 0x4D0)
#define MCU_CFG_BUS_LP_CFG_REG                  (COM_CTL_RB_BASE + 0x4E0)
#define MAIN_AXI_DEFAULT_SLAVE_REG              (COM_CTL_RB_BASE + 0x4F0)
#define DIS_AXI_DEFAULT_SLAVE_REG               (COM_CTL_RB_BASE + 0x4F4)
#define MCU_PERP_AXI_DEFAULT_SLAVE_REG          (COM_CTL_RB_BASE + 0x4F8)
#define EH2H_AUTO_CG_CFG_REG                    (COM_CTL_RB_BASE + 0x500)

#define MCU_AUTO_CG_CFG                         (COM_CTL_RB_BASE + 0x9C0)
#define MCU_APB_AUTO_CG_BYPASS_EN_BIT             5
#define AH2H_MCU_SUB_TAON_AUTOCG_BYPASS_EN_BIT    3
#define AH2H_MCU_SUB_SEC_AUTOCG_BYPASS_EN_BIT     2
#define SH2H_MCU_SUB_MFSH2H_AUTOCG_BYPASS_EN_BIT  1
#define SH2H_MCU_SUB_SFH2H_AUTOCG_BYPASS_EN_BIT   0
#define MCU_PERP_AXI_NEW_AUTO_CG                (COM_CTL_RB_BASE + 0x9C4)
#define MCU_PERP_AXI_GLB_CG_BYPASS_BIT          0
#define DMA_AUTO_CG_CFG                         (COM_CTL_RB_BASE + 0xA80)
#define SDMA_MTOP_SDMA_AUTO_CG_BYPASS_EN_BIT    2
#define DMA_MTOP_SDMA_AUTO_CG_BYPASS_EN_BIT     1
#define BTOP_DMA_SDMA_AUTO_CG_BYPASS_EN_BIT     0
#define SPI_AUTO_CG_CFG                         (COM_CTL_RB_BASE + 0xA9C)
#define OPI_AUTO_CG_BYPASS_EN_BIT               8
#define QSPI2_AUTO_CG_BYPASS_EN_BIT             6
#define QSPI1_AUTO_CG_BYPASS_EN_BIT             5
#define QSPI0_AUTO_CG_BYPASS_EN_BIT             4
#define SPI2_2CS_AUTO_CG_BYPASS_EN_BIT          2
#define SPI1_3CS_AUTO_CG_BYPASS_EN_BIT          1
#define SPI0_3CS_AUTO_CG_BYPASS_EN_BIT          0
#define CACHE_BUS_LP_CFG                        (COM_CTL_RB_BASE + 0x470)
#define CACHE_AUTO_CG_BYPASS_EN_H2H_BIT         5
#define CACHE_AUTO_CG_BYPASS_EN_BIT             0
#define MAIN_AXI_NEW_AUTO_CG                    (COM_CTL_RB_BASE + 0x904)
#define MAIN_AXI_GLB_CG_BYPASS_BIT              0
#define MEM_SUB_AUTO_CG_CFG                     (COM_CTL_RB_BASE + 0x908)
#define DSP_SHARE_AUTO_CG_BY_PASS_EN            3

#define CMU_CLOCKS_MODULE_AUTO_CG_BIT           0

#define M_AXI_BRG_LP_CFG0_REG                   (M_CTL_RB_BASE + 0x180)
#define MAIN_AXI_S1_BIT                         0
#define MAIN_AXI_S2_BIT                         1
#define MAIN_AXI_S8_BIT                         2
#define MDMA_H2X_BIT                            3
#define MAXI2MAHB_AH2X_BIT                      4
#define M_AXI_BRG_LP_CFG1_REG                   (M_CTL_RB_BASE + 0x184)
#define SDIO_M_AH2X_BIT                         0
#define MCPU_X2X_BIT                            1
#define AUDIO_BRG_LP_CFG_REG                    (COM_CTL_RB_BASE + 0x1B8)
#define COM_AHB_C2D_BIT                         0
#define COM_AHB_D2C_BIT                         1

#define CMU_CLOCKS_COM_CR_CH_EN_BIT             0
#define CMU_CLOCKS_COM_CR_CLK_SEL_BIT           1
#define CMU_CLOCKS_COM_CR_CLK_SEL_LEN           3
#define CMU_CLOCKS_COM_CR_CLK_DIV_EN_BIT        4
#define CMU_CLOCKS_COM_CR_CLK_DIV_NUM_BIT       5
#define CMU_CLOCKS_COM_CR_CLK_DIV_NUM_LEN       4
#define CMU_CLOCKS_CODEC_CR_CLK_DIV_NUM_LEN     7
#define CMU_CLOCKS_COM_CR_CH_SEL_STS_BIT        12
#define CMU_CLOCKS_COM_CR_CH_SEL_STS_LEN        2

#define CMU_CLOCK_XIP_CACHE_SYNC_MODE_REG       (XIP_CACHE_CTL + 0x14)
#define CMU_CLOCK_MEM_CLKEN0_REG                (XIP_CACHE_CTL + 0x20)
#define CMU_CLOCK_MEM_CLKEN1_REG                (XIP_CACHE_CTL + 0x24)
#define XIP_CACHE_DIV_2_FREQ_VAL                2
#define XIP_CACHE_DIV_NUM_BEGIN_BIT             0x4
#define XIP_CACHE_DIV_NUM_LEN                   0x2
#define CMU_CLOCK_MEM_SOFT_RST_REG              (XIP_CACHE_CTL + 0x30)

#define PMU1_APERP_32K_SEL_MAN_REG              (PMU1_CTL_RB_BASE + 0xE4)
#define PMU1_APERP_32K_SEL_REG                  (PMU1_CTL_RB_BASE + 0xE8)
#define PMU1_APERP_32K_SEL_STS_REG              (PMU1_CTL_RB_BASE + 0xEC)
#define PMU1_APERP_32K_SEL_BIT                  7


#define PMU2_PLL_CLK_EN_CFG                     (PMU2_CMU_CTL_RB_BASE + 0x020)
#define PMU2_PLL_CLKEN_BIT                      0

/**
 * @brief  Application peripheral clocks.
 */
typedef enum {
    /* Clocks M_CLKEN0 region */
    HAL_CLOCKS_APP_MCLK_EN0_BASE,
    HAL_CLOCKS_APP_SEC_CLK          = HAL_CLOCKS_APP_MCLK_EN0_BASE + 0,
    HAL_CLOCKS_APP_DIAG_CLK         = HAL_CLOCKS_APP_MCLK_EN0_BASE + 1,
    HAL_CLOCKS_APP_PWM_CLK          = HAL_CLOCKS_APP_MCLK_EN0_BASE + 7,
    HAL_CLOCKS_APP_X2H_MEM_BUS_CLK  = HAL_CLOCKS_APP_MCLK_EN0_BASE + 13,

    /* Clocks M_CLKEN1 region */
    HAL_CLOCKS_APP_MCLK_EN1_BASE,
    HAL_CLOCKS_APP_TIMER_CLK        = HAL_CLOCKS_APP_MCLK_EN1_BASE + 0,
    HAL_CLOCKS_APP_SPI0_M_CLK       = HAL_CLOCKS_APP_MCLK_EN1_BASE + 1,
    HAL_CLOCKS_APP_SPI1_MS_CLK      = HAL_CLOCKS_APP_MCLK_EN1_BASE + 2,
    HAL_CLOCKS_APP_SPI2_MS_CLK      = HAL_CLOCKS_APP_MCLK_EN1_BASE + 3,
    HAL_CLOCKS_APP_UART_H0_CLK      = HAL_CLOCKS_APP_MCLK_EN1_BASE + 4,
    HAL_CLOCKS_APP_UART_H1_CLK      = HAL_CLOCKS_APP_MCLK_EN1_BASE + 5,
    HAL_CLOCKS_APP_I2C0_CLK         = HAL_CLOCKS_APP_MCLK_EN1_BASE + 6,
    HAL_CLOCKS_APP_I2C1_CLK         = HAL_CLOCKS_APP_MCLK_EN1_BASE + 7,
    HAL_CLOCKS_APP_I2C2_CLK         = HAL_CLOCKS_APP_MCLK_EN1_BASE + 8,
    HAL_CLOCKS_APP_I2C3_CLK         = HAL_CLOCKS_APP_MCLK_EN1_BASE + 9,
    HAL_CLOCKS_APP_QSPI_1CS_CLK     = HAL_CLOCKS_APP_MCLK_EN1_BASE + 10,
    HAL_CLOCKS_APP_XIP_BUS_CLK      = HAL_CLOCKS_APP_MCLK_EN1_BASE + 13,
    HAL_CLOCKS_APP_COM_BUS_DSP_CLK  = HAL_CLOCKS_APP_MCLK_EN1_BASE + 14,
    HAL_CLOCKS_APP_BT_TGTWS_CLK     = HAL_CLOCKS_APP_MCLK_EN1_BASE + 15,

    /* Clocks M_CLKEN2 region */
    HAL_CLOCKS_APP_MCLK_EN2_BASE,
    HAL_CLOCKS_APP_PULSE_CAPTURE_CLK  = HAL_CLOCKS_APP_MCLK_EN2_BASE + 0,
    HAL_CLOCKS_APP_M_DAP_CLK          = HAL_CLOCKS_APP_MCLK_EN2_BASE + 1,
    HAL_CLOCKS_APP_RC_CLK             = HAL_CLOCKS_APP_MCLK_EN2_BASE + 2,
    HAL_CLOCKS_APP_XO_CLK             = HAL_CLOCKS_APP_MCLK_EN2_BASE + 3,
    HAL_CLOCKS_APP_BUTT               = HAL_CLOCKS_APP_XO_CLK + 1
} hal_clocks_app_perips_clk_type_t;

/**
 * @brief  Modules in the common crg.
 */
typedef enum {
    HAL_CLOCKS_MODULE_MCU_CORE,
    HAL_CLOCKS_MODULE_MCU_PERP_LS,
    HAL_CLOCKS_MODULE_MCU_PERP_UART,
    HAL_CLOCKS_MODULE_MCU_PERP_SPI,
    HAL_CLOCKS_MODULE_COM_BUS,
    HAL_CLOCKS_MODULE_SDIOM,
    HAL_CLOCKS_MODULE_MEM_BUS,
    HAL_CLOCKS_MODULE_XIP_OPI,
    HAL_CLOCKS_MODULE_XIP_QSPI,
    HAL_CLOCKS_MODULE_GPU,
    HAL_CLOCKS_MODULE_HIFI,
    HAL_CLOCKS_MODULE_CODEC,
    HAL_CLOCKS_MODULE_PAD_OUT0,       /*!< 1 channel, Common CRG of pad clk out0. */
    HAL_CLOCKS_MODULE_PAD_OUT1,       /*!< 1 channel, Common CRG of pad clk out1. */
    HAL_CLOCKS_MODULE_PAD_OUT2,       /*!< 1 channel, Common CRG of pad clk out2. */
    HAL_CLOCKS_MODULE_MAX,
} hal_clocks_ccrg_module_t;

/**
 * @brief  MCU group clock source in the common crg.
 * @note   The orders of the elements can't be modified as it is defined by the soc in the register list.
 */
typedef enum {
    HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_RC = 0,           /*!< RC clock with speed 49.152M */
    HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_XO = 1,           /*!< TCXO clock with speed 32M. */
    HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_XO_DLL2 = 2,      /*!< TCXO-2x clock with speed 64M. */
    HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_PLL = 3,          /*!<  */
    HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_MAX,
} hal_clocks_ccrg_mcu_group_clk_src_t;

/**
 * @brief  PAD group clock source in the common crg.
 * @note   The orders of the elements can't be modified as it is defined by the soc in the register list.
 */
typedef enum {
    HAL_CLOCKS_CCRG_PAD_GROUP_CLK_SRC_RC,
    HAL_CLOCKS_CCRG_PAD_GROUP_CLK_SRC_32K,
    HAL_CLOCKS_CCRG_PAD_GROUP_CLK_SRC_XO,
    HAL_CLOCKS_CCRG_PAD_GROUP_CLK_SRC_CLKIN,
    HAL_CLOCKS_CCRG_PAD_GROUP_CLK_SRC_PLL,
    HAL_CLOCKS_CCRG_PAD_GROUP_CLK_SRC_MAX,
} hal_clocks_ccrg_pad_group_clk_src_t;

/**
 * @brief  Channels definition of the crg.
 */
typedef enum {
    HAL_CLOCKS_CRG_CH_INVALID = 0,
    HAL_CLOCKS_CRG_CH0 = 1,             /*!< Channel 0, the value can be read in the ch_sel_sts register is 1. */
    HAL_CLOCKS_CRG_CH1 = 2,             /*!< Channel 1, the value can be read in the ch_sel_sts register is 2. */
} hal_clocks_crg_ch_t;

/**
 * @brief  The dsp code index definitions.
 */
typedef enum {
    HAL_CLOCKS_DSP0,    /*!< DSP0 index. */
    HAL_CLOCKS_DSP1,    /*!< DSP1 index. */
    HAL_CLOCKS_DSP_MAX,
} hal_clocks_dsp_index_t;

typedef enum {
    HAL_GLB_CLKEN_MCU_PERP_LS   = 1,
    HAL_GLB_CLKEN_MCU_PERP_UART = 2,
    HAL_GLB_CLKEN_MCU_PERP_SPI  = 3,
    HAL_GLB_CLKEN_SDIOM         = 4,
    HAL_GLB_CLKEN_MEM_BUS       = 5,
    HAL_GLB_CLKEN_XIP_OPI       = 6,
    HAL_GLB_CLKEN_XIP_QSPI      = 7,
    HAL_GLB_CLKEN_GPU           = 8,
    HAL_GLB_CLKEN_HIFI          = 11,
    HAL_GLB_CLKEN_CODEC         = 12,
    HAL_GLB_CLKEN_XIP_OPI_CACHE = 13,
} hal_clocks_glb_clken_t;

/**
 * @brief The xip memory bus clock type.
 */
typedef enum {
    HAL_CLOCKS_MEM_QSPI0_DIV_EN              = 0,
    HAL_CLOCKS_MEM_QSPI0_DIV_CLKEN           = 4,
    HAL_CLOCKS_MEM_QSPI0_CLKEN               = 8,
    HAL_CLOCKS_MEM_OPI_CLKEN                 = 11,
    HAL_CLOCKS_MEM_QSPI0_XIP_CLKEN           = 12,
    HAL_CLOCKS_MEM_GLB_CLKEN_XIP_QSPI_CACHE  = 14,
    HAL_CLOCKS_MEM_OPI_XIP_CLKEN             = 15,
    HAL_CLOCKS_MEM_XIP_CACHE_CLKEN           = 16,
    HAL_CLOCKS_MEM_XIP_SUB_DIAG_CLKEN        = 17,
    HAL_CLOCKS_MEM_PINMUX_CTL_CLKEN          = 18,
    HAL_CLOCKS_MEM_XIP_CACHE_DIV_EN_CLKEN    = 19,
} hal_clocks_mem_perips_clk_type_t;

/**
 * @brief The xip memory bus reset type.
 */
typedef enum {
    HAL_MEM_SOFT_RST_QSPI0_XIP      = 0,

    HAL_MEM_SOFT_RST_OPI_XIP        = 3,
    HAL_MEM_SOFT_RST_XIP_CACHE      = 4,
    HAL_MEM_SOFT_RST_XIP_SUB_DIAG   = 5,
    HAL_MEM_SOFT_RST_PINMUX_CTL     = 6,
} hal_mem_soft_rst_type_t;

/**
 * @brief Clock auto clock gating module name.
 */
typedef enum {
    HAL_CLOCKS_MODULE_AUTO_CG_COM_BUS,
    HAL_CLOCKS_MODULE_AUTO_CG_MEM_AHB,
    HAL_CLOCKS_MODULE_AUTO_CG_AON,
    HAL_CLOCKS_MODULE_AUTO_CG_BT,
    HAL_CLOCKS_MODULE_AUTO_CG_MCU_MAIN,
    HAL_CLOCKS_MODULE_AUTO_CG_MCU_SDIO_M,
    HAL_CLOCKS_MODULE_AUTO_CG_SDIO_CFG,
    HAL_CLOCKS_MODULE_AUTO_CG_DISPLAY,
    HAL_CLOCKS_MODULE_AUTO_CG_MCU_CFG,
    HAL_CLOCKS_MODULE_AUTO_CG_MAIN_AXI,
    HAL_CLOCKS_MODULE_AUTO_CG_DISPLAY_AXI,
    HAL_CLOCKS_MODULE_AUTO_CG_MCU_PERP_AXI,
    HAL_CLOCKS_MODULE_AUTO_CG_EH2H_OPI,
    HAL_CLOCKS_MODULE_AUTO_CG_EH2H_FCOM,
    HAL_CLOCKS_MODULE_AUTO_CG_EH2H_TCOM,
    HAL_CLOCKS_MODULE_AUTO_CG_EH2H_SDIO,
    HAL_CLOCKS_MODULE_AUTO_CG_EH2H_BSUB_FCOM,
    HAL_CLOCKS_MODULE_AUTO_CG_EH2H_BSUB_TCOM,
    HAL_CLOCKS_MODULE_AUTO_CG_MCU_SUB_CFG,
    HAL_CLOCKS_MODULE_AUTO_CG_MCU_PERP_AXI_NEW,
    HAL_CLOCKS_MODULE_AUTO_CG_DMA,
    HAL_CLOCKS_MODULE_AUTO_CG_SPI,
    HAL_CLOCKS_MODULE_AUTO_CG_CACHE_BUS_LP,
    HAL_CLOCKS_MODULE_AUTO_CG_MAIN_AXI_NEW,
    HAL_CLOCKS_MODULE_AUTO_CG_MEM_SUB,
    HAL_CLOCKS_MODULE_AUTO_CG_MCU_APB,
    HAL_CLOCKS_MODULE_AUTO_CG_AH2H_MCU_SUB_TAON,
    HAL_CLOCKS_MODULE_AUTO_CG_AH2H_MCU_SUB_SEC,
    HAL_CLOCKS_MODULE_AUTO_CG_AH2H_SH2H_MCU_SUB_MFSH2H,
    HAL_CLOCKS_MODULE_AUTO_CG_SH2H_MCU_SUB_SFH2H,
    HAL_CLOCKS_MODULE_AUTO_CG_MCU_PERP_AXI_GLB,
    HAL_CLOCKS_MODULE_AUTO_CG_SDMA_MTOP_SDMA,
    HAL_CLOCKS_MODULE_AUTO_CG_DMA_MTOP_SDMA,
    HAL_CLOCKS_MODULE_AUTO_CG_BTOP_DMA_SDMA,
    HAL_CLOCKS_MODULE_AUTO_CG_OPI,
    HAL_CLOCKS_MODULE_AUTO_CG_QSPI2,
    HAL_CLOCKS_MODULE_AUTO_CG_QSPI1,
    HAL_CLOCKS_MODULE_AUTO_CG_QSPI0,
    HAL_CLOCKS_MODULE_AUTO_CG_SPI2_2CS,
    HAL_CLOCKS_MODULE_AUTO_CG_SPI1_3CS,
    HAL_CLOCKS_MODULE_AUTO_CG_SPI0_3CS,
    HAL_CLOCKS_MODULE_AUTO_CG_CACHE_H2H,
    HAL_CLOCKS_MODULE_AUTO_CG_CACHE,
    HAL_CLOCKS_MODULE_AUTO_CG_MAIN_AXI_GLB,
    HAL_CLOCKS_MODULE_AUTO_CG_DSP_SHARE,
} hal_clocks_module_auto_cg_t;

/**
 * @brief  Enable or diable the application peripheral clocks.
 * @param  app_clk The clock need to enable or disable.
 * @param  on TURN_ON indicates to enable clock, otherwise to disable.
 */
void hal_clocks_app_perips_config(hal_clocks_app_perips_clk_type_t app_clk, switch_type_t on);

/**
 * @brief  Get the application peripheral clocks enable status.
 * @return True: enable, false: disable.
 */
bool hal_clocks_app_perips_get_config(hal_clocks_app_perips_clk_type_t app_clk);

/**
 * @brief  Get the register associating to the specified module.
 * @param  module The module that need to get the register.
 * @param  ch The channel to be set, If there's only one channel,
 *         this param should be set to HAL_CLOCKS_CRG_CH_INVALID.
 * @return The register associating to the specified module.
 */
uint32_t hal_clocks_ccrg_reg_get(hal_clocks_ccrg_module_t module, hal_clocks_crg_ch_t ch);

/**
 * @brief  If there're multipule channels in a module, get the current using channel.
 * @param  module The module that need to get the current using channel. Can only be HAL_CLOCKS_MODULE_MCU_CORE\\
 *         HAL_CLOCKS_MODULE_COM_BUS\\HAL_CLOCKS_MODULE_HIFI\\HAL_CLOCKS_MODULE_CODEC.
 * @return For the valid module, return the current using channel,
 *         if the module is invalid, will return HAL_CLOCKS_CRG_CH_INVALID.
 */
hal_clocks_crg_ch_t hal_clocks_ccrg_current_ch_get(hal_clocks_ccrg_module_t module);

/**
 * @brief  Set the devision of dsp core.
 * @param  dsp The dsp core index.
 * @param  div The dsp core devision number.
 * @return The result of set dsp devision, true indicates success, otherwise failed.
 */
bool hal_clocks_dsp_div_set(hal_clocks_dsp_index_t dsp, uint8_t div);

/**
 * @brief  Get the devision of dsp core.
 * @param  dsp The dsp core index.
 * @param  div The dsp core devision number.
 * @return The result of get dsp devision, true indicates success, otherwise failed.
 */
bool hal_clocks_dsp_div_get(hal_clocks_dsp_index_t dsp, uint8_t *div);

/**
 * @brief  Config hifi clocks.
 * @param  clk_source  The selected clock source.
 * @param  div  The divide number.
 */
void hal_clocks_set_hifi_freq(hal_clocks_ccrg_mcu_group_clk_src_t clk_source, uint8_t div);

/**
 * @brief  Module auto cg control libra private.
 * @param  auto_cg_module Config which module.
 * @param  auto_en  Auto cg or not.
 */
void hal_clocks_module_auto_cg_control_private(hal_clocks_module_auto_cg_t auto_cg_module, switch_type_t auto_en);

/**
 * @brief  Get Module perp ls clk src.
 */
uint8_t hal_clocks_ccrg_module_get_src(hal_clocks_ccrg_module_t module);

/**
 * @brief  Get Module perp ls clk div.
 */
uint8_t hal_clocks_ccrg_module_get_div(hal_clocks_ccrg_module_t module);

/**
 * @}
 */
#endif
