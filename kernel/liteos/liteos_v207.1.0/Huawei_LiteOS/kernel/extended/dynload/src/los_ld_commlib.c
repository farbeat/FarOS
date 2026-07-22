/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Ld CommLib
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

#include "los_ld_commlib_pri.h"
#include "los_ld_macro_pri.h"
#include "los_ld_initlib_pri.h"
#include "los_ld_modulelib_pri.h"
#include "limits.h"
#include "unistd.h"
#include "los_hw.h"
#include "los_memory.h"
#include "los_mux.h"

LITE_OS_SEC_BSS_MINOR STATIC CHAR *g_apscDefaultPath[N_DEFAULT_PATH];

LITE_OS_SEC_TEXT_MINOR BOOL LD_ElfSymIsVisible(const LD_ELF_SYM *symbol)
{
    UINT32 symBinding; /* symbol visibility and behavior */
    UINT32 symAssoc; /* type of entity associated to sym */

    /* Determine the type of processing based on the symbol type */
    symBinding = LD_ELF_ST_BIND(symbol->stInfo);
    symAssoc = LD_ELF_ST_TYPE(symbol->stInfo);

    /* Only consider symbols defined in current module. */
    if (symbol->stShndx == LD_SHN_UNDEF) {
        return FALSE;
    }

    /* Only consider symbols related to functions and variables (objects) */
    if ((symAssoc != LD_STT_OBJECT) && (symAssoc != LD_STT_FUNC)) {
        return FALSE;
    }

    /* Depending on the load flags we add globals and locals or only globals */
    if ((symBinding != LD_STB_GLOBAL) && (symBinding != LD_STB_WEAK)) {
        return FALSE;
    }

    return TRUE;
}

LITE_OS_SEC_TEXT_MINOR INT8 LD_ElfSymTypeGet(const LD_ELF_SYM *symbol, const LD_ELF_SHDR *scnHdrTbl)
{
    UINT8 symType = LD_SYM_UNDF; /* internal value for symbol type */
    const LD_ELF_SHDR *scnHdr = scnHdrTbl + symbol->stShndx; /* pointer to section header */

    /*
     * For absolute symbols, don't consider the section type and flags. Otherwise, the type and flags of the section to
     * which belongs the symbol are tested to determined the type of the symbol.
     */
    if (symbol->stShndx == LD_SHN_ABS) {
        return LD_SYM_ABS;
    }

    switch (scnHdr->shType) {
        case LD_SHT_PROGBITS:
            if ((scnHdr->shFlags & LD_SHF_ALLOC) &&
                (scnHdr->shFlags & LD_SHF_EXECINSTR)) {
                /* This is text section */
                symType |= LD_SYM_TEXT;
            } else if ((scnHdr->shFlags & LD_SHF_ALLOC) &&
                       (scnHdr->shFlags & LD_SHF_WRITE)) {
                /* This is data section */
                symType |= LD_SYM_DATA;
            } else if (scnHdr->shFlags & LD_SHF_ALLOC) {
                /*
                 * This is a read only data section. these sections hold constant data,
                 * but since we don't have an appropriate symbol type
                 * their contents is considered as "data" by the loader so that tools,
                 * such as the shell, behave properly.
                 */
                symType |= LD_SYM_DATA;
            }
            break;
        case LD_SHT_NOBITS:
            if (scnHdr->shFlags & LD_SHF_ALLOC) {
                /* This is a bss section */
                symType |= LD_SYM_BSS;
            }
            break;
        default:
            /* Well, we don't handle this type of symbol. Give them type SYM_UNDF. */
            symType = LD_SYM_UNDF;
            break;
    }

    return (INT8)symType;
}

LITE_OS_SEC_TEXT_MINOR BOOL LD_IsHandleLegal(const VOID *handle)
{
    UINTPTR offset;
    DynModuleInfo *moduleHandle = (DynModuleInfo *)handle;

    if ((moduleHandle < g_sysModuleInfo) ||
        (moduleHandle >= (g_sysModuleInfo + N_MAX_MODULES))) {
        return FALSE;
    }

    offset = (UINTPTR)moduleHandle - (UINTPTR)g_sysModuleInfo;
    if ((offset % sizeof(DynModuleInfo)) != 0) {
        /* Test if is the legal offset */
        return FALSE;
    }

    return TRUE;
}

LITE_OS_SEC_TEXT_MINOR BOOL LD_IsModuleTaken(const CHAR *elfFileName, DynModuleInfo **moduleHandle)
{
    UINT32 idx;
    DynModuleInfo *moduleHandleTmp = NULL;

    for (idx = 0; idx < N_MAX_MODULES; ++idx) {
        moduleHandleTmp = GET_MODULE_BY_ID(idx);
        if (!IS_TAKEN(moduleHandleTmp)) {
            continue;
        }

        if (strcmp(moduleHandleTmp->moduleFileName, elfFileName) == 0) {
            *moduleHandle = moduleHandleTmp;
            return TRUE;
        }
    }

    *moduleHandle = NULL;
    return FALSE;
}

LITE_OS_SEC_TEXT_MINOR VOID LD_FlushDCache(UINTPTR start, UINTPTR end)
{
    flush_dcache(start, end);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_FlushICache(VOID)
{
    flush_icache();
}

LITE_OS_SEC_TEXT_MINOR BOOL LD_IsFileNameLegal(const CHAR *elfFileName)
{
    return ((elfFileName != NULL) && (strlen(elfFileName) < PATH_MAX));
}

LITE_OS_SEC_TEXT_MINOR BOOL LD_IsFilePath(const CHAR *path)
{
#if defined(LOSCFG_DYNLOAD_DYN_FROM_FS) || defined(LOSCFG_DYNLOAD_REL_FROM_FS)
    return (access(path, F_OK) == 0);
#else
    (VOID)path;
    return TRUE;
#endif
}

LITE_OS_SEC_TEXT_MINOR CHAR *LD_FilePathGet(const CHAR *path)
{
    INT32 idx;
    size_t maxLen;
    CHAR *pathRet = NULL;
    size_t pathLen;
    size_t defaultPathLen;

    if (!LD_IsFileNameLegal(path)) {
        return NULL;
    }

    pathLen = strlen(path);

    if (LD_IsFilePath(path)) {
        pathRet = (CHAR *)LOS_MemAlloc(m_aucSysMem1, (UINT32)(pathLen + 1) * sizeof(CHAR));
        if (pathRet == NULL) {
            return NULL;
        }
        (VOID)strcpy(pathRet, path);
        pathRet[pathLen] = '\0';
        return pathRet;
    }

    for (idx = 0; idx < N_DEFAULT_PATH; ++idx) {
        if (g_apscDefaultPath[idx] == NULL) {
            return NULL;
        }
        defaultPathLen = strlen(g_apscDefaultPath[idx]);
        if ((defaultPathLen == 1) && (g_apscDefaultPath[idx][0] == '/')) {
            defaultPathLen = 0;
        }
        maxLen = defaultPathLen + 1 + pathLen + 1;
        pathRet = (CHAR *)LOS_MemAlloc(m_aucSysMem1, (UINT32)maxLen * sizeof(CHAR));
        if (pathRet == NULL) {
            return NULL;
        }
        /* CSEC 6.6(4) copy 3 continuous memory, total size matches the src memory length */
        if (strncpy_s(pathRet, maxLen, g_apscDefaultPath[idx], defaultPathLen) != EOK) {
            PRINT_ERR("%s copy failed!\n", __FUNCTION__);
            goto COPY_ERR;
        }
        pathRet[defaultPathLen] = '/';
        if (strncpy_s(pathRet + defaultPathLen + 1, (maxLen - defaultPathLen) - 1, path, pathLen) != EOK) {
            PRINT_ERR("%s copy failed!\n", __FUNCTION__);
            goto COPY_ERR;
        }
        if (LD_IsFileNameLegal(pathRet) && LD_IsFilePath(pathRet)) {
            return pathRet;
        }
        (VOID)LOS_MemFree(m_aucSysMem1, pathRet);
    }

    return NULL;

COPY_ERR:
    (VOID)LOS_MemFree(m_aucSysMem1, pathRet);
    return NULL;
}

LITE_OS_SEC_TEXT_MINOR VOID OsPathDelete(VOID)
{
    INT32 idx;
    for (idx = 0; idx < N_DEFAULT_PATH; ++idx) {
        if (g_apscDefaultPath[idx] != NULL) {
            (VOID)LOS_MemFree(m_aucSysMem1, g_apscDefaultPath[idx]);
            g_apscDefaultPath[idx] = NULL;
        }
    }
}

LITE_OS_SEC_TEXT_MINOR INT32 LOS_PathAdd(CHAR *path)
{
    INT32 idx;
    size_t pathLen;
    CHAR *addPath = NULL;

    if (path == NULL) {
        return LOS_NOK;
    }

    if (LD_LoadLock() != LOS_OK) {
        return LOS_NOK;
    }

    pathLen = strlen(path);
    while ((pathLen > 1) && (path[pathLen - 1] == '/')) {
        --pathLen;
    }
    if ((pathLen == 0) || (pathLen >= PATH_MAX)) {
        goto OUT;
    }

    addPath = (CHAR *)LOS_MemAlloc(m_aucSysMem1, (UINT32)(pathLen + 1));
    if (addPath == NULL) {
        goto OUT;
    }
    if (strncpy_s(addPath, pathLen + 1, path, pathLen) != EOK) {
        PRINT_ERR("%s copy failed!\n", __FUNCTION__);
        goto LOS_ERREND;
    }

    for (idx = 0; idx < N_DEFAULT_PATH; ++idx) {
        if (g_apscDefaultPath[idx] == NULL) {
            g_apscDefaultPath[idx] = addPath;
            LD_LoadUnlock();
            return LOS_OK;
        }

        /* check if multi-add the same path. */
        if (strcmp(g_apscDefaultPath[idx], addPath) == 0) {
            goto LOS_ERREND;
        }
    }

LOS_ERREND:
    (VOID)LOS_MemFree(m_aucSysMem1, addPath);
OUT:
    LD_LoadUnlock();
    return LOS_NOK;
}

BOOL LOS_DynMemPoolSet(VOID *memPool)
{
    if ((memPool == NULL) ||
        (((UINTPTR)memPool >= (UINTPTR)OS_SYS_MEM_ADDR) && ((UINTPTR)memPool <= g_sys_mem_addr_end))) {
        PRINT_ERR("Invalid input parameter\n");
        return FALSE;
    }

    if (LD_LoadLock() != LOS_OK) {
        return FALSE;
    }

    if (LD_IsLoaderInit() == TRUE) {
        PRINT_ERR("Dynamically loaded memory pool has been set, "
                   "If want to set, please call the function LOS_LdDestroy first\n");
        LD_LoadUnlock();
        return FALSE;
    }
    g_dynloadHeap = memPool;
    LD_LoadUnlock();
    return TRUE;
}

LITE_OS_SEC_TEXT_MINOR BOOL LD_DlLoadQueueEmpty(VOID)
{
    return LOS_ListEmpty(&g_dlLoadQueue);
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlLoadEnqueue(DynModuleInfo * const moduleHandle)
{
    DlInfo *dlInfo = (DlInfo *)LOS_MemAlloc(g_dynloadHeap, sizeof(DlInfo));
    if (dlInfo == NULL) {
        return LOS_NOK;
    }
    (VOID)memset(dlInfo, 0, sizeof(DlInfo));
    dlInfo->moduleHandle = moduleHandle;
    LOS_ListTailInsert(&g_dlLoadQueue, &dlInfo->dlLinkNode);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR DlInfo *LD_DlLoadQueueTop(VOID)
{
    return LOS_DL_LIST_ENTRY(LOS_DL_LIST_FIRST(&g_dlLoadQueue), DlInfo, dlLinkNode);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlLoadQueueLock(VOID)
{
    UINT32 ret = LOS_MuxPend(g_dlLoadQueueLocker, LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
        PRINT_ERR("%s failed, err: 0x%x\n", __FUNCTION__, ret);
    }
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlLoadQueueUnlock(VOID)
{
    UINT32 ret = LOS_MuxPost(g_dlLoadQueueLocker);
    if (ret != LOS_OK) {
        PRINT_ERR("%s failed, err: 0x%x\n", __FUNCTION__, ret);
    }
}

LITE_OS_SEC_TEXT_MINOR BOOL LD_DlRelocQueueEmpty(VOID)
{
    return LOS_ListEmpty(&g_dlRelocQueue);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlRelocEnqueue(DlInfo *dlInfo)
{
    LOS_ListTailInsert(&g_dlRelocQueue, &dlInfo->dlLinkNode);
}

LITE_OS_SEC_TEXT_MINOR DlInfo *LD_DlRelocQueueTop(VOID)
{
    return LOS_DL_LIST_ENTRY(LOS_DL_LIST_FIRST(&g_dlRelocQueue), DlInfo, dlLinkNode);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlRelocQueueLock(VOID)
{
    UINT32 ret = LOS_MuxPend(g_dlRelocQueueLocker, LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
        PRINT_ERR("%s failed, err: 0x%x\n", __FUNCTION__, ret);
    }
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlRelocQueueUnlock(VOID)
{
    UINT32 ret = LOS_MuxPost(g_dlRelocQueueLocker);
    if (ret != LOS_OK) {
        PRINT_ERR("%s failed, err: 0x%x\n", __FUNCTION__, ret);
    }
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlLoadDequeue(VOID)
{
    DlInfo *dlInfo = LOS_DL_LIST_ENTRY(LOS_DL_LIST_FIRST(&g_dlLoadQueue), DlInfo, dlLinkNode);
    LOS_ListDelete(&dlInfo->dlLinkNode);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlRelocDequeue(VOID)
{
    DlInfo *dlInfo = LOS_DL_LIST_ENTRY(LOS_DL_LIST_FIRST(&g_dlRelocQueue), DlInfo, dlLinkNode);
    LOS_ListDelete(&dlInfo->dlLinkNode);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlFreeDlInfo(DlInfo *dlInfo)
{
    if (dlInfo->elfHdr != NULL) {
        (VOID)LOS_MemFree(g_dynloadHeap, (VOID *)dlInfo->elfHdr);
        dlInfo->elfHdr = NULL;
    }
    (VOID)LOS_MemFree(g_dynloadHeap, (VOID *)dlInfo);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_FreeDlInfoAll(VOID)
{
    DlInfo *dlInfo = NULL;

    while (!LD_DlLoadQueueEmpty()) {
        dlInfo = LD_DlLoadQueueTop();
        LD_DlLoadDequeue();
        (VOID)LD_ModuleUnload(dlInfo->moduleHandle);
        LD_DlFreeDlInfo(dlInfo);
    }

    while (!LD_DlRelocQueueEmpty()) {
        dlInfo = LD_DlRelocQueueTop();
        LD_DlRelocDequeue();
        (VOID)LD_ModuleUnload(dlInfo->moduleHandle);
        LD_DlFreeDlInfo(dlInfo);
    }
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlLoadAll(const SrcInfo *srcInfo, DynModuleInfo * const moduleHandle,
                                          const LoadFunc loadFunc, BOOL useHash)
{
    DlInfo *dlInfo = NULL;

    LD_DlLoadQueueLock();
    /* Breadth first search, load all depended dynamic library */
    if (LD_DlLoadEnqueue(moduleHandle) != LOS_OK) {
        (VOID)LD_ModuleMemRelease(moduleHandle);
        LD_DlLoadQueueUnlock();
        return LOS_NOK;
    }

    while (!LD_DlLoadQueueEmpty()) {
        dlInfo = LD_DlLoadQueueTop();
        if ((loadFunc == NULL) || (loadFunc(srcInfo, dlInfo, useHash) != LOS_OK)) {
            LD_FreeDlInfoAll();
            LD_DlLoadQueueUnlock();
            return LOS_NOK;
        }

        LD_DlLoadDequeue();
        LD_DlRelocEnqueue(dlInfo);
    }

    LD_DlLoadQueueUnlock();
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlRelocAll(const RelocFunc relocFunc)
{
    DlInfo *dlInfo = NULL;

    LD_DlRelocQueueLock();
    while (!LD_DlRelocQueueEmpty()) {
        dlInfo = LD_DlRelocQueueTop();
        if ((relocFunc == NULL) || (relocFunc(dlInfo) != LOS_OK)) {
            LD_FreeDlInfoAll();
            LD_DlRelocQueueUnlock();
            return LOS_NOK;
        }

        SET_LOADED(dlInfo->moduleHandle);

        LD_DlRelocDequeue();

        LD_DlFreeDlInfo(dlInfo);
    }

    LD_DlRelocQueueUnlock();
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlDoRead(const SrcInfo *srcInfo, DestInfo *destInfo)
{
    ssize_t retSize;
    errno_t err;

    if (destInfo->needSize > destInfo->bufSize) {
        return LOS_NOK;
    }

    if (srcInfo->fd >= 0) {
#if defined(LOSCFG_DYNLOAD_DYN_FROM_FS) || defined(LOSCFG_DYNLOAD_REL_FROM_FS)
        if (lseek(srcInfo->fd, srcInfo->offset, SEEK_SET) == -1) {
            return LOS_NOK;
        }

        retSize = read(srcInfo->fd, destInfo->buf, destInfo->needSize);
        if ((retSize < 0) || ((size_t)retSize != destInfo->needSize)) {
            return LOS_NOK;
        }
#else
        (void)retSize;
#endif
    } else {
        if ((srcInfo->fileSize < ((UINT64)srcInfo->offset + destInfo->needSize)) ||
            (srcInfo->fileBuf == NULL)) {
            return LOS_NOK;
        }
        err = memcpy_s(destInfo->buf, destInfo->bufSize, (srcInfo->fileBuf + srcInfo->offset), destInfo->needSize);
        if (err != EOK) {
            PRINT_ERR("%s,%d,err: %d\n", __FUNCTION__, __LINE__, err);
            return LOS_NOK;
        }
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LD_ElfHdrVerify(const LD_ELF_EHDR *ehdr)
{
    /* Check that the module begins with "\0x7fELF" */
    if (memcmp(ehdr->elfIdent, LD_ELFMAG, LD_SELFMAG) != 0) {
        return LOS_NOK;
    }

    /*
     * Try to give a clear diagnostic for ELF class mismatches, since they're
     * an easy mistake to make during the 32-bit/64-bit transition period.
     */
    if (ehdr->elfIdent[LD_EI_CLASS] != LD_ELF_CLASS) {
        return LOS_NOK;
    }

    /* Check the data format is little endian */
    if (ehdr->elfIdent[LD_EI_DATA] != LD_ELF_DATA2LSB) {
        return LOS_NOK;
    }

    if (ehdr->elfVersion != LD_EV_CURRENT) {
        return LOS_NOK;
    }

    /* Check that the object module is appropriate for target machine identification */
    if (ehdr->elfMachine != LD_EM_TYPE) {
        return LOS_NOK;
    }

    /* Check the size of ELF header */
    if (ehdr->elfHeadSize != sizeof(LD_ELF_EHDR)) {
        return LOS_NOK;
    }

    return LOS_OK;
}
