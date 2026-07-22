set(COMPONENT_NAME "mtd_com")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/common/src/common.c
	${CMAKE_CURRENT_SOURCE_DIR}/common/src/mtd_list.c
	${CMAKE_CURRENT_SOURCE_DIR}/common/src/mtdblock.c
#	${CMAKE_CURRENT_SOURCE_DIR}/common/src/mtdchar.c
)

set(PUBLIC_HEADER
	${CMAKE_CURRENT_SOURCE_DIR}/common/include
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
    LOSCFG_FS_YAFFS
)

set(PUBLIC_DEFINES
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
