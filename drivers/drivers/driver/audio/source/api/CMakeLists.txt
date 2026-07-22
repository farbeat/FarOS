#[[
Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
Description: CMake audio api module.
Author: audio
Create: 2022-06-05
]]

set(SAP_TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
#===================================================================================
include(${SAP_TOP_DIR}/build/cmake/audio_config.cmake)
#===================================================================================
set(COMPONENT_NAME audio_api)
#===================================================================================
set(COMPONENT_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/sys/mpi_sys.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sys/uapi_sys.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/uapi_vendor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adec/mpi_adec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adec/uapi_adec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adp/mpi_adp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adp/uapi_adp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aenc/mpi_aenc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aenc/uapi_aenc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/mpi_ai.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/mpi_sea.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/mpi_vad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/uapi_ai.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/uapi_sea.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/uapi_vad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/anc/mpi_anc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/anc/uapi_anc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aoe/mpi_ao.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aoe/mpi_track.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aoe/uapi_sound.c
)

if("${CFG_SAP_HAID_SUPPORT}" STREQUAL "y")
set(COMPONENT_SRC ${COMPONENT_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/haid/mpi_haid.c
    ${CMAKE_CURRENT_SOURCE_DIR}/haid/uapi_haid.c
)
endif()

if("${CFG_SAP_DPM_SUPPORT}" STREQUAL "y")
set(COMPONENT_SRC ${COMPONENT_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/dpm/mpi_dpm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dpm/uapi_dpm.c
)
endif()

if("${CHIP}" STREQUAL "brandy")
install_sdk(${CMAKE_CURRENT_SOURCE_DIR}/dpm "*")
endif()

if("${CFG_SAP_DUMP_SUPPORT}" STREQUAL "y")
set(COMPONENT_SRC ${COMPONENT_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/dump/mpi_dump.c
)
endif()
#===================================================================================
set(COMPONENT_INC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SAP_UAPI_DIR}
    ${SAP_TOP_DIR}/include
    ${SAP_TOP_DIR}/drv/include
    ${SAP_TOP_DIR}/drv/arch/include
    ${SAP_TOP_DIR}/drv/arch/${CFG_SAP_CHIP_TYPE}/include
    ${SAP_TOP_DIR}/drv/common/include
    ${SAP_TOP_DIR}/drv/vendor/include
    ${SAP_TOP_DIR}/core/adp
    ${SAP_TOP_DIR}/core/include
    ${SAP_TOP_DIR}/core/osal/liteos
    ${SAP_TOP_DIR}/core/common/include
    ${SAP_TOP_DIR}/core/arch/${CFG_SAP_CHIP_TYPE}/include
)
#===================================================================================
set(COMPONENT_DEF
    "ADP_HOST_FLAG=1"
)
#===================================================================================
include(${SAP_TOP_DIR}/build/cmake/audio_common.cmake)
