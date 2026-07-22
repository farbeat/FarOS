set(COMPONENT_NAME "c")

set(LIBC_DIR_PATH "${ROOT_DIR}/open_source/musl")

option(SUPPORT_LIBC_STDIO "support libc stdio" ON)
option(SUPPORT_LIBC_FORMAT "support libc format" ON)
option(CONFIG_LIBC_ARCH_RISCV_RV32IMC "32-bit RISCV architecture implementations" ON)
include(libc_src.cmake)

# musl src list
set(SOURCES
    ${LIBC_ERRNO_SRC} ${LIBC_STRING_SRC} ${LIBC_STDLIB_SRC}
    ${LIBC_INTERNAL_SRC} ${LIBC_TIME_SRC} ${LIBC_MISC_SRC}
    ${LIBC_LOCALE_SRC} ${LIBC_CTYPE_SRC} ${LIBC_TEMP_SRC}
    ${LIBC_FCNTL_SRC} ${LIBC_SIGNAL_SRC} ${LIBC_NETWORK_SRC}
    ${LIBC_UNISTD_SRC} ${LIBC_COMPLEX_SRC} ${LIBC_TERMIOS_SRC}
    ${LIBC_MULTIBYTE_SRC} ${LIBC_EXIT_SRC} ${LIBC_LEGACY_SRC}
    ${LIBC_PRNG_SRC} ${LIBC_ENV_SRC} ${LIBC_PROCESS_SRC}
    ${LIBC_LINUX_SRC}
)

set(PRIVATE_DEFINES
)
list(APPEND SOURCES ${LIBC_STDIO_BASE})
if(SUPPORT_LIBC_STDIO)
list(APPEND SOURCES ${LIBC_STDIO_SRC})
list(APPEND PRIVATE_DEFINES CONFIG_LIBC_STDIO)
endif()
if(SUPPORT_LIBC_FORMAT)
list(APPEND SOURCES ${LIBC_FORMAT_SRC})
endif()

set(LOCAL_DIRS_XLT_EXTENDED string tzdst time64 time_rtc)
list(TRANSFORM LOCAL_DIRS_XLT_EXTENDED PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/xlt/extended/)
FOREACH(CUR_DIR ${LOCAL_DIRS_XLT_EXTENDED})
    aux_source_directory(${CUR_DIR} SOURCES)
ENDFOREACH(CUR_DIR)

set(LOCAL_DIRS_XLT_LIBC errno stdlib signal lock exit time stdio conf thread c++)
#if("-D__FREERTOS__" IN_LIST MODULE_CMACRO_PRI OR "__FREERTOS__" IN_LIST MODULE_CMACRO_PRI)
list(TRANSFORM LOCAL_DIRS_XLT_LIBC PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/xlt/portable/freertos/)
FOREACH(CUR_DIR ${LOCAL_DIRS_XLT_LIBC})
    aux_source_directory(${CUR_DIR} SOURCES)
ENDFOREACH(CUR_DIR)
#endif()

file(GLOB LOCAL_ASMSRCS_LIST_TEMP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "${LIBC_DIR_PATH}/src/string/riscv32/*.s"
)
if(LOCAL_ASMSRCS_LIST_TEMP)
    list(APPEND SOURCES ${LOCAL_ASMSRCS_LIST_TEMP})
endif()
if(NOT CONFIG_LIBC_ARCH_ARM_CORTEX_M)
    file(GLOB LOCAL_ASMSRCS_LIST_TEMP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        "${LIBC_DIR_PATH}/src/string/${ARCH}/*.S"
    )
    if(LOCAL_ASMSRCS_LIST_TEMP)
        list(APPEND SOURCES ${LOCAL_ASMSRCS_LIST_TEMP})
    endif()
endif()

if(CONFIG_LIBC_ARCH_RISCV_RV32IMC)
    set(LOCAL_SRCS_REMOVED
        ${LIBC_DIR_PATH}/src/string/memcpy.c
        ${LIBC_DIR_PATH}/src/string/memset.c
        ${LIBC_DIR_PATH}/src/string/strcmp.c
    )
    list(REMOVE_ITEM SOURCES ${LOCAL_SRCS_REMOVED})
endif()

# include dir
set(PUBLIC_HEADER
    ${COMPILER_ROOT}/riscv32-elf/include/c++/v1
    ${LIBC_DIR_PATH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/xlt/include
    ${LIBC_DIR_PATH}/arch/riscv32
    ${LIBC_DIR_PATH}/arch/generic
)

if(${TOOLCHAIN_PREFIX} MATCHES "riscv")
    list(APPEND PRIVATE_DEFINES CONFIG_COMPILER_RISCV)
endif()

set(PRIVATE_HEADER
    ${LIBC_DIR_PATH}/src/include
    ${LIBC_DIR_PATH}/src/internal
    ${LIBC_DIR_PATH}/arch/riscv32
    ${LIBC_DIR_PATH}/arch/generic
    ${LIBC_DIR_PATH}/src/time
    ${CMAKE_CURRENT_SOURCE_DIR}/xlt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/xlt/extended/tzdst
    ${CMAKE_CURRENT_SOURCE_DIR}/xlt/portable
    ${LIBC_DIR_PATH}/include
)

set(PUBLIC_DEFINES
    _ALL_SOURCE
    __XLT_OS__
    __FREERTOS__
    XLTCFG_LIB_LIBC
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

if(${CHIP} MATCHES "ws63")
set(COMPONENT_CCFLAGS
    -nostdinc
    -nostdlib
    "-Wno-error=uninitialized"
    "-Wno-error=int-conversion"
    "-Wno-error=long-long"
    "-Wno-unused-variable"
    "-Wno-error=unused-variable"
    "-Wno-error=unused-value"
    "-Wno-error=unused-function"
    "-Wno-error=sign-compare"
    "-Wno-error=unused-parameter"
    "-Wno-error=tautological-compare"
    "-Wno-error=bad-function-cast"
    "-Wno-error=incompatible-pointer-types"
    "-Wno-error=implicit-function-declaration"
    "-Wno-missing-braces"
    "-Wno-missing-field-initializers"
)
else()
set(COMPONENT_CCFLAGS
    -nostdinc
    -nostdlib
    -march=rv32imfcxlinxma_xlinxmb_xlinxmc
    -mabi=ilp32f
    --target=riscv32
    "-Wno-unused-parameter"
    "-Wno-implicit-function-declaration"
    "-Wno-sign-compare"
    "-Wno-tautological-constant-out-of-range-compare"
    "-Wno-error=out-of-line-declaration"
    "-Wno-error=uninitialized"
    "-Wno-error=int-conversion"
    "-Wno-error=enum-conversion"
    "-Wno-error=long-long"
    "-Wno-unused-variable"
    "-Wno-error=unused-variable"
    "-Wno-error=unused-value"
    "-Wno-error=unused-function"
    "-Wno-error=sign-compare"
    "-Wno-error=unused-parameter"
    "-Wno-error=tautological-compare"
    "-Wno-error=bad-function-cast"
    "-Wno-error=incompatible-pointer-types"
    "-Wno-error=implicit-function-declaration"
    "-Wno-missing-braces"
    "-Wno-main-return-type"
    "-Wno-unused-command-line-argument"
    "-Wno-missing-field-initializers"
    "-Wno-string-plus-int"
    "-Wno-macro-redefined"
)
endif()

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()

install_sdk(${CMAKE_CURRENT_SOURCE_DIR} "*")