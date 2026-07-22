/**
 * Copyright (c) @CompanyNameMagicTag 2012-2022. All rights reserved. \n
 * Description: header file for audio output control \n
 * Author: @CompanyNameTag \n
 */

#ifndef __SOC_UAPI_SOUND_H__
#define __SOC_UAPI_SOUND_H__

#include "soc_uapi_audio.h"
#include "soc_uapi_aef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup soc_uapi_sound soc_uapi_sound
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  Maximum track inst num.
 * @else
 * @brief  最大音频通道数
 * @endif
 */
#define UAPI_SND_TRACK_MAX 4

/**
 * @if Eng
 * @brief  Maximum sound outputport.
 * @else
 * @brief  最大声卡输出端口
 * @endif
 */
#define UAPI_SND_OUT_PORT_NUM_MAX 4

/**
 * @if Eng
 * @brief  The ID of the audio sound device.
 * @else
 * @brief  音频声卡设备号
 * @endif
 */
typedef enum {
    UAPI_SND_0 = 0, /*!< @if Eng Audio sound device 0.
                         @else   音频声卡设备0 @endif */
    UAPI_SND_1,     /*!< @if Eng Audio sound device 1.
                         @else   音频声卡设备1 @endif */
    UAPI_SND_2,     /*!< @if Eng Audio sound device 2.
                         @else   音频声卡设备2 @endif */

    UAPI_SND_MAX,
} uapi_snd;

/**
 * @if Eng
 * @brief  High Precision Gain, ranging from -81dB to 18dB, step 0.125dB.
 * @else
 * @brief  音频输出增益，范围是[-81dB, 18dB]，步进为0.125dB
 * @endif
 */
typedef struct {
    td_s32 integer; /*!< @if Eng Interger part of high preicision gain.
                         @else   高精度增益的整数部分 @endif */

    td_s32 decimal; /*!< @if Eng Decimal part of high preicision gain, if -0.125dB, value is -125.
                         @else   高精度增益的小数部分，如果是-0.125dB，值为-125 @endif */
} uapi_snd_gain;

/**
 * @if Eng
 * @brief  Audio port: DAC0,I2S0,SPDIF0,HDMI0,ARC0.
 * @else
 * @brief  音频输出端口定义：DAC0,I2S0,SPDIF0,HDMI0,ARC0
 * @endif
 */
typedef enum {
    UAPI_SND_OUT_PORT_DAC0 = 0x00,
    UAPI_SND_OUT_PORT_DAC1,
    UAPI_SND_OUT_PORT_DAC2,
    UAPI_SND_OUT_PORT_DAC3,

    UAPI_SND_OUT_PORT_I2S0 = 0x10,
    UAPI_SND_OUT_PORT_I2S1,
    UAPI_SND_OUT_PORT_I2S2,
    UAPI_SND_OUT_PORT_I2S3,
    UAPI_SND_OUT_PORT_I2S4,

    UAPI_SND_OUT_PORT_SPDIF0 = 0x20,

    UAPI_SND_OUT_PORT_HDMI0 = 0x30,
    UAPI_SND_OUT_PORT_HDMI1,

    UAPI_SND_OUT_PORT_ARC0 = 0x40,
    UAPI_SND_OUT_PORT_ARC1,

    UAPI_SND_OUT_PORT_USB0 = 0x50,
    UAPI_SND_OUT_PORT_USB1,

    UAPI_SND_OUT_PORT_BT0 = 0x60,
    UAPI_SND_OUT_PORT_BT1,

    UAPI_SND_OUT_PORT_CAST0 = 0x70,
    UAPI_SND_OUT_PORT_CAST1,

    UAPI_SND_OUT_PORT_ALL = 0x7fff,

    UAPI_SND_OUT_PORT_MAX,
} uapi_snd_out_port;

/**
 * @if Eng
 * @brief  Defines the mode of audio channels.
 * @else
 * @brief  定义音轨模式
 * @endif
 */
typedef enum {
    UAPI_SND_TRACK_STEREO = 0,   /*!< @if Eng Stereo.
                                      @else   立体声 @endif */
    UAPI_SND_TRACK_DOUBLE_MONO,  /*!< @if Eng Data is output after being mixed in the audio-left channel \n
                                              and audio-right channel. \n
                                      @else   左右声道混合后输出 @endif */
    UAPI_SND_TRACK_DOUBLE_LEFT,  /*!< @if Eng The audio-left channel and audio-right channel output \n
                                              the data of the audio-left channel. \n
                                      @else   左右声道输出左声道数据 @endif */
    UAPI_SND_TRACK_DOUBLE_RIGHT, /*!< @if Eng The audio-left channel and audio-right channel output \n
                                              the data of the audio-right channel. \n
                                      @else   左右声道输出右声道数据 @endif */
    UAPI_SND_TRACK_EXCHANGE,     /*!< @if Eng Data is output after being exchanged in the audio-left channel \n
                                              and audio-right channel. \n
                                      @else   左右声道数据交换输出 @endif */
    UAPI_SND_TRACK_ONLY_RIGHT,   /*!< @if Eng Only the data in the audio-right channel is output.
                                      @else   只输出右声道数据 @endif */
    UAPI_SND_TRACK_ONLY_LEFT,    /*!< @if Eng Only the data in the audio-left channel is output.
                                      @else   只输出左声道数据 @endif */
    UAPI_SND_TRACK_MUTE,         /*!< @if Eng Mute.
                                      @else   静音 @endif */

    UAPI_SND_TRACK_MODE_MAX,
} uapi_snd_track_mode;

/**
 * @if Eng
 * @brief  Defines the state of audio channels.
 * @else
 * @brief  定义音频通道状态
 * @endif
 */
typedef enum {
    UAPI_SND_TRACK_STOP = 0,
    UAPI_SND_TRACK_START,
    UAPI_SND_TRACK_PAUSE,

    UAPI_SND_TRACK_STATE_MAX,
} uapi_snd_track_state;

/**
 * @if Eng
 * @brief  Internal Audio DAC outport attribute.
 * @else
 * @brief  内部音频DAC输出端口属性
 * @endif
 */
typedef struct {
    td_u32 reserved;
} uapi_snd_dac_attr;

/**
 * @if Eng
 * @brief  Audio I2S outport attribute.
 * @else
 * @brief  音频I2S输出端口属性
 * @endif
 */
typedef struct {
    uapi_audio_i2s_attr attr;
} uapi_snd_i2s_attr;

/**
 * @if Eng
 * @brief  Audio outport attribute.
 * @else
 * @brief  音频输出端口属性
 * @endif
 */
typedef struct {
    uapi_snd_out_port out_port;
    union {
        uapi_snd_dac_attr dac_attr;
        uapi_snd_i2s_attr i2s_attr;
    } u;
} uapi_snd_out_port_attr;

/**
 * @if Eng
 * @brief  Audio Sound device attribute.
 * @else
 * @brief  音频声卡设备属性
 * @endif
 */
typedef struct {
    uapi_audio_channel channels;
    uapi_audio_bit_depth bit_depth;
    uapi_audio_sample_rate sample_rate;
    td_u32 port_num; /*!< @if Eng Outport number attached to sound.
                          @else   绑定到声卡的输出端口 @endif */
    uapi_snd_out_port_attr port_attr[UAPI_SND_OUT_PORT_NUM_MAX]; /*!< @if Eng Outport attr attached to sound.
                                                                      @else   绑定到声卡的输出端口的属性 @endif */
} uapi_snd_attr;

/**
 * @if Eng
 * @brief  Audio Sound track type
 * @else
 * @brief  音频声卡通道类型
 * @endif
 */
typedef enum {
    UAPI_SND_TRACK_TYPE_MASTER = 0, /*!< @if Eng Master Track.
                                         @else   主通道 @endif */
    UAPI_SND_TRACK_TYPE_SLAVE,      /*!< @if Eng Slave Track.
                                         @else   副通道 @endif */
    UAPI_SND_TRACK_TYPE_BT_MUSIC,   /*!< @if Eng Bluetooth Music Track.
                                         @else   蓝牙音乐通道 @endif */
    UAPI_SND_TRACK_TYPE_BT_VOIP,    /*!< @if Eng Bluetooth voip call Track.
                                         @else   蓝牙通话通道 @endif */

    UAPI_SND_TRACK_TYPE_MAX,
} uapi_snd_track_type;

/**
 * @if Eng
 * @brief  Audio output attribute.
 * @else
 * @brief  音频输出通道属性
 * @endif
 */
typedef struct {
    uapi_snd_track_type track_type; /*!< @if Eng Audio Sound track type
                                         @else   音频声卡通道类型 @endif */
    td_u32 fade_in_ms;  /*!< @if Eng Fade in time(unit:ms).
                             @else   淡入时间（单位：毫秒） @endif */
    td_u32 fade_out_ms; /*!< @if Eng Fade out time(unit:ms).
                             @else   淡出时间（单位：毫秒） @endif */
    td_u32 buf_dur_ms;  /*!< @if Eng Buffer duration(unit: ms).
                             @else   缓冲持续时间（单位：毫秒） @endif */
} uapi_snd_track_attr;

/**
 * @if Eng
 * @brief  Sound audio effect profile.
 * @else
 * @brief  声卡音效配置枚举
 * @endif
 */
typedef enum {
    UAPI_SND_AEF_PROFILE_NONE = 0,
    UAPI_SND_AEF_PROFILE_MUSIC,
    UAPI_SND_AEF_PROFILE_VOIP,
    UAPI_SND_AEF_PROFILE_RINGTONE,

    UAPI_SND_AEF_PROFILE_MAX,
} uapi_snd_aef_profile;

/**
 * @if Eng
 * @brief  Initialize an sound device.
 * @attention Call uapi_snd_init at first before calling other sound api(s)
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  初始化声卡
 * @attention 调用sound模块要求首先调用该接口
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_init(td_void);

/**
 * @if Eng
 * @brief  Initialize an sound device.
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  去初始化声卡
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_deinit(td_void);

/**
 * @if Eng
 * @brief  Obtains the default open attribute of an sound device.
 * @param  [in]  snd_id Sound ID
 * @param  [out] attr Sound attribute
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取声卡的默认属性
 * @param  [in]  snd_id 声卡端口号
 * @param  [out] attr 声卡属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_default_attr(uapi_snd snd_id, uapi_snd_attr *attr);

/**
 * @if Eng
 * @brief  Open an AO device.
 * @param  [out] sound Sound handle pointer
 * @param  [in]  snd_id Sound ID
 * @param  [in]  attr Sound attribute
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  打开音频输出设备
 * @param  [out] sound 声卡句柄指针
 * @param  [in]  snd_id 声卡端口号
 * @param  [in]  attr 声卡属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_open(td_handle *sound, uapi_snd snd_id, const uapi_snd_attr *attr);

/**
 * @if Eng
 * @brief  Close an AO sound instance.
 * @param  [in]  sound Sound handle
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  关闭音频输出实例
 * @param  [in]  sound 声卡句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_close(td_handle sound);

/**
 * @if Eng
 * @brief  Get start/stop status of outport.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [out] enable Enable/disable flag
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取音频输出设备状态
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [out] enable 使能标志
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_port_enable(td_handle sound, uapi_snd_out_port port, td_bool *enable);

/**
 * @if Eng
 * @brief  Sets out port Enable.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [in]  enable Enable/disable flag
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置音频输出设备状态
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [in]  enable 使能标志
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_set_port_enable(td_handle sound, uapi_snd_out_port port, td_bool enable);

/**
 * @if Eng
 * @brief  Obtains the mute status of an sound port.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [out] mute Mute/unmute flag
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取声卡静音状态
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [out] mute 静音标志
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_mute(td_handle sound, uapi_snd_out_port port, td_bool *mute);

/**
 * @if Eng
 * @brief  Sets the mute status of an sound port.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [out] mute Mute/unmute flag
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置声卡静音状态
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [out] mute 静音标志
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_set_mute(td_handle sound, uapi_snd_out_port port, td_bool mute);

/**
 * @if Eng
 * @brief  Obtains the output volume value.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [out] gain Volume
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取音频输出音量
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [out] gain 音量
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_volume(td_handle sound, uapi_snd_out_port port, uapi_snd_gain *gain);

/**
 * @if Eng
 * @brief  Sets the output volume value.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [in]  gain Volume
 * @param  [in]  toggle_config volume toggle config
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置音频输出音量
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [in]  gain 音量
 * @param  [in]  toggle_config 音量同步生效配置
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_set_volume(td_handle sound, uapi_snd_out_port port, const uapi_snd_gain *gain,
    const uapi_audio_toggle_config *toggle_config);

/**
 * @if Eng
 * @brief  Obtains the AO track mode.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [out] mode Audio track mode
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取音频输出通道模式
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [out] mode 音频通道模式
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_track_mode(td_handle sound, uapi_snd_out_port port, uapi_snd_track_mode *mode);

/**
 * @if Eng
 * @brief  Set the AO track mode.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [in]  mode Audio track mode
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置音频输出通道模式
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [in]  mode 音频通道模式
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_set_track_mode(td_handle sound, uapi_snd_out_port port, uapi_snd_track_mode mode);

/**
 * @if Eng
 * @brief  Gets mute status of all players.
 * @param  [in]  sound Sound handle
 * @param  [out] mute Mute status
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取所有播放的静音状态
 * @param  [in]  sound 声卡句柄
 * @param  [out] mute 静音状态
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_all_track_mute(td_handle sound, td_bool *mute);

/**
 * @if Eng
 * @brief  Sets mute or unmute of all players.
 * @param  [in]  sound Sound handle
 * @param  [in]  mute Mute status
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置所有播放的静音状态
 * @param  [in]  sound 声卡句柄
 * @param  [in]  mute 静音状态
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_set_all_track_mute(td_handle sound, td_bool mute);

/**
 * @if Eng
 * @brief  Attach a output instance to sound.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [in]  output Output handle
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  将输出实例附加到声音
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [in]  output 输出句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_attach_output(td_handle sound, uapi_snd_out_port port, td_handle output);

/**
 * @if Eng
 * @brief  Detach a output instance from sound.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [in]  output Output handle
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  从声音中分离输出实例
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [in]  output 输出句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_detach_output(td_handle sound, uapi_snd_out_port port, td_handle output);

/**
 * @if Eng
 * @brief  Get algorithm parameters.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [in]  cmd Instruction
 * @param  [in]  param_size parameter size
 * @param  [out] param Parameter information
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取算法参数
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [in]  cmd 指令
 * @param  [in]  param_size 参数大小
 * @param  [out] param 参数信息
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_aef_param(td_handle sound, uapi_snd_out_port port,
    td_u32 cmd, td_void *param, td_u32 param_size);

/**
 * @if Eng
 * @brief  Get algorithm parameters.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [in]  cmd Instruction
 * @param  [in]  param_size parameter size
 * @param  [in]  param Parameter information
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置算法参数
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [in]  cmd 指令
 * @param  [in]  param_size 参数大小
 * @param  [in]  param 参数信息
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_set_aef_param(td_handle sound, uapi_snd_out_port port,
    td_u32 cmd, const td_void *param, td_u32 param_size);

/**
 * @if Eng
 * @brief  Sound audio effect profile.
 * @param  [in]  sound Sound handle
 * @param  [in]  aef_profile Effect profile
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  声音音效配置文件
 * @param  [in]  sound 声卡句柄
 * @param  [in]  aef_profile 音效配置
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_set_aef_profile(td_handle sound, uapi_snd_aef_profile aef_profile);

/**
 * @if Eng
 * @brief  Enable or Disable audio effect of sound port.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [in]  aef_type Audio effect type
 * @param  [in]  enable Enable or Disable
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  打开或者关闭端口的音效
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [in]  aef_type 音效类型
 * @param  [in]  enable 打开或关闭标志
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_set_port_aef_enable(td_handle sound, uapi_snd_out_port out_port,
    uapi_aef_type effect_type, td_bool enable);

/**
 * @if Eng
 * @brief  Get the audio effect status of sound port.
 * @param  [in]  sound Sound handle
 * @param  [in]  port Sound port
 * @param  [in]  aef_type Audio effect type
 * @param  [out] enable Enable or Disable
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取端口的音效状态
 * @param  [in]  sound 声卡句柄
 * @param  [in]  port 声卡端口
 * @param  [in]  aef_type 音效类型
 * @param  [out] enable 打开或关闭标志
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_port_aef_enable(td_handle sound, uapi_snd_out_port out_port,
    uapi_aef_type effect_type, td_bool *enable);

/**
 * @if Eng
 * @brief  Set speaker calibration biquad filters.
 * @param  [in]  sound handle
 * @param  [in]  Calib filters parameters, e.g. freqs, Qs, gains, filters are 8 bands
 * @param  [in]  Calib filters parameters' length in byte, it's value is 48
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  设置喇叭输出校准滤波器参数
 * @param  [in]  sound句柄
 * @param  [in]  校准滤波器参数, 包括中心频率、品质因数、增益等，滤波器共8段
 * @param  [in]  滤波器参数长度字节数，值为48
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_set_spk_cali_coef(td_handle sound, const td_u8 *filter_coefs, td_u32 coef_len);

/**
 * @if Eng
 * @brief  Get track default attribute.
 * @param  [out] attr Track attribute
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取音频通道默认属性
 * @param  [out] attr 音频通道属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_track_default_attr(uapi_snd_track_attr *attr);

/**
 * @if Eng
 * @brief  Create a Track.
 * @param  [in]  sound Sound handle
 * @param  [in]  attr Track attributes
 * @param  [out] track Track handle pointer
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  创建音频通道
 * @param  [in]  sound 声卡句柄
 * @param  [in]  attr 音频通道属性
 * @param  [out] track 音频通道句柄指针
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_create_track(td_handle *track, td_handle sound, const uapi_snd_track_attr *attr);

/**
 * @if Eng
 * @brief  Destroy a Track.
 * @param  [in]  Track handle
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  销毁音频通道
 * @param  [in]  sound 音频通道句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_destroy_track(td_handle track);

/**
 * @if Eng
 * @brief  Get track state.
 * @param  [in]  track Track handle
 * @param  [out] state Track state
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取音频通道状态
 * @param  [in]  track 音频通道句柄
 * @param  [out] state 音频通道状态
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_track_state(td_handle track, uapi_snd_track_state *state);

/**
 * @if Eng
 * @brief  Start the specified track.
 * @param  [in]  track Track handle
 * @param  [in]  toggle_config Toggle config
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  开始指定音频通道
 * @param  [in]  track 音频通道句柄
 * @param  [in]  toggle_config 同步起播配置
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_track_start(td_handle track, const uapi_audio_toggle_config *toggle_config);

/**
 * @if Eng
 * @brief  Stop the specified track.
 * @param  [in]  track Track handle
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  停止指定音频通道
 * @param  [in]  track 音频通道句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_track_stop(td_handle track);

/**
 * @if Eng
 * @brief  Pause the specified track.
 * @param  [in]  track Track handle
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  暂停指定音频通道
 * @param  [in]  track 音频通道句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_track_pause(td_handle track);

/**
 * @if Eng
 * @brief  Resume and Start  the specified track.
 * @param  [in]  track Track handle
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  恢复并开始指定的音频通道
 * @param  [in]  track 音频通道句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_track_resume(td_handle track);

/**
 * @if Eng
 * @brief  Get track play pts(us).
 * @param  [in]  track Track handle
 * @param  [out] pts_us play pts(us)
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取音频通道播放时间戳(单位：微秒)
 * @param  [in]  track 音频通道句柄
 * @param  [out] pts_us 播放时间戳
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_track_play_pts(td_handle track, td_s64 *pts_us);

/**
 * @if Eng
 * @brief  Gets mute status of an audio player.
 * @param  [in]  track Track handle
 * @param  [out] mute Mute/Unmute flag
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取音频播放器的静音状态
 * @param  [in]  track 音频通道句柄
 * @param  [out] mute 静音状态
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_track_mute(td_handle track, td_bool *mute);

/**
 * @if Eng
 * @brief  Sets mute or unmute of an audio player.
 * @param  [in]  track Track handle
 * @param  [in]  mute Mute/Unmute flag
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置音频播放器的静音状态
 * @param  [in]  track 音频通道句柄
 * @param  [in]  mute 静音状态
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_set_track_mute(td_handle track, td_bool mute);

/**
 * @if Eng
 * @brief  Get volume of an audio player.
 * @param  [in]  track Track handle
 * @param  [out] gain Volume
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取音频播放器的音量
 * @param  [in]  track 音频通道句柄
 * @param  [out] gain 音量
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_get_track_volume(td_handle track, uapi_snd_gain *gain);

/**
 * @if Eng
 * @brief  Set volume of an audio player.
 * @param  [in]  track Track handle
 * @param  [in]  gain Volume, ranging from -81dB to 18dB
 * @param  [in]  toggle_config volume toggle config
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置音频播放器的音量
 * @param  [in]  track 音频通道句柄
 * @param  [in]  gain 音量，范围是[-81dB, 18dB]
 * @param  [in]  toggle_config 音量同步生效配置
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_set_track_volume(td_handle track, const uapi_snd_gain *gain,
    const uapi_audio_toggle_config *toggle_config);

/**
 * @if Eng
 * @brief  Attach a output instance to Track.
 * @param  [in]  track Track handle
 * @param  [in]  output Output instance handle
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  将输出实例绑定到音频通道
 * @param  [in]  track 音频通道句柄
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_attach_track_output(td_handle track, td_handle output);

/**
 * @if Eng
 * @brief  Detach a output instance from Track.
 * @param  [in]  track Track handle
 * @param  [in]  output Output instance handle
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  从音频通道分离输出实例
 * @param  [in]  track 音频通道句柄
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_snd_detach_track_output(td_handle track, td_handle output);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_SOUND_H__ */
