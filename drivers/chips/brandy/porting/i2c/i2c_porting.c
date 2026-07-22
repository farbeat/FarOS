/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Provides i2c port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-15， Create file. \n
 */

#include "i2c_porting.h"
#include "hal_i2c.h"
#include "hal_i2c_v100.h"
#include "hal_i2c_v100_comm.h"
#include "pinctrl.h"
#include "osal_interrupt.h"
#include "chip_core_irq.h"
#include "platform_core.h"
#if defined(CONFIG_I2C_SUPPORT_DMA) && (CONFIG_I2C_SUPPORT_DMA == 1)
#include "dma_porting.h"
#endif
#include "hal_clocks_app_priv.h"

#define BUS_CLOCK_TIME_32M 32000000UL
#define CON                     0x0
#define TAR                     0x4
#define SAR                     0x8
#define HS_MADDR                0xc
#define SS_SCL_HCNT             0x14
#define SS_SCL_LCNT             0x18
#define FS_SCL_HCNT             0x1C
#define FS_SCL_LCNT             0x20
#define HS_SCL_HCNT             0x24
#define HS_SCL_LCNT             0x28
#define INTR_MASK               0x30
#define RX_TL                   0x38
#define TX_TL                   0x3C
#define ENABLE                  0x6C
#define SDA_HOLD                0x7C
#define SLAVE_DATA_NACK_ONLY    0x84
#define DMA_CR                  0x88
#define DMA_TDLR                0x8C
#define DMA_RDLR                0x90
#define DMA_SETUP               0x94
#define ACK_GENERAL_CALL        0x98

typedef void (*i2c_porting_irq_handler)(void);

typedef struct i2c_irq_handler {
    uint32_t irq_id;
    i2c_porting_irq_handler irq_handler;
} i2c_irq_handler_t;

i2c_save_reg_t g_i2c_save_regs[I2C_BUS_MAX_NUM] = { 0 };

uintptr_t g_i2c_base_addrs[I2C_BUS_MAX_NUM] = {
    (uintptr_t)I2C_BUS_0_BASE_ADDR,
    (uintptr_t)I2C_BUS_1_BASE_ADDR,
    (uintptr_t)I2C_BUS_2_BASE_ADDR,
    (uintptr_t)I2C_BUS_3_BASE_ADDR,
};

uintptr_t i2c_porting_base_addr_get(i2c_bus_t bus)
{
    return g_i2c_base_addrs[bus];
}

static void irq_i2c0_handler(void)
{
    hal_i2c_v100_irq_handler(I2C_BUS_0);
}

static void irq_i2c1_handler(void)
{
    hal_i2c_v100_irq_handler(I2C_BUS_1);
}

static void irq_i2c2_handler(void)
{
    hal_i2c_v100_irq_handler(I2C_BUS_2);
}

static void irq_i2c3_handler(void)
{
    hal_i2c_v100_irq_handler(I2C_BUS_3);
}

static i2c_irq_handler_t g_i2c_irq_id[I2C_BUS_MAX_NUM] = {
    {
        I2C_0_IRQN,
        irq_i2c0_handler,
    },
    {
        I2C_1_IRQN,
        irq_i2c1_handler,
    },
    {
        I2C_2_IRQN,
        irq_i2c2_handler,
    },
    {
        I2C_3_IRQN,
        irq_i2c3_handler,
    },
};

uint32_t i2c_port_get_clock_value(i2c_bus_t bus)
{
    if (bus >= I2C_BUS_MAX_NUM) {
        return 0;
    }
    return BUS_CLOCK_TIME_32M;
}


void i2c_port_register_hal_funcs(i2c_bus_t bus)
{
    hal_i2c_register_funcs(bus, hal_i2c_v100_funcs_get());
}

void i2c_port_unregister_hal_funcs(i2c_bus_t bus)
{
    hal_i2c_unregister_funcs(bus);
}

void i2c_port_register_irq(i2c_bus_t bus)
{
    unused(bus);
#if defined(CONFIG_I2C_SUPPORT_INT) && (CONFIG_I2C_SUPPORT_INT == 1)
    i2c_irq_handler_t irq = g_i2c_irq_id[bus];
    osal_irq_request(irq.irq_id, (osal_irq_handler)irq.irq_handler, NULL, NULL, NULL);
    osal_irq_enable(irq.irq_id);
#endif
}

void i2c_port_unregister_irq(i2c_bus_t bus)
{
    i2c_irq_handler_t irq = g_i2c_irq_id[bus];
    osal_irq_free(irq.irq_id, NULL);
}

uint32_t i2c_porting_lock(i2c_bus_t bus)
{
    unused(bus);
    return osal_irq_lock();
}

void i2c_porting_unlock(i2c_bus_t bus, uint32_t irq_sts)
{
    unused(bus);
    osal_irq_restore(irq_sts);
}

void i2c_save_reg(i2c_bus_t bus)
{
    uint32_t base = i2c_porting_base_addr_get(bus);
    g_i2c_save_regs[bus].con = readw(base + CON);
    g_i2c_save_regs[bus].tar = readw(base + TAR);
    g_i2c_save_regs[bus].sar = readw(base + SAR);
    g_i2c_save_regs[bus].hs_maddr = readw(base + HS_MADDR);
    g_i2c_save_regs[bus].ss_scl_hcnt = readw(base + SS_SCL_HCNT);
    g_i2c_save_regs[bus].ss_scl_lcnt = readw(base + SS_SCL_LCNT);
    g_i2c_save_regs[bus].fs_scl_hcnt = readw(base + FS_SCL_HCNT);
    g_i2c_save_regs[bus].fs_scl_lcnt = readw(base + FS_SCL_LCNT);
    g_i2c_save_regs[bus].hs_scl_hcnt = readw(base + HS_SCL_HCNT);
    g_i2c_save_regs[bus].hs_scl_lcnt = readw(base + HS_SCL_LCNT);
    g_i2c_save_regs[bus].intr_mask = readw(base + INTR_MASK);
    g_i2c_save_regs[bus].rx_tl = readw(base + RX_TL);
    g_i2c_save_regs[bus].tx_tl = readw(base + TX_TL);
    g_i2c_save_regs[bus].enable = readw(base + ENABLE);
    g_i2c_save_regs[bus].sda_hold = readw(base + SDA_HOLD);
    g_i2c_save_regs[bus].slave_data_nack_only = readw(base + SLAVE_DATA_NACK_ONLY);
    g_i2c_save_regs[bus].dma_cr = readw(base + DMA_CR);
    g_i2c_save_regs[bus].dma_tdlr = readw(base + DMA_TDLR);
    g_i2c_save_regs[bus].dma_rdlr = readw(base + DMA_RDLR);
    g_i2c_save_regs[bus].sda_setup = readw(base + DMA_SETUP);
    g_i2c_save_regs[bus].ack_general_call = readw(base + ACK_GENERAL_CALL);
}

void i2c_recovery_reg(i2c_bus_t bus)
{
    uint32_t base = i2c_porting_base_addr_get(bus);
    reg16_clrbit(base + ENABLE, 0);
    writew(base + CON, g_i2c_save_regs[bus].con);
    writew(base + TAR, g_i2c_save_regs[bus].tar);
    writew(base + SAR, g_i2c_save_regs[bus].sar);
    writew(base + HS_MADDR, g_i2c_save_regs[bus].hs_maddr);
    writew(base + SS_SCL_HCNT, g_i2c_save_regs[bus].ss_scl_hcnt);
    writew(base + SS_SCL_LCNT, g_i2c_save_regs[bus].ss_scl_lcnt);
    writew(base + FS_SCL_HCNT, g_i2c_save_regs[bus].fs_scl_hcnt);
    writew(base + FS_SCL_LCNT, g_i2c_save_regs[bus].fs_scl_lcnt);
    writew(base + HS_SCL_HCNT, g_i2c_save_regs[bus].hs_scl_hcnt);
    writew(base + HS_SCL_LCNT, g_i2c_save_regs[bus].hs_scl_lcnt);
    writew(base + INTR_MASK, g_i2c_save_regs[bus].intr_mask);
    writew(base + RX_TL, g_i2c_save_regs[bus].rx_tl);
    writew(base + TX_TL, g_i2c_save_regs[bus].tx_tl);
    writew(base + SDA_HOLD, g_i2c_save_regs[bus].sda_hold);
    writew(base + SLAVE_DATA_NACK_ONLY, g_i2c_save_regs[bus].slave_data_nack_only);
    writew(base + DMA_CR, g_i2c_save_regs[bus].dma_cr);
    writew(base + DMA_TDLR, g_i2c_save_regs[bus].dma_tdlr);
    writew(base + DMA_RDLR, g_i2c_save_regs[bus].dma_rdlr);
    writew(base + DMA_SETUP, g_i2c_save_regs[bus].sda_setup);
    writew(base + ACK_GENERAL_CALL, g_i2c_save_regs[bus].ack_general_call);
    writew(base + ENABLE, g_i2c_save_regs[bus].enable);
    reg16_setbit(base + ENABLE, 0);
}

#if defined(CONFIG_I2C_SUPPORT_DMA)
uint8_t i2c_port_get_dma_trans_dest_handshaking(i2c_bus_t bus)
{
    switch (bus) {
        case I2C_BUS_0:
            return (uint8_t)HAL_DMA_HANDSHAKING_I2C0_TX;
        case I2C_BUS_1:
            return (uint8_t)HAL_DMA_HANDSHAKING_I2C1_TX;
        case I2C_BUS_2:
            return (uint8_t)HAL_DMA_HANDSHAKING_I2C2_TX;
        case I2C_BUS_3:
            return (uint8_t)HAL_DMA_HANDSHAKING_I2C3_TX;
#if I2C_BUS_MAX_NUMBER > 4
        case I2C_BUS_4:
            return (uint8_t)HAL_DMA_HANDSHAKING_I2C4_TX;
#endif
        default:
            return (uint8_t)HAL_DMA_HANDSHAKING_MAX_NUM;
    }
}

uint8_t i2c_port_get_dma_trans_src_handshaking(i2c_bus_t bus)
{
    switch (bus) {
        case I2C_BUS_0:
            return (uint8_t)HAL_DMA_HANDSHAKING_I2C0_RX;
        case I2C_BUS_1:
            return (uint8_t)HAL_DMA_HANDSHAKING_I2C1_RX;
        case I2C_BUS_2:
            return (uint8_t)HAL_DMA_HANDSHAKING_I2C2_RX;
        case I2C_BUS_3:
            return (uint8_t)HAL_DMA_HANDSHAKING_I2C3_RX;
#if I2C_BUS_MAX_NUMBER > 4
        case I2C_BUS_4:
            return (uint8_t)HAL_DMA_HANDSHAKING_I2C4_RX;
#endif
        default:
            return (uint8_t)HAL_DMA_HANDSHAKING_MAX_NUM;
    }
}
#endif  /* CONFIG_I2C_SUPPORT_DMA */

#ifdef TEST_SUITE
void i2c_port_test_i2c_init_pin(void)
{
    uapi_pin_set_mode(I2C_0_SCL_PIN, PIN_MODE_1);
    uapi_pin_set_mode(I2C_0_SDA_PIN, PIN_MODE_1);
    uapi_pin_set_mode(I2C_3_SCL_PIN, PIN_MODE_1);
    uapi_pin_set_mode(I2C_3_SDA_PIN, PIN_MODE_1);
}
#endif

#if defined(CONFIG_I2C_SUPPORT_LPC)
void i2c_port_clock_enable(i2c_bus_t bus, bool on)
{
    switch (bus) {
        case I2C_BUS_0:
            hal_clocks_app_perips_config(HAL_CLOCKS_APP_I2C0_CLK, (switch_type_t)on);
            break;
        case I2C_BUS_1:
            hal_clocks_app_perips_config(HAL_CLOCKS_APP_I2C1_CLK, (switch_type_t)on);
            break;
        case I2C_BUS_2:
            hal_clocks_app_perips_config(HAL_CLOCKS_APP_I2C2_CLK, (switch_type_t)on);
            break;
        case I2C_BUS_3:
            hal_clocks_app_perips_config(HAL_CLOCKS_APP_I2C3_CLK, (switch_type_t)on);
            break;
        default:
            break;
    }
}
#endif