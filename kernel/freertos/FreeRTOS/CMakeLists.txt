set(COMPONENT_NAME "freertos")

if(${CHIP} MATCHES "ws63")
set(DEVICE_CORE "ws63")
set(LOCAL_ARCH "RISCV31")
else()
set(DEVICE_CORE "brandy")
set(LOCAL_ARCH "RISC-V")
endif()
message(STATUS "FreeRTOS: DEVICE_CORE is ${DEVICE_CORE}")

set(KERNEL_DIR_PATH ${ROOT_DIR}/open_source/FreeRTOS-Kernel)
set(FREERTOSTOPDIR "${ROOT_DIR}/kernel/freertos")

set(SOURCES
    ${KERNEL_DIR_PATH}/list.c
    ${KERNEL_DIR_PATH}/event_groups.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Kernel/hwi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Kernel/print.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Arch/${LOCAL_ARCH}/port.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Arch/${LOCAL_ARCH}/portASM.S
    ${CMAKE_CURRENT_SOURCE_DIR}/Arch/${LOCAL_ARCH}/pmp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Arch/${LOCAL_ARCH}/cache.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Targets/board/${DEVICE_CORE}/os_adapt.c
)

if("FREERTOS_DEBUG" IN_LIST DEFINES)
list (APPEND SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Kernel/debug/src/queue_debug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Kernel/debug/src/task_debug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Kernel/debug/src/swtmr_debug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Kernel/debug/src/sysinfo_debug.c
)
else()
list (APPEND SOURCES
    ${KERNEL_DIR_PATH}/queue.c
    ${KERNEL_DIR_PATH}/tasks.c
    ${KERNEL_DIR_PATH}/timers.c
)
endif()

if(${CHIP} MATCHES "ws63")
list (APPEND SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Targets/drivers/timer/riscv_linx131_timer.c
)
elseif(${CHIP} MATCHES "brandy")
list (APPEND SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Targets/drivers/timer/riscv_linx170_timer.c
)
else()
list (APPEND SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Targets/drivers/timer/arm_sp804_timer.c
)
endif()

set(PUBLIC_HEADER
    ${KERNEL_DIR_PATH}
    ${KERNEL_DIR_PATH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Targets/board/${DEVICE_CORE}
    ${CMAKE_CURRENT_SOURCE_DIR}/Targets/board/${DEVICE_CORE}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Targets/board/${DEVICE_CORE}/include/soc
    ${CMAKE_CURRENT_SOURCE_DIR}/Targets/drivers/timer
    ${CMAKE_CURRENT_SOURCE_DIR}/Targets/drivers/timer/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Arch/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Arch/${LOCAL_ARCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/Arch/${LOCAL_ARCH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Arch/${LOCAL_ARCH}/chip_specific_extensions/RV32I_LINX_PLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Kernel/include

    ${FREERTOSTOPDIR}/include/cmsis #cmsis_os2.h
    ${FREERTOSTOPDIR}/include/cmsis/Core/Include #cmsis_compiler.h
    ${FREERTOSTOPDIR}/include/utils/securec

    ${CMAKE_CURRENT_SOURCE_DIR}/Kernel/debug/include
)

if("FREERTOS_TEST" IN_LIST DEFINES)
list (APPEND SOURCES
    ${FREERTOSTOPDIR}/Test/main_full.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/BlockQ.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/semtest.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/PollQ.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/integer.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/GenQTest.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/QPeek.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/countsem.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/dynamic.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/QueueOverwrite.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/EventGroupsDemo.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/IntSemTest.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/StaticAllocation.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/TimerDemo.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/death.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/blocktim.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/flop.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/TaskNotify.c
    ${FREERTOSTOPDIR}/Test/Demo/Common/Minimal/recmutex.c
)
list(APPEND PUBLIC_HEADER
    ${FREERTOSTOPDIR}/Test
    ${FREERTOSTOPDIR}/Test/Demo/Common/include
)
endif()

if(${ARCH} MATCHES "riscv70")
set(FREERTOS_DEFINES ARCH_LINX_170)
endif()

set(PRIVATE_HEADER
    ${KERNEL_DIR_PATH}
    ${KERNEL_DIR_PATH}/include
)

set(PRIVATE_DEFINES
    ${FREERTOS_DEFINES}
)

set(PUBLIC_DEFINES
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-sign-compare
    -Wno-unused-parameter
    -Wno-unused-function
    -Wno-implicit-function-declaration
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
