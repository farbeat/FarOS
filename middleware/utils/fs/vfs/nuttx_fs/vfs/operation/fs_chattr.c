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

#include "errno.h"
#include "fs/fs.h"
#include "sys/stat.h"
#include "inode/inode.h"
#include "string.h"
#include "stdlib.h"
#include <assert.h>
#include "dpal.h"

/****************************************************************************
 * Static Functions
 ****************************************************************************/

static int chattr_pseudo(struct inode *pinode, mode_t mode)
{
  return ENOSYS;
}

/****************************************************************************
 * Name: chattr
 *
 * Returned Value:
 *   Zero on success; -1 on failure with errno set:
 *
 ****************************************************************************/

int chattr(const char *path, mode_t mode)
{
  struct inode_search_s desc;
  struct inode *pinode = NULL;
  char *fullpath = NULL;
  int ret;

  ret = vfs_normalize_path((const char *)NULL, path, &fullpath);
  if (ret < 0)
    {
      ret = -ret;
      goto errout;
    }

  SETUP_SEARCH(&desc, fullpath, false);

  ret = inode_find(&desc);
  if (ret < 0) {
    ret = -ret;
    goto errout_with_search;
  }

  pinode = desc.node;
  DEBUGASSERT(pinode != NULL);

#ifndef CONFIG_DISABLE_MOUNTPOINT

  /* Check if the inode is a valid mountpoint. */

  if (INODE_IS_MOUNTPT(pinode) && pinode->u.i_mops)
    {
      if (pinode->u.i_mops->chattr)
        {
          if (!strlen(desc.relpath))
            {
              ret = EPERM;
              goto errout_with_inode;
            }

          ret = pinode->u.i_mops->chattr(pinode, desc.relpath, mode);
          if (ret < 0)
            {
              ret = -ret;
              goto errout_with_inode;
            }
        }
      else
        {
          ret = ENOSYS;
          goto errout_with_inode;
        }
    }
  else
#endif
    {
      ret = chattr_pseudo(pinode, mode);
      goto errout_with_inode;
    }
  inode_release(pinode);
  dpal_free(fullpath);
  return OK;

errout_with_inode:
  inode_release(pinode);

errout_with_search:
  RELEASE_SEARCH(&desc);
  dpal_free(fullpath);

errout:
  set_errno(ret);
  return VFS_ERROR;
}
