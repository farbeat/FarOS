/****************************************************************************
 * include/fs/file.h
 *
 *   Copyright (C) 2007-2009, 2011-2013 Gregory Nutt. All rights reserved.
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

/**@defgroup fs Filesystem
 *
 */


#ifndef __INCLUDE_FS_FILE_H
#define __INCLUDE_FS_FILE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "vfs_config.h"
#include "compiler.h"

#include "sys/types.h"
#include "stdarg.h"
#include "stdint.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

struct filelist *sched_getfiles(void);

/* fs_inode.c ***************************************************************/
/****************************************************************************
 * Name: fs_initialize
 *
 * Description:
 *   This is called from the OS initialization logic to configure the file
 *   system.
 *
 ****************************************************************************/

void fs_initialize(void);

/****************************************************************************
 * Name: files_initlist
 *
 * Description:
 *   Initializes the list of files for a new task
 *
 ****************************************************************************/

void files_initlist(FAR struct filelist *list);

/****************************************************************************
 * Name: files_releaselist
 *
 * Description:
 *   Release a reference to the file list
 *
 ****************************************************************************/

void files_releaselist(FAR struct filelist *list);

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

int file_dup2(FAR struct file *filep1, FAR struct file *filep2);

/****************************************************************************
 * Name: fs_dupfd OR dup
 *
 * Description:
 *   Clone a file descriptor 'fd' to an arbitrary descriptor number (any value
 *   greater than or equal to 'minfd'). If socket descriptors are
 *   implemented, then this is called by dup() for the case of file
 *   descriptors.  If socket descriptors are not implemented, then this
 *   function IS dup().
 *
 *   This alternative naming is used when dup could operate on both file and
 *   socket descriptors to avoid drawing unused socket support into the link.
 *
 * Returned Value:
 *   fs_dupfd is sometimes an OS internal function and sometimes is a direct
 *   substitute for dup().  So it must return an errno value as though it
 *   were dup().
 *
 ****************************************************************************/

int fs_dupfd(int fd, int minfd);

/****************************************************************************
 * Name: file_dup
 *
 * Description:
 *   Equivalent to the non-standard fs_dupfd() function except that it
 *   accepts a struct file instance instead of a file descriptor and does
 *   not set the errno variable.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int file_dup(FAR struct file *filep, int minfd);

/****************************************************************************
 * Name: fs_dupfd2 OR dup2
 *
 * Description:
 *   Clone a file descriptor to a specific descriptor number. If socket
 *   descriptors are implemented, then this is called by dup2() for the
 *   case of file descriptors.  If socket descriptors are not implemented,
 *   then this function IS dup2().
 *
 *   This alternative naming is used when dup2 could operate on both file and
 *   socket descriptors to avoid drawing unused socket support into the link.
 *
 * Returned Value:
 *   fs_dupfd2 is sometimes an OS internal function and sometimes is a direct
 *   substitute for dup2().  So it must return an errno value as though it
 *   were dup2().
 *
 ****************************************************************************/

int fs_dupfd2(int fd1, int fd2);

/****************************************************************************
 * Name: file_open
 *
 * Description:
 *   file_open() is similar to the standard 'open' interface except that it
 *   returns an instance of 'struct file' rather than a file descriptor.  It
 *   also is not a cancellation point and does not modify the errno variable.
 *
 * Input Parameters:
 *   filep  - The caller provided location in which to return the 'struct
 *            file' instance.
 *   path   - The full path to the file to be open.
 *   oflags - open flags
 *   ...    - Variable number of arguments, may include 'mode_t mode'
 *
 * Returned Value:
 *   Zero (OK) is returned on success.  On failure, a negated errno value is
 *   returned.
 *
 ****************************************************************************/

int file_open(FAR struct file *filep, FAR const char *path, int oflags, ...);

/****************************************************************************
 * Name: file_close
 *
 * Description:
 *   Close a file that was previously opened with file_open() (or detached
 *   with file_detach()).
 *
 * Input Parameters:
 *   filep - A pointer to a user provided memory location containing the
 *           open file data returned by file_detach().
 *
 * Returned Value:
 *   Zero (OK) is returned on success; A negated errno value is returned on
 *   any failure to indicate the nature of the failure.
 *
 ****************************************************************************/

int file_close(FAR struct file *filep);

/* fs/vfs/fs_ioctl.c ********************************************************/
/****************************************************************************
 * Name: fs_ioctl
 *
 * Description:
 *   Perform device specific operations.
 *
 * Parameters:
 *   fd       File/socket descriptor of device
 *   req      The ioctl command
 *   arg      The argument of the ioctl cmd
 *
 * Return:
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
int fs_ioctl(int fd, int req, unsigned long arg);
#endif

/* fs_fdopen.c **************************************************************/
/****************************************************************************
 * Name: fs_fdopen
 *
 * Description:
 *   This function does the core operations for fopen and fdopen.  It is
 *   used by the OS to clone stdin, stdout, stderr
 *
 ****************************************************************************/

#if CONFIG_NFILE_STREAMS > 0
struct tcb_s; /* Forward reference */
FAR struct file_struct *fs_fdopen(int fd, int oflags);
#endif

/* libc/stdio/lib_fflush.c *************************************************/
/****************************************************************************
 * Name: lib_flushall
 *
 * Description:
 *   Called either (1) by the OS when a task exits, or (2) from fflush()
 *   when a NULL stream argument is provided.
 *
 ****************************************************************************/

#if CONFIG_NFILE_STREAMS > 0
int lib_flushall(FAR struct streamlist *list);
#endif

/* libc/misc/lib_sendfile.c *************************************************/
/****************************************************************************
 * Name: lib_sendfile
 *
 * Description:
 *   Transfer a file
 *
 ****************************************************************************/

#ifdef CONFIG_NET_SENDFILE
ssize_t lib_sendfile(int outfd, int infd, off_t *offset, size_t count);
#endif

/* fs/fs_getfilep.c *********************************************************/
/****************************************************************************
 * Name: fs_getfilep
 *
 * Description:
 *   Given a file descriptor, return the corresponding instance of struct
 *   file.  NOTE that this function will currently fail if it is provided
 *   with a socket descriptor.
 *
 * Input Parameters:
 *   fd    - The file descriptor
 *   filep - The location to return the struct file instance
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int fs_getfilep(int fd, FAR struct file **filep);


/* fs/fs_read.c *************************************************************/
/****************************************************************************
 * Name: file_read
 *
 * Description:
 *   Equivalent to the standard read() function except that is accepts a
 *   struct file instance instead of a file descriptor.  Currently used
 *   only by net_sendfile() and aio_read();
 *
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0
ssize_t file_read(FAR struct file *filep, FAR void *buf, size_t nbytes);
#endif

/* fs/fs_write.c ************************************************************/
/****************************************************************************
 * Name: file_write
 *
 * Description:
 *   Equivalent to the standard write() function except that is accepts a
 *   struct file instance instead of a file descriptor.  Currently used
 *   only by aio_write();
 *
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0
ssize_t file_write(FAR struct file *filep, FAR const void *buf, size_t nbytes);
#endif

/* fs/fs_pread.c ************************************************************/
/****************************************************************************
 * Name: file_pread
 *
 * Description:
 *   Equivalent to the standard pread function except that is accepts a
 *   struct file instance instead of a file descriptor.  Currently used
 *   only by aio_read();
 *
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0
ssize_t file_pread(FAR struct file *filep, FAR void *buf, size_t nbytes,
                   off_t offset);
#endif

/* fs/fs_pwrite.c ***********************************************************/
/****************************************************************************
 * Name: file_pwrite
 *
 * Description:
 *   Equivalent to the standard pwrite function except that is accepts a
 *   struct file instance instead of a file descriptor.  Currently used
 *   only by aio_write();
 *
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0
ssize_t file_pwrite(FAR struct file *filep, FAR const void *buf,
                    size_t nbytes, off_t offset);
#endif

/* fs/fs_lseek.c ************************************************************/
/****************************************************************************
 * Name: file_seek
 *
 * Description:
 *   Equivalent to the standard lseek() function except that is accepts a
 *   struct file instance instead of a file descriptor.  Currently used
 *   only by net_sendfile()
 *
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0
off_t file_seek(FAR struct file *filep, off_t offset, int whence);
#endif

/* fs/fs_lseek64.c ************************************************************/
/****************************************************************************
 * Name: file_seek64
 *
 * Description:
 *   Equivalent to the standard lseek64() function except that is accepts a
 *   struct file instance instead of a file descriptor.  Currently used
 *   only by net_sendfile()
 *
 ****************************************************************************/
#if CONFIG_NFILE_DESCRIPTORS > 0
off64_t file_seek64(FAR struct file *filep, off64_t offset, int whence);
#endif

/* fs/fs_fsync.c ************************************************************/
/****************************************************************************
 * Name: file_fsync
 *
 * Description:
 *   Equivalent to the standard fsync() function except that is accepts a
 *   struct file instance instead of a file descriptor.  Currently used
 *   only by aio_fsync();
 *
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0
int file_fsync(FAR struct file *filep);
#endif

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

int file_ioctl(FAR struct file *filep, int req, unsigned long arg);

/* fs/fs_fcntl.c ************************************************************/
/****************************************************************************
 * Name: file_vfcntl
 *
 * Description:
 *   Similar to the standard vfcntl function except that is accepts a struct
 *   struct file instance instead of a file descriptor.  Currently used
 *   only by aio_fcntl();
 *
 ****************************************************************************/

#if CONFIG_NFILE_DESCRIPTORS > 0
int file_vfcntl(FAR struct file *filep, int cmd, va_list ap);
#endif

/****************************************************************************
 * Name: nx_open and nx_vopen
 *
 * Description:
 *   nx_open() is similar to the standard 'open' interface except that is is
 *   not a cancellation point and it does not modify the errno variable.
 *
 *   nx_vopen() is identical except that it accepts a va_list as an argument
 *   versus taking a variable length list of arguments.
 *
 *   nx_open() and nx_vopen are internal NuttX interface and should not be
 *   called from applications.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int nx_vopen(FAR const char *path, int oflags, va_list ap);
int nx_open(FAR const char *path, int oflags, ...);

/****************************************************************************
 * Name: file_detach
 *
 * Description:
 *   This function is used to device drivers to create a task-independent
 *   handle to an entity in the file system.  file_detach() duplicates the
 *   'struct file' that underlies the file descriptor, then closes the file
 *   descriptor.
 *
 *   This function will fail if fd is not a valid file descriptor.  In
 *   particular, it will fail if fd is a socket descriptor.
 *
 * Input Parameters:
 *   fd    - The file descriptor to be detached.  This descriptor will be
 *           closed and invalid if the file was successfully detached.
 *   filep - A pointer to a user provided memory location in which to
 *           received the duplicated, detached file structure.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; A negated errno value is returned on
 *   any failure to indicate the nature of the failure.
 *
 ****************************************************************************/

int file_detach(int fd, FAR struct file *filep);

void clear_fd(int fd);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* __INCLUDE_FS_FILE_H */
