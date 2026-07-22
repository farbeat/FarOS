/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for drv dpm msg
 * Author: audio
 * Create: 2020-07-30
 * Notes: NA
 * History: 2020-07-30 1st version
 */

#ifndef __DRV_MSG_DPM_H__
#define __DRV_MSG_DPM_H__

#include "ao_struct.h"
#include "audio_msg.h"
#include "dpm_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


td_s32 drv_msg_dpm_init(td_void);

td_s32 drv_msg_dpm_deinit(td_void);

td_s32 drv_msg_dpm_open(td_handle *dpm);

td_s32 drv_msg_dpm_close(td_handle dpm);

td_s32 drv_msg_dpm_start(td_handle dpm);

td_s32 drv_msg_dpm_stop(td_handle dpm);

td_s32 drv_msg_dpm_get_attr(td_handle dpm, dpm_attr *attr);

td_s32 drv_msg_dpm_set_attr(td_handle dpm, dpm_attr *attr);

td_s32 drv_msg_dpm_set_bypass(td_handle dpm, td_bool enable);

td_s32 drv_msg_dpm_attach_output(td_handle dpm, td_handle output);

td_s32 drv_msg_dpm_detach_output(td_handle dpm, td_handle output);

td_s32 drv_msg_dpm_get_proc_info(td_handle dpm, dpm_proc_item *proc);

td_s32 drv_msg_dpm_get_lib_type(dpm_id dpm_id, audio_lib_type *lib_type);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_MSG_DPM_H__ */
