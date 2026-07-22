#===============================================================================
# @brief    cmake make file
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "brandy_app_standard")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/chip_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/board_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/service_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/thread_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/thread_pri_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/startup.S
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.c

)

if(NOT "${TARGET_COMMAND}" STREQUAL "brandy-mcu-bt" AND
   NOT "${TARGET_COMMAND}" STREQUAL "brandy-mcu-bt-ota")
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/input_app.c)
endif()

set(PRIVATE_HEADER
    ${CMAKE_SOURCE_DIR}/middleware/utils/update/inner_include
    ${CMAKE_SOURCE_DIR}/middleware/chips/brandy/update/include
    ${CMAKE_SOURCE_DIR}/middleware/chips/brandy/partition/include
    ${CMAKE_SOURCE_DIR}/middleware/utils/nv/nv_storage_app
    ${CMAKE_SOURCE_DIR}/middleware/utils/nv/nv_storage_lib
    ${CMAKE_SOURCE_DIR}/include/middleware/utils
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
)

set(COMPONENT_CCFLAGS
    -Wno-implicit-function-declaration
)

set(WHOLE_LINK
true
)

set(BUILD_AS_OBJ
false
)

set(MAIN_COMPONENT
true
)
build_component()
