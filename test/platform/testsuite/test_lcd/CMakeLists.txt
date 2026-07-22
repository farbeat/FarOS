#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "test_lcd")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_lcd_api.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_screen_ddic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_screen_edo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_screen_kdk.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_screen_lcd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_screen_ruxin.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_screen_weitai.c
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
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
