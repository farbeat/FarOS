/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:   HAL APP CLOCK PRIVATE SOURCE.
 */
#include "hal_clocks_app_priv.h"
#include "hal_reg_config.h"
#include "soc_osal.h"
#include "hal_clocks_app.h"

/* This register not belongs to com crg, need to move to other place. */
#define CMU_CLOCKS_MCU_CORE_CR_CH0_REG          (COM_CTL_RB_BASE + 0x60C)
#define CMU_CLOCKS_MCU_CORE_CR_CH1_REG          (COM_CTL_RB_BASE + 0x610)
#define CMU_CLOCKS_MCU_PERP_LS_CR_REG           (COM_CTL_RB_BASE + 0x614)
#define CMU_CLOCKS_MCU_PERP_UART_CR_REG         (COM_CTL_RB_BASE + 0x618)
#define CMU_CLOCKS_MCU_PERP_SPI_CR_REG          (COM_CTL_RB_BASE + 0x61C)
#define CMU_CLOCKS_COM_BUS_CR_CH0_REG           (COM_CTL_RB_BASE + 0x620)
#define CMU_CLOCKS_COM_BUS_CR_CH1_REG           (COM_CTL_RB_BASE + 0x624)
#define CMU_CLOCKS_SDIOM_CR_REG                 (COM_CTL_RB_BASE + 0x628)
#define CMU_CLOCKS_MEM_BUS_CR_REG               (COM_CTL_RB_BASE + 0x62C)
#define CMU_CLOCKS_XIP_OPI_CR_REG               (COM_CTL_RB_BASE + 0x630)
#define CMU_CLOCKS_XIP_QSPI_CR_REG              (COM_CTL_RB_BASE + 0x634)
#define CMU_CLOCKS_GPU_CR_REG                   (COM_CTL_RB_BASE + 0x638)
#define CMU_CLOCKS_HIFI_CR0_REG                 (COM_CTL_RB_BASE + 0x63C)
#define CMU_CLOCKS_HIFI_CR1_REG                 (COM_CTL_RB_BASE + 0x640)
#define CMU_CLOCKS_CODEC_CR0_REG                (COM_CTL_RB_BASE + 0x644)
#define CMU_CLOCKS_CODEC_CR1_REG                (COM_CTL_RB_BASE + 0x648)
#define CMU_CLOCKS_PAD_CLK_OUT0_CR_REG          (COM_CTL_RB_BASE + 0x64C)
#define CMU_CLOCKS_PAD_CLK_OUT1_CR_REG          (COM_CTL_RB_BASE + 0x650)
#define CMU_CLOCKS_PAD_CLK_OUT2_CR_REG          (COM_CTL_RB_BASE + 0x654)

#define DSP_DIV_NUM_REG                         0x58007000
#define DSP0_DIV_NUM_POS                        0
#define DSP1_DIV_NUM_POS                        16
#define DSP_DIV_NUM_LEN                         6

#define COMM_CLK_DIV_NUM_POS                    0x5
#define COMM_CLK_NUM_LEN                        0x4
#define COMM_CLK_DIV_EN_POS                     0x4
#define COMM_CLK_SRC_POS                        0x1
#define COMM_CLK_SRC_LEN                        0x3
#define CLK_DIV_1                               0x1
#define CODEC_DIV_NUM_LEN                       0x7

#define CMU_CLOCKS_MSUB_NORMAL_REG     (M_CTL_RB_BASE + 0x3e0)
#define CMU_CLOCKS_COMCTRL_CH_SEL_REG  (COM_CTL_RB_BASE + 0xAF0)
#define MSUB_CFG_REFRESH_REG           (M_CTL_RB_BASE + 0x3F0)
#define MSUB_CFG_REFRESH_BIT           0

static uint32_t g_ccrg_modules_reg_map[HAL_CLOCKS_MODULE_MAX] = {
    CMU_CLOCKS_MCU_CORE_CR_CH1_REG,
    CMU_CLOCKS_MCU_PERP_LS_CR_REG,
    CMU_CLOCKS_MCU_PERP_UART_CR_REG,
    CMU_CLOCKS_MCU_PERP_SPI_CR_REG,
    CMU_CLOCKS_COM_BUS_CR_CH1_REG,
    CMU_CLOCKS_SDIOM_CR_REG,
    CMU_CLOCKS_MEM_BUS_CR_REG,
    CMU_CLOCKS_XIP_OPI_CR_REG,
    CMU_CLOCKS_XIP_QSPI_CR_REG,
    CMU_CLOCKS_GPU_CR_REG,
    CMU_CLOCKS_HIFI_CR1_REG,
    CMU_CLOCKS_CODEC_CR1_REG,
    CMU_CLOCKS_PAD_CLK_OUT0_CR_REG,
    CMU_CLOCKS_PAD_CLK_OUT1_CR_REG,
    CMU_CLOCKS_PAD_CLK_OUT2_CR_REG,
};

uint32_t hal_clocks_ccrg_reg_get(hal_clocks_ccrg_module_t module, hal_clocks_crg_ch_t ch)
{
    if (module == HAL_CLOCKS_MODULE_MAX) {
        return 0;
    }
    uint32_t reg_addr = g_ccrg_modules_reg_map[module];

    switch (module) {
        case HAL_CLOCKS_MODULE_MCU_CORE:
            reg_addr = (ch == HAL_CLOCKS_CRG_CH0) ? CMU_CLOCKS_MCU_CORE_CR_CH0_REG : CMU_CLOCKS_MCU_CORE_CR_CH1_REG;
            break;
        case HAL_CLOCKS_MODULE_COM_BUS:
            reg_addr = (ch == HAL_CLOCKS_CRG_CH0) ? CMU_CLOCKS_COM_BUS_CR_CH0_REG : CMU_CLOCKS_COM_BUS_CR_CH1_REG;
            break;
        case HAL_CLOCKS_MODULE_HIFI:
            reg_addr = (ch == HAL_CLOCKS_CRG_CH0) ? CMU_CLOCKS_HIFI_CR0_REG : CMU_CLOCKS_HIFI_CR1_REG;
            break;
        case HAL_CLOCKS_MODULE_CODEC:
            reg_addr = (ch == HAL_CLOCKS_CRG_CH0) ? CMU_CLOCKS_CODEC_CR0_REG : CMU_CLOCKS_CODEC_CR1_REG;
            break;
        default:
            break;
    }

    return reg_addr;
}

hal_clocks_crg_ch_t hal_clocks_ccrg_current_ch_get(hal_clocks_ccrg_module_t module)
{
    if (module != HAL_CLOCKS_MODULE_MCU_CORE) {
        return HAL_CLOCKS_CRG_CH_INVALID;
    }
    hal_clocks_crg_ch_t ch;
    uint32_t irq_sts = osal_irq_lock();
    ch = reg16_getbits(g_ccrg_modules_reg_map[module], CMU_CLOCKS_COM_CR_CH_SEL_STS_BIT, \
                       CMU_CLOCKS_COM_CR_CH_SEL_STS_LEN);
    osal_irq_restore(irq_sts);
    return ch;
}

void hal_clocks_app_perips_config(hal_clocks_app_perips_clk_type_t app_clk, switch_type_t on)
{
    if (app_clk < HAL_CLOCKS_APP_MCLK_EN1_BASE) {
        app_clk -= HAL_CLOCKS_APP_MCLK_EN0_BASE;
        hal_clocks_config_bit(CMU_CLOCKS_M_CLKEN0_REG, on, (REG16_POS)app_clk);
    } else if (app_clk < HAL_CLOCKS_APP_MCLK_EN2_BASE) {
        app_clk -= HAL_CLOCKS_APP_MCLK_EN1_BASE;
        hal_clocks_config_bit(CMU_CLOCKS_M_CLKEN1_REG, on, (REG16_POS)app_clk);
    } else {
        hal_clocks_config_bit(CMU_CLOCKS_M_CLKEN2_REG, on, (REG16_POS)app_clk);
    }
}

bool hal_clocks_app_perips_get_config(hal_clocks_app_perips_clk_type_t app_clk)
{
    if (app_clk < HAL_CLOCKS_APP_MCLK_EN1_BASE) {
        app_clk -= HAL_CLOCKS_APP_MCLK_EN0_BASE;
        return reg16_getbit(CMU_CLOCKS_M_CLKEN0_REG, (REG16_POS)app_clk);
    } else {
        app_clk -= HAL_CLOCKS_APP_MCLK_EN1_BASE;
        return reg16_getbit(CMU_CLOCKS_M_CLKEN1_REG, (REG16_POS)app_clk);
    }
}

uint8_t hal_clocks_ccrg_module_get_src(hal_clocks_ccrg_module_t module)
{
    uint32_t reg_addr;
    hal_clocks_crg_ch_t ch;

    switch (module) {
        case HAL_CLOCKS_MODULE_MCU_CORE:
        case HAL_CLOCKS_MODULE_COM_BUS:
        case HAL_CLOCKS_MODULE_HIFI:
        case HAL_CLOCKS_MODULE_CODEC:
            ch = hal_clocks_ccrg_current_ch_get(module);
            reg_addr = hal_clocks_ccrg_reg_get(module, ch);
            break;
        default:
            reg_addr = g_ccrg_modules_reg_map[module];
            break;
    }
    return reg16_getbits(reg_addr, COMM_CLK_SRC_POS, COMM_CLK_SRC_LEN);
}

uint8_t hal_clocks_ccrg_module_get_div(hal_clocks_ccrg_module_t module)
{
    uint8_t div_len = COMM_CLK_NUM_LEN;
    uint32_t reg_addr;
    hal_clocks_crg_ch_t ch;

    switch (module) {
        case HAL_CLOCKS_MODULE_MCU_CORE:
        case HAL_CLOCKS_MODULE_COM_BUS:
        case HAL_CLOCKS_MODULE_HIFI:
        case HAL_CLOCKS_MODULE_CODEC:
            ch = hal_clocks_ccrg_current_ch_get(module);
            reg_addr = hal_clocks_ccrg_reg_get(module, ch);
            break;
        default:
            reg_addr = g_ccrg_modules_reg_map[module];
            break;
    }

    if (module == HAL_CLOCKS_MODULE_CODEC) {
        div_len = CODEC_DIV_NUM_LEN;
    }

    uint8_t div_en = reg16_getbit(reg_addr, COMM_CLK_DIV_EN_POS);
    if (div_en == 0) {
        return CLK_DIV_1;
    } else {
        uint8_t div = reg16_getbits(reg_addr, COMM_CLK_DIV_NUM_POS, COMM_CLK_SRC_LEN);
        if (div == 0) {
            div = 1;
        }
        return div;
    }
}

bool hal_clocks_dsp_div_set(hal_clocks_dsp_index_t dsp, uint8_t div)
{
    if (dsp == HAL_CLOCKS_DSP_MAX) {
        return false;
    }

    uint32_t irq_sts = osal_irq_lock();
    if (dsp == HAL_CLOCKS_DSP0) {
        reg32_setbits(DSP_DIV_NUM_REG, DSP0_DIV_NUM_POS, DSP_DIV_NUM_LEN, div - 1);
    } else {
        reg32_setbits(DSP_DIV_NUM_REG, DSP1_DIV_NUM_POS, DSP_DIV_NUM_LEN, div - 1);
    }
    osal_irq_restore(irq_sts);
    return true;
}

bool hal_clocks_dsp_div_get(hal_clocks_dsp_index_t dsp, uint8_t *div)
{
    if (dsp == HAL_CLOCKS_DSP_MAX) {
        return false;
    }

    uint32_t irq_sts = osal_irq_lock();
    if (dsp == HAL_CLOCKS_DSP0) {
        *div = (uint8_t)reg32_getbits(DSP_DIV_NUM_REG, DSP0_DIV_NUM_POS, DSP_DIV_NUM_LEN) + 1;
    } else {
        *div = (uint8_t)reg32_getbits(DSP_DIV_NUM_REG, DSP1_DIV_NUM_POS, DSP_DIV_NUM_LEN) + 1;
    }
    osal_irq_restore(irq_sts);
    return true;
}

void hal_clocks_glb_clken_config(hal_clocks_ccrg_module_t module, switch_type_t on)
{
    uint8_t pos;
    switch (module) {
        case HAL_CLOCKS_MODULE_MCU_PERP_LS:
            pos = HAL_GLB_CLKEN_MCU_PERP_LS;
            break;
        case HAL_CLOCKS_MODULE_MCU_PERP_UART:
            pos = HAL_GLB_CLKEN_MCU_PERP_UART;
            break;
        case HAL_CLOCKS_MODULE_MCU_PERP_SPI:
            pos = HAL_GLB_CLKEN_MCU_PERP_SPI;
            break;
        case HAL_CLOCKS_MODULE_SDIOM:
            pos = HAL_GLB_CLKEN_SDIOM;
            break;
        case HAL_CLOCKS_MODULE_MEM_BUS:
            pos = HAL_GLB_CLKEN_MEM_BUS;
            break;
        case HAL_CLOCKS_MODULE_XIP_OPI:
            pos = HAL_GLB_CLKEN_XIP_OPI;
            break;
        case HAL_CLOCKS_MODULE_XIP_QSPI:
            pos = HAL_GLB_CLKEN_XIP_QSPI;
            break;
        case HAL_CLOCKS_MODULE_GPU:
            pos = HAL_GLB_CLKEN_GPU;
            break;
        case HAL_CLOCKS_MODULE_HIFI:
            pos = HAL_GLB_CLKEN_HIFI;
            break;
        case HAL_CLOCKS_MODULE_CODEC:
            pos = HAL_GLB_CLKEN_CODEC;
            break;
        default:
            return;
    }
    hal_clocks_config_bit(CMU_CLOCKS_GLB_CLKEN_REG, on, (REG16_POS)pos);
}

bool hal_clocks_is_dual_channel_module(hal_clocks_ccrg_module_t module)
{
    if (module == HAL_CLOCKS_MODULE_MCU_CORE || module == HAL_CLOCKS_MODULE_COM_BUS ||
        module == HAL_CLOCKS_MODULE_HIFI || module == HAL_CLOCKS_MODULE_CODEC) {
        return true;
    }
    return false;
}

void hal_clocks_sub_normal_sel_cfg(hal_clocks_ccrg_module_t module, hal_clocks_crg_ch_t ch)
{
    if (ch == HAL_CLOCKS_CRG_CH_INVALID) {
        return;
    }
    uint8_t pos;
    uint32_t ch_switch_reg;
    bool refresh_flag;
    switch (module) {
        case HAL_CLOCKS_MODULE_MCU_CORE:
            pos = HAL_CLOCKS_MCU_CORE_CH_SEL_NOR;
            ch_switch_reg = CMU_CLOCKS_MSUB_NORMAL_REG;
            refresh_flag = true;
            break;
        case HAL_CLOCKS_MODULE_COM_BUS:
            pos = HAL_CLOCKS_MCPU_COM_BUS_CH_SEL;
            ch_switch_reg = CMU_CLOCKS_COMCTRL_CH_SEL_REG;
            refresh_flag = false;
            break;
        case HAL_CLOCKS_MODULE_HIFI:
            pos = HAL_CLOCKS_MCPU_HIFI_CH_SEL;
            ch_switch_reg = CMU_CLOCKS_COMCTRL_CH_SEL_REG;
            refresh_flag = false;
            break;
        case HAL_CLOCKS_MODULE_CODEC:
            pos = HAL_CLOCKS_MCPU_CODEC_CH_SEL;
            ch_switch_reg = CMU_CLOCKS_COMCTRL_CH_SEL_REG;
            refresh_flag = false;
            break;
        default:
            return;
    }

    if (ch == HAL_CLOCKS_CRG_CH1) {
        hal_clocks_config_bit(ch_switch_reg, true, (REG16_POS)pos);
    } else {
        hal_clocks_config_bit(ch_switch_reg, false, (REG16_POS)pos);
    }
    if (refresh_flag) {
        reg16_setbit(MSUB_CFG_REFRESH_REG, MSUB_CFG_REFRESH_BIT);
        reg16_clrbit(MSUB_CFG_REFRESH_REG, MSUB_CFG_REFRESH_BIT);
    }
}

void hal_clocks_set_hifi_freq(hal_clocks_ccrg_mcu_group_clk_src_t clk_source, uint8_t div)
{
    uint32_t reg_addr = hal_clocks_ccrg_reg_get(HAL_CLOCKS_MODULE_HIFI, HAL_CLOCKS_CRG_CH_INVALID);
    if (reg_addr == 0) {
        return;
    }

    uint16_t reg_val = (uint16_t)((1u << CMU_CLOCKS_COM_CR_CH_EN_BIT) | \
                    ((uint16_t)clk_source << CMU_CLOCKS_COM_CR_CLK_SEL_BIT));
    if (div > 0x1) {
        reg_val |= (((uint16_t)1 << CMU_CLOCKS_COM_CR_CLK_DIV_EN_BIT) | \
                   ((uint16_t)div << CMU_CLOCKS_COM_CR_CLK_DIV_NUM_BIT));
    }

    uint32_t irq_sts = osal_irq_lock();
    reg16_clrbit(CMU_CLOCKS_GLB_CLKEN_REG, (REG16_POS)HAL_GLB_CLKEN_HIFI);
    writew(reg_addr, reg_val);
    reg16_setbit(CMU_CLOCKS_GLB_CLKEN_REG, (REG16_POS)HAL_GLB_CLKEN_HIFI);
    osal_irq_restore(irq_sts);
}

void hal_clocks_module_auto_cg_control_private(hal_clocks_module_auto_cg_t auto_cg_module, switch_type_t auto_en)
{
    switch (auto_cg_module) {
        case HAL_CLOCKS_MODULE_AUTO_CG_MCU_APB:
            hal_reg_config_bit(MCU_AUTO_CG_CFG, auto_en, (REG16_POS)MCU_APB_AUTO_CG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_AH2H_MCU_SUB_TAON:
            hal_reg_config_bit(MCU_AUTO_CG_CFG, auto_en, (REG16_POS)AH2H_MCU_SUB_TAON_AUTOCG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_AH2H_MCU_SUB_SEC:
            hal_reg_config_bit(MCU_AUTO_CG_CFG, auto_en, (REG16_POS)AH2H_MCU_SUB_SEC_AUTOCG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_AH2H_SH2H_MCU_SUB_MFSH2H:
            hal_reg_config_bit(MCU_AUTO_CG_CFG, auto_en, (REG16_POS)SH2H_MCU_SUB_MFSH2H_AUTOCG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_SH2H_MCU_SUB_SFH2H:
            hal_reg_config_bit(MCU_AUTO_CG_CFG, auto_en, (REG16_POS)SH2H_MCU_SUB_SFH2H_AUTOCG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_MCU_PERP_AXI_GLB:
            hal_reg_config_bit(MCU_PERP_AXI_NEW_AUTO_CG, auto_en, (REG16_POS)MCU_PERP_AXI_GLB_CG_BYPASS_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_CACHE_H2H:
            hal_reg_config_bit(CACHE_BUS_LP_CFG, auto_en, (REG16_POS)CACHE_AUTO_CG_BYPASS_EN_H2H_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_CACHE:
            hal_reg_config_bit(CACHE_BUS_LP_CFG, auto_en, (REG16_POS)CACHE_AUTO_CG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_MAIN_AXI_GLB:
            hal_reg_config_bit(MAIN_AXI_NEW_AUTO_CG, auto_en, (REG16_POS)MAIN_AXI_GLB_CG_BYPASS_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_DSP_SHARE:
            hal_reg_config_bit(MEM_SUB_AUTO_CG_CFG, auto_en, (REG16_POS)DSP_SHARE_AUTO_CG_BY_PASS_EN);
            return;
        default:
            break;
    }
}

void hal_clocks_module_dma_spi_cg_control(hal_clocks_module_auto_cg_t auto_cg_module, switch_type_t auto_en)
{
    switch (auto_cg_module) {
        case HAL_CLOCKS_MODULE_AUTO_CG_SDMA_MTOP_SDMA:
            hal_reg_config_bit(DMA_AUTO_CG_CFG, auto_en, (REG16_POS)SDMA_MTOP_SDMA_AUTO_CG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_DMA_MTOP_SDMA:
            hal_reg_config_bit(DMA_AUTO_CG_CFG, auto_en, (REG16_POS)DMA_MTOP_SDMA_AUTO_CG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_BTOP_DMA_SDMA:
            hal_reg_config_bit(DMA_AUTO_CG_CFG, auto_en, (REG16_POS)BTOP_DMA_SDMA_AUTO_CG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_OPI:
            hal_reg_config_bit(SPI_AUTO_CG_CFG, auto_en, (REG16_POS)OPI_AUTO_CG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_QSPI2:
            hal_reg_config_bit(SPI_AUTO_CG_CFG, auto_en, (REG16_POS)QSPI2_AUTO_CG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_QSPI1:
            hal_reg_config_bit(SPI_AUTO_CG_CFG, auto_en, (REG16_POS)QSPI1_AUTO_CG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_QSPI0:
            hal_reg_config_bit(SPI_AUTO_CG_CFG, auto_en, (REG16_POS)QSPI0_AUTO_CG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_SPI2_2CS:
            hal_reg_config_bit(SPI_AUTO_CG_CFG, auto_en, (REG16_POS)SPI2_2CS_AUTO_CG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_SPI1_3CS:
            hal_reg_config_bit(SPI_AUTO_CG_CFG, auto_en, (REG16_POS)SPI1_3CS_AUTO_CG_BYPASS_EN_BIT);
            return;
        case HAL_CLOCKS_MODULE_AUTO_CG_SPI0_3CS:
            hal_reg_config_bit(SPI_AUTO_CG_CFG, auto_en, (REG16_POS)SPI0_3CS_AUTO_CG_BYPASS_EN_BIT);
            return;
        default:
            break;
    }
}