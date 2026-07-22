#===============================================================================
# @brief    cmake make file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
add_subdirectory_if_exist(wstp)

if("${CHIP}" STREQUAL "socmn1")
    add_subdirectory_if_exist(melody)
else()
    add_subdirectory_if_exist(${CHIP})
endif()
