#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
if("${CHIP}" STREQUAL "brandy")
enable_language(CXX)
set(COMPONENT_NAME "bt_host_audio_manager_wrapper")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bt_audio_manager_wrapper.cpp
)

#set(PUBLIC_HEADER
#    ${COMPILER_ROOT}/cxx/include/c++/7.3.0/
#    ${COMPILER_ROOT}/cxx/include/c++/7.3.0/riscv32-linux-musl
#    ${COMPILER_ROOT}/cxx/sys-include
#)

set(PRIVATE_HEADER
    ${ROOT_DIR}/middleware/services/media/foundation/audiomanager/interfaces/kits/audiocommon
    ${ROOT_DIR}/middleware/services/media/foundation/audiomanager/frameworks/audiostream/include/cpp
    ${ROOT_DIR}/middleware/services/media/foundation/audiomanager/interfaces/kits/audiomanager/cpp
)

set(DEFINES ${DEFINES} "_GNU_SOURCE" "_XOPEN_SOURCE=700" "CMSIS_SUPPORT")

set(PRIVATE_DEFINES
    ${DEFINES}
)

set(PUBLIC_DEFINES
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_FLAGS ${ccflags})
list(REMOVE_ITEM COMPONENT_FLAGS -std=gnu99 -std=c99 -std=gnu11 -std=c11)
list(APPEND COMPONENT_FLAGS -std=c++11 -Wno-unused-parameter -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -Wno-undef -Wno-bitwise-op-parentheses -Wno-shift-op-parentheses -nostdinc)
set(COMPONENT_CCFLAGS
    ${COMPONENT_FLAGS}
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)
if(${CHIP} MATCHES "brandy")
    if("SUPPORT_BLE" IN_LIST DEFINES AND "SUPPORT_BREDR" IN_LIST DEFINES)
        if("MEMORY_MINI" IN_LIST DEFINES)
            set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION}/${OS}/mini/br)
        else()
            set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION}/${OS}/br)
        endif()
    else()
        if("MEMORY_MINI" IN_LIST DEFINES)
            set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION}/${OS}/mini/ble)
        else()
            set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION}/${OS}/ble)
        endif()
    endif()
    #add for close component
    #set(LIB ${LIBRARY_OUTPUT_PATH})
    #add for sdk makefile
    #set(LIB_DIR ${LIBRARY_OUTPUT_PATH})
else()
    set(LIBRARY_OUTPUT_PATH ${BIN_DIR}/${CHIP}/libs/bt/${BOARD}_${APPLICATION})
endif()

build_component()
endif()
