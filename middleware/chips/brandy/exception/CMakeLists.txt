#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "dfx_exception")
if(${ARCH_FAMILY} STREQUAL "arm")
    set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/exception_arm.s
        ${CMAKE_CURRENT_SOURCE_DIR}/exception_arm.c
    )
elseif (${ARCH_FAMILY} STREQUAL "riscv")
    set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/exception_riscv.s
        ${CMAKE_CURRENT_SOURCE_DIR}/exception_riscv.c
        ${CMAKE_CURRENT_SOURCE_DIR}/chip_system_time.c
        ${CMAKE_CURRENT_SOURCE_DIR}/chip_exception_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/exception_dump.c
    )
endif()


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
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

install_sdk_by_sh("${CMAKE_CURRENT_SOURCE_DIR}" "*.h")

build_component()
