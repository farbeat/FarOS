#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
#===============================================================================
if(${NV_UPDATE})
add_custom_target(GENERAT_NVBIN ALL
    COMMAND ${Python3_EXECUTABLE} ${ROOT_DIR}/build/config/target_config/${CHIP}/build_nvbin.py
    COMMENT "update nv bin"
    WORKING_DIRECTORY ${ROOT_DIR}
    DEPENDS GENERAT_BIN
)
endif()
