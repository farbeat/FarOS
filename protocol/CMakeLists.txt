#===============================================================================
# @brief    cmake make file
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================

set(_CHIP "${CHIP}")
if(${CHIP} STREQUAL "socmn1")
    set(_CHIP "melody")
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bt/controller/CMakeLists.txt)
    add_subdirectory(bt/controller btc)
elseif(EXISTS ${ROOT_DIR}/libs_url/${_CHIP}/cmake/btc.cmake)
    include(${ROOT_DIR}/libs_url/${_CHIP}/cmake/btc.cmake)
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bt/host/CMakeLists.txt)
    add_subdirectory(bt/host bth)
elseif(EXISTS ${ROOT_DIR}/libs_url/${_CHIP}/cmake/bth.cmake)
    include(${ROOT_DIR}/libs_url/${_CHIP}/cmake/bth.cmake)
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bt/host/bt/bts/CMakeLists.txt)
    add_subdirectory(bt/host/bt/bts)
elseif(EXISTS ${ROOT_DIR}/libs_url/${_CHIP}/cmake/bts.cmake)
    include(${ROOT_DIR}/libs_url/${_CHIP}/cmake/bts.cmake)
endif()
