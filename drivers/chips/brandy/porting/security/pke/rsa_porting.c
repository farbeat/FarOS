/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides rsa port template\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "securec.h"
#include "osal_interrupt.h"
#include "chip_core_irq.h"
#include "common_def.h"
#include "hal_rsa_v2_regs_op.h"
#include "hal_rsa_v2.h"
#include "rsa_porting.h"

#define RSA_BASE_ADDR                     0x52030000
#define RSA_SOFT_CTRL_BASE_ADDR           0x52009020

uint8_t g_rsa_key_mem[RSA_MEM_SIZE] __attribute__ ((aligned (RSA_ADDR_ALIGN))) = {0};
uint8_t g_rsa_input_mem[RSA_MEM_SIZE] __attribute__ ((aligned (RSA_ADDR_ALIGN))) = {0};
uint8_t g_rsa_output_mem[RSA_MEM_SIZE] __attribute__ ((aligned (RSA_ADDR_ALIGN))) = {0};

typedef union rst_soft_n {
    struct {
        uint32_t    reserved_0            : 4;
        uint32_t    soft_rst_rsa_n        : 1;
        uint32_t    reserved_1            : 27;
    } b;
    uint32_t    d32;
} rst_soft_n_t;

typedef union soft_clken {
    struct {
        uint32_t    reserved_0            : 4;
        uint32_t    rsa_clken             : 1;
        uint32_t    reserved_1            : 27;
    } b;
    uint32_t    d32;
} soft_clken_t;

typedef union cfg_endian {
    struct {
        uint32_t    cfg_endian            : 2;
        uint32_t    reserved_0            : 30;
    } b;
    uint32_t    d32;
} cfg_endian_t;

typedef struct rsa_soft_ctrl_regs {
    volatile rst_soft_n_t rst_soft_n;
    volatile soft_clken_t soft_clken;
    volatile cfg_endian_t cfg_endian;
} rsa_soft_ctrl_regs_t;

uintptr_t g_rsa_v2_reg = (uintptr_t)RSA_BASE_ADDR;

static rsa_soft_ctrl_regs_t *g_rsa_soft_ctrl_regs = (rsa_soft_ctrl_regs_t*)(uintptr_t)RSA_SOFT_CTRL_BASE_ADDR;

static void rsa_set_clk_en(uint32_t clken)
{
    soft_clken_t soft_clken;
    soft_clken.d32 = g_rsa_soft_ctrl_regs->soft_clken.d32;
    soft_clken.b.rsa_clken = clken;
    g_rsa_soft_ctrl_regs->soft_clken.d32 = soft_clken.d32;
}

static void rsa_set_soft_reset(uint32_t soft_reset)
{
    rst_soft_n_t rst_soft_n;
    rst_soft_n.d32 = g_rsa_soft_ctrl_regs->rst_soft_n.d32;
    rst_soft_n.b.soft_rst_rsa_n = soft_reset;
    g_rsa_soft_ctrl_regs->rst_soft_n.d32 = rst_soft_n.d32;
}

void rsa_set_endian(uint32_t cfg_endian)
{
    cfg_endian_t cfg_endian_tmp;
    cfg_endian_tmp.d32 = g_rsa_soft_ctrl_regs->cfg_endian.d32;
    cfg_endian_tmp.b.cfg_endian = cfg_endian;
    g_rsa_soft_ctrl_regs->cfg_endian.d32 = cfg_endian_tmp.d32;
}

rsa_endian_mode_t rsa_get_endian(void)
{
    return g_rsa_soft_ctrl_regs->cfg_endian.b.cfg_endian;
}

errcode_t rsa_set_attr(rsa_attr_t *rsa_attr, const uint8_t *n, const uint8_t *key, const uint32_t key_len,
    const uint8_t *data, const uint32_t data_len)
{
#if SEC_NEW_RSA == YES
    // 待适配
#endif
    rsa_attr->key_len = key_len;
    rsa_attr->key_addr = (uint32_t)RSA_KEY_MEM;
    rsa_attr->ptext_addr = (uint32_t)RSA_INPUT_MEM;
    rsa_attr->output_addr = (uint32_t)RSA_OUTPUT_MEM;

    if (memcpy_s((uint8_t*)(uintptr_t)rsa_attr->ptext_addr, data_len, data, data_len) != EOK) {
        return ERRCODE_MEMCPY;
    }

    // 整体的KEY包括{N, E}或者{N, D},先拷贝N到KEY的内存区域
    if (memcpy_s((uint8_t*)(uintptr_t)rsa_attr->key_addr, key_len, n, key_len) != EOK) {
        return ERRCODE_MEMCPY;
    }

    // E或D拷贝到N的后面
    if (memcpy_s((uint8_t*)(uintptr_t)(rsa_attr->key_addr + key_len), key_len, key, key_len) != EOK) {
        return ERRCODE_MEMCPY;
    }

    return ERRCODE_SUCC;
}

void rsa_port_register_hal_funcs(void)
{
    hal_rsa_register_funcs(hal_rsa_v2_get_funcs());
}

void rsa_port_unregister_hal_funcs(void)
{
    hal_rsa_unregister_funcs();
}

hal_rsa_funcs_t *rsa_port_get_funcs(void)
{
    return hal_rsa_get_funcs();
}

uintptr_t rsa_get_base_addr(void)
{
    return g_rsa_v2_reg;
}

void rsa_port_set_clk_en(uint32_t clken)
{
    rsa_set_clk_en(clken);
}

void rsa_port_set_soft_reset(uint32_t soft_reset)
{
    rsa_set_soft_reset(soft_reset);
}

void rsa_irq_handler(void)
{
    hal_rsa_irq_handler();
}

void rsa_port_register_irq(void)
{
    osal_irq_request(SEC_INT_IRQN, (osal_irq_handler)rsa_irq_handler, NULL, NULL, NULL);
    hal_rsa_int_enable();
}

void rsa_port_unregister_irq(void)
{
    hal_rsa_int_disable();
    hal_rsa_int_clear();
    osal_irq_free(SEC_INT_IRQN, NULL);
}