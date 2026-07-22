/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Hwi Debug
 * Author: Huawei LiteOS Team
 * Create: 2021-11-08
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
#include "los_hwi_pri.h"

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
#include "los_cpup_pri.h"
#endif

STATIC VOID PrintHwiInfoInfoTitle(VOID)
{
    PRINTK("InterruptNo     Share     ResponseCount     Name             DevId/Param");
#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    PRINTK("        CYCLECOST    CPUP    CPUP%3d.%ds    CPUP%3d.%ds",
        OS_CPUP_MULTI_PERIOD_S, OS_CPUP_MULTI_PERIOD_MS, OS_CPUP_PERIOD_S, OS_CPUP_PERIOD_MS);
#endif
    PRINTK("\n");

    PRINTK("-----------     -----     -------------     ---------        -----------");
#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    PRINTK("    ---------     ----    ----------    ----------");
#endif
    PRINTK("\n");
}

STATIC BOOL GetHwiShare(const HwiHandleInfo *hwiForm)
{
#ifndef LOSCFG_SHARED_IRQ
    return false;
#else
    return (hwiForm->shareMode);
#endif
}

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
STATIC CPUP_INFO_S g_hwiCpupAll[LOSCFG_PLATFORM_HWI_LIMIT];
STATIC CPUP_INFO_S g_hwiCpupMultiRecord[LOSCFG_PLATFORM_HWI_LIMIT];
STATIC CPUP_INFO_S g_hwiCpupOneRecord[LOSCFG_PLATFORM_HWI_LIMIT];

STATIC VOID PrintHwiCpupInfo(UINT32 irqId)
{
    UINT64 cycles = 0;
    UINT32 irqCpupCbId = OsCpupIrqCBIdGet(irqId, TRUE);
    UINT32 irqCpupCbTotalId = OsCpupIrqCBIdGet(irqId, FALSE);
    UINT32 respCount;
    UINT32 ret = LOS_HwiRespCntGet(irqId, &respCount);
    if ((ret != LOS_OK) || (irqCpupCbId == UINT32_MAX)) {
        return;
    } else if (respCount != 0) {
        cycles = (OsCpupCBGet(irqCpupCbTotalId))->allTime / respCount;
    }

    PRINTK("    %-10llu  %2u.%-7u %2u.%-7u    %2u.%-6u", cycles,
           g_hwiCpupAll[irqCpupCbId].uwUsage / LOS_CPUP_PRECISION_MULT,
           g_hwiCpupAll[irqCpupCbId].uwUsage % LOS_CPUP_PRECISION_MULT,
           g_hwiCpupMultiRecord[irqCpupCbId].uwUsage / LOS_CPUP_PRECISION_MULT,
           g_hwiCpupMultiRecord[irqCpupCbId].uwUsage % LOS_CPUP_PRECISION_MULT,
           g_hwiCpupOneRecord[irqCpupCbId].uwUsage / LOS_CPUP_PRECISION_MULT,
           g_hwiCpupOneRecord[irqCpupCbId].uwUsage % LOS_CPUP_PRECISION_MULT);
}

STATIC VOID OsHwiCpupInfoGet(VOID)
{
    UINT32 intSave;

    (VOID)memset(g_hwiCpupAll, 0, sizeof(g_hwiCpupAll));
    (VOID)memset(g_hwiCpupMultiRecord, 0, sizeof(g_hwiCpupMultiRecord));
    (VOID)memset(g_hwiCpupOneRecord, 0, sizeof(g_hwiCpupOneRecord));

    intSave = LOS_IntLock();
    (VOID)LOS_AllCpuUsage(LOSCFG_PLATFORM_HWI_LIMIT, g_hwiCpupAll, CPUP_ALL_TIME, 0);
    (VOID)LOS_AllCpuUsage(LOSCFG_PLATFORM_HWI_LIMIT, g_hwiCpupMultiRecord, CPUP_LAST_MULIT_RECORD, 0);
    (VOID)LOS_AllCpuUsage(LOSCFG_PLATFORM_HWI_LIMIT, g_hwiCpupOneRecord, CPUP_LAST_ONE_RECORD, 0);
    LOS_IntRestore(intSave);
}
#endif

STATIC UINT32 HwiCreatedStatusGet(HWI_HANDLE_T hwiNum, VOID *devID, UINT32 *isCreated)
{
    HwiHandleInfo *hwiForm = NULL;
    UINT32 intSave;

    hwiForm = OsGetHwiForm(hwiNum);
    if (hwiForm == NULL) {
        PRINT_ERR("hwiNum %u is invalid\n", hwiNum);
        return LOS_NOK;
    }
    *isCreated = FALSE;

#ifndef LOSCFG_SHARED_IRQ
    (VOID)intSave;
    if (devID != NULL) {
        PRINT_ERR("%u is not shared interrupt, dont need parameter [devId]\n", hwiNum);
        return LOS_NOK;
    }
    if (hwiForm->hook != NULL) {
        *isCreated = TRUE;
    }
#else
    /* Shared interrupt. */
    if ((hwiForm->shareMode & IRQF_SHARED)) {
        if (devID == NULL) {
            PRINT_ERR("%u is shared interrupt, but parameter [devId] is NULL\n", hwiNum);
            return LOS_NOK;
        }
        HWI_LOCK(intSave);
        while (hwiForm->next != NULL) {
            hwiForm = hwiForm->next;
            if (((HWI_IRQ_PARAM_S *)(hwiForm->registerInfo))->pDevId == devID) {
                *isCreated = TRUE;
                break;
            }
        }
        HWI_UNLOCK(intSave);
    } else if (hwiForm->next != NULL) {
        if (devID != NULL) {
            PRINT_ERR("%u is not shared interrupt, dont need parameter [devId]\n", hwiNum);
            return LOS_NOK;
        }
        *isCreated = TRUE;
    }
#endif
    return LOS_OK;
}

STATIC UINT32 HwiAttrStatusGet(UINT32 hwiNum, HwiStatus *stat)
{
    if ((g_hwiOps == NULL) || (g_hwiOps->getIrqStatus == NULL)) {
        return LOS_ERRNO_HWI_PROC_FUNC_NULL;
    }

    return g_hwiOps->getIrqStatus(hwiNum, stat);
}

STATIC VOID HwiStatusTitlePrint(VOID)
{
    PRINTK("InterruptNo     DevId   Priority    Affinity    Created     Enable      Pending\n");
    PRINTK("-----------     -----   --------    --------    -------     ------      -------\n");
}

STATIC VOID HwiStatusDetailPrint(UINT32 hwiNum, HwiStatus stat, VOID *devId)
{
    PRINTK("%-8u   \t0x%lx   \t%-8d    0x%-8x", hwiNum, (UINTPTR)devId, (INT32)stat.pri, stat.affinity);
    PRINTK("\t%-8d    %-8d    %-8d\n", (INT32)stat.isCreated, (INT32)stat.enable, (INT32)stat.pending);
}

VOID OsHwiStatusAllPrint(VOID)
{
    UINT32          irqId;
    HwiHandleInfo   *hwiForm = NULL;
    UINT32          intSave;
    VOID            *devId = NULL;
    HwiStatus        stat;

    HwiStatusTitlePrint();
    for (irqId = 0; irqId < LOSCFG_PLATFORM_HWI_LIMIT; irqId++) {
        if (!HWI_IS_REGISTED(irqId)) {
            continue;
        }
        stat.isCreated = TRUE;
#ifndef LOSCFG_SHARED_IRQ
        (VOID)intSave;
        (VOID)hwiForm;
        (VOID)devId;
        (VOID)HwiAttrStatusGet(irqId, &stat);
        HwiStatusDetailPrint(irqId, stat, NULL);
#else
        /* Shared interrupt. */
        hwiForm = OsGetHwiForm(irqId);
        if (hwiForm == NULL) {
            PRINT_ERR("[%s, %d]OsGetHwiForm err!\n", __FUNCTION__, __LINE__);
            return;
        }

        if ((hwiForm->shareMode & IRQF_SHARED)) {
            HWI_LOCK(intSave);
            while (hwiForm->next != NULL) {
                hwiForm = hwiForm->next;
                devId = ((HWI_IRQ_PARAM_S *)(hwiForm->registerInfo))->pDevId;
                (VOID)HwiAttrStatusGet(irqId, &stat);
                HwiStatusDetailPrint(irqId, stat, devId);
            }
            HWI_UNLOCK(intSave);
        } else if (hwiForm->next != NULL) {
            (VOID)HwiAttrStatusGet(irqId, &stat);
            HwiStatusDetailPrint(irqId, stat, NULL);
        }
#endif
    }
}

UINT32 OsHwiStatusGet(HWI_HANDLE_T hwiNum, VOID *devId, HwiStatus *hwiStatus)
{
    UINT32 ret;
    if (hwiStatus == NULL) {
        return LOS_ERRNO_HWI_PTR_NULL;
    }

    ret = HwiCreatedStatusGet(hwiNum, devId, &hwiStatus->isCreated);
    if (ret != LOS_OK) {
        return ret;
    }
    return HwiAttrStatusGet(hwiNum, hwiStatus);
}

VOID OsHwiStatusPrint(UINT32 hwiNum, VOID *devId)
{
    HwiStatus stat;
    UINT32 ret;

    ret = OsHwiStatusGet(hwiNum, devId, &stat);
    if (ret != LOS_OK) {
        return;
    }
    HwiStatusTitlePrint();
    HwiStatusDetailPrint(hwiNum, stat, devId);
}

VOID PrintHwiInfo(VOID)
{
    UINT32 irqId;
    HwiHandleInfo *hwiForm = NULL;
    UINT32 respCount;

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    OsHwiCpupInfoGet();
#endif
    PrintHwiInfoInfoTitle();
    for (irqId = 0; irqId < LOSCFG_PLATFORM_HWI_LIMIT; irqId++) {
        if (!HWI_IS_REGISTED(irqId)) {
            continue;
        }
        hwiForm = OsGetHwiForm(irqId);
        if (hwiForm == NULL) {
            PRINT_ERR("[%s, %d]OsGetHwiForm err!\n", __FUNCTION__, __LINE__);
            return;
        }

        (VOID)LOS_HwiRespCntGet(irqId, &respCount);
        PRINTK("%-8u\t  %-s\t  %-10u", irqId, GetHwiShare(hwiForm) ? "Y" : "N", respCount);
#ifdef LOSCFG_SHARED_IRQ
        hwiForm = hwiForm->next;
#endif
        if ((hwiForm->registerInfo != 0) && ((HWI_IRQ_PARAM_S *)hwiForm->registerInfo)->pName != NULL) {
            PRINTK("\t    %-16s 0x%-.*lx", ((HWI_IRQ_PARAM_S *)hwiForm->registerInfo)->pName,
                   OS_HEX_ADDR_WIDTH, ((HWI_IRQ_PARAM_S *)hwiForm->registerInfo)->pDevId);
        } else {
#ifdef LOSCFG_CPUP_INCLUDE_IRQ
            PRINTK("\t                               ");
#endif
        }

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
        PrintHwiCpupInfo(irqId);
#endif
        PRINTK("\n");

#ifdef LOSCFG_SHARED_IRQ
        while ((hwiForm = hwiForm->next) != NULL) {
            PRINTK("\t\t\t\t\t    %-16s 0x%-.*lx\n", ((HWI_IRQ_PARAM_S *)hwiForm->registerInfo)->pName,
                   OS_HEX_ADDR_WIDTH, ((HWI_IRQ_PARAM_S *)hwiForm->registerInfo)->pDevId);
        }
#endif
    }
}
