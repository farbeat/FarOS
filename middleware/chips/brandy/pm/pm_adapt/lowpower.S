/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: cpu register lowpower process file
 */

#include "chip_core_definition.h"

.extern g_core_suspend_regs

.global  lowpower_cpu_suspend
.global  lowpower_cpu_resume

#ifdef __riscv64
#define LREG ld
#define SREG sd
#define REGBYTES 8
#else
#define LREG lw
#define SREG sw
#define REGBYTES 4
#endif

.section .text
.option rvc

#if (ARCH == RISCV70)
lowpower_cpu_suspend:
    csrrw a0 ,mepc,a0
    la a0,  g_core_suspend_regs
    SREG ra, 0 * REGBYTES(a0)
    SREG sp, 1 * REGBYTES(a0)
    SREG gp, 2 * REGBYTES(a0)
    SREG tp, 3 * REGBYTES(a0)
    SREG t0, 4 * REGBYTES(a0)
    SREG t1, 5 * REGBYTES(a0)
    SREG t2, 6 * REGBYTES(a0)
    SREG s0, 7 * REGBYTES(a0)
    SREG s1, 8 * REGBYTES(a0)
    SREG a1, 10 * REGBYTES(a0)
    csrr a1, mepc
    SREG a1, 9 * REGBYTES(a0)
    LREG a1, 10 * REGBYTES(a0)
    SREG a2, 11 * REGBYTES(a0)
    SREG a3, 12 * REGBYTES(a0)
    SREG a4, 13 * REGBYTES(a0)
    SREG a5, 14 * REGBYTES(a0)
    SREG a6, 15 * REGBYTES(a0)
    SREG a7, 16 * REGBYTES(a0)
    SREG s2, 17 * REGBYTES(a0)
    SREG s3, 18 * REGBYTES(a0)
    SREG s4, 19 * REGBYTES(a0)
    SREG s5, 20 * REGBYTES(a0)
    SREG s6, 21 * REGBYTES(a0)
    SREG s7, 22 * REGBYTES(a0)
    SREG s8, 23 * REGBYTES(a0)
    SREG s9, 24 * REGBYTES(a0)
    SREG s10, 25 * REGBYTES(a0)
    SREG s11, 26 * REGBYTES(a0)
    SREG t3, 27 * REGBYTES(a0)
    SREG t4, 28 * REGBYTES(a0)
    SREG t5, 29 * REGBYTES(a0)
    SREG t6, 30 * REGBYTES(a0)

    addi a0, a0, 124

    /* Below save memattr0-63 */
    csrr a7, 0xBC0
    csrr a6, 0xBC1
    csrr a5, 0xBC2
    csrr a4, 0xBC3
    csrr a3, 0xBC4
    csrr a2, 0xBC5
    csrr a1, 0xBC6
    csrr t0, 0xBC7
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    csrr a7, 0xBC8
    csrr a6, 0xBC9
    csrr a5, 0xBCA
    csrr a4, 0xBCB
    csrr a3, 0xBCC
    csrr a2, 0xBCD
    csrr a1, 0xBCE
    csrr t0, 0xBCF
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    /*  Below save Machine Cache Control */
    csrr a7, 0x7C0
    csrr a6, 0x7C1
    csrr a5, 0x7C2
    csrr a4, 0x7C3
    csrr a3, 0x7C4
    csrr a2, 0x7C5
    csrr a1, 0x7C6
    csrr t0, 0x7C7
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    csrr a7, mstatus
    csrr a6, misa
    csrr a5, mie
    csrr a4, mtvec
    csrr a3, mscratch
    csrr a2, mepc
    csrr a1, mcause
    csrr t0, 0x307
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    /* Below save pmpaddr 0 ~ 15 */
    csrr a7, pmpaddr0
    csrr a6, pmpaddr1
    csrr a5, pmpaddr2
    csrr a4, pmpaddr3
    csrr a3, pmpaddr4
    csrr a2, pmpaddr5
    csrr a1, pmpaddr6
    csrr t0, pmpaddr7
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    csrr a7, pmpaddr8
    csrr a6, pmpaddr9
    csrr a5, pmpaddr10
    csrr a4, pmpaddr11
    csrr a3, pmpaddr12
    csrr a2, pmpaddr13
    csrr a1, pmpaddr14
    csrr t0, pmpaddr15
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    /* Below save pmpaddr 16 ~ 31 */
    csrr a7, 0x3C0
    csrr a6, 0x3C1
    csrr a5, 0x3C2
    csrr a4, 0x3C3
    csrr a3, 0x3C4
    csrr a2, 0x3C5
    csrr a1, 0x3C6
    csrr t0, 0x3C7
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    csrr a7, 0x3C8
    csrr a6, 0x3C9
    csrr a5, 0x3CA
    csrr a4, 0x3CB
    csrr a3, 0x3CC
    csrr a2, 0x3CD
    csrr a1, 0x3CE
    csrr t0, 0x3CF
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    /* Below save pmpaddr 32 ~ 47 */
    csrr a7, 0x3D0
    csrr a6, 0x3D1
    csrr a5, 0x3D2
    csrr a4, 0x3D3
    csrr a3, 0x3D4
    csrr a2, 0x3D5
    csrr a1, 0x3D6
    csrr t0, 0x3D7
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    csrr a7, 0x3D8
    csrr a6, 0x3D9
    csrr a5, 0x3DA
    csrr a4, 0x3DB
    csrr a3, 0x3DC
    csrr a2, 0x3DD
    csrr a1, 0x3DE
    csrr t0, 0x3DF
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    /* Below save pmpaddr 48 ~ 63 */
    csrr a7, 0x3E0
    csrr a6, 0x3E1
    csrr a5, 0x3E2
    csrr a4, 0x3E3
    csrr a3, 0x3E4
    csrr a2, 0x3E5
    csrr a1, 0x3E6
    csrr t0, 0x3E7
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    csrr a7, 0x3E8
    csrr a6, 0x3E9
    csrr a5, 0x3EA
    csrr a4, 0x3EB
    csrr a3, 0x3EC
    csrr a2, 0x3ED
    csrr a1, 0x3EE
    csrr t0, 0x3EF
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    /* Below save pmpcfg 0 ~ 15 */
    csrr a7, pmpcfg0
    csrr a6, pmpcfg1
    csrr a5, pmpcfg2
    csrr a4, pmpcfg3
    csrr a3, 0x3A4
    csrr a2, 0x3A5
    csrr a1, 0x3A6
    csrr t0, 0x3A7
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    csrr a7, 0x3A8
    csrr a6, 0x3A9
    csrr a5, 0x3AA
    csrr a4, 0x3AB
    csrr a3, 0x3AC
    csrr a2, 0x3AD
    csrr a1, 0x3AE
    csrr t0, 0x3AF
    SREG a7, 0 * REGBYTES(a0)
    SREG a6, 1 * REGBYTES(a0)
    SREG a5, 2 * REGBYTES(a0)
    SREG a4, 3 * REGBYTES(a0)
    SREG a3, 4 * REGBYTES(a0)
    SREG a2, 5 * REGBYTES(a0)
    SREG a1, 6 * REGBYTES(a0)
    SREG t0, 7 * REGBYTES(a0)
    addi a0, a0, 32

    addi a0, a0, -448 /* back to mepc */
    auipc a1,  0
    addi a1, a1,  16
    SREG a1, 29 * REGBYTES(a0)
    addi a0, a0, -124
    LREG a1, 10 * REGBYTES(a0)
    LREG a0, 9 * REGBYTES(a0)
    csrrw a0, mepc, a0
    ret

lowpower_cpu_resume:
    /* clear reg */
    la      ra, 0
    la      tp, 0
    la      s0, 0
    la      s1, 0
    la      a0, 0
    la      a1, 0
    la      a2, 0
    la      a3, 0
    la      a4, 0
    la      a5, 0
    la      a6, 0
    la      a7, 0
    la      s2, 0
    la      s3, 0
    la      s4, 0
    la      s5, 0
    la      s6, 0
    la      s7, 0
    la      s8, 0
    la      s9, 0
    la      s10, 0
    la      s11, 0
    la      t3, 0
    la      t4, 0
    la      t5, 0
    la      t6, 0
    la a0,  g_core_suspend_regs
    addi a0, a0, 124

    /* Below save memattr */
    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw 0xBC0, a7
    csrw 0xBC1, a6
    csrw 0xBC2, a5
    csrw 0xBC3, a4
    csrw 0xBC4, a3
    csrw 0xBC5, a2
    csrw 0xBC6, a1
    csrw 0xBC7, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw 0xBC8, a7
    csrw 0xBC9, a6
    csrw 0xBCA, a5
    csrw 0xBCB, a4
    csrw 0xBCC, a3
    csrw 0xBCD, a2
    csrw 0xBCE, a1
    csrw 0xBCF, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw 0x7C0, a7
    csrw 0x7C1, a6
    csrw 0x7C2, a5
    csrw 0x7C3, a4
    csrw 0x7C4, a3
    csrw 0x7C5, a2
    csrw 0x7C6, a1
    csrw 0x7C7, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw mstatus, a7
    csrw misa, a6
    csrw mie, a5
    csrw mtvec,a4
    csrw mscratch, a3
    csrw mepc, a2
    csrw mcause, a1
    csrw 0x307, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw pmpaddr0, a7
    csrw pmpaddr1, a6
    csrw pmpaddr2, a5
    csrw pmpaddr3, a4
    csrw pmpaddr4, a3
    csrw pmpaddr5, a2
    csrw pmpaddr6, a1
    csrw pmpaddr7, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw pmpaddr8,  a7
    csrw pmpaddr9,  a6
    csrw pmpaddr10, a5
    csrw pmpaddr11, a4
    csrw pmpaddr12, a3
    csrw pmpaddr13, a2
    csrw pmpaddr14, a1
    csrw pmpaddr15, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw 0x3C0, a7
    csrw 0x3C1, a6
    csrw 0x3C2, a5
    csrw 0x3C3, a4
    csrw 0x3C4, a3
    csrw 0x3C5, a2
    csrw 0x3C6, a1
    csrw 0x3C7, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw 0x3C8, a7
    csrw 0x3C9, a6
    csrw 0x3CA, a5
    csrw 0x3CB, a4
    csrw 0x3CC, a3
    csrw 0x3CD, a2
    csrw 0x3CE, a1
    csrw 0x3CF, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw 0x3D0, a7
    csrw 0x3D1, a6
    csrw 0x3D2, a5
    csrw 0x3D3, a4
    csrw 0x3D4, a3
    csrw 0x3D5, a2
    csrw 0x3D6, a1
    csrw 0x3D7, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw 0x3D8, a7
    csrw 0x3D9, a6
    csrw 0x3DA, a5
    csrw 0x3DB, a4
    csrw 0x3DC, a3
    csrw 0x3DD, a2
    csrw 0x3DE, a1
    csrw 0x3DF, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw 0x3E0, a7
    csrw 0x3E1, a6
    csrw 0x3E2, a5
    csrw 0x3E3, a4
    csrw 0x3E4, a3
    csrw 0x3E5, a2
    csrw 0x3E6, a1
    csrw 0x3E7, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw 0x3E8, a7
    csrw 0x3E9, a6
    csrw 0x3EA, a5
    csrw 0x3EB, a4
    csrw 0x3EC, a3
    csrw 0x3ED, a2
    csrw 0x3EE, a1
    csrw 0x3EF, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw pmpcfg0, a7
    csrw pmpcfg1, a6
    csrw pmpcfg2, a5
    csrw pmpcfg3, a4
    csrw 0x3A4, a3
    csrw 0x3A5, a2
    csrw 0x3A6, a1
    csrw 0x3A7, t0
    addi a0, a0, 32

    LREG a7, 0 * REGBYTES(a0)
    LREG a6, 1 * REGBYTES(a0)
    LREG a5, 2 * REGBYTES(a0)
    LREG a4, 3 * REGBYTES(a0)
    LREG a3, 4 * REGBYTES(a0)
    LREG a2, 5 * REGBYTES(a0)
    LREG a1, 6 * REGBYTES(a0)
    LREG t0, 7 * REGBYTES(a0)
    csrw 0x3A8, a7
    csrw 0x3A9, a6
    csrw 0x3AA, a5
    csrw 0x3AB, a4
    csrw 0x3AC, a3
    csrw 0x3AD, a2
    csrw 0x3AE, a1
    csrw 0x3AF, t0

    la a0,  g_core_suspend_regs
    LREG x1, 0 * REGBYTES(a0)
    LREG x2, 1 * REGBYTES(a0)
    LREG x3, 2 * REGBYTES(a0)
    LREG x4, 3 * REGBYTES(a0)
    LREG x5, 4 * REGBYTES(a0)
    LREG x6, 5 * REGBYTES(a0)
    LREG x7, 6 * REGBYTES(a0)
    LREG x8, 7 * REGBYTES(a0)
    LREG x9, 8 * REGBYTES(a0)
    LREG x11, 10 * REGBYTES(a0)
    LREG x12, 11 * REGBYTES(a0)
    LREG x13, 12 * REGBYTES(a0)
    LREG x14, 13 * REGBYTES(a0)
    LREG x15, 14 * REGBYTES(a0)
    LREG x16, 15 * REGBYTES(a0)
    LREG x17, 16 * REGBYTES(a0)
    LREG x18, 17 * REGBYTES(a0)
    LREG x19, 18 * REGBYTES(a0)
    LREG x20, 19 * REGBYTES(a0)
    LREG x21, 20 * REGBYTES(a0)
    LREG x22, 21 * REGBYTES(a0)
    LREG x23, 22 * REGBYTES(a0)
    LREG x24, 23 * REGBYTES(a0)
    LREG x25, 24 * REGBYTES(a0)
    LREG x26, 25 * REGBYTES(a0)
    LREG x27, 26 * REGBYTES(a0)
    LREG x28, 27 * REGBYTES(a0)
    LREG x29, 28 * REGBYTES(a0)
    LREG x30, 29 * REGBYTES(a0)
    LREG x31, 30 * REGBYTES(a0)
    LREG x10, 9 * REGBYTES(a0)
    csrrw a0, mepc, a0
    jr a0

#else
lowpower_cpu_suspend:
    csrrw a0, mepc, a0
    la a0, g_core_suspend_regs
    SREG ra, 0 * REGBYTES(a0)
    SREG sp, 1 * REGBYTES(a0)
    SREG gp, 2 * REGBYTES(a0)
    SREG tp, 3 * REGBYTES(a0)
    SREG t0, 4 * REGBYTES(a0)
    SREG t1, 5 * REGBYTES(a0)
    SREG t2, 6 * REGBYTES(a0)
    SREG s0, 7 * REGBYTES(a0)
    SREG s1, 8 * REGBYTES(a0)
    SREG a1, 10 * REGBYTES(a0)
    csrr a1, mepc
    SREG a1, 9 * REGBYTES(a0)
    LREG a1, 10 * REGBYTES(a0)
    SREG a2, 11 * REGBYTES(a0)
    SREG a3, 12 * REGBYTES(a0)
    SREG a4, 13 * REGBYTES(a0)
    SREG a5, 14 * REGBYTES(a0)
    SREG a6, 15 * REGBYTES(a0)
    SREG a7, 16 * REGBYTES(a0)
    SREG s2, 17 * REGBYTES(a0)
    SREG s3, 18 * REGBYTES(a0)
    SREG s4, 19 * REGBYTES(a0)
    SREG s5, 20 * REGBYTES(a0)
    SREG s6, 21 * REGBYTES(a0)
    SREG s7, 22 * REGBYTES(a0)
    SREG s8, 23 * REGBYTES(a0)
    SREG s9, 24 * REGBYTES(a0)
    SREG s10, 25 * REGBYTES(a0)
    SREG s11, 26 * REGBYTES(a0)
    SREG t3, 27 * REGBYTES(a0)
    SREG t4, 28 * REGBYTES(a0)
    SREG t5, 29 * REGBYTES(a0)
    SREG t6, 30 * REGBYTES(a0)

    addi a0, a0, 124

    csrr a7, 0xBC0
    csrr a6, 0xBC1
    csrr a5, 0xBC2
    csrr a4, 0xBC3
    csrr a3, 0xBE0
    csrr a2, 0xBE8
    csrr a1, 0xBFE
    csrr t0, 0x7C0
    stmia {a1-a7, t0}, (a0)
    addi a0, a0, 32

    csrr a7, 0x7C1
    csrr a6, 0x7C2
    csrr a5, 0x7C3
    csrr a4, 0x7C4
    csrr a3, 0x7C5
    csrr a2, 0x7D8
    csrr a1, 0x7D9
    csrr t0, mstatus
    stmia {a1-a7, t0}, (a0)
    addi a0, a0, 32

    csrr a7, misa
    csrr a6, mie
    csrr a5, mtvec
    csrr a4, mscratch
    csrr a3, mepc
    csrr a2, mcause
    csrr a1, pmpaddr0
    csrr t0, pmpaddr1
    stmia {a1-a7, t0}, (a0)
    addi a0, a0, 32

    csrr a7, pmpaddr2
    csrr a6, pmpaddr3
    csrr a5, pmpaddr4
    csrr a4, pmpaddr5
    csrr a3, pmpaddr6
    csrr a2, pmpaddr7
    csrr a1, pmpaddr8
    csrr t0, pmpaddr9
    stmia {a1-a7, t0}, (a0)
    addi a0, a0, 32

    csrr a7, pmpaddr10
    csrr a6, pmpaddr11
    csrr a5, pmpaddr12
    csrr a4, pmpaddr13
    csrr a3, pmpaddr14
    csrr a2, pmpaddr15
    csrr a1, pmpcfg0
    csrr t0, pmpcfg1
    stmia {a1-a7, t0}, (a0)
    addi a0, a0, 32

    csrr a2, pmpcfg2
    csrr a1, pmpcfg3
    stmia {a1-a2}, (a0)
    addi a0, a0, 8

    addi a0, a0, -168
    auipc a1,  0
    addi a1, a1,  16
    SREG a1, 20 * REGBYTES(a0)
    addi a0, a0, -124
    LREG a1, 10 * REGBYTES(a0)
    LREG a0, 9 * REGBYTES(a0)
    csrrw a0 ,mepc,a0
    ret

/* Restore CPU-Registers from RAM */
lowpower_cpu_resume:
    /* clear reg */
    la      ra, 0
    la      tp, 0
    la      s0, 0
    la      s1, 0
    la      a0, 0
    la      a1, 0
    la      a2, 0
    la      a3, 0
    la      a4, 0
    la      a5, 0
    la      a6, 0
    la      a7, 0
    la      s2, 0
    la      s3, 0
    la      s4, 0
    la      s5, 0
    la      s6, 0
    la      s7, 0
    la      s8, 0
    la      s9, 0
    la      s10, 0
    la      s11, 0
    la      t3, 0
    la      t4, 0
    la      t5, 0
    la      t6, 0
    la a0, g_core_suspend_regs
    addi a0, a0, 124
    ldmia {a1-a7, t0}, (a0)
    csrw 0xBC0, a7
    csrw 0xBC1, a6
    csrw 0xBC2, a5
    csrw 0xBC3, a4
    csrw 0xBE0, a3
    csrw 0xBE8, a2
    csrw 0xBFE, a1
    csrw 0x7C0, t0
    addi a0, a0, 32

    ldmia {a1-a7, t0}, (a0)
    csrw 0x7C1, a7
    csrw 0x7C2, a6
    csrw 0x7C3, a5
    csrw 0x7C4, a4
    csrw 0x7C5, a3
    csrw 0x7D8, a2
    csrw 0x7D9, a1
    csrw mstatus, t0
    addi a0, a0, 32

    ldmia {a1-a7, t0}, (a0)
    csrw misa, a7
    csrw mie, a6
    csrw mtvec,a5
    csrw mscratch, a4
    csrw mepc, a3
    csrw mcause, a2
    csrw pmpaddr0, a1
    csrw pmpaddr1, t0
    addi a0, a0, 32

    ldmia {a1-a7, t0}, (a0)
    csrw pmpaddr2, a7
    csrw pmpaddr3, a6
    csrw pmpaddr4, a5
    csrw pmpaddr5, a4
    csrw pmpaddr6, a3
    csrw pmpaddr7, a2
    csrw pmpaddr8, a1
    csrw pmpaddr9, t0
    addi a0, a0, 32

    ldmia {a1-a7, t0}, (a0)
    csrw pmpaddr10, a7
    csrw pmpaddr11, a6
    csrw pmpaddr12, a5
    csrw pmpaddr13, a4
    csrw pmpaddr14, a3
    csrw pmpaddr15, a2
    csrw pmpcfg0, a1
    csrw pmpcfg1, t0
    addi a0, a0, 32

    ldmia {a1-a2}, (a0)
    csrw pmpcfg2, a2
    csrw pmpcfg3, a1

    la a0,  g_core_suspend_regs
    LREG x1, 0 * REGBYTES(a0)
    LREG x2, 1 * REGBYTES(a0)
    LREG x3, 2 * REGBYTES(a0)
    LREG x4, 3 * REGBYTES(a0)
    LREG x5, 4 * REGBYTES(a0)
    LREG x6, 5 * REGBYTES(a0)
    LREG x7, 6 * REGBYTES(a0)
    LREG x8, 7 * REGBYTES(a0)
    LREG x9, 8 * REGBYTES(a0)
    LREG x11, 10 * REGBYTES(a0)
    LREG x12, 11 * REGBYTES(a0)
    LREG x13, 12 * REGBYTES(a0)
    LREG x14, 13 * REGBYTES(a0)
    LREG x15, 14 * REGBYTES(a0)
    LREG x16, 15 * REGBYTES(a0)
    LREG x17, 16 * REGBYTES(a0)
    LREG x18, 17 * REGBYTES(a0)
    LREG x19, 18 * REGBYTES(a0)
    LREG x20, 19 * REGBYTES(a0)
    LREG x21, 20 * REGBYTES(a0)
    LREG x22, 21 * REGBYTES(a0)
    LREG x23, 22 * REGBYTES(a0)
    LREG x24, 23 * REGBYTES(a0)
    LREG x25, 24 * REGBYTES(a0)
    LREG x26, 25 * REGBYTES(a0)
    LREG x27, 26 * REGBYTES(a0)
    LREG x28, 27 * REGBYTES(a0)
    LREG x29, 28 * REGBYTES(a0)
    LREG x30, 29 * REGBYTES(a0)
    LREG x31, 30 * REGBYTES(a0)
    LREG x10, 9 * REGBYTES(a0)
    csrrw a0 ,mepc,a0
    jr a0
#endif
