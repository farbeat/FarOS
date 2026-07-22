#===============================================================================
# @brief    cmake file
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "app_at_service")

if("${TARGET_COMMAND}" STREQUAL "brandy-mcu-bt" OR
   "${TARGET_COMMAND}" STREQUAL "brandy-mcu-bt-ota")
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/app_msg_manager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/app_at_mcu_bt_stub.c
)
else()
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/app_cmd_manager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/app_at_process.c
    ${CMAKE_CURRENT_SOURCE_DIR}/app_rx_process.c
    ${CMAKE_CURRENT_SOURCE_DIR}/app_tx_process.c
    ${CMAKE_CURRENT_SOURCE_DIR}/app_msg_manager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/app_test_fs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/app_common_api.c
    ${CMAKE_CURRENT_SOURCE_DIR}/app_test_perf.c
)
endif()

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_HEADER
)

if (DEFINED BUILD_TIME)
    set(PRIVATE_DEFINES
        TARGET_NAME="${TARGET_COMMAND}"
        BUILD_TIME="${BUILD_TIME}"
        SUPPORT_AT_APP
    )
else()
    set(PRIVATE_DEFINES
    )
endif()

set(PUBLIC_DEFINES
    SUPPORT_AT_APP
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-implicit-function-declaration
    -Wno-unused-parameter
    -Wno-int-conversion
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
