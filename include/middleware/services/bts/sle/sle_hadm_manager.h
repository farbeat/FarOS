/**
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 *
 * Description: SLE Hadm Manager module.
 */

/**
 * @defgroup sle_hadm_manager hadm manager API
 * @ingroup  SLE
 * @{
 */

#ifndef SLE_HADM_MANAGER_H
#define SLE_HADM_MANAGER_H

#include <stdint.h>
#include "errcode.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @if Eng
 * @brief  size of channel sounding capability set。
 * @else
 * @brief  channel sounding 能力集大小。
 * @endif
 */
#define SLR_CS_CAPS_LEN 0x15

/**
 * @if Eng
 * @brief sle channel sounding param.
 * @else
 * @brief 设置channel sounding 参数
 * @endif
 */
typedef struct {
    uint8_t  config_id;             /*!< @if Eng Position measurement signal configuration index
                                         @else   位置测量信号配置索引 @endif */
    uint8_t  time_slot;             /*!< @if Eng System scheduling timeslot
                                         @else   系统调度时隙 @endif */
    uint8_t  rtt_phy : 3;           /*!< @if Eng Measurement signal bandwidth indicator
                                         @else   测量信号带宽指示 @endif */
    uint8_t  freq_hopping_mode : 2; /*!< @if Eng Frequency Hopping Mode Indication
                                         @else   测量信号带宽指示 @endif */
    uint8_t  resv : 1;              /*!< @if Eng reserv
                                         @else   预留字节对齐 @endif */
    uint16_t freq : 10;             /*!< @if Eng Frequency in the initialization phase
                                         @else   初始化阶段的频点 @endif */
    uint16_t interaction_type : 6;  /*!< @if Eng Interaction type indicator in the initialization phase
                                         @else   初始化阶段交互类型指示 @endif */
    uint16_t group_period;          /*!< @if Eng Event Group Period
                                         @else   事件组周期 @endif */
    uint16_t group_interval;        /*!< @if Eng Inter-event interval in initialization phase
                                         @else   初始化阶段事件间间隔 @endif */
    uint16_t measure_interval1;     /*!< @if Eng Measurement Frame Type 1 Inter-Event Interval
                                         @else   测量帧类型1 事件间间隔 @endif */
    uint16_t measure_interval2;     /*!< @if Eng Measurement Frame Type 2 Inter-Event Interval
                                         @else   测量帧类型2 事件间间隔 @endif */
    uint16_t intra_event_interval1; /*!< @if Eng Intra-event interval in initialization phase
                                         @else   初始化阶段事件内间隔 @endif */
    uint16_t intra_event_interval2; /*!< @if Eng Intra-event interval
                                         @else   事件内间隔 @endif */
    uint8_t  t_guard;               /*!< @if Eng Internal switching interval of measurement frames in the
                                                 initialization phase
                                         @else   初始化阶段的测量帧的内部切换间隔 @endif */
    uint8_t  signal2_length;        /*!< @if Eng Length of the measurement signal 2 in the initialization phase
                                         @else   初始化阶段的测量信号2的长度 @endif */
    uint8_t  init_ant_count : 4;    /*!< @if Eng Indicates the number of antennas of the first-transmission node.
                                         @else   先发节点天线数量指示 @endif */
    uint8_t  init_signal2_tone : 4; /*!< @if Eng Multi-tone indication of first-transmit link measurement signal 2.
                                         @else   先发链路测量信号2 多音指示 @endif */
    uint8_t  refl_ant_count : 4;    /*!< @if Eng Indicates the number of antennas of the post-transmitting node.
                                         @else   后发节点天线数量指示 @endif */
    uint8_t  refl_signal2_tone : 4; /*!< @if Eng Multi-tone indication of post-transmit link measurement signal 2.
                                         @else   后发链路测量信号2 多音指示 @endif */
    uint8_t  freq_hopping_band;     /*!< @if Eng Hopping frequency band.
                                         @else   后跳频频带 @endif */
    uint8_t  band_2400m[10];        /*!< @if Eng 2.4 GHz FH Channel Indication.
                                         @else   2.4GHz 跳频信道指示 @endif */
    uint8_t  glp_mode;              /*!< @if Eng glp mode.
                                         @else   2.4GHz 模式 @endif */
    uint8_t  ble_hadm_mode;         /*!< @if Eng ble hadm mode.
                                         @else   ble hadm 模式 @endif */
} sle_set_channel_sounding_param_t;

/**
 * @if Eng
 * @brief sle channel sounding capability set.
 * @else
 * @brief channel sounding 能力集
 * @endif
 */
typedef struct {
    uint8_t caps[SLR_CS_CAPS_LEN]; /*!< @if Eng sle channel sounding capability set.
                                        @else   channel sounding 能力集 @endif */
} sle_channel_sounding_caps_t;

/**
 * @if Eng
 * @brief Callback invoked when read local channnel sounding.
 * @par Callback invoked when read local channnel sounding.
 * @attention 1.This function is called in SLE service context,should not be blocked or do long time waiting.
 * @attention 2.The memories of pointer are requested and freed by the SLE service automatically.
 * @param [in] conn_id discovery ID.
 * @param [in] caps    channel sounding capability set.
 * @param [in] status  error code.
 * @retval #void no return value.
 * @par Dependency:
 * @li  sle_common.h
 * @see sle_connection_callbacks_t
 * @else
 * @brief  读取本端channnen sounding的回调函数。
 * @par    读取本端channnen sounding的回调函数。
 * @attention  1. 该回调函数运行于SLE service线程，不能阻塞或长时间等待。
 * @attention  2. 指针由SLE service申请内存，也由SLE service释放，回调中不应释放。
 * @param [in] conn_id 公开ID。
 * @param [in] caps    channel sounding能力集。
 * @param [in] status  执行结果错误码。
 * @retval 无返回值。
 * @par 依赖:
 * @li  sle_common.h
 * @see sle_connection_callbacks_t
 * @endif
 */
typedef void (*sle_read_local_channel_sounding_callback)(uint16_t conn_id, sle_channel_sounding_caps_t *caps,
    errcode_t status);

/**
 * @if Eng
 * @brief Callback invoked when read remote channnel sounding.
 * @par Callback invoked when read remote channnel sounding.
 * @attention 1.This function is called in SLE service context,should not be blocked or do long time waiting.
 * @attention 2.The memories of pointer are requested and freed by the SLE service automatically.
 * @param [in] conn_id discovery ID.
 * @param [in] caps    channel sounding capability set.
 * @param [in] status  error code.
 * @retval #void no return value.
 * @par Dependency:
 * @li  sle_common.h
 * @see sle_connection_callbacks_t
 * @else
 * @brief  读取对端channnen sounding的回调函数。
 * @par    读取对端channnen sounding的回调函数。
 * @attention  1. 该回调函数运行于SLE service线程，不能阻塞或长时间等待。
 * @attention  2. 指针由SLE service申请内存，也由SLE service释放，回调中不应释放。
 * @param [in] conn_id 公开ID。
 * @param [in] caps    channel sounding能力集。
 * @param [in] status  执行结果错误码。
 * @retval 无返回值。
 * @par 依赖:
 * @li  sle_common.h
 * @see sle_connection_callbacks_t
 * @endif
 */
typedef void (*sle_read_remote_channel_sounding_callback)(uint16_t conn_id, sle_channel_sounding_caps_t *caps,
    errcode_t status);

/**
 * @if Eng
 * @brief Callback invoked when set channnel sounding parameter.
 * @par Callback invoked when channnel sounding parameter.
 * @attention 1.This function is called in SLE service context,should not be blocked or do long time waiting.
 * @attention 2.The memories of pointer are requested and freed by the SLE service automatically.
 * @param [in] conn_id discovery ID.
 * @param [in] status  error code.
 * @retval #void no return value.
 * @par Dependency:
 * @li  sle_common.h
 * @see sle_connection_callbacks_t
 * @else
 * @brief  channnen sounding参数设置的回调函数。
 * @par    channnen sounding参数设置的回调函数。
 * @attention  1. 该回调函数运行于SLE service线程，不能阻塞或长时间等待。
 * @attention  2. 指针由SLE service申请内存，也由SLE service释放，回调中不应释放。
 * @param [in] conn_id 公开ID。
 * @param [in] status  执行结果错误码。
 * @retval 无返回值。
 * @par 依赖:
 * @li  sle_common.h
 * @see sle_connection_callbacks_t
 * @endif
 */
typedef void (*sle_set_channel_sounding_param_callback)(uint16_t conn_id, errcode_t status);

/**
 * @if Eng
 * @brief Callback invoked when channnel sounding state changed.
 * @par Callback invoked when channnel sounding state changed.
 * @attention 1.This function is called in SLE service context,should not be blocked or do long time waiting.
 * @attention 2.The memories of pointer are requested and freed by the SLE service automatically.
 * @param [in] status error code.
 * @retval #void no return value.
 * @par Dependency:
 * @li  sle_common.h
 * @see sle_connection_callbacks_t
 * @else
 * @brief  channnen sounding状态变化的回调函数。
 * @par    channnen sounding状态变化的回调函数。
 * @attention  1. 该回调函数运行于SLE service线程，不能阻塞或长时间等待。
 * @attention  2. 指针由SLE service申请内存，也由SLE service释放，回调中不应释放。
 * @param [in] status 执行结果错误码。
 * @retval 无返回值。
 * @par 依赖:
 * @li  sle_common.h
 * @see sle_connection_callbacks_t
 * @endif
 */
typedef void (*sle_channel_sounding_state_changed_callback)(errcode_t status);

/**
 * @if Eng
 * @brief Struct of SLE hadm manager callback function.
 * @else
 * @brief SLE hadm 管理回调函数接口定义。
 * @endif
 */
typedef struct {
    sle_read_local_channel_sounding_callback read_local_cs_caps_cb;   /*!< @if Eng read local channel sounding
                                                                                   callback.
                                                                           @else   读取本端channel sounding回调函数。
                                                                           @endif */
    sle_read_remote_channel_sounding_callback read_remote_cs_caps_cb; /*!< @if Eng read remote channel sounding
                                                                                   callback.
                                                                           @else   读取对端channel sounding回调函数。
                                                                           @endif */
    sle_set_channel_sounding_param_callback set_cs_param_cb;          /*!< @if Eng channel sounding parameter set
                                                                                   callback.
                                                                           @else   channel sounding参数设置回调函数。
                                                                           @endif */
    sle_channel_sounding_state_changed_callback cs_state_changed_cb;  /*!< @if Eng channel sounding state changed
                                                                                   callback.
                                                                           @else   channel sounding状态改变回调函数。
                                                                           @endif */
} sle_hadm_callbacks_t;

/**
 * @if Eng
 * @brief  Read local channel sounding capability.
 * @par Description: Read local channel sounding capability.
 * @param NULL
 * @retval error code, read result will be returned at { @ref sle_read_local_channel_sounding_callback }.
 * @par Depends:
 * @li sle_common.h
 * @else
 * @brief  读取本端channel sounding能力。
 * @par Description: 读取本端channel sounding能力。
 * @param NULL
 * @retval 执行结果错误码，读取结果将在{ @ref sle_read_local_channel_sounding_callback }中返回。
 * @par 依赖：
 * @li sle_common.h
 * @endif
 */
errcode_t sle_read_local_channel_sounding_caps(void);

/**
 * @if Eng
 * @brief  Read remote channel sounding capability.
 * @par Description: Read remote channel sounding capability.
 * @param [in] conn_id connection ID.
 * @retval error code, read result will be returned at { @ref sle_read_remote_channel_sounding_callback }.
 * @par Depends:
 * @li sle_common.h
 * @else
 * @brief  读取对端channel sounding能力。
 * @par Description: 读取对端channel sounding能力。
 * @param [in] conn_id 连接 ID
 * @retval 执行结果错误码，读取结果将在{ @ref sle_read_remote_channel_sounding_callback }中返回。
 * @par 依赖：
 * @li sle_common.h
 * @endif
 */
errcode_t sle_read_remote_channel_sounding_caps(uint16_t conn_id);

/**
 * @if Eng
 * @brief  Set channel sounding parameter.
 * @par Description: Set channel sounding parameter.
 * @param [in] conn_id connection ID.
 * @param [in] param   parameter.
 * @retval error code, read result will be returned at { @ref sle_set_channel_sounding_param_callback }.
 * @par Depends:
 * @li sle_common.h
 * @else
 * @brief  设置channel sounding参数。
 * @par Description: 设置channel sounding参数。
 * @param [in] conn_id 连接 ID。
 * @param [in] param   参数。
 * @retval 执行结果错误码，读取结果将在{ @ref sle_set_channel_sounding_param_callback }中返回。
 * @par 依赖：
 * @li sle_common.h
 * @endif
 */
errcode_t sle_set_channel_sounding_param(uint16_t conn_id, sle_set_channel_sounding_param_t *param);

/**
 * @if Eng
 * @brief  Set channel sounding enable.
 * @par Description: Set channel sounding enable.
 * @param NULL
 * @retval error code, read result will be returned at { @ref sle_channel_sounding_state_changed_callback }.
 * @par Depends:
 * @li sle_common.h
 * @else
 * @brief  设置channel sounding使能。
 * @par Description: 设置channel sounding使能。
 * @param NULL
 * @retval 执行结果错误码，读取结果将在{ @ref sle_channel_sounding_state_changed_callback }中返回。
 * @par 依赖：
 * @li sle_common.h
 * @endif
 */
errcode_t sle_set_channel_sounding_enable(void);

/**
 * @if Eng
 * @brief  Set channel sounding disable.
 * @par Description: Set channel sounding disable.
 * @param NULL
 * @retval error code, read result will be returned at { @ref sle_channel_sounding_state_changed_callback }.
 * @par Depends:
 * @li sle_common.h
 * @else
 * @brief  设置channel sounding关闭。
 * @par Description: 设置channel sounding关闭。
 * @param NULL
 * @retval 执行结果错误码，读取结果将在{ @ref sle_channel_sounding_state_changed_callback }中返回。
 * @par 依赖：
 * @li sle_common.h
 * @endif
 */
errcode_t sle_set_channel_sounding_disable(void);

/**
 * @if Eng
 * @brief  Register SLE hadm manager callbacks.
 * @par Description: Register SLE hadm manager callbacks.
 * @param [in] func Callback function.
 * @retval error code.
 * @else
 * @brief  注册 SLE hadm 管理回调函数。
 * @par Description: 注册 SLE hadm 管理回调函数。
 * @param [in] func 回调函数。
 * @retval 执行结果错误码。
 * @endif
 */
errcode_t sle_hadm_register_callbacks(sle_hadm_callbacks_t *func);


#ifdef __cplusplus
}
#endif
#endif /* SLE_HADM_MANAGER */
/**
 * @}
 */
