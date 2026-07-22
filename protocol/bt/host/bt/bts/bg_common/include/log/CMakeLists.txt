set(
    bth_log_def_list
    ${CMAKE_CURRENT_SOURCE_DIR}/log_def_bth.h
    ${CMAKE_CURRENT_SOURCE_DIR}/log_def_bt_sdk.h
)
set(BTH_LOG_DEF_LIST "${BTH_LOG_DEF_LIST}" "${bth_log_def_list}" CACHE INTERNAL "log_def header list" FORCE)