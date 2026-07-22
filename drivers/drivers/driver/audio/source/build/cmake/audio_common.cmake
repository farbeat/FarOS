#[[
Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
Description: CMake audio protocol module.
Author: audio
Create: 2022-06-05
]]

cmake_minimum_required(VERSION 3.14.1)

# 添加配置宏定义
set(CCDEFINES
    ${DEFINES}
    ${SAP_FEATURE_CONFIGS}
    ${COMPONENT_DEF}
    "LOSCFG_COMPAT_CMSIS_VER_2"
    "SAP_OS_TYPE=os_type_${CFG_SAP_CPU_OS}"
    )

# 添加配置宏定义
set(CCOPTIONS
    )

set(COMPONENT_NAME "${COMPONENT_NAME}")

set(SOURCES
    ${COMPONENT_SRC}
)

set(PUBLIC_HEADER
    ${AUDI_PUBLIC_HEADER}
)

set(PRIVATE_HEADER
    ${COMPONENT_INC}
)

set(PRIVATE_DEFINES
    ${CCDEFINES}

)

set(PUBLIC_DEFINES
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    ${CCOPTIONS}
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

set(AUTO_ADD_PATH
    false
)

set(SAP_BOARD ${BOARD})
# Switch audio libs dir for brandy evb4
if("BRANDY_PRODUCT_EVB4" IN_LIST DEFINES)
    string(APPEND SAP_BOARD "4")
endif()

if("${CHIP}" STREQUAL "socmn1")
    set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/melody/libs/audio/${SAP_BOARD}_${APPLICATION}/${OS})
else()
    set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/audio/${SAP_BOARD}_${APPLICATION}/${OS})
endif()

build_component()

