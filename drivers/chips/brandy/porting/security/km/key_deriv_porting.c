/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides keyslot port template\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "securec.h"
#include "hal_key_deriv_v2_regs_op.h"
#include "hal_key_deriv_v2.h"
#include "hal_key_deriv.h"
#include "key_deriv_porting.h"

#define DRV_KEYSLOT_MCIPHER_MAX 1

typedef union rst_soft_n {
    struct {
        uint32_t    soft_rst_key_deriv_n  : 1;
        uint32_t    reserved              : 31;
    } b;
    uint32_t    d32;
} rst_soft_n_t;

typedef union soft_clken {
    struct {
        uint32_t    key_deriv_clken       : 1;
        uint32_t    reserved_1            : 31;
    } b;
    uint32_t    d32;
} soft_clken_t;

typedef union cfg_endian {
    struct {
        uint32_t    cfg_endian            : 2;
        uint32_t    reserved_0            : 30;
    } b;
    uint32_t    b32;
} cfg_endian_t;

typedef struct key_deriv_soft_ctrl_regs {
    volatile rst_soft_n_t rst_soft_n;
    volatile soft_clken_t soft_clken;
    volatile cfg_endian_t cfg_endian;
} key_deriv_soft_ctrl_regs_t;

#define KEY_DERIV_BASE_ADDR               (key_deriv_regs_t*)(uintptr_t)0x52009030
#define KEY_DERIV_SOFT_CTRL_BASE_ADDR     (key_deriv_soft_ctrl_regs_t*)(uintptr_t)0x52009020

static key_deriv_soft_ctrl_regs_t *g_key_deriv_soft_ctrl_regs = KEY_DERIV_SOFT_CTRL_BASE_ADDR;
static key_deriv_regs_t *g_key_deriv_v2_reg = KEY_DERIV_BASE_ADDR;

static void key_deriv_set_clk_en(uint32_t clken)
{
    soft_clken_t soft_clken;
    soft_clken.d32 = g_key_deriv_soft_ctrl_regs->soft_clken.d32;
    soft_clken.b.key_deriv_clken = clken;
    g_key_deriv_soft_ctrl_regs->soft_clken.d32 = soft_clken.d32;
}

static void key_deriv_set_soft_reset(uint32_t soft_reset)
{
    rst_soft_n_t rst_soft_n;
    rst_soft_n.d32 = g_key_deriv_soft_ctrl_regs->rst_soft_n.d32;
    rst_soft_n.b.soft_rst_key_deriv_n = soft_reset;
    g_key_deriv_soft_ctrl_regs->rst_soft_n.d32 = rst_soft_n.d32;
}

void key_deriv_port_register_hal_funcs(void)
{
    hal_key_deriv_register_funcs(hal_key_deriv_v2_get_funcs());
}

void key_deriv_port_unregister_hal_funcs(void)
{
    hal_key_deriv_unregister_funcs();
}

hal_key_deriv_funcs_t *key_deriv_port_get_funcs(void)
{
    return hal_key_deriv_get_funcs();
}

uintptr_t key_deriv_get_base_addr(void)
{
    return (uintptr_t)g_key_deriv_v2_reg;
}

void key_deriv_port_set_clk_en(uint32_t clken)
{
    key_deriv_set_clk_en(clken);
}

void key_deriv_port_set_soft_reset(uint32_t soft_reset)
{
    key_deriv_set_soft_reset(soft_reset);
}

void key_deriv_irq_handler(void)
{
    hal_key_deriv_irq_handler();
}

void key_deriv_port_register_irq(void)
{
    hal_key_deriv_int_clear();
    // 此处省略oal_int_create实现
    hal_key_deriv_int_enable();
}

void key_deriv_port_unregister_irq(void)
{
    hal_key_deriv_int_disable();
    hal_key_deriv_int_clear();
    // 此处省略oal_int_delete实现
}

void key_deriv_set_endian(uint32_t cfg_endian)
{
    cfg_endian_t cfg_endian_tmp;
    cfg_endian_tmp.b32 = g_key_deriv_soft_ctrl_regs->cfg_endian.b32;
    cfg_endian_tmp.b.cfg_endian = cfg_endian;
    g_key_deriv_soft_ctrl_regs->cfg_endian.b32 = cfg_endian_tmp.b32;
}