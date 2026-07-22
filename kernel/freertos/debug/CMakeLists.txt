set(COMPONENT_NAME freertos_debug)
message("[add_subdirectory]: ${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/debug_cmd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/os_handler.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_HEADER
    ${PROJECT_SOURCE_DIR}/kernel/freertos/utils/xlt_mem/include
    ${PROJECT_SOURCE_DIR}/kernel/freertos/utils/xlt_mem/src/include
    ${PROJECT_SOURCE_DIR}/kernel/freertos/utils/xlt_mem/src/bestfit
    ${PROJECT_SOURCE_DIR}/kernel/freertos/FreeRTOS/Kernel/debug/include
    ${PROJECT_SOURCE_DIR}/kernel/freertos/FreeRTOS/Kernel/include
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