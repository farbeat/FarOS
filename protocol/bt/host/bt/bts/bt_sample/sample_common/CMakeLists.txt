set(
    bt_comm_src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_common.c
)
set(BTH_SAMPLE_LIST "${BTH_SAMPLE_LIST}" "${bt_comm_src_list}" CACHE INTERNAL "bt ram obj list" FORCE)
