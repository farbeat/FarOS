#[[
Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
Description: CMake audio drv module.
Author: audio
Create: 2022-06-05
]]

set(SAP_TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
#===================================================================================
include(${SAP_TOP_DIR}/build/cmake/audio_config.cmake)
#===================================================================================
set(COMPONENT_NAME audio_drv)
#===================================================================================
set(COMPONENT_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/sys/drv_sys.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sys/drv_msg_sys.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sys/drv_proc_sys.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ab/drv_ab.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ab/drv_msg_ab.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ab/drv_proc_ab.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adec/drv_adec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adec/drv_msg_adec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adec/drv_proc_adec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aenc/drv_aenc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aenc/drv_msg_aenc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aenc/drv_proc_aenc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adp/drv_adp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adp/drv_msg_adp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adp/drv_proc_adp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/drv_ai.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/drv_msg_ai.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/drv_msg_sea.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/drv_proc_ai.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/drv_proc_sea.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/drv_sea.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/drv_vad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/anc/drv_anc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/anc/drv_msg_anc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aoe/drv_ao.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aoe/drv_msg_ao.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aoe/drv_proc_ao.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aoe/drv_track.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aoe/drv_msg_track.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/drv_sap_msg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/drv_sap_intf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${CFG_SAP_CHIP_TYPE}/drv_hal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/drv_overlay_load.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/proc/drv_audio_proc.c
)

if("${CFG_SAP_HAID_SUPPORT}" STREQUAL "y")
set(COMPONENT_SRC ${COMPONENT_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/haid/drv_haid.c
    ${CMAKE_CURRENT_SOURCE_DIR}/haid/drv_msg_haid.c
    ${CMAKE_CURRENT_SOURCE_DIR}/haid/drv_proc_haid.c
)
endif()

if("${CFG_SAP_DPM_SUPPORT}" STREQUAL "y")
set(COMPONENT_SRC ${COMPONENT_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/dpm/drv_dpm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dpm/drv_msg_dpm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dpm/drv_proc_dpm.c
)
endif()

if("${CFG_SAP_VENDOR_DRIVER_SUPPORT}" STREQUAL "y")
set(COMPONENT_SRC ${COMPONENT_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/drv_audio_vendor.c
)
endif()

if("${CHIP}" STREQUAL "brandy")
install_sdk(${CMAKE_CURRENT_SOURCE_DIR}/dpm "*")
endif()

if("${CFG_SAP_DRV_OVERLAY_LOAD_FROM}" STREQUAL "flash")
set(COMPONENT_SRC ${COMPONENT_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/common/drv_overlay_load_from_flash.c
)
endif()

if("${CFG_SAP_DRV_OVERLAY_LOAD_FROM}" STREQUAL "file")
set(COMPONENT_SRC ${COMPONENT_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/common/drv_overlay_load_from_file.c
)
endif()
#===================================================================================
set(COMPONENT_INC
    ${SAP_UAPI_DIR}
    ${SAP_TOP_DIR}/include
    ${SAP_TOP_DIR}/core/adp
    ${SAP_TOP_DIR}/core/aie/ai
    ${SAP_TOP_DIR}/core/common
    ${SAP_TOP_DIR}/core/include
    ${SAP_TOP_DIR}/core/osal/liteos
    ${SAP_TOP_DIR}/core/common/include
    ${SAP_TOP_DIR}/core/common/sys_ctrl
    ${SAP_TOP_DIR}/core/arch/${CFG_SAP_CHIP_TYPE}/include
    ${SAP_SDK_DIR}/middleware/chips/${CFG_SAP_CHIP_TYPE}/pm/include
    ${SAP_SDK_DIR}/middleware/chips/${CFG_SAP_CHIP_TYPE}/pm/pm_adapt
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/common/include
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/include
    ${CMAKE_CURRENT_SOURCE_DIR}/sys/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ab/include
    ${CMAKE_CURRENT_SOURCE_DIR}/adec/include
    ${CMAKE_CURRENT_SOURCE_DIR}/aenc/include
    ${CMAKE_CURRENT_SOURCE_DIR}/adp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/include
    ${CMAKE_CURRENT_SOURCE_DIR}/aoe/include
    ${CMAKE_CURRENT_SOURCE_DIR}/dpm/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/include
    ${CMAKE_CURRENT_SOURCE_DIR}/anc
)
#===================================================================================
set(COMPONENT_DEF
    SOC_SUPPORT_DSP_MSG_TO_APPS
    )
#===================================================================================
include(${SAP_TOP_DIR}/build/cmake/audio_common.cmake)
