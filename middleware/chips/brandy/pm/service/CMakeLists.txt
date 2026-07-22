#===============================================================================
# @brief    cmake make file
# Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "pm_service")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_dev.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_dfx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_fsm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_sleep.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_sys.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_tickless.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_veto.c
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
    -Wno-implicit-function-declaration
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
