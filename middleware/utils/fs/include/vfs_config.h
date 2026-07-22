/* ----------------------------------------------------------------------------
 * Copyright (c) <2013-2019> @CompanyNameMagicTag
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
 * --------------------------------------------------------------------------- */


#ifndef _VFS_CONFIG_H_
#define _VFS_CONFIG_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define CONFIG_DISABLE_MQUEUE   // disable posix mqueue inode configure

/* file system configur */

#define CONFIG_FS_WRITABLE      // enable file system can be written
#define CONFIG_FS_READABLE      // enable file system can be read
#define CONFIG_DEBUG_FS         // enable vfs debug function


/* yaffs and fatfs cache configur */
/* config block size for fat file system, only can be 0,32,64,128,256,512,1024 */

#define CONFIG_FS_FAT_SECTOR_PER_BLOCK  32

/* config block num for fat file system */

#ifdef CONFIG_PSRAM_SUPPORT
#define CONFIG_FS_FAT_BLOCK_NUMS        16
#else
#define CONFIG_FS_FAT_BLOCK_NUMS        8
#endif


#ifdef CONFIG_FS_FAT_CACHE_SYNC_THREAD

/* config the priority of sync task */

#define CONFIG_FS_FAT_SYNC_THREAD_PRIO 10

/* config dirty ratio of bcache for fat file system */

#define CONFIG_FS_FAT_DIRTY_RATIO      80

/* config time interval of sync thread for fat file system, in milliseconds */

#define CONFIG_FS_FAT_SYNC_INTERVAL    5000
#endif

#define CONFIG_FS_FLASH_BLOCK_NUM 1

/* page numbers of per yaffs block */

#define CONFIG_FS_YLIB_PAGE_PER_BLOCK (64 * CONFIG_FS_FLASH_BLOCK_NUM)

/* config erase size for yaffs file system */

#define CONFIG_FS_YAFFS_BLOCK_ERASE_SIZE (2048 * CONFIG_FS_YLIB_PAGE_PER_BLOCK)

/* config block size for yaffs file system */

#define CONFIG_FS_YAFFS_BLOCK_SIZE  ((2048 + 64) * CONFIG_FS_YLIB_PAGE_PER_BLOCK)

/* config cache size for yaffs file system */

#define CONFIG_FS_YAFFS_BLOCK_CACHE_MEMSIZE (16*CONFIG_FS_YAFFS_BLOCK_SIZE)


/* nfs configure */

#define CONFIG_NFS_MACHINE_NAME "IPC"   // nfs device name is IPC
#define CONFIG_NFS_MACHINE_NAME_SIZE 3  // size of nfs machine name


/* file descriptors configure */

#define CONFIG_NFILE_STREAMS        1   // enable file stream
#define CONFIG_STDIO_BUFFER_SIZE    0
#define CONFIG_NUNGET_CHARS         0

#ifndef FD_SETSIZE
#define FD_SETSIZE  512  // total fds
#endif

/* net configure */

#ifdef CONFIG_NET_LWIP_SACK             // enable socket and net function
#include "lwip/opt.h"
#define CONFIG_NSOCKET_DESCRIPTORS LWIP_CONFIG_NUM_SOCKETS  // max numbers of socket descriptor

/* max numbers of other descriptors except socket descriptors */
#define CONFIG_NET_SENDFILE         1   // enable sendfile function
#define CONFIG_NET_TCP              1   // enable sendfile and send function
#else
#define CONFIG_NSOCKET_DESCRIPTORS  0
#define CONFIG_NET_SENDFILE         0   // disable sendfile function
#define CONFIG_NET_TCP              0   // disable sendfile and send function
#endif

#ifndef CONFIG_NFILE_DESCRIPTORS
#define CONFIG_NFILE_DESCRIPTORS    128
#endif
/* directory configure */

#define VFS_USING_WORKDIR               // enable current working directory

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
