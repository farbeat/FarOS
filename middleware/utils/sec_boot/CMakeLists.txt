#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "sec_boot")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sec_boot_fota.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sec_boot_image_sign_validate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sec_boot_otp_write.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sec_boot_public.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sec_boot_subkey_cert_validate.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

set(PRIVATE_HEADER
    ${ROOT_DIR}/drivers/drivers/driver/security/hash
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

set(COMPONENT_NAME "sec_boot_rom")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sec_boot_fsb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sec_boot_image_sign_validate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sec_boot_otp_write.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sec_boot_public.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sec_boot_subkey_cert_validate.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

set(WHOLE_LINK
    true
)

build_component()