/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: header file for audio vendor
 * Author: audio
 */

#ifndef __VENDOR_STRUCT_H__
#define __VENDOR_STRUCT_H__

#include "td_type.h"
#include "osal_list.h"
#include "ao_struct.h"
#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    audio_sample_rate sample_rate;
    audio_bit_depth bit_depth;
    td_bool master_mode;
} vendor_codec_attr;

typedef struct {
    struct osal_list_head node;

    ao_port_id out_port;
    td_s32 (*init)(const vendor_codec_attr *codec_attr);
    td_s32 (*deinit)(td_void);
    td_s32 (*set_aef_profile)(ao_aef_profile aef_profile);
} drv_vendor_driver;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __VENDOR_STRUCT_H__ */
