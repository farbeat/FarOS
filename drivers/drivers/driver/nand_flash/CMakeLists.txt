#===============================================================================
# @brief    cmake make file
# Copyright (c) 2021 @CompanyNameMagicTag
#===============================================================================
set(COMPONENT_NAME "nand_flash")
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/nand/nandflash_ds35x1g.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nand/nandflash_tc58cyg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nand/nandflash_w25n01g.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nand/nandflash_config.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nand/hal_nandflash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nand/nandflash_to_os.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nand/nandflash_qspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nand/nandflash.c
)


set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/nand/include
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