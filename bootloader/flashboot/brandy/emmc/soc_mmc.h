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

#define EMMC_TUNING_BM0_VAL (0x4210)
#define EMMC_TUNING_BM1_VAL (0x707)
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

#define CMU_CLOCKS_SDIO_RST_DEVICE_SD_BIT 0

#define CMU_CLOCK_SOFT_RST6_REG (GLB_CTL_M_RB_BASE + 0xC8)
#define CMU_CLOCK_SOFT_RST_GLB_SDIO_CRG_BIT 0
#define CMU_CLOCK_SOFT_RST_GLB_SDIO_LGC_BIT 1

#define CMU_CLOCKS_SDIO_CRG_SOFT_RST_REG        (M_CTL_RB_BASE + 0x128)
#define CMU_CLOCKS_SDIO_RST_DEVICE_SD_BIT       0

#define brandy_reg32(register)                             (*(volatile uint32_t*)(uintptr_t)(register))
#define brandy_reg16(register)                             (*(volatile uint16_t*)(uintptr_t)(register))
#define brandy_reg8(register)                              (*(volatile uint8_t*)(uintptr_t)(register))

#define brandy_reg32_clrbit(register, pos)           (brandy_reg32(register) &= ~((uint32_t)((1U) << (uint32_t)(pos))))
#define brandy_reg16_clrbit(register, pos)           (brandy_reg16(register) &= ~((uint16_t)((1U) << (uint16_t)(pos))))
#define brandy_reg8_clrbit(register, pos)            (brandy_reg8(register) &= ~((uint8_t)((1U) << (pos))))

#define brandy_reg32_setbit(register, pos)           (brandy_reg32(register) |= ((uint32_t)((1U) << (uint32_t)(pos))))
#define brandy_reg16_setbit(register, pos)           (brandy_reg16(register) |= ((uint16_t)((1U) << (uint16_t)(pos))))
#define brandy_reg8_setbit(register, pos)            (brandy_reg8(register) |= ((uint8_t)((1U) << (pos))))

#define brandy_reg16_setbits(r, p, b, v) (brandy_reg16(r) = (brandy_reg16(r) & \
                                              (uint16_t)(~((((uint32_t)1 << (uint16_t)(b)) - 1) << (uint16_t)(p)))) \
                                              | (uint16_t)((uint32_t)((uint16_t)(v) & \
                                              (((uint32_t)1 << (uint16_t)(b)) - 1)) << (uint16_t)(p)))

#define PMU_TOP_PERIPHERALS_MTOP0_L_REG (M_CTL_RB_BASE + 0x150)
#define PMU_MTOP0_MDMA_RST              2
#define PMU_MTOP0_SMDMA_RST             1
#define PMU_MTOP0_TIMER_RST             0
#define PMU_TOP_PERIPHERALS_MTOP0_H_REG (M_CTL_RB_BASE + 0x154)
#define PMU_MTOP0_MDMA_STS_OFFSET       2
#define PMU_MTOP0_SMDMA_STS_OFFSET      1
#define PMU_MTOP0_MTIMER_STS_OFFSET     0

/* I2C0/1/2/3 peripheral clock gating. */
#define PMU_TOP_PERIPHERALS_MTOP1_L_REG (M_CTL_RB_BASE + 0x158)
#define PMU_MTOP1_I2C3_RST              3
#define PMU_MTOP1_I2C2_RST              2
#define PMU_MTOP1_I2C1_RST              1
#define PMU_MTOP1_I2C0_RST              0
#define PMU_TOP_PERIPHERALS_MTOP1_H_REG (M_CTL_RB_BASE + 0x15C)
#define PMU_MTOP1_I2C3_STS_OFFSET       3
#define PMU_MTOP1_I2C2_STS_OFFSET       2
#define PMU_MTOP1_I2C1_STS_OFFSET       1
#define PMU_MTOP1_I2C0_STS_OFFSET       0

/* SPI0_M, SPI1_MS, SPI2_MS and QSPI2_1CS peripheral clock gating. */
#define PMU_TOP_PERIPHERALS_MTOP2_L_REG (M_CTL_RB_BASE + 0x160)
#define PMU_MTOP2_QSPI2_1CS_RST         3
#define PMU_MTOP2_SPI2_RST              2
#define PMU_MTOP2_SPI1_RST              1
#define PMU_MTOP2_SPI0_RST              0
#define PMU_TOP_PERIPHERALS_MTOP2_H_REG (M_CTL_RB_BASE + 0x164)
#define PMU_MTOP2_QSPI2_1CS_STS_OFFSET  3
#define PMU_MTOP2_SPI2_STS_OFFSET       2
#define PMU_MTOP2_SPI1_STS_OFFSET       1
#define PMU_MTOP2_SPI0_STS_OFFSET       0

/* UART_H0, UART_H1 and UART_L0 peripheral clock gating. */
#define PMU_TOP_PERIPHERALS_MTOP3_L_REG (M_CTL_RB_BASE + 0x168)
#define PMU_MTOP3_UART_L0_RST           2
#define PMU_MTOP3_UART_H1_RST           1
#define PMU_MTOP3_UART_H0_RST           0
#define PMU_TOP_PERIPHERALS_MTOP3_H_REG (M_CTL_RB_BASE + 0x16C)
#define PMU_MTOP3_UART_L0_STS_OFFSET    2
#define PMU_MTOP3_UART_H1_STS_OFFSET    1
#define PMU_MTOP3_UART_H0_STS_OFFSET    0

/* EMMC and SDIO_HOST peripheral clock gating. */
#define PMU_TOP_PERIPHERALS_MTOP4_L_REG (M_CTL_RB_BASE + 0x170)
#define PMU_MTOP4_SDIO_HOST_RST         1
#define PMU_MTOP4_EMMC_RST              0
#define PMU_TOP_PERIPHERALS_MTOP4_H_REG (M_CTL_RB_BASE + 0x174)
#define PMU_MTOP4_SDIO_HOST_STS_OFFSET  1
#define PMU_MTOP4_EMMC_STS_OFFSET       0

/* I2C4 5 6 and spi3 peripheral clock gating. */
#define PMU_TOP_PERIPHERALS_MTOP5_L_REG (M_CTL_RB_BASE + 0x178)
#define PMU_MTOP4_I2C_BUS_4_RST         0
#define PMU_MTOP4_I2C_BUS_5_RST         1
#define PMU_MTOP4_I2C_BUS_6_RST         2
#define PMU_MTOP4_SPI_M3_RST            3

#define CMU_CLOCKS_MSUB_NORMAL_REG  (M_CTL_RB_BASE + 0x3e0)

#define MSUB_CFG_REFRESH_REG        (M_CTL_RB_BASE + 0x3F0)
#define MSUB_CFG_REFRESH_BIT        0

#define CMU_CLOCKS_COM_CR_CH_EN_BIT             0
#define CMU_CLOCKS_COM_CR_CLK_SEL_BIT           1
#define CMU_CLOCKS_COM_CR_CLK_SEL_LEN           3
#define CMU_CLOCKS_COM_CR_CLK_DIV_EN_BIT        4
#define CMU_CLOCKS_COM_CR_CLK_DIV_NUM_BIT       5
#define CMU_CLOCKS_COM_CR_CLK_DIV_NUM_LEN       4
#define CMU_CLOCKS_COM_CR_CH_SEL_STS_BIT        12
#define CMU_CLOCKS_COM_CR_CH_SEL_STS_LEN        2

#define CMU_CLOCKS_COM_BUS_CR_REG               (GLB_CTL_M_RB_BASE + 0x8C)
#define CMU_CLOCKS_MCU_CORE_CR_CH0_REG          (M_CTL_RB_BASE + 0x550)
#define CMU_CLOCKS_MCU_CORE_CR_CH1_REG          (M_CTL_RB_BASE + 0x554)
#define CMU_CLOCKS_MCU_PERP_LS_CR_REG           (M_CTL_RB_BASE + 0x558)
#define CMU_CLOCKS_MCU_PERP_HS_CR_REG           (M_CTL_RB_BASE + 0x55C)
#define CMU_CLOCKS_SDIOM_CR_REG                 (M_CTL_RB_BASE + 0x564)
#define CMU_CLOCKS_MEM_BUS_CR_REG               (M_CTL_RB_BASE + 0x568)
#define CMU_CLOCKS_XIP_OPI_CR_REG               (M_CTL_RB_BASE + 0x56C)
#define CMU_CLOCKS_XIP_QSPI_CR_REG              (M_CTL_RB_BASE + 0x570)
#define CMU_CLOCKS_GPU_CR_REG                   (M_CTL_RB_BASE + 0x574)
#define CMU_CLOCKS_RGB_CR_REG                   (M_CTL_RB_BASE + 0x578)
#define CMU_CLOCKS_HIFI_CR_REG                  (M_CTL_RB_BASE + 0x57C)
#define CMU_CLOCKS_CODEC_CR_REG                 (M_CTL_RB_BASE + 0x580)
#define CMU_CLOCKS_PAD_CLK_OUT0_CR_REG          (M_CTL_RB_BASE + 0x584)
#define CMU_CLOCKS_PAD_CLK_OUT1_CR_REG          (M_CTL_RB_BASE + 0x588)
#define CMU_CLOCKS_PAD_CLK_OUT2_CR_REG          (M_CTL_RB_BASE + 0x58C)

#define EMMC_CAPCITY_SIZE (4ULL * GB)    /* emmc capcity size, unit: bytes */
#define RESERVED_BOOT_SECTOR 2048        /* reserved sector number for boot */
#define SECTOR_NUM_PER_CLUSTER 8         /* cluster size, unit: sector */
#define GB (1024ULL * 1024ULL * 1024ULL)

/** reg32 bit pos types */
typedef enum {
    BRANDY_POS_0,
    BRANDY_POS_1,
    BRANDY_POS_2,
    BRANDY_POS_3,
    BRANDY_POS_4,
    BRANDY_POS_5,
    BRANDY_POS_6,
    BRANDY_POS_7,
    BRANDY_POS_8,
    BRANDY_POS_9,
    BRANDY_POS_10,
    BRANDY_POS_11,
    BRANDY_POS_12,
    BRANDY_POS_13,
    BRANDY_POS_14,
    BRANDY_POS_15
} BRANDY_REG16_POS;

/** PMU_LPM wakeup types */
typedef enum {
    BRANDY_TURN_OFF,
    BRANDY_TURN_ON
} brandy_switch_type_t;

/**
 * @brief  Describe pheriphral request behavior.
 */
typedef enum {
    HAL_PMU_PERIP_REQUEST_RESET = 0,   // !< Request reset the peripheral, make it in initial status, can't running.
    HAL_PMU_PERIP_REQUEST_DERESET = 1, // !< Request dereset the peripheral, make it can run.
    HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER = 2, // !< Request the peripheral enter low power mode.
    HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER = 3,  // !< Request the peripheral exit low power mode.
} hal_pmu_perip_request_type_t;

typedef enum {
    HAL_SOFT_RST6_SDIO_CRG,
    HAL_SOFT_RST6_SDIO_LGC,
} hal_glb_sdio_soft_rst_type_t;

typedef enum {
    BRANDY_CLOCKS_SDIO_FREQ_32M,      // sdio:32M
    BRANDY_CLOCKS_SDIO_FREQ_50M,      // rc clock, real value 49.512MHz
    BRANDY_CLOCKS_SDIO_FREQ_64M,      // sdio:64M
    BRANDY_CLOCKS_SDIO_FREQ_H,        // 193.3M
    BRANDY_CLOCKS_SDIO_FREQ_MAX,
} brandy_system_sdio_freq_t;

/**
 * @brief  Describe mcpu pheriphrals.
 */
typedef enum {
    HAL_PMU_MCPU_PERIP_TIMER        = 0,    // !< The mcpu peripheral timer, can not enter low power mode.
    HAL_PMU_MCPU_PERIP_SMDMA        = 1,    // !< The smcpu peripheral dma
    HAL_PMU_MCPU_PERIP_MDMA         = 2,    // !< The mcpu peripheral dma
    HAL_PMU_MCPU_PERIP_I2C_BUS_0    = 3,    // !< The mcpu peripheral I2C BUS0
    HAL_PMU_MCPU_PERIP_I2C_BUS_1    = 4,    // !< The mcpu peripheral I2C BUS1
    HAL_PMU_MCPU_PERIP_I2C_BUS_2    = 5,    // !< The mcpu peripheral I2C BUS2
    HAL_PMU_MCPU_PERIP_I2C_BUS_3    = 6,    // !< The mcpu peripheral I2C BUS3
    HAL_PMU_MCPU_PERIP_SPI_MST      = 7,    // !< The mcpu peripheral SPI BUS0
    HAL_PMU_MCPU_PERIP_SPI_MS_0     = 8,    // !< The mcpu peripheral SPI BUS1
    HAL_PMU_MCPU_PERIP_SPI_MS_1     = 9,    // !< The mcpu peripheral SPI BUS2
    HAL_PMU_MCPU_PERIP_QSPI2_1CS    = 10,   // !< The mcpu peripheral QSPI BUS2
    HAL_PMU_MCPU_PERIP_UART_H0      = 11,   // !< The mcpu peripheral UARTH0
    HAL_PMU_MCPU_PERIP_UART_H1      = 12,   // !< The mcpu peripheral UARTH1
    HAL_PMU_MCPU_PERIP_UART_L0      = 13,   // !< The mcpu peripheral UARTL0
    HAL_PMU_MCPU_PERIP_EMMC         = 14,   // !< The mcpu peripheral EMMC
    HAL_PMU_MCPU_PERIP_SDIO_HOST    = 15,   // !< The mcpu peripheral SDIO HOST
    HAL_PMU_MCPU_PERIP_I2C_BUS_4    = 16,   // !< The mcpu peripheral I2C BUS4
    HAL_PMU_MCPU_PERIP_I2C_BUS_5    = 17,   // !< The mcpu peripheral I2C BUS5
    HAL_PMU_MCPU_PERIP_I2C_BUS_6    = 18,   // !< The mcpu peripheral I2C BUS6
    HAL_PMU_MCPU_PERIP_SPI_M3       = 19,   // !< The mcpu peripheral SPI BUS3
    HAL_PMU_MCPU_PERIP_MAX,
} hal_pmu_mcpu_perips_type_t;

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

