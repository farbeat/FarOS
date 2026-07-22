/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: LiteOS MMC OS Adapt Header File
 * Author: @CompanyNameTag
 * Create: 2022-01-06
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
#ifndef _DEV_MMC_ADAPT_OS_H
#define _DEV_MMC_ADAPT_OS_H

#ifdef LOSCFG_FS_PROC
#include "linux/seq_file.h" /* for proc.c */
#endif
#include "los_mux.h"
#include "linux/device.h"
#include "los_misc_pri.h"
#include "linux/device.h"
#include "linux/module.h"
#include "linux/platform_device.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct device *device_t;

/* Platform mmc header file */
#define PLATFORM_MMC_HEADER_FILE    "soc_mmc.h"
#define CFG_BASE_CORE_TICK_PER_SECOND 1000
/* Address conversion operation */
#define MMC_IO_DEVICE_ADDR(x)       x
#define MMC_VMM_TO_DMA_ADDR(x)      x

/* DMA memory */
#define MMC_DMA_VADDR_TO_PADDR(x)   x
#define MMC_DMA_CACHE	            0
#define mmc_dma_mem_alloc(pool, size, boundary, type)   memalign(boundary, size)
#define mmc_dma_mem_free(addr)                          free(addr)
#define mmc_dcache_flush_range(start, end)              dma_cache_clean(start, end)
#define mmc_dcache_inv_range(start, end)                dma_cache_inv(start, end)

/* Proc operation */
#define mmc_seq_printf              seq_printf
#define mmc_seq_file                seq_file
#define mmc_proc_dir_entry          proc_dir_entry
#define mmc_remove_proc_entry       remove_proc_entry
#define mmc_create_proc_entry       create_proc_entry
#define mmc_proc_mkdir              proc_mkdir
#define SET_PROC_ENTRY_OPS(entry, ops)  ((entry)->proc_fops = &(ops))

/* Mutex adapt */
#define MMC_MUTEX                   UINT32
#define mmc_mtx_init(m, n, t, o)    LOS_MuxCreate((m))
#define mmc_mtx_destroy(m)          (void)LOS_MuxDelete(*(uint32_t *)(m))
#define mmc_mtx_lock(m)             (void)LOS_MuxPend(*(uint32_t *)(m), LOS_WAIT_FOREVER)
#define mmc_mtx_unlock(m)           (void)LOS_MuxPost(*(uint32_t *)(m))
#define INVALID_MMC_MUTEX_ID        0xffffffff
#define INVALID_MMC_MUTEX           INVALID_MMC_MUTEX_ID
#define MMC_MUTEX_IS_INVALID(mutex) ((mutex) == INVALID_MMC_MUTEX_ID)

/* OS kernel task adapt */
#define MMC_TASK_PARAM_INIT_ARG(task_param, arg)    LOS_TASK_PARAM_INIT_ARG(task_param, arg)

/* Device memory operation */
#define mmc_device_alloc()              (device_t)zalloc(sizeof(struct device))
#define mmc_device_free(addr)           free(addr)

/* Platfrom device operation */
#define MMC_RES_MEMORY                  IORESOURCE_MEM
#define MMC_RES_IRQ                     IORESOURCE_IRQ
#define mmc_get_resource(a, b, c, d)    platform_get_resource(a, b, d)

/* BSD operation */
#define __predict_false(exp)    exp
#define assert(x) (void)0
#define KASSERT(exp, msg)       LOS_ASSERT(exp)

typedef struct {
    int64_t tv_sec;
    int64_t tv_usec;
} mmc_timeval;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _DEV_MMC_ADAPT_OS_H */
