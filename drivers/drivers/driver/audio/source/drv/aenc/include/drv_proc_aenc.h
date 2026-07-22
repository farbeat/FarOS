/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: header file for drv aenc proc
 * Author: audio
 * Create: 2021-08-08
 */

#ifndef __DRV_PROC_AENC_H__
#define __DRV_PROC_AENC_H__

#include "aenc_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 drv_proc_aenc_show(td_void *file, td_handle aenc, const aenc_proc_item *proc);

#ifndef SW_UART_DEBUG
td_s32 drv_diag_log_proc_aenc_show(td_handle aenc, const aenc_proc_item *proc);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_PROC_AENC_H__ */
