/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Ld DlLib
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

#include "los_ld_dllib_pri.h"
#include "los_ld_modulelib_pri.h"
#include "los_ld_commlib_pri.h"
#include "los_ld_elflib_pri.h"
#include "los_ld_macro_pri.h"
#include "los_ld_reloc_pri.h"
#include "los_ld_symlib_pri.h"
#include "los_ld_elflib.h"

#include "stdio.h"
#include "limits.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "sys/types.h"

#include "los_mux.h"
#include "los_task.h"
#include "los_builddef.h"
#include "los_hwi.h"

#if defined(LOSCFG_ARCH_CORTEX_A7) || defined(LOSCFG_ARCH_CORTEX_A53_AARCH32)
#include "elf.h"
#include "arch/mmu.h"
#endif

#ifdef LOSCFG_KERNEL_DYNLOAD_DYN
#ifdef LOSCFG_DYNLOAD_DYN_FROM_FS
DYNLOAD_PARAM_S g_dynloadParam = { NOZIP };
LITE_OS_SEC_TEXT_MINOR VOID LOS_DynParamReg(const DYNLOAD_PARAM_S *dynloadParam)
{
    if (dynloadParam == NULL) {
        PRINT_ERR("the set param can not be NULL!\n");
        return;
    }

    if ((dynloadParam->enLoadStrategy != ZIP) && (dynloadParam->enLoadStrategy != NOZIP)) {
        PRINT_ERR("Invalid load strategy!\n");
        return;
    }

    if (LD_LoadLock() != LOS_OK) {
        return;
    }

    g_dynloadParam.enLoadStrategy = dynloadParam->enLoadStrategy;
    LD_LoadUnlock();
}
#endif

#if defined(LOSCFG_DYNLOAD_DYN_FROM_FS) || defined(LOSCFG_DYNLOAD_DYN_FROM_MEM)
LITE_OS_SEC_TEXT_MINOR VOID LD_DlRefCntInc(DynModuleInfo *moduleHandle)
{
    UINT32 idx;

    if (IS_LOADDING(moduleHandle) || !IS_LOADED(moduleHandle)) {
        return;
    }

    ++moduleHandle->refCnt;
    SET_LOADDING(moduleHandle);

    for (idx = 0; idx < N_MAX_MODULES; ++idx) {
        if (moduleHandle->apDlNeeded[idx] == NULL) {
            break;
        }
        LD_DlRefCntInc(moduleHandle->apDlNeeded[idx]);
    }
}

LITE_OS_SEC_TEXT_MINOR DynModuleInfo *LD_DlHandleAlloc(const CHAR *elfFileName)
{
    DynModuleInfo *moduleHandle = LD_ModuleHandleAlloc(elfFileName);
    if (moduleHandle == NULL) {
        return NULL;
    }

    moduleHandle->type = ET_SO;
    SET_LOADDING(moduleHandle);

    return moduleHandle;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlAdd2NeededDlList(DynModuleInfo *downstreamModuleHandle,
                                                   DynModuleInfo * const moduleHandle)
{
    UINT32 idx;

    for (idx = 0; idx < N_MAX_MODULES; ++idx) {
        if (downstreamModuleHandle->apDlNeeded[idx] == NULL) {
            downstreamModuleHandle->apDlNeeded[idx] = moduleHandle;
            return LOS_OK;
        }
    }
    return LOS_NOK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlNeededExtract(const DlInfo *dlInfo)
{
    CHAR *realPath = NULL;
    CHAR *dlNeededName = NULL;
    LD_ELF_DYN *dyn = NULL;
    DynModuleInfo *moduleHandle = NULL;

    for (dyn = dlInfo->dynamic; dyn->dynTag != LD_DT_NULL; ++dyn) {
        if (dyn->dynTag != LD_DT_NEEDED) {
            continue;
        }

        dlNeededName = dlInfo->strTab + dyn->dyn.ptr;
        realPath = LD_FilePathGet(dlNeededName);
        if (realPath == NULL) {
            PRINT_ERR("[%s] no such file: %s\n", __FUNCTION__, dlNeededName);
            return LOS_NOK;
        }

        /* Test if the module is already loaded */
        if (LD_IsModuleTaken(realPath, &moduleHandle) == TRUE) {
            (VOID)LOS_MemFree(m_aucSysMem1, realPath);

            if (LD_DlAdd2NeededDlList(dlInfo->moduleHandle, moduleHandle) != LOS_OK) {
                return LOS_NOK;
            }
            LD_DlRefCntInc(moduleHandle);
            continue;
        }

        moduleHandle = LD_DlHandleAlloc(realPath);
        if (moduleHandle == NULL) {
            (VOID)LOS_MemFree(m_aucSysMem1, realPath);
            return LOS_NOK;
        }
        (VOID)LOS_MemFree(m_aucSysMem1, realPath);

        if (LD_DlAdd2NeededDlList(dlInfo->moduleHandle, moduleHandle) != LOS_OK) {
            return LOS_NOK;
        }

        if (LD_DlLoadEnqueue(moduleHandle) != LOS_OK) {
            return LOS_NOK;
        }
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINTPTR LD_DlPhdrTableLoadSizeGet(const LD_ELF_PHDR *phdrTbl, DlInfo *dlInfo)
{
    UINT32 idx;
    UINTPTR minVaddr = UINTMAX;
    UINTPTR maxVaddr = 0;
    BOOL foundPtLoad = FALSE;
    const LD_ELF_PHDR *phdr = NULL;

    for (idx = 0; idx < dlInfo->phNum; ++idx) {
        phdr = &phdrTbl[idx];

        if (phdr->type != LD_PT_LOAD) {
            continue;
        }
        foundPtLoad = TRUE;

        if (phdr->vAddr < minVaddr) {
            minVaddr = phdr->vAddr;
        }

        if ((phdr->vAddr + phdr->memSize) > maxVaddr) {
            maxVaddr = phdr->vAddr + phdr->memSize;
        }
    }

    if (!foundPtLoad) {
        minVaddr = 0;
    }

    dlInfo->minVaddr = minVaddr;

    /* the loadsize is equal to maxVaddr - minVaddr */
    return maxVaddr;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_SoHdrVerify(const LD_ELF_EHDR *ehdr)
{
    if (LD_ElfHdrVerify(ehdr) != LOS_OK) {
        return LOS_NOK;
    }

    if (ehdr->elfType != LD_ET_DYN) {
        return LOS_NOK;
    }

    /* Check the offset of section header table */
    if (ehdr->elfPhNum == 0) {
        return LOS_NOK;
    }

    /* Check the size of segment */
    if (ehdr->elfPhSize != sizeof(LD_ELF_PHDR)) {
        return LOS_NOK;
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlReadElfHeader(INT32 fd, LD_ELF_EHDR *hdr)
{
    SrcInfo srcInfo = { fd, NULL, 0, 0 };
    DestInfo destInfo = { (CHAR *)hdr, sizeof(LD_ELF_EHDR), sizeof(LD_ELF_EHDR) };
    return LD_DlDoRead(&srcInfo, &destInfo);
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlReadProgramHeader(INT32 fd, LD_ELF_PHDR *phdrTbl, UINTPTR phdrOff, UINT16 phdrNum)
{
    SrcInfo srcInfo = { fd, NULL, 0, phdrOff };
    UINT32 bufSize = (UINT32)phdrNum * sizeof(LD_ELF_PHDR);
    DestInfo destInfo = { (CHAR *)phdrTbl, bufSize, bufSize };
    return LD_DlDoRead(&srcInfo, &destInfo);
}

/* Reserve a virtual address range big enough to hold all loadable segments of a program header table. */
LITE_OS_SEC_TEXT_MINOR INT32 LD_DlReserveAddressSpace(const LD_ELF_PHDR *phdrTbl, DlInfo *dlInfo)
{
    VOID *moduleAddr = NULL;
    UINTPTR loadSize = LD_DlPhdrTableLoadSizeGet(phdrTbl, dlInfo);
    if (loadSize == 0) {
        return LOS_NOK;
    }

    loadSize = LOS_Align(loadSize, DYNLOAD_ALIGN_SIZE);

    moduleAddr = LOS_MemAllocAlign(g_dynloadHeap, (UINT32)loadSize, DYNLOAD_ALIGN_SIZE);
    if (moduleAddr == NULL) {
        return LOS_NOK;
    }

    dlInfo->loadStart = LD_SeekAddr((UINTPTR)moduleAddr, dlInfo->minVaddr);
    dlInfo->loadBias = dlInfo->loadStart - dlInfo->minVaddr;
    dlInfo->moduleHandle->segmentsAddr = (UINTPTR)moduleAddr;
    dlInfo->moduleHandle->segmentsSize = loadSize;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT INT32 LD_DlLoadSegments(const LD_ELF_PHDR *phdrTbl, const DlInfo *dlInfo,
                                              SrcInfo *srcInfo)
{
    UINT32 idx;
    UINTPTR memSize;
    VOID *vaddr = NULL;
    const LD_ELF_PHDR *phdr = NULL;
    errno_t err;
    DestInfo destInfo;

    for (idx = 0; idx < dlInfo->phNum; ++idx) {
        phdr = &phdrTbl[idx];

        if ((phdr->type != LD_PT_LOAD) || (phdr->memSize == 0)) {
            continue;
        }

        vaddr = (VOID *)(phdr->vAddr + dlInfo->loadBias);
        srcInfo->offset = (off_t)phdr->offset;
        memSize = phdr->memSize;
        destInfo.buf = vaddr;
        destInfo.bufSize = (UINT32)(dlInfo->moduleHandle->segmentsSize - phdr->vAddr);
        destInfo.needSize = (UINT32)phdr->fileSize;

        if (LD_DlDoRead(srcInfo, &destInfo) != LOS_OK) {
            return LOS_NOK;
        }

        /* BSS */
        if (phdr->memSize > phdr->fileSize) {
            err = memset_s((VOID *)((UINTPTR)vaddr + phdr->fileSize), dlInfo->moduleHandle->segmentsSize -
                           phdr->vAddr - phdr->fileSize, 0, memSize - phdr->fileSize);
            if (err != EOK) {
                return LOS_NOK;
            }
        }
#if defined(LOSCFG_ARCH_CORTEX_A7) || defined(LOSCFG_ARCH_CORTEX_A53_AARCH32)
        if (LOS_GET_NX_CFG() && CheckDynNxHeap()) {
            DYN_NX_PROTECT(vaddr, memSize, phdr->flags);
        }
#endif
    }

    LD_FlushDCache(dlInfo->moduleHandle->segmentsAddr,
                   (dlInfo->moduleHandle->segmentsAddr + dlInfo->moduleHandle->segmentsSize));
    return LOS_OK;
}

/*
 * Ensures that our program header is actually within a loadable
 * segment. This should help catch badly-formed ELF files that
 * would cause the linker to crash later when trying to access it.
 */
LITE_OS_SEC_TEXT_MINOR INT32 LD_DlCheckPhdr(UINTPTR loaded, const LD_ELF_PHDR *phdrTbl, DlInfo *dlInfo)
{
    UINTPTR segStart;
    UINTPTR segEnd;
    UINTPTR loadedEnd = loaded + dlInfo->phNum * sizeof(LD_ELF_PHDR);
    const LD_ELF_PHDR *phdrLimit = phdrTbl + dlInfo->phNum;
    const LD_ELF_PHDR *curPhdr = NULL;

    /* If there is a PT_PHDR, use it directly. */
    for (curPhdr = phdrTbl; curPhdr < phdrLimit; ++curPhdr) {
        if (curPhdr->type != LD_PT_LOAD) {
            continue;
        }

        segStart = curPhdr->vAddr + dlInfo->loadBias;
        segEnd = curPhdr->fileSize + segStart;
        if ((segStart <= loaded) && (loadedEnd <= segEnd)) {
            dlInfo->phdrTbl = (LD_ELF_PHDR *)loaded;
            return LOS_OK;
        }
    }

    return LOS_NOK;
}

/*
 * Returns the address of the program header table as it appears in the loaded
 * segments in memory. This is in contrast with 'phdrTbl' which
 * is temporary and will be released before the library is relocated.
 */
LITE_OS_SEC_TEXT_MINOR INT32 LD_DlFindPhdr(const LD_ELF_PHDR *phdrTbl, DlInfo *dlInfo)
{
    UINTPTR elfAddr;
    UINTPTR offset;
    LD_ELF_EHDR *ehdr = NULL;
    const LD_ELF_PHDR *phdrLimit = phdrTbl + dlInfo->phNum;
    const LD_ELF_PHDR *curPhdr = NULL;

    /* If there is a PT_PHDR, use it directly. */
    for (curPhdr = phdrTbl; curPhdr < phdrLimit; ++curPhdr) {
        if (curPhdr->type == LD_PT_PHDR) {
            return LD_DlCheckPhdr(dlInfo->loadBias + curPhdr->vAddr, phdrTbl, dlInfo);
        }
    }

    /*
     * Otherwise, check the first loadable segment. If its file offset
     * is 0, it starts with the ELF header, and we can trivially find the
     * loaded program header from it.
     */
    for (curPhdr = phdrTbl; curPhdr < phdrLimit; ++curPhdr) {
        if (curPhdr->type == LD_PT_LOAD) {
            if (curPhdr->offset == 0) {
                elfAddr = dlInfo->loadBias + curPhdr->vAddr;
                ehdr = (LD_ELF_EHDR *)elfAddr;
                offset = ehdr->elfPhoff;
                return LD_DlCheckPhdr((UINTPTR)ehdr + offset, phdrTbl, dlInfo);
            }
            break;
        }
    }

    return LOS_NOK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlSoFileOpen(const CHAR *elfFileName)
{
    return open(elfFileName, O_RDONLY);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlSoFileClose(INT32 fd)
{
    (VOID)close(fd);
}

LITE_OS_SEC_TEXT_MINOR VOID LD_DlPhdrTableGetDynamicSection(DlInfo *dlInfo, UINT32 *dynamicFlags)
{
    LD_ELF_PHDR *phdrLimit = dlInfo->phdrTbl + dlInfo->phNum;
    LD_ELF_PHDR *curPhdr = NULL;

    /* If there is a PT_PHDR, use it directly. */
    for (curPhdr = dlInfo->phdrTbl; curPhdr < phdrLimit; ++curPhdr) {
        if (curPhdr->type == LD_PT_DYNAMIC) {
            dlInfo->dynamic = (LD_ELF_DYN *)(dlInfo->loadBias + curPhdr->vAddr);
            if (dynamicFlags != NULL) {
                *dynamicFlags = curPhdr->flags;
            }
            return;
        }
    }
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_DlReadDynamicCheckSymEnt(const LD_ELF_DYN *dyn)
{
    if (dyn->dyn.val != sizeof(LD_ELF_SYM)) {
        PRINT_ERR("invalid DT_SYMENT");
        return LOS_NOK;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_DlReadDynamicSetRelEnt(DlInfo *dlInfo, const LD_ELF_DYN *dyn)
{
    if (dyn->dyn.val != sizeof(LD_ELF_REL)) {
        PRINT_ERR("invalid DT_RELENT");
        return LOS_NOK;
    }
    dlInfo->relocation.relocations.rel.relEntrySize = dyn->dyn.val;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_DlReadDynamicSetRelaEnt(DlInfo *dlInfo, const LD_ELF_DYN *dyn)
{
    if (dyn->dyn.val != sizeof(LD_ELF_RELA)) {
        PRINT_ERR("invalid DT_RELAENT");
        return LOS_NOK;
    }
    dlInfo->relocation.relocations.rela.relEntrySize = dyn->dyn.val;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_DlReadDynamicCheckPltRel(DlInfo *dlInfo, const LD_ELF_DYN *dyn)
{
    if (dyn->dyn.val == LD_DT_REL) {
        dlInfo->relocation.relocations.jmpRel.relEntrySize = sizeof(LD_ELF_REL);
    } else if (dyn->dyn.val == LD_DT_RELA) {
        dlInfo->relocation.relocations.jmpRel.relEntrySize = sizeof(LD_ELF_RELA);
    } else {
        PRINT_ERR("invalid DT_PLTREL, dyn->val : %lu\n", dyn->dyn.val);
        return LOS_NOK;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC INLINE VOID LD_DlSetHashInfo(DlInfo *dlInfo, const LD_ELF_DYN *dyn)
{
    dlInfo->moduleHandle->hashInfo.bucketSize = ((UINT32 *)(dlInfo->loadBias + dyn->dyn.ptr))[0];
    dlInfo->moduleHandle->hashInfo.chainSize  = ((UINT32 *)(dlInfo->loadBias + dyn->dyn.ptr))[1];
    /* 2: the number of "bucketSize" and "chainSize" */
    dlInfo->moduleHandle->hashInfo.bucket = (UINT32 *)(dlInfo->loadBias + dyn->dyn.ptr + 2 * sizeof(UINT32));
    dlInfo->moduleHandle->hashInfo.chain  = (UINT32 *)((UINTPTR)dlInfo->moduleHandle->hashInfo.bucket +
                                                       dlInfo->moduleHandle->hashInfo.bucketSize * sizeof(UINT32));
}

LITE_OS_SEC_TEXT_MINOR STATIC INLINE VOID LD_DlSetSymNum(DlInfo *dlInfo, const LD_ELF_DYN *dyn)
{
    dlInfo->symNum = ((UINT32 *)(dlInfo->loadBias + dyn->dyn.ptr))[1];
}

LITE_OS_SEC_TEXT_MINOR STATIC VOID LD_DlSetSymInfo(DlInfo *dlInfo, const LD_ELF_DYN *dyn, BOOL hash)
{
    if (hash) {
        LD_DlSetHashInfo(dlInfo, dyn);
    } else {
        LD_DlSetSymNum(dlInfo, dyn);
    }
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlReadDynamic(DlInfo *dlInfo, BOOL hash)
{
    LD_ELF_DYN *dyn = NULL;
    INT32 ret = LOS_OK;

    for (dyn = dlInfo->dynamic; (dyn->dynTag != LD_DT_NULL) && (ret == LOS_OK); ++dyn) {
        switch (dyn->dynTag) {
            case LD_DT_HASH:
                LD_DlSetSymInfo(dlInfo, dyn, hash);
                break;
            case LD_DT_STRTAB:
                dlInfo->strTab = (CHAR *)(dlInfo->loadBias + dyn->dyn.ptr);
                break;
            case LD_DT_STRSZ:
                dlInfo->strTabSize = dyn->dyn.val;
                break;
            case LD_DT_SYMTAB:
                dlInfo->symTab = (LD_ELF_SYM *)(dlInfo->loadBias + dyn->dyn.ptr);
                break;
            case LD_DT_SYMENT:
                ret = LD_DlReadDynamicCheckSymEnt(dyn);
                break;
            case LD_DT_REL:
                dlInfo->relocation.relocations.rel.relTable = dlInfo->loadBias + dyn->dyn.ptr;
                break;
            case LD_DT_RELSZ:
                dlInfo->relocation.relocations.rel.relTableSize = dyn->dyn.val;
                break;
            case LD_DT_RELENT:
                ret = LD_DlReadDynamicSetRelEnt(dlInfo, dyn);
                break;
            case LD_DT_RELA:
                dlInfo->relocation.relocations.rela.relTable = dlInfo->loadBias + dyn->dyn.ptr;
                break;
            case LD_DT_RELASZ:
                dlInfo->relocation.relocations.rela.relTableSize = dyn->dyn.val;
                break;
            case LD_DT_RELAENT:
                ret = LD_DlReadDynamicSetRelaEnt(dlInfo, dyn);
                break;
            case LD_DT_JMPREL:
                dlInfo->relocation.relocations.jmpRel.relTable = dlInfo->loadBias + dyn->dyn.ptr;
                break;
            case LD_DT_PLTRELSZ:
                dlInfo->relocation.relocations.jmpRel.relTableSize = dyn->dyn.val;
                break;
            case LD_DT_PLTREL:
                ret = LD_DlReadDynamicCheckPltRel(dlInfo, dyn);
                break;
            default:
                break;
        }
    }

    return ret;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlSymTableBuild(const DlInfo *dlInfo)
{
    UINT32 symIndex; /* symbol index */
    CHAR *name = NULL; /* symbol name (plus EOS) */
    LD_ELF_SYM *symbol = NULL; /* current symbol being processed */
    VOID *addr = NULL; /* table resident symbol address */
    INT8 symType = LD_STB_GLOBAL; /* internal value for symbol type */

    if (!LD_ModuleSymTblCreate(dlInfo->moduleHandle,
                               dlInfo->symTab,
                               dlInfo->strTab,
                               dlInfo->symNum)) {
        return LOS_NOK;
    }

    /* Loop thru all symbol table entries in object file. */
    for (symIndex = 0; symIndex < dlInfo->symNum; symIndex++) {
        symbol = dlInfo->symTab + symIndex;
        name = dlInfo->strTab + symbol->stName;

        /* Symbol is neither an undefined external, nor a common symbol */
        if ((symbol->stShndx == LD_SHN_UNDEF) || (symbol->stShndx == LD_SHN_COMMON)) {
            continue;
        }
        addr = (VOID *)(symbol->stValue + dlInfo->loadBias);

        /* Determine if symbol should be put into symbol table. */
        if (LD_ElfSymIsVisible(symbol)) {
            symType = (INT8)((UINT8)symType | LD_SYM_GLOBAL);
            /* Add symbol to target's symbol table. */
            if (LD_SymAdd(dlInfo->moduleHandle, name, (INT8 *)addr, symType) != LOS_OK) {
                PRINT_ERR("Can't add '%s' to symbol table\n", name);
                return LOS_NOK;
            }
        }
    }

    if (LD_ModuleSymTblSort(dlInfo->moduleHandle) != LOS_OK) {
        PRINT_ERR("sort symbol tbl error!\n");
        return LOS_NOK;
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlPreLinkImage(DlInfo *dlInfo, BOOL useHash)
{
    LD_DlPhdrTableGetDynamicSection(dlInfo, NULL);

    if (LD_DlReadDynamic(dlInfo, useHash) != LOS_OK) {
        return LOS_NOK;
    }

    if (useHash == TRUE) {
        dlInfo->moduleHandle->strTab = dlInfo->strTab;
        dlInfo->moduleHandle->symTab = dlInfo->symTab;
        return LOS_OK;
    }

    if (LD_DlNeededExtract(dlInfo) != LOS_OK) {
        return LOS_NOK;
    }

    /* Build / update the target's symbol table with symbols found in module's symbol tables. */
    if (LD_DlSymTableBuild(dlInfo) != LOS_OK) {
        return LOS_NOK;
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_DlLoadImp(const LD_ELF_PHDR *phdrTbl, DlInfo *dlInfo,
                                                 SrcInfo *srcInfo, BOOL useHash)
{
    /* Reserve space for loadable segment */
    if (LD_DlReserveAddressSpace(phdrTbl, dlInfo) != LOS_OK) {
        return LOS_NOK;
    }

    /* Load loadable to memory */
    if (LD_DlLoadSegments(phdrTbl, dlInfo, srcInfo) != LOS_OK) {
        return LOS_NOK;
    }

    if (LD_DlFindPhdr(phdrTbl, dlInfo) != LOS_OK) {
        return LOS_NOK;
    }

    if (LD_DlPreLinkImage(dlInfo, useHash) != LOS_OK) {
        return LOS_NOK;
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlLoad(const SrcInfo *srcInfo, DlInfo *dlInfo, BOOL useHash)
{
    INT32 fd;
    LD_ELF_EHDR ehdr = {{0}};
    DynModuleInfo *moduleHandle = dlInfo->moduleHandle;
    LD_ELF_PHDR *phdrTbl = NULL;
    SrcInfo newSrcInfo = { -1, NULL, 0, 0 };

    (VOID)srcInfo;
    ++moduleHandle->refCnt;

    fd = LD_DlSoFileOpen(moduleHandle->moduleFileName);
    if (fd < 0) {
        goto LOS_ERREND;
    }

    /* Read & check the elf header */
    if (LD_DlReadElfHeader(fd, &ehdr) != LOS_OK) {
        goto LOS_ERREND_FILEREAD;
    }

    if (LD_SoHdrVerify(&ehdr) != LOS_OK) {
        PRINT_ERR("So Elf head Error!\n");
        goto LOS_ERREND_FILEREAD;
    }

    dlInfo->phNum = ehdr.elfPhNum;

    phdrTbl = LOS_MemAlloc(g_dynloadHeap, ehdr.elfPhNum * sizeof(LD_ELF_PHDR));
    if (phdrTbl == NULL) {
        goto LOS_ERREND_FILEREAD;
    }
    /* Read program header */
    if (LD_DlReadProgramHeader(fd, phdrTbl, ehdr.elfPhoff, ehdr.elfPhNum) != LOS_OK) {
        goto LOS_ERREND_FILEREAD;
    }
    newSrcInfo.fd = fd;
    if (LD_DlLoadImp(phdrTbl, dlInfo, &newSrcInfo, useHash) != LOS_OK) {
        goto LOS_ERREND_FILEREAD;
    }

    LD_DlSoFileClose(fd);
    (VOID)LOS_MemFree(g_dynloadHeap, phdrTbl);

    return LOS_OK;

LOS_ERREND_FILEREAD:
    LD_DlSoFileClose(fd);
LOS_ERREND:
    if (phdrTbl != NULL) {
        (VOID)LOS_MemFree(g_dynloadHeap, phdrTbl);
    }
    (VOID)LD_ModuleUnload(moduleHandle);
    return LOS_NOK;
}

LITE_OS_SEC_TEXT_MINOR LD_ELF_EHDR *LD_ZipDlReadElfHeader(UINTPTR moduleAddr)
{
    return (LD_ELF_EHDR *)LD_SeekAddr(moduleAddr, 0);
}

LITE_OS_SEC_TEXT_MINOR LD_ELF_PHDR *LD_ZipDlReadProgramHeader(UINTPTR moduleAddr, UINTPTR phoff)
{
    return (LD_ELF_PHDR *)LD_SeekAddr(moduleAddr, phoff);
}

LITE_OS_SEC_TEXT_MINOR UINTPTR LD_ZipDlReadFile2Mem(const CHAR *elfFileName, UINTPTR buffAddr)
{
    SrcInfo srcInfo = { -1, NULL, 0, 0 };
    DestInfo destInfo = { (CHAR *)buffAddr, 0, 0 };
    struct stat statBuff = {0};

    if (stat(elfFileName, &statBuff) < 0) {
        return LOS_NOK;
    }
    if (statBuff.st_size > (INT_MAX - 1)) {
        return LOS_NOK;
    }
    destInfo.bufSize = (UINT32)statBuff.st_size;
    destInfo.needSize = (UINT32)statBuff.st_size;

    srcInfo.fd = open(elfFileName, O_RDONLY);
    if (srcInfo.fd < 0) {
        return LOS_NOK;
    }
    srcInfo.offset = 0;

    if (LD_DlDoRead(&srcInfo, &destInfo) != LOS_OK) {
        goto LOS_ERREND;
    }

    if (close(srcInfo.fd) < 0) {
        return LOS_NOK;
    }

    return LOS_OK;

LOS_ERREND:
    (VOID)close(srcInfo.fd);
    return LOS_NOK;
}

LITE_OS_SEC_TEXT_MINOR UINTPTR LD_ZipDlLoadSizeGet(const CHAR *elfFileName, DlInfo *dlInfo)
{
    UINTPTR moduleAddr = 0;
    UINTPTR loadSize = 0;
    LD_ELF_EHDR *hdr = NULL;
    UINT32 fileSize = 0;
    struct stat statBuff = {0};

    if (stat(elfFileName, &statBuff) < 0) {
        goto OUT;
    }
    if (statBuff.st_size > (INT_MAX - 1)) {
        goto OUT;
    }
    fileSize = (UINT32)statBuff.st_size;

    moduleAddr = (UINTPTR)LOS_MemAlloc(g_dynloadHeap, fileSize);
    if (moduleAddr == 0) {
        goto OUT;
    }

    if (LD_ZipDlReadFile2Mem(elfFileName, moduleAddr) != LOS_OK) {
        goto OUT;
    }

    /* Get ELF file's Header. */
    hdr = LD_ZipDlReadElfHeader(moduleAddr);
    if (LD_SoHdrVerify(hdr) != LOS_OK) {
        PRINT_ERR("So Elf head Error!\n");
        goto OUT;
    }

    /* Get program header. */
    dlInfo->phdrTbl = LD_ZipDlReadProgramHeader(moduleAddr, hdr->elfPhoff);

    /* Calculate the load size. */
    dlInfo->phNum = hdr->elfPhNum;
    loadSize = LD_DlPhdrTableLoadSizeGet(dlInfo->phdrTbl, dlInfo);

OUT:
    if (moduleAddr != 0) {
        (VOID)LOS_MemFree(g_dynloadHeap, (VOID *)moduleAddr);
    }

    /* if load size is not zero or less than filesize, read the zip file to memory directly. */
    loadSize = ((loadSize < fileSize) && (loadSize != 0)) ? fileSize : loadSize;

    /* return 0 means error or there's no loadable segments. */
    return loadSize;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LD_ZipDlLoadFile(const CHAR *elfFileName, DlInfo *dlInfo)
{
    UINTPTR moduleAddr = 0;
    UINTPTR loadSize;

    loadSize = LD_ZipDlLoadSizeGet(elfFileName, dlInfo);
    if (loadSize == 0) {
        goto OUT;
    }

    loadSize = LOS_Align(loadSize, DYNLOAD_ALIGN_SIZE);

    moduleAddr = (UINTPTR)LOS_MemAllocAlign(g_dynloadHeap, (UINT32)loadSize, DYNLOAD_ALIGN_SIZE);
    if (moduleAddr == 0) {
        goto OUT;
    }

    if (LD_ZipDlReadFile2Mem(elfFileName, moduleAddr) != LOS_OK) {
        goto OUT;
    }

    dlInfo->moduleHandle->segmentsAddr = moduleAddr;
    dlInfo->moduleHandle->segmentsSize = loadSize;

    return LOS_OK;

OUT:
    if (moduleAddr != 0) {
        (VOID)LOS_MemFree(g_dynloadHeap, (VOID *)moduleAddr);
    }

    return LOS_NOK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_ZipDlLoadFix(UINTPTR moduleAddr, DlInfo *dlInfo)
{
    INT32 idx;
    VOID *noAlignedAddr = NULL;
    VOID *alignedAddr = NULL;
    LD_ELF_PHDR *phdrTbl = dlInfo->phdrTbl;
    LD_ELF_PHDR *phdr = NULL;
    UINTPTR moveOffset;
    errno_t err;

    for (idx = dlInfo->phNum - 1; idx >= 0; --idx) {
        phdr = &phdrTbl[idx];
        if ((phdr->type != LD_PT_LOAD) || (phdr->memSize == 0)) {
            continue;
        }

        noAlignedAddr = (VOID *)LD_SeekAddr(moduleAddr, phdr->offset);
        alignedAddr = (VOID *)LD_SeekAddr(moduleAddr, phdr->vAddr);
        err = memmove_s(alignedAddr, (moduleAddr + dlInfo->moduleHandle->segmentsSize - (UINTPTR)alignedAddr),
                        noAlignedAddr, phdr->fileSize);
        if (err != EOK) {
            PRINT_ERR("%s,%d failed\n", __func__, __LINE__);
            return LOS_NOK;
        }

        /* the pointer to the program header need move together with the segment. */
        if (((UINTPTR)phdrTbl >= (UINTPTR)noAlignedAddr) &&
            ((UINTPTR)phdrTbl <= ((UINTPTR)noAlignedAddr + phdr->fileSize))) {
            moveOffset = (UINTPTR)alignedAddr - (UINTPTR)noAlignedAddr;
            phdr = (LD_ELF_PHDR *)((UINTPTR)phdr + moveOffset);
            phdrTbl = (LD_ELF_PHDR *)((UINTPTR)phdrTbl + moveOffset);
        }

        /* BSS */
        if (phdr->memSize > phdr->fileSize) {
            err = memset_s((VOID *)((UINTPTR)alignedAddr + phdr->fileSize), (moduleAddr +
                           dlInfo->moduleHandle->segmentsSize - ((UINTPTR)alignedAddr + phdr->fileSize)),
                           0, phdr->memSize - phdr->fileSize);
            if (err != EOK) {
                PRINT_ERR("%s,%d failed\n", __func__, __LINE__);
                return LOS_NOK;
            }
        }
    }

    /* point to the program head table, it's may be moved in a loadable segment. */
    dlInfo->phdrTbl = phdrTbl;
    dlInfo->loadStart = LD_SeekAddr(moduleAddr, dlInfo->minVaddr);
    dlInfo->loadBias = dlInfo->loadStart - dlInfo->minVaddr;

    LD_FlushDCache(dlInfo->moduleHandle->segmentsAddr,
                   dlInfo->moduleHandle->segmentsAddr + dlInfo->moduleHandle->segmentsSize);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_ZipDlLoad(const SrcInfo *srcInfo, DlInfo *dlInfo, BOOL useHash)
{
    LD_ELF_EHDR *ehdr = NULL;
    DynModuleInfo *moduleHandle = dlInfo->moduleHandle;

    (VOID)srcInfo;
    ++moduleHandle->refCnt;

    if (LD_ZipDlLoadFile(moduleHandle->moduleFileName, dlInfo) != LOS_OK) {
        goto LOS_ERREND;
    }

    /* Read & check the elf header */
    ehdr = LD_ZipDlReadElfHeader(moduleHandle->segmentsAddr);
    if (LD_SoHdrVerify(ehdr) != LOS_OK) {
        PRINT_ERR("So Elf head Error!\n");
        goto LOS_ERREND;
    }

    dlInfo->phNum = ehdr->elfPhNum;

    /* Read program header */
    dlInfo->phdrTbl = LD_ZipDlReadProgramHeader(moduleHandle->segmentsAddr, ehdr->elfPhoff);

    if (LD_ZipDlLoadFix(moduleHandle->segmentsAddr, dlInfo) != LOS_OK) {
        goto LOS_ERREND;
    }

    if (LD_DlFindPhdr(dlInfo->phdrTbl, dlInfo) != LOS_OK) {
        goto LOS_ERREND;
    }

    if (LD_DlPreLinkImage(dlInfo, useHash) != LOS_OK) {
        goto LOS_ERREND;
    }

    return LOS_OK;

LOS_ERREND:
    (VOID)LD_ModuleUnload(moduleHandle);
    return LOS_NOK;
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_DlSegRelocSetRelParam(const DlInfo *dlInfo, const LD_ELF_RELA *rela,
                                                             LDRelocParam *relocParam)
{
    if (relocParam->shType == LD_SHT_RELA) {
        relocParam->addend = rela->addend;
    }

    relocParam->position = rela->offset + dlInfo->loadBias;
    if (LD_SymLoadAddrGet(dlInfo, LD_ELF_R_SYM(rela->info), &relocParam->symAdd) != LOS_OK) {
        PRINT_ERR("cannot relocate symbol \"%s\"\n",
                  (dlInfo->strTab + dlInfo->symTab[LD_ELF_R_SYM(rela->info)].stName));
        return LOS_NOK;
    }
    relocParam->relocType = LD_ELF_R_TYPE(rela->info);
    relocParam->baseSegment = dlInfo->loadBias;
    return  LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC BOOL LD_SetModuleInfo(DynModuleInfo **moduleHandle, const CHAR *elfFileName)
{
    *moduleHandle = LD_DlHandleAlloc(elfFileName);
    if (*moduleHandle == NULL) {
        return FALSE;
    }
    (*moduleHandle)->type = ET_SO;

    INC_SOHEAD(*moduleHandle);
    return TRUE;
}

LITE_OS_SEC_TEXT_MINOR STATIC UINT32 LD_SoRelocCheckError(const RelocationInfo *relocInfo)
{
    if (relocInfo->relTableSize == 0) {
        PRINT_ERR("reloc table size error!\n");
        return LOS_NOK;
    }

    if ((relocInfo->relEntrySize != sizeof(LD_ELF_REL)) &&
        (relocInfo->relEntrySize != sizeof(LD_ELF_RELA))) {
        PRINT_ERR("reloc entry size error!\n");
        return LOS_NOK;
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlSegArchReloc(const DlInfo *dlInfo, const RelocationInfo *relocInfos)
{
    UINT32 ret;
    UINTPTR index;
    LD_ELF_RELA *rela = NULL;
    UINTPTR relCount;
    UINTPTR bytes; /* Sizeof reloc entry. */
    LDRelocParam relocParam = {0};

    bytes = relocInfos->relEntrySize;
    relCount = relocInfos->relTableSize / bytes;
    relocParam.shType = (sizeof(LD_ELF_REL) == bytes) ? LD_SHT_REL : LD_SHT_RELA;

    /* Loop thru the relocation section headers */
    for (index = 0; index < relCount; ++index) {
        rela = (LD_ELF_RELA *)(relocInfos->relTable + (bytes * index));

        /*
        * Calculate actual remote address that needs relocation, and perform
        * external or section relative relocation.
        */
        if (LD_DlSegRelocSetRelParam(dlInfo, rela, &relocParam) == LOS_NOK) {
            return LOS_NOK;
        }

        /* Do relocation */
        ret = ArchSegRelocImp(rela, &relocParam);
        if (ret != LOS_OK) {
            PRINT_ERR("reloc error, type %lu\n", LD_ELF_R_TYPE(rela->info));
            return LOS_NOK;
        }
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 LD_DlSegReloc(const DlInfo *dlInfo)
{
    UINT32 relTypeIndex;

    for (relTypeIndex = 0; relTypeIndex < RELOCATIONINFO_COUNT; relTypeIndex++) {
        if (ArchSegRelocTblCheck(&dlInfo->relocation.rellocationInfos[relTypeIndex]) != LOS_OK) {
            continue;
        }

        if (LD_SoRelocCheckError(&dlInfo->relocation.rellocationInfos[relTypeIndex]) != LOS_OK) {
            return LOS_NOK;
        }

        if (LD_DlSegArchReloc(dlInfo, &dlInfo->relocation.rellocationInfos[relTypeIndex]) != LOS_OK) {
            return LOS_NOK;
        }
    }

    LD_FlushDCache(dlInfo->moduleHandle->segmentsAddr,
                   dlInfo->moduleHandle->segmentsAddr + dlInfo->moduleHandle->segmentsSize);
    LD_FlushICache();
    return LOS_OK;
}

LoadFunc LD_SoLoadFuncGet(DYN_LOAD_MODE mode)
{
#ifdef LOSCFG_DYNLOAD_DYN_FROM_MEM
    if (mode == LD_MODE_MEM) {
        return LD_DlMemLoad;
    }
#endif

#ifdef LOSCFG_DYNLOAD_DYN_FROM_FS
    if (mode == LD_MODE_FS) {
        if (g_dynloadParam.enLoadStrategy == ZIP) {
            return LD_ZipDlLoad;
        } else if (g_dynloadParam.enLoadStrategy == NOZIP) {
            return LD_DlLoad;
        } else {
            PRINT_ERR("invalid so load strategy!\n");
            return NULL;
        }
    }
#endif

    PRINT_ERR("invalid so load mode, mode = %d\n", mode);
    return NULL;
}

LITE_OS_SEC_TEXT_MINOR VOID *LD_DynLoadModule(const SrcInfo *srcInfo, const CHAR *realPath,
                                              BOOL useHash, DYN_LOAD_MODE mode)
{
    DynModuleInfo *moduleHandle = NULL;

    /* Test if the module is already loaded */
    if (LD_IsModuleTaken(realPath, &moduleHandle) == TRUE) {
        if (!IS_LOADED(moduleHandle)) {
            goto LOS_ERREND;
        }
        LD_DlRefCntInc(moduleHandle);
        INC_SOHEAD(moduleHandle);
        /* need clear the temp flags. */
        goto LOS_END;
    }

    if (LD_SetModuleInfo(&moduleHandle, realPath) == FALSE) {
        goto LOS_ERREND;
    }

    if (LD_DlLoadAll(srcInfo, moduleHandle, LD_SoLoadFuncGet(mode), useHash) != LOS_OK) {
        goto LOS_ERREND;
    }

    if (LD_DlRelocAll(LD_DlSegReloc) != LOS_OK) {
        (VOID)LD_ModuleUnload(moduleHandle);
        goto LOS_ERREND;
    }

    goto LOS_END;

LOS_ERREND:
    moduleHandle = NULL;
LOS_END:
    LD_FlagsClear();
    return moduleHandle;
}
#endif

#ifdef LOSCFG_DYNLOAD_DYN_FROM_FS
LITE_OS_SEC_TEXT_MINOR VOID *LOS_SoLoad(const CHAR *elfFileName)
{
    CHAR *realPath = NULL;
    DynModuleInfo *moduleHandle = NULL;

    if (LD_LoadLock() != LOS_OK) {
        return NULL;
    }

    if (LD_IsLoaderInit() == FALSE) {
        if (LD_Init() != LOS_OK) {
            goto OUT;
        }
    }

    realPath = LD_FilePathGet(elfFileName);
    if (realPath == NULL) {
        PRINT_ERR("[%s] no such file: %s\n", __FUNCTION__, elfFileName);
        goto OUT;
    }

    moduleHandle = LD_DynLoadModule(NULL, realPath, FALSE, LD_MODE_FS);

    (VOID)LOS_MemFree(m_aucSysMem1, realPath);

OUT:
    LD_LoadUnlock();
    return moduleHandle;
}
#endif /* LOSCFG_DYNLOAD_DYN_FROM_FS */

#ifdef LOSCFG_DYNLOAD_DYN_FROM_MEM
LITE_OS_SEC_TEXT_MINOR INT32 LD_DlMemLoad(const SrcInfo *srcInfo, DlInfo *dlInfo, BOOL useHash)
{
    LD_ELF_EHDR *ehdr = NULL;
    DynModuleInfo *moduleHandle = dlInfo->moduleHandle;
    LD_ELF_PHDR *phdrTbl = NULL;
    UINT32 needSize;

    ++moduleHandle->refCnt;

    /* Read & check the elf header */
    needSize = sizeof(LD_ELF_EHDR);
    if (srcInfo->fileSize < needSize) {
        return LOS_NOK;
    }
    ehdr = (LD_ELF_EHDR *)srcInfo->fileBuf;
    if (LD_SoHdrVerify(ehdr) != LOS_OK) {
        PRINT_ERR("So Elf head Error!\n");
        return LOS_NOK;
    }

    dlInfo->phNum = ehdr->elfPhNum;
    needSize = ehdr->elfPhNum * sizeof(LD_ELF_PHDR);
    /* Read program header */
    if (srcInfo->fileSize < (ehdr->elfPhoff + needSize)) {
        return LOS_NOK;
    }
    phdrTbl = (LD_ELF_PHDR *)(srcInfo->fileBuf + ehdr->elfPhoff);

    /* Reserve space for loadable segment */
    if (LD_DlLoadImp(phdrTbl, dlInfo, (SrcInfo *)srcInfo, useHash) != LOS_OK) {
        goto LOS_ERREND;
    }

    return LOS_OK;

LOS_ERREND:
    (VOID)LD_ModuleUnload(moduleHandle);
    return LOS_NOK;
}

#endif /* LOSCFG_DYNLOAD_DYN_FROM_MEM */
#endif /* LOSCFG_KERNEL_DYNLOAD_DYN */
