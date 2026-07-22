#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
add_subdirectory_if_exist(bootrom)
add_subdirectory_if_exist(flashboot)
add_subdirectory_if_exist(provision)

if ((${CHIP} STREQUAL "bs21") OR (${CHIP} STREQUAL "ws63"))
add_subdirectory_if_exist(commonboot)
endif()

add_subdirectory_if_exist(provision_${CHIP})
add_subdirectory_if_exist(flashboot_${CHIP})
add_subdirectory_if_exist(bootrom_${CHIP})
