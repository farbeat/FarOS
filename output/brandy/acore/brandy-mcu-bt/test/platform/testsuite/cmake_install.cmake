# Install script for directory: C:/Users/25288/Desktop/sdk_mini_650/test/platform/testsuite

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
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_adc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_dma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_int/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_lcd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_mipi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_pinmux/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_pm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_pwm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_qspi_display/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_security/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_systick/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_tcxo/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_touch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_uart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_watchdog/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_sdio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_update/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_partition/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_calendar/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_nv/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_xip/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_pmp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_reboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_rtc_unified/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/25288/Desktop/sdk_mini_650/output/brandy/acore/brandy-mcu-bt/test/platform/testsuite/test_cmsis/cmake_install.cmake")
endif()

