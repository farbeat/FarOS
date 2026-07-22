#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
add_subdirectory_if_exist(mbedtls)
add_subdirectory_if_exist(7-zip-lzma-sdk)
add_subdirectory_if_exist(coremark)
add_subdirectory_if_exist(libboundscheck)