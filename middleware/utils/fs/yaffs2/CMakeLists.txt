set(COMPONENT_NAME "fs_yaffs2")
set(FS_PATH ${ROOT_DIR}/middleware/utils/fs)
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/os_adapt/mtd_nandcfg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/os_adapt/vfs_yaffs2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/os_adapt/yaffs_nandcfg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/os_adapt/yaffs_osglue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_allocator.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_bitmap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_checkptrw.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_ecc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_endian.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_guts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_nameval.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_nand.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_packedtags1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_packedtags2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_summary.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_tagscompat.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_tagsmarshall.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_verify.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_yaffs1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/yaffs_yaffs2.c

    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/direct/yaffs_attribs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/direct/yaffs_hweight.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/direct/yaffs_nand_drv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/direct/yaffs_qsort.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/direct/yaffsfs.c
)

set(PUBLIC_HEADER
    ${FS_PATH}/include
    ${FS_PATH}/vfs/include/disk
    ${FS_PATH}/vfs/include/multi_partition
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/direct/include
    ${CMAKE_CURRENT_SOURCE_DIR}/yaffs/include
)

LIST(APPEND PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/os_adapt/adapt_freertos)

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
    CONFIG_FS_YAFFS
)


set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-typedef-redefinition
    -Wno-unused-parameter
    -Wno-int-conversion
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
