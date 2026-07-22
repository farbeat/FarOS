/*
 * Copyright (c) @CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: user api for active noise canceling module
 * Author: @CompanyNameTag
 */

#ifndef __SOC_UAPI_ANC_H__
#define __SOC_UAPI_ANC_H__

#include "td_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @defgroup soc_uapi_anc soc_uapi_anc
 * @ingroup  audio
 * @{
 */

#define UAPI_ANC_REG_SIZE 1024

/**
 * @if Eng
 * @brief  AHA mode.
 * @else
 * @brief  AHA模式
 * @endif
 */
typedef enum {
    UAPI_AHA_MODE_ANC_OFF = 0,
    UAPI_AHA_MODE_ANC_ON,
    UAPI_AHA_MODE_AH,
    UAPI_AHA_MODE_HT,

    UAPI_AHA_MODE_MAX,
} uapi_aha_mode;

/**
 * @if Eng
 * @brief  ANC register.
 * @else
 * @brief  ANC寄存器
 * @endif
 */
typedef struct {
    td_u32 offset;
    td_u32 value;
} uapi_anc_reg;

/**
 * @if Eng
 * @brief  ANC config.
 * @else
 * @brief  ANC配置
 * @endif
 */
typedef struct {
    uapi_aha_mode aha_mode;
    uapi_anc_reg anc_reg[UAPI_ANC_REG_SIZE];
} uapi_anc_config;

/**
 * @if Eng
 * @brief  ANC module init.
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  ANC模块初始化
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_anc_init(td_void);

/**
 * @if Eng
 * @brief  ANC module deinit.
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  ANC模块去初始化
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_anc_deinit(td_void);

/**
 * @if Eng
 * @brief  Set the config of ANC instance.
 * @param  [in]  attr ANC config
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  设置ANC实例配置
 * @param  [in]  attr ANC配置
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_anc_set_config(uapi_aha_mode mode, const td_s8 *config, td_u32 config_len);

/**
 * @if Eng
 * @brief  Get the config of ANC instance.
 * @param  [out] attr ANC config
 * @retval SUCCESS Success
 * @retval FAILURE Failed
 * @else
 * @brief  获取ANC实例配置
 * @param  [out] attr ANC配置
 * @retval SUCCESS 成功
 * @retval FAILURE 失败
 * @endif
 */
td_s32 uapi_anc_get_config(uapi_aha_mode *mode, td_s8 *config, td_u32 *config_len);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_ANC_H__ */
