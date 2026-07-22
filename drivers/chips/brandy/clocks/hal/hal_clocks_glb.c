/*
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved.
 * Description:  hal clock glb DRIVER.
 */

#include "soc_osal.h"
#include "hal_clocks_glb.h"

#define CMU_CLOCK_1M_DIV_REG                        (GLB_CTL_M_RB_BASE + 0x60)
#define CMU_CLOCK_A32M_COM_DIV_NUM                  0
#define CMU_CLOCK_A32M_COM_DIV_LEN                  7
#define CMU_CLOCK_A32M_COM_CLK_EN                   11
#define CMU_CLOCK_A32M_COM_CLKDIV_EN                12

#define CMU_CLOCK_32K_DIV_REG                       (GLB_CTL_M_RB_BASE + 0x64)
#define CMU_CLOCK_HF_32K_DIV_NUM                    0
#define CMU_CLOCK_HF_32K_DIV_LEN                    7
#define CMU_CLOCK_HF_32K_CLK_EN                     9
#define CMU_CLOCK_HF_32K_CLKDIV_EN                  10

#define CMU_CLOCK_32K_32M_EN_SEL                    (GLB_CTL_M_RB_BASE + 0x68)
#define CMU_CLOCK_A32M_COM_CLKEN                    0
#define CMU_CLOCK_A32M_COM_SEL                      1
#define CMU_CLOCK_XO_RC_SEL                         8
#define CMU_CLOCK_XO_32K_SEL                        9
#define CMU_CLOCK_A32M_MX_CLKA_STS                  10
#define CMU_CLOCK_A32M_MX_CLKB_STS                  11
#define CMU_CLOCK_RCXO_32K_SEL                      12

#define CMU_CLOCK_CLB_CLKSEL0_REG                   (GLB_CTL_M_RB_BASE + 0x88)
#define CMU_CLOCK_SSI_XO_32K_MX_CLKA_STS            10
#define CMU_CLOCK_SSI_XO_32K_MX_CLKB_STS            11

#define CMU_CLOCK_C_CRG_GLB_BUS_CLKEN               (GLB_CTL_M_RB_BASE + 0x9C)

#define CMU_CLOCK_AON_BUS_DIV_REG                   (GLB_CTL_M_RB_BASE + 0xA8)
#define CMU_CLOCK_AON_BUS_DIV_NUM                   0
#define CMU_CLOCK_AON_BUS_DIV_NUM_LEN               4
#define CMU_CLOCK_AON_BUS_CLK_EN                    7
#define CMU_CLOCK_AON_BUS_CLKDIV_EN                 8

#define CMU_CLOCK_MBUS_CLKEN_NOR_REG                (GLB_CTL_M_RB_BASE + 0xf0)
#define CMU_CLOCK_MBUS_CLKEN_NOR_BIT                0

#define CMU_CLOCK_GATING_GLB_CLKEN_1                (GLB_CTL_M_RB_BASE + 0x84)
#define CMU_CLOCK_COM_CRG_CLKEN_1                   (GLB_CTL_M_RB_BASE + 0xA0)

#define CMU_CLOCK_SOFT_RST0_REG                     (GLB_CTL_M_RB_BASE + 0xB0)

#define CMU_CLOCK_SOFT_RST1_REG                     (GLB_CTL_M_RB_BASE + 0xB4)
#define CMU_CLOCK_SOFT_RST_GLB_COM_CRG              0
#define CMU_CLOCK_SOFT_RST_GLB_COM_LGC              1

#define CMU_CLOCK_SOFT_RST2_REG                     (GLB_CTL_M_RB_BASE + 0xB8)
#define CMU_CLOCK_SOFT_RST_GLB_B_CRG_BIT            0
#define CMU_CLOCK_SOFT_RST_GLB_B_LGC_BIT            1
#define CMU_CLOCK_SOFT_RST_GLB_B_CPU_BIT            2

#define CMU_CLOCK_SOFT_RST3_REG                     (GLB_CTL_M_RB_BASE + 0xBC)
#define CMU_CLOCK_SOFT_RST_GLB_DSP_LGC_BIT          0

#define CMU_CLOCK_SOFT_RST4_REG                     (GLB_CTL_M_RB_BASE + 0xC0)
#define CMU_CLOCK_SOFT_RST_GLB_DISPLAY_CRG_BIT      0
#define CMU_CLOCK_SOFT_RST_GLB_DISPLAY_LGC_BIT      1

#define CMU_CLOCK_SOFT_RST5_REG                     (GLB_CTL_M_RB_BASE + 0xC4)
#define CMU_CLOCK_SOFT_RST_GLB_MCU_CRG_BIT          0
#define CMU_CLOCK_SOFT_RST_GLB_MCU_LGC_BIT          1
#define CMU_CLOCK_SOFT_RST_GLB_MCU_CPU_BIT          2

#define CMU_CLOCK_SOFT_RST6_REG                     (GLB_CTL_M_RB_BASE + 0xC8)
#define CMU_CLOCK_SOFT_RST_GLB_SDIO_CRG_BIT         0
#define CMU_CLOCK_SOFT_RST_GLB_SDIO_LGC_BIT         1

#define CMU_CLOCK_SOFT_RST7_REG                     (GLB_CTL_M_RB_BASE + 0xCC)
#define CMU_CLOCK_SOFT_RST_GLB_XIP_CRG_BIT          0
#define CMU_CLOCK_SOFT_RST_GLB_XIP_LGC_BIT          1

// codec diag regs
#define CODEC_DIG_RW_REG_L                          (COM_CTL_RB_BASE + 0x150)
#define CODEC_DIG_RW_REG_H                          (COM_CTL_RB_BASE + 0x154)

#define CMU_CLOCK_BT_CORE_WAIT_REG                  (GLB_CTL_B_RB_BASE + 0x3B8)

// Cpu need 5 instruction to handle 1 number,
// so there need 1000 * 5 = 5000,1ms cpu can run 200-300instruction,so here that is 2.5ms.
#define AON_TCXO_DELAY              500UL
#define HAL_CLOCKS_GLB_TIMEOUT      1000

uint32_t g_aon_tcxo_delay = 0;

void hal_clocks_a32m_common_clk_en(switch_type_t clken)
{
    hal_clocks_config_bit(CMU_CLOCK_1M_DIV_REG, clken, (REG16_POS)CMU_CLOCK_A32M_COM_CLK_EN);
}

void hal_clocks_set_a32m_common_clk_div(uint8_t div_num)
{
    uint32_t irq_sts = osal_irq_lock();
    reg16_clrbit(CMU_CLOCK_1M_DIV_REG, CMU_CLOCK_A32M_COM_CLKDIV_EN);
    reg16_setbits(CMU_CLOCK_1M_DIV_REG, CMU_CLOCK_A32M_COM_DIV_NUM, CMU_CLOCK_A32M_COM_DIV_LEN, div_num);
    reg16_setbit(CMU_CLOCK_1M_DIV_REG, CMU_CLOCK_A32M_COM_CLKDIV_EN);
    reg16_clrbit(CMU_CLOCK_1M_DIV_REG, CMU_CLOCK_A32M_COM_CLKDIV_EN);
    osal_irq_restore(irq_sts);
}

void hal_clocks_hf_32k_clk_en(switch_type_t clken)
{
    hal_clocks_config_bit(CMU_CLOCK_32K_DIV_REG, clken, (REG16_POS)CMU_CLOCK_HF_32K_CLK_EN);
}

void hal_clocks_set_hf_32k_div(uint8_t div_num)
{
    uint32_t irq_sts = osal_irq_lock();
    reg16_clrbit(CMU_CLOCK_32K_DIV_REG, CMU_CLOCK_HF_32K_CLKDIV_EN);
    reg16_setbits(CMU_CLOCK_32K_DIV_REG, CMU_CLOCK_HF_32K_DIV_NUM, CMU_CLOCK_HF_32K_DIV_LEN, div_num);
    reg16_setbit(CMU_CLOCK_32K_DIV_REG, CMU_CLOCK_HF_32K_CLKDIV_EN);
    reg16_clrbit(CMU_CLOCK_32K_DIV_REG, CMU_CLOCK_HF_32K_CLKDIV_EN);
    osal_irq_restore(irq_sts);
}

void hal_clocks_a32m_common_clken(switch_type_t clken)
{
    hal_clocks_config_bit(CMU_CLOCK_32K_32M_EN_SEL, clken, (REG16_POS)CMU_CLOCK_A32M_COM_CLKEN);
}

void hal_clocks_a32m_common_sel(hal_clocks_aon_clk_t clksel)
{
    hal_clocks_config_bit(CMU_CLOCK_32K_32M_EN_SEL, (switch_type_t)clksel, (REG16_POS)CMU_CLOCK_A32M_COM_SEL);
    hal_clocks_status_check_timeout(CMU_CLOCK_32K_32M_EN_SEL,
        (uint16_t)(CMU_CLOCK_A32M_MX_CLKA_STS + clksel), TURN_ON, HAL_CLOCKS_GLB_TIMEOUT);
}

void hal_clocks_xo_rc_clk_sel(hal_clocks_aon_clk_t clksel)
{
    hal_clocks_config_bit(CMU_CLOCK_32K_32M_EN_SEL, (switch_type_t)clksel, (REG16_POS)CMU_CLOCK_XO_RC_SEL);
}

void hal_clocks_xo_32k_clk_sel(switch_type_t clksel)
{
    hal_clocks_config_bit(CMU_CLOCK_32K_32M_EN_SEL, clksel, (REG16_POS)CMU_CLOCK_XO_32K_SEL);
}

void hal_clocks_rcxo_32k_clk_sel(switch_type_t clksel)
{
    hal_clocks_config_bit(CMU_CLOCK_32K_32M_EN_SEL, clksel, (REG16_POS)CMU_CLOCK_RCXO_32K_SEL);
}

void hal_clocks_glb_fon_bus_control(hal_clocks_glb_fon_bus_t fon_bus, switch_type_t clken)
{
    hal_clocks_config_bit(CMU_CLOCK_C_CRG_GLB_BUS_CLKEN, clken, (REG16_POS)fon_bus);
}

void hal_clocks_set_aon_bus_div(uint8_t div_num)
{
    uint32_t irq_sts = osal_irq_lock();
    reg16_clrbit(CMU_CLOCK_AON_BUS_DIV_REG, CMU_CLOCK_AON_BUS_CLKDIV_EN);
    reg16_setbits(CMU_CLOCK_AON_BUS_DIV_REG, CMU_CLOCK_AON_BUS_DIV_NUM, CMU_CLOCK_AON_BUS_DIV_NUM_LEN, div_num);
    reg16_setbit(CMU_CLOCK_AON_BUS_DIV_REG, CMU_CLOCK_AON_BUS_CLKDIV_EN);
    reg16_clrbit(CMU_CLOCK_AON_BUS_DIV_REG, CMU_CLOCK_AON_BUS_CLKDIV_EN);
    osal_irq_restore(irq_sts);
}

void hal_clocks_aon_bus_clken(switch_type_t clken)
{
    hal_clocks_config_bit(CMU_CLOCK_AON_BUS_DIV_REG, clken, (REG16_POS)CMU_CLOCK_AON_BUS_CLK_EN);
}

void hal_clocks_glb_xip_soft_reset_control(hal_glb_xip_soft_rst_type_t hal_glb_xip_soft_rst,
                                           hal_soft_rst_control_type_t rst_control)
{
    hal_clocks_config_bit(CMU_CLOCK_SOFT_RST7_REG, (switch_type_t)rst_control, (REG16_POS)hal_glb_xip_soft_rst);
}

void hal_clocks_mbus_clken_nor_control(switch_type_t clken)
{
    hal_clocks_config_bit(CMU_CLOCK_MBUS_CLKEN_NOR_REG, clken, (REG16_POS)CMU_CLOCK_MBUS_CLKEN_NOR_BIT);
}

static void hal_clocks_gating_glb_clken_1_enable(switch_type_t on, REG16_POS position)
{
    hal_clocks_config_bit(CMU_CLOCK_GATING_GLB_CLKEN_1, on, position);
}

void hal_clocks_aon_config(hal_clock_aon_perips_type_t perip, switch_type_t clken)
{
    // Multiple peripheral may call the funciton, need protect the register.
    uint32_t irq_sts = osal_irq_lock();
    hal_clocks_gating_glb_clken_1_enable(clken, (REG16_POS)perip);
    osal_irq_restore(irq_sts);
}

void hal_clocks_aon_soft_reset_control(hal_aon_soft_rst_type_t hal_aon_soft_rst,
                                       hal_soft_rst_control_type_t rst_control)
{
    hal_clocks_config_bit(CMU_CLOCK_SOFT_RST0_REG, (switch_type_t)rst_control, (REG16_POS)hal_aon_soft_rst);
}

void hal_clocks_glb_mcu_soft_reset_control(hal_glb_mcu_soft_rst_type_t hal_glb_mcu_soft_rst,
                                           hal_soft_rst_control_type_t rst_control)
{
    uint32_t reg_addr;
    uint8_t offset;
    switch (hal_glb_mcu_soft_rst) {
        case HAL_SOFT_RST5_MCU_CRG:
            reg_addr = CMU_CLOCK_SOFT_RST5_REG;
            offset = CMU_CLOCK_SOFT_RST_GLB_MCU_CRG_BIT;
            break;
        case HAL_SOFT_RST5_MCU_LGC:
            reg_addr = CMU_CLOCK_SOFT_RST5_REG;
            offset = CMU_CLOCK_SOFT_RST_GLB_MCU_LGC_BIT;
            break;
        case HAL_SOFT_RST5_MCU_CPU:
            reg_addr = CMU_CLOCK_SOFT_RST5_REG;
            offset = CMU_CLOCK_SOFT_RST_GLB_MCU_CPU_BIT;
            break;
        default:
            return;
    }
    hal_clocks_config_bit(reg_addr, (switch_type_t)rst_control, (REG16_POS)offset);
}

void hal_clocks_glb_bt_soft_reset_control(hal_glb_bt_soft_rst_type_t hal_glb_bt_soft_rst,
                                          hal_soft_rst_control_type_t rst_control)
{
    uint32_t reg_addr;
    uint8_t offset;
    switch (hal_glb_bt_soft_rst) {
        case HAL_SOFT_RST1_GLB_COM_CRG:
            reg_addr = CMU_CLOCK_SOFT_RST1_REG;
            offset = CMU_CLOCK_SOFT_RST_GLB_COM_CRG;
            break;
        case HAL_SOFT_RST1_GLB_COM_LGC:
            reg_addr = CMU_CLOCK_SOFT_RST1_REG;
            offset = CMU_CLOCK_SOFT_RST_GLB_COM_LGC;
            break;
        case HAL_SOFT_RST2_B_CRG:
            reg_addr = CMU_CLOCK_SOFT_RST2_REG;
            offset = CMU_CLOCK_SOFT_RST_GLB_B_CRG_BIT;
            break;
        case HAL_SOFT_RST2_B_LGC:
            reg_addr = CMU_CLOCK_SOFT_RST2_REG;
            offset = CMU_CLOCK_SOFT_RST_GLB_B_LGC_BIT;
            break;
        case HAL_SOFT_RST2_B_CPU:
            reg_addr = CMU_CLOCK_SOFT_RST2_REG;
            offset = CMU_CLOCK_SOFT_RST_GLB_B_CPU_BIT;
            break;
        default:
            return;
    }
    hal_clocks_config_bit(reg_addr, (switch_type_t)rst_control, (REG16_POS)offset);
}

void hal_clocks_glb_dsp_soft_reset_control(hal_soft_rst_control_type_t rst_control)
{
    hal_clocks_config_bit(CMU_CLOCK_SOFT_RST3_REG, (switch_type_t)rst_control,
                          (REG16_POS)CMU_CLOCK_SOFT_RST_GLB_DSP_LGC_BIT);
}

void hal_clocks_glb_display_soft_reset_control(hal_glb_display_soft_rst_type_t hal_glb_display_soft_rst,
                                               hal_soft_rst_control_type_t rst_control)
{
    uint32_t reg_addr;
    uint8_t offset;
    switch (hal_glb_display_soft_rst) {
        case HAL_SOFT_RST4_DISPLAY_CRG:
            reg_addr = CMU_CLOCK_SOFT_RST4_REG;
            offset = CMU_CLOCK_SOFT_RST_GLB_DISPLAY_CRG_BIT;
            break;
        case HAL_SOFT_RST4_DISPLAY_LGC:
            reg_addr = CMU_CLOCK_SOFT_RST4_REG;
            offset = CMU_CLOCK_SOFT_RST_GLB_DISPLAY_LGC_BIT;
            break;
        default:
            return;
    }
    hal_clocks_config_bit(reg_addr, (switch_type_t)rst_control, (REG16_POS)offset);
}

void hal_clocks_glb_sdio_soft_reset_control(hal_glb_sdio_soft_rst_type_t hal_glb_sdio_soft_rst,
                                            hal_soft_rst_control_type_t rst_control)
{
    uint32_t reg_addr;
    uint8_t offset;

    switch (hal_glb_sdio_soft_rst) {
        case HAL_SOFT_RST6_SDIO_CRG:
            reg_addr = CMU_CLOCK_SOFT_RST6_REG;
            offset = CMU_CLOCK_SOFT_RST_GLB_SDIO_CRG_BIT;
            break;
        case HAL_SOFT_RST6_SDIO_LGC:
            reg_addr = CMU_CLOCK_SOFT_RST6_REG;
            offset = CMU_CLOCK_SOFT_RST_GLB_SDIO_LGC_BIT;
            break;
        default:
            return;
    }
    hal_clocks_config_bit(reg_addr, (switch_type_t)rst_control, (REG16_POS)offset);
}

void hal_clocks_com_bus_enable(hal_clocks_com_clk_type_t com_clk, switch_type_t clken)
{
    hal_clocks_config_bit(CMU_CLOCK_COM_CRG_CLKEN_1, clken, (REG16_POS)com_clk);
}

void hal_clocks_bt_core_wait_release(void)
{
    writew(CMU_CLOCK_BT_CORE_WAIT_REG, 0);
}

void hal_clocks_codec_enable(void)
{
    writew(CODEC_DIG_RW_REG_L, 0xFFFF);
    writew(CODEC_DIG_RW_REG_H, 0xFFFF);
}

void hal_clocks_codec_disable(void)
{
    writew(CODEC_DIG_RW_REG_L, 0);
    writew(CODEC_DIG_RW_REG_H, 0);
}