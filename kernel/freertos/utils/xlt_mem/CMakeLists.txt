set(COMPONENT_NAME "xlt_mem")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/multipool/xlt_multipool.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/debug/xlt_mem_debug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/debug/xlt_slab_debug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bestfit/xlt_mem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bestfit/xlt_multipledlinkhead.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/slab/xlt_slab.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/slab/xlt_slabmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/freertos_mem.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    XLTCFG_SUPPORT_MEMMNG
    XLTCFG_MEM_SLAB_EXTENTION
    XLTCFG_MEM_MUL_MODULE
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-unused-parameter
    -Wno-unused-function
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()

install_sdk(${CMAKE_CURRENT_SOURCE_DIR} "*")