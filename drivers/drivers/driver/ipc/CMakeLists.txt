#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "ipc_old")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/ipc_old/${CORE}/ipc_irq.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ipc_old/shared/ipc.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/ipc_old/${CORE}
    ${CMAKE_CURRENT_SOURCE_DIR}/ipc_old/shared
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_IPC
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-incompatible-function-pointer-types
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

install_sdk_by_sh("${CMAKE_CURRENT_SOURCE_DIR}/ipc_old/shared" "*.h")

build_component()

set(COMPONENT_NAME "ipc")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/ipc/ipc.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/ipc
)

if(DEFINED CONFIG_IPC_USING_RING_BUFFER_MAILBOX)
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ipc/ipc_ring.c)
endif()

if(DEFINED CONFIG_IPC_USING_SW_MAILBOX)
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ipc/ipc_sw_mail.c)
endif()

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
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