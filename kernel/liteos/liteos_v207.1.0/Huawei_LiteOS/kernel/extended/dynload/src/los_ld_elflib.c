/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Ld ElfLib
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

#include "los_ld_elflib_pri.h"
#include "los_ld_macro_pri.h"
#include "los_ld_commlib_pri.h"
#include "los_ld_reloc_pri.h"
#include "los_ld_modulelib_pri.h"
#include "los_ld_dllib_pri.h"
#include "los_ld_symlib_pri.h"

#include "fcntl.h"
#include "unistd.h"
#include "sys/stat.h"
#include "limits.h"

#include "los_hwi.h"
#include "los_memory.h"

#ifdef LOSCFG_KERNEL_DYNLOAD_REL
#ifdef LOSCFG_DYNLOAD_REL_FROM_FS
LITE_OS_SEC_TEXT_MINOR UINTPTR LD_ObjReadFile2Mem(const CHAR *elfFileName)
{
    struct stat statBuff;
    SrcInfo srcInfo = { -1, NULL, 0, 0 };
    DestInfo destInfo = { NULL, 0, 0 };

    srcInfo.fd = open(elfFileName, O_RDONLY);
    if (srcInfo.fd < 0) {
        return 0;
    }

    if (stat(elfFileName, &statBuff) < 0) {
        goto LOS_ERREND;
    }
    if (statBuff.st_size > (INT_MAX - 1)) {
        goto LOS_ERREND;
    }

    destInfo.bufSize = (UINT32)statBuff.st_size;
    destInfo.needSize = (UINT32)statBuff.st_size;
    destInfo.buf = LOS_MemAlloc(g_dynloadHeap, destInfo.bufSize);
    if (destInfo.buf == NULL) {
        goto LOS_ERREND;
    }

    if (LD_DlDoRead(&srcInfo, &destInfo) != LOS_OK) {
        (VOID)LOS_MemFree(g_dynloadHeap, (VOID *)destInfo.buf);
        goto LOS_ERREND;
    }

    if (close(srcInfo.fd) < 0) {
        (VOID)LOS_MemFree(g_dynloadHeap, (VOID *)destInfo.buf);
        return 0;
    }
    return (UINTPTR)destInfo.buf;

LOS_ERREND:
    (VOID)close(srcInfo.fd);
    return 0;
}
#endif

LITE_OS_SEC_TEXT_MINOR UINT32 LD_ObjHdrVerify(const LD_ELF_EHDR *ehdr)
{
    if (LD_ElfHdrVerify(ehdr) != LOS_OK) {
        return LOS_NOK;
    }

    /* Check module type */
    if ((ehdr->elfType != LD_ET_REL) && (ehdr->elfType != LD_ET_EXEC)) {
        return LOS_NOK;
    }

    if (ehdr->elfShNum == 0) {
        return LOS_NOK;
    }

    /* Check the size of section */
    if (ehdr->elfShEntSize != sizeof(LD_ELF_SHDR)) {
        return LOS_NOK;
    }

    return LOS_OK;
}

STATIC UINT32 LD_ObjRelocCheckError(const LD_ELF_SHDR *relHdr)
{
    /* Some sanity checking */
    if ((relHdr->shType != LD_SHT_RELA) && (relHdr->shType != LD_SHT_REL)) {
        PRINT_ERR("reloc type error!\n");
        return LOS_NOK;
    }

    if ((relHdr->shEntSize != sizeof(LD_ELF_RELA)) && (relHdr->shEntSize != sizeof(LD_ELF_REL))) {
        PRINT_ERR("reloc entry size error!\n");
        return LOS_NOK;
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC UINT32 LD_ObjScnsLoad(UINTPTR loadAddr, DynModuleInfo *moduleHandle,
                                                    const LD_ELF_SHDR *scnHdr, UINTPTR scnsLoadSize)
{
    if (scnHdr->shType == LD_SHT_PROGBITS) {  /* .text .rodata .data */
        if (memcpy_s((VOID *)loadAddr, (moduleHandle->segmentsAddr + scnsLoadSize) - loadAddr,
                     (VOID *)(UINTPTR)scnHdr->shOffset, scnHdr->shSize) != EOK) {
            goto ERR;
        }
    } else if (scnHdr->shType == LD_SHT_NOBITS) {  /* .bss */
        if (memset_s((VOID *)loadAddr, (moduleHandle->segmentsAddr + scnsLoadSize) - loadAddr,
                     0, scnHdr->shSize) != EOK) {
            goto ERR;
        }
    }
    return LOS_OK;
ERR:
    (VOID)LOS_MemFree(g_dynloadHeap, (VOID *)moduleHandle->segmentsAddr);
    moduleHandle->segmentsAddr = 0;
    return LOS_NOK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LD_ObjScnsStore(const DlInfo *dlInfo)
{
    UINT32 scnHdrIdx;
    LD_ELF_SHDR *scnHdr = NULL; /* pointer of a section descriptor */
    DynModuleInfo *moduleHandle = dlInfo->moduleHandle;
    UINTPTR loadAddr;
    UINTPTR scnsLoadSize = moduleHandle->segmentsSize;

    moduleHandle->segmentsAddr = (UINTPTR)LOS_MemAllocAlign(g_dynloadHeap, (UINT32)scnsLoadSize, DYNLOAD_ALIGN_SIZE);
    if (moduleHandle->segmentsAddr == 0) {
        return LOS_NOK;
    }
    loadAddr = moduleHandle->segmentsAddr;

    for (scnHdrIdx = 0; scnHdrIdx < dlInfo->scnNum; scnHdrIdx++) {
        scnHdr = &dlInfo->scnHdrTbl[scnHdrIdx];

        if ((scnHdr->shFlags & LD_SHF_ALLOC) && (scnHdr->shSize != 0)) {
            if (LD_ObjScnsLoad(loadAddr, moduleHandle, scnHdr, scnsLoadSize) != LOS_OK) {
                (VOID)LOS_MemFree(g_dynloadHeap, (VOID *)moduleHandle->segmentsAddr);
                moduleHandle->segmentsAddr = 0;
                return LOS_NOK;
            }
            /*
             * If the section attribute is allocable, the offset represents
             * the actual loading address of this section.
             */
            scnHdr->shOffset = loadAddr;
            loadAddr += LOS_Align(scnHdr->shSize, sizeof(UINTPTR));
        }
    }

    LD_FlushDCache(dlInfo->moduleHandle->segmentsAddr,
                   dlInfo->moduleHandle->segmentsAddr + dlInfo->moduleHandle->segmentsSize);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_ObjScnHdrReadSetSymTab(DlInfo *dlInfo, const LD_ELF_SHDR *scnHdrTbl,
                                                              const LD_ELF_SHDR *scnHdr, UINT32 *strTblIdx)
{
    if (scnHdr->shEntSize != sizeof(LD_ELF_SYM)) {
        return LOS_NOK;
    }
    /* shLink of symbol section header is the section header index of the associated string table. */
    if (scnHdrTbl[scnHdr->shLink].shType != LD_SHT_STRTAB) {
        return LOS_NOK;
    }
    dlInfo->symTab = (LD_ELF_SYM *)(UINTPTR)scnHdr->shOffset;
    dlInfo->symNum = (UINT32)(scnHdr->shSize / scnHdr->shEntSize);
    *strTblIdx = scnHdr->shLink; /* record the index of string table section */
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_ObjScnHdrReadCheckRel(const LD_ELF_SHDR *scnHdrTbl, const LD_ELF_SHDR *scnHdr)
{
    if (scnHdrTbl[scnHdr->shLink].shType != LD_SHT_SYMTAB) {
        return LOS_NOK;
    }
    if (LD_ObjRelocCheckError(scnHdr) != LOS_OK) {
        return LOS_NOK;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_ObjScnHdrReadSetStrTab(DlInfo *dlInfo, const LD_ELF_SHDR *scnHdrTbl,
                                                              const LD_ELF_EHDR *ehdr, UINT32 strTblIdx)
{
    if ((strTblIdx > 0) && (strTblIdx < ehdr->elfShNum)) {
        dlInfo->strTab = (CHAR *)(UINTPTR)scnHdrTbl[strTblIdx].shOffset;
    } else {
        return LOS_NOK;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_ObjScnHdrReadSetDlInfo(DlInfo *dlInfo, LD_ELF_SHDR * const scnHdrTbl,
                                                              const LD_ELF_EHDR *ehdr, UINTPTR scnsLoadSize)
{
    dlInfo->scnHdrTbl = scnHdrTbl;
    dlInfo->scnNum = ehdr->elfShNum;

    if (scnsLoadSize == 0) {
        PRINT_ERR("no alloc sections.\n");
        return LOS_NOK;
    }
    scnsLoadSize = LOS_Align(scnsLoadSize, DYNLOAD_ALIGN_SIZE);
    dlInfo->moduleHandle->segmentsSize = scnsLoadSize;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC UINT32 LD_ObjScnHdrReadByType(DlInfo *dlInfo, const LD_ELF_SHDR *scnHdrTbl,
                                                            const LD_ELF_SHDR *scnHdr, UINTPTR *scnsLoadSize,
                                                            UINT32 *strTblIdx)
{
    switch (scnHdr->shType) {
        case LD_SHT_PROGBITS: /* text, data, literal section */
        case LD_SHT_NOBITS: /* bss */
            if (scnHdr->shFlags & LD_SHF_ALLOC) {
                *scnsLoadSize += LOS_Align(scnHdr->shSize, sizeof(UINTPTR));
            }
            break;
        case LD_SHT_SYMTAB: /* symbol table */
            if (LD_ObjScnHdrReadSetSymTab(dlInfo, scnHdrTbl, scnHdr, strTblIdx) == LOS_NOK) {
                return LOS_NOK;
            }
            break;
        case LD_SHT_REL:
        case LD_SHT_RELA: /* relocation table */
            /* shLink of relocation section header is the section header index of the associated symbol table. */
            if (LD_ObjScnHdrReadCheckRel(scnHdrTbl, scnHdr) == LOS_NOK) {
                return LOS_NOK;
            }
            break;
        default:
            break;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LD_ObjScnHdrRead(UINTPTR moduleAddr, DlInfo *dlInfo)
{
    INT32 scnHdrIdx;
    UINT32 strTblIdx = 0;
    UINTPTR scnsLoadSize = 0;
    LD_ELF_SHDR *scnHdrTbl = NULL;
    LD_ELF_SHDR *scnHdr = NULL; /* pointer of a section descriptor */
    LD_ELF_EHDR *ehdr = (LD_ELF_EHDR *)LD_SeekAddr(moduleAddr, 0);
    dlInfo->elfHdr = ehdr;

    /* Read & check elf file header */
    if (LD_ObjHdrVerify(ehdr) != LOS_OK) {
        PRINT_ERR("Obj Elf head Error!\n");
        return LOS_NOK;
    }

    scnHdrTbl = (LD_ELF_SHDR *)LD_SeekAddr(moduleAddr, ehdr->elfShoff);

    for (scnHdrIdx = 0; scnHdrIdx < ehdr->elfShNum; scnHdrIdx++) {
        scnHdr = &scnHdrTbl[scnHdrIdx];

        if (!LD_CHECK_2_ALIGN(scnHdr->shAddrAlign)) { /* Align must be power of 2 */
            return LOS_NOK;
        }

        if (scnHdr->shLink >= ehdr->elfShNum) {
            PRINT_ERR("%s failed, shLink: %u\n", __FUNCTION__, scnHdr->shLink);
            return LOS_NOK;
        }

        /* Section memory offset */
        scnHdr->shOffset = LD_SeekAddr(moduleAddr, scnHdr->shOffset);
        if (LD_ObjScnHdrReadByType(dlInfo, scnHdrTbl, scnHdr, &scnsLoadSize, &strTblIdx) != LOS_OK) {
            return LOS_NOK;
        }
    }

    /* get the string table */
    if (LD_ObjScnHdrReadSetStrTab(dlInfo, scnHdrTbl, ehdr, strTblIdx) == LOS_NOK) {
        return LOS_NOK;
    }

    if (LD_ObjScnHdrReadSetDlInfo(dlInfo, scnHdrTbl, ehdr, scnsLoadSize) == LOS_NOK) {
        return LOS_NOK;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LD_CommonManage(DynModuleInfo *moduleHandle,
                                              UINTPTR comAreaSize,
                                              const CHAR *symName,
                                              VOID **symAddr,
                                              INT8 *symType)
{
    /* If additional info is available then we are dealing with a PowerPC */
    *symType = LD_SYM_BSS | LD_SYM_COMM | LD_SYM_GLOBAL;

    /*
     * Allocate memory for new symbol. This must be done even if flag
     * LOAD_NO_SYMBOLS is applied (SPR #9259).
     */
    *symAddr = (VOID *)LOS_MemAlloc(g_dynloadHeap, (UINT32)comAreaSize);
    if (*symAddr == NULL) {
        return LOS_NOK;
    }

    if (LD_ComSymAdd(moduleHandle, (INT8 *)*symAddr) != LOS_OK) {
        PRINT_ERR("%s, %d\n", __FUNCTION__, __LINE__);
        goto LOS_ERREND;
    }

    (VOID)memset((VOID *)*symAddr, 0, comAreaSize);

    if (LD_SymAdd(moduleHandle, symName, (INT8 *)*symAddr, *symType) != LOS_OK) {
        PRINT_ERR("Can't add '%s' to symbol table.\n", symName);
        goto LOS_ERREND;
    }

    return LOS_OK;

LOS_ERREND:
    (VOID)LOS_MemFree(g_dynloadHeap, (VOID *)*symAddr);
    *symAddr = NULL;

    return LOS_NOK;
}

LITE_OS_SEC_TEXT_MINOR STATIC UINT32 LD_ObjSymTabDoProcess(const DlInfo *dlInfo, const LD_ELF_SYM *symbol,
                                                           UINT32 symIndex, INT8 *symType, const CHAR *name)
{
    VOID *addr = NULL; /* table resident symbol address */
    DynModuleInfo *moduleHandle = dlInfo->moduleHandle; /* handle of the module to be loaded */

    /*
     * Undefined external symbol or "common" symbol
     * A common symbol type is denoted by a special section index
     */
    if (symbol->stShndx == LD_SHN_COMMON) {
        if (LD_CommonManage(moduleHandle, symbol->stSize, name, &addr, symType) != LOS_OK) {
            PRINT_ERR("%s, %d\n", __FUNCTION__, __LINE__);
            return LOS_NOK;
        }
    } else {
        if (LD_SymLoadAddrGet(dlInfo, symIndex, (UINTPTR *)(VOID *)&addr) != LOS_OK) {
            return LOS_NOK;
        }

        /* Add symbol to target's symbol table */
        if (LD_SymAdd(moduleHandle, name, (INT8 *)addr, *symType) != LOS_OK) {
            PRINT_ERR("Can't add '%s' to symbol table\n", name);
            return LOS_NOK;
        }
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LD_ObjSymTabProcess(const DlInfo *dlInfo)
{
    UINT32 symIndex; /* symbol index */
    CHAR *name = NULL; /* symbol name (plus EOS) */
    LD_ELF_SYM *symbol = NULL; /* current symbol being processed */
    INT8 symType; /* internal value for symbol type */

    /* Loop through all symbol table entries in object file */
    for (symIndex = 0; symIndex < dlInfo->symNum; symIndex++) {
        symbol = &dlInfo->symTab[symIndex];

        /* Determine the symbol type */
        symType = LD_ElfSymTypeGet(symbol, dlInfo->scnHdrTbl);
        name = dlInfo->strTab + symbol->stName;

        /* Determine if symbol should be put into symbol table */
        if (LD_ElfSymIsVisible(symbol)) {
            symType = (INT8)((UINT8)symType | LD_SYM_GLOBAL);
            if (LD_ObjSymTabDoProcess(dlInfo, symbol, symIndex, &symType, name) != LOS_OK) {
                return LOS_NOK;
            }
        }
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LD_ObjSymTableBuild(const DlInfo *dlInfo)
{
    UINT32 symNum = dlInfo->symNum; /* number of syms in current symtab */
    CHAR *stringTable = dlInfo->strTab; /* pointer on current string table */
    LD_ELF_SYM *symTblRefs = dlInfo->symTab;
    DynModuleInfo *moduleHandle = dlInfo->moduleHandle; /* Handle of the module to be loaded */

    if (!LD_ModuleSymTblCreate(moduleHandle, symTblRefs, stringTable, symNum)) {
        return LOS_NOK;
    }

    if (LD_ObjSymTabProcess(dlInfo) != LOS_OK) {
        return LOS_NOK;
    }

    if (LD_ModuleSymTblSort(moduleHandle) != LOS_OK) {
        PRINT_ERR("sort symbol tbl error!\n");
        return LOS_NOK;
    }

    return LOS_OK;
}

#if defined(LOSCFG_DYNLOAD_REL_FROM_FS) || defined(LOSCFG_DYNLOAD_REL_FROM_MEM)
LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_DoObjLoad(UINTPTR moduleAddr, DlInfo *dlInfo)
{
    /* Read section header */
    if (LD_ObjScnHdrRead(moduleAddr, dlInfo) != LOS_OK) {
        return LOS_NOK;
    }

    /* load loadable sections */
    if (LD_ObjScnsStore(dlInfo) != LOS_OK) {
        return LOS_NOK;
    }

    /* Build / update the target's symbol table with symbols found in module's symbol tables. */
    if (LD_ObjSymTableBuild(dlInfo) != LOS_OK) {
        return LOS_NOK;
    }

    return LOS_OK;
}
#endif

#ifdef LOSCFG_DYNLOAD_REL_FROM_FS
LITE_OS_SEC_TEXT_MINOR INT32 LD_ObjFsLoad(const SrcInfo *srcInfo, DlInfo *dlInfo, BOOL useHash)
{
    (VOID)srcInfo;
    (VOID)useHash;

    DynModuleInfo *moduleHandle = dlInfo->moduleHandle;
    UINTPTR moduleAddr = LD_ObjReadFile2Mem(moduleHandle->moduleFileName);
    if (moduleAddr == 0) {
        return LOS_NOK;
    }

    return LD_DoObjLoad(moduleAddr, dlInfo);
}
#endif

LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_ObjArchRelocSetRelParam(const DlInfo *dlInfo, const LD_ELF_SHDR *relHdr,
                                                               const LD_ELF_RELA *relocCmd, LDRelocParam *relocParam)
{
    LD_ELF_SHDR *scnHdrTbl = dlInfo->scnHdrTbl;
    relocParam->relocType = LD_ELF_R_TYPE(relocCmd->info);
    relocParam->shType = relHdr->shType;

    /*
     * Calculate actual remote address that needs relocation,
     * and perform external or section relative relocation.
     */
    relocParam->position = scnHdrTbl[relHdr->shInfo].shOffset + relocCmd->offset;
    relocParam->addend = relocCmd->addend;
    if (LD_SymLoadAddrGet(dlInfo, LD_ELF_R_SYM(relocCmd->info), &relocParam->symAdd) != LOS_OK) {
        return LOS_NOK;
    }
    relocParam->thumbFunc = 0;

    return  LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LD_ObjArchReloc(const DlInfo *dlInfo, const LD_ELF_SHDR *relHdr)
{
    UINTPTR relocNum = relHdr->shSize / relHdr->shEntSize; /* number of reloc entries in section */
    UINTPTR relocIdx; /* index of the reloc entry being processed */
    UINTPTR bytes = relHdr->shEntSize; /* Sizeof reloc entry */
    LD_ELF_RELA *relocCmd = NULL; /* relocation structure */
    UINT32 ret;
    LDRelocParam relocParam = {0};

    for (relocIdx = 0; relocIdx < relocNum; ++relocIdx) {
        /* Read relocation command */
        relocCmd = (LD_ELF_RELA *)((UINTPTR)relHdr->shOffset + (bytes * relocIdx));
        if (LD_ObjArchRelocSetRelParam(dlInfo, relHdr, relocCmd, &relocParam) == LOS_NOK) {
            return LOS_NOK;
        }

        ret = ArchObjRelocImp(relocCmd, &relocParam);
        if (ret != LOS_OK) {
            PRINT_ERR("Reloc error, failed to reloc type %lu\n", LD_ELF_R_TYPE(relocCmd->info));
            return LOS_NOK;
        }
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_ObjDoReloc(const DlInfo *dlInfo, const LD_ELF_SHDR *scnHdr)
{
    LD_ELF_SHDR *allocScnHdr = &dlInfo->scnHdrTbl[scnHdr->shInfo];
    if ((allocScnHdr->shFlags & LD_SHF_ALLOC) && (allocScnHdr->shType == LD_SHT_PROGBITS)) {
        if (LD_ObjArchReloc(dlInfo, scnHdr) != LOS_OK) {
            return LOS_NOK;
        }
    }
    return LOS_OK;
}

/*
 * Description: relocate the text & data section
 *
 * Input:   dlInfo - information used by relocation
 *
 * Output:  NONE.
 * RETURNS: LOS_OK- relocate succeed
 *          LOS_NOK-relocate failed
 */
LITE_OS_SEC_TEXT_MINOR INT32 LD_ObjReloc(const DlInfo *dlInfo)
{
    UINT32 scnHdrIdx;
    LD_ELF_SHDR *scnHdr = NULL; /* pointer of a section table entry */

    /* Loop through the relocation section headers */
    for (scnHdrIdx = 0; scnHdrIdx < dlInfo->scnNum; scnHdrIdx++) {
        scnHdr = &dlInfo->scnHdrTbl[scnHdrIdx];

        switch (scnHdr->shType) {
            case LD_SHT_REL:
            case LD_SHT_RELA: /* relocation table */
                if (LD_ObjDoReloc(dlInfo, scnHdr) != LOS_OK) {
                    return LOS_NOK;
                }
                break;
            default:
                break;
        }
    }

    LD_FlushDCache(dlInfo->moduleHandle->segmentsAddr,
                   dlInfo->moduleHandle->segmentsAddr + dlInfo->moduleHandle->segmentsSize);
    LD_FlushICache();
    return LOS_OK;
}

#ifdef LOSCFG_DYNLOAD_REL_FROM_MEM
LITE_OS_SEC_TEXT_MINOR STATIC INT32 LD_ObjMemLoad(const SrcInfo *srcInfo, DlInfo *dlInfo, BOOL useHash)
{
    (VOID)useHash;

    INT32 ret;
    CHAR *buf = NULL;

    LOS_ASSERT(srcInfo->offset == 0);  /* offset if passed from LOS_MemLoad and should be zero */
    LOS_ASSERT(srcInfo->fileSize > 0); /* checked in LOS_MemLoad */

    buf = LOS_MemAlloc(g_dynloadHeap, srcInfo->fileSize);
    if (buf == NULL) {
        return LOS_NOK;
    }

    DestInfo destInfo = {
        .buf = buf,
        .bufSize = srcInfo->fileSize,
        .needSize = srcInfo->fileSize,
    };

    ret = LD_DlDoRead(srcInfo, &destInfo);
    if (ret != LOS_OK) {
        (VOID)LOS_MemFree(g_dynloadHeap, destInfo.buf);
        return ret;
    }

    return LD_DoObjLoad((UINTPTR)destInfo.buf, dlInfo);
}
#endif

LITE_OS_SEC_TEXT_MINOR STATIC INLINE LoadFunc LD_ObjLoadFuncGet(DYN_LOAD_MODE mode)
{
#ifdef LOSCFG_DYNLOAD_REL_FROM_FS
    if (mode == LD_MODE_FS) {
        return LD_ObjFsLoad;
    }
#endif
#ifdef LOSCFG_DYNLOAD_REL_FROM_MEM
    if (mode == LD_MODE_MEM) {
        return LD_ObjMemLoad;
    }
#endif

    return NULL;
}

LITE_OS_SEC_TEXT_MINOR VOID *LD_RelLoadModule(const SrcInfo *srcInfo, const CHAR *realPath,
                                              BOOL useHash, DYN_LOAD_MODE mode)
{
    DynModuleInfo *moduleHandle = NULL;

    /* Test if the module is already loaded */
    if (LD_IsModuleTaken(realPath, &moduleHandle) == TRUE) {
        if (IS_LOADED(moduleHandle)) {
            goto LOS_END;
        } else {
            goto LOS_ERREND;
        }
    }

    moduleHandle = LD_ModuleHandleAlloc(realPath);
    if (moduleHandle == NULL) {
        goto LOS_ERREND;
    }
    moduleHandle->type = ET_OBJ;

    if (LD_DlLoadAll(srcInfo, moduleHandle, LD_ObjLoadFuncGet(mode), useHash) != LOS_OK) {
        goto LOS_ERREND;
    }

    if (LD_DlRelocAll(LD_ObjReloc) != LOS_OK) {
        (VOID)LD_ModuleUnload(moduleHandle);
        goto LOS_ERREND;
    }

    goto LOS_END;

LOS_ERREND:
    moduleHandle = NULL;
LOS_END:
    return moduleHandle;
}

#ifdef LOSCFG_DYNLOAD_REL_FROM_FS
LITE_OS_SEC_TEXT_MINOR VOID *LOS_ObjLoad(const CHAR *elfFileName)
{
    VOID *moduleHandle = NULL;
    CHAR *realPath = NULL;

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

    moduleHandle = LD_RelLoadModule(NULL, realPath, FALSE, LD_MODE_FS);

    (VOID)LOS_MemFree(m_aucSysMem1, realPath);

OUT:
    LD_LoadUnlock();
    return moduleHandle;
}
#endif /* LOSCFG_DYNLOAD_REL_FROM_FS */
#endif /* LOSCFG_KERNEL_DYNLOAD_REL */
