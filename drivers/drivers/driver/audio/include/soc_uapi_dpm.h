/**
 * Copyright (c) @CompanyNameMagicTag 2020-2022. All rights reserved. \n
 * Description: user header file for data precess module \n
 * Author: @CompanyNameTag \n
 */

#ifndef __SOC_UAPI_DPM_H__
#define __SOC_UAPI_DPM_H__

#include "soc_uapi_audio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @defgroup soc_uapi_dpm soc_uapi_dpm
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  Data process module ID definition.
 * @else
 * @brief  数据处理模块ID定义
 * @endif
 */
typedef enum {
    UAPI_DPM_ID_FIR,
    UAPI_DPM_ID_IIR,
    UAPI_DPM_ID_FFT,
    UAPI_DPM_ID_IFFT,

    UAPI_DPM_ID_MAX,
} uapi_dpm_id;

/**
 * @if Eng
 * @brief  dpm event type.
 * @else
 * @brief  数据处理事件类型
 * @endif
 */
typedef enum {
    UAPI_DPM_EVENT_NULL = 0,
    UAPI_DPM_EVENT_TYPE_MAX,
} uapi_dpm_event_type;

/**
 * @if Eng
 * @brief  Parameters for creating an data process module
 * @else
 * @brief  数据处理模块创建配置参数
 * @endif
 */
typedef struct {
    td_void *private_data;    /*!< @if Eng Pointer of data process private open parameters \n
                                           note: set NULL if there is no private parameters
                                   @else   数据处理模块私有参数 \n
                                           注意: 若没有私有参数，设置为NULL @endif */
    td_u32 private_data_size; /*!< @if Eng Size of data process private open parameters \n
                                           note: set to 0 if there is no private parameters
                                   @else   数据处理模块私有参数大小 \n
                                           注意: 若没有私有参数，设置为0 @endif */
} uapi_dpm_param;

/**
 * @if Eng
 * @brief  Defines the attributes of data process module.
 * @else
 * @brief  数据处理模块属性
 * @endif
 */
typedef struct {
    uapi_dpm_id dpm_id; /*!< @if Eng dpm index.
                                  @else   数据处理模块ID @endif */
    uapi_dpm_param param; /*!< @if Eng data process module parameter.
                                      @else   数据处理模块参数 @endif */
} uapi_dpm_attr;

/**
 * @if Eng
 * @brief  Defines dpm event callback function.
 * @else
 * @brief  定义数据处理事件回调函数
 * @endif
 */
typedef td_s32 (*uapi_dpm_event_proc)(td_handle dpm, uapi_dpm_event_type event, td_void *param, td_void *context);

/**
 * @if Eng
 * @brief  Initialize Data Process module.
 * @attention Call uapi_dpm_init at first before calling other dpm api(s)
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  初始化数据处理模块
 * @attention 调用dpm模块要求首先调用该接口
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_dpm_init(td_void);

/**
 * @if Eng
 * @brief  Deinitialize Data Process module.
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  去初始化数据处理模块
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_dpm_deinit(td_void);

/**
 * @if Eng
 * @brief  Create a instance for dpm.
 * @param  [out] dpm dpm handle pointer
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  打开dpm通道，创建句柄
 * @param  [out] dpm dpm句柄指针
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_dpm_create(td_handle *dpm);

/**
 * @if Eng
 * @brief  Destory dpm instance.
 * @param  [in]  dpm dpm handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  关闭dpm通道，销毁句柄
 * @param  [in]  dpm dpm句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_dpm_destroy(td_handle dpm);

/**
 * @if Eng
 * @brief  Register callback to process dpm events.
 * @param  [in]  dpm dpm handle
 * @param  [in]  event_proc User callback to process dpm events
 * @param  [in]  context Data passthrough to callback
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  注册dpm事件处理的回调函数
 * @param  [in]  dpm dpm句柄
 * @param  [in]  event_proc 用户的dpm事件处理回调函数
 * @param  [in]  context 透传给回调函数的数据
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_dpm_register_event_proc(td_handle dpm, uapi_dpm_event_proc event_proc, td_void *context);

/**
 * @if Eng
 * @brief  Start dpm instance to process PCM data.
 * @param  [in]  dpm dpm handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  启动dpm通道，对PCM数据进行编码
 * @param  [in]  dpm dpm句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_dpm_start(td_handle dpm);

/**
 * @if Eng
 * @brief  Stop dpm instance to process PCM data.
 * @param  [in]  dpm dpm handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  停止dpm通道，停止PCM数据编码
 * @param  [in]  dpm dpm句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_dpm_stop(td_handle dpm);

/**
 * @if Eng
 * @brief  Set attributes of dpm instance.
 * @param  [in]  dpm dpm handle
 * @param  [in]  attr dpm instance attribute
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  设置dpm通道属性
 * @param  [in]  dpm dpm句柄
 * @param  [in]  attr dpm实例属性
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_dpm_set_attr(td_handle dpm, const uapi_dpm_attr *attr);

/**
 * @if Eng
 * @brief  Get attributes of dpm instance.
 * @param  [in]  dpm dpm handle
 * @param  [out] attr dpm instance attribute
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  获取dpm通道属性
 * @param  [in]  dpm dpm句柄
 * @param  [out] attr dpm实例属性
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_dpm_get_attr(td_handle dpm, uapi_dpm_attr *attr);

/**
 * @if Eng
 * @brief  Attach a output instance to dpm instance.
 * @param  [in]  dpm dpm handle
 * @param  [in]  output Output instance handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  绑定输出实例给dpm实例
 * @param  [in]  dpm dpm句柄
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_dpm_attach_output(td_handle dpm, td_handle output);

/**
 * @if Eng
 * @brief  Detach a output instance from dpm instance.
 * @param  [in]  dpm dpm handle
 * @param  [in]  output Output instance handle
 * @retval SUCCESS   Success
 * @retval FAILURE   Failure
 * @else
 * @brief  解绑dpm实例的输出实例
 * @param  [in]  dpm dpm句柄
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS   成功
 * @retval FAILURE   失败
 * @endif
 */
td_s32 uapi_dpm_detach_output(td_handle dpm, td_handle output);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_DPM_H__ */
