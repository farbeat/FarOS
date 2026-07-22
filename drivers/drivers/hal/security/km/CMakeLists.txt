#===============================================================================
# @brief    cmake make file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "hal_km")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_key_deriv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_key_deriv_v2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_key_deriv_v2_regs_op.c
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
build_component()
