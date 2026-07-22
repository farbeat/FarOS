/**
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved. \n
 * Description: user api header file for audio input (AI) module \n
 * Author: @CompanyNameTag \n
 */

#ifndef __SOC_UAPI_AI_H__
#define __SOC_UAPI_AI_H__

#include "soc_uapi_sound.h"
#include "soc_uapi_vad.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @defgroup soc_uapi_ai soc_uapi_ai
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  maximum microphone num used by audio input.
 * @else
 * @brief  音频输入使用的麦克风数量最大值
 * @endif
 */
#define UAPI_AI_MIC_NUM_MAX 2

/**
 * @if Eng
 * @brief  Audio input event type.
 * @else
 * @brief  音频输入事件类型
 * @endif
 */
typedef enum {
    UAPI_AI_EVENT_VAD_VALID = 0,
    UAPI_AI_EVENT_VAD_TIMEOUT,

    UAPI_AI_EVENT_TYPE_MAX,
} uapi_ai_event_type;

/**
 * @if Eng
 * @brief  Audio input port type.
 * @else
 * @brief  音频输入端口定义
 * @endif
 */
typedef enum {
    UAPI_AI_PORT_I2S0 = 0,
    UAPI_AI_PORT_I2S1,
    UAPI_AI_PORT_I2S2,
    UAPI_AI_PORT_I2S3,
    UAPI_AI_PORT_I2S4,

    UAPI_AI_PORT_ADC0 = 0x10,
    UAPI_AI_PORT_ADC1,
    UAPI_AI_PORT_ADC2,
    UAPI_AI_PORT_ADC3,

    UAPI_AI_PORT_SIF0 = 0x20,

    UAPI_AI_PORT_HDMIRX0 = 0x30,
    UAPI_AI_PORT_HDMIRX1,
    UAPI_AI_PORT_HDMIRX2,
    UAPI_AI_PORT_HDMIRX3,

    UAPI_AI_PORT_PDM0 = 0x40,
    UAPI_AI_PORT_PDM1,
    UAPI_AI_PORT_PDM2,
    UAPI_AI_PORT_PDM3,

    UAPI_AI_PORT_LPADC0 = 0x50,
    UAPI_AI_PORT_LPADC1,

    UAPI_AI_PORT_MAX = 0xff
} uapi_ai_port;

/**
 * @if Eng
 * @brief  Defines the RX type.
 * @else
 * @brief  音频输入模块类型
 * @endif
 */
typedef enum {
    UAPI_AI_RX_DEFAULT = 0, /*!< @if Eng Default RX type:AIAO_RX
                                 @else   音频默认输入模块为AIAO_RX @endif */
    UAPI_AI_RX_MAD,         /*!< @if Eng RX type:MAD
                                 @else   音频输入模块为MAD @endif */
    UAPI_AI_RX_TYPE_MAX
} uapi_ai_rx_type;

/**
 * @if Eng
 * @brief  Defines the ADC work mode.
 * @else
 * @brief  内置音频ADC模块工作模式
 * @endif
 */
typedef enum {
    UAPI_AI_ADC_NORM_MODE = 0, /*!< @if Eng Normal mode
                                    @else   正常模式 @endif */
    UAPI_AI_ADC_LP_MODE,       /*!< @if Eng Low power mode
                                    @else   低功耗模式 @endif */
    UAPI_AI_ADC_HP_MODE,       /*!< @if Eng High performance mode
                                    @else   高性能模式 @endif */

    UAPI_AI_ADC_MODE_MAX
} uapi_ai_adc_mode;

/**
 * @if Eng
 * @brief  Defines Audio ADC port attribute.
 * @else
 * @brief  定义内置音频ADC输入端口属性
 * @endif
 */
typedef struct {
    uapi_ai_rx_type rx_type;
    uapi_ai_adc_mode mode;
} uapi_ai_adc_attr;

/**
 * @if Eng
 * @brief  Defines Audio PDM port attribute.
 * @else
 * @brief  定义音频PDM输入端口属性
 * @endif
 */
typedef struct {
    uapi_ai_rx_type rx_type;
    uapi_audio_i2s_attr i2s_attr; /*!< @if Eng I2S Attribute
                                       @else   I2S属性 @endif */
} uapi_ai_pdm_attr;

/**
 * @if Eng
 * @brief  Defines Audio I2S port attribute.
 * @else
 * @brief  定义音频I2S输入端口属性
 * @endif
 */
typedef struct {
    uapi_audio_i2s_attr i2s_attr; /*!< @if Eng I2S Attribute
                                       @else   I2S属性 @endif */
} uapi_ai_i2s_attr;

/**
 * @if Eng
 * @brief  Defines Audio input High Precision Gain.
 * @else
 * @brief  定义音频输入高精度增益
 * @endif
 */
typedef struct {
    td_s32 integer; /*!< @if Eng Interger part of high preicision gain
                         @else   高精度增益的整数部分 @endif */
    td_s32 decimal; /*!< @if Eng Decimal part of high preicision gain
                         @else   高精度增益的小数部分 @endif */
} uapi_ai_gain;

/**
 * @if Eng
 * @brief  microphone calibration volume
 * @else
 * @brief  麦克风的校准音量
 * @endif
 */
typedef struct {
    td_u32 mic_num;  /*!< @if Eng microphone num used by audio input.
                          @else   使用的麦克风数量 @endif */
    uapi_ai_gain volume[UAPI_AI_MIC_NUM_MAX]; /*!< @if Eng microphone calibration volume
                                                   @else   每个麦克风的音量校准值 @endif */
} uapi_ai_cali_volume;

/**
 * @if Eng
 * @brief  Defines Audio port attribute.
 * @else
 * @brief  定义音频输入端口属性
 * @endif
 */
typedef struct {
    uapi_audio_pcm_format pcm_attr;
    struct {
        td_bool enable;
        uapi_snd_out_port port;
    } ref_attr;
    union {
        uapi_ai_adc_attr adc;
        uapi_ai_pdm_attr pdm;
        uapi_ai_i2s_attr i2s;
    } port_attr;
    struct {
        td_bool enable;
        uapi_vad_attr attr;
    } vad_attr;
} uapi_ai_attr;

/**
 * @if Eng
 * @brief  Defines AI event callback function.
 * @else
 * @brief  定义音频输入事件回调函数
 * @endif
 */
typedef td_s32 (*uapi_ai_event_proc)(td_handle ai, uapi_ai_event_type event, const td_void *param, td_void *context);

/**
 * @if Eng
 * @brief  Initialize Audio Input module.
 * @attention Call uapi_ai_init at first before calling other ai api(s)
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  初始化音频输入模块
 * @attention 调用AI模块要求首先调用该接口
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_init(td_void);

/**
 * @if Eng
 * @brief  Deinitialize Audio Input module.
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  去初始化音频输入模块
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_deinit(td_void);

/**
 * @if Eng
 * @brief  Get the default open attributes of a AI port.
 * @param  [in]  port AI port ID
 * @param  [out] attr AI instance attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  获取AI端口默认属性
 * @param  [in]  port AI端口号
 * @param  [out] attr AI实例属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_get_default_attr(uapi_ai_port port, uapi_ai_attr *attr);

/**
 * @if Eng
 * @brief  Open a instance for AI port.
 * @param  [in]  port AI port ID
 * @param  [in]  attr AI instance attribute
 * @param  [out] ai   AI handle pointer
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  打开AI端口，创建AI句柄
 * @param  [in]  port AI端口号
 * @param  [in]  attr AI实例属性
 * @param  [out] ai   AI句柄指针
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_open(td_handle *ai, uapi_ai_port port, const uapi_ai_attr *attr);

/**
 * @if Eng
 * @brief  Close AI instance.
 * @param  [in]  ai AI handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  关闭AI端口，销毁句柄
 * @param  [in]  ai AI句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_close(td_handle ai);

/**
 * @if Eng
 * @brief  Register callback to process AI events.
 * @param  [in]  ai      AI handle
 * @param  [in]  proc    User callback to process AI events
 * @param  [in]  context Data passthrough to callback
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  注册AI事件处理的回调函数
 * @param  [in]  ai      AI句柄
 * @param  [in]  proc    用户的AI事件处理回调函数
 * @param  [in]  context 透传给回调函数的数据
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_register_event_proc(td_handle ai, uapi_ai_event_proc proc, td_void *context);

/**
 * @if Eng
 * @brief  Start AI instance to get PCM data.
 * @param  [in]  ai AI handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  启动AI通道，获取PCM数据
 * @param  [in]  ai AI句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_start(td_handle ai);

/**
 * @if Eng
 * @brief  Stop AI instance to get PCM data.
 * @param  [in]  ai AI handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  停止AI通道，停止获取PCM数据
 * @param  [in]  ai AI句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_stop(td_handle ai);

/**
 * @if Eng
 * @brief  Reset VAD to process PCM data got by AI instance.
 * @param  [in]  ai AI handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  复位VAD，处理获取的PCM数据
 * @param  [in]  ai AI句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_reset_vad(td_handle ai);

/**
 * @if Eng
 * @brief  Get attributes of AI instance.
 * @param  [in]  ai   AI handle
 * @param  [out] attr AI instance attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  获取AI端口属性
 * @param  [in]  ai   AI句柄
 * @param  [out] attr AI实例属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_get_attr(td_handle ai, uapi_ai_attr *attr);

/**
 * @if Eng
 * @brief  Set attributes of AI instance.
 * @param  [in]  ai   AI handle
 * @param  [in]  attr AI instance attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  设置AI端口属性
 * @param  [in]  ai   AI句柄
 * @param  [in]  attr AI实例属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_set_attr(td_handle ai, const uapi_ai_attr *attr);

/**
 * @if Eng
 * @brief  Get mute status of AI instance.
 * @param  [in]  ai   AI handle
 * @param  [out] mute Mute/Unmute flag
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  获取AI静音状态
 * @param  [in]  ai   AI句柄
 * @param  [out] mute 静音状态
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_get_mute(td_handle ai, td_bool *mute);

/**
 * @if Eng
 * @brief  Set mute status of AI instance.
 * @param  [in]  ai   AI handle
 * @param  [in]  mute Mute/Unmute flag
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  设置AI静音状态
 * @param  [in]  ai   AI句柄
 * @param  [in]  mute 静音状态
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_set_mute(td_handle ai, td_bool mute);

/**
 * @if Eng
 * @brief  Get the input volume of AI instance.
 * @param  [in]  ai   AI handle
 * @param  [out] gain AI port volume
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  获取AI音量
 * @param  [in]  ai   AI句柄
 * @param  [out] gain AI端口音量
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_get_volume(td_handle ai, uapi_ai_gain *gain);

/**
 * @if Eng
 * @brief  Set the input volume of AI instance.
 * @param  [in]  ai   AI handle
 * @param  [in]  gain AI port volume
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  设置AI音量
 * @param  [in]  ai   AI句柄
 * @param  [in]  gain AI端口音量
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_set_volume(td_handle ai, const uapi_ai_gain *gain);

/**
 * @if Eng
 * @brief  Set audio input microphone calibration volume.
 * @param  [in]  ai audio input handle
 * @param  [in]  cali_volume audio input microphone calibration volume
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  设置麦克风校准音量
 * @param  [in]  ai 音频输入句柄
 * @param  [in]  cali_volume 音频输入麦克风的校准音量
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_set_mic_cali_volume(td_handle ai, const uapi_ai_cali_volume *cali_volume);

/**
 * @if Eng
 * @brief  Attach a output instance to AI instance.
 * @param  [in]  ai     AI handle
 * @param  [in]  output Output instance handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  绑定输出实例给AI实例
 * @param  [in]  ai     AI句柄
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_attach_output(td_handle ai, td_handle output);

/**
 * @if Eng
 * @brief  Detach a output instance from AI instance.
 * @param  [in]  ai     AI handle
 * @param  [in]  output Output instance handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  解绑AI实例的输出实例
 * @param  [in]  ai     AI句柄
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_ai_detach_output(td_handle ai, td_handle output);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_AI_H__ */
