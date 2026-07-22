#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
if("${CHIP}" STREQUAL "brandy")
set(MODULE_NAME "bth")
set(AUTO_DEF_FILE_ID TRUE)

if(${chip} MATCHES "bs25")
    add_subdirectory_if_exist(${CODE_DIR_BG_COMMON} bg_common_binary_dir)
else()
    add_subdirectory(../bg_common comm_binary_dir)
endif()

add_subdirectory_if_exist(src)

set(COMPONENT_NAME "bt_host_sdk")

set(SOURCES
    ${BTH_SDK_LIST}
)

set(PUBLIC_HEADER
)

if(${CHIP} MATCHES "bs25")
set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../host/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../service/wear/gdk/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/log
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/sdk_common
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/host
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/gdk_common
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/ahi_common
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/ahi_a
)
else()
set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/stack_api
    ${CMAKE_CURRENT_SOURCE_DIR}/src/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../host/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../service/wear/gdk/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stack_api
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/log
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/sdk_common
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/host
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/hci
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/gdk_common
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/ahi_common
    ${CMAKE_CURRENT_SOURCE_DIR}/../bg_common/include/ahi_a
    ${CMAKE_CURRENT_SOURCE_DIR}/../brandy_service/service/src/base
    ${ROOT_DIR}/application/wstp/at_service/bt
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
    #add for close component
    set(LIB ${LIBRARY_OUTPUT_PATH})
    #add for sdk makefile
    set(LIB_DIR ${LIBRARY_OUTPUT_PATH})
else()
    set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION})
endif()
build_component()
endif()