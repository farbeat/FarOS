# Install script for directory: C:/Users/25288/Desktop/sdk_mini_650/drivers/drivers/hal

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
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/adc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/cpu_core/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/cpu_trace/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/dma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/ipc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/lpc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/l2ram/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/memory_monitor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/mipi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/mips/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/otp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/pinmux/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/pmp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/pwm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/qspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/reboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/reg_config/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/sectors/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/security/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/systick/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/tcxo/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/uart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/watchdog/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/xip/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/calendar/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/hal/rtc_unified/cmake_install.cmake")
endif()

