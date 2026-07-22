#===============================================================================
# @brief    cmake make file
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "brandy_app_recovery")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/app_os_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/input_app.c
    ${CMAKE_CURRENT_SOURCE_DIR}/startup.S
)

set(PRIVATE_HEADER
    ${CMAKE_SOURCE_DIR}/middleware/utils/update/inner_include
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/version
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
)

set(COMPONENT_CCFLAGS
)

set(WHOLE_LINK
true
)

set(BUILD_AS_OBJ
false
)

set(MAIN_COMPONENT
true
)
build_component()
