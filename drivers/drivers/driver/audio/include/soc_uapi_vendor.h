/**
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved. \n
 * Description: user api header file for audio vendor module \n
 * Author: @CompanyNameTag \n
 */

#ifndef __SOC_UAPI_VENDOR_H__
#define __SOC_UAPI_VENDOR_H__

#include "td_type.h"
#include "osal_list.h"
#include "soc_uapi_sound.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @defgroup soc_uapi_vendor soc_uapi_vendor
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  Defines the attributes of the vendor driver.
 * @else
 * @brief  vendor驱动的属性
 * @endif
 */
typedef struct {
    uapi_audio_sample_rate sample_rate;
    uapi_audio_bit_depth bit_depth;
    td_bool master_mode;
} uapi_vendor_codec_attr;

/**
 * @if Eng
 * @brief  vendor driver.
 * @else
 * @brief  驱动模块
 * @endif
 */
typedef struct {
    struct osal_list_head node;

    uapi_snd_out_port out_port;
    td_s32 (*init)(const uapi_vendor_codec_attr *codec_attr);
    td_s32 (*deinit)(td_void);
    td_s32 (*set_aef_profile)(uapi_snd_aef_profile aef_profile);
} uapi_vendor_driver;

/**
 * @if Eng
 * @brief  Register callback function to vendor driver.
 * @param  [in]  driver Vendor driver
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  注册回调函数给vendor驱动
 * @param  [in]  driver vendor驱动
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_audio_vendor_register(uapi_vendor_driver *driver);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_VENDOR_H__ */
