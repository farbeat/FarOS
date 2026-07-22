#===============================================================================
# @brief    cmake file
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "spi_porting")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/spi_porting.c
)

set(WHOLE_LINK
    true
)

build_component()
