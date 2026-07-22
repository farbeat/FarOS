#===============================================================================
# @brief    cmake make file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "cipher")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/aes_ccm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/aes_gcm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/symc_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/symc_crypto.c
    ${CMAKE_CURRENT_SOURCE_DIR}/symc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cipher_api.c
)

set(PRIVATE_HEADER
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
)

set(COMPONENT_CCFLAGS
    -Wno-implicit-function-declaration
)

set(WHOLE_LINK
true
)
build_component()
