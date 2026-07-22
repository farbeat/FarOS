/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: power manage common header file
 */

#ifndef LIB_PM_COMMON_H
#define LIB_PM_COMMON_H

#include "errcode.h"
#include "pm_definition.h"
#include "pm_core.h"
#include "pm_control.h"

/**
 * @defgroup middleware_utils_pm_api PM
 * @ingroup  middleware_utils
 * @{
 */

/**
 * @if Eng
 * @brief  Power Management component initialization.
 * @else
 * @brief  电源管理组件初始化
 * @endif
 */
void uapi_pm_init(void);

/**
 * @if Eng
 * @brief    Power Management control.
 * @param    type The PM management mode.
 * @else
 * @brief    电源管理控制
 * @param    type 电源管理类型
 * @endif
 */
void uapi_pm_control(pm_control_t type);

/**
 * @if Eng
 * @brief    Request enter pm mode.
 * @param    uid The ID register handler.
 * @param    mode The Mode type.
 * @return   errcode_t ERRER number.
 * @else
 * @brief    请求进入某种特定模式
 * @param    uid 模块 id
 * @param    mode 模式类型
 * @return   errcode_t 错误码
 * @endif
 */
errcode_t uapi_pm_request_mode(pm_id_t uid, pm_mode_t mode);

/**
 * @if Eng
 * @brief  Put the system into sleep mode.
 * @else
 * @brief  将系统置于睡眠模式
 * @endif
 */
void uapi_pm_enter_sleep(void);

/**
 * @if Eng
 * @brief  Init pm vote information.
 * @else
 * @brief  初始化投票数据结构
 * @endif
 */
void uapi_pm_veto_init(void);

/**
 * @if Eng
 * @brief    Registers the device management function for the PM.
 * @param    dev_id device ID.
 * @param    dev_ops Specifies the operation functions of the device ID.
 * @return   errcode_t ERRER number.
 * @else
 * @brief    为PM注册设备操作函数
 * @param    dev_id 模块 id
 * @param    dev_ops 指定设备ID的操作函数
 * @return   errcode_t 错误码
 * @endif
 */
errcode_t uapi_pm_register_dev_ops(pm_dev_id_t dev_id, pm_dev_ops_t *dev_ops);

/**
 * @if Eng
 * @brief    Add a callback to the handler array.
 * @param    id The ID register handler.
 * @param    state The state on callback handler.
 * @param    content Structure of the fsm handler.
 * @return   errcode_t ERRER number.
 * @else
 * @brief    注册PM状态机回调函数
 * @param    id 模块 id
 * @param    state 系统状态
 * @param    content 状态机处理函数及关键参数
 * @return   errcode_t 错误码
 * @endif
 */
errcode_t uapi_pm_register_fsm_handler(pm_id_t id, pm_states_t state, pm_fsm_content_t *content);

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/**
 * @if Eng
 * @brief    Add Deep Sleep Veto.
 * @param    veto_id The module ID of the veto.
 * @return   errcode_t ERRER number.
 * @else
 * @brief    添加深度睡眠否决票
 * @param    veto_id 模块 id
 * @return   errcode_t 错误码
 * @endif
 */
errcode_t uapi_pm_add_sleep_veto(pm_id_t veto_id);

/**
 * @if Eng
 * @brief    Add Deep Sleep Veto with timeout.
 * @param    veto_id The module ID of the veto.
 * @param    timeout_ms Timeout interval for veto.
 * @return   errcode_t ERRER number.
 * @else
 * @brief    添加带超时的深度睡眠否决票
 * @param    veto_id 模块 id
 * @param    timeout_ms 超时时间
 * @return   errcode_t 错误码
 * @endif
 */
errcode_t uapi_pm_add_sleep_veto_with_timeout(pm_id_t veto_id, uint32_t timeout_ms);

/**
 * @if Eng
 * @brief    Remove Deep Sleep Veto.
 * @param    veto_id The module ID of the veto.
 * @return   errcode_t ERRER number.
 * @else
 * @brief    移除深睡否决票
 * @param    veto_id 模块 id
 * @return   errcode_t 错误码
 * @endif
 */
errcode_t uapi_pm_remove_sleep_veto(pm_id_t veto_id);

/**
 * @if Eng
 * @brief The system enters the shipmode mode..
 * @param arg Reserved parameter.
 * @else
 * @brief 系统进shipmode
 * @param arg 保留参数.
 * @endif
 */
void uapi_sys_shipmode(uint32_t arg);
/**
 * @}
 */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif