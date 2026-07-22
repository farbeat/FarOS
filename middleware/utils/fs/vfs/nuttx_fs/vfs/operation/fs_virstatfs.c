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


/****************************************************************************
* Included Files
****************************************************************************/

#include "vfs_config.h"

#include "sys/statfs.h"
#include "string.h"
#include "sched.h"

#include "inode/inode.h"
#include "errno.h"
#include "stdlib.h"
#include <assert.h>
/****************************************************************************
* Private Functions
****************************************************************************/

/****************************************************************************
* Name: statpseudo
****************************************************************************/

#ifdef CONFIG_FS_FAT_VIRTUAL_PARTITION
int virstatfs(FAR const char *path, FAR struct statfs *buf)
{
  struct inode_search_s desc;
  FAR struct inode *inode;
  int              ret       = OK;
  char             *fullpath = NULL;

  /* Sanity checks */

  if (!path || !buf)
    {
      ret = EFAULT;
      goto errout;
    }

  if (!path[0])
    {
      ret = ENOENT;
      goto errout;
    }

  ret = vfs_normalize_path((const char *)NULL, path, &fullpath);
  if (ret < 0)
    {
      ret = -ret;
      goto errout;
    }

  /* Get an inode for this file */

  SETUP_SEARCH(&desc, fullpath, false);

  ret = inode_find(&desc);
  if (ret < 0)
    {
      /* This name does not refer to an inode in the pseudo file system and
       * there is no mountpoint that includes in this path.
       */

      ret = -ret;
      goto errout_with_search;
    }

  /* Get the search results */

  inode = desc.node;
  DEBUGASSERT(inode != NULL);

  /* The way we handle the statfs depends on the type of inode that we
   * are dealing with.
   */

#ifndef CONFIG_DISABLE_MOUNTPOINT
  if (INODE_IS_MOUNTPT(inode))
    {
      /* The node is a file system mointpoint. Verify that the mountpoint
      * supports the statfs() method
      */

      if (inode->u.i_mops && inode->u.i_mops->virstatfs)
        {
          /* Perform the statfs() operation */

          ret = inode->u.i_mops->virstatfs(inode, desc.relpath, buf);
        }
      else
        {
          ret = EINVAL;
          goto errout_with_inode;
        }
    }
  else
#endif
    {
      ret = EINVAL;
      goto errout_with_inode;
    }

  /* Check if the statfs operation was successful */

  if (ret < 0)
    {
      ret = -ret;
      goto errout_with_inode;
    }

  /* Successfully statfs'ed the file */

  inode_release(inode);
  dpal_free(fullpath);
  return OK;

  /* Failure conditions always set the errno appropriately */

errout_with_inode:
  inode_release(inode);

errout_with_search:
  RELEASE_SEARCH(&desc);
  dpal_free(fullpath);

errout:
  set_errno(ret);
  return VFS_ERROR;
}
#endif
