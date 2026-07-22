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

#ifndef _DCACHE_DENTRY_H
#define _DCACHE_DENTRY_H

#include "hash.h"
#include "fcntl.h"
#include "time.h"
#include "sys/stat.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define DENTRY_SHORT_NAME_LEN   32

enum DentryTime {
    ATIME = 1 << 0,
    MTIME = 1 << 1,
    CTIME = 1 << 2
};

typedef struct LosDentry {
    union {
        struct LosDentry *parent;
        HashTable *table;   /* Root dentry need not parent */
    };

    /* inode struct */
    char *autoName;
    char name[DENTRY_SHORT_NAME_LEN];
    mode_t            mode;     /* Access mode flags */
    loff_t            size;
    unsigned long     blkSize;
    struct timespec   atime;
    struct timespec   mtime;
    struct timespec   ctime;
} LosDentry;

#define HASH_TABLE_FLAG                     0x1
#define DENTRY_ENTRY_SHIFT                  10 /* 1024 dentries */
#define DENTRY_TABLE_SHIFT                  8  /* 256 hash entries */
#define HASH_ROOT_PARENT_DENTRY(dentry)     ((UINTPTR)(dentry) & HASH_TABLE_FLAG)
#define HASH_TABLE_GET(dentry)              (HashTable *)((UINTPTR)(dentry) & (~HASH_TABLE_FLAG))

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _DCACHE_DENTRY_H */
