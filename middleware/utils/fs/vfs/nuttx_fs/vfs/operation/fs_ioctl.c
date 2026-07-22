/****************************************************************************
 * fs/vfs/operation/fs_ioctl.c
 *
 *   Copyright (C) 2007-2010, 2012-2014, 2016-2017 Gregory Nutt. All rights
 *     reserved.
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
#include "sched.h"
#include "assert.h"
#include "errno.h"
#include "unistd.h"

#if defined(CONFIG_NET_LWIP_SACK)
# include "net/net.h"
#include "lwip/sockets.h"
#endif

#include "inode/inode.h"

int ConsoleUpdateFd(void) __attribute__((weak));

#ifdef __LP64__
typedef unsigned long UINTPTR;
#else
typedef unsigned int UINTPTR;
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: file_ioctl
 *
 * Description:
 *   Perform device specific operations.
 *
 * Input Parameters:
 *   file     File structure instance
 *   req      The ioctl command
 *   arg      The argument of the ioctl cmd
 *
 * Returned Value:
 *   Returns a non-negative number on success;  A negated errno value is
 *   returned on any failure (see comments ioctl() for a list of appropriate
 *   errno values).
 *
 ****************************************************************************/

int file_ioctl(FAR struct file *filep, int req, unsigned long arg)
{
  FAR struct inode *inode;

  DEBUGASSERT(filep != NULL);

  /* Is a driver opened? */

  inode = filep->f_inode;
  if (!inode)
    {
      return -EBADF;
    }

  /* Does the driver support the ioctl method? */

  if (inode->u.i_ops == NULL || inode->u.i_ops->ioctl == NULL)
    {
      return -ENOTTY;
    }

  /* Yes on both accounts.  Let the driver perform the ioctl command */

  return (int)inode->u.i_ops->ioctl(filep, req, arg);
}

/****************************************************************************
 * Name: ioctl/fs_ioctl
 *
 * Description:
 *   Perform device specific operations.
 *
 * Input Parameters:
 *   fd       File/socket descriptor of device
 *   req      The ioctl command
 *   arg      The argument of the ioctl cmd
 *
 * Returned Value:
 *   >=0 on success (positive non-zero values are cmd-specific)
 *   -1 on failure with errno set properly:
 *
 *   EBADF
 *     'fd' is not a valid descriptor.
 *   EFAULT
 *     'arg' references an inaccessible memory area.
 *   EINVAL
 *     'cmd' or 'arg' is not valid.
 *   ENOTTY
 *     'fd' is not associated with a character special device.
 *   ENOTTY
 *      The specified request does not apply to the kind of object that the
 *      descriptor 'fd' references.
 *
 ****************************************************************************/

#ifdef CONFIG_LIBC_IOCTL_VARIADIC
int fs_ioctl(int fd, int req, ...)
#else
int ioctl(int fd, int req, ...)
#endif
{
  int errcode;
  UINTPTR arg = 0;
  va_list ap;
  FAR struct file *filep;
  int ret;

  va_start(ap, req);
  arg = va_arg(ap, UINTPTR);
  va_end(ap);

  /* Did we get a valid file descriptor? */

  if ((unsigned int)fd >= CONFIG_NFILE_DESCRIPTORS)
    {
      /* Perform the socket ioctl */

#if defined(CONFIG_NET_LWIP_SACK)
      if ((unsigned int)fd < (CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS))
        {
          return lwip_ioctl(fd, (long)req, (void *)arg);
        }
      else
#endif
        {
          errcode = EBADF;
          goto errout;
        }
    }

  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO && ConsoleUpdateFd != NULL) /* fd : [0,2] */
    {
      fd = ConsoleUpdateFd();
      if (fd < 0)
        {
          errcode = EBADF;
          goto errout;
        }
    }

  /* Get the file structure corresponding to the file descriptor. */

  ret = fs_getfilep(fd, &filep);
  if (ret < 0)
    {
      errcode = -ret;
      goto errout;
    }

  DEBUGASSERT(filep != NULL);

  /* Perform the file ioctl.  If file_ioctl() fails, it will set the errno
   * value appropriately.
   */

  ret = file_ioctl(filep, req, arg);
  if (ret < 0)
    {
      set_errno(-ret);
      return ret;
    }

  return ret;

errout:
  set_errno(errcode);
  return VFS_ERROR;
}
