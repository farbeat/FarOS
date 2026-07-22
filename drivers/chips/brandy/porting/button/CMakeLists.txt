#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "button_port")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/button_porting.c
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
    ${CMAKE_SOURCE_DIR}/drivers/drivers/driver/button
)

set(WHOLE_LINK
    true
)

build_component()
