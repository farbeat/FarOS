/**
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved. \n
 * Description: user api header file for voice active detect (VAD) module \n
 * Author: @CompanyNameTag \n
 */

#ifndef __SOC_UAPI_VAD_H__
#define __SOC_UAPI_VAD_H__

#include "soc_uapi_audio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @defgroup soc_uapi_vad soc_uapi_vad
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  VAD event type.
 * @else
 * @brief  语音检测事件类型
 * @endif
 */
typedef enum {
    UAPI_VAD_EVENT_VAD_VALID = 0,
    UAPI_VAD_EVENT_VAD_TIMEOUT,

    UAPI_VAD_EVENT_TYPE_MAX,
} uapi_vad_event_type;

/**
 * @if Eng
 * @brief  Defines the VAD type.
 * @else
 * @brief  语音检测模块类型
 * @endif
 */
typedef enum {
    UAPI_VAD_AVAD = 0,
    UAPI_VAD_MAD,

    UAPI_VAD_TYPE_MAX
} uapi_vad_type;

/**
 * @if Eng
 * @brief  Defines the VAD work mode.
 * @else
 * @brief  语音检测模块工作模式
 * @endif
 */
typedef enum {
    UAPI_VAD_NORM_MODE = 0, /*!< @if Eng Normal mode.
                                 @else   正常模式 @endif */
    UAPI_VAD_LP_MODE,       /*!< @if Eng Low power mode.
                                 @else   低功耗模式 @endif */
    UAPI_VAD_HP_MODE,       /*!< @if Eng High performance mode.
                                 @else   高性能模式 @endif */

    UAPI_VAD_MODE_MAX
} uapi_vad_mode;

/**
 * @if Eng
 * @brief  Defines the VAD attribute.
 * @else
 * @brief  语音检测模块属性
 * @endif
 */
typedef struct {
    td_bool always_on;
    td_u32 timeout; /*!< @if Eng If always_on is TD_FALSE, configure it to trigger #UAPI_VAD_EVENT_VAD_TIMEOUT.
                         @else   若always_on设置为TD_FALSE,配置其来触发#UAPI_VAD_EVENT_VAD_TIMEOUT @endif */
    uapi_vad_type vad_type;
    uapi_vad_mode vad_mode;
} uapi_vad_attr;

/**
 * @if Eng
 * @brief  Defines VAD event callback function.
 * @else
 * @brief  定义音频输入事件回调函数
 * @endif
 */
typedef td_s32 (*uapi_vad_event_proc)(td_handle vad, uapi_vad_event_type event, const td_void *param, td_void *context);

/**
 * @if Eng
 * @brief  Initialize VAD module.
 * @attention Call uapi_vad_init at first before calling other vad api(s).
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  初始化语音检测模块
 * @attention 调用语音检测模块要求首先调用该接口
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_vad_init(td_void);

/**
 * @if Eng
 * @brief  Deinitialize VAD module.
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  去初始化语音检测模块
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_vad_deinit(td_void);

/**
 * @if Eng
 * @brief  Get the default open attributes of VAD.
 * @param  [out] attr VAD attribute
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取语音检测默认属性
 * @param  [out] attr 语音检测属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_vad_get_default_attr(uapi_vad_attr *attr);

/**
 * @if Eng
 * @brief  Open a instance for VAD.
 * @param  [in]  attr VAD instance attribute
 * @param  [out] vad VAD instance handle
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  打开语音检测，获取句柄
 * @param  [in]  attr 语音检测属性
 * @param  [out] vad 语音检测句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_vad_open(td_handle *vad, const uapi_vad_attr *attr);

/**
 * @if Eng
 * @brief  Close VAD instance.
 * @param  [in]  vad VAD instance handle
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  关闭语音检测，销毁句柄
 * @param  [in]  vad 语音检测句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_vad_close(td_handle vad);

/**
 * @if Eng
 * @brief  Register callback to process VAD events.
 * @param  [in]  vad VAD instance handle
 * @param  [in]  proc User callback to process AI events
 * @param  [in]  context User data passthrough to callback
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  注册语音检测事件处理的回调函数
 * @param  [in]  vad 语音检测句柄
 * @param  [in]  proc 用户的语音检测事件处理回调函数
 * @param  [in]  context 透传给回调函数的数据
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_vad_register_event_proc(td_handle vad, uapi_vad_event_proc proc, td_void *context);

/**
 * @if Eng
 * @brief  Reset VAD to detect voice.
 * @param  [in]  vad VAD instance handle
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  启动语音检测
 * @param  [in]  vad 语音检测句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_vad_reset(td_handle vad);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_VAD_H__ */
