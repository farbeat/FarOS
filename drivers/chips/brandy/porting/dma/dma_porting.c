/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides dma port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-16， Create file. \n
 */

#include "dma_porting.h"
#include "common_def.h"
#include "soc_osal.h"
#include "hal_dmac_v100.h"
#include "hal_dmac_v100_regs_def.h"
#include "hal_dmac_v100_regs_op.h"
#include "platform_core.h"
#include "chip_core_irq.h"
#include "platform_core.h"
#include "pm_definition.h"
#include "pm_veto.h"

#define DMA_SEL_CFG               0x52000960
#define DMA_CHANNEL_HS_SELECT     0x52000920

/* MDMA(m0) address judge */
#define DMA_MUX_I2C2_AND_I2C6     3
#define DMA_MUX_IR_AND_I2C5       2
#define DMA_MUX_UARTL0_AND_I2C4   1
#define DMA_MUX_QSPI0_AND_QSPI3   0

#define HAL_DMA_CHANNEL_HANDSHAKING_CHS  2
#define HAL_DMA_CHANNEL_HANDSHAKING_BITS 8

#define hal_dma_channel_handshaking_select_offset(ch)  (((uint32_t)(ch)) / HAL_DMA_CHANNEL_HANDSHAKING_CHS)
#define hal_dma_channel_handshaking_select_pos(ch)     ((((uint32_t)(ch)) % HAL_DMA_CHANNEL_HANDSHAKING_CHS) * \
                                                        HAL_DMA_CHANNEL_HANDSHAKING_BITS)
#define DMA_HARDSHAKING_OFFSET_NUM 4

#define HAL_DMA_PORT_CH_EN_WE_OFFSET 8

uintptr_t g_dma_base_addr =  (uintptr_t)DMA_BASE_ADDR;
uintptr_t g_sdma_base_addr =  (uintptr_t)SDMA_BASE_ADDR;

typedef enum dma_mux_channel {
    DMA_MUX_CHANNEL_QSPI0_QSPI3_TX,
    DMA_MUX_CHANNEL_QSPI0_QSPI3_RX,
    DMA_MUX_CHANNEL_I2C4_UARTL0_TX,
    DMA_MUX_CHANNEL_I2C4_UARTL0_RX,
    DMA_MUX_CHANNEL_I2C5_IR_TX,
    DMA_MUX_CHANNEL_I2C5_IR_RX,
    DMA_MUX_CHANNEL_I2C6_I2C2_TX,
    DMA_MUX_CHANNEL_I2C6_I2C2_RX,
    DMA_MUX_CHANNEL_INVALID
} dma_mux_channel_t;

typedef struct dma_hs_regs {
    volatile uint32_t dma_hs_ch_config[DMA_HARDSHAKING_OFFSET_NUM];    /* DMA handshaking configuration. */
} dma_hs_regs_t;

static uint32_t g_dma_handshaking_channel_status = 0;

void dma_port_register_hal_funcs(void)
{
    hal_dma_register_funcs(hal_dmac_v100_funcs_get());
}

void dma_port_unregister_hal_funcs(void)
{
    hal_dma_unregister_funcs();
}

static void irq_dma_handler(void)
{
    hal_dma_irq_handler();
}

#if defined(CONFIG_DMA_SUPPORT_SMDMA)
static void irq_sdma_handler(void)
{
    hal_sdma_irq_handler();
}
#endif /* CONFIG_DMA_SUPPORT_SMDMA */

void dma_port_register_irq(void)
{
    osal_irq_request(M_DMA_IRQN, (osal_irq_handler)irq_dma_handler, NULL, NULL, NULL);
    osal_irq_enable(M_DMA_IRQN);
#if defined(CONFIG_DMA_SUPPORT_SMDMA)
    osal_irq_request(M_SDMA_IRQN, (osal_irq_handler)irq_sdma_handler, NULL, NULL, NULL);
    osal_irq_enable(M_SDMA_IRQN);
#endif /* CONFIG_DMA_SUPPORT_SMDMA */
}

void dma_port_unregister_irq(void)
{
    osal_irq_disable(M_DMA_IRQN);
    osal_irq_free(M_DMA_IRQN, NULL);
#if defined(CONFIG_DMA_SUPPORT_SMDMA)
    osal_irq_disable(M_SDMA_IRQN);
    osal_irq_free(M_SDMA_IRQN, NULL);
#endif /* CONFIG_DMA_SUPPORT_SMDMA */
}

static hal_dma_mux_handshaking_status_t dma_port_get_handshaking_channel_status(hal_dma_handshaking_source_t channel)
{
    uint32_t status = 0;
    switch (channel) {
        case HAL_DMA_HANDSHAKING_UART_L_TX:
        case HAL_DMA_HANDSHAKING_I2C4_TX:
            status = g_dma_handshaking_channel_status & (1UL << DMA_MUX_CHANNEL_I2C4_UARTL0_TX);
            break;
        case HAL_DMA_HANDSHAKING_I2C4_RX:
        case HAL_DMA_HANDSHAKING_UART_L_RX:
            status = (g_dma_handshaking_channel_status & (1UL << DMA_MUX_CHANNEL_I2C4_UARTL0_RX));
            break;
        case HAL_DMA_HANDSHAKING_IR_TX:
        case HAL_DMA_HANDSHAKING_I2C5_TX:
            status = g_dma_handshaking_channel_status & (1UL << DMA_MUX_CHANNEL_I2C5_IR_TX);
            break;
        case HAL_DMA_HANDSHAKING_I2C5_RX:
        case HAL_DMA_HANDSHAKING_IR_RX:
            status = (g_dma_handshaking_channel_status & (1UL << DMA_MUX_CHANNEL_I2C5_IR_RX));
            break;
        case HAL_DMA_HANDSHAKING_I2C2_TX:
        case HAL_DMA_HANDSHAKING_I2C6_TX:
            status = g_dma_handshaking_channel_status & (1UL << DMA_MUX_CHANNEL_I2C6_I2C2_TX);
            break;
        case HAL_DMA_HANDSHAKING_I2C6_RX:
        case HAL_DMA_HANDSHAKING_I2C2_RX:
            status = (g_dma_handshaking_channel_status & (1UL << DMA_MUX_CHANNEL_I2C6_I2C2_RX));
            break;
        case HAL_DMA_HANDSHAKING_QSPI0_2CS_TX:
        case HAL_DMA_HANDSHAKING_QSPI3_1CS_TX:
            status = g_dma_handshaking_channel_status & (1UL << DMA_MUX_CHANNEL_QSPI0_QSPI3_TX);
            break;
        case HAL_DMA_HANDSHAKING_QSPI0_2CS_RX:
        case HAL_DMA_HANDSHAKING_QSPI3_1CS_RX:
            status = (g_dma_handshaking_channel_status & (1UL << DMA_MUX_CHANNEL_QSPI0_QSPI3_RX));
            break;
        default:
            break;
    }
    if (status != 0) {
        return HAL_DMA_MUX_HANDSHAKING_USING;
    } else {
        return HAL_DMA_MUX_HANDSHAKING_IDLE;
    }
}

void dma_port_set_handshaking_channel_status(hal_dma_handshaking_source_t channel, bool on)
{
    dma_mux_channel_t mux_channel = DMA_MUX_CHANNEL_INVALID;
    switch (channel) {
        case HAL_DMA_HANDSHAKING_UART_L_TX :
            mux_channel = DMA_MUX_CHANNEL_I2C4_UARTL0_TX;
            break;
        case HAL_DMA_HANDSHAKING_UART_L_RX :
            mux_channel = DMA_MUX_CHANNEL_I2C4_UARTL0_RX;
            break;
        case HAL_DMA_HANDSHAKING_IR_TX :
            mux_channel = DMA_MUX_CHANNEL_I2C5_IR_TX;
            break;
        case HAL_DMA_HANDSHAKING_IR_RX :
            mux_channel = DMA_MUX_CHANNEL_I2C5_IR_RX;
            break;
        case HAL_DMA_HANDSHAKING_I2C2_TX :
            mux_channel = DMA_MUX_CHANNEL_I2C6_I2C2_TX;
            break;
        case HAL_DMA_HANDSHAKING_I2C2_RX :
            mux_channel = DMA_MUX_CHANNEL_I2C6_I2C2_RX;
            break;
        case HAL_DMA_HANDSHAKING_QSPI0_2CS_TX :
            mux_channel = DMA_MUX_CHANNEL_QSPI0_QSPI3_TX;
            break;
        case HAL_DMA_HANDSHAKING_QSPI0_2CS_RX :
            mux_channel = DMA_MUX_CHANNEL_QSPI0_QSPI3_RX;
            break;
        default:
            break;
    }
    if (mux_channel == DMA_MUX_CHANNEL_INVALID) { return; }
    if (on) {
        g_dma_handshaking_channel_status |= (1UL << mux_channel);
    } else {
        g_dma_handshaking_channel_status &= (uint32_t)(~(1UL << mux_channel));
    }
}

static void dma_port_switch_hardware_channel(hal_dma_handshaking_source_t channel)
{
    uint16_t *g_dma_hw_regs = (uint16_t *)(uintptr_t)DMA_SEL_CFG;
    switch (channel) {
        case HAL_DMA_HANDSHAKING_I2C6_TX:
        case HAL_DMA_HANDSHAKING_I2C6_RX:
            *g_dma_hw_regs |= ((uint16_t)(1) << (DMA_MUX_I2C2_AND_I2C6));
            break;
        case HAL_DMA_HANDSHAKING_I2C2_TX:
        case HAL_DMA_HANDSHAKING_I2C2_RX:
            *g_dma_hw_regs &= ~((uint16_t)(1) << (DMA_MUX_I2C2_AND_I2C6));
            break;
        case HAL_DMA_HANDSHAKING_I2C5_TX:
        case HAL_DMA_HANDSHAKING_I2C5_RX:
            *g_dma_hw_regs |= ((uint16_t)(1) << (DMA_MUX_IR_AND_I2C5));
            break;
        case HAL_DMA_HANDSHAKING_IR_TX:
        case HAL_DMA_HANDSHAKING_IR_RX:
            *g_dma_hw_regs &= ~((uint16_t)(1) << (DMA_MUX_IR_AND_I2C5));
            break;
        case HAL_DMA_HANDSHAKING_I2C4_TX:
        case HAL_DMA_HANDSHAKING_I2C4_RX:
            *g_dma_hw_regs |= ((uint16_t)(1) << (DMA_MUX_UARTL0_AND_I2C4));
            break;
        case HAL_DMA_HANDSHAKING_UART_L_TX:
        case HAL_DMA_HANDSHAKING_UART_L_RX:
            *g_dma_hw_regs &= ~((uint16_t)(1) << (DMA_MUX_UARTL0_AND_I2C4));
            break;
        case HAL_DMA_HANDSHAKING_QSPI3_1CS_TX:
        case HAL_DMA_HANDSHAKING_QSPI3_1CS_RX:
            *g_dma_hw_regs |= ((uint16_t)(1) << (DMA_MUX_QSPI0_AND_QSPI3));
            break;
        case HAL_DMA_HANDSHAKING_QSPI0_2CS_TX:
        case HAL_DMA_HANDSHAKING_QSPI0_2CS_RX:
            *g_dma_hw_regs &= ~((uint16_t)(1) << (DMA_MUX_QSPI0_AND_QSPI3));
            break;
        default:
            break;
    }
}

static hal_dma_handshaking_source_t dma_port_set_mux_get_real_ch(hal_dma_handshaking_source_t ch)
{
    switch (ch) {
        case HAL_DMA_HANDSHAKING_I2C4_TX:
            return HAL_DMA_HANDSHAKING_UART_L_TX;
        case HAL_DMA_HANDSHAKING_I2C4_RX:
            return HAL_DMA_HANDSHAKING_UART_L_RX;
        case HAL_DMA_HANDSHAKING_I2C5_TX:
            return HAL_DMA_HANDSHAKING_IR_TX;
        case HAL_DMA_HANDSHAKING_I2C5_RX:
            return HAL_DMA_HANDSHAKING_IR_RX;
        case HAL_DMA_HANDSHAKING_I2C6_TX:
            return HAL_DMA_HANDSHAKING_I2C2_TX;
        case HAL_DMA_HANDSHAKING_I2C6_RX:
            return HAL_DMA_HANDSHAKING_I2C2_RX;
        case HAL_DMA_HANDSHAKING_QSPI3_1CS_TX:
            return HAL_DMA_HANDSHAKING_QSPI0_2CS_TX;
        case HAL_DMA_HANDSHAKING_QSPI3_1CS_RX:
            return HAL_DMA_HANDSHAKING_QSPI0_2CS_RX;
        default:
            return ch;
    }
}

errcode_t dma_port_set_mux_channel(dma_channel_t ch, hal_dma_transfer_peri_config_t *per_cfg)
{
    unused(ch);
    if ((dma_port_get_handshaking_channel_status(per_cfg->hs_source) == HAL_DMA_MUX_HANDSHAKING_USING) &&
        (dma_port_get_handshaking_channel_status(per_cfg->hs_dest) == HAL_DMA_MUX_HANDSHAKING_USING)) {
        return ERRCODE_DMA_RET_HANDSHAKING_USING;
    }
    dma_port_switch_hardware_channel(per_cfg->hs_source);
    dma_port_switch_hardware_channel(per_cfg->hs_dest);

    per_cfg->hs_source = dma_port_set_mux_get_real_ch(per_cfg->hs_source);
    per_cfg->hs_dest = dma_port_set_mux_get_real_ch(per_cfg->hs_dest);

    dma_port_set_handshaking_channel_status(per_cfg->hs_source, true);
    dma_port_set_handshaking_channel_status(per_cfg->hs_dest, true);

#if defined(CONFIG_DMA_SUPPORT_SMDMA)
    if (per_cfg->hs_source >= HAL_DMA_HANDSHAKING_SDMA) {
        per_cfg->hs_source = (hal_dma_handshaking_source_t)(per_cfg->hs_source - HAL_DMA_HANDSHAKING_SDMA);
    }
    if (per_cfg->hs_dest >= HAL_DMA_HANDSHAKING_SDMA) {
        per_cfg->hs_dest = (hal_dma_handshaking_source_t)(per_cfg->hs_dest - HAL_DMA_HANDSHAKING_SDMA);
    }
#endif /* CONFIG_DMA_SUPPORT_SMDMA */
    return ERRCODE_SUCC;
}

void dma_port_set_channel_handshaking_source(hal_dma_hardshaking_channel_t ch, hal_dma_handshaking_source_t source)
{
    dma_hs_regs_t *dma_hs_regs = (dma_hs_regs_t *)(uintptr_t)DMA_CHANNEL_HS_SELECT;
    uint32_t offset = hal_dma_channel_handshaking_select_offset(ch);
    uint32_t pos = hal_dma_channel_handshaking_select_pos(ch);
    dma_hs_regs->dma_hs_ch_config[offset] |= source << pos;
}

hal_dma_master_select_t dma_port_get_master_select(dma_channel_t ch, uint32_t addr)
{
    /* APP core mdma/smdma has ms0 and ms1. */
    if (((addr >= MEM_X2P_MEMORY_START) && (addr <= MEM_X2P_MEMORY_END)) ||
        ((addr >= L2RAM_MEMORY_START) && (addr <= L2RAM_MEMORY_END)) ||
        ((addr >= QSPI_XIP_MEMORY_START) && (addr <= QSPI_XIP_MEMORY_END))) {
        if (ch >= DMA_CHANNEL_8) {
            return HAL_DMA_MASTER_SELECT_ERROR;
        } else {
            return HAL_DMA_MASTER_SELECT_0;
        }
    } else {
        if (ch >= DMA_CHANNEL_8) {
            return HAL_DMA_MASTER_SELECT_0;
        } else {
            return HAL_DMA_MASTER_SELECT_1;
        }
    }
}

errcode_t dmac_port_config_periph(dma_channel_t ch,
                                  const hal_dma_transfer_base_config_t *base_cfg,
                                  hal_dma_transfer_peri_config_t *periph_cfg)
{
    dma_regs_t *tmp_dma_regs =  (dma_regs_t *)g_dma_base_addr; /* M_DMA */
    dma_channel_t tmp_dma_ch = (dma_channel_t)((uint32_t)ch % B_DMA_CHANNEL_MAX_NUM);

    if (periph_cfg != NULL) {
        if (periph_cfg->trans_dir == HAL_DMA_TRANSFER_DIR_PERIPHERAL_TO_MEM) {
            hal_dma_cfg_l_set_hs_sel_src(tmp_dma_ch, (uint32_t)HAL_DMA_HARDWARE_HANDSHAKING, tmp_dma_regs);
            hal_dma_cfg_l_set_hs_sel_dst(tmp_dma_ch, (uint32_t)HAL_DMA_SOFTWARE_HANDSHAKING, tmp_dma_regs);
            dma_port_set_channel_handshaking_source((hal_dma_hardshaking_channel_t)tmp_dma_ch, periph_cfg->hs_source);
        } else {
            hal_dma_cfg_l_set_hs_sel_dst(tmp_dma_ch, (uint32_t)HAL_DMA_HARDWARE_HANDSHAKING, tmp_dma_regs);
            hal_dma_cfg_l_set_hs_sel_src(tmp_dma_ch, (uint32_t)HAL_DMA_HARDWARE_HANDSHAKING, tmp_dma_regs);
            dma_port_set_channel_handshaking_source((hal_dma_hardshaking_channel_t)tmp_dma_ch, periph_cfg->hs_dest);
        }
        hal_dma_cfg_h_set_src_per(tmp_dma_ch, (uint32_t)periph_cfg->hs_source, tmp_dma_regs);
        hal_dma_cfg_h_set_dest_per(tmp_dma_ch, (uint32_t)periph_cfg->hs_dest, tmp_dma_regs);
        hal_dma_cfg_h_set_protctl(tmp_dma_ch, (uint32_t)periph_cfg->protection, tmp_dma_regs);
    }
    hal_dma_cfg_l_set_ch_prior(tmp_dma_ch, (uint32_t)base_cfg->priority, tmp_dma_regs);
    uint32_t ch_data = bit(tmp_dma_ch) | bit(tmp_dma_ch + HAL_DMA_PORT_CH_EN_WE_OFFSET);
    uapi_reg_write32(&(tmp_dma_regs->dma_interrupt_type[DMA_INT_REG_MASK].tfr), ch_data);
    uapi_reg_write32(&(tmp_dma_regs->dma_interrupt_type[DMA_INT_REG_MASK].err), ch_data);

    return ERRCODE_SUCC;
}

void dma_port_add_sleep_veto(void)
{
    return;
}

void dma_port_remove_sleep_veto(void)
{
    return;
}

void dma_suspend(void)
{
    uapi_dma_close();
    uapi_dma_deinit();
}

void dma_resume(void)
{
    uapi_dma_init();
    uapi_dma_open();
}