#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
add_subdirectory_if_exist(fat)
add_subdirectory_if_exist(vfs)
add_subdirectory_if_exist(vfs/disk)
add_subdirectory_if_exist(yaffs2)
add_subdirectory_if_exist(sfs)

if("CFG_DRIVERS_NANDFLASH" IN_LIST DEFINES OR "CFG_DRIVERS_MMC" IN_LIST DEFINES)
    install_sdk(${CMAKE_CURRENT_SOURCE_DIR}/yaffs2 "*.h")
    install_sdk(${CMAKE_CURRENT_SOURCE_DIR}/vfs "*.h")
endif()