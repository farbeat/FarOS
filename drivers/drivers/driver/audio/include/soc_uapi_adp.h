/**
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved. \n
 * Description: user api header file for audio data port (ADP) module \n
 * Author: @CompanyNameTag \n
 */

#ifndef __SOC_UAPI_ADP_H__
#define __SOC_UAPI_ADP_H__

#include "soc_uapi_audio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @defgroup soc_uapi_adp soc_uapi_adp
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  ADP event type.
 * @else
 * @brief  ADP事件类型
 * @endif
 */
typedef enum {
    UAPI_ADP_EVENT_EOS,
    UAPI_ADP_EVENT_EMPTY,
    UAPI_ADP_EVENT_TYPE_MAX,
} uapi_adp_event_type;

/**
 * @if Eng
 * @brief  ADP attribute.
 * @else
 * @brief  ADP属性
 * @endif
 */
typedef struct {
    td_u32 buf_size;
} uapi_adp_attr;

/**
 * @if Eng
 * @brief  ADP event callback function.
 * @else
 * @brief  ADP事件回调函数
 * @endif
 */
typedef td_s32 (*uapi_adp_event_proc)(td_handle adp, uapi_adp_event_type event, td_void *param, td_void *context);

/**
 * @if Eng
 * @brief  Initializes ADP module.
 * @attention Call uapi_adp_init at first before calling other ADP api(s)
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  初始化ADP模块
 * @attention 调用其他接口前，先调用uapi_adp_init
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_init(td_void);

/**
 * @if Eng
 * @brief  Deinitializes ADP module.
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  去初始化ADP模块
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_deinit(td_void);

/**
 * @if Eng
 * @brief  Get ADP default attribute.
 * @param  [out] attr ADP default attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  获取ADP默认属性
 * @param  [out] attr ADP默认属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_get_def_attr(uapi_adp_attr *attr);

/**
 * @if Eng
 * @brief  Create ADP instance.
 * @param  [in]  attr ADP instance attribute
 * @param  [out] adp ADP handle pointer
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  创建ADP实例
 * @param  [in]  attr ADP实例属性
 * @param  [out] adp ADP句柄指针
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_create(td_handle *adp, const uapi_adp_attr *attr);

/**
 * @if Eng
 * @brief  Destroy ADP instance.
 * @param  [in]  adp ADP handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  销毁ADP实例
 * @param  [in]  adp ADP句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_destroy(td_handle adp);

/**
 * @if Eng
 * @brief  Map an existing ADP instance.
 * @param  [in]  adp ADP handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  映射一个已存在的ADP实例
 * @param  [in]  adp ADP句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_map(td_handle adp);

/**
 * @if Eng
 * @brief  Unmap an existing ADP instance.
 * @param  [in]  adp ADP handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  取消映射一个已存在的ADP实例
 * @param  [in]  adp ADP句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_unmap(td_handle adp);

/**
 * @if Eng
 * @brief  Get ADP instance attribute.
 * @param  [in]  adp ADP handle
 * @param  [out] attr ADP instance attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  获取ADP实例属性
 * @param  [in]  adp ADP句柄
 * @param  [out] attr ADP实例属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_get_attr(td_handle adp, uapi_adp_attr *attr);

/**
 * @if Eng
 * @brief  Set ADP instance attribute.
 * @param  [in]  adp ADP handle
 * @param  [in]  attr ADP instance attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  设置ADP实例属性
 * @param  [in]  adp ADP句柄
 * @param  [in]  attr ADP实例属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_set_attr(td_handle adp, const uapi_adp_attr *attr);

/**
 * @if Eng
 * @brief  Attach a output instance to ADP instance.
 * @param  [in]  adp ADP handle
 * @param  [in]  output Output instance handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  绑定一个输出实例给ADP实例
 * @param  [in]  adp ADP句柄
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_attach_output(td_handle adp, td_handle output);

/**
 * @if Eng
 * @brief  Detach a output instance from ADP instance.
 * @param  [in]  adp ADP handle
 * @param  [in]  output Output instance handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  从ADP实例解绑定一个输出实例
 * @param  [in]  adp ADP句柄
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_detach_output(td_handle adp, td_handle output);

/**
 * @if Eng
 * @brief  Register callback to process ADP events.
 * @param  [in]  adp ADP handle
 * @param  [in]  proc User callback to process ADP events
 * @param  [in]  context Data passthrough to callback
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  注册处理ADP事件的回调函数
 * @param  [in]  adp ADP句柄
 * @param  [in]  proc 用户的ADP事件处理回调函数
 * @param  [in]  context 透传给回调函数的数据
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_register_event_proc(td_handle adp, uapi_adp_event_proc proc, td_void *context);

/**
 * @if Eng
 * @brief  Query free size of ADP instance.
 * @param  [in]  adp ADP handle
 * @param  [out] free_size ADP free size
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  查询ADP实例的空闲空间
 * @param  [in]  adp ADP句柄
 * @param  [out] free_size ADP空闲空间
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_query_free(td_handle adp, td_u32 *free_size);

/**
 * @if Eng
 * @brief  Query data size of ADP instance.
 * @param  [in]  adp ADP handle
 * @param  [out] busy_size ADP data size
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  查询ADP实例的数据大小
 * @param  [in]  adp ADP句柄
 * @param  [out] busy_size ADP数据大小
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_query_busy(td_handle adp, td_u32 *busy_size);

/**
 * @if Eng
 * @brief  Get a buffer from ADP instance for writing data.
 * @param  [in]  adp ADP handle
 * @param  [out] stream The ADP buffer
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  获取ADP实例的缓存空间，用于送数据
 * @param  [in]  adp ADP句柄
 * @param  [out] stream ADP缓存空间
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_get_buffer(td_handle adp, uapi_stream_buf *stream);

/**
 * @if Eng
 * @brief  Put the buffer to ADP instance after writing data.
 * @param  [in]  adp ADP handle
 * @param  [in]  stream The ADP buffer
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  送完数据后，更新ADP缓存空间
 * @param  [in]  adp ADP句柄
 * @param  [in]  stream ADP缓存空间
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_put_buffer(td_handle adp, const uapi_stream_buf *stream);

/**
 * @if Eng
 * @brief  Send stream to ADP instance.
 * @param  [in]  adp ADP handle
 * @param  [in]  stream The stream pointer
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  发送码流数据给ADP实例
 * @param  [in]  adp ADP句柄
 * @param  [in]  stream 码流结构体指针
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_send_stream(td_handle adp, const uapi_stream_buf *stream);

/**
 * @if Eng
 * @brief  Acquire stream from ADP instance.
 * @param  [in]  adp ADP handle
 * @param  [out] stream The stream pointer
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  从ADP实例获取码流数据
 * @param  [in]  adp ADP句柄
 * @param  [out] stream 码流结构体指针
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_acquire_stream(td_handle adp, uapi_stream_buf *stream);

/**
 * @if Eng
 * @brief  Release stream buffer to ADP instance.
 * @param  [in]  adp ADP handle
 * @param  [in]  stream The stream pointer
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  释放ADP实例的码流空间
 * @param  [in]  adp ADP句柄
 * @param  [in]  stream 码流结构体指针
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_release_stream(td_handle adp, const uapi_stream_buf *stream);

/**
 * @if Eng
 * @brief  Send frame to ADP instance.
 * @param  [in]  adp ADP handle
 * @param  [in]  frame The audio frame
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  发送音频帧给ADP实例
 * @param  [in]  adp ADP句柄
 * @param  [in]  frame 音频帧
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_send_frame(td_handle adp, const uapi_audio_frame *frame);

/**
 * @if Eng
 * @brief  Acquire a frame from ADP instance.
 * @param  [in]  adp ADP handle
 * @param  [out] frame The audio frame
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  从ADP实例获取一帧音频帧
 * @param  [in]  adp ADP句柄
 * @param  [out] frame 音频帧
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_acquire_frame(td_handle adp, uapi_audio_frame *frame);

/**
 * @if Eng
 * @brief  Release a frame to ADP instance.
 * @param  [in]  adp ADP handle
 * @param  [in]  frame The audio frame
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  从ADP实例释放一帧音频帧
 * @param  [in]  adp ADP句柄
 * @param  [in]  frame 音频帧
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_adp_release_frame(td_handle adp, const uapi_audio_frame *frame);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_ADP_H__ */
