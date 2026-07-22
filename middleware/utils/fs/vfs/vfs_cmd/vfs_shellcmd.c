/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2013-2019. All rights reserved.
 * Description: implementation for vfs commands.
 * Create: 2013-01-01
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

#include "sys/mount.h"
#include "dpal.h"
#ifdef CONFIG_SHELL
#include "vfs_cmd/vfs_shellcmd.h"
#include "shell.h"
#include "fs/fs.h"
#include "sys/stat.h"
#include "inode/inode.h"
#include "stdlib.h"
#include "unistd.h"
#include "fs_other.h"
#include "fcntl.h"
#include "sys/statfs.h"
#include "stdio.h"
#include "securec.h"
#ifdef CONFIG_SHELL_FULL_CAP
#include "show.h"
#endif

#ifdef CONFIG_SHELL
#define CMD_KEY_LEN 16
#define CMD_MAX_LEN (256U + CMD_KEY_LEN)
#define SHOW_MAX_LEN CMD_MAX_LEN
#define TEMP_PATH_MAX (PATH_MAX + SHOW_MAX_LEN)
#else
#define TEMP_PATH_MAX PATH_MAX
#endif

typedef enum
{
  RM_RECURSIVER,
  RM_FILE,
  RM_DIR,
  CP_FILE,
  CP_COUNT
} wildcard_type;

#define ERROR_OUT_IF(condition, message_function, handler) \
  do \
    { \
      if (condition) \
        { \
          message_function; \
          handler; \
        } \
    } \
  while (0)

static inline void set_err(int errcode, const char *err_message)
{
  set_errno(errcode);
  printf("%s errno:%d\n", err_message, errno);
}

int OsShellCmdDoChdir(const char *path)
{
  char *fullpath = NULL;
  DIR *dirent = NULL;
  char *fullpath_bak = NULL;
  int ret;
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }

  if (path == NULL)
    {
      dpal_task_lock();
      printf("%s\n", shell_working_directory);
      dpal_task_unlock();

      return 0;
    }

  ERROR_OUT_IF(strlen(path) > PATH_MAX, set_err(ENOTDIR, "cd error"), return -1);

  ret = vfs_normalize_path(shell_working_directory, path, &fullpath);
  ERROR_OUT_IF(ret < 0, set_err(-ret, "cd error"), return -1);

  fullpath_bak = fullpath;

  dirent = opendir(fullpath);
  if (dirent == NULL)
    {
      dpal_free(fullpath_bak);

      /* this is a not exist directory */

      printf("no such file or directory\n");
      return -1;
    }

  /* close directory stream */

  (void)closedir(dirent);

  /* copy full path to working directory */
  dpal_task_lock();
  ret = strncpy_s(shell_working_directory, PATH_MAX, fullpath, strlen(fullpath));
  if (ret != EOK)
    {
      dpal_free(fullpath_bak);
      dpal_task_unlock();
      return -1;
    }
  dpal_task_unlock();

  /* release normalize directory path name */

  dpal_free(fullpath_bak);

  return 0;
}

int OsShellCmdLs(int argc, const char **argv)
{
  char *fullpath = NULL;
  const char *filename = NULL;
  int ret;
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }

  ERROR_OUT_IF(argc > 1, printf("ls or ls [DIRECTORY]\n"), return -1);

  if (argc == 0)
    {
      ls(shell_working_directory);
      return 0;
    }

  filename = argv[0];
  ret = vfs_normalize_path(shell_working_directory, filename, &fullpath);
  ERROR_OUT_IF(ret < 0, set_err(-ret, "ls error"), return -1);

  ls(fullpath);
  dpal_free(fullpath);

  return 0;
}

int OsShellCmdCd(int argc, const char **argv)
{
  if (argc == 0)
    {
      (void)OsShellCmdDoChdir("/");
      return 0;
    }

  (void)OsShellCmdDoChdir(argv[0]);

  return 0;
}

#define CAT_BUF_SIZE  512
#define CAT_TASK_PRIORITY  10
#define CAT_TASK_STACK_SIZE  0x3000

int OsShellCmdDoCatShow(void *arg)
{
  char buf[CAT_BUF_SIZE];
  ssize_t size;
  int fd = -1;
  char *fullpath = (char *)arg;

  fd = open(fullpath, O_RDONLY, 0777);

  if (fd < 0)
    {
      printf("cat open failed errno:%d\n", errno);
      dpal_free(fullpath);
      return -1;
    }

  do
    {
      (void)memset_s(buf, sizeof(buf), 0, sizeof(buf));
      size = read(fd, buf, CAT_BUF_SIZE);
      if (size < 0)
        {
          printf("cat read failed errno:%d\n", errno);
          dpal_free(fullpath);
          (void)close(fd);
          return -1;
        }
      (void)write(1, buf, size);
      (void)dpal_mdelay(1);
    }
  while (size == CAT_BUF_SIZE);

  dpal_free(fullpath);
  (void)close(fd);
  return 0;
}

int OsShellCmdCat(int argc, const char **argv)
{
  char *fullpath = NULL;
  const char *filename = NULL;
  char *fullpath_bak = NULL;
  struct inode_search_s desc;
  int ret;
  unsigned int ca_task;
  FAR struct inode *inode = NULL;
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }

  ERROR_OUT_IF(argc != 1, printf("cat [FILE]\n"), return -1);

  filename = argv[0];
  ret = vfs_normalize_path(shell_working_directory, filename, &fullpath);
  ERROR_OUT_IF(ret < 0, set_err(-ret, "cat error"), return -1);

  inode_semtake();
  fullpath_bak = fullpath;
  SETUP_SEARCH(&desc, fullpath, false);
  ret = inode_search(&desc);
  if (ret < 0)
    {
      set_errno(ENOENT);
      printf("No such file or directory\n");
      inode_semgive();
      dpal_free(fullpath_bak);
      RELEASE_SEARCH(&desc);
      return -1;
    }
    inode = desc.node;
    if (INODE_IS_BLOCK(inode) || INODE_IS_DRIVER(inode))
    {
        set_errno(EPERM);
        printf("Operation not permitted\n");
        inode_semgive();
        dpal_free(fullpath_bak);
        RELEASE_SEARCH(&desc);
        return -1;
    }
    inode_semgive();
    dpal_task_attr_t task_attr;
    (void)memset_s(&task_attr, sizeof(task_attr), 0, sizeof(task_attr));
    task_attr.task_prio = CAT_TASK_PRIORITY;
    task_attr.stack_size = CAT_TASK_STACK_SIZE;
    task_attr.task_name = "shellcmd_cat";
    task_attr.reserved = NULL;
    ret = dpal_task_create(&ca_task, &task_attr, (dpal_tsk_entry_func)OsShellCmdDoCatShow, (void *)fullpath_bak);
    RELEASE_SEARCH(&desc);
    return ret;
}

static int nfs_mount_ref(const char *server_ip_and_path, const char *mount_path,
                         unsigned int uid, unsigned int gid) __attribute__((weakref("nfs_mount")));

void print_mount_usage(void)
{
  printf("mount [DEVICE] [PATH] [NAME]\n");
}

int OsShellCmdMount(int argc, const char **argv)
{
  int ret;
  char *fullpath = NULL;
  const char *filename = NULL;
  unsigned int gid, uid;
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }

  ERROR_OUT_IF(argc < 3, print_mount_usage(), return -1);

  if (strncmp(argv[0], "-t", 2) == 0 || strncmp(argv[0], "-o", 2) == 0)
    {
      filename = argv[2];
      ret = vfs_normalize_path(shell_working_directory, filename, &fullpath);
      ERROR_OUT_IF(ret < 0, set_err(-ret, "mount error"), return -1);

      if (strncmp(argv[3], "nfs", 3) == 0)
        {
          if (argc <= 6)
            {
              uid = (argv[4] != NULL) ? (unsigned int)strtoul(argv[4], (char **)NULL, 0) : 0;
              gid = (argv[5] != NULL) ? (unsigned int)strtoul(argv[5], (char **)NULL, 0) : 0;

              if (nfs_mount_ref != NULL)
                {
                  ret = nfs_mount_ref(argv[1], fullpath, uid, gid);
                  if (ret != 0)
                    {
                      printf("mount -t [DEVICE] [PATH] [NAME]\n[DEVICE] format error, should be IP:PATH\n");
                    }
                }
              else
                {
                  printf("can't find nfs_mount\n");
                }
              dpal_free(fullpath);
              return 0;
            }
        }

      if (strcmp(argv[1], "0") == 0)
        {
          ret = mount((const char *)NULL, fullpath, argv[3], 0, NULL);
        }
      else
        {
          ret = mount(argv[1], fullpath, argv[3], 0, NULL);
        }
      if (ret != 0)
        {
          printf("mount error %d\n", errno);
        }
      else
        {
          printf("mount ok\n");
        }
    }
  else
    {
      filename = argv[1];
      ret = vfs_normalize_path(shell_working_directory, filename, &fullpath);
      ERROR_OUT_IF(ret < 0, set_err(-ret, "mount error"), return -1);

      if (strncmp(argv[2], "nfs", 3) == 0)
        {
          if (argc <= 5)
            {
              uid = (argv[3] != NULL) ? (unsigned int)strtoul(argv[3], (char **)NULL, 0) : 0;
              gid = (argv[4] != NULL) ? (unsigned int)strtoul(argv[4], (char **)NULL, 0) : 0;

              if (nfs_mount_ref != NULL)
                {
                  ret = nfs_mount_ref(argv[0], fullpath, uid, gid);
                  if (ret != 0)
                    {
                      printf("mount [DEVICE] [PATH] [NAME]\n[DEVICE] format error, should be IP:PATH\n");
                    }
                }
              else
                {
                  printf("can't find nfs_mount\n");
                }
              dpal_free(fullpath);
              return 0;
            }

          print_mount_usage();
          dpal_free(fullpath);
          return 0;
        }

      if (strcmp(argv[0], "0") == 0)
        {
          ret = mount((const char *)NULL, fullpath, argv[2], 0, NULL);
        }
      else
        {
          ret = mount(argv[0], fullpath, argv[2], 0, NULL);
        }
      if (ret != 0)
        {
          printf("mount error %d\n", errno);
        }
      else
        {
          printf("mount ok\n");
        }

    }

  dpal_free(fullpath);
  return 0;
}

int OsShellCmdUmount(int argc, const char **argv)
{
  int ret;
  const char *filename = NULL;
  char *fullpath = NULL;
  char *target_path = NULL;
  size_t cmp_num;
  char *work_path = NULL;
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }
  work_path = shell_working_directory;

  ERROR_OUT_IF(argc == 0, printf("umount [PATH]\n"), return 0);

  filename = argv[0];
  ret = vfs_normalize_path(shell_working_directory, filename, &fullpath);
  ERROR_OUT_IF(ret < 0, set_err(-ret, "umount error"), return -1);

  target_path = fullpath;
  cmp_num = strlen(fullpath);
  ret = strncmp(work_path, target_path, cmp_num);
  if (ret == 0)
    {
      work_path += cmp_num;
      if (*work_path == '/' || *work_path == '\0')
        {
          set_errno(EBUSY);
          printf("Resource busy\n");
          dpal_free(fullpath);
          return -1;
        }
    }

  ret = umount(fullpath);
  dpal_free(fullpath);
  if (ret != 0)
    {
      printf("umount error %d\n", errno);
      return 0;
    }

  printf("umount ok\n");
  return 0;
}

int OsShellCmdMkdir(int argc, const char **argv)
{
  int ret;
  char *fullpath = NULL;
  const char *filename = NULL;
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }

  ERROR_OUT_IF(argc != 1, printf("mkdir [DIRECTORY]\n"), return 0);

  filename = argv[0];
  ret = vfs_normalize_path(shell_working_directory, filename, &fullpath);
  ERROR_OUT_IF(ret < 0, set_err(-ret, "mkdir error"), return -1);

  ret = mkdir(fullpath, 0);
  if (ret == -1)
    {
      printf("mkdir error:%d\n", errno);
    }
  dpal_free(fullpath);
  return 0;
}

int OsShellCmdPwd(int argc, const char **argv)
{
  char buf[SHOW_MAX_LEN] = {0};
  DIR *dir = NULL;
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }

  ERROR_OUT_IF(argc > 0, printf("\nUsage: pwd\n"), return -1);

  dir = opendir(shell_working_directory);
  if (dir == NULL)
    {
      printf("pwd error %d\n", errno);
      return -1;
    }

  dpal_task_lock();
  if (strncpy_s(buf, SHOW_MAX_LEN, shell_working_directory, SHOW_MAX_LEN - 1) != EOK)
    {
      dpal_task_unlock();
      printf("pwd error: strncpy_s error!\n");
      (void)closedir(dir);
      return -1;
    }
  dpal_task_unlock();

  printf("%s\n", buf);
  (void)closedir(dir);
  return 0;
}

static inline void print_statfs_usage(void)
{
  printf("Usage  :\n");
  printf("    statfs <path>\n");
  printf("    path  : Mounted file system path that requires query information\n");
  printf("Example:\n");
  printf("    statfs /ramfs\n");
}

int OsShellCmdStatfs(int argc, const char **argv)
{
  struct statfs sfs;
  int result;
  unsigned long long total_size, free_size;
  char *fullpath = NULL;
  const char *filename = NULL;

  if (argc != 1) {
    return -1;
  }
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }

  (void)memset_s(&sfs, sizeof(sfs), 0, sizeof(sfs));

  filename = argv[0];
  result = vfs_normalize_path(shell_working_directory, filename, &fullpath);
  ERROR_OUT_IF(result < 0, set_err(-result, "statfs error"), return -1);

  result = statfs(fullpath, &sfs);
  dpal_free(fullpath);

  if (result != 0 || sfs.f_type == 0)
    {
      printf("statfs failed! Invalid argument!\n");
      print_statfs_usage();
      return -1;
    }

  total_size  = (unsigned long long)sfs.f_bsize * sfs.f_blocks;
  free_size   = (unsigned long long)sfs.f_bsize * sfs.f_bfree;

  printf("statfs got:\n f_type     = %lu\n cluster_size   = %lu\n", sfs.f_type, sfs.f_bsize);
  printf(" total_clusters = %llu\n free_clusters  = %llu\n", sfs.f_blocks, sfs.f_bfree);
  printf(" avail_clusters = %llu\n f_namelen    = %lu\n", sfs.f_bavail, sfs.f_namelen);
  printf("\n%s\n total size: %4llu Bytes\n free  size: %4llu Bytes\n", argv[0], total_size, free_size);

  return 0;
}

int OsShellCmdTouch(int argc, const char **argv)
{
  int ret;
  int fd = -1;
  char *fullpath = NULL;
  const char *filename = NULL;
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }

  ERROR_OUT_IF(argc != 1, printf("touch [FILE]\n"), return -1);

  filename = argv[0];
  ret = vfs_normalize_path(shell_working_directory, filename, &fullpath);
  ERROR_OUT_IF(ret < 0, set_err(-ret, "touch error"), return -1);

  fd = open(fullpath, O_RDWR | O_CREAT, 0);
  dpal_free(fullpath);
  if (fd == -1)
    {
      printf("touch error %d\n", errno);
      return -1;
    }

  (void)close(fd);
  return 0;
}

#define CP_BUF_SIZE 4096
static int OsShellCmdDoCp(const char *src_filepath, const char *dst_filename)
{
  int  ret;
  char *src_fullpath = NULL;
  char *dst_fullpath = NULL;
  const char *src_filename = NULL;
  char *dst_filepath = NULL;
  char *buf = NULL;
  const char *filename = NULL;
  ssize_t r_size, w_size;
  int src_fd = -1;
  int dst_fd = -1;
  struct stat stat_buf;
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }

  buf = (char *)dpal_malloc(CP_BUF_SIZE);
  if (buf == NULL)
    {
      printf("cp error: Out of memory!\n");
      return -1;
    }

  /* Get source fullpath. */

  ret = vfs_normalize_path(shell_working_directory, src_filepath, &src_fullpath);
  if (ret < 0)
    {
      set_errno(-ret);
      printf("cp error:%d\n", errno);
      dpal_free(buf);
      return -1;
    }

  /* Is source path exist? */

  ret = stat(src_fullpath, &stat_buf);
  if (ret == -1)
    {
      printf("cp %s error:%d\n", src_fullpath, errno);
      goto errout_with_srcpath;
    }

  /* Is source path a directory? */

  if (S_ISDIR(stat_buf.st_mode))
    {
      printf("cp %s error: Source file can't be a directory.\n", src_fullpath);
      goto errout_with_srcpath;
    }

  /* Get dest fullpath. */

  dst_fullpath = strdup(dst_filename);
  if (dst_fullpath == NULL)
    {
      printf("cp error: Out of memory.\n");
      goto errout_with_srcpath;
    }

  /* Is dest path exist? */

  ret = stat(dst_fullpath, &stat_buf);
  if (ret == 0)
    {
      /* Is dest path a directory? */

      if (S_ISDIR(stat_buf.st_mode))
        {
          /* Get source file name without '/'. */

          src_filename = src_filepath;
          while (1)
            {
              filename = strchr(src_filename, '/');
              if (filename == NULL)
                {
                  break;
                }
              src_filename = filename + 1;
            }

          /* Add the source file after dest path. */

          ret = vfs_normalize_path(dst_fullpath, src_filename, &dst_filepath);
          if (ret < 0)
            {
              set_errno(-ret);
              printf("cp error.%d.\n", errno);
              goto errout_with_path;
            }
          dpal_free(dst_fullpath);
          dst_fullpath = dst_filepath;
        }
    }

  /* Is dest file same as source file? */

  if (strcmp(src_fullpath, dst_fullpath) == 0)
    {
      printf("cp error: '%s' and '%s' are the same file\n", src_fullpath, dst_fullpath);
      goto errout_with_path;
    }

  /* Copy begins. */
  src_fd = open(src_fullpath, O_RDONLY, 0777);
  if (src_fd < 0)
    {
      printf("cp error: can't open %s. errno:%d.\n", src_fullpath, errno);
      goto errout_with_path;
    }

  dst_fd = open(dst_fullpath, O_CREAT | O_RDWR, 0777);
  if (dst_fd < 0)
    {
      printf("cp error: can't open %s. errno%d\n", dst_fullpath, errno);
      goto errout_with_srcfd;
    }

  do
    {
      (void)memset_s(buf, CP_BUF_SIZE, 0, CP_BUF_SIZE);
      r_size = read(src_fd, buf, CP_BUF_SIZE);
      if (r_size == EOF)
        {
          printf("cp %s %s failed.%d.\n", src_fullpath, dst_fullpath, errno);
          goto errout_with_fd;
        }
      w_size = write(dst_fd, buf, r_size);
      if (w_size != r_size)
        {
          printf("cp %s %s failed. Check space left on device.\n", src_fullpath, dst_fullpath);
          goto errout_with_fd;
        }
    }
  while (r_size == CP_BUF_SIZE);

  /* Release resource. */

  dpal_free(buf);
  dpal_free(src_fullpath);
  dpal_free(dst_fullpath);
  (void)close(src_fd);
  (void)close(dst_fd);
  return 0;

errout_with_fd:
  (void)close(dst_fd);
errout_with_srcfd:
  (void)close(src_fd);
errout_with_path:
  dpal_free(dst_fullpath);
errout_with_srcpath:
  dpal_free(src_fullpath);
  dpal_free(buf);
  return -1;
}

/* The separator and EOF for a directory fullpath: '/'and '\0' */

#define SEPARATOR_EOF_LEN 2

static int OsShellCmdDoRmdir(const char *pathname)
{
  struct dirent *dirent = NULL;
  struct stat stat_info;
  DIR *d = NULL;
  char *fullpath = NULL;
  int ret;

  (void)memset_s(&stat_info, sizeof(stat_info), 0, sizeof(struct stat));
  if (stat(pathname, &stat_info) != 0)
    {
      return -1;
    }

  if (S_ISREG(stat_info.st_mode))
    {
      return unlink(pathname);
    }
  d = opendir(pathname);
  while (1)
    {
      dirent = readdir(d);
      if (dirent == NULL)
        {
          break;
        }
      if (strcmp(dirent->d_name, "..") && strcmp(dirent->d_name, "."))
        {
          size_t fullpath_buf_size = strlen(pathname) + strlen(dirent->d_name) + SEPARATOR_EOF_LEN;
          fullpath = (char *)dpal_malloc(fullpath_buf_size);
          if (fullpath == NULL)
            {
              printf("malloc failure!\n");
              (void)closedir(d);
              return -1;
            }
          ret = snprintf_s(fullpath, fullpath_buf_size, fullpath_buf_size - 1, "%s/%s", pathname, dirent->d_name);
          if (ret < 0)
            {
              printf("name is too long!\n");
              dpal_free(fullpath);
              (void)closedir(d);
              return -1;
            }
          (void)OsShellCmdDoRmdir(fullpath);
          dpal_free(fullpath);
        }
    }
  (void)closedir(d);
  return rmdir(pathname);
}

/*  Wildcard matching operations  */

static int os_wildcard_match(const char *src, const char *filename)
{
  int ret;

  if (*src != '\0')
    {
      if (*filename == '*')
        {
          while ((*filename == '*') || (*filename == '?'))
            {
              filename++;
            }

          if (*filename == '\0')
            {
              return 0;
            }

          while (*src != '\0' && !(*src == *filename))
            {
              src++;
            }

          if (*src == '\0')
            {
              return -1;
            }

          ret = os_wildcard_match(src, filename);

          while ((ret != 0) && (*(++src) != '\0'))
            {
              if (*src == *filename)
                {
                  ret = os_wildcard_match(src, filename);
                }
            }
          return ret;
        }
      else
        {
          if ((*src == *filename) || (*filename == '?'))
            {
              return os_wildcard_match(++src, ++filename);
            }
          return -1;
        }
    }

  while (*filename != '\0')
    {
      if (*filename != '*')
        {
          return -1;
        }
      filename++;
    }
  return 0;
}

/*   To determine whether a wildcard character exists in a path   */

static int os_is_containers_wildcard(const char *filename)
{
  while (*filename != '\0')
    {
      if ((*filename == '*') || (*filename == '?'))
        {
          return 1;
        }
      filename++;
    }
  return 0;
}

/*  Delete a matching file or directory  */

static int os_wildcard_delete_file_or_dir(const char *fullpath, wildcard_type mark)
{
  int ret;

  switch (mark)
    {
      case RM_RECURSIVER:
        ret = OsShellCmdDoRmdir(fullpath);
        break;
      case RM_FILE:
        ret = unlink(fullpath);
        break;
      case RM_DIR:
        ret = rmdir(fullpath);
        break;
      default:
        return VFS_ERROR;
    }
  if (ret == -1)
    {
      printf("%s  ", fullpath);
      printf("rm/rmdir error! %d\n", errno);
      return ret;
    }

  printf("%s match successful!delete!\n", fullpath);
  return 0;
}

/*  Split the path with wildcard characters  */

static char* os_wildcard_split_path(char *fullpath, char **handle, char **wait)
{
  size_t n;
  size_t a = 0;
  size_t b = 0;
  size_t len  = strlen(fullpath);

  for (n = 0; n < len; n++)
    {
      if (fullpath[n] == '/')
        {
          if (b != 0)
            {
              fullpath[n] = '\0';
              *wait = fullpath + n + 1;
              break;
            }
          a = n;
        }
      else if (fullpath[n] == '*' || fullpath[n] == '?')
        {
          b = n;
          fullpath[a] = '\0';
          if (a == 0)
            {
              *handle = fullpath + a + 1;
              continue;
            }
          *handle = fullpath + a + 1;
        }
    }
  return fullpath;
}

/*  Handling entry of the path with wildcard characters  */

static int os_wildcard_extract_directory(char *fullpath, void *dst, wildcard_type mark)
{
  char separator[] = "/";
  char src[PATH_MAX] = {0};
  struct dirent *dirent = NULL;
  char *f = NULL;
  char *s = NULL;
  char *t = NULL;
  int ret = 0;
  DIR *d = NULL;
  struct stat stat_buf;

  f = os_wildcard_split_path(fullpath, &s, &t);

  if (s == NULL)
    {
      if (mark == CP_FILE)
        {
          ret = OsShellCmdDoCp(fullpath, dst);
        }
      else if (mark == CP_COUNT)
        {
          ret = stat(fullpath, &stat_buf);
          if (ret == 0 && S_ISREG(stat_buf.st_mode))
            {
              (*(int *)dst)++;
            }
        }
      else
        {
          ret = os_wildcard_delete_file_or_dir(fullpath, mark);
        }
      return ret;
    }

  d = (*f == '\0') ? opendir("/") : opendir(f);

  if (d == NULL)
    {
      printf("opendir error %d\n", errno);
      return VFS_ERROR;
    }

  while (1)
    {
      dirent = readdir(d);
      if (dirent == NULL)
        {
          break;
        }

      ret = strcpy_s(src, PATH_MAX, f);
      if (ret != EOK)
        {
          goto closedir_out;
        }

      ret = os_wildcard_match(dirent->d_name, s);
      if (ret == 0)
        {
          ret = strcat_s(src, sizeof(src), separator);
          if (ret != EOK)
            {
              goto closedir_out;
            }
          ret = strcat_s(src, sizeof(src), dirent->d_name);
          if (ret != EOK)
            {
              goto closedir_out;
            }
          if (t == NULL)
            {
              if (mark == CP_FILE)
                {
                  ret = OsShellCmdDoCp(src, dst);
                }
              else if (mark == CP_COUNT)
                {
                  ret = stat(src, &stat_buf);
                  if (ret == 0 && S_ISREG(stat_buf.st_mode))
                    {
                      (*(int *)dst)++;
                      if ((*(int *)dst) > 1)
                        {
                          break;
                        }
                    }
                }
              else
                {
                  ret = os_wildcard_delete_file_or_dir(src, mark);
                }
            }
          else
            {
              ret = strcat_s(src, sizeof(src), separator);
              if (ret != EOK)
                {
                  goto closedir_out;
                }
              ret = strcat_s(src, sizeof(src), t);
              if (ret != EOK)
                {
                  goto closedir_out;
                }
              ret = os_wildcard_extract_directory(src, dst, mark);
              if (mark == CP_COUNT && (*(int *)dst) > 1)
                {
                  break;
                }
            }
        }
    }
  (void)closedir(d);
  return ret;
closedir_out:
  (void)closedir(d);
  return VFS_ERROR;
}

int OsShellCmdCp(int argc, const char **argv)
{
  int  ret;
  const char *src = NULL;
  const char *dst = NULL;
  char *src_fullpath = NULL;
  char *dst_fullpath = NULL;
  struct stat stat_buf;
  int count = 0;
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }

  ERROR_OUT_IF(argc < 2, printf("cp [SOURCEFILE] [DESTFILE]\n"), return -1);

  src = argv[0];
  dst = argv[1];

  /* Get source fullpath. */

  ret = vfs_normalize_path(shell_working_directory, src, &src_fullpath);
  if (ret < 0)
    {
      set_errno(-ret);
      printf("cp error:%d\n", errno);
      return -1;
    }

  if (src[strlen(src) - 1] == '/')
    {
      printf("cp %s error: Source file can't be a directory.\n", src);
      goto errout_with_srcpath;
    }

  /* Get dest fullpath. */

  ret = vfs_normalize_path(shell_working_directory, dst, &dst_fullpath);
  if (ret < 0)
    {
      set_errno(-ret);
      printf("cp error: can't open %s. %d\n", dst, errno);
      goto errout_with_srcpath;
    }

  /* Is dest path exist? */

  ret = stat(dst_fullpath, &stat_buf);
  if (ret < 0)
    {
      /* Is dest path a directory? */

      if (dst[strlen(dst) - 1] == '/')
        {
          printf("cp error: %s, %d.\n", dst_fullpath, errno);
          goto errout_with_path;
        }
    }
  else
    {
      if (S_ISREG(stat_buf.st_mode) && dst[strlen(dst) - 1] == '/')
        {
          printf("cp error: %s is not a directory.\n", dst_fullpath);
          goto errout_with_path;
        }
    }

   if (os_is_containers_wildcard(src_fullpath))
    {
      if (ret < 0 || S_ISREG(stat_buf.st_mode))
        {
          char *src_copy = strdup(src_fullpath);
          if (src_copy == NULL)
            {
              printf("cp error : Out of memory.\n");
              goto errout_with_path;
            }
          (void)os_wildcard_extract_directory(src_copy, &count, CP_COUNT);
          dpal_free(src_copy);
          if (count > 1)
            {
              printf("cp error : %s is not a directory.\n", dst_fullpath);
              goto errout_with_path;
            }
        }
      ret = os_wildcard_extract_directory(src_fullpath, dst_fullpath, CP_FILE);
    }
  else
    {
      ret = OsShellCmdDoCp(src_fullpath, dst_fullpath);
    }
  dpal_free(dst_fullpath);
  dpal_free(src_fullpath);
  return ret;

errout_with_path:
  dpal_free(dst_fullpath);
errout_with_srcpath:
  dpal_free(src_fullpath);
  return VFS_ERROR;
}

static inline void print_rm_usage(void)
{
  printf("rm [FILE] or rm [-r/-R] [FILE]\n");
}

int OsShellCmdRm(int argc, const char **argv)
{
  int  ret = 0;
  char *fullpath = NULL;
  const char *filename = NULL;
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }

  ERROR_OUT_IF(argc != 1 && argc != 2, print_rm_usage(), return -1);

  if (argc == 2)
    {
      ERROR_OUT_IF(strcmp(argv[0], "-r") != 0 && strcmp(argv[0], "-R") != 0, print_rm_usage(), return -1);

      filename = argv[1];
      ret = vfs_normalize_path(shell_working_directory, filename, &fullpath);
      ERROR_OUT_IF(ret < 0, set_err(-ret, "rm error"), return -1);

      if (os_is_containers_wildcard(fullpath))
        {
          ret = os_wildcard_extract_directory(fullpath, NULL, RM_RECURSIVER);
        }
      else
        {
          ret = OsShellCmdDoRmdir(fullpath);
        }
    }
  else
    {
      filename = argv[0];
      ret = vfs_normalize_path(shell_working_directory, filename, &fullpath);
      ERROR_OUT_IF(ret < 0, set_err(-ret, "rm error"), return -1);

      if (os_is_containers_wildcard(fullpath))
        {
          ret = os_wildcard_extract_directory(fullpath, NULL, RM_FILE);
        }
      else
        {
          ret = unlink(fullpath);
        }
    }
  if (ret == -1)
    {
      printf("rm error %d\n", errno);
    }
  dpal_free(fullpath);
  return 0;
}

int OsShellCmdRmdir(int argc, const char **argv)
{
  int  ret;
  char *fullpath = NULL;
  const char *filename = NULL;
  char *shell_working_directory = dpal_get_shell_work_dir();
  if (shell_working_directory == NULL)
    {
      return -1;
    }

  ERROR_OUT_IF(argc == 0, printf("rmdir [DIRECTORY]\n"), return -1);

  filename = argv[0];
  ret = vfs_normalize_path(shell_working_directory, filename, &fullpath);
  ERROR_OUT_IF(ret < 0, set_err(-ret, "rmdir error"), return -1);

  if (os_is_containers_wildcard(fullpath))
    {
      ret = os_wildcard_extract_directory(fullpath, NULL, RM_DIR);
    }
  else
    {
      ret = rmdir(fullpath);
    }
  dpal_free(fullpath);

  if (ret == -1)
    {
      printf("rmdir error %d\n", errno);
    }

  return 0;
}

int OsShellCmdSync(int argc, const char **argv)
{
  ERROR_OUT_IF(argc > 0, printf("\nUsage: sync\n"), return -1);

  sync();
  return 0;
}

int OsShellCmdLsfd(int argc, const char **argv)
{
  ERROR_OUT_IF(argc > 0, printf("\nUsage: lsfd\n"), return -1);

  lsfd();

  return 0;
}

#endif
