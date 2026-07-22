/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides at cmd register func \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-20， Create file. \n
 */
#ifndef BT_MANAGER_CMD_MANAGER_H
#define BT_MANAGER_CMD_MANAGER_H

#include <stdint.h>
#include <stdio.h>
#include "errcode.h"
#include "at_cmd_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#define MAX_AT_CMD_NAME_LEN         32
/*AT命令多参数命令序列*/
#define AT_TMODE_3 "36 5A 00 03 00 13 19 B8 72"

typedef enum {
    PARAM_TYPE_NULL,
    PARAM_TYPE_ADDR,
    PARAM_TYPE_SET_ADDR,
    PARAM_TYPE_CALL_NUM,
    PARAM_TYPE_MUSIC,
    PARAM_TYPE_MUSIC_PLAY,
    PARAM_TYPE_DISC_PHONE,
    PARAM_TYPE_HIGH_POWER,
    PARAM_TYPE_SET_VALUE,
} ParamTypeIdE;

at_cmd_table_t *get_bt_manager_cmd_table(uint32_t *cmd_count);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif