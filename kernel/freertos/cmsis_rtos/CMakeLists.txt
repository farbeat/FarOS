set(COMPONENT_NAME "cmsis_rtos")

# targets list
set(CMSIS_RELEASE_VERSION CMSIS-FreeRTOS-10.4.3)
set(CMSIS_RTOS_TAR ${CMSIS_RELEASE_VERSION}.tar.gz)

set(CMSIS_SRC cmsis)
set(CMSIS_RTOS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${CMSIS_SRC}/CMSIS/RTOS2/FreeRTOS)

if(NOT EXISTS ${CMSIS_RTOS_PATH})
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar zxf ${CMSIS_RTOS_TAR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E rename ${CMSIS_RELEASE_VERSION} ${CMSIS_SRC}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E rm -fr ${CMSIS_RELEASE_VERSION}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    message("tar ${CMSIS_RTOS_TAR} done\n")
endif()

set(SOURCES
    ${CMSIS_RTOS_PATH}/Source/cmsis_os2.c
)

set(PUBLIC_HEADER
    ${CMSIS_RTOS_PATH}/Include
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
