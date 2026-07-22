#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================

add_subdirectory(porting)

set(COMPONENT_NAME "chip_brandy")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv70/interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv70/interrupt_handler.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv70/vectors.s
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/interrupt_adapter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/interrupt_os_adapter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/vectors.c
    ${CMAKE_CURRENT_SOURCE_DIR}/config/pmp_config.c
)

if(DEFINED OS AND NOT ${OS} STREQUAL "non-os")
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv70/oam_trace.c)
endif()

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/include
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv70
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv_common
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/
    ${CMAKE_CURRENT_SOURCE_DIR}/porting/include
    ${CMAKE_SOURCE_DIR}/drivers/drivers/driver/button
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-implicit-function-declaration
    -Wno-unused-function
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()

set(COMPONENT_NAME "brandy_clocks")

set(SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/clocks/drivers/acore/clocks_config_priv.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks/drivers/acore/clocks.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/hal_clocks_app.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/hal_clocks_app_priv.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/hal_clocks_glb.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/hal_clocks_sdio.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/hal_clocks.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/hal_cpu_clocks.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/acore/clocks_core.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/acore/clocks_switch_priv.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/acore/clocks_switch.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/shared/clock_calibration.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/shared/clocks_core_common.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/hal/hal_32k_clock.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/drivers/acore
    # ${CMAKE_CURRENT_SOURCE_DIR}/clocks/drivers/shared
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/acore
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/shared
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/hal
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_CLOCKS
    SUPPORT_CLOCKS_CORE
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-unused-function
    -Wno-incompatible-pointer-types-discards-qualifiers
    -Wno-implicit-function-declaration
    -Wno-int-conversion
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()

set(COMPONENT_NAME "pmu_brandy")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/drivers/acore/pmu_cmu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/drivers/acore/pmu_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/drivers/acore/pmu_ldo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/drivers/acore/pmu_pg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/drivers/acore/pmu_vdd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/drivers/shared/pmu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/hal/hal_pmu_cmu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/hal/hal_pmu_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/hal/hal_pmu_ldo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/hal/hal_pmu_pg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/hal/hal_pmu_vdd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/hal/hal_pmu_peripheral_app.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/drivers/acore
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/drivers/shared
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/hal
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_CLOCKS
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-implicit-function-declaration
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
