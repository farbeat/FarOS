/**
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved. \n
 * Description: common UNF header file for audio \n
 * Author: @CompanyNameTag \n
 */

 /** @defgroup audio sys */

#ifndef __SOC_UAPI_AUDIO_SYS_H__
#define __SOC_UAPI_AUDIO_SYS_H__

#include "soc_uapi_audio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @defgroup soc_uapi_audio_sys soc_uapi_audio_sys
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  Init audio system.
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  初始化音频系统
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_audio_init(td_void);

/**
 * @if Eng
 * @brief  Deinit audio system.
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  去初始化音频系统
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_audio_deinit(td_void);

/**
 * @if Eng
 * @brief  Set audio product form.
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置音频产品形态
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_audio_set_product_form(uapi_audio_product_form product_form);

/**
 * @if Eng
 * @brief  Set configuration related to debugging.
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置调试相关的配置
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_audio_set_debug_cfg(const uapi_audio_debug_cfg *cfg);

/**
 * @if Eng
 * @brief  Get configuration related to debugging.
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  获取调试相关的配置
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_audio_get_debug_cfg(uapi_audio_debug_cfg *cfg);

/**
 * @if Eng
 * @brief  Set audio tws(True Wireless Stereo) mono mode.
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置真无线耳机单耳模式
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_audio_set_tws_mono_mode(td_bool mono);

/**
 * @if Eng
 * @brief  Set audio tws(True Wireless Stereo) mode.
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置真无线耳机工作模式
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_audio_set_tws_mode(uapi_audio_tws_mode tws_mode);

/**
 * @if Eng
 * @brief  Set audio tws(True Wireless Stereo) role.
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置真无线耳机角色
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_audio_set_tws_role(uapi_audio_tws_role tws_role);

/**
 * @if Eng
 * @brief  Set DSP log level.
 * @retval SUCCESS Success.
 * @retval FAILURE Failure.
 * @else
 * @brief  设置DSP打印等级
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_audio_set_dsp_log_level(td_u32 log_level);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_AUDIO_SYS_H__ */
