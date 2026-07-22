#===============================================================================
# @brief    cmake file
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "bt_manager_at_service")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bt_manager_cmd_manager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bt_manager_at_process.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bt_manager_rx_process.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bt_manager_tx_process.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bt_manager_msg_manager.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/sample/
    ${ROOT_DIR}/include/middleware/services/bts/br
    ${ROOT_DIR}/include/middleware/services/bts/common
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_AT_BT_MANAGER
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-unused-function
    -Wno-implicit-function-declaration
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
