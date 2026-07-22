#[[
Description: CMake entries of sub-directories.
Create: 2021-03-15
]]

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/posix)
    add_subdirectory(posix)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libc)
    add_subdirectory(libc)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libm)
    add_subdirectory(libm)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/securec)
    add_subdirectory(securec)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libc/xlt/csysdeps/arch/riscv32)
    add_subdirectory(libc/xlt/csysdeps/arch/riscv32)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/shell)
    #add_subdirectory(shell)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/xlt_mem)
    add_subdirectory(xlt_mem)
endif()
