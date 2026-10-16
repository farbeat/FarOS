/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: Header file for FarOS Health Service AT commands. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#ifndef APP_HEALTH_AT_PROCESS_H
#define APP_HEALTH_AT_PROCESS_H

#include "stdint.h"
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @if Eng
 * @brief  Initialize health service AT command processing.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  初始化运动健康AT命令处理。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t app_health_at_init(void);

/**
 * @if Eng
 * @brief  Process health service AT commands.
 * @param  [in] msg_id    Message ID.
 * @param  [in] payload   Command payload.
 * @param  [in] len       Payload length.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  处理运动健康AT命令。
 * @param  [in] msg_id    消息ID。
 * @param  [in] payload   命令负载。
 * @param  [in] len       负载长度。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t app_health_at_process(uint8_t msg_id, const uint8_t *payload, uint32_t len);

/**
 * @if Eng
 * @brief  Get health AT command table and count.
 * @param  [out] cmd_count Command count.
 * @return Command table pointer.
 * @else
 * @brief  获取健康AT命令表和数量。
 * @param  [out] cmd_count 命令数量。
 * @return 命令表指针。
 * @endif
 */
void *app_health_get_cmd_table(uint32_t *cmd_count);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* APP_HEALTH_AT_PROCESS_H */
