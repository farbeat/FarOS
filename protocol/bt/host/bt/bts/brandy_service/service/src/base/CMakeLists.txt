set(
    bt_comm_src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/btsrv_task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/btsrv_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/connection_manager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/local_device_manager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/remote_device_manager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bts_uuid.c
)
set(BTH_SERVICE_LIST "${BTH_SERVICE_LIST}" "${bt_comm_src_list}" CACHE INTERNAL "bt ram obj list" FORCE)