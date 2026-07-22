/*
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: header file for drv sea proc
 * Author: audio
 * Create: 2022-01-08
 */

#ifndef __DRV_PROC_SEA_H__
#define __DRV_PROC_SEA_H__

#include "sea_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 drv_proc_sea_show(td_void *file, td_handle sea, const sea_proc_item *proc);

#ifndef SW_UART_DEBUG
td_s32 drv_diag_log_proc_sea_show(td_handle sea, const sea_proc_item *proc);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_PROC_SEA_H__ */
