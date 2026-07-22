#if defined(CONFIG_FS_YAFFS) && defined(CFG_DRIVERS_NANDFLASH)
#include "yaffsfs.h"
#endif
#include "compiler.h"
#include "fs/fs.h"
#include "fs/dirent_fs.h"
#ifndef VFS_FS
#define VFS_FS
const struct fsmap_t *mount_findfs(const char *filesystemtype);
#if defined(CONFIG_FS_YAFFS) && defined(CFG_DRIVERS_NANDFLASH)
int vfs_yaffs_bind(FAR struct inode *blkdriver, FAR const void *data,
  FAR void **handle, FAR const char *relpath);
int vfs_yaffs_unbind(FAR void *handle, FAR struct inode **blkdriver);
int vfs_yaffs_open(FAR struct file *filep, FAR const char *relpath,
  int oflags, mode_t mode);
int vfs_yaffs_close(FAR struct file *filep);
ssize_t vfs_yaffs_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
ssize_t vfs_yaffs_write(FAR struct file *filep, FAR const char *buffer,
       size_t buflen);
off_t vfs_yaffs_seek(FAR struct file *filep, off_t offset, int whence);
loff_t vfs_yaffs_seek64(FAR struct file *filep, loff_t offset, int whence);
int vfs_yaffs_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
int vfs_yaffs_sync(FAR struct file *filep);
int vfs_yaffs_dup(FAR const struct file *oldp, FAR struct file *newp);
int vfs_yaffs_opendir(FAR struct inode *mountpt,
     FAR const char *relpath,
     FAR struct fs_dirent_s *dir);
int vfs_yaffs_closedir(FAR struct inode *mountpt, FAR struct fs_dirent_s *dir);
int vfs_yaffs_readdir(FAR struct inode *mountpt, FAR struct fs_dirent_s *dir);
int vfs_yaffs_rewinddir(FAR struct inode *mountpt, FAR struct fs_dirent_s *dir);
int vfs_yaffs_mkdir(FAR struct inode *mountpt, FAR const char *relpath, mode_t mode);
#ifdef __OHOS__
int vfs_yaffs_chattr(FAR struct inode *mountpt, FAR const char *relpath, struct IATTR *attr);
#endif
int vfs_yaffs_rmdir(FAR struct inode *mountpt, FAR const char *relpath);
int vfs_yaffs_unlink(FAR struct inode *mountpt, FAR const char *relpath);
int vfs_yaffs_rename(FAR struct inode *mountpt,
    FAR const char *oldrelpath,
    FAR const char *newrelpath);
int vfs_yaffs_stat(FAR struct inode *mountpt, FAR const char *relpath, FAR struct stat *buf);
int vfs_yaffs_statfs(FAR struct inode *mountpt, FAR struct statfs *buf);
#endif
#endif
