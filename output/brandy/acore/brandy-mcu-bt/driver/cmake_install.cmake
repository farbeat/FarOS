# Install script for directory: C:/Users/25288/Desktop/sdk_mini_650/drivers/drivers/driver

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
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/adc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/cpu_trace/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/dma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/ipc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/lcd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/lpc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/memory_core/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/memory_monitor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/mipi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/mmc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/nand_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/otp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/pinmux/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/pmp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/psram/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/pwm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/qspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/security/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/systick/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/tcxo/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/touch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/uart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/ulp_aon/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/watchdog/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/calendar/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/qspi_display/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/mtd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/button/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/rtc_unified/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/efuse/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/driver/audio/cmake_install.cmake")
endif()

