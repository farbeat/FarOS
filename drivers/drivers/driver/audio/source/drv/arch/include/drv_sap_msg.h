/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: define the hal interface
 * Author: audio
 * Create: 2019-10-24
 */

#ifndef __DRV_SAP_MSG_H__
#define __DRV_SAP_MSG_H__

#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 drv_sap_msg_start(audio_core_id core_id);
td_s32 drv_sap_msg_stop(audio_core_id core_id);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_SAP_MSG_H__ */
