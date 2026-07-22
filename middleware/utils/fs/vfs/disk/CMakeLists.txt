set(COMPONENT_NAME "x_disk")
set(FREERTOSTOPDIR ${ROOT_DIR}/open_source/freebsd)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/disk.c
    ${CMAKE_CURRENT_SOURCE_DIR}/disk_shellcmd.c
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    CONFIG_DRIVER_DISK
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-typedef-redefinition
    -Wno-incompatible-pointer-types
    -Wno-unused-parameter
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
