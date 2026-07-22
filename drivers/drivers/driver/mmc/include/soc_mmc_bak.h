/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag. 2021-2021. All rights reserved.
 * Description: Mmc module External interface
 * Author: @CompanyNameTag
 * Create: 2021-03-01
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

#ifndef __SOC_MMC_H_
#define __SOC_MMC_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MAX_MMC_NUM    2
#ifndef USE_MMC0
#define USE_MMC0 1
#endif
#ifndef USE_MMC1
#define USE_MMC1 1
#endif

#define IO_MUX_REG_BASE 0x50005000
#define DPAL_CRG_REG_BASE    0x70001000

#ifdef BOARD_CS
#define EMMC_CLK_REG_CS 0x50002318
#define EMMC_CKSEL 0x1 << 9
#define EMMC_CKDIV_MASK 0xff
#define EMMC_CKDIV 0x4
#endif

#define EMMC_CRG_CTRL             (DPAL_CRG_REG_BASE + 0x0150)    // PERI_CRG125(DPAL_CRG_REG_BASE + 0x01F4)
#define  EMMC_CKEN                (1U << 0)                    // SDIO0_CKEN(1<< 28), PERI_CRG106_EMMC_CKEN(1<< 28)
#define  EMMC_SRST_REQ            (1U << 16)                // PERI_CRG106_EMMC_CRG_REQ(1 << 27)

#define EMMC_CRG_P4_DLL_CTRL      (DPAL_CRG_REG_BASE + 0x0154)    // PERI_CRG125(DPAL_CRG_REG_BASE + 0x01F4)
#define  EMMC_DLL_SRST_REQ        (1U << 1)                    // PERI_CRG106_EMMC_DLL_RST(1 << 29)

// eMMC DRV DLL
#define EMMC_CRG_DRV_DLL_CTRL     (DPAL_CRG_REG_BASE + 0x0158)    // PERI_CRG127(DPAL_CRG_REG_BASE + 0x01FC)
#define  EMMC_DRV_CLK_PHASE_SEL_MASK  (0x1f << 15)            // PERI_SAM_DRV_MASK(0x1f << 24)
#define  EMMC_DRV_CLK_PHASE_SEL_SHFT  15                    // PERI_SAM_DRV_SHFT(24)

#define EMMC_CRG_SAM_DLL_CTRL     (DPAL_CRG_REG_BASE + 0x015C)    // PERI_CRG125(DPAL_CRG_REG_BASE + 0x01F4)
#define  EMMC_SAMPL_DLL_SLAVE_EN  (0x1 << 10)                // SDIO_SAMPL_DLL_SLAVE_EN(1 << 16)

#define EMMC_CRG_DS_DLL_CTRL      (DPAL_CRG_REG_BASE + 0x0160)

#define EMMC_CRG_DETECT           (DPAL_CRG_REG_BASE + 0x0164) // PERI_SDIO0_SAMPLB_DLL_CTRL(DPAL_CRG_REG_BASE + 0x1f8)
#define  EMMC_SAMPL_B_CCLK_SEL    (0x1f << 0)                // SDIO_SAMPLB_DLL_CLK_MASK(0x1fU << 0)
#define  EMMC_SAMPL_DLL_CLK       (0x1 << 3)                // SDIO_SAMPLB_DLL_CLK(1 << 3)
#define  EMMC_SAMPL_SEL(phase)    ((phase) << 0)            // SDIO_SAMPLB_SEL(phase)      ((phase) << 0)

#define EMMC_CRG_STATE            (DPAL_CRG_REG_BASE + 0x0168)    // PERI_SDIO0_DRV_DLL_CTRL(DPAL_CRG_REG_BASE + 0x210),

#define  EMMC_P4_DLL_LOCKED          (0x1 << 9)                // SDIO_DRV_DLL_LOCK(1 << 15)
#define  EMMC_SAM_DLL_READY          (0x1 << 12)                // SDIO_SAMPL_DLL_SLAVE_READY(1 << 0)

#ifndef MMC_CRG_REG_BASE
#define MMC_CRG_REG_BASE          EMMC_CRG_CTRL
#endif

#ifndef SDIO_CRG_REG_BASE
#define SDIO_CRG_REG_BASE         0x70001000
#endif

// SDIO1 DRV DLL
#define PERI_SDIO1_CRG136 (SDIO_CRG_REG_BASE + 0x0220)    /* SDIO1 only */
#ifdef BOARD_CS
#define A_SDIO_HOST_CTRL (SDIO_CRG_REG_BASE + 0x0190)    /* SDIO for CS */
#else
#define A_SDIO_HOST_CTRL (SDIO_CRG_REG_BASE + 0x022C)    /* SDIO1 only */
#endif
#define PERI_SDIO1_SAMPLB_DLL_CTRL   (SDIO_CRG_REG_BASE + 0x21c) /* SDIO1 only */
#define PERI_SDIO1_DRV_DLL_CTRL      (SDIO_CRG_REG_BASE + 0x228) /* SDIO1 only */
#define PERI_SDIO1_SAMPL_DLL_STATUS  (SDIO_CRG_REG_BASE + 0x224) /* SDIO1 only */

#define MMC_FREQ_400K       400000
#define MMC_FREQ_25M        25000000
#define MMC_FREQ_50M        50000000
/* only support for EMMC chip */
#define MMC_FREQ_80M        80000000
#define MMC_FREQ_112P5M     112500000
#define MMC_FREQ_150M       150000000

#define SDIO0_CLK_SEL_100K  (0)
#define SDIO0_CLK_SEL_400K  (7)
#define SDIO0_CLK_SEL_25M   (1)
#define SDIO0_CLK_SEL_50M   (2)
#define SDIO0_CLK_SEL_100M  (3)
#define SDIO0_CLK_SEL_125M  (4)
#define SDIO0_CLK_SEL_150M  (5)
#define SDIO0_CLK_SEL_200M  (6)

#define REG_CTRL_BASE           IO_MUX_REG_BASE
#define REG_DRV_CAP_BASE        (REG_CTRL_BASE + 0x800)

#define REG_CTRL_SDIO0_CLK      (IO_MUX_REG_BASE + 0x910)
#define REG_CTRL_SDIO0_CMD      (IO_MUX_REG_BASE + 0x914)
#define REG_CTRL_SDIO0_DATA0    (IO_MUX_REG_BASE + 0x900)
#define REG_CTRL_SDIO0_DATA1    (IO_MUX_REG_BASE + 0x904)
#define REG_CTRL_SDIO0_DATA2    (IO_MUX_REG_BASE + 0x908)
#define REG_CTRL_SDIO0_DATA3    (IO_MUX_REG_BASE + 0x90C)

#define REG_CTRL_EMMC_CLK       (REG_DRV_CAP_BASE + 0x24)
#define REG_CTRL_EMMC_CMD       (REG_DRV_CAP_BASE + 0x20)
#define REG_CTRL_EMMC_DATA0     (REG_DRV_CAP_BASE + 0x0)
#define REG_CTRL_EMMC_DATA1     (REG_DRV_CAP_BASE + 0x04)
#define REG_CTRL_EMMC_DATA2     (REG_DRV_CAP_BASE + 0x08)
#define REG_CTRL_EMMC_DATA3     (REG_DRV_CAP_BASE + 0x0C)
#define REG_CTRL_EMMC_DATA4     (REG_DRV_CAP_BASE + 0x10)
#define REG_CTRL_EMMC_DATA5     (REG_DRV_CAP_BASE + 0x14)
#define REG_CTRL_EMMC_DATA6     (REG_DRV_CAP_BASE + 0x18)
#define REG_CTRL_EMMC_DATA7     (REG_DRV_CAP_BASE + 0x1C)

#define REG_CTRL_EMMC_DS        (REG_DRV_CAP_BASE + 0x2C)
#define REG_CTRL_EMMC_RST       (REG_DRV_CAP_BASE + 0x28)

// macro for io_mux
#define BIT(n)                  (1U << (n))
#define IO_CFG_PULL_DOWN        BIT(10)
#define IO_CFG_PULL_UP          BIT(9)
#define IO_CFG_SR               BIT(8)

#define IO_CFG_DRV_STR_MASK     (0xfU << 4)
#define IO_DRV_MASK             (0x7f0)

#define IO_DRV_STR_SEL(str)     ((str) << 4)

#define IO_MUX_CLK_TYPE_EMMC    0x0
#define IO_MUX_CLK_TYPE_SD      0x1
#define IO_MUX_SHIFT(type)      ((type) << 0)
#define IO_MUX_MASK             (0xfU << 0)

enum mmc_width_seletion_mode {
    /* The bus width is auto determined by capabilities of a card and host */
    MMC_WIDTH_SELCTION_AUTO = 0,
    /*
     * The bus width of host and card are both manually forced into 1 bit mode
     * whether the card and host supports 4-bit or not.
     */
    MMC_WIDTH_FORCED_1_BIT,
};

/*
 * Description:
 * This API is used to set the bus width selection mode of sd and sdio host devices.
 * Param: host_id [IN]  ranging from 0 to (MAX_MMC_NUM - 1). The types of hosts, either sd or sdio,
 *                      can be inqueried from user's guide according to the param host_id.
 * Param: type    [IN]  the width selection mode, either MMC_WIDTH_SELCTION_AUTO, or MMC_WIDTH_FORCED_1_BIT.
 *                      please refer to enum mmc_width_seletion_mode for more details.
 * Attention: The width selection mode is default auto mode if this API is not called.
 *            This API should be called before initial and registration of mmc driver.
 */
extern void mmc_width_seletion_mode_set(unsigned int host_id, enum mmc_width_seletion_mode type);

enum mmc_type {
    MMC_EMMC,
    MMC_SDIO
};

/*
 * Description:
 * This API is used to get mmc type.
 * Param: none.
 * Attention: none.
 * Return:  MMC_EMMC: emmc to be init; MMC_SDIO: sdio to be init
 */
unsigned int uapi_get_mmc_type(void);

/*
 * Description:
 * This API is used to init emmc host.
 * Param: The type of the device to be initialized
 * Attention: none.
 */
void uapi_mmc_init(enum mmc_type type);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
