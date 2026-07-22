#===============================================================================
# @brief    cmake file
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "km_port")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/keyslot_porting.c
    ${CMAKE_CURRENT_SOURCE_DIR}/klad_porting.c
    ${CMAKE_CURRENT_SOURCE_DIR}/key_deriv_porting.c
)

set(PUBLIC_HEADER
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
