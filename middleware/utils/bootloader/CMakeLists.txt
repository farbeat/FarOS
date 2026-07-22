#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "bootloader")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/private/bootloader_configuration.c
    ${CMAKE_CURRENT_SOURCE_DIR}/private/bootloader_utils.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)

set(PRIVATE_HEADER
    ${ROOT_DIR}/drivers/drivers/driver/security/hash
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
