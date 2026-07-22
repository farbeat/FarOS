set(
    bt_comm_src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/advertising_scan.c
)
set(BTH_SDK_LIST "${BTH_SDK_LIST}" "${bt_comm_src_list}" CACHE INTERNAL "bt ram obj list" FORCE)

