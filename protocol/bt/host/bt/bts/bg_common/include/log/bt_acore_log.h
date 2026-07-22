/*
 * Copyright (c) @CompanyNameMagicTag 2023. All rights reserved.
 * Description: This file improve all BTS WEAR Log related enum and interfaces.
 */
#ifndef BT_ACORE_LOG_DEF_H
#define BT_ACORE_LOG_DEF_H

#include "log_oam_logger.h"
#include "btsrv_log.h"

#define bth_uapi_diag_log(subMod, lvl, fmt, ...)                         \
    do {                                                                 \
        if (lvl == LOG_LEVEL_INFO) {                                     \
            if ((g_bth_log_type_level & BTSRV_LOG_LEVEL_INFO) != 0) {    \
                uapi_diag_info_log(0, fmt, ##__VA_ARGS__);               \
            }                                                            \
        } else if (lvl == LOG_LEVEL_WARNING) {                           \
            if ((g_bth_log_type_level & BTSRV_LOG_LEVEL_WARNING) != 0) { \
                uapi_diag_warning_log(0, fmt, ##__VA_ARGS__);            \
            }                                                            \
        } else if (lvl == LOG_LEVEL_ERROR) {                             \
            if ((g_bth_log_type_level & BTSRV_LOG_LEVEL_ERROR) != 0) {   \
                uapi_diag_error_log(0, fmt, ##__VA_ARGS__);              \
            }                                                            \
        } else if (lvl == LOG_LEVEL_DBG) {                               \
            if ((g_bth_log_type_level & BTSRV_LOG_LEVEL_DEBUG) != 0) {   \
                uapi_diag_debug_log(0, fmt, ##__VA_ARGS__);              \
            }                                                            \
        } else {                                                         \
            if ((g_bth_log_type_level & BTSRV_LOG_LEVEL_INFO) != 0) {    \
                uapi_diag_info_log(0, fmt, ##__VA_ARGS__);               \
            }                                                            \
        }                                                                \
    } while (0)

#define bt_acore_log0(subMod, lvl, fmt, args...)  bth_uapi_diag_log(subMod, lvl, fmt, ##args)
#define bt_acore_log1(subMod, lvl, fmt, args...)  bth_uapi_diag_log(subMod, lvl, fmt, ##args)
#define bt_acore_log2(subMod, lvl, fmt, args...)  bth_uapi_diag_log(subMod, lvl, fmt, ##args)
#define bt_acore_log3(subMod, lvl, fmt, args...)  bth_uapi_diag_log(subMod, lvl, fmt, ##args)
#define bt_acore_log4(subMod, lvl, fmt, args...)  bth_uapi_diag_log(subMod, lvl, fmt, ##args)

#endif