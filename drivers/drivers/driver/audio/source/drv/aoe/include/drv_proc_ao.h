/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: header file for drv ao proc
 * Author: audio
 * Create: 2021-07-01
 */

#ifndef __DRV_PROC_AO_H__
#define __DRV_PROC_AO_H__

#include "ao_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 drv_proc_ao_show(td_void *file, td_handle ao, const ao_proc_item *proc);

#ifndef SW_UART_DEBUG
td_s32 drv_diag_log_proc_ao_show(td_handle ao, const ao_proc_item *proc);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_PROC_AO_H__ */
