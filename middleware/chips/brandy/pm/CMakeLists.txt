#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
add_subdirectory_if_exist(service)

set(COMPONENT_NAME "pm_brandy")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_adapt/pm_clocks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_adapt/pm_config.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_adapt/pm_control.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_adapt/pm_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_adapt/pm_lp_stub.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_adapt/pm_lpc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_adapt/pm_os.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_adapt/pm_pmu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_adapt/lowpower.S
    ${CMAKE_CURRENT_SOURCE_DIR}/pm.c


)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/pm_adapt
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
    -Wno-incompatible-pointer-types
    -Wno-int-conversion
    -Wno-enum-conversion
    -Wno-unused-parameter
    -Wno-unused-function
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
