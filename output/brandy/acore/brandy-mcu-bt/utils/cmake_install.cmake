# Install script for directory: C:/Users/25288/Desktop/sdk_mini_650/middleware/utils

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/brandy_CFBB")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Users/25288/Desktop/sdk_mini_650/tools/bin/compiler/linx/linx_170/linx-llvm-binary-debug/bugfix/win/bin/riscv32/riscv32-linux-musl-objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/algorithm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/app_version/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/at/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/bootloader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/build_version/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/codeloader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/codeloader_ssb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/common_headers/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/connectivity/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/cpu_load/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/dfx/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/error_code/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/fs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/lib_utils/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/nv/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/partition/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/sec_boot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/utils/update/cmake_install.cmake")
endif()

