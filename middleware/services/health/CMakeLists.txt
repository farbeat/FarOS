# Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved.
# Description: CMake configuration for FarOS Health Service.
# Author: 国体智慧体育技术创新中心

set(MODULE_NAME "health_service")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/health_service_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/far_sensor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/far_activity.c
    ${CMAKE_CURRENT_SOURCE_DIR}/far_physio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/far_sleep.c
    ${CMAKE_CURRENT_SOURCE_DIR}/far_health_data.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/middleware/services/health
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Create static library
add_library(${MODULE_NAME} STATIC ${SOURCES})

# Set include directories
target_include_directories(${MODULE_NAME}
    PUBLIC
        ${PUBLIC_HEADER}
    PRIVATE
        ${PRIVATE_HEADER}
        ${ROOT_DIR}/include
        ${ROOT_DIR}/include/driver
        ${ROOT_DIR}/include/middleware/utils
        ${ROOT_DIR}/kernel/osal/include
        ${ROOT_DIR}/kernel/freertos/include
        ${ROOT_DIR}/middleware/utils/algorithm/cstl
)

# Link dependencies
target_link_libraries(${MODULE_NAME}
    PUBLIC
        osal
    PRIVATE
        dfx
        fs
        nv
)

# Set compile options
target_compile_options(${MODULE_NAME} PRIVATE
    -Wall
    -Wextra
    -Werror=return-type
)

# Print info
message(STATUS "FarOS Health Service module configured")
message(STATUS "  Sources: ${SOURCES}")
message(STATUS "  Public headers: ${PUBLIC_HEADER}")
