set(
    bt_comm_src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/dis_profile_server.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dis_profile_client.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ble_bas_client.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ble_bas_server.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ble_hid_take_photo_server.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ble_fmp_target.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ble_fmp_locator.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ble_gatts_callbacks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ble_ancs_client.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ble_ams_client.c
)
set(BTH_SAMPLE_LIST "${BTH_SAMPLE_LIST}" "${bt_comm_src_list}" CACHE INTERNAL "bt ram obj list" FORCE)
