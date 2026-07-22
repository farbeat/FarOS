/**
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved. \n
 * Description: header file for Speech Enhancement and AI (SEA) module. \n
 * Author: @CompanyNameTag \n
 */

#ifndef __SOC_UAPI_SEA_H__
#define __SOC_UAPI_SEA_H__

#include "soc_uapi_audio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @defgroup soc_uapi_sea soc_uapi_sea
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  Max length of phrase and name.
 * @else
 * @brief  短语或名称的最大长度
 * @endif
 */
#define UAPI_SEA_STRING_SIZE  32

/**
 * @if Eng
 * @brief  Max number of Audio AI engine for a SEA instance.
 * @else
 * @brief  单个语音处理会话支持的语音智能引擎最大数目
 * @endif
 */
#define UAPI_SEA_AAI_MAX_NUM  3

/**
 * @if Eng
 * @brief  Max number of engine combination of a SEA library.
 * @else
 * @brief  语音处理库支持的引擎组合最大数目
 * @endif
 */
#define UAPI_SEA_ENG_SEL_MAX_NUM  8

/**
 * @if Eng
 * @brief  SEA library type.
 * @else
 * @brief  语音处理库类型
 * @endif
 */
typedef enum {
    UAPI_SEA_LIB_NULL = 0x00, /*!< @if Eng Invalidate Engine ID
                                   @else   无效的引擎库 @endif */
    UAPI_SEA_LIB_SEE,         /*!< @if Eng Speech Enhancement Engine
                                   @else   语音增强引擎库 @endif */
    UAPI_SEA_LIB_VQE,         /*!< @if Eng Voice Quality Engine
                                   @else   语音通话引擎库 @endif */
    UAPI_SEA_LIB_AHE,         /*!< @if Eng Augment Hearing Engine
                                   @else   人声增强引擎库 @endif */
    UAPI_SEA_LIB_KWS,         /*!< @if Eng Keyword Spotting
                                   @else   唤醒词或短语库 @endif */
    UAPI_SEA_LIB_ASR,         /*!< @if Eng Automatic Speech Recognition
                                   @else   语音识别库 @endif */
    UAPI_SEA_LIB_VID,         /*!< @if Eng Voice ID
                                   @else   声纹识别库 @endif */
    UAPI_SEA_LIB_ASD,         /*!< @if Eng Ambient Sounds Detection
                                   @else   环境声检测库 @endif */
    UAPI_SEA_LIB_HAID,        /*!< @if Eng Hearing Aid
                                   @else   助辅听算法库 @endif */
    UAPI_SEA_LIB_EXT0,        /*!< @if Eng External Engine
                                   @else   外部扩展库 @endif */
    UAPI_SEA_LIB_EXT1,        /*!< @if Eng External Engine
                                   @else   外部扩展库 @endif */
    UAPI_SEA_LIB_EXT2,        /*!< @if Eng External Engine
                                   @else   外部扩展库 @endif */
    UAPI_SEA_LIB_EXT3,        /*!< @if Eng External Engine
                                   @else   外部扩展库 @endif */
    UAPI_SEA_LIB_MAX
} uapi_sea_lib_id;

/**
 * @if Eng
 * @brief  AAI(Audio Artificial Intelligence) type.
 * @else
 * @brief  语音智能类型
 * @endif
 */
typedef enum {
    UAPI_SEA_AAI_NULL = 0x00,
    UAPI_SEA_AAI_KWS,         /*!< @if Eng Keyword Spotting
                                   @else   唤醒词或短语 @endif */
    UAPI_SEA_AAI_ASR,         /*!< @if Eng Automatic Speech Recognition
                                   @else   语音识别 @endif */
    UAPI_SEA_AAI_VID,         /*!< @if Eng Voice ID
                                   @else   声纹识别库 @endif */
    UAPI_SEA_AAI_ASD,         /*!< @if Eng Ambient Sounds Detection
                                   @else   环境声检测 @endif */
    UAPI_SEA_AAI_MAX
} uapi_sea_aai_type;

/**
 * @if Eng
 * @brief  AFE(Audio Front-End) type.
 * @else
 * @brief  语音前处理类型
 * @endif
 */
typedef enum {
    UAPI_SEA_AFE_NULL = 0x80,
    UAPI_SEA_AFE_SEE,         /*!< @if Eng Speech Enhancement Engine
                                   @else   语音增强 @endif */
    UAPI_SEA_AFE_VQE,         /*!< @if Eng Voice Quality Engine
                                   @else   语音通话 @endif */
    UAPI_SEA_AFE_AHE,         /*!< @if Eng Augment Hearing Engine
                                   @else   人声增强 @endif */
    UAPI_SEA_AFE_MAX
} uapi_sea_afe_type;

/**
 * @if Eng
 * @brief  SEA event type, reported by ::uapi_sea_event_proc.
 * @else
 * @brief  语音处理事件类型，通过::uapi_sea_event_proc上报该类型事件
 * @endif
 */
typedef enum {
    UAPI_SEA_EVENT_VAD_BEGIN = 0,  /*!< @if Eng Voice begin event
                                        @else   语音起始事件 @endif */
    UAPI_SEA_EVENT_VAD_END,        /*!< @if Eng Voice end event
                                        @else   语音结束事件 @endif */
    UAPI_SEA_EVENT_STATE_CHANGED,  /*!< @if Eng SEA state changed event, Event parameter: #uapi_sea_state_type
                                        @else   状态变更事件，事件参数: #uapi_sea_state_type @endif */
    UAPI_SEA_EVENT_KWS_MATCH,      /*!< @if Eng Keyword match event, Event parameter: #uapi_sea_kws_event_param
                                        @else   唤醒词或短语匹配事件，事件参数: #uapi_sea_kws_event_param @endif */
    UAPI_SEA_EVENT_ASR_MATCH,      /*!< @if Eng Skill match event, Event parameter: #uapi_sea_asr_event_param
                                        @else   意图或技能识别事件，事件参数: #uapi_sea_asr_event_param @endif */
    UAPI_SEA_EVENT_VID_MATCH,      /*!< @if Eng Voice ID match event, Event parameter: #uapi_sea_vid_event_param
                                        @else   身份识别事件，事件参数: #uapi_sea_vid_event_param @endif */
    UAPI_SEA_EVENT_VID_ENROLL_RET, /*!< @if Eng Enroll result event, Event parameter: #uapi_sea_vid_enroll_result
                                        @else   注册结果上报事件，事件参数: #uapi_sea_vid_enroll_result @endif */
    UAPI_SEA_EVENT_MAX
} uapi_sea_event_type;

/**
 * @if Eng
 * @brief  SEA state type.
 * @else
 * @brief  语音处理状态类型
 * @endif
 */
typedef enum {
    UAPI_SEA_STATE_IDLE = 0,  /*!< @if Eng IDLE: no voice detected
                                   @else   空闲状态 @endif */
    UAPI_SEA_STATE_WAITING,   /*!< @if Eng WAITINE: waiting for voice spot (keyword)
                                   @else   等待用户唤醒 @endif */
    UAPI_SEA_STATE_LISTENING, /*!< @if Eng LISTENING: listening for voice directive
                                   @else   等待用户指令 @endif */
    UAPI_SEA_STATE_THINKING,  /*!< @if Eng THINKING: thinking voice directive
                                   @else   正在识别用户指令 @endif */
    UAPI_SEA_STATE_MAX
} uapi_sea_state_type;

/**
 * @if Eng
 * @brief  SEA work mode.
 * @else
 * @brief  语音处理工作模式
 * @endif
 */
typedef enum {
    UAPI_SEA_NORM_MODE = 0, /*!< @if Eng Normal mode
                                 @else   正常工作模式 @endif */
    UAPI_SEA_LP_MODE,       /*!< @if Eng Low Power mode
                                 @else   低功耗模式 @endif */
    UAPI_SEA_HP_MODE,       /*!< @if Eng High Performance mode
                                 @else   高性能模式 @endif */
    UAPI_SEA_MODE_MAX
} uapi_sea_work_mode;

/**
 * @if Eng
 * @brief  SEA output track for AI engine or APP.
 * @else
 * @brief  语音处理输出通道(提供给语音AI引擎或应用)
 * @endif
 */
typedef enum {
    UAPI_SEA_OUTPUT_REC_SRC = 0, /*!< @if Eng Output source for recording
                                      @else   输出录制语音 @endif */
    UAPI_SEA_OUTPUT_ASR_SRC,     /*!< @if Eng Output source for Voice Kit APP
                                      @else   输出给识别语音 @endif */
    UAPI_SEA_OUTPUT_VC_SRC,      /*!< @if Eng Output source for Voice Call APP
                                      @else   输出通话语音 @endif */
    UAPI_SEA_OUTPUT_AH_SRC,      /*!< @if Eng Output source for AH output
                                      @else   输出增强人声 @endif */
    UAPI_SEA_OUTPUT_KWS_SRC,     /*!< @if Eng Output source for Keyword Engine
                                      @else   输出唤醒语音 @endif */
    UAPI_SEA_OUTPUT_MAX
} uapi_sea_output_type;

/**
 * @if Eng
 * @brief  Algorithm options to configure AFE stack.
 * @else
 * @brief  配置语音前处理软件的算法选项
 * @endif
 */
typedef enum {
    UAPI_SEA_AFE_OPT_NULL = 0x0000,
    UAPI_SEA_AFE_OPT_ANR  = 0x0001, /*!< @if Eng Automatic Noise Restraint
                                         @else   自动降噪选项位 @endif */
    UAPI_SEA_AFE_OPT_AEC  = 0x0002, /*!< @if Eng Acoustic Echo Cancellation
                                         @else   回声消除选项位 @endif */
    UAPI_SEA_AFE_OPT_AGC  = 0x0004, /*!< @if Eng Automatic Gain Control
                                         @else   自动增益选项位 @endif */
    UAPI_SEA_AFE_OPT_WNR  = 0x0008, /*!< @if Eng Wind Noise Reduction
                                         @else   降风噪选项位 @endif */
    UAPI_SEA_AFE_OPT_BFM  = 0x0010, /*!< @if Eng Beamforming
                                         @else   波束形成 @endif */
    UAPI_SEA_AFE_OPT_BSS  = 0x0020, /*!< @if Eng Blind Source Separation
                                         @else   盲源分离选项位 @endif */
    UAPI_SEA_AFE_OPT_DRB  = 0x0040, /*!< @if Eng De-Reverb
                                         @else   抗混响选项位 @endif */
    UAPI_SEA_AFE_OPT_LMT  = 0x0080, /*!< @if Eng Limiter
                                         @else   抗削波选项位 @endif */
    UAPI_SEA_AFE_OPT_PEQ  = 0x0100, /*!< @if Eng Parametric Equalizer
                                         @else   参数均衡器选项位 @endif */
    UAPI_SEA_AFE_OPT_DRC  = 0x0200, /*!< @if Eng Dynamic Range Control
                                         @else   动态控制选项位 @endif */
    UAPI_SEA_AFE_OPT_ALL  = 0xFFFF, /*!< @if Eng Select all options
                                         @else   所有选项 @endif */
} uapi_sea_afe_option;

/**
 * @if Eng
 * @brief  Struct of Speech Enhancement Engine attributes.
 * @else
 * @brief  语音增强引擎属性结构体
 * @endif
 */
typedef struct {
    td_u32 options; /*!< @if Eng Algorithm options: #uapi_sea_afe_option
                         @else   算法选项: #uapi_sea_afe_option @endif */
} uapi_sea_see_attr;

/**
 * @if Eng
 * @brief  Struct of Voice Quality Engine attributes.
 * @else
 * @brief  语音通话引擎属性结构体
 * @endif
 */
typedef struct {
    td_u32 options; /*!< @if Eng Algorithm options: #uapi_sea_afe_option
                         @else   算法选项: #uapi_sea_afe_option @endif */
} uapi_sea_vqe_attr;

/**
 * @if Eng
 * @brief  Struct of Augment Hearing Engine Engine attributes.
 * @else
 * @brief  人声增强引擎属性结构体
 * @endif
 */
typedef struct {
    td_s32 volume;  /*!< @if Eng Volume
                         @else   音量值 @endif */
} uapi_sea_ahe_attr;

/**
 * @if Eng
 * @brief  Struct of Keyword Spotting attributes.
 * @else
 * @brief  唤醒词或短语属性结构体
 * @endif
 */
typedef struct {
    td_u16 sensitivity;   /*!< @if Eng Sensitivity, Percentage, Rang: 0~100, default 90
                               @else   灵敏度，百分数，范围：0~100，默认值：90 @endif */
    td_u16 max_dura_ms;   /*!< @if Eng Max duration, Unit: ms, Rang: 2000~4000, default 2000
                               @else   命令词或短语最大时长，单位：ms，范围：2000~4000，默认值：2000 @endif */
    td_u16 vad_bgn_thres; /*!< @if Eng Voice begin threshold, Unit: ms, Rang: 10~200, default 40
                               @else   语音起始门限，单位：ms，范围：10~200，默认值：40 @endif */
    td_u16 vad_end_thres; /*!< @if Eng Voice end threshold, Unit: ms, Rang: 400~1000, default 500
                               @else   语音结束门限，单位：ms，范围：400~1000，默认值：500 @endif */
    td_char keyword[UAPI_SEA_STRING_SIZE]; /*!< @if Eng List of keywords or phrases
                                                @else   唤醒词或短语列表 @endif */
} uapi_sea_kws_attr;

/**
 * @if Eng
 * @brief  Struct of Automatic Speech Recognition attributes.
 * @else
 * @brief  语音识别属性结构体
 * @endif
 */
typedef struct {
    td_u16 nlp_enable;    /*!< @if Eng Nature Language Process switch
                               @else   语义理解使能开关 @endif */
    td_u16 max_dura_ms;   /*!< @if Eng Max duration, Unit: ms, Rang: 4000~60000, default 10000
                               @else   命令词或短语最大时长，单位：ms，范围：4000~60000，默认值：10000 @endif */
    td_u16 vad_bgn_thres; /*!< @if Eng Voice begin threshold, Unit: ms, Rang: 10~200, default 40
                               @else   语音起始门限，单位：ms，范围：10~200，默认值：40 @endif */
    td_u16 vad_end_thres; /*!< @if Eng Voice end threshold, Unit: ms, Rang: 400~1000, default 500
                               @else   语音结束门限，单位：ms，范围：400~1000，默认值：500 @endif */
} uapi_sea_asr_attr;

/**
 * @if Eng
 * @brief  Struct of Voice ID attributes.
 * @else
 * @brief  声纹识别属性结构体
 * @endif
 */
typedef struct {
    td_u16 sensitivity; /*!< @if Eng Sensitivity, Percentage, Rang: 0~100, default 90
                             @else   灵敏度，百分数，范围：0~100，默认值：90 @endif */
    td_u16 min_dura_ms; /*!< @if Eng Min duration, Unit: ms, Rang: 100~1000, default 500
                             @else   语音最小时长，单位：ms，范围：100~1000，默认值：500 @endif */
    td_u32 vid_count;   /*!< @if Eng Voice ID count
                             @else   声纹标识数 @endif */
} uapi_sea_vid_attr;

/**
 * @if Eng
 * @brief  Struct of Ambient Sounds Detection attributes.
 * @else
 * @brief  环境声检测属性结构体
 * @endif
 */
typedef struct {
    td_u32 latency; /*!< @if Eng Unit: ms, Rang: 10~2000, default 100
                         @else   时延，单位：ms，范围：10~2000，默认值：100 @endif */
} uapi_sea_asd_attr;

/**
 * @if Eng
 * @brief  Struct of Audio Front-End attributes.
 * @else
 * @brief  语音前处理属性结构体
 * @endif
 */
typedef struct {
    uapi_sea_afe_type type;    /*!< @if Eng AFE type
                                    @else   语音前处理类型 @endif */
    uapi_sea_work_mode mode;   /*!< @if Eng Work mode
                                    @else   工作模式 @endif */
    union {
        uapi_sea_see_attr see; /*!< @if Eng Speech Enhancement Engine attributes
                                    @else   语音增强属性 @endif */
        uapi_sea_vqe_attr vqe; /*!< @if Eng Voice Quality Engine attributes
                                    @else   语音通话属性 @endif */
        uapi_sea_ahe_attr ahe; /*!< @if Eng Augment Hearing Engine attributes
                                    @else   人声增强属性 @endif */
    } u;
} uapi_sea_afe_attr;

/**
 * @if Eng
 * @brief  Struct of Audio Artificial Intelligence attributes.
 * @else
 * @brief  语音智能属性结构体
 * @endif
 */
typedef struct {
    uapi_sea_aai_type type;    /*!< @if Eng AAI type
                                    @else   语音智能类型 @endif */
    uapi_sea_work_mode mode;   /*!< @if Eng Work mode
                                    @else   工作模式 @endif */
    union {
        uapi_sea_kws_attr kws; /*!< @if Eng Keyword Spotting attributes
                                    @else   唤醒词或短语属性 @endif */
        uapi_sea_asr_attr asr; /*!< @if Eng Automatic Speech Recognition attributes
                                    @else   语音识别属性 @endif */
        uapi_sea_vid_attr vid; /*!< @if Eng Voice ID attributes
                                    @else   声纹识别属性 @endif */
        uapi_sea_asd_attr asd; /*!< @if Eng Ambient Sounds Detection attributes
                                    @else   环境声检测属性 @endif */
    } u;
} uapi_sea_aai_attr;

/**
 * @if Eng
 * @brief  Struct of Engine Selector for SEA chain configure.
 * @else
 * @brief  引擎选择器（用于配置SEA链路）结构体
 * @endif
 */
typedef struct {
    struct {
        uapi_sea_afe_type type; /*!< @if Eng AFE type
                                     @else   语音前处理类型 @endif */
        uapi_sea_lib_id lib_id; /*!< @if Eng SEA library type
                                     @else   SEA库类型 @endif */
    } afe;
    struct {
        uapi_sea_aai_type type; /*!< @if Eng AAI type
                                     @else   语音智能类型 @endif */
        uapi_sea_lib_id lib_id; /*!< @if Eng SEA library type
                                     @else   SEA库类型 @endif */
    } aai[UAPI_SEA_AAI_MAX_NUM];
} uapi_sea_eng_sel;

/**
 * @if Eng
 * @brief  Struct of Engine capability.
 * @else
 * @brief  语音处理引擎能力结构体
 * @endif
 */
typedef struct {
    uapi_sea_eng_sel eng_sel[UAPI_SEA_ENG_SEL_MAX_NUM]; /*!< @if Eng Engine selector
                                                             @else   引擎选择器列表 @endif */
    td_u32 eng_sel_cnt;                                 /*!< @if Eng Count of Engine selector
                                                             @else   引擎选择器数目 @endif */
} uapi_sea_eng_caps;

/**
 * @if Eng
 * @brief  Struct of SEA attributes.
 * @else
 * @brief  语音处理属性结构体
 * @endif
 */
typedef struct {
    td_u32 in_buf_dur_ms;                               /*!< @if Eng Input buffer duration
                                                             @else   输入缓存时长 @endif */
    uapi_audio_pcm_format in_pcm;                       /*!< @if Eng Input PCM format
                                                             @else   输入PCM格式 @endif */
    uapi_audio_pcm_format ref_pcm;                      /*!< @if Eng Reference PCM format
                                                             @else   参考PCM格式 @endif */
    uapi_audio_pcm_format out_pcm[UAPI_SEA_OUTPUT_MAX]; /*!< @if Eng Output PCM format
                                                             @else   输出PCM格式 @endif */
} uapi_sea_attr;

/**
 * @if Eng
 * @brief  Struct of SEA item attributes.
 * @else
 * @brief  语音处理条目属性结构体
 * @endif
 */
typedef struct {
    td_u32 id;                        /*!< @if Eng ID
                                           @else   标识 @endif */
    td_u8 name[UAPI_SEA_STRING_SIZE]; /*!< @if Eng Name
                                           @else   名称 @endif */
} uapi_sea_item;

/**
 * @if Eng
 * @brief  Struct of SEA item sets attributes.
 * @else
 * @brief  语音处理条目集属性结构体
 * @endif
 */
typedef struct {
    td_u32 size;          /*!< @if Eng Input array size
                               @else   输入的数组容量 @endif */
    td_u32 count;         /*!< @if Eng Output item count
                               @else   输出的条目数 @endif */
    uapi_sea_item *array; /*!< @if Eng Array of SEA items
                               @else   SEA条目数组 @endif */
} uapi_sea_item_sets;

/**
 * @if Eng
 * @brief  Struct of Event param for #UAPI_SEA_EVENT_KWS_MATCH.
 * @else
 * @brief  #UAPI_SEA_EVENT_KWS_MATCH 事件参数结构体
 * @endif
 */
typedef struct {
    td_u32 match_score;  /*!< @if Eng Match score
                              @else   唤醒词或短语匹配率 @endif */
    td_u32 start_offset; /*!< @if Eng PCM stream bytes start offset
                              @else   PCM流起始字节偏移 @endif */
    td_u32 end_offset;   /*!< @if Eng PCM stream bytes end offset
                              @else   PCM流结束字节偏移 @endif */
    td_u32 target_angle; /*!< @if Eng Source angle
                              @else   声源角度 @endif */
    td_u32 phrase_id;    /*!< @if Eng Keyword or phrase ID
                              @else   唤醒词或短语标识 @endif */
} uapi_sea_kws_event_param;

/**
 * @if Eng
 * @brief  Struct of Event param for #UAPI_SEA_EVENT_ASR_MATCH.
 * @else
 * @brief  #UAPI_SEA_EVENT_ASR_MATCH 事件参数结构体
 * @endif
 */
typedef struct {
    td_u32 skill_id;  /*!< @if Eng Skill or phrase ID
                           @else   技能或短语编号 @endif */
    td_u32 voice_id;  /*!< @if Eng Voice ID
                           @else   声纹标识 @endif */
    td_u8 *data_buf;  /*!< @if Eng Data buffer
                           @else   处理后的数据缓存 @endif */
    td_u32 data_size; /*!< @if Eng Data Size, Unit: byte
                           @else   数据量，单位：字节 @endif */
} uapi_sea_asr_event_param;

/**
 * @if Eng
 * @brief  Struct of Event param for #UAPI_SEA_EVENT_VID_MATCH.
 * @else
 * @brief  #UAPI_SEA_EVENT_VID_MATCH 事件参数结构体
 * @endif
 */
typedef struct {
    td_u32 voice_id; /*!< @if Eng Voice ID
                          @else   声纹标识 @endif */
} uapi_sea_vid_event_param;

/**
 * @if Eng
 * @brief  Struct of Event param for #UAPI_SEA_EVENT_VID_ENROLL_RET.
 * @else
 * @brief  #UAPI_SEA_EVENT_VID_ENROLL_RET 事件参数结构体
 * @endif
 */
typedef struct {
    td_u32 result;   /*!< @if Eng If SUCCESS, call : #uapi_sea_add_vid_entity
                          @else   若为SUCCESS，调用: #uapi_sea_add_vid_entity @endif */
    td_u32 voice_id; /*!< @if Eng If result is SUCCESS, voice_id is validated
                          @else   若结果为SUCCESS，声纹标识已验证 @endif */
} uapi_sea_vid_enroll_result;

/**
 * @if Eng
 * @brief  Callback function invoked when SEA event is reported.
 * @attention This function is called in driver context,should not be blocked or do long time waiting. \n
 *            The memories of param are requested and freed by the driver automatically.
 * @param  [in]  sea     Instance handle
 * @param  [in]  event   Event type
 * @param  [in]  param   Event parameter
 * @param  [in]  context User context
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_register_event_proc
 * @else
 * @brief  语音处理事件上报时调用的回调函数
 * @attention 该回调函数运行于驱动线程，不能阻塞或长时间等待 \n
 *            事件参数由驱动申请内存，也由驱动释放，回调中不应释放
 * @param  [in]  sea     实例句柄
 * @param  [in]  event   事件类型
 * @param  [in]  param   事件参数
 * @param  [in]  context 用户上下文
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_register_event_proc
 * @endif
 */
typedef td_s32 (*uapi_sea_event_proc)(td_handle sea, uapi_sea_event_type event, td_void *param, td_void *context);

/**
 * @if Eng
 * @brief  Initialize SEA module.
 * @attention Call uapi_sea_init at first before calling other sea api(s)
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_deinit
 * @else
 * @brief  初始化语音处理模块
 * @attention 调用SEA模块要求首先调用该接口
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_deinit
 * @endif
 */
td_s32 uapi_sea_init(td_void);

/**
 * @if Eng
 * @brief  Deinitializes SEA module.
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_init
 * @else
 * @brief  去初始化语音处理模块
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_init
 * @endif
 */
td_s32 uapi_sea_deinit(td_void);

/**
 * @if Eng
 * @brief  Load SEA engine library.
 * @attention The related engine should be loaded to RAM before used
 * @param  [in]  lib_id   Library ID #uapi_sea_lib_id
 * @param  [in]  lib_name Library name or path
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_unload_engine
 * @else
 * @brief  加载语音处理引擎库
 * @attention 相关的引擎库必须先加载后使用
 * @param  [in]  lib_id   引擎库标识 #uapi_sea_lib_id
 * @param  [in]  lib_name 引擎库名称或路径
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_unload_engine
 * @endif
 */
td_s32 uapi_sea_load_engine(uapi_sea_lib_id lib_id, const td_char *lib_name);

/**
 * @if Eng
 * @brief  Unload SEA engine library.
 * @param  [in]  lib_id   Library ID #uapi_sea_lib_id
 * @param  [in]  lib_name Library name or path
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_load_engine
 * @else
 * @brief  卸载语音处理引擎库
 * @param  [in]  lib_id   引擎库标识 #uapi_sea_lib_id
 * @param  [in]  lib_name 引擎库名称或路径
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_load_engine
 * @endif
 */
td_s32 uapi_sea_unload_engine(uapi_sea_lib_id lib_id, const td_char *lib_name);

/**
 * @if Eng
 * @brief  Get the capability of engine library.
 * @param  [in]  lib_id   Library ID #uapi_sea_lib_id
 * @param  [out] eng_caps Engine capabilities
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  获取引擎库的能力
 * @param  [in]  lib_id   引擎库标识 #uapi_sea_lib_id
 * @param  [out] eng_caps 引擎库能力
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_sea_get_engine_caps(uapi_sea_lib_id lib_id, uapi_sea_eng_caps *eng_caps);

/**
 * @if Eng
 * @brief  Get the default attributes of SEA.
 * @param  [in]  sea_sel  Engine selector
 * @param  [out] sea_attr SEA instance attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  获取指定配置的语音处理默认属性
 * @param  [in]  sea_sel  引擎选择器
 * @param  [out] sea_attr SEA实例属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_sea_get_default_attr(const uapi_sea_eng_sel *sea_sel, uapi_sea_attr *sea_attr);

/**
 * @if Eng
 * @brief  Create a instance for SEA.
 * @param  [in]  sea_sel  Engine selector
 * @param  [in]  sea_attr SEA instance attribute
 * @param  [out] sea      SEA handle pointer
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_destroy
 * @else
 * @brief  创建语音处理实例
 * @param  [in]  sea_sel  引擎选择器
 * @param  [in]  sea_attr SEA实例属性
 * @param  [out] sea      SEA句柄指针
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_destroy
 * @endif
 */
td_s32 uapi_sea_create(td_handle *sea, const uapi_sea_eng_sel *sea_sel, const uapi_sea_attr *sea_attr);

/**
 * @if Eng
 * @brief  Destroy SEA instance.
 * @param  [in]  sea SEA handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_create
 * @else
 * @brief  销毁语音处理实例
 * @param  [in]  sea SEA句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_create
 * @endif
 */
td_s32 uapi_sea_destroy(td_handle sea);

/**
 * @if Eng
 * @brief  Register callback to process SEA events.
 * @param  [in]  sea     SEA handle
 * @param  [in]  proc    User callback to process SEA events
 * @param  [in]  context Data passthrough to callback
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_event_proc
 * @else
 * @brief  注册语音处理事件处理的回调函数
 * @param  [in]  sea     SEA句柄
 * @param  [in]  proc    用户的SEA事件处理回调函数
 * @param  [in]  context 透传给回调函数的数据
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_event_proc
 * @endif
 */
td_s32 uapi_sea_register_event_proc(td_handle sea, uapi_sea_event_proc proc, td_void *context);

/**
 * @if Eng
 * @brief  Start SEA instance.
 * @param  [in]  sea SEA handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_stop
 * @else
 * @brief  启动语音处理实例
 * @param  [in]  sea SEA句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_stop
 * @endif
 */
td_s32 uapi_sea_start(td_handle sea);

/**
 * @if Eng
 * @brief  Stop SEA instance.
 * @param  [in]  sea SEA handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_start
 * @else
 * @brief  停止语音处理实例
 * @param  [in]  sea SEA句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_start
 * @endif
 */
td_s32 uapi_sea_stop(td_handle sea);

/**
 * @if Eng
 * @brief  Get attributes of SEA instance.
 * @param  [in]  sea      SEA handle
 * @param  [out] sea_attr SEA instance attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_set_attr
 * @else
 * @brief  获取语音处理实例属性
 * @param  [in]  sea      SEA句柄
 * @param  [out] sea_attr SEA实例属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_set_attr
 * @endif
 */
td_s32 uapi_sea_get_attr(td_handle sea, uapi_sea_attr *sea_attr);

/**
 * @if Eng
 * @brief  Set attributes of SEA instance.
 * @param  [in]  sea      SEA handle
 * @param  [in]  sea_attr SEA instance attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_get_attr
 * @else
 * @brief  设置语音处理实例属性
 * @param  [in]  sea      SEA句柄
 * @param  [in]  sea_attr SEA实例属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_get_attr
 * @endif
 */
td_s32 uapi_sea_set_attr(td_handle sea, const uapi_sea_attr *sea_attr);

/**
 * @if Eng
 * @brief  Get attributes of AFE engine.
 * @param  [in]  sea      SEA handle
 * @param  [in]  type     AFE engine type
 * @param  [out] afe_attr AFE engine attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_set_afe_attr
 * @else
 * @brief  获取语音前处理引擎属性
 * @param  [in]  sea      SEA句柄
 * @param  [in]  type     语音前处理引擎类型
 * @param  [out] afe_attr 语音前处理引擎属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_set_afe_attr
 * @endif
 */
td_s32 uapi_sea_get_afe_attr(td_handle sea, uapi_sea_afe_type type, uapi_sea_afe_attr *afe_attr);

/**
 * @if Eng
 * @brief  Set attributes of AFE engine.
 * @param  [in]  sea      SEA handle
 * @param  [in]  type     AFE engine type
 * @param  [in]  afe_attr AFE engine attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_get_afe_attr
 * @else
 * @brief  设置语音前处理引擎属性
 * @param  [in]  sea      SEA句柄
 * @param  [in]  type     语音前处理引擎类型
 * @param  [in]  afe_attr 语音前处理引擎属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_get_afe_attr
 * @endif
 */
td_s32 uapi_sea_set_afe_attr(td_handle sea, uapi_sea_afe_type type, const uapi_sea_afe_attr *afe_attr);

/**
 * @if Eng
 * @brief  Get attributes of AAI engine.
 * @param  [in]  sea      SEA handle
 * @param  [in]  type     AAI engine type
 * @param  [out] aai_attr AAI engine attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_set_aai_attr
 * @else
 * @brief  获取语音智能引擎属性
 * @param  [in]  sea      SEA句柄
 * @param  [in]  type     语音智能引擎类型
 * @param  [out] aai_attr 语音智能引擎属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_set_aai_attr
 * @endif
 */
td_s32 uapi_sea_get_aai_attr(td_handle sea, uapi_sea_aai_type type, uapi_sea_aai_attr *aai_attr);

/**
 * @if Eng
 * @brief  Set attributes of AAI engine.
 * @param  [in]  sea      SEA handle
 * @param  [in]  type     AAI engine type
 * @param  [in]  aai_attr AAI engine attribute
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_get_aai_attr
 * @else
 * @brief  设置语音前处理引擎属性
 * @param  [in]  sea      SEA句柄
 * @param  [in]  type     语音智能引擎类型
 * @param  [in]  aai_attr 语音智能引擎属性
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_set_aai_attr
 * @endif
 */
td_s32 uapi_sea_set_aai_attr(td_handle sea, uapi_sea_aai_type type, const uapi_sea_aai_attr *aai_attr);

/**
 * @if Eng
 * @brief  get algorithm parameters.
 * @param  [in]  sea      SEA handle
 * @param  [in]  type     Audio Front-End
 * @param  [out] param    algorithm parameters
 * @param  [in]  param_size parameters size
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_get_aai_attr
 * @else
 * @brief  获取算法参数
 * @param  [in]  sea      SEA句柄
 * @param  [in]  type     语音前处理类型
 * @param  [out] param    算法参数
 * @param  [in]  param_size 参数大小
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_get_param
 * @endif
 */
td_s32 uapi_sea_get_param(td_handle sea, uapi_sea_afe_type type, td_void *param, td_u32 param_size);

/**
 * @if Eng
 * @brief  Set algorithm parameters.
 * @param  [in]  sea      SEA handle
 * @param  [in]  type     Audio Front-End
 * @param  [in]  param    algorithm parameters
 * @param  [in]  param_size parameters size
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_get_aai_attr
 * @else
 * @brief  设置算法参数
 * @param  [in]  sea      SEA句柄
 * @param  [in]  type     语音前处理类型
 * @param  [in]  param    算法参数
 * @param  [in]  param_size 参数大小
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_set_param
 * @endif
 */
td_s32 uapi_sea_set_param(td_handle sea, uapi_sea_afe_type type, const td_void *param, td_u32 param_size);

/**
 * @if Eng
 * @brief  Attach a output instance to SEA instance.
 * @param  [in]  sea    SEA handle
 * @param  [in]  type   SEA output type
 * @param  [in]  output Output instance handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_detach_output
 * @else
 * @brief  绑定输出实例给语音处理实例
 * @param  [in]  sea    SEA句柄
 * @param  [in]  type   SEA输出通道类型
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_detach_output
 * @endif
 */
td_s32 uapi_sea_attach_output(td_handle sea, uapi_sea_output_type type, td_handle output);

/**
 * @if Eng
 * @brief  Detach a output instance from SEA instance.
 * @param  [in]  sea    SEA handle
 * @param  [in]  type   SEA output type
 * @param  [in]  output Output instance handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_attach_output
 * @else
 * @brief  解绑语音处理实例的输出实例
 * @param  [in]  sea    SEA句柄
 * @param  [in]  type   SEA输出通道类型
 * @param  [in]  output 输出实例句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_attach_output
 * @endif
 */
td_s32 uapi_sea_detach_output(td_handle sea, uapi_sea_output_type type, td_handle output);

/**
 * @if Eng
 * @brief  Get phrase count of SEA instance.
 * @param  [in]  sea   SEA handle
 * @param  [out] count Phrase count
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  获取语音处理支持的短语数目
 * @param  [in]  sea   SEA句柄
 * @param  [out] count 短语数目
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_sea_get_phrase_count(td_handle sea, td_u32 *count);

/**
 * @if Eng
 * @brief  Get phrase sets of SEA instance.
 * @param  [in]  sea  SEA handle
 * @param  [out] sets Phrase sets
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_get_phrase_count
 * @else
 * @brief  获取语音处理支持的短语集合
 * @param  [in]  sea  SEA句柄
 * @param  [out] sets 短语集合
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_get_phrase_count
 * @endif
 */
td_s32 uapi_sea_get_phrase_sets(td_handle sea, uapi_sea_item_sets *sets);

/**
 * @if Eng
 * @brief  Get entity count of SEA instance.
 * @param  [in]  sea   SEA handle
 * @param  [out] count Entity count
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  获取声纹实体数目
 * @param  [in]  sea   SEA句柄
 * @param  [out] count 声纹实体数目
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_sea_get_entity_count(td_handle sea, td_u32 *count);

/**
 * @if Eng
 * @brief  Get entity sets of SEA instance.
 * @param  [in]  sea  SEA handle
 * @param  [out] sets Entity sets
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_get_entity_count
 * @else
 * @brief  获取声纹实体集合
 * @param  [in]  sea  SEA句柄
 * @param  [out] sets 声纹实体集合
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_get_entity_count
 * @endif
 */
td_s32 uapi_sea_get_entity_sets(td_handle sea, uapi_sea_item_sets *sets);

/**
 * @if Eng
 * @brief  Start Voice ID enroll.
 * @param  [in]  sea SEA handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_stop_vid_enroll
 * @else
 * @brief  启动声纹注册
 * @param  [in]  sea SEA句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_stop_vid_enroll
 * @endif
 */
td_s32 uapi_sea_start_vid_enroll(td_handle sea);

/**
 * @if Eng
 * @brief  Stop Voice ID enroll.
 * @param  [in]  sea SEA handle
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_start_vid_enroll
 * @else
 * @brief  停止声纹注册
 * @param  [in]  sea SEA句柄
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_start_vid_enroll
 * @endif
 */
td_s32 uapi_sea_stop_vid_enroll(td_handle sea);

/**
 * @if Eng
 * @brief  Add entity to Voice ID.
 * @param  [in]  sea  SEA handle
 * @param  [in]  item Entity item
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_remove_vid_entity
 * @else
 * @brief  增加一个声纹实体
 * @param  [in]  sea  SEA句柄
 * @param  [in]  item 声纹实体
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_remove_vid_entity
 * @endif
 */
td_s32 uapi_sea_add_vid_entity(td_handle sea, const uapi_sea_item *item);

/**
 * @if Eng
 * @brief  Remove entity from Voice ID.
 * @param  [in]  sea  SEA handle
 * @param  [in]  item Entity item
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @see    #uapi_sea_add_vid_entity
 * @else
 * @brief  删除一个声纹实体
 * @param  [in]  sea  SEA句柄
 * @param  [in]  item 声纹实体
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @see    #uapi_sea_add_vid_entity
 * @endif
 */
td_s32 uapi_sea_remove_vid_entity(td_handle sea, const uapi_sea_item *item);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_SEA_H__ */
