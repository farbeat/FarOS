/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  reboot.h
 * Author: @CompanyNameTag
 * Create:
 */

#ifndef DFX_REBOOT_H
#define DFX_REBOOT_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    SYSTEM_SOFT_REBOOT,
    SYSTEM_HARD_REBOOT,
} reboot_type_t;
/**
 * @brief 复位前处理回调函数
 */
typedef void (*process_callback_func)(void);

/**
 * @if Eng
 * @brief Processing Before Reboot.
 * @param arg callback func.
 * @else
 * @brief 复位前处理流程
 * @param arg 回调函数
 * @endif
 */
void uapi_register_pre_reboot_process_cb(process_callback_func callback);
/**
 * @if Eng
 * @brief Processing Before Shutdown.
 * @param arg callback func.
 * @else
 * @brief 关机前处理流程
 * @param arg 回调函数
 * @endif
 */
void uapi_register_pre_shutdown_process_cb(process_callback_func callback);

/**
 * @if Eng
 * @brief The system reboot.
 * @param arg reboot tupe.
 * @else
 * @brief 系统复位
 * @param arg 复位类型
 * @endif
 */
void uapi_system_reboot(reboot_type_t type);

/**
 * @if Eng
 * @brief The system shutdown.
 * @param arg Reserved parameter.
 * @else
 * @brief 系统关机
 * @param arg 保留参数
 * @endif
 */
void uapi_system_shutdown(uint32_t arg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
