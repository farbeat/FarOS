set(COMPONENT_NAME "x_vfs")
set(FREERTOSTOPDIR ${ROOT_DIR}/open_source/freebsd)
set(NUTTX_FS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nuttx_fs)
set(FS_PATH ${ROOT_DIR}/middleware/utils/fs)

# src list
set(SOURCES
    ${NUTTX_FS_DIR}/vfs/operation/fs_fallocate64.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_unlink.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_rmdir.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_open.c
    ${NUTTX_FS_DIR}/vfs/operation/fullpath.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_utime.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_chattr.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_truncate64.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_dup.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_lseek.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_poll.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_fallocate.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_dupfd2.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_mkdir.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_statfs.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_virstatfs.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_pwrite.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_truncate.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_dupfd.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_stat.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_pwrite64.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_pread.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_rename.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_select.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_read.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_writev.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_fsync.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_ioctl.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_dup2.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_fcntl.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_initialize.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_check.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_pread64.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_readv.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_getfilep.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_close.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_init.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_getlabel.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_other.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_lseek64.c
    ${NUTTX_FS_DIR}/vfs/operation/fs_write.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_inoderelease.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_inode.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_files.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_inodesearch.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_filedetach.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_inoderemove.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_fileclose.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_fileopen.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_inodefree.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_inodereserve.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_inodeaddref.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_inodefind.c
    ${NUTTX_FS_DIR}/vfs/inode/fs_foreachinode.c
    ${NUTTX_FS_DIR}/vfs/dirent/fs_readdir.c
    ${NUTTX_FS_DIR}/vfs/dirent/fs_opendir.c
    ${NUTTX_FS_DIR}/vfs/dirent/fs_telldir.c
    ${NUTTX_FS_DIR}/vfs/dirent/fs_seekdir.c
    ${NUTTX_FS_DIR}/vfs/dirent/fs_closedir.c
    ${NUTTX_FS_DIR}/vfs/dirent/fs_rewinddir.c
    ${NUTTX_FS_DIR}/vfs/mount/fs_mount.c
    ${NUTTX_FS_DIR}/vfs/mount/fs_foreachmountpoint.c
    ${NUTTX_FS_DIR}/vfs/mount/fs_umount.c
    ${NUTTX_FS_DIR}/vfs/driver/fs_findblockdriver.c
    ${NUTTX_FS_DIR}/vfs/driver/fs_unregisterblockdriver.c
    ${NUTTX_FS_DIR}/vfs/driver/fs_openblockdriver.c
    ${NUTTX_FS_DIR}/vfs/driver/fs_registerblockdriver.c
    ${NUTTX_FS_DIR}/vfs/driver/fs_mtdproxy.c
    ${NUTTX_FS_DIR}/vfs/driver/fs_closeblockdriver.c
    ${NUTTX_FS_DIR}/vfs/driver/fs_unregisterdriver.c
    ${NUTTX_FS_DIR}/vfs/driver/fs_blockproxy.c
    ${NUTTX_FS_DIR}/vfs/driver/fs_findmtddriver.c
    ${NUTTX_FS_DIR}/vfs/driver/fs_registerdriver.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nxsem/nuttx_sem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vfs_cmd/dd_shellcmd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vfs_cmd/vfs_shellcmd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vfs_fs.c
)

add_definitions(-DCONFIG_NFILE_DESCRIPTORS=128)

set(PRIVATE_HEADER
    ${FS_PATH}/include
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/disk
    ${CMAKE_CURRENT_SOURCE_DIR}/include/operation
    ${FS_PATH}/include
	${FS_PATH}/fat/include
    ${FS_PATH}/fat/virpart/include
	${FS_PATH}/vfs/include/bcache
    ${ROOT_DIR}/kernel/dpal/include
    ${ROOT_DIR}/kernel/dpal/device/include
    ${ROOT_DIR}/kernel/osal/include
    ${FREERTOSTOPDIR}/sys/dev/mmc
)

    aux_source_directory(${FS_PATH}/yaffs2/os_adapt/adapt_freertos PUBLIC_HEADER)
if(${OS} STREQUAL "freertos")
    LIST(APPEND PRIVATE_HEADER
        ${FS_PATH}/include/adapt_freertos
    )
endif()


list(APPEND PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include/multi_partition
    ${ROOT_DIR}/drivers/drivers/driver/mtd/common/include
    ${FS_PATH}/yaffs2/yaffs/include
    ${FS_PATH}/yaffs2/yaffs/direct/include
)

set(PRIVATE_DEFINES
    _GNU_SOURCE
)

set(PUBLIC_DEFINES
    CONFIG_FS_VFS
    CONFIG_FS_FAT_CACHE
)

aux_source_directory(${FS_PATH}/yaffs2/os_adapt/adapt_freertos PUBLIC_HEADER)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-typedef-redefinition
    -Wno-unused-parameter
    -Wno-incompatible-pointer-types
    -Wno-implicit-function-declaration
    -Wno-unused-function
    -Wno-int-conversion
    -Wno-sign-compare
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()

set(COMPONENT_NAME "x_vfs_private")

# src list
set(SOURCES

    ${CMAKE_CURRENT_SOURCE_DIR}/bcache/src/bcache.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bcache/compat/rbtree.c
    ${CMAKE_CURRENT_SOURCE_DIR}/multi_partition/src/mtd_partition.c
)

add_definitions(-DCONFIG_NFILE_DESCRIPTORS=128)

set(PRIVATE_HEADER
    ${FS_PATH}/include
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${FS_PATH}/include
	${FS_PATH}/vfs/include/bcache
    ${ROOT_DIR}/kernel/dpal/include
    ${ROOT_DIR}/kernel/dpal/device/include
    ${ROOT_DIR}/kernel/osal/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/multi_partition
    ${ROOT_DIR}/drivers/drivers/driver/mtd/common/include
    ${FS_PATH}/yaffs2/yaffs/include
    ${FS_PATH}/yaffs2/yaffs/direct/include
)

if(${OS} STREQUAL "freertos")
    LIST(APPEND PRIVATE_HEADER
        ${FS_PATH}/include/adapt_freertos
    )
endif()

set(PRIVATE_DEFINES
    _GNU_SOURCE
)

set(PUBLIC_DEFINES
    CONFIG_FS_VFS
    CONFIG_FS_FAT_CACHE
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-typedef-redefinition
    -Wno-unused-parameter
    -Wno-incompatible-pointer-types
    -Wno-implicit-function-declaration
    -Wno-unused-function
    -Wno-int-conversion
    -Wno-sign-compare
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()

