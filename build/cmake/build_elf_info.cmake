#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================

set(BUILT_LDS ${CMAKE_BINARY_DIR}/linker.lds)

if (DEFINED BUILD_LEVEL AND ${BUILD_LEVEL} STREQUAL "debug")
    set(EXTRA_DUMP_OPT -S -l)
endif()

if (DEFINED NHSO AND "${NHSO}" STREQUAL "True")
    set(GEN_PARSE_TOOL False)
endif()

if((NOT DEFINED DEBUG_FILES OR "lst" IN_LIST DEBUG_FILES OR "mem" IN_LIST DEBUG_FILES) AND NOT ${ROM_CHECK})
    add_custom_target(GENERAT_LST ALL
        COMMAND ${CMAKE_OBJDUMP} -x ${EXTRA_DUMP_OPT} ${BIN_NAME}.elf > ${BIN_NAME}.lst 2>&1
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${TARGET_NAME}
    )
endif()

if(NOT DEFINED DEBUG_FILES OR "nm" IN_LIST DEBUG_FILES OR "${GEN_PARSE_TOOL}" STREQUAL "True")
    add_custom_target(GENERAT_NM ALL
        COMMAND ${CMAKE_NM} -S -n  --format=sysv ${BIN_NAME}.elf > ${BIN_NAME}.nm
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${TARGET_NAME}
    )
endif()

if(NOT DEFINED DEBUG_FILES OR "asm" IN_LIST DEBUG_FILES)
    add_custom_target(GENERAT_ASM ALL
        COMMAND ${CMAKE_OBJDUMP} -d -m ${ARCH_FAMILY} ${BIN_NAME}.elf > ${BIN_NAME}.asm 2>&1
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${TARGET_NAME}
    )
endif()

if("${GEN_PARSE_TOOL}" STREQUAL "True")
    add_custom_target(GENERAT_INFO ALL
        COMMAND ${CMAKE_OBJDUMP} -Wi ${BIN_NAME}.elf > ${BIN_NAME}.info 2>&1
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${TARGET_NAME}
    )
endif()

if((NOT DEFINED DEBUG_FILES OR "mem" IN_LIST DEBUG_FILES) AND NOT ${ROM_CHECK})
add_custom_target(GENERAT_MEM ALL
    COMMAND ${Python3_EXECUTABLE} ${ELF_TO_DU} ${ROOT_DIR} ${BIN_NAME}.elf ${CMAKE_NM} > ${BIN_NAME}.du
    COMMAND ${Python3_EXECUTABLE} ${MEM_STATE} ${BIN_NAME}.lst ${BIN_NAME}.du ${BUILT_LDS} ${CHIP} > ${BIN_NAME}.mem
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    DEPENDS "${TARGET_NAME};GENERAT_LST"
)
endif()

if(NOT DEFINED DEBUG_FILES OR "hex" IN_LIST DEBUG_FILES)
add_custom_target(GENERAT_HEX ALL
    COMMAND ${Python3_EXECUTABLE} ${GEN_HEX} ${BIN_NAME}.bin ${BIN_NAME}.hex
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    DEPENDS GENERAT_BIN
)
if(DEFINED ROM_COMPONENT)
    add_custom_target(GENERAT_ROM_HEX ALL
        COMMAND ${Python3_EXECUTABLE} ${GEN_HEX} ${BIN_NAME}_rom.bin ${BIN_NAME}_rom.hex
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS GENERAT_BIN
    )
endif()
endif()

if (DEFINED BUILD_LEVEL AND ${BUILD_LEVEL} STREQUAL "debug")
if("${IMAGE_ANALYSIS}" STREQUAL "True")
    add_custom_target(GENERAT_IMAGE_ANALYSIS ALL
        COMMAND ${Python3_EXECUTABLE} ${IMAGE_ANALSIS_TOOL} ${TARGET_COMMAND} ${OBJDUMP_PATH}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${TARGET_NAME}
    )
endif()

if("${CODESIZE_STATISTIC}" STREQUAL "True")
    add_custom_target(GENERAT_CODESIZE_STATISTIC ALL
        COMMAND ${Python3_EXECUTABLE} ${CODESIZE_STATISTIC_TOOL} ${TARGET_COMMAND}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${TARGET_NAME}
    )
endif()
endif()

if(NOT DEFINED PATCH)
    set(PATCH False)
endif()

if((DEFINED ROM_COMPONENT AND ${PATCH}) AND NOT ${ROM_CHECK})
    set(PATCH_CONFIG ${ROOT_DIR}/build/config/target_config/${CHIP}/patch_config)
    add_custom_target(GENERAT_ROM_PATCH ALL
        COMMAND ${Python3_EXECUTABLE} ${GEN_PATCH} ${BIN_NAME}.bin ${BIN_NAME}_rom.bin ${BIN_NAME}.nm ${PATCH_CONFIG} ${CORE} ${TARGET_COMMAND} ${PROJECT_BINARY_DIR}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS GENERAT_BIN
    )
endif()