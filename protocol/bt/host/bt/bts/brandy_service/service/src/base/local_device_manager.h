/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: Local Device Manager module.
 */
#ifndef __LOCAL_DEVICE_MANAGER_H__
#define __LOCAL_DEVICE_MANAGER_H__

#include <soc_osal.h>
#include <td_base.h>
#include <stdbool.h>
#include "bts_def.h"
#include "btsrv_task.h"
#include "mpc_send.h"
#include "mpc_dispatch.h"

extern osal_semaphore g_bcore_ready_sem;
/* 本地设备默认名称前缀 */
#define BTSRV_LOCAL_NAME_DEFAULT "WatchDemo"

#define BTSRV_FILE_DIR "/bin/vs/sd0p0/usr/data/bluetooth/"
/* 本地设备信息持久化存储文件路径 */
#define BTSRV_LOCAL_DEVICE_FILE "local_device.bin"
#define BTSRV_LOCAL_DEVICE_FILE_FULL_PATH (BTSRV_FILE_DIR BTSRV_LOCAL_DEVICE_FILE)
/* 远端设备信息持久化存储文件路径 */
#define BTSRV_REMOTE_DEVICE_FILE "remote_device.bin"
#define BTSRV_REMOTE_DEVICE_FILE_FULL_PATH (BTSRV_FILE_DIR BTSRV_REMOTE_DEVICE_FILE)

/* 本地设备运行模式 */
typedef enum {
    BTS_LOCAL_DEV_MODE_DEFAULT, /* 默认模式 */
    BTS_LOCAL_DEV_MODE_NOT_FILTER, /* 不过滤远端设备 */
    BTS_LOCAL_DEV_MODE_INVALID, /* 非法模式 */
} btsrv_local_device_mode;

/* 本地设备管理模块消息定义 */
enum {
    LOCAL_MGR_MSG_ID_DISABLE_BT,            /* 关闭经典蓝牙 */
    LOCAL_MGR_MSG_ID_CONN_MGR_DEINIT_OK,    /* 连接管理模块取消初始化完成 */
    LOCAL_MGR_MSG_ID_DISABLE_BLE,           /* 关闭低功耗蓝牙 */
    LOCAL_MGR_MSG_ID_ENABLE_BLE,            /* 使能低功耗蓝牙 */
    LOCAL_MGR_MSG_ID_HANDLE_BLE_ADV_DATA,   /* 处理广播数据 */
    LOCAL_MGR_MSG_ID_REPORT_START_BLE_ADV,  /* 上报BLE广播启动结果 */
    LOCAL_MGR_MSG_ID_REPORT_STOP_BLE_ADV,   /* 上报BLE广播停止结果 */
    LOCAL_MGR_MSG_ID_NOTIFY_ADV_DATA_SET,   /* 上报BLE广播数据设置结果 */
    LOCAL_MGR_MSG_ID_NOTIFY_ADV_PARAM_SET,  /* 上报BLE广播参数设置结果 */
    LOCAL_MGR_MSG_ID_NOTIFY_SCAN_PARAM_SET, /* 上报BLE扫描参数设置结果 */
    LOCAL_MGR_MSG_ID_CONNECT_STATE_CHANGE,  /* 上报BLE连接状态变更结果 */
    LOCAL_MGR_MSG_ID_PAIRED_RESULT,         /* 上报BLE配对完成结果 */
    LOCAL_MGR_MSG_ID_READ_REMOTE_RSSI_BLE,
    LOCAL_MGR_MSG_ID_PAIRED_KEYS,           /* 上报BLE配对完成密钥 */
    LOCAL_MGR_MSG_ID_REPORT_TERMINATE_ADV,  /* 上报广播终止结果 */
    LOCAL_MGR_MSG_ID_CONN_PARAM_UPDATE,     /* 连接参数更新 */
};

/* 发送本地设备管理模块消息 */
#define btsrv_send_local_mgr_msg(id, p0, p1, p2) \
    btsrv_send_simple_msg(BTSRV_MSG_ID_LOCAL_DEVICE_MANAGER, \
    (uintptr_t)(id), (uintptr_t)(p0), (uintptr_t)(p1), (uintptr_t)(p2))

bool btsrv_is_factory_reset(void);
bool btsrv_is_bluetooth_enable(void);
bool btsrv_is_bluetooth_disable(void);

bool btsrv_set_local_addr(const bd_addr_t *bd_addr);
bool is_bts_auto_reconnect_enable(void);
bool is_bts_a2dp_snk_enable(void);

/* 本地设备运行模式设置和查询 */
uint32_t btsrv_set_local_device_mode(btsrv_local_device_mode mode);
btsrv_local_device_mode btsrv_get_local_device_mode(void);

void ble_gap_manage_callback_regist(void);
void btsrv_connection_manager_deinit(void);
void btsrv_set_log_type_level(uint32_t log_type_level);
uint32_t btsrv_get_log_type_level(void);
uint8_t btsrv_get_gap_ble_state(void);
uint8_t btsrv_get_gap_bt_state(void);

#endif /* __LOCAL_DEVICE_MANAGER_H__ */
