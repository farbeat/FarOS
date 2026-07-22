/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: LiteOS Disk Module Inside Headfile
 * Create: 2019-06-12
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

/**
 * @defgroup disk Disk
 * @ingroup filesystem
 */

#ifndef _DISK_PRI_H
#define _DISK_PRI_H

#include "dpal.h"
#include "dpal_lock.h"
#include "disk.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef BUILD_RECOVERY_IMAGE
#define disk_log_err dpal_log_err
#else
#define disk_log_err
#endif

#define DISK_LOCK(disk) do {                                               \
    uint32_t ret_ = dpal_mux_pend((disk)->disk_mutex, 0xFFFFFFFF);       \
    if (ret_ != DPAL_OK) {                                                  \
        disk_log_err("%s %d, mutex lock failed, ret = 0x%x\n",                \
                  __FUNCTION__, __LINE__, ret_);                           \
    }                                                                      \
} while (0)

#define DISK_UNLOCK(disk) do {                                             \
    uint32_t ret_ = dpal_mux_post((disk)->disk_mutex);                         \
    if (ret_ != DPAL_OK) {                                                  \
        disk_log_err("%s %d, mutex unlock failed, ret: 0x%x\n",               \
                  __FUNCTION__, __LINE__, ret_);                           \
    }                                                                      \
} while (0)

extern dpal_spinlock g_diskSpinlock;
extern dpal_spinlock g_diskFatBlockSpinlock;

#ifdef CONFIG_FS_FAT_CACHE
extern uint32_t GetFatBlockNums(VOID);
extern VOID SetFatBlockNums(uint32_t blockNums);
extern uint32_t GetFatSectorsPerBlock(VOID);
extern VOID SetFatSectorsPerBlock(uint32_t sectorsPerBlock);
#endif
extern int32_t SetDiskPartName(los_part *part, const char *src);
extern int32_t EraseDiskByID(uint32_t diskId, size_t startSector, uint32_t sectors);
extern BOOL IsBlockStatusReady(const struct inode *blkDriver);
extern VOID OsDiskInit(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
