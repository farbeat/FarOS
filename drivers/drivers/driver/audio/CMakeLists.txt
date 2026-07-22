#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(MODULE_NAME "dsp")
set(AUTO_DEF_FILE_ID TRUE)
if(NOT "audio_drv" IN_LIST TARGET_COMPONENT)
    return()
endif()

if("${CHIP}" STREQUAL "socmn1")
    set(PROJECT_CHIP_DIR melody)
else()
    set(PROJECT_CHIP_DIR ${CHIP})
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/source/CMakeLists.txt)
    set(AUDI_PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include)
    add_subdirectory(source)
elseif(EXISTS ${BIN_DIR}/${PROJECT_CHIP_DIR}/libs/audio/CMakeLists.txt)
    set(PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include)
    add_subdirectory(${BIN_DIR}/${PROJECT_CHIP_DIR}/libs/audio audio)
endif()
