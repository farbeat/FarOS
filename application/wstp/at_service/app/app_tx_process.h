/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides at cmd register func \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-20， Create file. \n
 */
#ifndef APP_TX_PROCESS_H
#define APP_TX_PROCESS_H

#include <stdint.h>
#include <stdio.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


void app_at_msg_send(uint8_t *data, uint32_t data_len);
void app_pkg_msg_send(uint8_t msg_id, uint8_t control, uint8_t *data, uint32_t data_len);
void app_pkg_diag_msg_send(uint8_t *data, uint32_t data_len);
uint32_t send_diag_msg_spp(uint8_t* msg, uint8_t cid, uint8_t* payload, uint32_t pay_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif