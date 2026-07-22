/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for drv adp msg
 * Author: audio
 */

#ifndef __DRV_MSG_ANC_H__
#define __DRV_MSG_ANC_H__

#include "anc_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 drv_msg_anc_set_config(aha_mode mode, const td_s8 *config, td_u32 config_len);
td_s32 drv_msg_anc_get_config(aha_mode *mode, td_s8 *config, td_u32 *config_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_MSG_ANC_H__ */
