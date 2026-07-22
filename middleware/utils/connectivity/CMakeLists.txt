#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "connectivity")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/common/${CORE}/connectivity_calibration.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/shared/connectivity_log.c
    )

if(${CORE} STREQUAL "acore")
    set(SOURCES
        ${SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/common/acore/identity.c
    )
endif()

#ifdef __LITEOS__
if(NOT ((${CHIP} MATCHES "bs25")  OR ("TARGET_CHIP_SOCMN1=1" IN_LIST DEFINES)))
    list(APPEND SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/common/${CORE}/los_status.c
        )
endif()
#endif

if(NOT ((${CHIP} MATCHES "brandy") OR ("TARGET_CHIP_SOCMN1=1" IN_LIST DEFINES) OR (${CHIP} MATCHES "bs25")))
    list(APPEND SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/common/${CORE}/low_power_control.c
        ${CMAKE_CURRENT_SOURCE_DIR}/common/shared/connectivity_sleep_internal.c
        ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}/${CORE}/connectivity_sleep.c
        )
endif()

if(NOT ((${CHIP} MATCHES "brandy") OR ("TARGET_CHIP_BS25=1" IN_LIST DEFINES)))
    if(${CORE} STREQUAL "acore")
        list(APPEND SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/common/acore/low_power_management.c
        )
    endif()
endif()

if(${CHIP} MATCHES "bs25")
    if(${CORE} STREQUAL "acore")
        list(APPEND SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}/acore/low_power_control_private.c
        )
    endif()
endif()

if ("TARGET_CHIP_SOCMN1=1" IN_LIST DEFINES)
    list(APPEND SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}/${CORE}/low_power_control_private.c
        ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}/${CORE}/connectivity_sleep.c
        ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}/${CORE}/los_status.c
        ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}/${CORE}/low_power_control.c
    )
endif()

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_CONNECTIVITY
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

install_sdk_by_sh("${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}" "*.h")
install_sdk_by_sh("${CMAKE_CURRENT_SOURCE_DIR}/common" "*.h")

build_component()


set(COMPONENT_NAME "connectivity_ssb")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/common/acore/connectivity_calibration.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/acore/identity.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/acore/los_status.c
)

if(NOT ((${CHIP} MATCHES "brandy") OR ("TARGET_CHIP_BS25=1" IN_LIST DEFINES)))
    list(APPEND SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/common/acore/low_power_control.c
        ${CMAKE_CURRENT_SOURCE_DIR}/common/acore/low_power_management.c
    )
endif()

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}
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
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

install_sdk_by_sh("${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}" "*.h")
install_sdk_by_sh("${CMAKE_CURRENT_SOURCE_DIR}/common" "*.h")

build_component()
