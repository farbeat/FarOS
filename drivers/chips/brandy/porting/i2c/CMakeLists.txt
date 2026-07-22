#===============================================================================
# @brief    cmake file
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "i2c_porting")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/i2c_porting.c
)

set(WHOLE_LINK
    true
)

build_component()
