#===============================================================================
# @brief    cmake make file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP})
    add_subdirectory(${CHIP})
endif()