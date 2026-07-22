/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 * Description: log output monitor.
 */

#ifndef MON_LOG_OUTPUT_H
#define MON_LOG_OUTPUT_H

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MON_LOG_CNT_CLEAR_TIMEOUT       (1000 * 60 * 15)  /* 15 min */
#define MAX_LOGCNT_BT             9000
#define MAX_LOGCNT_DSP            1000
#define MAX_LOGCNT_PF             2000
#define MAX_LOGCNT_MEDIA          1000
#define MAX_LOGCNT_APP            5000
#define MAX_LOGCNT_GPU            2000
#define MAX_LOGCNT_GUI            1000
#define MAX_LOGCNT_BTH            3000
#define MAX_LOGCNT_OHOS           1000
#define MAX_LOGCNT_BT_STATUS      3000
#define MAX_LOGCNT_BT_OTA         3000

void mon_log_output_ctrl_init(void);
uint32_t *mon_log_output_get_counter(void);
void mon_log_output_enable(bool enable);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
