#===============================================================================
# @brief    cmake toolchains (Windows only)
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================

function(find_env_exe var full_path exe)
    if(USE_LOCAL_COMPILER)
        file(TO_CMAKE_PATH ${full_path} full_path)
        set(${var} ${full_path}/${exe} CACHE PATH "" FORCE)
    else()
        find_program(${var} ${exe})
    endif()
endfunction(find_env_exe)

set(COMPILER_ROOT ${PROJECT_SOURCE_DIR}/tools/bin/compiler/linx/linx_170/linx-llvm-binary-debug/linx-llvm-binary-release-win-musl)
set(COMPILER_BUGFIX ${PROJECT_SOURCE_DIR}/tools/bin/compiler/linx/linx_170/linx-llvm-binary-debug/bugfix)
set(OBJDUMP_PATH ${COMPILER_ROOT}/bin/riscv32)

if(EXISTS ${COMPILER_ROOT})
    set(USE_LOCAL_COMPILER TRUE)
else()
    set(USE_LOCAL_COMPILER FALSE)
endif()

MESSAGE(STATUS "BUILD_PLATFORM: windows")
find_env_exe(CMAKE_C_COMPILER           ${COMPILER_ROOT}/bin           clang.exe)
find_env_exe(CMAKE_ASM_COMPILER         ${COMPILER_ROOT}/bin           clang.exe)
find_env_exe(CMAKE_CXX_COMPILER         ${COMPILER_ROOT}/bin           clang++.exe)
find_env_exe(CMAKE_LINKER               ${COMPILER_ROOT}/bin/riscv32   riscv32-linux-musl-ld.exe)
find_env_exe(CMAKE_AR                   ${COMPILER_ROOT}/bin/riscv32   riscv32-linux-musl-ar.exe)
find_env_exe(CMAKE_C_COMPILER_AR        ${COMPILER_ROOT}/bin/riscv32   riscv32-linux-musl-ar.exe)
find_env_exe(CMAKE_ASM_COMPILER_AR      ${COMPILER_ROOT}/bin/riscv32   riscv32-linux-musl-ar.exe)

find_env_exe(CMAKE_RANLIB               ${COMPILER_ROOT}/bin/riscv32   riscv32-linux-musl-ranlib.exe)
find_env_exe(CMAKE_C_COMPILER_RANLIB    ${COMPILER_ROOT}/bin/riscv32   riscv32-linux-musl-ranlib.exe)
find_env_exe(CMAKE_ASM_COMPILER_RANLIB  ${COMPILER_ROOT}/bin/riscv32   riscv32-linux-musl-ranlib.exe)

find_env_exe(CMAKE_STRIP                ${COMPILER_ROOT}/bin/riscv32   riscv32-linux-musl-strip.exe)
find_env_exe(CMAKE_NM                   ${COMPILER_ROOT}/bin/riscv32   riscv32-linux-musl-nm.exe)
find_env_exe(CMAKE_OBJCOPY              ${COMPILER_ROOT}/bin/riscv32   riscv32-linux-musl-objcopy.exe)
find_env_exe(CMAKE_OBJDUMP              ${COMPILER_BUGFIX}/win/bin/riscv32   riscv32-linux-musl-objdump.exe)
find_env_exe(CMAKE_READELF              ${COMPILER_ROOT}/bin/riscv32   riscv32-linux-musl-readelf.exe)
set(LIB_GCC "" CACHE PATH "" FORCE)

if(DEFINED CMAKE_C_COMPILER)
    STRING(REGEX REPLACE "(.+)/.+" "\\1" CMAKE_C_COMPILER_PATH ${CMAKE_C_COMPILER})
    set(LIB_C ${CMAKE_C_COMPILER_PATH}/../riscv32-elf/lib/rv32imfc_ilp32f CACHE PATH "" FORCE)
endif()
