set(COMPONENT_NAME "x_fat")
set(FREERTOSTOPDIR ${ROOT_DIR}/open_source/freebsd)
set(FATFS_PATH ${ROOT_DIR}/open_source/FatFs)
set(FS_PATH ${ROOT_DIR}/middleware/utils/fs)
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/os_adapt/fatfs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/os_adapt/fatfs_dentry.c
	${CMAKE_CURRENT_SOURCE_DIR}/os_adapt/fat_shellcmd.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/format.c
    ${FATFS_PATH}/source/ffsystem.c
    ${FATFS_PATH}/source/ff.c
    ${FATFS_PATH}/source/ffunicode.c
    ${FATFS_PATH}/source/diskio.c
)

set(PUBLIC_HEADER
	${FS_PATH}/include
	${FS_PATH}/fat/include
	${FS_PATH}/fat/virpart/include
    ${FS_PATH}/fat/dcache/include
	${FS_PATH}/vfs/include/disk
    ${FS_PATH}/vfs/include
	${FS_PATH}/vfs/include/bcache
    ${FATFS_PATH}/source
    ${ROOT_DIR}/kernel/dpal/include
    ${ROOT_DIR}/kernel/dpal/device/include
    ${ROOT_DIR}/kernel/osal/include

)

set(PRIVATE_HEADER
)

if(${OS} STREQUAL "freertos")
    LIST(APPEND PRIVATE_HEADER
        ${FS_PATH}/include/adapt_freertos
    )
endif()

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    CONFIG_FS_FAT
)


set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -DLOSCFG_FS_FAT_VOLUMES=16
    -Wno-typedef-redefinition
    -Wno-unused-parameter
    -Wno-sign-compare
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()