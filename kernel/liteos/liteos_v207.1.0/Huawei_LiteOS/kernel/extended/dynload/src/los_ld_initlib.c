/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Ld InitLib
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#include "los_ld_initlib_pri.h"
#include "los_ld_elflib.h"
#include "los_ld_symlib_pri.h"
#include "los_ld_modulelib_pri.h"
#include "los_ld_commlib_pri.h"
#include "los_ld_dllib_pri.h"
#include "los_ld_macro_pri.h"
#include "los_memory.h"
#include "los_hwi.h"
#include "los_mux.h"
#include "los_init.h"

LOS_HAL_TABLE_BEGIN(g_dynloadGlobalSymbolBegin, dynload_gsymbol);
LOS_HAL_TABLE_END(g_dynloadGlobalSymbolEnd, dynload_gsymbol);

LOS_DL_LIST g_dlLoadQueue;
LOS_DL_LIST g_dlRelocQueue;

UINT32 g_dlLoadQueueLocker;
UINT32 g_dlRelocQueueLocker;

/* module management */
LOS_DL_LIST g_freeHandleList;
DynModuleInfo g_sysModuleInfo[N_MAX_MODULES];

/* a handler contains symbols defined by OS */
STATIC DynModuleInfo *g_osSymtblModuleHandler = NULL;

LITE_OS_SEC_BSS VOID *g_dynloadHeap = NULL;
STATIC UINT8 *g_osDynloadHeap = NULL;
STATIC UINT32 g_loadLocker;

LITE_OS_SEC_TEXT_MINOR UINT32 LD_DlLoadQueueLockerInit(VOID)
{
    return LOS_MuxCreate(&g_dlLoadQueueLocker);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlLoadQueueLockerDel(VOID)
{
    UINT32 ret;
    ret = LOS_MuxDelete(g_dlLoadQueueLocker);
    if (ret != LOS_OK) {
        PRINT_ERR("%s err:0x%x\n", __FUNCTION__, ret);
    }
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlLoadQueueInit(VOID)
{
    LOS_ListInit(&g_dlLoadQueue);
}

LITE_OS_SEC_TEXT_MINOR UINT32 LD_DlRelocQueueLockerInit(VOID)
{
    return LOS_MuxCreate(&g_dlRelocQueueLocker);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlRelocQueueLockerDel(VOID)
{
    UINT32 ret;
    ret = LOS_MuxDelete(g_dlRelocQueueLocker);
    if (ret != LOS_OK) {
        PRINT_ERR("%s err:0x%x\n", __FUNCTION__, ret);
    }
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlRelocQueueInit(VOID)
{
    LOS_ListInit(&g_dlRelocQueue);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_InitHandler(DynModuleInfo *moduleHandle)
{
    (VOID)memset(moduleHandle, 0, sizeof(DynModuleInfo));

    LOS_ListInit(&moduleHandle->manageList);
    LOS_ListInit(&moduleHandle->comSymInfoList);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_ModuleInfoInit(VOID)
{
    UINT32 idx;
    UINT32 intSave;
    DynModuleInfo *moduleHandle = NULL;

    intSave = LOS_IntLock();
    LOS_ListInit(&g_freeHandleList);
    for (idx = 0; idx < N_MAX_MODULES; ++idx) {
        moduleHandle = GET_MODULE_BY_ID(idx);

        LD_InitHandler(moduleHandle);
        LOS_ListAdd(&g_freeHandleList, &moduleHandle->manageList);
    }
    LOS_IntRestore(intSave);
}

LITE_OS_SEC_TEXT_MINOR BOOL LD_IsLoaderInit(VOID)
{
    return (g_osSymtblModuleHandler != NULL);
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_BuildOSSymtbl(const CHAR *elfFileName)
{
    SymbolManage *symManage = NULL;
    DynModuleInfo *moduleHandle = LD_ModuleHandleAlloc(elfFileName);
    if (moduleHandle == NULL) {
        return LOS_NOK;
    }
    symManage = &moduleHandle->symManage;

    SET_LOADED(moduleHandle);
    INC_SOHEAD(moduleHandle);
    moduleHandle->refCnt = 1;
    moduleHandle->type = ET_SO;
    g_osSymtblModuleHandler = moduleHandle;

    symManage->symTotalNum = (((UINTPTR)&g_dynloadGlobalSymbolEnd) - ((UINTPTR)&g_dynloadGlobalSymbolBegin)) /
                             sizeof(LDSymbol);
    symManage->symNum = symManage->symTotalNum;
    if (symManage->symTotalNum != 0) {
        symManage->symTbl = &g_dynloadGlobalSymbolBegin;
    }
    symManage->strTotalLen = 0;
    symManage->strLen = 0;
    symManage->strTbl = NULL;

    if (LD_ModuleSymTblSort(moduleHandle) != LOS_OK) {
        return LOS_NOK;
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LD_LoadLock(VOID)
{
    UINT32 ret;

    ret = LOS_MuxPend(g_loadLocker, LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
        PRINT_ERR("loadlock err, 0x%x\n", ret);
    }
    return ret;
}

LITE_OS_SEC_TEXT_MINOR VOID LD_LoadUnlock(VOID)
{
    (VOID)LOS_MuxPost(g_loadLocker);
}

LITE_OS_SEC_TEXT_MINOR UINT32 LD_LoadLockerInit(VOID)
{
    return LOS_MuxCreate(&g_loadLocker);
}

BOOL CheckDynNxHeap(VOID)
{
    return (g_dynloadHeap == g_osDynloadHeap);
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_Init(VOID)
{
    const CHAR *name = "dynload_global_symbol_table_name";

    if (LD_IsLoaderInit() == TRUE) {
        return LOS_OK;
    }
    if (g_osDynloadHeap == NULL) {
        if (LOS_GET_NX_CFG()) {
            UINT32 poolSize, ret;
            g_osDynloadHeap = (UINT8 *)LOS_GET_DL_NX_HEAP_BASE();
            poolSize = LOS_GET_DL_NX_HEAP_SIZE();
            ret = LOS_MemInit(g_osDynloadHeap, poolSize);
            PRINT_INFO("LiteOS dynload heap memory size:0x%x\n", poolSize);
            if (ret != LOS_OK) {
                return LOS_NOK;
            }
        } else {
            g_osDynloadHeap = m_aucSysMem1;
        }
    }

    if (g_dynloadHeap == NULL) {
        g_dynloadHeap = (VOID *)g_osDynloadHeap;
    }

    LD_ModuleInfoInit();

    if (LD_DlLoadQueueLockerInit() != LOS_OK) {
        goto LOAD_ERR;
    }

    if (LD_DlRelocQueueLockerInit() != LOS_OK) {
        goto RELOC_ERR;
    }

    LD_DlLoadQueueInit();
    LD_DlRelocQueueInit();

    g_osSymtblModuleHandler = (DynModuleInfo *)1;

    if (LD_BuildOSSymtbl(name) != LOS_OK) {
        goto LOS_ERREND;
    }

    return LOS_OK;

LOS_ERREND:
    LD_DlRelocQueueLockerDel();
RELOC_ERR:
    LD_DlLoadQueueLockerDel();
LOAD_ERR:
    return LOS_NOK;
}

LITE_OS_SEC_TEXT_MINOR VOID LOS_LdDestroy(VOID)
{
    UINT32 idx;
    DynModuleInfo *moduleHandle = NULL;

    if (LD_LoadLock() != LOS_OK) {
        return;
    }
    OsPathDelete();
    if (LD_IsLoaderInit() == FALSE) {
        goto ERROUT;
    }

    LD_DlLoadQueueLockerDel();
    LD_DlRelocQueueLockerDel();

    for (idx = 0; idx < N_MAX_MODULES; ++idx) {
        moduleHandle = GET_MODULE_BY_ID(idx);
        if (!IS_TAKEN(moduleHandle) || !IS_LOADED(moduleHandle)) {
            continue;
        }

        if (moduleHandle->type == ET_OBJ) {
            (VOID)LD_ModuleUnload(moduleHandle);
            continue;
        }

        if (!IS_SOHEAD(moduleHandle)) {
            continue;
        }

        do {
            if (LD_ModuleUnload(moduleHandle) != LOS_OK) {
                PRINT_ERR("%s failed\n", __FUNCTION__);
                break;
            }
        } while (moduleHandle->refCnt);
    }

    g_osSymtblModuleHandler = NULL;
    g_dynloadHeap = NULL;

ERROUT:
    LD_LoadUnlock();
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsDynloadInit(VOID)
{
    return LD_LoadLockerInit();
}
LOS_SYS_INIT(OsDynloadInit, SYS_INIT_LEVEL_KERNEL_ADDITION, SYS_INIT_SYNC_0);
