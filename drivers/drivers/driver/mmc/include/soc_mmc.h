/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: Platform Config HeadFile
 * Author: @CompanyNameTag
 * Create: 2021-09-03
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#ifndef _BRANDY_SOC_MMC_H
#define _BRANDY_SOC_MMC_H

#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define EMMC_BOARD_DEBUG_ON 0

#define GLB_CTL_M_RB_BASE 0x57000000
#define M_CTL_RB_BASE 0x52000000
#define COM_CTL_RB_BASE 0x55000000

#ifndef BIT
#define BIT(x) (1UL << (uint32_t)(x))
#endif

#define MAX_MMC_NUM 2

#define BRANDY_INTERRUPT_SDIO 76
#define BRANDY_INTERRUPT_EMMC 78

#define S_HGPIO 0x52065800
#define S_HGPIO_PAD 0x5700d000

#define S_HGPIO0_OFFSET 0
#define S_HGPIO0_PAD_OFFSET 0x200

#define S_HGPIO6_OFFSET 0x18
#define S_HGPIO6_PAD_OFFSET 0x218
#define FUNC_SDIO 0x01
#define FUNC_EMMC 0x02
#ifdef PRE_FPGA
#define IE_PINMUX_EN 0x02
#else
#define IE_PINMUX_EN 0xC7
#endif

#define BRANDY_SDIO_REG_BASE 0x52061000
#define BRANDY_SDIO_REG_END 0x52061FFF

#define BRANDY_EMMC_REG_BASE 0x52062000
#define BRANDY_EMMC_REG_END 0x52063FFF
// emmc external clk EMMC_CCLK_DIV
#define BRANDY_EXTERNAL_EMMC_CCLK_DIV     0x52000100
#define EMMC_TUNING_COARSE_MODE_BIT       0xD

#define PERI_EMMC_CRG_RST_CTL (GLB_CTL_M_RB_BASE + 0xc8)
#define SOFT_RST_GLB_SDIO_CRG_N BIT(0)
#define SOFT_RST_GLB_SDIO_LGC_N BIT(1)
#define SOFT_RST_GLB_SDIO_DERESET ((SOFT_RST_GLB_SDIO_CRG_N) | (SOFT_RST_GLB_SDIO_LGC_N))

#define ARGUMENT                (BRANDY_EMMC_REG_BASE + 0x8)
#define CMD                     (BRANDY_EMMC_REG_BASE + 0xE)
#define RESP01                  (BRANDY_EMMC_REG_BASE + 0x10)
#define RESP23                  (BRANDY_EMMC_REG_BASE + 0x14)
#define RESP45                  (BRANDY_EMMC_REG_BASE + 0x18)
#define RESP67                  (BRANDY_EMMC_REG_BASE + 0x1C)
#define PSTATE_REG              (BRANDY_EMMC_REG_BASE + 0x24)
#define HOST_CTRL1              (BRANDY_EMMC_REG_BASE + 0x28)
#define HIGH_SPEED_EN_BIT       0x2
#define EXT_DAT_XFER_BIT        0x5
#define PWR_CTRL                (BRANDY_EMMC_REG_BASE + 0x29)
#define POWR_ON_1_8V            0xBF
#define CLK_CTRL                (BRANDY_EMMC_REG_BASE + 0x2C)
#define TOUT_CTRL               (BRANDY_EMMC_REG_BASE + 0x2E)
#define SW_RST                  (BRANDY_EMMC_REG_BASE + 0x2F)
#define SW_RST_ALL              BIT(0)
#define SW_RST_CMD              BIT(1)
#define SW_RST_DAT              BIT(2)
#define SW_RE_RST_ALL           (SW_RST_ALL | SW_RST_CMD | SW_RST_DAT)
#define NORMAL_INT_STAT         (BRANDY_EMMC_REG_BASE + 0x30)
#define ERROR_INT_STAT          (BRANDY_EMMC_REG_BASE + 0x32)
#define ALL_NORMAL_ERR_INT      0xffff
#define NORMAL_INT_STAT_EN      (BRANDY_EMMC_REG_BASE + 0x34)
#define ERROR_INT_STAT_EN       (BRANDY_EMMC_REG_BASE + 0x36)
#define NORMAL_ERR_INT_EN       0x7EFF
#define NORMAL_INT_SIGNAL_EN    (BRANDY_EMMC_REG_BASE + 0x38)
#define ERROR_INT_SIGNAL_EN     (BRANDY_EMMC_REG_BASE + 0x3A)
#define HOST_CTRL2              (BRANDY_EMMC_REG_BASE + 0x3E)
#define SAMPLE_CLK_SEL_BIT      0x7
#define SIGNALING_EN_BIT        0x3
#define UHS_MODE_SEL_BIT        0x0
#define UHS_MODE_SEL_LEN        0x3
#define HS_SDR                  0x1
#define HS_DDR                  0x4
#define HS200                   0x3
#define HS400                   0x7
#define MBIIU_CTRL              (BRANDY_EMMC_REG_BASE + DWC_MSHC_PTR_VENDOR1 + 0x10)
#define AT_CTRL                 (BRANDY_EMMC_REG_BASE + DWC_MSHC_PTR_VENDOR1 + 0x40)
#define AT_STAT                 (BRANDY_EMMC_REG_BASE + DWC_MSHC_PTR_VENDOR1 + 0x44)
#define EMMC_CTRL               (BRANDY_EMMC_REG_BASE + DWC_MSHC_PTR_VENDOR1 + 0x2C)
#define LEN_3BIT                0x3
#define LEN_8BIT                0x8

#define CMD1_RESP_READY_BIT     0x80000000
#define ERR_INT_BIT             0x8000
#define RETRY_CNT1              1000
#define RETRY_CNT2              10000
#define CMD_COMPLETE            0x1
#define XFER_COMPLETE           0x2
#define CMD_XFER_COMPLETE       (CMD_COMPLETE | XFER_COMPLETE)
#define CMD_INHIBIT             0x1
#define CMD_INHIBIT_DAT         0x2
#define DAT_LINE_ACTIVE         0x4
#define CMD_DATA_NOT_READY      (CMD_INHIBIT | CMD_INHIBIT_DAT | DAT_LINE_ACTIVE)
#define CMD_DATA_INHIBIT        (CMD_INHIBIT | CMD_INHIBIT_DAT)
#define CMD6_4BIT_WIDTH_ARGS    0x3b70100
#define CMD6_8BIT_WIDTH_ARGS    0x3b70200
#define CMD6_TIMING_HS_ARGS     0x3b90100
#define CMD6_TIMING_HS200_ARGS  0x3b90200
#define CMD6_TIMING_HS400_ARGS  0x3b90300
#define CMD0_INDEX              0x10
#define CMD1_INDEX              0x102
#define CMD1_ARGS               0xC0FF8080
#define CMD2_INDEX              0x209
#define CMD3_INDEX              0x31a
#define CMD3_ARGS               0x10000
#define CMD9_INDEX              0x909
#define CMD9_ARGS               0x10000
#define CMD7_INDEX              0x71a
#define CMD7_ARGS               0x10000
#define CMD6_INDEX              0x61b

#define EMMC_CCLK_DIV_REG (M_CTL_RB_BASE + 0x0100)
#define EMMC_CCLK_ORI_DIV_NUM_MASK 0x0FF0
#define EMMC_CCLK_ORI_DIV_NUM_2 0x20
#define EMMC_TUNING_BM0_REG (M_CTL_RB_BASE + 0x104)
#define EMMC_TUNING_BM1_REG (M_CTL_RB_BASE + 0x108)
#define SD_HOST_CCLK_DIV_REG (M_CTL_RB_BASE + 0x0110)
#define SDIO_HOST_CCLK_DIV_REG (M_CTL_RB_BASE + 0x014C)

#define SDIO_HOST_CLKEN BIT(0)
#define SDIO_CCLK_ORI_DIV_EN BIT(1)
#define SDIO_CCLK_ORI_LOAD_DIV_EN BIT(2)
#define SDIO_HOST_DIV_EN BIT(0)
#define SDIO_HOST_DIV_SHFIT 1
#define SDIO_HOST_DIV_MASK 0xE

#define EMMC_DIV_REG (M_CTL_RB_BASE + 0x0120)
#define SDIO_BUS_CGDIV_REG (M_CTL_RB_BASE + 0x0124)
#define SD_BUS_CLKEN BIT(0)
#define SD_BUS_DIV_EN BIT(1)
#define SD_BUS_LOAD_DIV_EN BIT(8)
#define SD_BUS_DIV_NUM_MASK 0xF8 // bit3---bit7
#define SD_BUS_DIV_NUM_POST_SHIFT 3

#define EMMC_TUNING_BM0_VAL (0x4244)
#define EMMC_TUNING_BM1_VAL (0x777)
#define EMMC_TUNING_COARSE_MODE BIT(13)

#define DWC_MSHC_PTR_VENDOR1 0x500
#define SDHCI_MSHC_CTRL (DWC_MSHC_PTR_VENDOR1 + 0x08)
#define SDHCI_AXI_MBIIU_CTRL (DWC_MSHC_PTR_VENDOR1 + 0x10)

#define SDHCI_AXI_MBIIU_BURST_INCR16_EN 8
#define SDHCI_GM_WR_OSRC_LMT_MASK (0x7u << 24)
#define SDHCI_GM_WR_OSRC_LMT_SEL(x) ((x) << 24)
#define SDHCI_GM_RD_OSRC_LMT_MASK (0x7u << 16)
#define SDHCI_GM_RD_OSRC_LMT_SEL(x) ((x) << 16)
#define SDHCI_UNDEFL_INCR_EN 0x1
#define SDHCI_GM_WR_OSRC_LMT_VAL (0x7u << 24)
#define SDHCI_GM_RD_OSRC_LMT_VAL (0x7u << 16)

#define COM_GP_REG0_ADDR (COM_CTL_RB_BASE + 0x10)
#ifdef PRE_FPGA
#define EMMC_SDIO_RAM_CONTROL_MASK 1
#else
#define EMMC_SDIO_RAM_CONTROL_MASK 8
#endif
#define EMMC_CARD_CLK_INV_SET BIT(14)

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

#define CONFIG_MMC1_CCLK_MAX 32000000 // 32MHz
#define SDIO_EMMC_DIV_NUM_MASK 0xFF0

#define SDIOM_SRC_CLK_LOW_FREQ_VAL 32000000 // 32MHz
#define SDIOM_SRC_CLK_FREQ_LVL_50M_VAL 52000000 // 52MHz
#define SDIOM_SRC_CLK_FREQ_LVL_64M_VAL 64000000 // 64MHz
#define SDIOM_SRC_CLK_FREQ_LVL_49M_VAL 49000000
#define SDIOM_SRC_CLK_FREQ_LVL_98M_VAL 98000000 // 98MHz
#define SDIOM_SRC_CLK_HIGH_FREQ_LVL_VAL 294000000 // 294MHz

#define SDHCI_CMD_CONFLIT_CHECK 0x01

#define DRV_PHASE_112P5_DEGREE 10 /* 112.5 degree */
#define DRV_PHASE_258P75_DEGREE 23 /* 258.75 degree */
#define DRV_PHASE_225_DEGREE 20 /* 225 degree */
#define DRV_PHASE_180_DEGREE 16  /* 180 degree */

#define SAMPLE_PHASE 4
#define DEFAULT_PHASE 0x10

#define MSHC_WAIT_INT_PER_MSEC 1

#define EMMC_CAPCITY_SIZE (4ULL * GB)    /* emmc capcity size, unit: bytes */
#define RESERVED_BOOT_SECTOR 2048        /* reserved sector number for boot */
#define SECTOR_NUM_PER_CLUSTER 8         /* cluster size, unit: sector */
#define GB (1024ULL * 1024ULL * 1024ULL)

int emmc_drv_context_init(void);
void init_emmc_pinmux(void);

void sdio_drv_context_init(void);
void init_sdio_pinmux(void);
void mmc_set_internal_clk(uint16_t clk_level);
void mmc_set_first_init_flag(bool flag);
bool mmc_get_first_init_flag(void);
void mmc_set_sleep_init_flag(bool flag);
bool mmc_get_sleep_init_flag(void);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _BRANDY_MMC_H */

