#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "boot_emmc")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_mmc_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/drv_mmc_adapter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mmc_block.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mmcsd_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sdio.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/
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