#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
if("${CHIP}" STREQUAL "brandy")
add_subdirectory_if_exist(service)
if ("SUPPORT_BREDR" IN_LIST DEFINES)
add_subdirectory_if_exist(bt_audio_hal)
endif()

set(MODULE_NAME "bth")
set(AUTO_DEF_FILE_ID TRUE)

set(COMPONENT_NAME "bt_host_brandy_service")

set(SOURCES
    ${BTH_SERVICE_LIST}
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/../bt_sample/ble_profile/profiles/include
)

set(PRIVATE_HEADER
    ${CMAKE_SOURCE_DIR}/include/middleware/services/bts/br
    ${CMAKE_SOURCE_DIR}/include/middleware/services/bts/ble
    ${CMAKE_SOURCE_DIR}/include/middleware/services/bts/common
    ${CMAKE_SOURCE_DIR}/open_source/openmg/drivers/peripheral/audio/interfaces/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/service/include
    ${CMAKE_CURRENT_SOURCE_DIR}/service/src
    ${CMAKE_CURRENT_SOURCE_DIR}/service/src/base
    ${CMAKE_CURRENT_SOURCE_DIR}/bt_audio_hal
    ${CMAKE_CURRENT_SOURCE_DIR}/../host/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/inc/stack_api
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/src/stack_api
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/src/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/log
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/sdk_common
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/gdk_common
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/ahi_common
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/ahi_a
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/host
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/host
)

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
    #add for close component
    set(LIB ${LIBRARY_OUTPUT_PATH})
    #add for sdk makefile
    set(LIB_DIR ${LIBRARY_OUTPUT_PATH})
    install_sdk(${BIN_DIR}/${CHIP}/libs/bt "CMakeLists.txt")
    install_sdk(${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION}/${OS}/mini/br "*")
    install_sdk(${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION}/${OS}/mini/ble "*")
    install_sdk(${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION}/${OS}/br "*")
    install_sdk(${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION}/${OS}/ble "*")
else()
    set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION})
endif()
build_component()
endif()

######################################### sdk end ###########################################
