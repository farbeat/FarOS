#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "dfx_port_brandy")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/dfx_channel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/diag_audio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dfx_system_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/diag_adapt_cmd_mem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/last_dump_adapt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_data_adapt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zdiag_adapt_sdt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mon_log_output.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dfx_reboot.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

set(PRIVATE_HEADER
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/diag_system_cmd/include
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/diag_system_cmd/dfx_include
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/diag_mocked_shell/include
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/last_dump/dfx_include
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/last_dump/include
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/sample_data/dfx_include
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/sample_data/include
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/diag_update/include
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/diag_nv/include
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/diag_audio_dump/include
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/diag_audio_probe/include
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/diag_audio_proc/include
    ${ROOT_DIR}/middleware/utils/dfx/diag_dfx_cmd/diag_gpu_proc/include
    ${ROOT_DIR}/include/middleware/services/bts
    ${ROOT_DIR}/include/middleware/services/bts/ble
    ${ROOT_DIR}/include/middleware/services/bts/br
    ${ROOT_DIR}/include/middleware/services/bts/common
)

if("${TARGET_COMMAND}" MATCHES "-user$")
    set(PRIVATE_DEFINES
        USER_VERSION
    )
else()
    set(PRIVATE_DEFINES
    )
endif()

set(PUBLIC_DEFINES
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-implicit-function-declaration
    -Wno-incompatible-function-pointer-types
    -Wno-incompatible-pointer-types-discards-qualifiers
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

set(LOG_DEF
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_bt.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_gpu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_gui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_dsp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_pf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_app.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_media.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_bth.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_ohos.h
)

build_component()



set(COMPONENT_NAME "dfx_log_brandy")

set(SOURCES
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
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

set(LOG_DEF
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_bt.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_dsp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_pf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_media.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_bth.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log_def_ohos.h
)

build_component()
