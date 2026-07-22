add_subdirectory_if_exist(base)

set(
    bt_comm_src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/dft_service.c
)
if ("SUPPORT_BLE" IN_LIST DEFINES)
set(
    bt_comm_src_list
    ${bt_comm_src_list}
    ${CMAKE_CURRENT_SOURCE_DIR}/btsrv_gatt_client.c
    ${CMAKE_CURRENT_SOURCE_DIR}/btsrv_gatt_server.c
    ${CMAKE_CURRENT_SOURCE_DIR}/btsrv_gatt_server_default_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/btsv_l2cap_chan.c
    ${CMAKE_CURRENT_SOURCE_DIR}/btsv_l2cap_chan_ota.c
)
endif()

if ("SUPPORT_BREDR" IN_LIST DEFINES)
set(
    bt_comm_src_list
    ${bt_comm_src_list}
    ${CMAKE_CURRENT_SOURCE_DIR}/a2dp_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/a2dp_snk_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/avrcp_ct_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/avrcp_service_mgr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/avrcp_tg_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hfp_hf_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hfp_ag_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pbap_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/spp_service.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pan_service.c
)
endif()
set(BTH_SERVICE_LIST "${BTH_SERVICE_LIST}" "${bt_comm_src_list}" CACHE INTERNAL "bt ram obj list" FORCE)
