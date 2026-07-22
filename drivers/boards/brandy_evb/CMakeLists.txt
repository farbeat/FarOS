#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================

set(COMPONENT_NAME "board_brandy")

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/board_config
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_config/include
    ${CMAKE_CURRENT_SOURCE_DIR}/product

)
build_component()

add_subdirectory(linker)
