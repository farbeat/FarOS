# 从deconfig中读取CONFIG_XXX变量并读取对应的值，加到CMAKE全局变量域内
# file config formate:
# key=value
function(read_config file_name)
    file(STRINGS ${file_name} ConfigContents)
    set(local_cfg "")
    foreach(NameAndValue ${ConfigContents})
        # Strip leading spaces
        string(REGEX REPLACE "^[ ]+" "" NameAndValue ${NameAndValue})
        # Skip comments
        string(REGEX MATCH "^[#]" Comment ${NameAndValue})
        if("${Comment}" STREQUAL "#")
             continue()
        endif()
        # Find variable name
        string(REGEX MATCH "^[^=]+" Name ${NameAndValue})
        # Find the value
        string(REPLACE "${Name}=" "" Value ${NameAndValue})
        # string(REPLACE "\"" "" Value ${Value})

        string(REPLACE "CFG_" "" macro ${Name})
        if("${Value}" STREQUAL "y")
            list(APPEND local_cfg ${macro})
        else()
            list(APPEND local_cfg "${macro}=${Value}")
        endif()
        # Set the variable
        set(${Name} "${Value}" PARENT_SCOPE)
    endforeach()
    set(SAP_FEATURE_CONFIGS ${local_cfg} PARENT_SCOPE)
endfunction()

# 从deconfig中加载CONFIG_XXX到GLOBAL_CONFIG，用于kbuild递归递归遍历
# file config formate:
# key=value
function(load_config file_name)
    ## 读取deconfig
    file(STRINGS ${file_name} T_CONFIG)
    ## 遍历, 去除无效选项，有效内容分行打印
    set(local_cfg "")
    foreach(item ${T_CONFIG})
        if (${item} STREQUAL "")
            continue()
        endif()
        #message("config = " ${item})
        string(FIND ${item} "CFG_" pos)
        #message("done ->" ${item} ",${pos}" )
        if (${pos} STREQUAL "0")
            string(APPEND local_cfg ${item} "\n")
            list(APPEND local_cfg ${item})
            message(STATUS ${item})
        endif()
    endforeach()
    set(GLOBAL_CONFIG ${local_cfg} PARENT_SCOPE)
endfunction()
