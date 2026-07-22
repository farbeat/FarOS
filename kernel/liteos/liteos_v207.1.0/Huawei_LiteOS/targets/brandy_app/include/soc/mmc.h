/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  os mmc header.
 * Author:
 * Create: 2023-03-09
 */

#ifndef _BRANDY_MMC_H
#define _BRANDY_MMC_H

#include "platform_core.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define EMMC_BOARD_DEBUG_ON 0

#define MAX_MMC_NUM 2

#define BRANDY_INTERRUPT_SDIO 76
#define BRANDY_INTERRUPT_EMMC 78

#define S_HGPIO 0x52065800
#define S_HGPIO_PAD 0x5700d000

#define S_HGPIO6_OFFSET 0x18
#define S_HGPIO6_PAD_OFFSET 0x218
#define FUNC_EMMC 0x02
#ifdef PRE_FPGA
#define IE_PINMUX_EN 0x02
#else
#define IE_PINMUX_EN 0xC7
#endif

#define BRANDY_SDIO_REG_BASE 0x52061000
#define BRANDY_EMMC_REG_BASE 0x52062000
#define BRANDY_SDIO_REG_END 0x52061FFF
#define BRANDY_EMMC_REG_END 0x52063FFF

#define PERI_EMMC_CRG_RST_CTL (GLB_CTL_M_RB_BASE + 0xc8)
#define SOFT_RST_GLB_SDIO_CRG_N BIT(0)
#define SOFT_RST_GLB_SDIO_LGC_N BIT(1)
#define SOFT_RST_GLB_SDIO_DERESET ((SOFT_RST_GLB_SDIO_CRG_N) | (SOFT_RST_GLB_SDIO_LGC_N))

#define EMMC_CCLK_DIV_REG (M_CTL_RB_BASE + 0x0100)
#define EMMC_TUNING_BM0_REG (M_CTL_RB_BASE + 0x104)
#define EMMC_TUNING_BM1_REG (M_CTL_RB_BASE + 0x108)
#define EMMC_DIV_REG (M_CTL_RB_BASE + 0x0120)

#define SD_BUS_LOAD_DIV_EN BIT(8)
#define SD_BUS_DIV_NUM_MASK 0xF8 // bit3---bit7
#define SD_BUS_DIV_NUM_POST_SHIFT 3

#define EMMC_BM0_VAL (0x4211)
#define EMMC_BM1_VAL (0x700)
#define EMMC_TUNING_COARSE_MODE BIT(13)

#define DWC_MSHC_PTR_VENDOR1 0x500
#define SDHCI_MSHC_CTRL (DWC_MSHC_PTR_VENDOR1 + 0x08)
#define SDHCI_AXI_MBIIU_CTRL (DWC_MSHC_PTR_VENDOR1 + 0x10)

#define SDHCI_AXI_MBIIU_BURST_INCR16_EN 8
#define SD_HOST_CCLK_DIV_REG (M_CTL_RB_BASE + 0x0110)
#define SDIO_HOST_CLKEN BIT(0)
#define SDIO_CCLK_ORI_DIV_EN BIT(1)
#define SDIO_CCLK_ORI_LOAD_DIV_EN BIT(2)

#define SDIO_BUS_CGDIV_REG (M_CTL_RB_BASE + 0x0124)
#define SD_BUS_CLKEN BIT(0)
#define SD_BUS_DIV_EN BIT(1)
#define SD_BUS_LOAD_DIV_EN BIT(8)

#define COM_GP_REG0_ADDR (COM_CTL_RB_BASE + 0x10)
#ifdef PRE_FPGA
#define EMMC_SDIO_RAM_CONTROL_MASK 1
#else
#define EMMC_SDIO_RAM_CONTROL_MASK 8
#endif
#define EMMC_CARD_CLK_INV_SET BIT(14)

#define SDHCI_ERR_INT_STAT_R 0x32
#define SDHCI_NORMAL_INT_STAT_EN_R 0x34
#define SDHCI_ERROR_INT_STAT_EN_R 0x36

#define DWC_MSHC_PTR_VENDOR1 0x500
#define SDHCI_MSHC_CTRL (DWC_MSHC_PTR_VENDOR1 + 0x08)

#define GLB_CLKEN_REG (COM_CTL_RB_BASE + 0x0608)
#define GLB_CLKEN_SDIOM BIT(4)
#define GLB_CLKEN_DISABLE (0x7fee)
#define GLB_CLKEN_ENABLE 0x7ffe

#define SDIOM_CR_REG (COM_CTL_RB_BASE + 0x0628)
#define SDIOM_CR_CTRL_CH_ENABLE BIT(0)
#define SDIOM_CR_CTRL_DIV_EN BIT(4)

#define SDIOM_CR_CTRL_DIV_OFFSET 5
#define SDIOM_CR_CTRL_MIN_DIV 2

#define SDIOM_CR_CTRL_CLK_RESOURCE_RC 0
#define SDIOM_CR_CTRL_CLK_RESOURCE_XO 1
#define SDIOM_CR_CTRL_CLK_RESOURCE_XO_DLL2 2
#define SDIOM_CR_CTRL_CLK_RESOURCE_PLL 3

#define SDIOM_CR_CTRL_CLK_RESOURCE_PLL_EN (SDIOM_CR_CTRL_CH_ENABLE | (SDIOM_CR_CTRL_CLK_RESOURCE_PLL << 1))
#define SDIOM_CR_CTRL_XO_32M_CLK_EN (SDIOM_CR_CTRL_CH_ENABLE | (SDIOM_CR_CTRL_CLK_RESOURCE_XO << 1))

#define MCU_GPIO3_BASE_ADDR (0x5701c000)

#define SDIO_HOST_CKDIV_REG (M_CTL_RB_BASE + 0x014c)

#define EMMC_EMMC_CCLK_EN BIT(0)
#define EMMC_CCLK_LOAD_DIV_EN BIT(1)

#define SDIO_EMMC_CCLK_EN BIT(0)
#define SDIO_EMMC_ORI_DIV_EN BIT(1)

#define SDIO_DIV_NUM_MASK 0xF8  // bit3---bit7
#define EMMC_DIV_NUM_MASK 0x70  // bit4---bit6

#define SDIO_DIV_NUM_POST_SHIFT 3
#define SDIO_EMMC_DIV_NUM_POST_SHIFT 4

#define OFF_CLK2CARD_ON_DELAY 25

#define CONFIG_MMC0_CCLK_MIN 400000 // 400KHz
#define CONFIG_MMC1_CCLK_MIN 100000 // 100KHz

#define SDIOM_SRC_CLK_LOW_FREQ_VAL 32000000 // 32MHz
#define SDIOM_SRC_CLK_FREQ_LEVEL_50M_VAL 50000000 // 50MHz
#define SDIOM_SRC_CLK_FREQ_LEVEL_64M_VAL 64000000 // 64MHz
#define SDIOM_SRC_CLK_FREQ_LEVEL_98M_VAL 98000000 // 64MHz
#define SDIOM_SRC_CLK_HIGH_FREQ_VAL 196000000 // 196MHz

#define SDHCI_CMD_CONFLIT_CHECK 0x01

#define SDHCI_GM_WR_OSRC_LMT_MASK       (0x7u << 24)
#define SDHCI_GM_WR_OSRC_LMT_SEL(x)     ((x) << 24)
#define SDHCI_GM_RD_OSRC_LMT_MASK       (0x7u << 16)
#define SDHCI_GM_RD_OSRC_LMT_SEL(x)     ((x) << 16)
#define SDHCI_UNDEFL_INCR_EN            0x1
#define SDHCI_GM_WR_OSRC_LMT_VAL        (0x7u << 24)
#define SDHCI_GM_RD_OSRC_LMT_VAL        (0x7u << 16)

#define DRV_PHASE_112P5_DEGREE 10 /* 112.5 degree */
#define DRV_PHASE_258P75_DEGREE 23 /* 258.75 degree */
#define DRV_PHASE_225_DEGREE 20 /* 225 degree */
#define DRV_PHASE_180_DEGREE 16  /* 180 degree */

#define SAMPLE_PHASE 4
#define DEFAULT_PHASE 0x10

#define MSHC_WAIT_INT_PER_MSEC 1

#define RESERVED_BOOT_SECTOR 2048 /* reserved sector number for boot */
#define SECTOR_NUM_PER_CLUSTER 8 /* cluster size, unit: sector */
#define GB (1024ULL * 1024ULL * 1024ULL)
#define EMMC_CAPCITY_SIZE (4ULL * GB) /* emmc capcity size, unit: bytes */

void init_emmc_pinmux(void);

int emmc_drv_format(void);

int emmc_drv_context_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _BRANDY_MMC_H */

