set(COMPONENT_NAME "x_dpal")
return_if_not_build()
set(FREERTOSTOPDIR ${ROOT_DIR}/kernel/freertos)
set(FREEBSD_PATH ${ROOT_DIR}/open_source/freebsd)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/device/src/dpal_devicebase.c
    ${CMAKE_CURRENT_SOURCE_DIR}/device/src/dpal_driverbase.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dpal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dpal_driver.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/device/include
    ${ROOT_DIR}/middleware/utils/fs/include
    ${ROOT_DIR}/kernel/osal/include
)

install_sdk(${ROOT_DIR}/middleware/utils/fs/include/fs "*.h")
install_sdk(${ROOT_DIR}/middleware/utils/fs/include/inode "*.h")
install_sdk(${ROOT_DIR}/middleware/utils/fs/include/net "*.h")

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SOC_OSAL
    DPALCFG_FS_VFS
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-typedef-redefinition
    -Wno-unused-parameter
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
