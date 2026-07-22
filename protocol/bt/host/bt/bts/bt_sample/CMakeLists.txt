#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
if("${CHIP}" STREQUAL "brandy")
set(MODULE_NAME "bth")
set(AUTO_DEF_FILE_ID TRUE)
 
if ("SUPPORT_BLE" IN_LIST DEFINES)
add_subdirectory_if_exist(ble_profile)
endif()
if ("SUPPORT_BREDR" IN_LIST DEFINES)
add_subdirectory_if_exist(br_profile)
if(NOT "BRANDY_MCU_BT" IN_LIST DEFINES)
add_subdirectory_if_exist(bt_audio)
endif()
endif()
add_subdirectory_if_exist(bt_core)
add_subdirectory_if_exist(sample_common)
 
set(COMPONENT_NAME "bt_host_brandy_sample")
 
set(SOURCES
    ${BTH_SAMPLE_LIST}
    ${CMAKE_CURRENT_SOURCE_DIR}/bt_srv_shell.c
)
 
set(PRIVATE_HEADER
    ${ROOT_DIR}/include/middleware/services/bts/ble
    ${ROOT_DIR}/include/middleware/services/bts/br
    ${ROOT_DIR}/include/middleware/services/bts/common
    ${CMAKE_CURRENT_SOURCE_DIR}/ble_profile/ble_at_sample/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ble_profile/profiles/include
    ${CMAKE_CURRENT_SOURCE_DIR}/br_profile
    ${CMAKE_CURRENT_SOURCE_DIR}/bt_core
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_common
)

if(NOT "BRANDY_MCU_BT" IN_LIST DEFINES)
list(APPEND PRIVATE_HEADER
    ${CMAKE_SOURCE_DIR}/open_source/openharmony/drivers/peripheral/audio/interfaces/include
    ${CMAKE_SOURCE_DIR}/middleware/services/media/foundation/audiomanager/interfaces/kits/audiocommon
    ${CMAKE_SOURCE_DIR}/middleware/services/media/foundation/audiomanager/interfaces/kits/audiomanager/cpp
    ${CMAKE_SOURCE_DIR}/middleware/services/media/foundation/sample/wrapper
    ${CMAKE_CURRENT_SOURCE_DIR}/bt_audio
    ${CMAKE_CURRENT_SOURCE_DIR}/../bt_audio_wrapper
)
endif()
 
set(PRIVATE_DEFINES
)
 
set(PUBLIC_DEFINES
)
 
# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

if (("SUPPORT_BLE" IN_LIST DEFINES) AND ("SUPPORT_BREDR" IN_LIST DEFINES))
    set(COMPONENT_CCFLAGS ${COMPONENT_CCFLAGS} -DBTH_BREDR_LE_DOUBLE=YES -DBTH_LE_ONLY=NO -DBTH_BREDR_ONLY=NO -DCFG_SRV_BLE_HID_SUPPORTED)
elseif("SUPPORT_BLE" IN_LIST DEFINES)
    set(COMPONENT_CCFLAGS ${COMPONENT_CCFLAGS} -DBTH_BREDR_LE_DOUBLE=NO -DBTH_LE_ONLY=YES -DBTH_BREDR_ONLY=NO -DCFG_SRV_BLE_HID_SUPPORTED)
else()
    set(COMPONENT_CCFLAGS ${COMPONENT_CCFLAGS} -DBTH_BREDR_LE_DOUBLE=NO -DBTH_LE_ONLY=NO -DBTH_BREDR_ONLY=YES)
endif()
 
set(WHOLE_LINK
    true
)
 
set(MAIN_COMPONENT
    false
)
 
set(LOG_DEF
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/log/log_def_bt_sdk.h
)
if(${CHIP} MATCHES "brandy")
    if("SUPPORT_BLE" IN_LIST DEFINES AND "SUPPORT_BREDR" IN_LIST DEFINES)
        if("MEMORY_MINI" IN_LIST DEFINES)
            set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION}/${OS}/mini/br)
        else()
            set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION}/${OS}/br)
        endif()
    else()
        if("MEMORY_MINI" IN_LIST DEFINES)
            set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION}/${OS}/mini/ble)
        else()
            set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION}/${OS}/ble)
        endif()
    endif()
else()
    set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION})
endif()
build_component()
endif()
