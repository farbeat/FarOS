#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "uart_port")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/uart_porting.c
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
    ${ROOT_DIR}/include/middleware/services/bts
    ${ROOT_DIR}/include/middleware/services/bts/ble
    ${ROOT_DIR}/include/middleware/services/bts/br
    ${ROOT_DIR}/include/middleware/services/bts/common
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    CONFIG_UART_FIFO_DEPTH=64
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
