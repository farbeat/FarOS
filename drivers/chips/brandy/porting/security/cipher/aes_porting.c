/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides aes port template\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "common_def.h"
#include "hal_aes_v2_regs_op.h"
#include "hal_aes_v2.h"
#include "hal_aes.h"
#include "aes_porting.h"

#define AES_BASE_ADDR                     0x52009030
#define AES_SOFT_CTRL_BASE_ADDR           0x52009020

uint8_t g_drv_aes_iv[AES_IV_SIZE] __attribute__ ((aligned (AES_PARAM_ALIGN))) = {0};
uint8_t g_drv_aes_aad[AES_AAD_SIZE] __attribute__ ((aligned (AES_PARAM_ALIGN))) = {0};
uint8_t g_drv_aes_tag[AES_TAG_SIZE] __attribute__ ((aligned (AES_PARAM_ALIGN))) = {0};
uint8_t g_drv_aes_input_addr[AES_INPUT_BUF_SIZE] __attribute__ ((aligned (AES_ADDR_ALIGN))) = {0};
uint8_t g_drv_aes_output_addr[AES_OUTPUT_BUF_SIZE] __attribute__ ((aligned (AES_ADDR_ALIGN))) = {0};

typedef union rst_soft_n {
    struct {
        uint32_t    reserved_0            : 3;
        uint32_t    soft_rst_aes_n        : 1;
        uint32_t    reserved_1            : 28;
    } b;
    uint32_t    b32;
} rst_soft_n_t;

typedef union soft_clken {
    struct {
        uint32_t    reserved_0            : 3;
        uint32_t    aes_clken             : 1;
        uint32_t    reserved_1            : 28;
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

typedef struct aes_soft_ctrl_regs {
    volatile rst_soft_n_t rst_soft_n;
    volatile soft_clken_t soft_clken;
    volatile cfg_endian_t cfg_endian;
} aes_soft_ctrl_regs_t;

uintptr_t g_aes_v2_reg = (uintptr_t)AES_BASE_ADDR;
static aes_soft_ctrl_regs_t *g_aes_soft_ctrl_regs = (aes_soft_ctrl_regs_t*)(uintptr_t)AES_SOFT_CTRL_BASE_ADDR;

static void aes_set_clk_en(uint32_t clken)
{
    soft_clken_t soft_clken;
    soft_clken.b32 = g_aes_soft_ctrl_regs->soft_clken.b32;
    soft_clken.b.aes_clken = clken;
    g_aes_soft_ctrl_regs->soft_clken.b32 = soft_clken.b32;
}

static void aes_set_soft_reset(uint32_t soft_reset)
{
    rst_soft_n_t rst_soft_n;
    rst_soft_n.b32 = g_aes_soft_ctrl_regs->rst_soft_n.b32;
    rst_soft_n.b.soft_rst_aes_n = soft_reset;
    g_aes_soft_ctrl_regs->rst_soft_n.b32 = rst_soft_n.b32;
}

void aes_port_register_hal_funcs(void)
{
    hal_aes_register_funcs(hal_aes_v2_get_funcs());
}

void aes_port_unregister_hal_funcs(void)
{
    hal_aes_unregister_funcs();
}

hal_aes_funcs_t *aes_port_get_funcs(void)
{
    return hal_aes_get_funcs();
}

uintptr_t aes_get_base_addr(void)
{
    return g_aes_v2_reg;
}

void aes_port_set_clk_en(uint32_t clken)
{
    aes_set_clk_en(clken);
}

void aes_port_set_soft_reset(uint32_t soft_reset)
{
    aes_set_soft_reset(soft_reset);
}

void aes_set_endian(uint32_t cfg_endian)
{
    cfg_endian_t cfg_endian_tmp;
    cfg_endian_tmp.b32 = g_aes_soft_ctrl_regs->cfg_endian.b32;
    cfg_endian_tmp.b.cfg_endian = cfg_endian;
    g_aes_soft_ctrl_regs->cfg_endian.b32 = cfg_endian_tmp.b32;
}

aes_endian_mode_t aes_get_endian(void)
{
    return g_aes_soft_ctrl_regs->cfg_endian.b.cfg_endian;
}

void aes_irq_handler(void)
{
    hal_aes_irq_handler();
}

void aes_port_register_irq(void)
{
    hal_aes_int_clear();
    // 此处省略oal_int_create实现
    hal_aes_int_enable();
}

void aes_port_unregister_irq(void)
{
    hal_aes_int_disable();
    hal_aes_int_clear();
    // 此处省略oal_int_delete实现
}