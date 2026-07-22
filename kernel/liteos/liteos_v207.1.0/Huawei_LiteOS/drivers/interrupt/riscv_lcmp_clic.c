/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description : Implemention Of The Riscv CLIC Interrupt.
 * Author: Huawei LiteOS Team
 * Create : 2021-09-03
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
 * ---------------------------------------------------------------------------- */

#include "los_hwi_pri.h"
#include "soc/clic.h"
#include "arch/regs.h"

#define CLIC_CFG_REGISTER_BITS_NUM 8
#define CLIC_CFG_INTERRUPT_OFFSET 1
#define SET_SYS_PRIOR(usrPrior)    ((UINT8)((LOSCFG_HWI_PRIO_LIMIT - 1) + CLIC_CFG_INTERRUPT_OFFSET - (usrPrior)))

#if LOSCFG_HWI_PRIO_LIMIT > CLIC_INFO_CLICINTCTLBITS_PRIO_LIMIT
#error "Actual interrupt indication bits less than cliccfg.nlbits."
#endif

UINT32 g_clicCfgLevelBitsNum = 1; /* the lowest level bits num is 1 */

__attribute__((aligned(64))) HWI_PROC_FUNC g_vectorIntTable[LOSCFG_PLATFORM_HWI_LIMIT] = {0}; /* non-vector */

LITE_OS_SEC_BSS HwiHandleInfo g_hwiForm[LOSCFG_PLATFORM_HWI_LIMIT] = {0};

UINT32 HalIrqSetHandler(UINT32 hwiNum, HWI_PROC_FUNC handler)
{
    UINT32 intSave;
    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }
    intSave = LOS_IntLock();
    g_vectorIntTable[hwiNum] = handler;
    LOS_IntRestore(intSave);
    return LOS_OK;
}

UINT32 HalIrqUnmask(UINT32 hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    WRITE_UINT8(1, CLIC_REG_BASE + CLIC_INTIE(hwiNum));
    return LOS_OK;
}

UINT32 HalIrqMask(UINT32 hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    WRITE_UINT8(0, CLIC_REG_BASE + CLIC_INTIE(hwiNum));
    return LOS_OK;
}

UINT32 HalIrqTrigger(UINT32 hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    WRITE_UINT8(1, CLIC_REG_BASE + CLIC_INTIP(hwiNum));
    return LOS_OK;
}

UINT32 HalIrqClear(UINT32 hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    WRITE_UINT8(0, CLIC_REG_BASE + CLIC_INTIP(hwiNum));
    return LOS_OK;
}

UINT8 HalIrqGetClicCfg(VOID)
{
    return GET_UINT8(CLIC_REG_BASE + CLIC_CFG);
}

VOID HalIrqSetClicCfg(UINT8 clicCfg)
{
    WRITE_UINT8(clicCfg, CLIC_REG_BASE + CLIC_CFG);
}

UINT32 HalIrqSetClicIntAttr(UINT32 hwiNum, UINT8 clicintattr)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }
    WRITE_UINT8(clicintattr, CLIC_REG_BASE + CLIC_INTATTR(hwiNum));
    return LOS_OK;
}

UINT32 HalIrqSetPrio(UINT32 hwiNum, HWI_PRIOR_T level)
{
    UINT8 localLevel;

    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }
    if (level >= LOSCFG_HWI_PRIO_LIMIT) {
        return LOS_ERRNO_HWI_PRIO_INVALID;
    }
    /* riscv's priority range is [1, 7] */
    localLevel = SET_SYS_PRIOR(level);
    localLevel = (UINT32)localLevel << (8 - g_clicCfgLevelBitsNum); /* max 8 bit */
    UINT8 mask = ((UINT8)(-1)) >> (g_clicCfgLevelBitsNum);
    UINT8 clicIntCtl = GET_UINT8(CLIC_REG_BASE + CLIC_INTCTL(hwiNum));
    clicIntCtl = clicIntCtl & mask;
    clicIntCtl = clicIntCtl | localLevel;
    WRITE_UINT8(clicIntCtl, CLIC_REG_BASE + CLIC_INTCTL(hwiNum));
    return LOS_OK;
}

CHAR *HalIrqVersion(VOID)
{
    return "CLIC";
}

HwiHandleInfo *HalIrqGetHandleForm(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return NULL;
    }

    return &g_hwiForm[hwiNum];
}

UINT32 HalCurIrqGet(VOID)
{
    return (READ_CSR(mcause)) & 0xfff;
}

VOID HalIrqHandler(VOID)
{
    UINT32 hwiNum = HalCurIrqGet();
    if (!HWI_NUM_VALID(hwiNum)) {
        return;
    }

    OsIntHandle(hwiNum, &g_hwiForm[hwiNum]);
}

UINT32 HalIrqPendingGet(UINT32 hwiNum, UINT8 *isPending)
{
    if (!HWI_NUM_VALID(hwiNum) || (isPending == NULL)) {
        return LOS_NOK;
    }

    READ_UINT8(*isPending, CLIC_REG_BASE + CLIC_INTIP(hwiNum));
    /* get one bit */
    return LOS_OK;
}

#ifdef LOSCFG_DEBUG_HWI
STATIC INLINE UINT32 GetUsrPri(UINT32 sysPrior)
{
    return ((LOSCFG_HWI_PRIO_LIMIT - 1) + CLIC_CFG_INTERRUPT_OFFSET - (sysPrior));
}

STATIC UINT32 HalIrqGetPrio(UINT32 hwiNum)
{
    UINT32 clicIntCtl = GET_UINT8(CLIC_REG_BASE + CLIC_INTCTL(hwiNum));

    return GetUsrPri((clicIntCtl >> (8 - g_clicCfgLevelBitsNum)));  /* max 8 bit */
}

STATIC UINT32 HalIrqStatusGet(HWI_HANDLE_T hwiNum, HwiStatus *status)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    status->pri = HalIrqGetPrio(hwiNum);
    status->affinity = 0x1U;
    READ_UINT8(status->enable, CLIC_REG_BASE + CLIC_INTIE(hwiNum));

    (VOID)HalIrqPendingGet(hwiNum, (UINT8 *)&status->pending);
    return LOS_OK;
}
#endif

STATIC const HwiControllerOps g_clicOps = {
    .triggerIrq     = HalIrqTrigger,
    .clearIrq       = HalIrqClear,
    .enableIrq      = HalIrqUnmask,
    .disableIrq     = HalIrqMask,
    .setIrqPriority = HalIrqSetPrio,
    .getCurIrqNum   = HalCurIrqGet,
    .getIrqVersion  = HalIrqVersion,
    .getHandleForm  = HalIrqGetHandleForm,
    .handleIrq      = HalIrqHandler,
#ifdef LOSCFG_DEBUG_HWI
    .getIrqStatus   = HalIrqStatusGet,
#endif
};

VOID HalIrqInit(VOID)
{
    /* set cliccfg reg. nvbits = 0; nlbits = 4; nmbits = 0. */
    UINT8 cliccfg = 0;
    cliccfg |= (4 << 1); /* non-vector, m-mode */
    WRITE_UINT8(cliccfg, CLIC_REG_BASE + CLIC_CFG);

    for (UINT32 i = 0; i < CLIC_CFG_REGISTER_BITS_NUM; i++) {
        if (((LOSCFG_HWI_PRIO_LIMIT - 1) & ((0x80) >> i)) != 0) {
            g_clicCfgLevelBitsNum = 8 - i; /* max 8bit */
            break;
        }
    }
    UINT8 clicintattr = 0;
    /* clicintattr[0] = 0 non-vector; clicintattr[2:1] = 1 edge-triggered; clicintattr[7:6] = 3 M-mode */
    clicintattr |= ((1 << 1) | (3 << 6));

    UINT32 index;
    for (index = 0; index < LOSCFG_PLATFORM_HWI_LIMIT; index++) {
        g_vectorIntTable[index] = (HWI_PROC_FUNC)OsIntEntry;
        g_hwiForm[index].hook = NULL;
        g_hwiForm[index].shareMode = 0;
#ifdef LOSCFG_SHARED_IRQ
        g_hwiForm[index].next = NULL;
#endif

        WRITE_UINT8(0, CLIC_REG_BASE + CLIC_INTIP(index));      /* clear pending */
        WRITE_UINT8(0, CLIC_REG_BASE + CLIC_INTIE(index));      /* disabled */
        WRITE_UINT8(clicintattr, CLIC_REG_BASE + CLIC_INTATTR(index));
    }

    OsHwiControllerReg(&g_clicOps);
}
