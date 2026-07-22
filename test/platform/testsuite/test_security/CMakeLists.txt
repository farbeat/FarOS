#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "test_security")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_trng.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_pke.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_efuse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_cipher.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_cipher_api.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_print.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
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
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()

set(COMPONENT_NAME "test_efuse")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_efuse.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
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
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
