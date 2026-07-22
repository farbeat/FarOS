/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hash port template. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "sha_porting.h"
#include "hal_sha_v2_regs_op.h"
#include "hal_sha_v2.h"
#include "osal_interrupt.h"
#include "chip_core_irq.h"
#include "common_def.h"

#define SHA_BASE_ADDR                     0x52009030
#define SHA_SOFT_CTRL_BASE_ADDR           0x52009020

typedef union rst_soft_n {
    struct {
        uint32_t    reserved_0            : 2;
        uint32_t    soft_rst_sha_n        : 1;
        uint32_t    reserved_1            : 29;
    } b;
    uint32_t    b32;
} rst_soft_n_t;

typedef union soft_clken {
    struct {
        uint32_t    reserved_0            : 2;
        uint32_t    sha_clken             : 1;
        uint32_t    reserved_1            : 29;
    } b;
    uint32_t    b32;
} soft_clken_t;

typedef union cfg_endian {
    struct {
        uint32_t    cfg_endian            : 2;
        uint32_t    reserved_0            : 30;
    } b;
    uint32_t    b32;
} cfg_endian_t;

typedef struct hash_sha_soft_ctrl_regs {
    volatile rst_soft_n_t rst_soft_n;
    volatile soft_clken_t soft_clken;
    volatile cfg_endian_t cfg_endian;
} sha_soft_ctrl_regs_t;
 
static sha_soft_ctrl_regs_t *g_sha_soft_ctrl_regs = (sha_soft_ctrl_regs_t*)(uintptr_t)SHA_SOFT_CTRL_BASE_ADDR;
 
uintptr_t g_sha_base_addr = (uintptr_t)SHA_BASE_ADDR;

static void sha_set_clk_en(uint32_t clken)
{
    soft_clken_t soft_clken;
    soft_clken.b32 = g_sha_soft_ctrl_regs->soft_clken.b32;
    soft_clken.b.sha_clken = clken;
    g_sha_soft_ctrl_regs->soft_clken.b32 = soft_clken.b32;
}

static void sha_set_soft_reset(uint32_t soft_reset)
{
    rst_soft_n_t rst_soft_n;
    rst_soft_n.b32 = g_sha_soft_ctrl_regs->rst_soft_n.b32;
    rst_soft_n.b.soft_rst_sha_n = soft_reset;
    g_sha_soft_ctrl_regs->rst_soft_n.b32 = rst_soft_n.b32;
}
 
void sha_set_endian(uint32_t cfg_endian)
{
    cfg_endian_t cfg_endian_tmp;
    cfg_endian_tmp.b32 = g_sha_soft_ctrl_regs->cfg_endian.b32;
    cfg_endian_tmp.b.cfg_endian = cfg_endian;
    g_sha_soft_ctrl_regs->cfg_endian.b32 = cfg_endian_tmp.b32;
}
 
endian_mode_t sha_get_endian(void)
{
    return g_sha_soft_ctrl_regs->cfg_endian.b.cfg_endian;
}
 
void sha_port_clean_cache(uint32_t *addr, unsigned int size)
{
    unused(addr);
    unused(size);
    // osal_dcache_region_clean(addr, size); // cfbb接口
}

void sha_port_register_hal_funcs(void)
{
    hal_sha_register_funcs(hal_sha_v2_get_funcs());
}

void sha_port_unregister_hal_funcs(void)
{
    hal_sha_unregister_funcs();
}

hal_sha_funcs_t *sha_port_get_funcs(void)
{
    return hal_sha_get_funcs();
}

uintptr_t sha_get_base_addr(void)
{
    return g_sha_base_addr;
}

void sha_port_set_clk_en(uint32_t clken)
{
    sha_set_clk_en(clken);
}

void sha_port_set_soft_reset(uint32_t soft_reset)
{
    sha_set_soft_reset(soft_reset);
}

void sha_irq_handler(void)
{
    hal_sha_irq_handler();
}

void sha_port_register_irq(void)
{
    osal_irq_request(SEC_INT_IRQN, (osal_irq_handler)sha_irq_handler, NULL, NULL, NULL);
    hal_sha_int_enable();
}

void sha_port_unregister_irq(void)
{
    hal_sha_int_disable();
    hal_sha_int_clear();
    osal_irq_free(SEC_INT_IRQN, NULL);
}