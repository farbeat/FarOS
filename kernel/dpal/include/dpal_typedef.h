/*----------------------------------------------------------------------------
 * Copyright (c) CompanyNameMagicTag 2021. All rights reserved.
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

/**
 * @defgroup dpal_typedef Type define
 * @ingroup driver
 */

#ifndef DPAL_TYPEDEF_H
#define DPAL_TYPEDEF_H

#if defined(EMMC_BOOT) || defined(FMC_FLASH_BOOT)
typedef     _Bool        bool;
#else
#include "stdbool.h"
#endif
#include "stdint.h"

#ifdef __LITEOS__
#include <sys/types.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* type definitions */
#ifndef __LITEOS__
typedef unsigned char      u_char;
typedef unsigned int       u_int;

#ifdef __LP64__
typedef uint64_t           size_t;
#else
typedef uint32_t           size_t;
#endif

#endif

typedef volatile int32_t    atomic;
typedef volatile int64_t    atomic64;

#ifndef FALSE
#define FALSE 0U
#endif

#ifndef TRUE
#define TRUE 1U
#endif

#define VOID               void
#ifndef NULL
#define NULL               ((VOID *)0)
#endif

#ifdef YES
#undef YES
#endif
#define YES                1

#ifdef NO
#undef NO
#endif
#define NO                 0

#ifndef DPAL_OK
#define DPAL_OK            0
#endif

#ifndef DPAL_NOK
#define DPAL_NOK           1
#endif

#define DPAL_FAIL          1
#define DPAL_ERROR         (uint32_t)(-1)
#define DPAL_INVALID       (uint32_t)(-1)

typedef enum {
    /*
    * 1 COMMON ERR
    */
    DPAL_RET_SUCCESS                                  = 0,            /*!< succes */
    DPAL_RET_FAILURE                                  = 0xFFFFFFFF,   /*!< failure */
} dpal_errno;

/**
 * @ingroup los_base
 * Define the timeout interval as LOS_NO_WAIT.
 */
#define DPAL_NO_WAIT                                 0

/**
 * @ingroup los_base
 * Define the timeout interval as DPAL_WAIT_FOREVER.
 */
#define DPAL_WAIT_FOREVER                            0xFFFFFFFF
#define DPAL_PRINT_WARN(fmt, ...)
#define DPAL_PRINT_ERR(fmt, ...)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _DPAL_TYPEDEF_H */
