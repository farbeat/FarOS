/**
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved. \n
 * Description: user header file for audio decoder \n
 * Author: @CompanyNameTag \n
 */

#ifndef __SOC_UAPI_ADEC_H__
#define __SOC_UAPI_ADEC_H__

#include "soc_uapi_acodec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @defgroup soc_uapi_adec soc_uapi_adec
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  ADEC event type.
 * @else
 * @brief  音频解码事件类型
 * @endif
 */
typedef enum {
    UAPI_ADEC_EVENT_ERROR_FRAME,
    UAPI_ADEC_EVENT_UNSUPPORT_STREAM,
    UAPI_ADEC_EVENT_TYPE_MAX,
} uapi_adec_event_type;

/**
 * @if Eng
 * @brief  Defines the attributes of the audio decoder.
 * @else
 * @brief  音频解码的属性
 * @endif
 */
typedef struct {
    uapi_acodec_id codec_id;     /*!< @if Eng Audio Codec id.
                                      @else   Audio Codec 序号 @endif */
    td_u32 buf_dur_ms;           /*!< @if Eng Buffer duration, unit: ms.
                                      @else   缓存Buffer的长度，以毫秒为单位 @endif */
    uapi_acodec_dec_param param; /*!< @if Eng Audio decoder parameter.
                                      @else   音频解码配置参数 @endif */
} uapi_adec_attr;

/**
 * @if Eng
 * @brief  ADEC event callback function.
 * @else
 * @brief  音频解码的事件回调函数
 * @endif
 */
typedef td_s32 (*uapi_adec_event_proc)(td_handle adec, uapi_adec_event_type event, td_void *param,
    td_void *context);

/**
 * @if Eng
 * @brief  Initialize Audio Decoder module.
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  ADEC初始化
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_adec_init(td_void);

/**
 * @if Eng
 * @brief  Deinitialize Audio Decoder module.
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  ADEC去初始化
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_adec_deinit(td_void);

/**
 * @if Eng
 * @brief  Create an Audio Decoder instance.
 * @param  [out] adec ADEC handle pointer
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  创建Audio Decoder实例
 * @param  [out] adec ADEC句柄指针
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_adec_create(td_handle *adec);

/**
 * @if Eng
 * @brief  Destroy an Audio Decoder instance.
 * @param  [in]  adec ADEC handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  销毁Audio Decoder实例
 * @param  [in]  adec ADEC句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_adec_destroy(td_handle adec);

/**
 * @if Eng
 * @brief  Register callback to process ADEC events.
 * @param  [in]  adec ADEC handle
 * @param  [in]  event_proc User callback to process ADEC events
 * @param  [in]  context Data passthrough to callback
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  注册ADEC事件处理回调函数
 * @param  [in]  adec ADEC句柄
 * @param  [in]  event_proc 用户的ADEC事件处理回调函数
 * @param  [in]  context 透传给回调函数的数据
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_adec_register_event_proc(td_handle adec, uapi_adec_event_proc event_proc, td_void *context);

/**
 * @if Eng
 * @brief  Start an ADEC instance.
 * @param  [in]  adec ADEC handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  启动ADEC实例开始解码
 * @param  [in]  adec ADEC句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_adec_start(td_handle adec);

/**
 * @if Eng
 * @brief  Stop an ADEC instance.
 * @param  [in]  adec ADEC handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  停止ADEC实例
 * @param  [in]  adec ADEC句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_adec_stop(td_handle adec);

/**
 * @if Eng
 * @brief  Get the attribute of ADEC instance.
 * @param  [in]  adec ADEC handle
 * @param  [out] attr ADEC instance attribute
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  获取ADEC实例属性
 * @param  [in]  adec ADEC句柄
 * @param  [out] attr ADEC实例属性
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_adec_get_attr(td_handle adec, uapi_adec_attr *attr);

/**
 * @if Eng
 * @brief  Set the attribute of ADEC instance.
 * @param  [in]  adec ADEC handle
 * @param  [in]  attr ADEC instance attribute
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  设置ADEC实例属性
 * @param  [in]  adec ADEC句柄
 * @param  [in]  attr ADEC实例属性
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_adec_set_attr(td_handle adec, const uapi_adec_attr *attr);

/**
 * @if Eng
 * @brief  Attach a output instance to ADEC instance.
 * @param  [in]  adec ADEC handle
 * @param  [in]  output Output instance handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  绑定输出实例到ADEC
 * @param  [in]  adec ADEC句柄
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_adec_attach_output(td_handle adec, td_handle output);

/**
 * @if Eng
 * @brief  Detach a output instance from ADEC instance.
 * @param  [in]  adec ADEC handle
 * @param  [in]  output Output instance handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  解绑输出实例
 * @param  [in]  adec ADEC句柄
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_adec_detach_output(td_handle adec, td_handle output);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_ADEC_H__ */
