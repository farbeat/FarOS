if("BRANDY_MCU_BT" IN_LIST DEFINES)
set(
    bt_comm_src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_spp_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_pan_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_pbap_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bt_mcu_bt_profile_stubs.c
)
else()
set(
    bt_comm_src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_a2dp_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_a2dp_snk_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_avrcp_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_hfp_hf_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_hfp_ag_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_pbap_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_spp_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_pan_service.c
)
endif()
set(BTH_SAMPLE_LIST "${BTH_SAMPLE_LIST}" "${bt_comm_src_list}" CACHE INTERNAL "bt ram obj list" FORCE)
