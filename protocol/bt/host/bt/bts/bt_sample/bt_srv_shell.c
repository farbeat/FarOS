/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: HLD SDK BT Service shell module.
 */

#include "bt_srv_shell.h"
#include <stdlib.h>
#include <td_base.h>
#include "sample_a2dp_service.h"
#include "sample_common.h"
#include "sample_hfp_hf_service.h"
#include "sample_hfp_ag_service.h"
#include "sample_pbap_service.h"
#include "sample_spp_service.h"
#include "sample_bts_gap.h"
#include "sample_bts_gatt.h"
#include "sample_avrcp_service.h"
#include "sample_ble_dis_profile.h"
#include "sample_ble_bas.h"
#include "sample_ble_hid.h"
#if !defined(BRANDY_MCU_BT)
#include "sample_bt_audio_hal.h"
#endif
#include "sample_ble_fmp.h"
#include "sample_pan_service.h"
#include "sample_ble_ancs.h"
#include "sample_ble_ams.h"

#define BT_SRV_SAMPLE_CMD_ARGC_MIN 2

/* 测试命令分组 */
typedef enum {
    BTSRV_SAMPLE_CMD_GROUP_GAP = 0x01,    /* 1 gap 测试命令 */
#if (BTH_BREDR_ONLY == YES) || (BTH_BREDR_LE_DOUBLE == YES)
    BTSRV_SAMPLE_CMD_GROUP_A2DP_SERVICE,  /* 2 a2dp service 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_HFP_HF = 0x04, /* 4 HFP-HF 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_PBAP,          /* 5 PBAP 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_SPP,           /* 6 SPP 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_AVRCP_SERVICE, /* 7 AVRCP 测试命令 */
#endif
#if (BTH_LE_ONLY == YES) || (BTH_BREDR_LE_DOUBLE == YES)
    BTSRV_SAMPLE_CMD_GROUP_BLE_GAP = 8, /* 8 BLE GAP 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_BLE_GATT = 9, /* 9 BLE GATT 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_BLE_SMP = 10, /* 10 BLE SMP 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_DIS_SRV = 11, /* 11 DIS server 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_DIS_CLN = 12, /* 12 DIS client 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_GATT = 13,    /* 13 GATT 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_BAS_CLN = 14, /* 14 BAS client 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_BAS_SRV = 15, /* 15 BAS server 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_HID_SRV = 18,  /* 18 HID server 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_HID_CLN = 19,  /* 19 HID client 测试命令 */
#endif
#if (BTH_BREDR_ONLY == YES) || (BTH_BREDR_LE_DOUBLE == YES)
    BTSRV_SAMPLE_CMD_GROUP_AUDIO_HAL = 20, /* 20 AUDIO HAL 测试命令 */
#endif
#if (BTH_LE_ONLY == YES) || (BTH_BREDR_LE_DOUBLE == YES)
    BTSRV_SAMPLE_CMD_GROUP_FMP = 21,       /* 21 FMP 测试命令 */
#endif
#if (BTH_BREDR_ONLY == YES) || (BTH_BREDR_LE_DOUBLE == YES)
    BTSRV_SAMPLE_CMD_GROUP_PAN = 22, /* 22 PAN 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_HFP_AG = 23, /* 23 HFP-AG 测试命令 */
#endif

    BTSRV_SAMPLE_CMD_GROUP_ANCS_CLIENT = 24, /* 24 ANCS CLIENT 测试命令 */
    BTSRV_SAMPLE_CMD_GROUP_AMS_CLIENT = 25, /* 25  AMS CLIENT 测试命令 */
} btsrv_sample_cmd_group;
#if (BTH_LE_ONLY == YES) || (BTH_BREDR_LE_DOUBLE == YES)
static void ble_service_sample_cmd(int32_t module_id, int32_t cmd_id, char **params, int32_t params_num)
{
    if (params == NULL) {
        return;
    }

    switch (module_id) {
        case BTSRV_SAMPLE_CMD_GROUP_BLE_GAP:
            break;

        case BTSRV_SAMPLE_CMD_GROUP_BLE_GATT:
            break;

        case BTSRV_SAMPLE_CMD_GROUP_BLE_SMP:
            break;

        case BTSRV_SAMPLE_CMD_GROUP_DIS_SRV:
            ble_at_dis_srv_cmd(cmd_id, params, params_num);
            break;

        case BTSRV_SAMPLE_CMD_GROUP_DIS_CLN:
            ble_at_dis_cln_cmd(cmd_id, params, params_num);
            break;

        case BTSRV_SAMPLE_CMD_GROUP_GATT:
            btsrv_gatt_sample_cmd((uint32_t)cmd_id, params, params_num);
            break;
        case BTSRV_SAMPLE_CMD_GROUP_BAS_CLN:
            ble_at_bas_cln_cmd((uint8_t)cmd_id, params, (uint8_t)params_num);
            break;
        case BTSRV_SAMPLE_CMD_GROUP_BAS_SRV:
            ble_at_bas_srv_cmd((uint8_t)cmd_id, params, (uint8_t)params_num);
            break;
        case BTSRV_SAMPLE_CMD_GROUP_HID_SRV:
            ble_at_hid_srv_cmd((uint8_t)cmd_id, params, (uint8_t)params_num);
            break;
        case BTSRV_SAMPLE_CMD_GROUP_FMP:
            ble_at_fmp_srv_cmd(cmd_id, params, params_num);
            break;
        case BTSRV_SAMPLE_CMD_GROUP_ANCS_CLIENT:
            ble_ancs_client_at_cmd(cmd_id, params, params_num);
            break;
        case BTSRV_SAMPLE_CMD_GROUP_AMS_CLIENT:
            ble_ams_client_at_cmd(cmd_id, params, params_num);
            break;
        default:
            bts_sample_log("invalid cmd group id\n");
            break;
    }
}
#endif
/* 蓝牙服务测试命令总入口 */
int bt_service_sample_cmd(int argc, char *argv[])
{
    bts_sample_log("bt_service_sample_cmd: ");
    if (argc < BT_SRV_SAMPLE_CMD_ARGC_MIN || argv == NULL) {
        goto help;
    }
    for (int32_t i = 0; i < argc; i++) {
        bts_sample_log("%s ", argv[i]);
    }
    bts_sample_log("\n");

    int32_t module_id = atoi(argv[0]);
    int32_t cmd_id = atoi(argv[1]);
    const char **params = (const char **)&argv[BT_SRV_SAMPLE_CMD_ARGC_MIN];
    int32_t params_num = argc - BT_SRV_SAMPLE_CMD_ARGC_MIN;
    switch (module_id) {
        case BTSRV_SAMPLE_CMD_GROUP_GAP:
            btsrv_gap_sample_cmd((uint32_t)cmd_id, params, params_num);
            break;
#if (BTH_BREDR_ONLY == YES) || (BTH_BREDR_LE_DOUBLE == YES)
#ifndef MEMORY_MINI
        case BTSRV_SAMPLE_CMD_GROUP_A2DP_SERVICE:
            bt_a2dp_service_sample_cmd((uint32_t)cmd_id, params, params_num);
            break;
#endif
        case BTSRV_SAMPLE_CMD_GROUP_HFP_HF:
            bt_hfp_hf_service_sample_cmd((uint32_t)cmd_id, params, params_num);
            break;

        case BTSRV_SAMPLE_CMD_GROUP_PBAP:
            break;

        case BTSRV_SAMPLE_CMD_GROUP_SPP:
            break;
#ifndef MEMORY_MINI
        case BTSRV_SAMPLE_CMD_GROUP_AVRCP_SERVICE:
            bt_avrcp_service_sample_cmd((uint32_t)cmd_id, params, params_num);
            break;

        case BTSRV_SAMPLE_CMD_GROUP_AUDIO_HAL:
#if !defined(BRANDY_MCU_BT)
            bt_audio_hal_sample_cmd((uint32_t)cmd_id, params, params_num);
#endif
            break;
#endif
        case BTSRV_SAMPLE_CMD_GROUP_PAN:
            bt_pan_service_sample_cmd((uint32_t)cmd_id, params, params_num);
            break;
        case BTSRV_SAMPLE_CMD_GROUP_HFP_AG:
            bt_hfp_ag_service_sample_cmd((uint32_t)cmd_id, params, params_num);
            break;
#endif
        default:
#if (BTH_LE_ONLY == YES) || (BTH_BREDR_LE_DOUBLE == YES)
            ble_service_sample_cmd(module_id, cmd_id, &argv[BT_SRV_SAMPLE_CMD_ARGC_MIN], params_num);
#endif
            break;
    }

    return 0;

help:
    bts_sample_log("cmd format: btsrv group_id cmd_id (params...) \n");
    bts_sample_log("group id: 1--gap cmd; 2--a2dp cmd; 4--hfp-hf cmd; 5--pbap cmd; 7--avrcp \n");
    return -1;
}
