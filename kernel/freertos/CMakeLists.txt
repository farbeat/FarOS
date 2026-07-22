#[[
Copyright (c) CompanyNameMagicTag, Ltd. 2021-2021. All rights reserved.
Description: Module compilation script.
Author: CompanyNameMagicTag
Create: 2021-03-17
]]

add_subdirectory(FreeRTOS ${PROJECT_BINARY_DIR}/freertos/FreeRTOS)
add_subdirectory(cmsis_rtos ${PROJECT_BINARY_DIR}/freertos/cmsis_rtos)
add_subdirectory(utils ${PROJECT_BINARY_DIR}/freertos/utils)
if("FREERTOS_DEBUG" IN_LIST DEFINES)
add_subdirectory(debug ${PROJECT_BINARY_DIR}/freertos/debug)
endif()
if("FREERTOS_CPUP" IN_LIST DEFINES)
add_subdirectory(cpup ${PROJECT_BINARY_DIR}/freertos/cpup)
endif()