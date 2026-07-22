/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: LiteOS trap module implemention.
 * Author: Huawei LiteOS Team
 * Create: 2022-12-20
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#include "arch/stack_ops.h"
#include "asm/interrupt_config.h"

.global  TrapVector
.global  OsPerfSetIrqRegs

.equ        MCAUSE_MASK_INT_NUM,        0x00000FFF /* bit 0-11 represent exception/interrupt code */

/* riscv interrupt & exception entry */
.section .itcm.text
.align 6
TrapVector:
#ifdef LOSCFG_ARCH_FPU_ENABLE
    /* reseverd (4*REGBYTES) for FPU, use 1 REGBYTES but sp must be 16 aligned when compile option -mpush-pop is on */
    addi   sp, sp, -FPU_STATUS_REG_SIZE
#endif
    PUSH_CALLER_REG /* sp must be 16 bytes aligned, now size is 32 REGBYTES (64 REGBYTES when FPU on) */
#ifdef LOSCFG_ARCH_FPU_ENABLE
    frcsr  t2 /* use t2 after save it in PUSH_CALLER_REG */
    SREG   t2,  FCSR_STACK_OFFSET(sp) /* store frcsr to to previously reseverd (4*REGBYTES) space for FPU */
#endif

#ifdef LOSCFG_KERNEL_PERF
    csrr    a0, mepc
    mv      a1, s0
    call    OsPerfSetIrqRegs
#endif

    csrr    a0, mcause
    li      a1, MCAUSE_MASK_INT_BIT
    li      a2, MCAUSE_MASK_INT_NUM
    and     a1, a0, a1
    and     a0, a0, a2            /* interrupt num */

    beqz    a1, TrapHandle        /* mcause and mepc will be saved in TrapHandle */

    csrr    t0, mcause
    sw      t0, 0 * REGBYTES(sp)  /* store mcause to previously reseverd (4*REGBYTES) space by PUSH_CALLER_REG */
    csrr    t0, mepc
    sw      t0, 1 * REGBYTES(sp)  /* store mepc to previously reseverd (4*REGBYTES) space by PUSH_CALLER_REG */

#ifdef LOSCFG_ARCH_INTERRUPT_PREEMPTION
    csrrw   sp, mscratchcswl, sp /* conditionally swap the stack pointer. */
    li      a1, NUM_HAL_INTERRUPT_TIMER
    beq     a0, a1, TickHandle /* If the interrupt is a tick interrupt, other interrupts are not allowed during OsIntEntry. */
    j       FirstInterruptHandle
TickHandle:
    call    OsIntEntry
    j       InterruptRestore
FirstInterruptHandle:
    csrrsi  a0, mnxti, LOS_MSTATUS_MIE /* Get highest current interrupt and enable interrupts. */
    beqz    a0, Exit
ServiceLoop:
    lw      a1, (a0)
    jalr    a1 /* Call C ABI Routine */
    csrrsi  a0, mnxti, LOS_MSTATUS_MIE
    bnez    a0, ServiceLoop
InterruptRestore:
    la      a0, __irq_stack_top
    beq     a0, sp, InterruptPost /* if a0 is eqaul to sp, there is no interrupt. */
    csrrci  x0, mstatus, LOS_MSTATUS_MIE /* disable interrupts. */
    lw      a0, 0 * REGBYTES(sp)
    csrw    mcause, a0           /* load mcause from previously reseverd (4 * REGBYTES) space */
    lw      a0, 1 * REGBYTES(sp)
    csrw    mepc, a0             /* load mepc from previously reseverd (4 * REGBYTES) space */
    POP_CALLER_REG
    mret
#else
    csrrw   sp, mscratchcswl, sp
    call    OsIntEntry
    csrrw   sp, mscratchcswl, sp
    j       TaskSwitch
#endif
InterruptPost:
    csrrci  x0, mstatus, LOS_MSTATUS_MIE
    csrrw   sp, mscratch, sp
TaskSwitch:
    call    OsSchedProcSchedFlag
    POP_IRQ_REG
    mret

#ifdef LOSCFG_ARCH_INTERRUPT_PREEMPTION
Exit:
    csrrci  x0, mstatus, LOS_MSTATUS_MIE
    csrrw   sp, mscratchcswl, sp
    j       TaskSwitch
#endif