#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "codeloader")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/private/codeloader.c
    ${CMAKE_CURRENT_SOURCE_DIR}/private/codeloader_command_processor_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/private/codeloader_configuration.c
    ${CMAKE_CURRENT_SOURCE_DIR}/private/codeloader_execute_command_authentication_commands.c
    ${CMAKE_CURRENT_SOURCE_DIR}/private/codeloader_execute_command_control_commands.c
    ${CMAKE_CURRENT_SOURCE_DIR}/private/codeloader_execute_command_flash_writing_commands.c
    ${CMAKE_CURRENT_SOURCE_DIR}/private/codeloader_execute_command_memory_commands.c
    ${CMAKE_CURRENT_SOURCE_DIR}/private/codeloader_line_layer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/private/codeloader_locking.c
    ${CMAKE_CURRENT_SOURCE_DIR}/private/codeloader_message_layer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/private/codeloader_uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/private/codeloader_efuse.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)

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
    -Wno-implicit-function-declaration
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
