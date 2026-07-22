#===============================================================================
# @brief    cmake file
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "efuse_porting")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/efuse_porting.c
)

set(PUBLIC_HEADER
)

set(WHOLE_LINK
    true
)

build_component()
