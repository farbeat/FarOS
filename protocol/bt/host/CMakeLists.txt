#===============================================================================
# @brief    cmake make file
# Copyright (c) @CompanyNameMagicTag. 2023. All rights reserved.
#===============================================================================
set(AUTO_DEF_FILE_ID FALSE)
if(${CHIP} MATCHES "bs21")
    set(BTH_ONETRACK TRUE)
elseif(${CHIP} MATCHES "ws63")
    set(BTH_ONETRACK TRUE)
else()
    set(BTH_ONETRACK FALSE)
endif()
add_subdirectory_if_exist(bg_common)
add_subdirectory_if_exist(bt)
add_subdirectory_if_exist(gle)