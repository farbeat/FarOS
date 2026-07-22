#[[
Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
Description: CMake audio core module.
Author: audio
Create: 2022-06-05
]]

set(SAP_TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
#===================================================================================
include(${SAP_TOP_DIR}/build/cmake/audio_config.cmake)
#===================================================================================
set(COMPONENT_NAME audio_core)
#===================================================================================
set(COMPONENT_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/osal/liteos/audio_osal_addr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/osal/liteos/audio_osal_debug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/osal/liteos/audio_osal_timer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/osal/liteos/drv_audio_osal_proc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adp/core_adp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/core_util.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/core_module.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/share_mem/core_msg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/share_mem/core_shrb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/share_mem/core_rbuf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/sys_ctrl/sys_cmd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/probe/probe.c
)
string(FIND "${SAP_FEATURE_CONFIGS}" "SAP_TIME_CONSUME" TIME_CONSUME_STAT_SUPPORT)
if(${TIME_CONSUME_STAT_SUPPORT} GREATER_EQUAL 0)
    list(APPEND COMPONENT_SRC ${SAP_TOP_DIR}/core/common/time_consume.c)
endif()
#===================================================================================
set(COMPONENT_INC
    ${SAP_UAPI_DIR}
    ${SAP_TOP_DIR}/include
    ${SAP_TOP_DIR}/drv/include
    ${SAP_TOP_DIR}/drv/arch/include
    ${SAP_TOP_DIR}/drv/common/include
    ${SAP_TOP_DIR}/core/include
    ${SAP_TOP_DIR}/core/adp
    ${SAP_TOP_DIR}/core/common
    ${SAP_TOP_DIR}/core/osal/liteos
    ${SAP_TOP_DIR}/core/common/include
    ${SAP_TOP_DIR}/core/arch/${CFG_SAP_CHIP_TYPE}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/hal/include
    ${CMAKE_CURRENT_SOURCE_DIR}/hal/afe/dallas_v100
    ${CMAKE_CURRENT_SOURCE_DIR}/aie/ai/include
)
#===================================================================================
set(COMPONENT_DEF
)
#===================================================================================
include(${SAP_TOP_DIR}/build/cmake/audio_common.cmake)
