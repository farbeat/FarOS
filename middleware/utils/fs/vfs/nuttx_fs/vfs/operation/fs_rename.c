/****************************************************************************
 * fs/vfs/operation/fs_rename.c
 *
 *   Copyright (C) 2007-2009, 2014, 2017 Gregory Nutt. All rights reserved.
 *   Copyright (c) Huawei Technologies Co., Ltd. 2014-2019. All rights reserved.
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

#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "assert.h"

#include "fs/fs.h"
#include "inode/inode.h"
#include "dpal.h"

#undef FS_HAVE_WRITABLE_MOUNTPOINT
#if !defined(CONFIG_DISABLE_MOUNTPOINT) && defined(CONFIG_FS_WRITABLE) && \
    CONFIG_NFILE_STREAMS > 0
#define FS_HAVE_WRITABLE_MOUNTPOINT 1
#endif

#undef FS_HAVE_PSEUDOFS_OPERATIONS
#if !defined(CONFIG_DISABLE_PSEUDOFS_OPERATIONS) && CONFIG_NFILE_STREAMS > 0
#define FS_HAVE_PSEUDOFS_OPERATIONS 1
#endif

#undef FS_HAVE_RENAME
#if defined(FS_HAVE_WRITABLE_MOUNTPOINT) || defined(FS_HAVE_PSEUDOFS_OPERATIONS)
#define FS_HAVE_RENAME 1
#endif

#ifdef FS_HAVE_RENAME

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pseudorename
 *
 * Description:
 *   Rename an inode in the pseudo file system
 *
 ****************************************************************************/

#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS

static char *fs_basename(char *path)
{
    static const char empty[] = ".";
    char *first = (char *)empty;
    register char *last = NULL;

    if ((path != NULL) && *path) {
        first = (char *)path;
        last = (char *)path - 1;

        do {
            if ((*path != '/') && (path > ++last)) {
                last = first = (char *)path;
            }
        } while (*++path);

        if (*first == '/') {
            last = first;
        }
        last[1] = '\0';
    }

    return first;
}

static int pseudorename(FAR const char *oldpath, FAR struct inode *oldinode,
                        FAR const char *newpath)
{
  FAR struct inode *newinode;
  FAR char *subdir = NULL;
  FAR const char *name;
  int ret;

  /* Special case the root directory.  There is no root inode and there is
   * no name for the root.  inode_find() will fail to the find the root
   * inode -- because there isn't one.
   */

  name = newpath;
  while (*name == '/')
    {
      name++;
    }

  if (*name == '\0')
    {
      FAR char *subdirname;

      /* In the newpath is the root directory, the target of the rename must
       * be a directory entry under the root.
       */

      subdirname = fs_basename((FAR char *)oldpath);

      (void)asprintf(&subdir, "/%s", subdirname);
      if (subdir == NULL)
        {
          ret = -ENOMEM;
          goto errout;
        }

      newpath = subdir;
    }


  /* Create a new, empty inode at the destination location.
   * NOTE that the new inode will be created with a reference count
   * of  zero.
   */

  inode_semtake();

  ret = inode_reserve(newpath, &newinode);
  if (ret < 0)
    {
      /* It is an error if a node at newpath already exists in the tree
       * OR if we fail to allocate memory for the new inode (and possibly
       * any new intermediate path segments).
       */

      ret = -EEXIST;
      goto errout_with_sem;
    }

  /* Copy the inode state from the old inode to the newly allocated inode */

  newinode->i_child   = oldinode->i_child;   /* Link to lower level inode */
  newinode->i_flags   = oldinode->i_flags;   /* Flags for inode */
  newinode->u.i_ops   = oldinode->u.i_ops;   /* Inode operations */
#ifdef CONFIG_FILE_MODE
  newinode->i_mode    = oldinode->i_mode;    /* Access mode flags */
#endif
  newinode->i_private = oldinode->i_private; /* Per inode driver private data */

#ifdef CONFIG_PSEUDOFS_SOFTLINKS
  /* Prevent the link target string from being deallocated.  The pointer to
   * the allocated link target path was copied above (under the guise of
   * u.i_ops).  Now we must nullify the u.i_link pointer so that it is not
   * deallocated when inode_free() is (eventually called.
   */

  oldinode->u.i_link  = NULL;
#endif

  /* We now have two copies of the inode.  One with a reference count of
   * zero (the new one), and one that may have multiple references
   * including one by this logic (the old one)
   *
   * Remove the old inode.  Because we hold a reference count on the
   * inode, it will not be deleted now.  It will be deleted when all of
   * the references to the inode have been released (perhaps when
   * inode_release() is called in remove()).  inode_remove() should return
   * -EBUSY to indicate that the inode was not deleted now.
   */

  ret = inode_remove(oldpath);
  if (ret < 0 && ret != -EBUSY)
    {
      /* Remove the new node we just recreated */

      (void)inode_remove(newpath);
      goto errout_with_sem;
    }

  /* Remove all of the children from the unlinked inode */

  oldinode->i_child = NULL;
  ret = OK;

errout_with_sem:
  inode_semgive();

errout:
  if (subdir != NULL)
    {
      kmm_free(subdir);
    }

  return ret;
}

#endif /* CONFIG_DISABLE_PSEUDOFS_OPERATIONS */

/****************************************************************************
 * Name: mountptrename
 *
 * Description:
 *   Rename a file residing on a mounted volume.
 *
 ****************************************************************************/

#ifndef CONFIG_DISABLE_MOUNTPOINT
static int mountptrename(FAR const char *oldpath, FAR struct inode *oldinode,
                         FAR const char *oldrelpath, FAR const char *newpath)
{
  struct inode_search_s newdesc;
  FAR struct inode *newinode;
  FAR const char *newrelpath;
  int ret;

  (void)oldpath;
  DEBUGASSERT(oldinode->u.i_mops);

  /* If the file system does not support the rename() method, then bail now.
   * As of this writing, only NXFFS does not support the rename method.  A
   * good fallback might be to copy the oldrelpath to the correct location,
   * then unlink it.
   */

  if (oldinode->u.i_mops->rename == NULL)
    {
      return -ENOSYS;
    }

  /* Get an inode for the new relpath -- it should lie on the same
   * mountpoint
   */

  SETUP_SEARCH(&newdesc, newpath, true);

  ret = inode_find(&newdesc);
  if (ret < 0)
    {
      /* There is no mountpoint that includes in this path */

      goto errout_with_newsearch;
    }

  /* Get the search results */

  newinode   = newdesc.node;
  newrelpath = newdesc.relpath;
  DEBUGASSERT(newinode != NULL && newrelpath != NULL);

  /* Verify that the two paths lie on the same mountpoint inode */

  if (oldinode != newinode)
    {
      ret = -EXDEV;
      goto errout_with_newinode;
    }


  /* Just declare success of the oldrepath and the newrelpath point to
   * the same directory entry.  That directory entry should have been
   * stat'ed above to assure that it exists.
   */

  ret = OK;

  /* Perform the rename operation using the relative paths at the common
   * mountpoint.
   */

  ret = oldinode->u.i_mops->rename(oldinode, oldrelpath, newrelpath);

errout_with_newinode:
  inode_release(newinode);

errout_with_newsearch:
  RELEASE_SEARCH(&newdesc);
  return ret;
}
#endif /* CONFIG_DISABLE_MOUNTPOINT */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rename
 *
 * Description:
 *   Rename a file or directory.
 *
 ****************************************************************************/

int rename(FAR const char *oldpath, FAR const char *newpath)
{
  struct inode_search_s olddesc;
  char             *fulloldpath;
  char             *fulloldpath_bak;
  FAR struct inode *oldinode;
  char             *fullnewpath;
  char             *fullnewpath_bak;
  int ret;

  /* Ignore paths that are interpreted as the root directory which has no name
   * and cannot be moved
   */

  if (!oldpath || *oldpath == '\0' ||
      !newpath || *newpath == '\0')
    {
      ret = -EINVAL;
      goto errout;
    }

  ret = vfs_normalize_path((const char *)NULL, oldpath, &fulloldpath);
  if (ret < 0)
   {
     goto errout;
   }
  fulloldpath_bak = fulloldpath;

  ret = vfs_normalize_path((const char *)NULL, newpath, &fullnewpath);
  if (ret < 0)
   {
     goto errout_with_oldpath;
   }
  fullnewpath_bak = fullnewpath;

  /* Get an inode that includes the oldpath */

  SETUP_SEARCH(&olddesc, fulloldpath, true);

  ret = inode_find(&olddesc);
  if (ret < 0)
    {
      /* There is no inode that includes in this path */

      goto errout_with_oldsearch;
    }

  /* Get the search results */

  oldinode = olddesc.node;
  DEBUGASSERT(oldinode != NULL);

#ifndef CONFIG_DISABLE_MOUNTPOINT
  /* Verify that the old inode is a valid mountpoint. */

  if (INODE_IS_MOUNTPT(oldinode))
    {
      ret = mountptrename(oldpath, oldinode, olddesc.relpath, fullnewpath);
    }
  else
#endif /* CONFIG_DISABLE_MOUNTPOINT */
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
    {
      ret = pseudorename(fulloldpath, oldinode, fullnewpath);
    }
#else
    {
      ret = -ENXIO;
    }
#endif

  inode_release(oldinode);

errout_with_oldsearch:
  RELEASE_SEARCH(&olddesc);
  dpal_free(fullnewpath_bak);
errout_with_oldpath:
  dpal_free(fulloldpath_bak);
errout:
  if (ret < 0)
    {
      set_errno(-ret);
      return VFS_ERROR;
    }

  return OK;
}

#endif /* FS_HAVE_RENAME */
