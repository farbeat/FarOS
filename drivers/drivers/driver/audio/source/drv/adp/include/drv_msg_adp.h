/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for drv adp msg
 * Author: audio
 * Create: 2020-12-23
 * Notes: NA
 * History: 2020-12-23 1st version
 */

#ifndef __DRV_MSG_ADP_H__
#define __DRV_MSG_ADP_H__

#include "audio_msg.h"
#include "adp_struct.h"
#include "td_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 drv_msg_adp_init(td_u32 core_id, td_u32 init_reg);
td_s32 drv_msg_adp_deinit(td_void);

td_s32 drv_msg_adp_get_def_attr(audio_adp_type type, audio_adp_attr *attr);
td_s32 drv_msg_adp_open(td_handle *adp, audio_adp_type type, const audio_adp_attr *attr);
td_s32 drv_msg_adp_close(td_handle adp);

td_s32 drv_msg_adp_alloc_shrb(td_handle adp, td_handle mod, audio_adp_type adp_type, td_u32 data_port_id);
td_s32 drv_msg_adp_free_shrb(td_handle adp);

td_s32 drv_msg_adp_attach_shrb(td_handle adp);
td_s32 drv_msg_adp_detach_shrb(td_handle adp);

td_s32 drv_msg_adp_get_attr(td_handle adp, audio_adp_attr *attr);
td_s32 drv_msg_adp_set_attr(td_handle adp, audio_adp_attr *attr);
td_s32 drv_msg_adp_sync_attr(td_handle adp, td_handle mod, audio_adp_type adp_type,
    td_u32 data_port_id, audio_adp_attr *attr);

td_s32 drv_msg_adp_attach_output(td_handle adp, td_handle output);
td_s32 drv_msg_adp_detach_output(td_handle adp, td_handle output);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_MSG_ADP_H__ */
