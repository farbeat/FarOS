set(
    bt_comm_src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_ble_dis_profile.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_ble_bas.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_ble_hid.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_ble_fmp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_ble_ancs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_ble_ams.c
)
set(BTH_SAMPLE_LIST "${BTH_SAMPLE_LIST}" "${bt_comm_src_list}" CACHE INTERNAL "bt ram obj list" FORCE)
