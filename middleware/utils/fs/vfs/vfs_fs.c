#include "vfs_fs.h"

#if defined(CONFIG_FS_FAT) && defined(CFG_DRIVERS_MMC)
#include "fatfs.h"
const struct mountpt_operations fat_operations = {
    fatfs_open,        /* open */
    fatfs_close,        /* close */
    fatfs_read,        /* read */
    fatfs_write,        /* write */
    fatfs_seek,        /* seek */
    fatfs_ioctl,        /* ioctl */

    fatfs_sync,        /* sync */
    fatfs_dup,        /* dup */
    NULL,            /* fstat */
    fatfs_truncate,        /* truncate */
    fatfs_opendir,        /* opendir */
    fatfs_closedir,        /* closedir */
    fatfs_readdir,        /* readdir */
    fatfs_rewinddir,    /* rewinddir */

    fatfs_bind,        /* bind */
    fatfs_unbind,        /* unbind */
    fatfs_statfs,        /* statfs */
    fatfs_virstatfs,    /* virstatfs */
    fatfs_unlink,        /* unlinke */
    fatfs_mkdir,        /* mkdir */
    fatfs_rmdir,        /* rmdir */
    fatfs_rename,        /* rename */
    fatfs_stat,        /* stat */
    fatfs_utime,        /* utime */
    fatfs_chattr,        /* chattr */
    fatfs_seek64,        /* seek64 */
    fatfs_getlabel,        /* getlabel */
    fatfs_fallocate,    /* fallocate */
    fatfs_fallocate64,    /* fallocate64 */
    fatfs_truncate64,    /* truncate64 */
    fatfs_fscheck        /* fscheck */
};
#endif

#if defined(CONFIG_FS_YAFFS) && defined(CFG_DRIVERS_NANDFLASH)
const struct mountpt_operations yaffs_operations = {
	vfs_yaffs_open, 		 /* open */
	vfs_yaffs_close,		 /* close */
	vfs_yaffs_read, 		 /* read */
	vfs_yaffs_write,		 /* write */
	vfs_yaffs_seek, 		 /* seek */
	vfs_yaffs_ioctl,		 /* ioctl */
#ifdef __OHOS__
    OsVfsFileMmap,			 /* mmap */
#endif

	vfs_yaffs_sync, 		 /* sync */
	vfs_yaffs_dup,			 /* dup */
	NULL,				 /* fstat */
	NULL,				 /* truncate */

	vfs_yaffs_opendir,		 /* opendir */
	vfs_yaffs_closedir, 		 /* closedir */
	vfs_yaffs_readdir,		 /* readdir */
	vfs_yaffs_rewinddir,		 /* rewinddir */

	vfs_yaffs_bind, 		 /* bind */
	vfs_yaffs_unbind,		 /* unbind */
	vfs_yaffs_statfs,		 /* statfs */
	NULL,				 /* virstatfs */
	vfs_yaffs_unlink,		 /* unlinke */
	vfs_yaffs_mkdir,		 /* mkdir */
	vfs_yaffs_rmdir,		 /* rmdir */
	vfs_yaffs_rename,		 /* rename */
	vfs_yaffs_stat, 		 /* stat */
	NULL,				 /* for utime */
#ifdef __OHOS__
	vfs_yaffs_chattr,		 /* chattr */
#else
	NULL,				 /* chattr */
#endif
	vfs_yaffs_seek64,		 /* seek64 */
	NULL,				 /* getlabel */
	NULL,				 /* fallocate */
	NULL,				 /* fallocate64 */
	NULL,				 /* truncate64 */
	NULL,				 /* fscheck */
#ifdef __OHOS__
	NULL,				 /* map_pages */
	NULL,				 /* readpage */
#endif
};
#endif

#if defined(CONFIG_FS_YAFFS) && defined(CFG_DRIVERS_NANDFLASH)
const struct fsmap_t g_yaffs_operations = {"yaffs", &yaffs_operations, TRUE, TRUE};
#endif

#if defined(CONFIG_FS_FAT) && defined(CFG_DRIVERS_MMC)
const struct fsmap_t g_vfat_operations = {"vfat", &fat_operations, FALSE, TRUE};
#endif

void* g_vfs_map[] = {
#if defined(CONFIG_FS_FAT) && (defined(CFG_DRIVERS_MMC))
    (void*)&g_vfat_operations,
#endif

#if defined(CONFIG_FS_YAFFS) && defined(CFG_DRIVERS_NANDFLASH)
    (void*)&g_yaffs_operations,
#endif
};

const struct fsmap_t *mount_findfs(const char *filesystemtype)
{
    const struct fsmap_t *m = NULL;
    int i;
    for (i = 0; i < sizeof(g_vfs_map)/sizeof(g_vfs_map[0]); i++) {
        m = (const struct fsmap_t *)g_vfs_map[i];
        if (m->fs_filesystemtype && strcmp(filesystemtype, m->fs_filesystemtype) == 0) {
            return m;
        }
    }

  return (const struct fsmap_t *)NULL;
}