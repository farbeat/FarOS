/*----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/

#ifndef _DCACHE_HASH_H
#define _DCACHE_HASH_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef int (*ObjHookFun)(VOID *data);
typedef BOOL (*HashCmpFun)(const VOID *srcData, const VOID *dstData);
typedef unsigned int (*HashCaclFun)(const VOID *data);

#ifdef __FREERTOS__
/**
 * @ingroup los_list
 * Structure of a node in a doubly linked list.
 */
typedef struct LOS_DL_LIST {
    struct LOS_DL_LIST *pstPrev; /**< Current node's pointer to the previous node */
    struct LOS_DL_LIST *pstNext; /**< Current node's pointer to the next node */
} LOS_DL_LIST;

#if defined ( __CC_ARM )
#ifndef CLZ
#define CLZ(value)                                  (__clz(value))
#endif

/* for IAR Compiler */
#elif defined ( __ICCARM__ )
#ifndef CLZ
#define CLZ(value)                                  (__iar_builtin_CLZ(value))
#endif

/* for GNU Compiler */
#elif defined ( __GNUC__ )
#ifndef CLZ
#define CLZ(value)                                  (__builtin_clz(value))
#endif
#else
#error Unknown compiler.
#endif

#else
#include "los_list.h"
#endif
typedef struct {
    unsigned int entrySize;
    unsigned int tableShift;
    unsigned int entryShift;
    HashCmpFun hashCompare;
    HashCaclFun hashCaculate;
} HashParam;

typedef struct {
    unsigned int tableShift;
    unsigned int entrySize;
    HashCaclFun hashCaculate;
    HashCmpFun hashCompare;
    LOS_DL_LIST *hashListHead;
    LOS_DL_LIST lruListHead;
    LOS_DL_LIST freeListHead;
    VOID *hashPool;
} HashTable;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _DCACHE_HASH_H */
