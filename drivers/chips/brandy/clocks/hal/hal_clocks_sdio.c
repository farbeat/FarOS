/*
 * Copyright (c) @CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description:   HAL SDIO CLOCK DRIVER.
 */

#include "hal_clocks_sdio.h"
#include "soc_osal.h"
#include "hal_clocks.h"
#include "hal_clocks_app.h"
#include "hal_clocks_glb.h"
#include "hal_reg_config.h"

#define CMU_CLOCKS_EMMC_CCLK_DIV_REG            (M_CTL_RB_BASE + 0x100)
#define CMU_CLOCKS_EMMC_HOST_CLKEN              0
#define CMU_CLOCKS_EMMC_CCLK_ORI_DIV_EN         1
#define CMU_CLOCKS_EMMC_CCLK_ORI_LOAD_DIV_EN    2
#define CMU_CLOCKS_EMMC_TUNING_COARSE_MODE      3
#define CMU_CLOCKS_EMMC_CCLK_ORI_DIV_NUM        4
#define CMU_CLOCKS_EMMC_CCLK_ORI_DIV_NUM_LEN    8

#define CMU_CLOCKS_EMMC_TUNING_BM0_REG          (M_CTL_RB_BASE + 0x104)
#define CMU_CLOCKS_EMMC_TUNING_BM1_REG          (M_CTL_RB_BASE + 0x108)

#define CMU_CLOCKS_SD_HOST_CCLK_DIV_REG         (M_CTL_RB_BASE + 0x110)
#define CMU_CLOCKS_SDIO_HOST_CLKEN              0
#define CMU_CLOCKS_SDIO_CCLK_ORI_DIV_EN         1
#define CMU_CLOCKS_SDIO_LOAD_DIV_EN             2
#define CMU_CLOCKS_SDIO_TUNING_COARSE_MODE      3
#define CMU_CLOCKS_SDIO_CCLK_ORI_DIV_NUM        4
#define CMU_CLOCKS_SDIO_CCLK_ORI_DIV_NUM_LEN    8

#define CMU_CLOCKS_SD_HOST_TUNING_BM0_REG       (M_CTL_RB_BASE + 0x114)
#define CMU_CLOCKS_SD_HOST_TUNING_BM1_REG       (M_CTL_RB_BASE + 0x118)

#define CMU_CLOCKS_SDIO_BUS_DIV_REG             (M_CTL_RB_BASE + 0x120)
#define CMU_CLOCKS_SDIO_BUS_DIV_EN              0
#define CMU_CLOCKS_SDIO_BUS_DIV_LOAD_EN         1
#define CMU_CLOCKS_SDIO_BUS_DIV_NUM             4
#define CMU_CLOCKS_SDIO_BUS_DIV_NUM_LEN         5

#define CMU_CLOCKS_SDIO_TM_DIV_REG              (M_CTL_RB_BASE + 0x124)
#define CMU_CLOCKS_SDIO_TM_DIV_EN               0
#define CMU_CLOCKS_SDIO_TM_DIV_LOAD_EN          1
#define CMU_CLOCKS_SDIO_TM_DIV_NUM              4
#define CMU_CLOCKS_SDIO_TM_DIV_NUM_LEN          9

#define CMU_CLOCKS_SDIO_CRG_SOFT_RST_REG        (M_CTL_RB_BASE + 0x128)
#define CMU_CLOCKS_SDIO_RST_QSPI1_BIT           1

#define HAL_CLOCKS_SDIO_TIMEOUT 1000

void hal_clocks_emmc_host_clken(switch_type_t clk_en)
{
    hal_reg_config_bit(CMU_CLOCKS_EMMC_CCLK_DIV_REG, clk_en, (REG16_POS)CMU_CLOCKS_EMMC_HOST_CLKEN);
}

void hal_clocks_set_emmc_cclk_div(uint8_t div)
{
    uint32_t irq_sts = osal_irq_lock();
    reg16_clrbit(CMU_CLOCKS_EMMC_CCLK_DIV_REG, CMU_CLOCKS_EMMC_CCLK_ORI_DIV_EN);
    reg16_clrbit(CMU_CLOCKS_EMMC_CCLK_DIV_REG, CMU_CLOCKS_EMMC_CCLK_ORI_LOAD_DIV_EN);
    reg16_setbits(CMU_CLOCKS_EMMC_CCLK_DIV_REG, CMU_CLOCKS_EMMC_CCLK_ORI_DIV_NUM,
                  CMU_CLOCKS_EMMC_CCLK_ORI_DIV_NUM_LEN, div);
    reg16_setbit(CMU_CLOCKS_EMMC_CCLK_DIV_REG, CMU_CLOCKS_EMMC_CCLK_ORI_DIV_EN);
    reg16_setbit(CMU_CLOCKS_EMMC_CCLK_DIV_REG, CMU_CLOCKS_EMMC_CCLK_ORI_LOAD_DIV_EN);
    osal_irq_restore(irq_sts);
}

void hal_clocks_emmc_tuning_coarse_mode_control(switch_type_t tuning_coarse_mode)
{
    hal_reg_config_bit(CMU_CLOCKS_EMMC_CCLK_DIV_REG, tuning_coarse_mode,
                       (REG16_POS)CMU_CLOCKS_EMMC_TUNING_COARSE_MODE);
}

void hal_clocks_set_emmc_tuning_cclk_bm0(uint16_t tuning_cclk_bm)
{
    uint32_t irq_sts = osal_irq_lock();
    writew(CMU_CLOCKS_EMMC_TUNING_BM0_REG, tuning_cclk_bm);
    osal_irq_restore(irq_sts);
}

void hal_clocks_set_emmc_tuning_cclk_bm1(uint16_t tuning_cclk_bm)
{
    uint32_t irq_sts = osal_irq_lock();
    writew(CMU_CLOCKS_EMMC_TUNING_BM1_REG, tuning_cclk_bm);
    osal_irq_restore(irq_sts);
}

void hal_clocks_sdio_host_clken(switch_type_t clk_en)
{
    hal_reg_config_bit(CMU_CLOCKS_EMMC_CCLK_DIV_REG, clk_en, (REG16_POS)CMU_CLOCKS_EMMC_HOST_CLKEN);
}

void hal_clocks_set_sdio_cclk_div(uint8_t div)
{
    uint32_t irq_sts = osal_irq_lock();
    reg16_clrbit(CMU_CLOCKS_SD_HOST_CCLK_DIV_REG, CMU_CLOCKS_SDIO_CCLK_ORI_DIV_EN);
    reg16_clrbit(CMU_CLOCKS_SD_HOST_CCLK_DIV_REG, CMU_CLOCKS_SDIO_LOAD_DIV_EN);
    reg16_setbits(CMU_CLOCKS_SD_HOST_CCLK_DIV_REG, CMU_CLOCKS_SDIO_CCLK_ORI_DIV_NUM,
                  CMU_CLOCKS_SDIO_CCLK_ORI_DIV_NUM_LEN, div);
    reg16_setbit(CMU_CLOCKS_SD_HOST_CCLK_DIV_REG, CMU_CLOCKS_SDIO_CCLK_ORI_DIV_EN);
    reg16_setbit(CMU_CLOCKS_SD_HOST_CCLK_DIV_REG, CMU_CLOCKS_SDIO_LOAD_DIV_EN);
    reg16_clrbit(CMU_CLOCKS_SD_HOST_CCLK_DIV_REG, CMU_CLOCKS_SDIO_LOAD_DIV_EN);
    osal_irq_restore(irq_sts);
}

void hal_clocks_sdio_tuning_coarse_mode_control(switch_type_t tuning_coarse_mode)
{
    hal_reg_config_bit(CMU_CLOCKS_SD_HOST_CCLK_DIV_REG, tuning_coarse_mode,
                       (REG16_POS)CMU_CLOCKS_SDIO_TUNING_COARSE_MODE);
}

void hal_clocks_set_sdio_tuning_cclk_bm0(uint16_t tuning_cclk_bm)
{
    uint32_t irq_sts = osal_irq_lock();
    writew(CMU_CLOCKS_SD_HOST_TUNING_BM0_REG, tuning_cclk_bm);
    osal_irq_restore(irq_sts);
}

void hal_clocks_set_sdio_tuning_cclk_bm1(uint16_t tuning_cclk_bm)
{
    uint32_t irq_sts = osal_irq_lock();
    writew(CMU_CLOCKS_SD_HOST_TUNING_BM1_REG, tuning_cclk_bm);
    osal_irq_restore(irq_sts);
}

void hal_clocks_set_sdio_bus_div(uint8_t div)
{
    uint32_t irq_sts = osal_irq_lock();
    reg16_clrbit(CMU_CLOCKS_SDIO_BUS_DIV_REG, CMU_CLOCKS_SDIO_BUS_DIV_EN);
    reg16_clrbit(CMU_CLOCKS_SDIO_BUS_DIV_REG, CMU_CLOCKS_SDIO_BUS_DIV_LOAD_EN);
    reg16_setbits(CMU_CLOCKS_SDIO_BUS_DIV_REG, CMU_CLOCKS_SDIO_BUS_DIV_NUM,
        CMU_CLOCKS_SDIO_BUS_DIV_NUM_LEN, div);
    reg16_setbit(CMU_CLOCKS_SDIO_BUS_DIV_REG, CMU_CLOCKS_SDIO_BUS_DIV_EN);
    reg16_setbit(CMU_CLOCKS_SDIO_BUS_DIV_REG, CMU_CLOCKS_SDIO_BUS_DIV_LOAD_EN);
    reg16_clrbit(CMU_CLOCKS_SDIO_BUS_DIV_REG, CMU_CLOCKS_SDIO_BUS_DIV_LOAD_EN);
    osal_irq_restore(irq_sts);
}

void hal_clocks_set_sdio_tm_div(uint8_t div)
{
    uint32_t irq_sts = osal_irq_lock();
    reg16_clrbit(CMU_CLOCKS_SDIO_TM_DIV_REG, CMU_CLOCKS_SDIO_TM_DIV_EN);
    reg16_clrbit(CMU_CLOCKS_SDIO_TM_DIV_REG, CMU_CLOCKS_SDIO_TM_DIV_LOAD_EN);
    reg16_setbits(CMU_CLOCKS_SDIO_TM_DIV_REG, CMU_CLOCKS_SDIO_TM_DIV_NUM,
        CMU_CLOCKS_SDIO_TM_DIV_NUM_LEN, div);
    reg16_setbit(CMU_CLOCKS_SDIO_TM_DIV_REG, CMU_CLOCKS_SDIO_TM_DIV_EN);
    reg16_setbit(CMU_CLOCKS_SDIO_TM_DIV_REG, CMU_CLOCKS_SDIO_TM_DIV_LOAD_EN);
    reg16_clrbit(CMU_CLOCKS_SDIO_TM_DIV_REG, CMU_CLOCKS_SDIO_TM_DIV_LOAD_EN);
    osal_irq_restore(irq_sts);
}

void hal_clocks_sdio_crg_rst_control(switch_type_t rst_ctl)
{
    hal_reg_config_bit(CMU_CLOCKS_SDIO_CRG_SOFT_RST_REG, rst_ctl, (REG16_POS)CMU_CLOCKS_SDIO_RST_QSPI1_BIT);
}