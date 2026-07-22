/**
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides systick register operation api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-28, Create file. \n
 */
#include <stdint.h>
#include "common_def.h"
#include "hal_pmp_riscv70_regs_op.h"

void hal_pmp_riscv70_regs_set_pmpaddr(uint32_t idx, uint32_t pmpaddr)
{
    /* Because like that pmpaddr0 is RISCV CSR, so cannot be performed using table lookup mode. */
    if (idx == PMPADDR0) {
        write_csr(pmpaddr0, pmpaddr);
    } else if (idx == PMPADDR1) {
        write_csr(pmpaddr1, pmpaddr);
    } else if (idx == PMPADDR2) {
        write_csr(pmpaddr2, pmpaddr);
    } else if (idx == PMPADDR3) {
        write_csr(pmpaddr3, pmpaddr);
    } else if (idx == PMPADDR4) {
        write_csr(pmpaddr4, pmpaddr);
    } else if (idx == PMPADDR5) {
        write_csr(pmpaddr5, pmpaddr);
    } else if (idx == PMPADDR6) {
        write_csr(pmpaddr6, pmpaddr);
    } else if (idx == PMPADDR7) {
        write_csr(pmpaddr7, pmpaddr);
    } else if (idx == PMPADDR8) {
        write_csr(pmpaddr8, pmpaddr);
    } else if (idx == PMPADDR9) {
        write_csr(pmpaddr9, pmpaddr);
    } else if (idx == PMPADDR10) {
        write_csr(pmpaddr10, pmpaddr);
    } else if (idx == PMPADDR11) {
        write_csr(pmpaddr11, pmpaddr);
    } else if (idx == PMPADDR12) {
        write_csr(pmpaddr12, pmpaddr);
    } else if (idx == PMPADDR13) {
        write_csr(pmpaddr13, pmpaddr);
    } else if (idx == PMPADDR14) {
        write_csr(pmpaddr14, pmpaddr);
    } else if (idx == PMPADDR15) {
        write_csr(pmpaddr15, pmpaddr);
    }
}

void hal_pmp_riscv70_regs_set_memxattr(uint32_t idx, uint8_t attr)
{
    if (idx == PMPADDR0) {
        write_custom_csr_val(MEMATTR0, attr);
    } else if (idx == PMPADDR1) {
        write_custom_csr_val(MEMATTR1, attr);
    } else if (idx == PMPADDR2) {
        write_custom_csr_val(MEMATTR2, attr);
    } else if (idx == PMPADDR3) {
        write_custom_csr_val(MEMATTR3, attr);
    } else if (idx == PMPADDR4) {
        write_custom_csr_val(MEMATTR4, attr);
    } else if (idx == PMPADDR5) {
        write_custom_csr_val(MEMATTR5, attr);
    } else if (idx == PMPADDR6) {
        write_custom_csr_val(MEMATTR6, attr);
    } else if (idx == PMPADDR7) {
        write_custom_csr_val(MEMATTR7, attr);
    } else if (idx == PMPADDR8) {
        write_custom_csr_val(MEMATTR8, attr);
    } else if (idx == PMPADDR9) {
        write_custom_csr_val(MEMATTR9, attr);
    } else if (idx == PMPADDR10) {
        write_custom_csr_val(MEMATTR10, attr);
    } else if (idx == PMPADDR11) {
        write_custom_csr_val(MEMATTR11, attr);
    } else if (idx == PMPADDR12) {
        write_custom_csr_val(MEMATTR12, attr);
    } else if (idx == PMPADDR13) {
        write_custom_csr_val(MEMATTR13, attr);
    } else if (idx == PMPADDR14) {
        write_custom_csr_val(MEMATTR14, attr);
    } else if (idx == PMPADDR15) {
        write_custom_csr_val(MEMATTR15, attr);
    }
}

void hal_pmp_riscv70_regs_set_pmpxcfg(uint32_t idx, hal_pmpx_config_t cfg)
{
    hal_pmp_riscv70_cfg_t pmp_cfg;

    /* Because like that pmpaddr0 is RISCV CSR, so cannot be performed using table lookup mode. */
    switch (idx / PMPXCFG_NUM_PER_REG) {
        case PMPCFG0:
            pmp_cfg.d32 = read_csr(pmpcfg0);
            pmp_cfg.b[idx % PMPXCFG_NUM_PER_REG] = cfg;
            write_csr(pmpcfg0, pmp_cfg.d32);
            break;
        case PMPCFG1:
            pmp_cfg.d32 = read_csr(pmpcfg1);
            pmp_cfg.b[idx % PMPXCFG_NUM_PER_REG] = cfg;
            write_csr(pmpcfg1, pmp_cfg.d32);
            break;
        case PMPCFG2:
            pmp_cfg.d32 = read_csr(pmpcfg2);
            pmp_cfg.b[idx % PMPXCFG_NUM_PER_REG] = cfg;
            write_csr(pmpcfg2, pmp_cfg.d32);
            break;
        case PMPCFG3:
            pmp_cfg.d32 = read_csr(pmpcfg3);
            pmp_cfg.b[idx % PMPXCFG_NUM_PER_REG] = cfg;
            write_csr(pmpcfg3, pmp_cfg.d32);
            break;
        default:
            break;
    }
}