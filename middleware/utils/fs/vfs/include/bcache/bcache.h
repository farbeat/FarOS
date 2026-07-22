/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2015-2019. All rights reserved.
 * Description: LiteOS Bcache Module Headfile
 * Create: 2015-01-01
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
#ifndef _BCACHE_H
#define _BCACHE_H

#include "inode/inode.h"
#include "rbtree.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define ALIGN_LIB(x)          (((x) + (HALARC_ALIGNMENT - 1)) & ~(HALARC_ALIGNMENT - 1))
#define ALIGN_DISP(x)         (HALARC_ALIGNMENT - ((x) & (HALARC_ALIGNMENT - 1)))
#define BCACHE_PREREAD_PRIO   12
#define UNSIGNED_INTEGER_BITS 32
#define UNINT_MAX_SHIFT_BITS  31
#define UNINT_LOG2_SHIFT      5
#define PREREAD_BLOCK_NUM     2
#define EVEN_JUDGED           2
#define PERCENTAGE            100
#define PREREAD_EVENT_MASK    0xf

#if CONFIG_FS_FAT_SECTOR_PER_BLOCK < UNSIGNED_INTEGER_BITS
#error cache too small
#else
#define BCACHE_BLOCK_FLAGS (CONFIG_FS_FAT_SECTOR_PER_BLOCK / UNSIGNED_INTEGER_BITS)
#endif

#define FS_OS_TASK_PRIORITY_LOWEST 31
#define FS_LOS_WAIT_FOREVER 0xFFFFFFFF
#define FS_LOS_TASK_STATUS_DETACHED   NULL //0x0100U
#define FS_LOS_WAITMODE_CLR 1U
#define FS_LOS_WAITMODE_OR 2U

typedef struct {
    dpal_dl_list_t listNode;   /* list node */
    struct rb_node rbNode;  /* red-black tree node */
    uint64_t num;             /* block number */
    BOOL modified;          /* is this block data modified (needs write) */
    BOOL readFlag;          /* is the block data have read form sd(real data) */
    uint32_t age;
    uint32_t pgHit;
    uint8_t *data;            /* block data */
    uint32_t flag[0];
} OsBcacheBlock;

typedef int32_t (*BcacheReadFun)(struct inode *, /* private data */
                               uint8_t *,        /* block buffer */
                               uint32_t,         /* number of blocks to read */
                               uint64_t);        /* starting block number */

typedef int32_t (*BcacheWriteFun)(struct inode *, /* private data */
                                const uint8_t *,  /* block buffer */
                                uint32_t,         /* number of blocks to write */
                                uint64_t);        /* starting block number */

struct tagOsBcache;

typedef void (*BcachePrereadFun)(struct tagOsBcache *,   /* block cache instance space holder */
                                 const OsBcacheBlock *); /* block data */

typedef struct tagOsBcache {
    void *priv;                   /* private data */
    dpal_dl_list_t listHead;         /* head of block list */
    struct rb_root rbRoot;        /* block red-black tree root */
    uint32_t blockSize;             /* block size in bytes */
    uint32_t blockSizeLog2;         /* block size log2 */
    uint64_t blockCount;            /* block count of the disk */
    uint32_t sectorSize;            /* device sector size in bytes */
    uint32_t sectorPerBlock;        /* sector count per block */
    uint8_t *memStart;              /* memory base */
    uint32_t prereadTaskId;         /* preread task id */
    uint64_t curBlockNum;           /* current preread block number */
    dpal_dl_list_t freeListHead;     /* list of free blocks */
    BcacheReadFun breadFun;       /* block read function */
    BcacheWriteFun bwriteFun;     /* block write function */
    BcachePrereadFun prereadFun;  /* block preread function */
    uint8_t *rwBuffer;              /* buffer for bcache block */
    uint32_t bcacheMutex;           /* mutex for bcache */
    dpal_event_t bcacheEvent;       /* event for bcache */
    uint32_t modifiedBlock;         /* number of modified blocks */
#ifdef CONFIG_FS_FAT_CACHE_SYNC_THREAD
    uint32_t syncTaskId;            /* sync task id */
#endif
} OsBcache;

/**
 * @ingroup  bcache
 *
 * @par Description:
 * The BlockCacheRead() function shall read data from the bcache, and if it doesn't hit, read the data from disk.
 *
 * @param  bc    [IN]  block cache instance
 * @param  buf   [OUT] data buffer ptr
 * @param  len   [IN]  number of bytes to read
 * @param  num   [IN]  starting block number
 * @param  pos   [IN]  starting position inside starting block
 *
 * @attention
 * <ul>
 * <li>The block number is automatically adjusted if position is greater than block size.</li>
 * </ul>
 *
 * @retval #0           read succeded
 * @retval #int32_t       read failed
 *
 * @par Dependency:
 * <ul><li>bcache.h</li></ul>
 */
int32_t BlockCacheRead(OsBcache *bc,
                     uint8_t *buf,
                     uint32_t *len,
                     uint64_t num,
                     uint64_t pos);

/**
 * @ingroup  bcache
 *
 * @par Description:
 * The BlockCacheWrite() function shall write data to the bcache.
 *
 * @param  bc    [IN]  block cache instance
 * @param  buf   [IN]  data buffer ptr
 * @param  len   [IN]  number of bytes to write
 * @param  num   [IN]  starting block number
 * @param  pos   [IN]  starting position inside starting block
 *
 * @attention
 * <ul>
 * <li>The block number is automatically adjusted if position is greater than block size.</li>
 * </ul>
 *
 * @retval #0           write succeded
 * @retval #int32_t       write failed
 *
 * @par Dependency:
 * <ul><li>bcache.h</li></ul>
 */
int32_t BlockCacheWrite(OsBcache *bc,
                      const uint8_t *buf,
                      uint32_t *len,
                      uint64_t num,
                      uint64_t pos);

/**
 * @ingroup  bcache
 *
 * @par Description:
 * The BlockCacheSync() function shall write-back all dirty data in the bcache into the disk.
 *
 * @param  bc    [IN]  block cache instance
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #0           sync succeded
 * @retval #int32_t       sync failed
 *
 * @par Dependency:
 * <ul><li>bcache.h</li></ul>
 */
int32_t BlockCacheSync(OsBcache *bc);

/**
 * @ingroup  bcache
 *
 * @par Description:
 * The BlockCacheInit() function shall alloc memory for bcache and init it.
 *
 * @param  devNode          [IN]  dev node instance
 * @param  sectorSize       [IN]  size of a sector
 * @param  sectorPerBlock   [IN]  sector count per block in bcache
 * @param  blockNum         [IN]  block number of bcache
 * @param  blockCount       [IN]  block count of the disk
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #OsBcache *      init succeded
 * @retval #NULL            init failed
 *
 * @par Dependency:
 * <ul><li>bcache.h</li></ul>
 */
OsBcache *BlockCacheInit(struct inode *devNode,
                         uint32_t sectorSize,
                         uint32_t sectorPerBlock,
                         uint32_t blockNum,
                         uint64_t blockCount);

/**
 * @ingroup  bcache
 *
 * @par Description:
 * The BlockCacheDeinit() function shall deinit the bcache and release resources.
 *
 * @param  bc    [IN]  block cache instance
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void None.
 *
 * @par Dependency:
 * <ul><li>bcache.h</li></ul>
 */
void BlockCacheDeinit(OsBcache *bc);

int32_t OsSdSync(int32_t id);

#ifdef CONFIG_FS_FAT_CACHE_SYNC_THREAD
void BcacheSyncThreadInit(OsBcache *bc, int32_t id);
void BcacheSyncThreadDeinit(const OsBcache *bc);
void LOS_SetDirtyRatioThreshold(uint32_t dirtyRatio);
void LOS_SetSyncThreadInterval(uint32_t interval);
int32_t LOS_SetSyncThreadPrio(uint32_t prio, const char *name);
#endif

#ifdef CONFIG_FS_FAT_CACHE_PREREAD
uint32_t BcacheAsyncPrereadInit(OsBcache *bc);
void ResumeAsyncPreread(OsBcache *arg1, const OsBcacheBlock *arg2);
uint32_t BcacheAsyncPrereadDeinit(OsBcache *bc);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */
#endif  /* _BCACHE_H */
