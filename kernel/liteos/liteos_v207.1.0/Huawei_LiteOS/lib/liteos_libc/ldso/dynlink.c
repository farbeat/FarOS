/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: dynamic link module
 * Author: Huawei LiteOS Team
 * Create: 2021-07-28
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

#include "los_ld_elflib.h"
#include "los_printf.h"

void *dlopen(const char *filename, int flag)
{
    void *ret = NULL;
    (void)flag;

#if defined(LOSCFG_KERNEL_DYNLOAD) && defined(LOSCFG_DYNLOAD_DYN_FROM_FS)
    ret = LOS_SoLoad((CHAR *)filename);
#else
    (void)filename;
    PRINT_ERR("Dynamic loading is not supported.\n");
#endif
    return ret;
}

int dlclose(void *handle)
{
#ifdef LOSCFG_KERNEL_DYNLOAD
    return LOS_ModuleUnload(handle);
#else
    (void)handle;
    PRINT_ERR("Dynamic loading is not supported.\n");
    return -1;
#endif
}

void *dlsym(void *handle, const char *symbol)
{
    void *ret = NULL;
#ifdef LOSCFG_KERNEL_DYNLOAD
    ret = LOS_FindSymByName(handle, (CHAR *)symbol);
#else
    (void)handle;
    (void)symbol;
    PRINT_ERR("Dynamic loading is not supported.\n");
#endif
    return ret;
}

#ifdef __LP64__
int dl_iterate_phdr(int (*callback)(void *info, size_t size, void *data), void *data)
{
    (VOID)callback;
    (VOID)data;
    PRINT_ERR("%s is not supported\n", __FUNCTION__);
    return -1;
}
#endif
