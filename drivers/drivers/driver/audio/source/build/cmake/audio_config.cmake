#[[
Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
Description: CMake audio protocol module.
Author: audio
Create: 2022-06-05
]]

cmake_minimum_required(VERSION 3.14.1)

#===================================================================================
set(CFG_SAP_SDK_VERSION SAP_SDKV100R001)
set(SAP_CFGFILE "")
set(SAP_FEATURE_CONFIGS "")
set(PROJECT_ROOT_DIR ${ROOTDIR})
#===================================================================================
get_filename_component(SAP_SDK_DIR ${SAP_TOP_DIR}/../../../../.. ABSOLUTE)
set(SAP_UAPI_DIR ${SAP_SDK_DIR}/drivers/drivers/driver/audio/include)
#===================================================================================
include(${SAP_TOP_DIR}/build/cmake/read-config.cmake)

set(SAP_BOARD ${BOARD})
# Switch audio config mak for brandy evb4
if("BRANDY_PRODUCT_EVB4" IN_LIST DEFINES)
    string(APPEND SAP_BOARD "4")
if("${DSP_VERSION}" STREQUAL "ultra")
    # Ultra version:: Switch audio config mak for brandy evb4. Must use 16M FLASH:
    if(NOT ("CFG_FLASH_SIZE=16" IN_LIST DEFINES))
        message(FATAL_ERROR "Must define CFG_FLASH_SIZE=16 when dsp version is ultra")
    endif()
elseif("${DSP_VERSION}" STREQUAL "mini")
    # Mini version:: Switch audio config mak for brandy evb4 mini:
    string(APPEND SAP_BOARD "mini")
else()
    # Max version:: Switch audio config mak for brandy evb4 max:
    string(APPEND SAP_BOARD "max")
endif() #CONFIG_SEA_PHS_SUPPORT
endif() #BRANDY_PRODUCT_EVB4

if("${CHIP}" STREQUAL "brandy" OR "${CHIP}" STREQUAL "socmn1")
    set(SAP_CFGFILE ${SAP_TOP_DIR}/build/configs/${CHIP}/${CHIP}_${SAP_BOARD}_liteos_cfg.mak)
else()
    set(SAP_CFGFILE ${SAP_TOP_DIR}/build/configs/brandy/brandy_${SAP_BOARD}_liteos_cfg.mak)
endif()
message("audio SAP_CFGFILE = " ${SAP_CFGFILE})
read_config(${SAP_CFGFILE})
install_sdk(${SAP_TOP_DIR}/build/configs/${CHIP} "*.mak")
#===================================================================================
