/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: ShellCmd Dynload
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

#include "los_config.h"
#ifdef LOSCFG_SHELL
#include "stdlib.h"
#include "stdio.h"

#include "los_ld_elflib.h"
#include "los_ld_type_pri.h"

#include "shcmd.h"
#include "shell.h"

#define  CONVERSION_DIGIT_BASE  16
#define  ARG_NUM 2

LITE_OS_SEC_TEXT_MINOR INT32 OsShellCmdLdDestroy(INT32 argc, const INT8 *argv[])
{
    if (argc > 0) {
        PRINTK("\nUsage: lddrop\n");
        return LOS_NOK;
    }

    LOS_LdDestroy();
    return LOS_OK;
}

UINTPTR OsShellCmdMopen(INT32 argc, const CHAR *argv[])
{
    VOID *moduleHandle = NULL;

    if (argc != 1) {
        PRINTK("mopen [module_path]\n");
        return (UINTPTR)-1;
    }
#ifdef LOSCFG_DYNLOAD_REL_FROM_FS
    moduleHandle = LOS_ObjLoad((CHAR *)argv[0]);
    if (moduleHandle == NULL) {
        PRINTK("load module failed !\n");
        return (UINTPTR)-1;
    }

    PRINTK("module handle: %p\n", moduleHandle);
#endif
    return (UINTPTR)moduleHandle;
}

LITE_OS_SEC_TEXT_MINOR INT32 OsShellCmdMclose(INT32 argc, const CHAR *argv[])
{
    UINTPTR moduleHandle;

    if (argc != 1) {
        PRINTK("mclose [module_handle]\n");
        return LOS_NOK;
    }

    moduleHandle = strtoul(argv[0], 0, CONVERSION_DIGIT_BASE);
    if (LOS_ModuleUnload((VOID *)moduleHandle) != LOS_OK) {
        PRINTK("unload module failed ! module handle %p\n", moduleHandle);
        return LOS_NOK;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 OsShellCmdFindSym(INT32 argc, const CHAR *argv[])
{
    UINTPTR moduleHandle;
    VOID *func = NULL;

    if (argc != ARG_NUM) {
        PRINTK("findsym [handle] [symbol_name]\n");
        return LOS_NOK;
    }

    moduleHandle = strtoul(argv[0], 0, CONVERSION_DIGIT_BASE);
    func = LOS_FindSymByName((VOID *)moduleHandle, (CHAR *)argv[1]);
    if (func == NULL) {
        PRINTK("symbol not found\n");
        return LOS_NOK;
    }

    PRINTK("symbol address: %p\n", func);
    return LOS_OK;
}

typedef UINT32 (*TstCaseFunc)(VOID);
LITE_OS_SEC_TEXT_MINOR INT32 OsShellCmdCall(INT32 argc, const CHAR *argv[])
{
    TstCaseFunc func = NULL;

    if (argc != 1) {
        PRINTK("call [symbol_addr]\n");
        return LOS_NOK;
    }

    func = (TstCaseFunc)((UINTPTR)strtoul(argv[0], 0, CONVERSION_DIGIT_BASE));
    if (func == NULL) {
        PRINTK("symbol not found\n");
        return LOS_NOK;
    }
    (VOID)func();
    return LOS_OK;
}

SHELLCMD_ENTRY(lddrop_shellcmd, CMD_TYPE_EX, "lddrop", 0, (CmdCallBackFunc)OsShellCmdLdDestroy);
SHELLCMD_ENTRY(mopen_shellcmd, CMD_TYPE_EX, "mopen", 1, (CmdCallBackFunc)OsShellCmdMopen);
SHELLCMD_ENTRY(mclose_shellcmd, CMD_TYPE_EX, "mclose", 1, (CmdCallBackFunc)OsShellCmdMclose);
SHELLCMD_ENTRY(findsym_shellcmd, CMD_TYPE_EX, "findsym", 1, (CmdCallBackFunc)OsShellCmdFindSym);
SHELLCMD_ENTRY(call_shellcmd, CMD_TYPE_EX, "call", 1, (CmdCallBackFunc)OsShellCmdCall);

#endif /* LOSCFG_SHELL */
