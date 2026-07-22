/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Ld ModuleLib
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

#include "los_ld_symlib_pri.h"
#include "los_ld_modulelib_pri.h"
#include "los_ld_initlib_pri.h"
#include "los_ld_commlib_pri.h"
#include "los_ld_macro_pri.h"
#include "los_ld_elflib_pri.h"
#include "los_ld_dllib_pri.h"
#include "stdlib.h"
#include "limits.h"
#include "los_memory.h"
#include "los_hwi.h"

#if defined(LOSCFG_ARCH_CORTEX_A7) || defined(LOSCFG_ARCH_CORTEX_A53_AARCH32)
#include "arch/mmu.h"
#endif

LITE_OS_SEC_TEXT_MINOR DynModuleInfo *LD_ModuleHandleAlloc(const CHAR *elfFileName)
{
    size_t nameLen;
    DynModuleInfo *moduleInfoNode = NULL;

    if (elfFileName == NULL) {
        return NULL;
    }

    /* No more available handler */
    if (LOS_ListEmpty(&g_freeHandleList)) {
        PRINT_ERR("%s, g_freeHandleList is NULL\n", __FUNCTION__);
        return NULL;
    }

    moduleInfoNode = LOS_DL_LIST_ENTRY(LOS_DL_LIST_FIRST(&g_freeHandleList), DynModuleInfo, manageList);
    LOS_ListDelete(&moduleInfoNode->manageList);

    nameLen = strlen(elfFileName);
    moduleInfoNode->moduleFileName = (CHAR *)LOS_MemAlloc(g_dynloadHeap, (UINT32)(nameLen + 1));
    if (moduleInfoNode->moduleFileName == NULL) {
        LOS_ListAdd(&g_freeHandleList, &moduleInfoNode->manageList);
        return NULL;
    }
    (VOID)strcpy(moduleInfoNode->moduleFileName, elfFileName);
    moduleInfoNode->moduleFileName[nameLen] = '\0';

    SET_TAKEN(moduleInfoNode);
    return moduleInfoNode;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_ModuleMemRelease(DynModuleInfo *moduleHandle)
{
    UINT32 ret;

    if (moduleHandle->segmentsAddr != 0) {
#if defined(LOSCFG_ARCH_CORTEX_A7) || defined(LOSCFG_ARCH_CORTEX_A53_AARCH32)
        if (LOS_GET_NX_CFG() && CheckDynNxHeap()) {
            DYN_NX_UNPROTECT(moduleHandle->segmentsAddr, moduleHandle->segmentsSize);
        }
#endif
        ret = LOS_MemFree(g_dynloadHeap, (VOID *)moduleHandle->segmentsAddr);
        if (ret != LOS_OK) {
            return LOS_NOK;
        }
        moduleHandle->segmentsAddr = 0;
    }

    ret = LOS_MemFree(g_dynloadHeap, moduleHandle->moduleFileName);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }
    moduleHandle->moduleFileName = NULL;

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_ModuleDeinit(DynModuleInfo *moduleHandle)
{
    if (LD_ModuleMemRelease(moduleHandle) != LOS_OK) {
        PRINT_ERR("%s, %d\n", __FUNCTION__, __LINE__);
        return LOS_NOK;
    }

    LD_InitHandler(moduleHandle);

    /* Unleash this handler */
    LOS_ListAdd(&g_freeHandleList, &moduleHandle->manageList);

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DoModuleUnload(VOID *handle)
{
    UINT32 idx;
    UINT32 intSave;
    DynModuleInfo *moduleHandle = (DynModuleInfo *)handle;
    if (!IS_TAKEN(moduleHandle)) {
        return LOS_OK;
    }

    SET_UNLOADDING(moduleHandle);
    if (moduleHandle->type == ET_SO) {
        for (idx = 0; idx < N_MAX_MODULES; ++idx) {
            if (moduleHandle->apDlNeeded[idx] == NULL) {
                break;
            }

            if (IS_UNLOADDING(moduleHandle->apDlNeeded[idx])) {
                continue;
            }

            if (LD_DoModuleUnload(moduleHandle->apDlNeeded[idx]) != LOS_OK) {
                return LOS_NOK;
            }
        }

        if (moduleHandle->refCnt > 1) {
            moduleHandle->refCnt--;
            return LOS_OK;
        }
    }

    intSave = LOS_IntLock();
    /* Remove symbols defined by the module from system symbol table */
    if (LD_SymRemoveByModuleHandle(moduleHandle) != LOS_OK) {
        LOS_IntRestore(intSave);
        return LOS_NOK;
    }

    if (LD_ModuleDeinit(moduleHandle) != LOS_OK) {
        LOS_IntRestore(intSave);
        return LOS_NOK;
    }
    LOS_IntRestore(intSave);

    return LOS_OK;
}

VOID LD_FlagsClear(VOID)
{
    UINT32 idx;
    DynModuleInfo *moduleHandle = NULL;

    for (idx = 0; idx < N_MAX_MODULES; ++idx) {
        moduleHandle = GET_MODULE_BY_ID(idx);
        /* clear all the temporary flags. */
        CLEAR_TMPFLAGS(moduleHandle);
    }
}

VOID LD_SearchFlagsClear(VOID)
{
    UINT32 idx;
    DynModuleInfo *moduleHandle = NULL;

    for (idx = 0; idx < N_MAX_MODULES; ++idx) {
        moduleHandle = GET_MODULE_BY_ID(idx);
        /* only clear the searching flag. */
        CLEAR_SEARCHING(moduleHandle);
    }
}

#if defined(LOSCFG_DYNLOAD_DYN_FROM_MEM) || defined(LOSCFG_DYNLOAD_REL_FROM_MEM)
STATIC enum ElfType LD_ElfTypeGet(const CHAR *elfFileBuf)
{
    const LD_ELF_EHDR *ehdr = (LD_ELF_EHDR *)elfFileBuf;
    if (LD_ElfHdrVerify(ehdr) != LOS_OK) {
        return ELF_TYPE_NOTYPE;
    }

    return (enum ElfType)ehdr->elfType;
}

STATIC INLINE UINT32 LD_MemLoadParamCheck(const CHAR *elfFileName, UINT32 fileNameLen,
                                          const CHAR *elfFileBuf, UINT32 bufLen)
{
    if ((elfFileName == NULL) || (elfFileBuf == NULL) || (bufLen < sizeof(LD_ELF_EHDR)) ||
        (fileNameLen == 0) || (fileNameLen >= PATH_MAX)) {
        PRINT_ERR("invalid params\n");
        return LOS_NOK;
    }
    if (fileNameLen != strlen(elfFileName)) {
        PRINT_ERR("fileNameLen:%u is not equal to strlen(%s)\n", fileNameLen, elfFileName);
        return LOS_NOK;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR VOID *LOS_MemLoad(const CHAR *elfFileName, UINT32 fileNameLen,
                                         const CHAR *elfFileBuf, UINT32 bufLen)
{
    SrcInfo srcInfo = { -1, elfFileBuf, bufLen, 0 };
    VOID *moduleHandle = NULL;
    enum ElfType elfType;

    if (LD_MemLoadParamCheck(elfFileName, fileNameLen, elfFileBuf, bufLen) != LOS_OK) {
        return NULL;
    }

    elfType = LD_ElfTypeGet(elfFileBuf);

    if (LD_LoadLock() != LOS_OK) {
        return NULL;
    }

    if (LD_IsLoaderInit() == FALSE) {
        if (LD_Init() != LOS_OK) {
            goto OUT;
        }
    }

#ifdef LOSCFG_DYNLOAD_DYN_FROM_MEM
    if (elfType == ET_SO) {
        moduleHandle = LD_DynLoadModule(&srcInfo, elfFileName, TRUE, LD_MODE_MEM);
    }
#endif
#ifdef LOSCFG_DYNLOAD_REL_FROM_MEM
    if (elfType == ET_OBJ) {
        moduleHandle = LD_RelLoadModule(&srcInfo, elfFileName, TRUE, LD_MODE_MEM);
    }
#endif

OUT:
    LD_LoadUnlock();
    return moduleHandle;
}
#endif /* defined(LOSCFG_DYNLOAD_DYN_FROM_MEM) || defined(LOSCFG_DYNLOAD_REL_FROM_MEM) */

LITE_OS_SEC_TEXT_MINOR INT32 LD_ModuleUnload(VOID *handle)
{
    INT32 ret = LD_DoModuleUnload(handle);

    /* clear the need-unload flags be used during unloading. */
    LD_FlagsClear();
    return ret;
}

LITE_OS_SEC_TEXT_MINOR INT32 LOS_ModuleUnload(VOID *handle)
{
    DynModuleInfo *moduleHandle = NULL;
    INT32 ret = LOS_NOK;

    if (handle == NULL) {
        return LOS_NOK;
    }

    if (LD_LoadLock() != LOS_OK) {
        return LOS_NOK;
    }

    if (LD_IsLoaderInit() == FALSE) {
        goto OUT;
    }

    if (LD_IsHandleLegal(handle) == FALSE) {
        goto OUT;
    }
    moduleHandle = (DynModuleInfo *)handle;
    /* users only can unload a module been loaded */
    if (!IS_TAKEN(moduleHandle) || !IS_LOADED(moduleHandle)) {
        goto OUT;
    }

    if (moduleHandle->type == ET_SO) {
        if (IS_SOHEAD(moduleHandle)) {
            DEC_SOHEAD(moduleHandle);
        } else {
            goto OUT;
        }
    }

    ret = LD_ModuleUnload(handle);
OUT:
    LD_LoadUnlock();
    return ret;
}
