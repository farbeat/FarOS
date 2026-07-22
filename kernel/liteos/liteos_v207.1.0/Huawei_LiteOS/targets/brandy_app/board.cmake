# C sources
set(CFBB_DRV_INCLUDE 
    ${ROOT_DIR}/drivers/chips/${CHIP}/clocks_core/drivers/${CORE}/
    ${ROOT_DIR}/drivers/chips/${CHIP}/clocks_core/drivers/shared/
    ${ROOT_DIR}/drivers/chips/${CHIP}/include/
    ${ROOT_DIR}/middleware/utils/common_headers/
    ${ROOT_DIR}/include
    ${ROOT_DIR}/application/wearable/SystemView/Src/SEGGER
    ${ROOT_DIR}/application/wearable/SystemView/Src/Config
    ${ROOT_DIR}/application/wearable/SystemView/Src/Sample/LiteOS

)

list(APPEND LOCAL_SRCS_y ${USER_SRC_TEMP})
list(APPEND MODULE_INCLUDE_PUB ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND MODULE_INCLUDE_PUB ${CFBB_DRV_INCLUDE})