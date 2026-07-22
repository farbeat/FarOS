/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: vendor user api
 * Author: Audio
 */

#include "soc_uapi_vendor.h"
#include "drv_audio_vendor.h"
#include "audio_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 uapi_audio_vendor_register(uapi_vendor_driver *driver)
{
    return drv_audio_vendor_register((drv_vendor_driver *)(td_void *)driver);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
