/****************************************************************************
 * fs/mount/fs_mount.c
 *
 *   Copyright (C) 2007-2009, 2011-2013, 2015, 2017-2019 Gregory Nutt. All
 *     rights reserved.
 *   Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 *
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "vfs_config.h"
#include "fatfs.h"

#include "sys/mount.h"
#include "string.h"
#include "errno.h"
#include "assert.h"
#include "debug.h"

#include "fs/fs.h"

#include "inode/inode.h"
#include "stdlib.h"
#include "driver/driver.h"
#if defined(CONFIG_FS_YAFFS) || defined(CONFIG_FS_JFFS) || defined(CONFIG_FS_ROMFS) || defined(CONFIG_FS_LITTLEFS)
#include "mtd_partition.h"
#endif
#ifdef CONFIG_FS_YAFFS
#include "mtd_nandcfg.h"
#endif
#ifdef CONFIG_FS_FAT_VIRTUAL_PARTITION
#include "errcode_fat.h"
#endif

/* At least one filesystem must be defined, or this file will not compile.
 * It may be desire-able to make file systems dynamically registered at
 * some time in the future, but at present, this file needs to know about
 * every configured filesystem.
 */

#define fs_log_err dpal_log_err

#ifdef CONFIG_FS_READABLE

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* In the canonical case, a file system is bound to a block driver.  However,
 * some less typical cases a block driver is not required.  Examples are
 * pseudo file systems (like BINFS or PROCFS) and MTD file systems (like
 * NXFFS).
 *
 * These file systems all require block drivers:
 */

#define BLKDRVR_NOT_MOUNTED 2

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mount
 *
 * Description:
 *   mount() attaches the filesystem specified by the 'source' block device
 *   name into the root file system at the path specified by 'target.'
 *
 * Returned Value:
 *   Zero is returned on success; -1 is returned on an error and errno is
 *   set appropriately:
 *
 *   EACCES A component of a path was not searchable or mounting a read-only
 *      filesystem was attempted without giving the MS_RDONLY flag.
 *   EBUSY 'source' is already  mounted.
 *   EFAULT One of the pointer arguments points outside the user address
 *      space.
 *   EINVAL 'source' had an invalid superblock.
 *   ENODEV 'filesystemtype' not configured
 *   ENOENT A pathname was empty or had a nonexistent component.
 *   ENOMEM Could not allocate a memory to copy filenames or data into.
 *   ENOTBLK 'source' is not a block device
 *
 ****************************************************************************/

int mount(FAR const char *source, FAR const char *target,
          FAR const char *filesystemtype, unsigned long mountflags,
          FAR const void *data)
{
  char *fullpath = NULL;
  char *fullpath_bak = NULL;

  FAR struct inode *drvr_inode = NULL;
  FAR struct inode *mountpt_inode;
  struct inode_search_s desc;
  const struct fsmap_t *fsmap = NULL;
  FAR const struct mountpt_operations *mops;

#if defined(CONFIG_FS_YAFFS) || defined(CONFIG_FS_JFFS) || defined(CONFIG_FS_ROMFS) || defined(CONFIG_FS_LITTLEFS)
  mtd_partition *partition = NULL;
#endif
  void *fshandle;
  int errcode = 0;
  bool target_exit = false;
  int ret;

  if (filesystemtype == NULL)
    {
      errcode = EINVAL;
      goto errout;
    }

  /* Verify required pointer arguments */

  DEBUGASSERT(target && filesystemtype);

  ret = vfs_normalize_path((const char *)NULL, target, &fullpath);
  if (ret < 0)
    {
      fs_log_err("Failed to get fullpath,target: %s\n", target);
      errcode = -ret;
      goto errout;
    }
  fullpath_bak = fullpath;

  /* Find the specified filesystem.  Try the block driver file systems first */
  fsmap = mount_findfs(filesystemtype);
  if (fsmap == NULL || (fsmap->is_bdfs && !source))
    {
      fs_log_err("Failed to find file system %s\n", filesystemtype);
      errcode = ENODEV;
      dpal_free(fullpath_bak);
      goto errout;
    }

  mops = fsmap->fs_mops;

  if (fsmap->is_bdfs && source)
    {
      /* Make sure that a block driver argument was provided */

      DEBUGASSERT(source);

      /* Find the block driver */
      ret = find_blockdriver(source, mountflags, &drvr_inode);
      if (ret < 0)
        {
          fs_log_err("Failed to find block driver %s\n", source);
          errcode = -ret;
          dpal_free(fullpath_bak);
          goto errout;
        }
    }

  /* Insert a dummy node -- we need to hold the inode semaphore
   * to do this because we will have a momentarily bad structure.
   */

  inode_semtake();

  ret = inode_reserve(fullpath, &mountpt_inode);
  if (ret < 0)
    {
      /* inode_reserve can fail for a couple of reasons, but the most
       * likely one is that the inode already exists. inode_reserve may
       * return:
       *
       *  -EINVAL - 'path' is invalid for this operation
       *  -EEXIST - An inode already exists at 'path'
       *  -ENOMEM - Failed to allocate in-memory resources for the
       *            operation
       */

      if (ret == -EEXIST)
        {
          SETUP_SEARCH(&desc, fullpath, false);
          target_exit = true;
          if (inode_search(&desc) < 0)
            {
              fs_log_err("Failed to reserve inode, %d\n", -ret);
              errcode = -ret;
              goto errout_with_semaphore;
            }
          mountpt_inode = desc.node;

          if (INODE_IS_MOUNTPT(mountpt_inode) || mountpt_inode->i_child || mountpt_inode->u.i_ops)
            {
              fs_log_err("Can't to mount to this inode, %d\n", -ret);
              errcode = -ret;
              goto errout_with_semaphore;
            }
        }
      else
        {
          fs_log_err("Failed to reserve inode, %d\n", -ret);
          errcode = -ret;
          goto errout_with_semaphore;
        }
    }

  mountpt_inode ->mountflags = mountflags;

  /* Bind the block driver to an instance of the file system.  The file
   * system returns a reference to some opaque, fs-dependent structure
   * that encapsulates this binding.
   */

  if (mops->bind == NULL)
    {
      /* The filesystem does not support the bind operation ??? */

      ferr("ERROR: Filesystem does not support bind\n");
      errcode = EINVAL;
      goto errout_with_mountpt;
    }

  /* Increment reference count for the reference we pass to the file system */

  if (drvr_inode != NULL)
    {
      drvr_inode->i_crefs++;

      /* On failure, the bind method returns -errorcode */

      if (drvr_inode->e_status != STAT_UNMOUNTED)
        {
          fdbg("ERROR: The node is busy\n");
          errcode = EBUSY;
          drvr_inode->i_crefs--;
          goto errout_with_mountpt;
        }
    }
#if defined(CONFIG_FS_YAFFS) || defined(CONFIG_FS_JFFS) || defined(CONFIG_FS_ROMFS) || defined(CONFIG_FS_LITTLEFS)
  if (fsmap->is_mtd_support && drvr_inode != NULL)
    {
      partition = (mtd_partition *)drvr_inode->i_private;
      partition->mountpoint_name = (char *)dpal_zalloc(strlen(target) + 1);
      if (partition->mountpoint_name == NULL)
        {
          errcode = ENOMEM;
          drvr_inode->i_crefs--;
          goto errout_with_mountpt;
        }

      (void)strncpy_s(partition->mountpoint_name, strlen(target) + 1, target, strlen(target));
      partition->mountpoint_name[strlen(target)] = '\0';
    }
#endif
  mountpt_inode ->mountflags = mountflags;
  ret = mops->bind(drvr_inode, data, &fshandle, fullpath_bak);
#ifdef CONFIG_FS_FAT_VIRTUAL_PARTITION
  if (ret >= VIRERR_BASE)
    {
      errcode = ret;
    }
  else
#endif
  if (ret != 0)
    {
      /* The inode is unhappy with the driver for some reason.  Back out
       * the count for the reference we failed to pass and exit with an
       * error.
       */

      ferr("ERROR: Bind method failed: %d\n", ret);
      if (drvr_inode != NULL)
        {
          drvr_inode->i_crefs--;
        }

      errcode = -ret;
#if defined(CONFIG_FS_YAFFS) || defined(CONFIG_FS_JFFS) || defined(CONFIG_FS_ROMFS) || defined(CONFIG_FS_LITTLEFS)
      if (fsmap->is_mtd_support && drvr_inode != NULL && partition != NULL)
        {
          dpal_free(partition->mountpoint_name);
          partition->mountpoint_name = NULL;
        }
#endif
      goto errout_with_mountpt;
    }

  /* We have it, now populate it with driver specific information. */
  INODE_SET_MOUNTPT(mountpt_inode);

  mountpt_inode->u.i_mops  = mops;
#ifdef CONFIG_FILE_MODE
  mountpt_inode->i_mode    = mode;
#endif
  mountpt_inode->i_private = fshandle;

  if (drvr_inode)
    {
      drvr_inode->e_status = STAT_MOUNTED;
    }
  inode_semgive();

  /* We can release our reference to the blkdrver_inode, if the filesystem
   * wants to retain the blockdriver inode (which it should), then it must
   * have called inode_addref().  There is one reference on mountpt_inode
   * that will persist until umount() is called.
   */

  if (drvr_inode != NULL)
    {
      inode_release(drvr_inode);
    }
  dpal_free(fullpath_bak);

#ifdef CONFIG_FS_FAT_VIRTUAL_PARTITION
  if (errcode >= VIRERR_BASE)
    {
      set_errno(errcode);
    }
#endif

  RELEASE_SEARCH(&desc);

  dprintf("mount outer.\n");
  return OK;

  /* A lot of goto's!  But they make the error handling much simpler */

errout_with_mountpt:
  if (target_exit == false)
    {
      (void)inode_remove(fullpath);
    }
errout_with_semaphore:
  inode_semgive();
  if (drvr_inode != NULL)
    {
      inode_release(drvr_inode);
    }
  dpal_free(fullpath_bak);
  RELEASE_SEARCH(&desc);

errout:
  set_errno(errcode);
  return VFS_ERROR;
}

#endif /* CONFIG_FS_READABLE */
