#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "hal_mem_monitor")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_mem_monitor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_mem_monitor_v150.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_mem_monitor_v150_regs_op.c
)

set(PRIVATE_HEADER
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
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

install_sdk_by_sh("${CMAKE_CURRENT_SOURCE_DIR}" "*.h")

build_component()
