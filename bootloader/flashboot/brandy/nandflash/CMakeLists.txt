#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "boot_nandflash")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/nandflash_ds35x1g.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nandflash_w25n01g.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nandflash_config.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_nandflash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nandflash_qspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nandflash.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
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
    -Wno-incompatible-function-pointer-types
    -Wno-incompatible-pointer-types
    -Wno-implicit-function-declaration
    -Wno-int-conversion
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()