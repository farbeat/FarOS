/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * Description: diag usr config
 * This file should be changed only infrequently and with great care.
 */

#ifndef ZDIAG_CONFIG_H
#define ZDIAG_CONFIG_H

#define DFX_OFFLINE_LOG_MUTI_FILE_NUM   5                       /* 离线日志最大文件个数 */
#define DFX_OFFLINE_LOG_FILE_SIZE       (1024 * 1024)           /* 离线日志每个文件最大字节数 */
#define DFX_OFFLINE_LOG_PATH            "/user/log/"            /* 离线日志存放路径 */
#define DFX_OFFLINE_LOG_NAME            "log_file_diag.bin"     /* 离线日志文件名 */
#define DFX_OFFLINE_LOG_CACHE_SIZE      1024                    /* 离线日志缓存大小 */
#define DFX_OFFLINE_LOG_CACHE_ENABLE    0                       /* 离线日志缓存开关 */

#endif
