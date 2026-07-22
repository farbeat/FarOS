add_subdirectory_if_exist(gap)
if(${CHIP} MATCHES "brandy")
    set(
        bt_comm_src_list
        ${CMAKE_CURRENT_SOURCE_DIR}/stack_mpc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stack_gap_if.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stack_av_if.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stack_avrcp_if.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stack_dft_if.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stack_hfp_if.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stack_pbap_if.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stack_spp_if.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stack_pan_if.c
    )
    set(BTH_SDK_LIST "${BTH_SDK_LIST}" "${bt_comm_src_list}" CACHE INTERNAL "bt ram obj list" FORCE)
endif()
