/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides trng port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "hal_trng_v1.h"
#include "trng_porting.h"

#define TRNG_BASE_ADDR               0x52009800
#define TRNG_SOFT_CTRL_BASE_ADDR     0x52009020
#define TRNG_SAMPLE_DIV_ADDR         0x52000FA0
#define TRNG_CLK_SAMPLE_EN_OFFSET    0x5

#define trng_reg16(register)                (*(volatile uint16_t*)(uintptr_t)(register))
#define trng_reg16_setbit(register, pos)    (trng_reg16(register) |= ((uint16_t)((1U) << (uint16_t)(pos))))
#define trng_reg16_clrbit(register, pos)    (trng_reg16(register) &= ~((uint16_t)((1U) << (uint16_t)(pos))))

typedef union rst_soft_n {
    struct {
        uint32_t    reserved_0       : 5;
        uint32_t    soft_rst_trng_n  : 1;
        uint32_t    reserved_1       : 26;
    } b;
    uint32_t    d32;
} rst_soft_n_t;

typedef union soft_clken {
    struct {
        uint32_t    reserved_0  : 5;
        uint32_t    trng_clken  : 1;
        uint32_t    reserved_1  : 26;
    } b;
    uint32_t    d32;
} soft_clken_t;

typedef struct trng_soft_ctrl_regs {
    volatile rst_soft_n_t rst_soft_n;
    volatile soft_clken_t soft_clken;
} trng_soft_ctrl_regs_t;

static trng_soft_ctrl_regs_t *g_trng_soft_ctrl_regs = (trng_soft_ctrl_regs_t*)(uintptr_t)TRNG_SOFT_CTRL_BASE_ADDR;

uintptr_t g_trng_v1_base_addr = (uintptr_t)TRNG_BASE_ADDR;

static void trng_set_clk_en(uint32_t clken)
{
    soft_clken_t soft_clken;
    soft_clken.d32 = g_trng_soft_ctrl_regs->soft_clken.d32;
    soft_clken.b.trng_clken = clken;
    g_trng_soft_ctrl_regs->soft_clken.d32 = soft_clken.d32;
}

static void trng_set_soft_reset(uint32_t soft_reset)
{
    rst_soft_n_t rst_soft_n;
    rst_soft_n.d32 = g_trng_soft_ctrl_regs->rst_soft_n.d32;
    rst_soft_n.b.soft_rst_trng_n = soft_reset;
    g_trng_soft_ctrl_regs->rst_soft_n.d32 = rst_soft_n.d32;
}

void trng_port_register_hal_funcs(void)
{
    hal_trng_register_funcs(hal_trng_v1_get_funcs());
}

void trng_port_unregister_hal_funcs(void)
{
    hal_trng_unregister_funcs();
}

hal_trng_funcs_t *trng_port_get_funcs(void)
{
    return hal_trng_get_funcs();
}

uintptr_t trng_get_base_addr(void)
{
    return g_trng_v1_base_addr;
}

void trng_port_set_clk_en(uint32_t clken)
{
    trng_set_clk_en(clken);
}

void trng_port_set_soft_reset(uint32_t soft_reset)
{
    trng_set_soft_reset(soft_reset);
}

void trng_port_set_clk_sample_en(uint32_t clk_en)
{
    if (clk_en == 0x1) {
        trng_reg16_setbit(TRNG_SAMPLE_DIV_ADDR, 0x5);
    } else {
        trng_reg16_clrbit(TRNG_SAMPLE_DIV_ADDR, 0x5);
    }
}