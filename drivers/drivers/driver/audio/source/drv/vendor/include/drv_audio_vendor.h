/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: header file for audio vendor
 * Author: audio
 */

#ifndef __DRV_AUDIO_VENDOR_H__
#define __DRV_AUDIO_VENDOR_H__

#include "td_type.h"
#include "vendor_struct.h"

td_s32 drv_audio_vendor_register(drv_vendor_driver *driver);

td_s32 drv_audio_vendor_init(ao_port_id out_port, audio_sample_rate sample_rate, audio_bit_depth bit_depth,
                             audio_channel channel);

td_s32 drv_audio_vendor_deinit(ao_port_id out_port);

td_s32 drv_audio_vendor_set_aef_profile(ao_port_id out_port, ao_aef_profile aef_profile);

#endif
