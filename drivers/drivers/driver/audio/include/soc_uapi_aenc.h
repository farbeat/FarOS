/**
 * Copyright (c) @CompanyNameMagicTag 2020-2022. All rights reserved. \n
 * Description: user header file for audio encoder \n
 * Author: @CompanyNameTag \n
 */

#ifndef __SOC_UAPI_AENC_H__
#define __SOC_UAPI_AENC_H__

#include "soc_uapi_acodec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @defgroup soc_uapi_aenc soc_uapi_aenc
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  AENC event type.
 * @else
 * @brief  音频编码事件类型
 * @endif
 */
typedef enum {
    UAPI_AENC_EVENT_NULL = 0,
    UAPI_AENC_EVENT_TYPE_MAX,
} uapi_aenc_event_type;

/**
 * @if Eng
 * @brief  AENC param type.
 * @else
 * @brief  音频编码参数类型
 * @endif
 */
typedef enum {
    UAPI_AENC_PARAM_BIT_RATE,
    UAPI_AENC_PARAM_MAX,
} uapi_aenc_param_type;

/**
 * @if Eng
 * @brief  Defines the attributes of audio encoder.
 * @else
 * @brief  音频编码器属性
 * @endif
 */
typedef struct {
    uapi_acodec_id codec_id;     /*!< @if Eng Codec index.
                                      @else   编码器ID @endif */
    td_bool frame_mode_output;   /*!< @if Eng Frame mode output. \n
                                  * Closed by default, encoder outputs data by frame when opened.
                                      @else   帧输出模式。\n
                                  * 默认关闭，开启时编码器将按帧输出数据。@endif */
    td_u32 max_trans_unit_size;  /*!< @if Eng Maximum transport unit size(mtu size), unit: Bytes. \n
                                  * Default 0: mtu size will be the frame size of encoder.
                                      @else   最大传输单元大小（mtu size），单位：字节。\n
                                  * 默认为 0：mtu 大小即为编码器的帧大小。  @endif */
    uapi_acodec_enc_param param; /*!< @if Eng Audio encoder parameter.
                                      @else   编码器参数 @endif */
} uapi_aenc_attr;

/**
 * @if Eng
 * @brief  Defines AENC event callback function.
 * @else
 * @brief  定义音频编码事件回调函数
 * @endif
 */
typedef td_s32 (*uapi_aenc_event_proc)(td_handle aenc, uapi_aenc_event_type event, td_void *param, td_void *context);

/**
 * @if Eng
 * @brief  Initialize Audio Encode module.
 * @attention Call uapi_aenc_init at first before calling other aenc api(s)
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  初始化音频编码模块
 * @attention 调用AENC模块要求首先调用该接口
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_aenc_init(td_void);

/**
 * @if Eng
 * @brief  Deinitialize Audio Encode module.
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  去初始化音频编码模块
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_aenc_deinit(td_void);

/**
 * @if Eng
 * @brief  Create a instance for AENC.
 * @param  [out] aenc AENC handle pointer
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  打开AENC通道，创建句柄
 * @param  [out] aenc AENC句柄指针
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_aenc_create(td_handle *aenc);

/**
 * @if Eng
 * @brief  Destory AENC instance.
 * @param  [in]  aenc AENC handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  关闭AENC通道，销毁句柄
 * @param  [in]  aenc AENC句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_aenc_destroy(td_handle aenc);

/**
 * @if Eng
 * @brief  Register callback to process AENC events.
 * @param  [in]  aenc AENC handle
 * @param  [in]  event_proc User callback to process AENC events
 * @param  [in]  context Data passthrough to callback
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  注册AENC事件处理的回调函数
 * @param  [in]  aenc AENC句柄
 * @param  [in]  event_proc 用户的AENC事件处理回调函数
 * @param  [in]  context 透传给回调函数的数据
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_aenc_register_event_proc(td_handle aenc, uapi_aenc_event_proc event_proc, td_void *context);

/**
 * @if Eng
 * @brief  Start AENC instance to encode PCM data.
 * @param  [in]  aenc AENC handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  启动AENC通道，对PCM数据进行编码
 * @param  [in]  aenc AENC句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_aenc_start(td_handle aenc);

/**
 * @if Eng
 * @brief  Stop AENC instance to encode PCM data.
 * @param  [in]  aenc AENC handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  停止AENC通道，停止PCM数据编码
 * @param  [in]  aenc AENC句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_aenc_stop(td_handle aenc);

/**
 * @if Eng
 * @brief  Set attributes of AENC instance.
 * @param  [in]  aenc AENC handle
 * @param  [in]  attr AENC instance attribute
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  设置AENC通道属性
 * @param  [in]  aenc AENC句柄
 * @param  [in]  attr AENC实例属性
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_aenc_set_attr(td_handle aenc, const uapi_aenc_attr *attr);

/**
 * @if Eng
 * @brief  Get attributes of AENC instance.
 * @param  [in]  aenc AENC handle
 * @param  [out] attr AENC instance attribute
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  获取AENC通道属性
 * @param  [in]  aenc AENC句柄
 * @param  [out] attr AENC实例属性
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_aenc_get_attr(td_handle aenc, uapi_aenc_attr *attr);

/**
 * @if Eng
 * @brief  Attach a output instance to AENC instance.
 * @param  [in]  aenc AENC handle
 * @param  [in]  output Output instance handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  绑定输出实例给AENC实例
 * @param  [in]  aenc AENC句柄
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_aenc_attach_output(td_handle aenc, td_handle output);

/**
 * @if Eng
 * @brief  Detach a output instance from AENC instance.
 * @param  [in]  aenc AENC handle
 * @param  [in]  output Output instance handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  解绑AENC实例的输出实例
 * @param  [in]  aenc AENC句柄
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_aenc_detach_output(td_handle aenc, td_handle output);

/**
 * @if Eng
 * @brief  Set AENC parameters.
 * @param  [in]  aenc       AENC handle
 * @param  [in]  type       AENC parameter type
 * @param  [in]  param      AENC parameter
 * @param  [in]  param_size parameter size
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  设置AENC参数
 * @param  [in]  aenc       AENC句柄
 * @param  [in]  type       AENC参数类型
 * @param  [in]  param      AENC参数
 * @param  [in]  param_size 参数大小
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_aenc_set_param(td_handle aenc, uapi_aenc_param_type type, const td_void *param, td_u32 param_size);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_AENC_H__ */
