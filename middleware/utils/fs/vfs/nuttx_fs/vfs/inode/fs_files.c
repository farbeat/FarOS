/****************************************************************************
 * fs/inode/fs_files.c
 *
 *   Copyright (C) 2007-2009, 2011-2013, 2016-2017 Gregory Nutt. All rights
 *     reserved.
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

#include "sys/types.h"
#include "string.h"
#include "assert.h"
#include "errno.h"

#include "fs/fs.h"
#include "stdlib.h"
#include "inode/inode.h"
#include "nuttx.h"
#include "dpal.h"

#if CONFIG_NFILE_DESCRIPTORS > 0
struct filelist tg_filelist;
#endif

#if CONFIG_NFILE_STREAMS > 0
struct streamlist tg_streamlist;
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

struct filelist *sched_getfiles(void)
{
#if CONFIG_NFILE_DESCRIPTORS > 0
  return &tg_filelist;
#else
  return NULL;
#endif
}

/* 32: An unsigned int takes 32 bits */

static unsigned int bitmap[CONFIG_NFILE_DESCRIPTORS / 32 + 1] = {0};

static void set_bit(int i, void *addr)
{
  unsigned int tem = (unsigned int)i >> 5; /* Get the bitmap subscript */
  unsigned int *addri = (unsigned int *)addr + tem;
  unsigned int old = *addri;
  old = old | (1UL << ((unsigned int)i & 0x1f)); /* set the new map bit */
  *addri = old;
}

static void clear_bit(int i, void *addr)
{
  unsigned int tem = (unsigned int)i >> 5; /* Get the bitmap subscript */
  unsigned int *addri = (unsigned int *)addr + tem;
  unsigned int old = *addri;
  old = old & ~(1UL << ((unsigned int)i & 0x1f)); /* Clear the old map bit */
  *addri = old;
}


/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: _files_semtake
 ****************************************************************************/

static void _files_semtake(FAR struct filelist *list)
{
  int ret;

  do
    {
      /* Take the semaphore (perhaps waiting) */

      ret = nxsem_wait(&list->fl_sem);

      /* The only case that an error should occur here is if the wait was
       * awakened by a signal.
       */

      DEBUGASSERT(ret == OK || ret == -EINTR);
    }
  while (ret == -EINTR);
}

/****************************************************************************
 * Name: _files_semgive
 ****************************************************************************/

#define _files_semgive(list)  nxsem_post(&list->fl_sem)

/****************************************************************************
 * Name: _files_close
 *
 * Description:
 *   Close an inode (if open)
 *
 * Assumuptions:
 *   Caller holds the list semaphore because the file descriptor will be freed.
 *
 ****************************************************************************/

static int _files_close(FAR struct file *filep)
{
  struct inode *inode = filep->f_inode;
  int ret = OK;

  /* Check if the struct file is open (i.e., assigned an inode) */

  if (inode)
    {
      /* Close the file, driver, or mountpoint. */

      if (inode->u.i_ops && inode->u.i_ops->close)
        {
          /* Perform the close operation */

          ret = inode->u.i_ops->close(filep);
          if (ret != OK)
            {
              return ret;
            }
        }

      /* And release the inode */

      inode_release(inode);

      /* Release the path of file */

      dpal_free(filep->f_path);

      /* Release the file descriptor */

      filep->f_oflags  = 0;
      filep->f_pos     = 0;
      filep->f_path    = NULL;
      filep->f_priv    = NULL;
      filep->f_inode   = NULL;
    }

  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: files_initialize
 *
 * Description:
 *   This is called from the FS initialization logic to configure the files.
 *
 ****************************************************************************/

void files_initialize(void)
{
}

/****************************************************************************
 * Name: files_initlist
 *
 * Description: Initializes the list of files for a new task
 *
 ****************************************************************************/

void files_initlist(FAR struct filelist *list)
{
  DEBUGASSERT(list);

  /* Initialize the list access mutex */

  int ret = nxsem_init(&list->fl_sem, 0, 1);
  if (ret != DPAL_OK) {
      printf("nxsem_init filelist fail.\n");
  }
}

/****************************************************************************
 * Name: files_releaselist
 *
 * Description:
 *   Release a reference to the file list
 *
 ****************************************************************************/

void files_releaselist(FAR struct filelist *list)
{
  int i;

  DEBUGASSERT(list);

  /* Close each file descriptor .. Normally, you would need take the list
   * semaphore, but it is safe to ignore the semaphore in this context because
   * there should not be any references in this context.
   */

  for (i = 0; i < CONFIG_NFILE_DESCRIPTORS; i++)
    {
      (void)_files_close(&list->fl_files[i]);
    }

  /* Destroy the semaphore */

  (void)nxsem_destroy(&list->fl_sem);
}

/****************************************************************************
 * Name: file_dup2
 *
 * Description:
 *   Assign an inode to a specific files structure.  This is the heart of
 *   dup2.
 *
 *   Equivalent to the non-standard fs_dupfd2() function except that it
 *   accepts struct file instances instead of file descriptors and it does
 *   not set the errno variable.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is return on
 *   any failure.
 *
 ****************************************************************************/

int file_dup2(FAR struct file *filep1, FAR struct file *filep2)
{
  FAR struct filelist *list = NULL;
  FAR struct inode *inode;
  char *fullpath = NULL;
  struct inode_search_s desc;
  size_t len;
  int ret;

  if (!filep1 || !filep1->f_inode || !filep2 || !filep1->f_path)
    {
      return -EBADF;
    }

  list = sched_getfiles();

  /* The file list can be NULL under two cases:  (1) One is an obscure
   * cornercase:  When memory management debug output is enabled.  Then
   * there may be attempts to write to stdout from malloc before the group
   * data has been allocated.  The other other is (2) if this is a kernel
   * thread.  Kernel threads have no allocated file descriptors.
   */

  if (list != NULL)
    {
      _files_semtake(list);
    }

  /* If there is already an inode contained in the new file structure,
   * close the file and release the inode.
   */

  ret = _files_close(filep2);
  if (ret < 0)
    {
      /* An error occurred while closing the driver */

      goto errout_with_ret;
    }

  len = strlen(filep1->f_path);
  if ((len == 0) || (len >= PATH_MAX))
    {
      ret = -EINVAL;
      goto errout_with_ret;
    }

  fullpath = (char *)dpal_malloc(len + 1);
  if (fullpath == NULL)
    {
      ret = -ENOMEM;
      goto errout_with_ret;
    }

  (void)memset_s(fullpath, len + 1, 0, len + 1);

  /* Increment the reference count on the contained inode */

  inode = filep1->f_inode;

  /* Then clone the file structure */

  filep2->f_oflags = filep1->f_oflags;
  filep2->f_pos    = filep1->f_pos;
  filep2->f_inode  = inode;
  filep2->f_priv   = filep1->f_priv;

  (void)strncpy_s(fullpath, len + 1, filep1->f_path, len);

  SETUP_SEARCH(&desc, fullpath, true);
  ret = inode_find(&desc);
  if (ret < 0){
      ret = -ret;
      dpal_free(fullpath);
      goto errout_with_inode;
    }

  filep2->f_path = fullpath;
  filep2->f_relpath = desc.relpath;

  /* Call the open method on the file, driver, mountpoint so that it
   * can maintain the correct open counts.
   */

  if (inode->u.i_ops)
    {
#ifndef CONFIG_DISABLE_MOUNTPOINT
      if (INODE_IS_MOUNTPT(inode))
        {
          /* Dup the open file on the in the new file structure */

          if (inode->u.i_mops->dup)
            {
              ret = inode->u.i_mops->dup(filep1, filep2);
            }
          else
            {
              ret = -ENOSYS;
            }
        }
      else
#endif
        {
          /* (Re-)open the pseudo file or device driver */

          if (inode->u.i_ops->open)
            {
              ret = inode->u.i_ops->open(filep2);
            }
          else
            {
              ret = -ENOSYS;
            }
        }

      /* Handle open failures */

      if (ret < 0)
        {
          goto errout_with_path;
        }
    }

  if (list != NULL)
    {
      _files_semgive(list);
    }
  RELEASE_SEARCH(&desc);

  return OK;

  /* Handle various error conditions */

errout_with_path:
  dpal_free(filep2->f_path);
  filep2->f_path    = NULL;
  filep2->f_relpath = NULL;

errout_with_inode:
  inode_release(filep2->f_inode);
  RELEASE_SEARCH(&desc);
  filep2->f_oflags = 0;
  filep2->f_pos    = 0;
  filep2->f_inode  = NULL;
  filep2->f_priv   = NULL;

errout_with_ret:
  _files_semgive(list);

  return ret;
}

#define FILE_START_FD 3

/****************************************************************************
 * Name: files_allocate
 *
 * Description:
 *   Allocate a struct files instance and associate it with an inode instance.
 *   Returns the file descriptor == index into the files array.
 *
 ****************************************************************************/

int files_allocate(FAR struct inode *inode, int oflags, off_t pos, void* priv, int minfd)
{
  FAR struct filelist *list = NULL;
  unsigned int *p = NULL;
  unsigned int mask;
  unsigned int i;

  /* minfd should be a positive number,and 0,1,2 had be distributed to stdin,stdout,stderr */

  if (minfd < FILE_START_FD)
    {
      minfd = FILE_START_FD;
    }
  i = (unsigned int)minfd;

  /* Get the file descriptor list.  It should not be NULL in this context. */

  list = sched_getfiles();
  DEBUGASSERT(list != NULL);

  _files_semtake(list);

  while (i < CONFIG_NFILE_DESCRIPTORS)
    {
      p = ((unsigned int *)bitmap) + (i >> 5); /* Gets the location in the bitmap */
      mask = 1 << (i & 0x1f); /* Gets the mask for the current bit int bitmap */
      if ((~(*p) & mask))
        {
          set_bit(i, bitmap);
          list->fl_files[i].f_oflags = oflags;
          list->fl_files[i].f_pos    = pos;
          list->fl_files[i].f_inode  = inode;
          list->fl_files[i].f_priv   = priv;
          _files_semgive(list);
          return (int)i;
        }
      i++;
    }

  _files_semgive(list);
  return VFS_ERROR;
}

/****************************************************************************
 * Name: files_close
 *
 * Description:
 *   Close an inode (if open)
 *
 * Assumuptions:
 *   Caller holds the list semaphore because the file descriptor will be freed.
 *
 ****************************************************************************/

int files_close(int fd)
{
  FAR struct filelist *list;
  int                  ret;

  /* Get the thread-specific file list.  It should never be NULL in this
   * context.
   */

  list = sched_getfiles();
  DEBUGASSERT(list != NULL);

  /* If the file was properly opened, there should be an inode assigned */

  if (fd < 0 || fd >= CONFIG_NFILE_DESCRIPTORS || !list->fl_files[fd].f_inode)
    {
      return -EBADF;
    }

  /* Perform the protected close operation */

  _files_semtake(list);
  ret = _files_close(&list->fl_files[fd]);
  if (ret == OK)
    {
      clear_bit(fd, bitmap);
    }
  _files_semgive(list);
  return ret;
}

/****************************************************************************
 * Name: files_release
 *
 * Assumuptions:
 *   Similar to files_close().  Called only from open() logic on error
 *   conditions.
 *
 ****************************************************************************/

void files_release(int fd)
{
  FAR struct filelist *list;

  list = sched_getfiles();
  DEBUGASSERT(list);

  if (fd >= 0 && fd < CONFIG_NFILE_DESCRIPTORS)
    {
      _files_semtake(list);
      list->fl_files[fd].f_oflags  = 0;
      list->fl_files[fd].f_pos     = 0;
      list->fl_files[fd].f_inode = NULL;
      clear_bit(fd, bitmap);
      _files_semgive(list);
    }
}

struct inode * files_get_openfile(int fd)
{
  FAR struct filelist *list = NULL;
  unsigned int *p = NULL;
  unsigned int mask;

  list = sched_getfiles();
  DEBUGASSERT(list);

  p = ((unsigned int *)bitmap) + ((unsigned int)fd >> 5); /* Gets the location in the bitmap */
  mask = 1 << ((unsigned int)fd & 0x1f); /* Gets the mask for the current bit int bitmap */
  if ((~(*p) & mask))
    {
      return NULL;
    }

  return list->fl_files[fd].f_inode;
}

void clear_fd(int fd)
{
  clear_bit(fd, bitmap);
}

int close_files(const struct inode *inode)
{
  int fd = 0;
  int ret = 0;
  FAR struct inode *open_file_inode = NULL;

  for (fd = FILE_START_FD; fd < CONFIG_NFILE_DESCRIPTORS; fd++)
    {
      open_file_inode = files_get_openfile(fd);
      if (open_file_inode && (open_file_inode == inode))
        {
          ret = files_close(fd);
          if (ret < 0)
            {
              return -EBUSY;
            }
        }
    }

  return 0;
}


