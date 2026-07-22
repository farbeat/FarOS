/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: header file for anc driver
 * Author: audio
 */

#ifndef __DRV_ANC_H__
#define __DRV_ANC_H__

#include "td_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sap_ioctl_anc_set_config(td_u32 cmd, td_void *arg, td_void *file);
td_s32 sap_ioctl_anc_get_config(td_u32 cmd, td_void *arg, td_void *file);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_ANC_H__ */
