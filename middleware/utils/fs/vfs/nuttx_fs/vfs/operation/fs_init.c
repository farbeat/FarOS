/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
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

#include "fs/fs.h"
#include "inode/inode.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/statfs.h"
#ifdef CONFIG_DRIVER_DISK
#include "disk/disk_pri.h"
#endif
#if defined(CONFIG_FS_YAFFS) || defined(CONFIG_FS_JFFS) || defined(CONFIG_FS_ROMFS) || defined (CONFIG_FS_LITTLEFS)
#include "mtd_partition.h"
#endif

#ifdef CONFIG_SHELL
#include "vfs_cmd/vfs_shellcmd.h"
#include "shell.h"
#endif

void vfs_cmd_init(void)
{
#ifdef CONFIG_SHELL
    (VOID)OsCmdReg("lsfd", "command lsfd", (CmdCallBackFunc)OsShellCmdLsfd);
    (VOID)OsCmdReg("ls", "command ls", (CmdCallBackFunc)OsShellCmdLs);
    (VOID)OsCmdReg("pwd", "command pwd", (CmdCallBackFunc)OsShellCmdPwd);
    (VOID)OsCmdReg("cd", "command cd", (CmdCallBackFunc)OsShellCmdCd);
    (VOID)OsCmdReg("cat", "command cat", (CmdCallBackFunc)OsShellCmdCat);
    (VOID)OsCmdReg("rm", "command rm", (CmdCallBackFunc)OsShellCmdRm);
    (VOID)OsCmdReg("rmdir", "command rmdir", (CmdCallBackFunc)OsShellCmdRmdir);
    (VOID)OsCmdReg("mkdir", "command rmdmk", (CmdCallBackFunc)OsShellCmdMkdir);
#if (defined(CONFIG_FS_FAT))
    (VOID)OsCmdReg("sync", "sync", (CmdCallBackFunc)OsShellCmdSync);
#endif
#if (defined(CONFIG_FS_FAT) || defined(CONFIG_FS_RAMFS) || defined(CONFIG_FS_YAFFS) || \
     defined(CONFIG_FS_JFFS) || defined(CONFIG_FS_ROMFS) || defined(CONFIG_FS_LITTLEFS))
    (VOID)OsCmdReg("statfs", "statfs", (CmdCallBackFunc)OsShellCmdStatfs);
    (VOID)OsCmdReg("mount", "mount", (CmdCallBackFunc)OsShellCmdMount);
    (VOID)OsCmdReg("umount", "umount", (CmdCallBackFunc)OsShellCmdUmount);
#endif
#if (defined(CONFIG_FS_FAT) || defined(CONFIG_FS_RAMFS) || defined(CONFIG_FS_YAFFS) || \
     defined(CONFIG_FS_JFFS) || defined(CONFIG_FS_LITTLEFS))
    (VOID)OsCmdReg("touch", "touch", (CmdCallBackFunc)OsShellCmdTouch);
    (VOID)OsCmdReg("cp", "cp", (CmdCallBackFunc)OsShellCmdCp);
#endif
    (VOID)OsCmdReg("dd", "dd", (CmdCallBackFunc)OsShellCmdDd);
#ifdef CONFIG_DRIVER_DISK
    (VOID)OsCmdReg("partinfo", "partinfo", (CmdCallBackFunc)OsShellCmdPartInfo);
#endif
#ifdef CONFIG_FS_FAT
#ifdef CONFIG_FS_FAT_VIRTUAL_PARTITION
    (VOID)OsCmdReg("virstatfs", "virstatfs", (CmdCallBackFunc)osShellCmdVirstatfs);
#endif
    (VOID)OsCmdReg("format", "format", (CmdCallBackFunc)osShellCmdFormat);
#endif
#ifdef CONFIG_FS_MTD_PARTITION
    (VOID)OsCmdReg("partition", "partition", (CmdCallBackFunc)OsShellCmdPartitionShow);
#endif
#endif
}

dpal_spinlock g_workdir_lock;
void los_vfs_init(void)
{
  int err;
  static bool g_vfs_init = false;
  if (g_vfs_init)
    {
      return;
    }

  dpal_spin_lock_init(&g_workdir_lock);
#ifdef CONFIG_DRIVER_DISK
  dpal_spin_lock_init(&g_diskSpinlock);
  dpal_spin_lock_init(&g_diskFatBlockSpinlock);
  OsDiskInit();
#endif
#if defined(CONFIG_FS_YAFFS) || defined(CONFIG_FS_JFFS) || defined(CONFIG_FS_ROMFS) || defined (CONFIG_FS_LITTLEFS)
  OsMtdPartitionInit();
#endif
  files_initlist(&tg_filelist);
  fs_initialize();
  if ((err = inode_reserve("/dev", &g_root_inode)) < 0)
    {
      printf("los_vfs_init failed error %d\n", -err);
      return;
    }
  g_vfs_init = true;
}
