/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: header file for drv adec proc
 * Author: audio
 * Create: 2021-06-18
 */

#ifndef __DRV_PROC_ADEC_H__
#define __DRV_PROC_ADEC_H__

#include "adec_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 drv_proc_adec_show(td_void *file, td_handle adec, const adec_proc_item *proc);

#ifndef SW_UART_DEBUG
td_s32 drv_diag_log_proc_adec_show(td_handle adec, const adec_proc_item *proc);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_PROC_ADEC_H__ */
