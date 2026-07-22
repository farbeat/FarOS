/****************************************************************************
 * fs/vfs/operation/fs_poll.c
 *
 *   Copyright (C) 2008-2009, 2012-2019 Gregory Nutt. All rights reserved.
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

#ifdef CONFIG_LIB_SUPPORT_COMPAT_LINUX
#include "vfs_config.h"
#include "stdint.h"
#include "poll.h"
#include "assert.h"
#include "errno.h"
#include "debug.h"
#include "fs/fs.h"
#include "inode/inode.h"
#include "stdlib.h"
#include "unistd.h"
#ifdef CONFIG_NET_LWIP_SACK
#include "lwip/sockets.h"
#endif
#include "nuttx.h"
#include "fs_poll_pri.h"
#include "dpal.h"
#include "dpal_lock.h"
#include "dpal_list.h"
#include "dpal_wait.h"
#include "dpal_sam.h"

#ifndef MSEC_PER_SEC
#define MSEC_PER_SEC        1000L
#endif

#ifndef NSEC_PER_MSEC
#define NSEC_PER_MSEC    1000000L
#endif

#define N_POLL_ITEMS 5
#define poll_semgive(sem) nxsem_post(sem)

typedef struct tag_poll_wait_node
{
  dpal_dl_list_t queue_node;
  pollevent_t key;
  struct tag_poll_wait_entry *entry;
  wait_queue_head_t *wait_queue;
  dpal_spinlock lock;
} poll_wait_node;

typedef struct tag_poll_wait_entry_table
{
  struct tag_poll_wait_entry_table *next;
  UINT32 index;
  poll_wait_node items[N_POLL_ITEMS];
} poll_wait_entry_table;

typedef struct tag_poll_wait_entry
{
  bool add_queue_flag;
  nu_sem sem;
  UINT32 inline_index;
  poll_wait_node inline_items[N_POLL_ITEMS];
  poll_wait_entry_table *table;
} poll_wait_entry;

int ConsoleUpdateFd(void) __attribute__((weak));

static int poll_semtake(FAR nu_sem *sem)
{
  int ret;

  /* Take the semaphore (perhaps waiting) */

  ret = nxsem_wait(sem);

  /* The only case that an error should occur here is if the wait were
   * awakened by a signal.
   */

  DEBUGASSERT(ret == OK || ret == -EINTR);
  return ret;
}


static void set_add_poll_wait_flag(poll_wait_head wait, bool add_queue_flag)
{
  wait->add_queue_flag = add_queue_flag;
}

static int destroy_poll_wait(poll_wait_head wait)
{
  unsigned int i;
  unsigned long int_save;
  poll_wait_node *wait_node = NULL;
  poll_wait_entry_table *curr_table = NULL;

  for (i = 0; i < wait->inline_index; ++i)
    {
      wait_node = &wait->inline_items[i];
      dpal_spin_lock_irqsave(&wait_node->lock, &int_save);
      dpal_list_delete(&wait_node->queue_node);
      dpal_spin_unlock_irqrestore(&wait_node->lock, &int_save);
    }

  while (wait->table)
    {
      curr_table = wait->table;
      wait->table = curr_table->next;

      for (i = 0; i < curr_table->index; ++i)
        {
          wait_node = &curr_table->items[i];
          dpal_spin_lock_irqsave(&wait_node->lock, &int_save);
          dpal_list_delete(&wait_node->queue_node);
          dpal_spin_unlock_irqrestore(&wait_node->lock, &int_save);
        }
      dpal_free(curr_table);
    }

  if (nxsem_destroy(&wait->sem) < 0)
    {
      PRINT_ERR("[%s] sem_destroy failed\n", __FUNCTION__);
      return -1;
    }

  return 0;
}

static poll_wait_node *get_poll_item(poll_wait_head wait)
{
  if (wait->inline_index < N_POLL_ITEMS)
    {
      return wait->inline_items + wait->inline_index++;
    }
  if (!wait->table || (wait->table && wait->table->index >= N_POLL_ITEMS))
    {
      poll_wait_entry_table *new_entry_table = NULL;

      new_entry_table = (poll_wait_entry_table *)dpal_zalloc(sizeof(poll_wait_entry_table));
      if (new_entry_table == NULL)
        {
          return (poll_wait_node *)NULL;
        }
      new_entry_table->index = 0;

      new_entry_table->next = wait->table;
      wait->table = new_entry_table;
    }

    return wait->table->items + wait->table->index++;
}

static void add_pollwait_queue(wait_queue_head_t *queue, const poll_table *p)
{
    unsigned long int_save;
    poll_wait_head wait = p->wait;
    poll_wait_node *new_node = get_poll_item(wait);
    dpal_spinlock lock;
    dpal_spinlock queue_lock;
    if (new_node != NULL)
    {
        dpal_spin_lock_init(&lock);
        (void)memcpy_s(&new_node->lock, sizeof(dpal_spinlock), &lock, sizeof(dpal_spinlock));
        new_node->entry = wait;
        new_node->key = p->key;
        new_node->wait_queue = queue;
        queue_lock.lock = (void *)(&queue->lock);
        dpal_spin_lock_irqsave(&queue_lock, &int_save);
        dpal_list_add((dpal_dl_list_t *)&queue->poll_queue, &new_node->queue_node);
        dpal_spin_unlock_irqrestore(&queue_lock, &int_save);
    }
}

static int poll_timedwait(nu_sem *sem, UINT32 millisecs)
{
    UINT32 ret;

    ret = dpal_sem_down_timeout(sem, millisecs);
    if (ret == LOS_OK) {
        return 0;
    } else if (ret == LOS_ERRNO_SEM_TIMEOUT) {
        errno = ETIMEDOUT;
        return -1;
    } else {
        errno = EINVAL;
        return -1;
    }
}

#if CONFIG_NFILE_DESCRIPTORS > 0

static int file_poll(struct file *filep, poll_table *wait)
{
  struct inode *inode = NULL;
  int ret = -ENOSYS;

  inode = filep->f_inode;

  if (inode != NULL && inode->u.i_ops != NULL && inode->u.i_ops->poll != NULL)
    {
      ret = inode->u.i_ops->poll(filep, wait);
    }

  return ret;
}
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0
static int fdesc_poll(int fd, poll_table *wait)
{
  struct file *filep = NULL;
  int ret;

  if (fd <= STDERR_FILENO && fd >= STDIN_FILENO && ConsoleUpdateFd != NULL) /* fd : [0,2] */
    {
      fd = ConsoleUpdateFd();
      if (fd < 0)
        {
          set_errno(EBADF);
          return VFS_ERROR;
        }
    }

  /* Get the file pointer corresponding to this file descriptor */

  ret = fs_getfilep(fd, &filep);
  if (ret < 0)
    {
      set_errno(-ret);
      return VFS_ERROR;
    }

  /* Let file_poll() do the rest */

  return file_poll(filep, wait);
}
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0
#if defined(CONFIG_NET_LWIP_SACK)
#include "lwip/sockets.h"
#endif
static int query_fd(int fd, poll_table *wait)
{
  /* Check for a valid file descriptor */

  if (fd >= CONFIG_NFILE_DESCRIPTORS)
    {
      /* Perform the socket ioctl */

#if defined(CONFIG_NET_LWIP_SACK)
      if (fd < (CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS))
        {
          return lwip_poll(fd, wait);
        }
      else
#endif
        {
          return -EBADF;
        }
    }

  return fdesc_poll(fd, wait);
}
#endif

static int query_fds(struct pollfd *fds, nfds_t nfds, poll_table *wait)
{
  nfds_t i;
  int ret;
  int count = 0;

  if (fds == NULL || wait == NULL || wait->wait == NULL)
    {
      set_errno(EINVAL);
      return -1;
    }

  for (i = 0; i < nfds; ++i)
    {
      struct pollfd *p_fds = &fds[i];
      if (p_fds->fd < 0)
        {
          set_errno(EBADF);
          return -1;
        }

      wait->key = p_fds->events | POLLERR | POLLHUP;

      ret = query_fd(p_fds->fd, wait);
      if (ret < 0)
        {
          set_errno(-ret);
          return -1;
        }

      p_fds->revents = (p_fds->events | POLLERR | POLLHUP) & (pollevent_t)ret;
      if (p_fds->revents)
        {
          ++count;
          set_add_poll_wait_flag(wait->wait, false);
        }
    }

  return count;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void notify_poll_with_key(wait_queue_head_t *wait, pollevent_t key)
{
  unsigned long int_save;
  int failed_count = 0;
  poll_wait_node *curr = NULL;
  dpal_spinlock lock;

  if (wait == NULL)
    {
      PRINT_ERR("[%s] parameter err! \n", __FUNCTION__);
      return;
    }
  lock.lock = (void *)(&wait->lock);
  dpal_spin_lock_irqsave(&lock, &int_save);
  DPAL_DL_LIST_FOR_EACH_ENTRY(curr, (dpal_dl_list_t *)&(wait->poll_queue), poll_wait_node, queue_node) {
      poll_wait_entry *curr_entry = curr->entry;
      if (!key || (key & curr->key))
        {
          if (poll_semgive(&curr_entry->sem) < 0)
            {
              failed_count++;
            }
        }
    }
  dpal_spin_unlock_irqrestore(&lock, &int_save);

  if (failed_count != 0)
    {
      PRINT_ERR("[%s] sem_post failed %d times\n", __FUNCTION__, failed_count);
    }
}

/* just for compatible */

void notify_poll(wait_queue_head_t *wait)
{
    notify_poll_with_key(wait, 0);
}

void poll_wait(struct file *filp, wait_queue_head_t *wait_address, poll_table *p)
{
  if (!wait_address || !p || !p->wait)
    {
      return;
    }

  if (p->wait->add_queue_flag)
    {
      add_pollwait_queue(wait_address, p);
    }
}

/****************************************************************************
 * Name: poll
 *
 * Description:
 *   poll() waits for one of a set of file descriptors to become ready to
 *   perform I/O.  If none of the events requested (and no error) has
 *   occurred for any of  the  file  descriptors,  then  poll() blocks until
 *   one of the events occurs.
 *
 * Input Parameters:
 *   fds  - List of structures describing file descriptors to be monitored
 *   nfds - The number of entries in the list
 *   timeout - Specifies an upper limit on the time for which poll() will
 *     block in milliseconds.  A negative value of timeout means an infinite
 *     timeout.
 *
 * Returned Value:
 *   On success, the number of structures that have non-zero revents fields.
 *   A value of 0 indicates that the call timed out and no file descriptors
 *   were ready.  On error, -1 is returned, and errno is set appropriately:
 *
 *   EBADF  - An invalid file descriptor was given in one of the sets.
 *   EFAULT - The fds address is invalid
 *   EINTR  - A signal occurred before any requested event.
 *   EINVAL - The nfds value exceeds a system limit.
 *   ENOMEM - There was no space to allocate internal data structures.
 *   ENOSYS - One or more of the drivers supporting the file descriptor
 *     does not support the poll method.
 *
 ****************************************************************************/

int poll(FAR struct pollfd *fds, nfds_t nfds, int timeout)
{
  poll_table wait_table;
  poll_wait_entry wait_entry;
  UINT64 start_ticks = 0;
  int millisecs_left;
  int ret = OK;
  int count = 0;
  int err = 0;
  int temp = 0;

  wait_table.wait = &wait_entry;
  wait_table.wait->table = NULL;
  wait_table.wait->inline_index = 0;
  if (nxsem_init(&wait_table.wait->sem, 0, 0) < 0)
    {
      set_errno(ENOMEM);
      return -1;
    }

  /* set wait flag */

  set_add_poll_wait_flag(wait_table.wait, ((timeout == 0) ? false : true));

  count = query_fds(fds, nfds, &wait_table);
  if (count > 0)
    {
      ret = OK;
      goto out;
    }
  else if (count < 0)
    {
      goto out;
    }

  /* clear wait flag if no fd got on first query */

  set_add_poll_wait_flag(wait_table.wait, false);

  if (timeout > 0)
    {
      start_ticks = dpal_gettime_mono(true);
    }

  millisecs_left = timeout;
  while (count == 0)
    {
      if (timeout < 0)
        {
          ret = poll_semtake(&wait_table.wait->sem);
        }
      else if (timeout == 0 || millisecs_left <= 0)
        {
          ret = OK;
          goto out;
        }
      else if (millisecs_left > 0)
        {
          UINT64 curr_ticks;
          UINT64 millisecs_last;

          curr_ticks = dpal_gettime_mono(true);
          millisecs_last = (curr_ticks - start_ticks) * MSEC_PER_SEC / LOSCFG_BASE_CORE_TICK_PER_SECOND;
          if (millisecs_last >= timeout)
            {
              ret = OK;
              goto out;
            }
          else
            {
              millisecs_left = timeout - millisecs_last;
            }

          ret = poll_timedwait(&wait_table.wait->sem, millisecs_left);
          if (ret < 0)
            {
              err = get_errno();

              if (err == ETIMEDOUT)
                {
                  ret = OK;
                }
              else
                {
                  ret = -err;
                }
            }
        }

      if (ret < 0)
        {
          goto out;
        }
      count = query_fds(fds, nfds, &wait_table);
      if (err == ETIMEDOUT)
        {
          break;
        }
    }

out:
  temp = get_errno();
  if (destroy_poll_wait(wait_table.wait) < 0)
    {
      temp = get_errno();
    }

  if (temp != 0)
    {
      set_errno(temp);
    }
  if (ret < 0)
    {
      set_errno(-ret);
      return -1;
    }

  return count;
}

#endif
