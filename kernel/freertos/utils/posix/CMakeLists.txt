set(COMPONENT_NAME "posix")

set(FREERTOS_POSIX_VERSION Lab-Project-FreeRTOS-POSIX-master)
set(FREERTOS_POSIX_SRC FreeRTOS-Plus-POSIX)

set(POSIX_SRC posix)
set(PORTABLE_INC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/include/portable)
set(FREERTOS_POSIX_ZIP ${FREERTOS_POSIX_VERSION}.zip)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${FREERTOS_POSIX_VERSION})
    execute_process(
        COMMAND ${UNZIP} ${FREERTOS_POSIX_ZIP}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
message("unzip ${FREERTOS_POSIX_ZIP} done\n")

if(EXISTS  ${CMAKE_CURRENT_SOURCE_DIR}/${FREERTOS_POSIX_VERSION})
    execute_process(
        COMMAND ${CP_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${FREERTOS_POSIX_VERSION}/${FREERTOS_POSIX_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
message("cp FreeRTOS-Plus-POSIX src done\n")

if(EXISTS  ${CMAKE_CURRENT_SOURCE_DIR}/${FREERTOS_POSIX_VERSION})
    execute_process(
        COMMAND ${CP_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${FREERTOS_POSIX_VERSION}/include  ${CMAKE_CURRENT_SOURCE_DIR}/include
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
message("cp FreeRTOS-Plus-POSIX include done\n")

if(EXISTS ${PORTABLE_INC_PATH})
    execute_process(
        COMMAND ${MKDIR} ${PORTABLE_INC_PATH}/brandy
        WORKING_DIRECTORY ${PORTABLE_INC_PATH}
    )
endif()

if(EXISTS ${PORTABLE_INC_PATH}/brandy)
    execute_process(
        COMMAND ${CP} ${PORTABLE_INC_PATH}/pc/windows/FreeRTOS_POSIX_portable.h ${PORTABLE_INC_PATH}/brandy/FreeRTOS_POSIX_portable.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

if(EXISTS  ${CMAKE_CURRENT_SOURCE_DIR}/${FREERTOS_POSIX_VERSION})	
    execute_process(
        COMMAND ${RM_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${FREERTOS_POSIX_VERSION}
        COMMAND ${RM} ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    message("rm  ${CMAKE_CURRENT_SOURCE_DIR}/${FREERTOS_POSIX_VERSION} success\n")
endif()

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/source/FreeRTOS_POSIX_mqueue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/source/FreeRTOS_POSIX_pthread_barrier.c
    ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/source/FreeRTOS_POSIX_pthread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/source/FreeRTOS_POSIX_pthread_cond.c
    ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/source/FreeRTOS_POSIX_pthread_mutex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/source/FreeRTOS_POSIX_sched.c
    ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/source/FreeRTOS_POSIX_semaphore.c
    ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/source/FreeRTOS_POSIX_timer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/source/FreeRTOS_POSIX_utils.c
)

set(PUBLIC_HEADER
    ${LIB_PUBLIC_HEADER_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/private
    ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/include/portable
    ${CMAKE_CURRENT_SOURCE_DIR}/${POSIX_SRC}/include/portable/brandy
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
	PTHREAD_MUTEX_DEFAULT=2
)

set(PUBLIC_DEFINES
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
