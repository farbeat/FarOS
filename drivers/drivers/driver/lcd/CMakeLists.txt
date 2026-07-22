#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "lcd")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/lcd_bus.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lcd_qspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lcd_mipi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lcd_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lcd_config.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lcd_private.c
    ${CMAKE_CURRENT_SOURCE_DIR}/soc_lcd_api.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-incompatible-pointer-types-discards-qualifiers
    -Wno-implicit-function-declaration
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
