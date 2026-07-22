#===============================================================================
# @brief    cmake make file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "km")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/key_deriv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/keyslot.c
    ${CMAKE_CURRENT_SOURCE_DIR}/klad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/km.c
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
    -Wno-implicit-function-declaration
)

set(WHOLE_LINK
true
)
build_component()
