#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "at_cmd")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/at_cmd_task.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

set(PRIVATE_HEADER
    ${ROOT_DIR}/middleware/chips/brandy/nv/nv_config/include/
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_AT_CMD
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
