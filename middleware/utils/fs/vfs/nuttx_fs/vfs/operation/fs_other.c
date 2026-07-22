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

#include "fs_other.h"
#include "errno.h"
#include "stdlib.h"
#include "string.h"
#include "dirent.h"
#include "unistd.h"
#include "sys/select.h"
#include "sys/stat.h"
#include "sys/prctl.h"
#include "inode/inode.h"
#include "operation.h"
#include "nuttx.h"
#include "dpal.h"
#include "dpal_lock.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef __LP64__
typedef unsigned long UINTPTR;
#else
typedef unsigned int UINTPTR;
#endif

#define MAX_DIR_ENT 1024
extern struct inode *files_get_openfile(int fd);

int fstat(int fildes, struct stat *buf)
{
  struct file *filep = NULL;
  int ret;

  ret = fs_getfilep(fildes, &filep);
  if (ret < 0)
    {
      set_errno(-ret);
      return VFS_ERROR;
    }

  return stat(filep->f_path, buf);
}

#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
int fstat64(int fildes, struct stat64 *buf)
{
  struct file *filep = NULL;
  int ret;

  ret = fs_getfilep(fildes, &filep);
  if (ret < 0)
    {
      set_errno(-ret);
      return VFS_ERROR;
    }

  return stat64(filep->f_path, buf);
}
#endif

int lstat(const char *path, struct stat *buffer)
{
  return stat(path, buffer);
}

int chdir(const char *path)
{
  int ret;
  char *fullpath = NULL;
  DIR *dirent_ptr = NULL;
  char *fullpath_bak = NULL;
  unsigned long lock_flags = 0;

  if (!path || !strlen(path))
    {
      set_errno(ENOENT);
      return -1;
    }

  if (strlen(path) > PATH_MAX)
    {
      set_errno(ENAMETOOLONG);

      return -1;
    }

  ret = vfs_normalize_path((const char *)NULL, path, &fullpath);
  if (ret < 0)
    {
      set_errno(-ret);
      return -1; /* build path failed */
    }
  fullpath_bak = fullpath;

  dirent_ptr = opendir(fullpath);
  if (dirent_ptr == NULL)
    {
      dpal_free(fullpath_bak);

      /* this is a not exist directory */

      return -1;
    }

  /* close directory stream */

  ret = closedir(dirent_ptr);
  if (ret < 0)
    {
      dpal_free(fullpath_bak);
      return ret;
    }

  /* copy full path to working directory */

  dpal_spin_lock_irqsave(&g_workdir_lock, &lock_flags);
  ret = strncpy_s(g_workingdirectory, PATH_MAX, fullpath, strlen(fullpath));
  dpal_spin_unlock_irqrestore(&g_workdir_lock, &lock_flags);
  if (ret != EOK)
    {
      printf("chdir path error!\n");
      ret = -1;
    }

  /* release normalize directory path name */

  dpal_free(fullpath_bak);

  return ret;
}

/**
 * this function is a POSIX compliant version, which will return current
 * working directory.
 *
 * @param buf the returned current directory.
 * @param size the buffer size.
 *
 * @return the returned current directory.
 */

char *getcwd(char *buf, size_t n)
{
#ifdef VFS_USING_WORKDIR
  int ret;
  unsigned int len;
  unsigned long lock_flags;
#endif
  if (buf == NULL)
    {
      set_errno(EINVAL);
      return buf;
    }
#ifdef VFS_USING_WORKDIR
  dpal_spin_lock_irqsave(&g_workdir_lock, &lock_flags);
  len = strlen(g_workingdirectory);
  if (n <= len)
    {
      set_errno(ERANGE);
      dpal_spin_unlock_irqrestore(&g_workdir_lock, &lock_flags);
      return NULL;
    }
  ret = memcpy_s(buf, n, g_workingdirectory, len + 1);
  if (ret != EOK)
    {
      set_errno(ENAMETOOLONG);
      return NULL;
    }
  dpal_spin_unlock_irqrestore(&g_workdir_lock, &lock_flags);
#else
  printf("NO_WORKING_DIR\n");
#endif

  return buf;
}

int chmod(const char *path, mode_t mode)
{
  set_errno(ENOSYS);
  return -1;
}

int access(const char *path, int amode)
{
  int result;
  mode_t mode;
  struct stat buf;

  result = stat(path, &buf);

  if (result != ENOERR)
    {
      return -1;
    }

  mode = buf.st_mode;
  if ((unsigned int)amode & R_OK)
    {
      if ((mode & (S_IROTH | S_IRGRP | S_IRUSR)) == 0)
        {
          set_errno(EACCES);
          return -1;
        }
    }

  if ((unsigned int)amode & W_OK)
    {
      if ((mode & (S_IWOTH | S_IWGRP | S_IWUSR)) == 0)
        {
          set_errno(EACCES);
          return -1;
        }
    }

  if ((unsigned int)amode & X_OK)
    {
      if ((mode & (S_IXOTH | S_IXGRP | S_IXUSR)) == 0)
        {
          set_errno(EACCES);
          return -1;
        }
    }

  return 0;
}

bool IS_MOUNTPT(const char *dev)
{
  struct inode_search_s desc;
  bool ret = 0;

  SETUP_SEARCH(&desc, dev, false);
  if (inode_find(&desc) < 0)
    {
      RELEASE_SEARCH(&desc);
      return 0;
    }

  ret = INODE_IS_MOUNTPT(desc.node);
  inode_release(desc.node);
  RELEASE_SEARCH(&desc);
  return ret;
}

int prctl(int option, ...)
{
  UINTPTR name;
  va_list ap;

  va_start(ap, option);

  if (option == PR_SET_NAME)
    {
      name = va_arg(ap, UINTPTR);
      (void)name; // todo
      // OsCurrTaskGet()->taskName = (char *) name;
    }

  va_end(ap);

  return 0;
}

static struct dirent **scandir_get_file_list(const char *dir, int *num, int(*filter)(const struct dirent *))
{
  DIR *od = NULL;
  int listSize = MAX_DIR_ENT;
  int n = *num;
  struct dirent **list = NULL;
  struct dirent **newList = NULL;
  struct dirent *ent = NULL;
  struct dirent *p = NULL;
  int err;

  od = opendir(dir);
  if (od == NULL)
    {
      return NULL;
    }

  list = (struct dirent **)dpal_malloc(listSize * sizeof(struct dirent *));
  if (list == NULL)
    {
      (void)closedir(od);
      return NULL;
    }

  for (ent = readdir(od); ent != NULL; ent = readdir(od))
    {
      if (filter && !filter(ent))
        {
          continue;
        }

      if (n == listSize)
        {
          listSize += MAX_DIR_ENT;
          newList = (struct dirent **)dpal_malloc(listSize * sizeof(struct dirent *));
          if (newList == NULL)
            {
              break;
            }

          err = memcpy_s(newList, listSize * sizeof(struct dirent *), list, n * sizeof(struct dirent *));
          if (err != EOK)
            {
              dpal_free(newList);
              break;
            }
          dpal_free(list);
          list = newList;
        }

      p = (struct dirent *)dpal_malloc(sizeof(struct dirent));
      if (p == NULL)
        {
          break;
        }

      (void)memcpy_s((void *)p, sizeof(struct dirent), (void *)ent, sizeof(struct dirent));
      list[n] = p;

      n++;
    }

  if (closedir(od) < 0)
    {
      while (n--)
        {
          dpal_free(list[n]);
        }
      dpal_free(list);
      return NULL;
    }

  *num = n;
  return list;
}

int scandir(const char *dir, struct dirent ***namelist,
            int(*filter)(const struct dirent *),
            int(*compar)(const struct dirent **,
            const struct dirent **))
{
  int n = 0;
  struct dirent **list = NULL;

  if ((dir == NULL) || (namelist == NULL))
    {
      return -1;
    }

  list = scandir_get_file_list(dir, &n, filter);
  if (list == NULL)
    {
      return -1;
    }

  /* Change to return to the array size */

  *namelist = (struct dirent **)dpal_malloc(n * sizeof(struct dirent *));
  if (*namelist == NULL && n > 0)
    {
      *namelist = list;
    }
  else if (*namelist != NULL)
    {
      (void)memcpy_s(*namelist, n * sizeof(struct dirent *), list, n * sizeof(struct dirent *));
      dpal_free(list);
    }
  else
    {
      dpal_free(list);
    }

  /* Sort array */

  if (compar && *namelist)
    {
      qsort((void *)*namelist, (size_t)n, sizeof(struct dirent *), (int(*)(const void *, const void *)) *compar);
    }

  return n;
}

int alphasort(const struct dirent **a, const struct dirent **b)
{
  return strcoll((*a)->d_name, (*b)->d_name);
}

char *rindex(const char *s, int c)
{
  if (s == NULL)
    {
      return NULL;
    }
  /* Don't bother tracing - strrchr can do that */

  return (char *)strrchr(s, c);
}

int (*sd_sync_fn)(int) = NULL;
int (*nand_sync_fn)(void) = NULL;

void set_sd_sync_fn(int (*sync_fn)(int))
{
  sd_sync_fn = sync_fn;
}

void sync(void)
{
#ifdef CONFIG_FS_FAT_CACHE
  if (sd_sync_fn != NULL)
    {
      (void)sd_sync_fn(0);
      (void)sd_sync_fn(1);
    }
#endif
}

static char *ls_get_fullpath(const char *path, const struct dirent *pdirent)
{
  char *fullpath = NULL;
  int ret = 0;

  if (path[1] != '\0')
    {
      /* 2: The position of the path character: / and the end character /0 */

      fullpath = (char *)dpal_malloc(strlen(path) + strlen(pdirent->d_name) + 2);
      if (fullpath == NULL)
        {
          goto exit_with_nomem;
        }

      /* 2: The position of the path character: / and the end character /0 */

      ret = snprintf_s(fullpath, strlen(path) + strlen(pdirent->d_name) + 2,
                       strlen(path) + strlen(pdirent->d_name) + 1, "%s/%s", path, pdirent->d_name);
      if (ret < 0)
        {
          dpal_free(fullpath);
          set_errno(ENAMETOOLONG);
          return NULL;
        }
    }
  else
    {
      /* 2: The position of the path character: / and the end character /0 */

      fullpath = (char *)dpal_malloc(strlen(pdirent->d_name) + 2);
      if (fullpath == NULL)
        {
          goto exit_with_nomem;
        }

      /* 2: The position of the path character: / and the end character /0 */

      ret = snprintf_s(fullpath, strlen(pdirent->d_name) + 2, strlen(pdirent->d_name) + 1,
                       "/%s", pdirent->d_name);
      if (ret < 0)
        {
          dpal_free(fullpath);
          set_errno(ENAMETOOLONG);
          return NULL;
        }
    }
  return fullpath;

exit_with_nomem:
  set_errno(ENOSPC);
  return (char *)NULL;
}

void ls(const char *pathname)
{
#ifdef __LITEOS__
  struct stat64 stat64_info;
#endif
  struct stat stat_info;
  struct dirent *pdirent = NULL;
  char *path = NULL;
  char *fullpath = NULL;
  char *fullpath_bak = NULL;
  int ret;
  DIR *d = NULL;

  if (pathname == NULL)
    {
#ifdef VFS_USING_WORKDIR
      unsigned long lock_flags;

      /* open current working directory */
      dpal_spin_lock_irqsave(&g_workdir_lock, &lock_flags);
      path = strdup(g_workingdirectory);
      dpal_spin_unlock_irqrestore(&g_workdir_lock, &lock_flags);
#else
      path = strdup("/");
#endif
      if (path == NULL)
        {
          return ;
        }
    }
  else
    {
      ret = vfs_normalize_path(NULL, pathname, &path);
      if (ret < 0)
        {
          set_errno(-ret);
          return;
        }
    }

  /* list all directory and file*/

  d = opendir(path);
  if (d == NULL)
    {
      printf("No such directory\n");
    }
  else
    {
      printf("Directory %s:\n", path);
      do
        {
          pdirent = readdir(d);
          if (pdirent != NULL)
            {
              (void)memset_s(&stat_info, sizeof(struct stat), 0, sizeof(struct stat));
              fullpath = ls_get_fullpath(path, pdirent);
              if (fullpath == NULL)
                {
                  dpal_free(path);
                  (void)closedir(d);
                  return;
                }

              fullpath_bak = fullpath;
#ifdef __LITEOS__
              if (stat(fullpath, &stat64_info) == 0)
                {
                  printf("%-20s", pdirent->d_name);
                  if (S_ISDIR(stat64_info.st_mode))
                    {
                      printf(" %-25s\n", "<DIR>");
                    }
                  else
                    {
                      printf(" %-25lld\n", stat64_info.st_size);
                    }
                } else
#endif
			    if (stat(fullpath, &stat_info) == 0)
                {
                  printf("%-20s", pdirent->d_name);
                  if (S_ISDIR(stat_info.st_mode))
                    {
                      printf(" %-25s\n", "<DIR>");
                    }
                  else
                    {
                      printf("%-25lld\n", stat_info.st_size);
                    }
                }
              else
                  printf("BAD file: %s\n", pdirent->d_name);
              dpal_free(fullpath_bak);
            }
        } while (pdirent != NULL);

      (void)closedir(d);
    }
  dpal_free(path);

  return;
}


char *realpath(const char *path, char *resolved_path)
{
  int ret,result;
  char *new_path = NULL;
  struct stat buf;

  ret = vfs_normalize_path(NULL, path, &new_path);
  if (ret < 0)
    {
      ret = -ret;
      set_errno(ret);
      return NULL;
    }

  result = stat(new_path, &buf);

  if (resolved_path == NULL)
    {
      if (result != ENOERR)
        {
          dpal_free(new_path);
          return NULL;
        }
      return new_path;
    }

  (void)strcpy(resolved_path, new_path);
  dpal_free(new_path);
  if (result != ENOERR)
    {
      return NULL;
    }
  return resolved_path;
}

void lsfd(void)
{
  FAR struct filelist *f_list = NULL;
  unsigned int i = 3; /* file start fd */
  int ret;
  FAR struct inode *node = NULL;

  f_list = &tg_filelist;

  printf("   fd    filename\n");
  ret = nxsem_wait(&f_list->fl_sem);
  if (ret < 0)
    {
      printf("sem_wait error, ret=%d\n", ret);
      return;
    }

  while (i < CONFIG_NFILE_DESCRIPTORS)
    {
      node = files_get_openfile(i);
      if (node)
        {
          printf("%5d   %s\n", i, f_list->fl_files[i].f_path);
        }
      i++;
    }
  (void)nxsem_post(&f_list->fl_sem);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
