#===============================================================================
# @brief    cmake toolchains
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
macro(return_if_not_build)
    if(NOT ${COMPONENT_NAME} IN_LIST TARGET_COMPONENT)
        return()
    endif()
endmacro()

macro(add_subdirectory_if_exist _path)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_path}/CMakeLists.txt)
        add_subdirectory(${_path})
    endif()
endmacro()

macro(add_subdirectory_with_alias_if_exist _path _path_alias)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_path}/CMakeLists.txt)
        add_subdirectory(${_path} ${_path_alias})
    endif()
endmacro()

function(add_path_if_exist _path _name)
    if(EXISTS ${_path}/CMakeLists.txt)
        add_subdirectory(${_path} ${_name})
    endif()
endfunction()

function(KCONFIG_GET_PARAMS KCONFIG_PATH)
    file(STRINGS ${KCONFIG_PATH} LOS_CONFIG_LIST ENCODING "UTF-8")

    foreach (CONFIG ${LOS_CONFIG_LIST})
        # CONFIG maybe: CONFIG_CC_STACKPROTECTOR_STRONG=y

        # The first: variable name
        string(REGEX MATCH "[^=]+" CONFIG_VAR_NAME ${CONFIG})

        # The second: variable value
        string(REGEX MATCH "=(.+$)" CONFIG_VAR_VAL ${CONFIG})
        set(CONFIG_VAR_VAL ${CMAKE_MATCH_1})
        if(CONFIG_VAR_VAL MATCHES "^y$")
            set(${CONFIG_VAR_NAME} ${CONFIG_VAR_VAL} PARENT_SCOPE)
        endif()
    endforeach()
endfunction()