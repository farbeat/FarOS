#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "brandy_ssb_lds")

if(NOT ${COMPONENT_NAME} IN_LIST TARGET_COMPONENT)
    return()
endif()

set(LINK_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.prelds" CACHE INTERNAL "" FORCE)
set(LDS_HEADER ${ROOT_DIR}/drivers/boards/${CHIP}_${BOARD}/board_config
    ${ROOT_DIR}/drivers/boards/${CHIP}_${BOARD}/memory_config/include
    ${ROOT_DIR}/drivers/boards/${CHIP}_${BOARD}/product
    ${ROOT_DIR}/drivers/chips/${CHIP}/include
    ${ROOT_DIR}/middleware/utils/common_headers
    CACHE INTERNAL "" FORCE)
build_component()
