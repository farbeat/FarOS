/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: Remote Device Manager Module.
 */
#ifndef __REMOTE_DEVICE_MANAGER_H__
#define __REMOTE_DEVICE_MANAGER_H__

#include <td_base.h>
#include <osal_list.h>
#include "bts_br_gap.h"
#include "mpc_dispatch.h"
#include "stack_gap_if.h"
#include "stack_def.h"

#define DEVICE_MAJOR_CLASS_AUDIO_VIDEO 0x400 /* 音频设备COD：Major Device Class = Audio/Video */
#define DEVICE_MAJOR_CLASS_PHONE 0x200 /* 手机设备COD：Major Device Class = phone */
#define DEVICE_LINKKEY_DISPLAY_LEN 64

#define is_audio_video_device(cod) (((uint32_t)(cod) & DEVICE_MAJOR_CLASS_AUDIO_VIDEO) == \
    DEVICE_MAJOR_CLASS_AUDIO_VIDEO)
#define is_phone_device(cod) (((uint32_t)(cod) & DEVICE_MAJOR_CLASS_PHONE) == DEVICE_MAJOR_CLASS_PHONE)

/* 远端设备状态 */
typedef enum {
    REMOTE_DEV_STATUS_DEFAULT,
    REMOTE_DEV_STATUS_FOUND,
    REMOTE_DEV_STATUS_PAIRED
} remote_device_status;

/* 远端设备信息中，需要持久化部分信息 */
typedef struct {
    uint8_t bd_addr[BD_ADDR_LEN];
    uint8_t have_key; /* 是否保存了link_key */
    uint8_t key_type;
    uint8_t name[BD_NAME_LEN];
    int32_t class_of_device;
    uint8_t link_key[BTSDK_LINKKEY_LEN];
} remote_device_info;

/* 远端设备链表节点 */
typedef struct remote_device_list_node {
    struct osal_list_head node;

    remote_device_info info;    /* 设备信息 */
    uint32_t dev_hdl;           /* BTSDK上报的设备HANDLE */
    uint8_t status;               /* 设备状态，定义see remote_device_status */
    int8_t rssi;                 /* 设备RSSI */
    uint16_t pad;
} remote_device_list_node;

/* 远端设备列表互斥锁，使用时特别注意不要嵌套调用 */
void btsrv_access_remote_dev_list(void);
void btsrv_release_remote_dev_list(void);

void btsrv_remote_device_init(void);
void btsrv_remote_device_deinit(void);
remote_device_list_node *btsrv_get_remote_paired_phone_node(void);
remote_device_list_node *btsrv_find_remote_dev_node(const uint8_t *bd_addr, uint32_t dev_hdl, bool add);
uint32_t btsrv_get_remote_dev_addr_by_hdl(uint32_t dev_hdl, bd_addr_t *addr);

void btsrv_set_remote_dev_status_found_2_none(void);
void btsrv_handle_remote_device_found(uint32_t dev_hdl, bt_sdk_dev_found_info *info);
void btsrv_delete_remote_dev_node(remote_device_list_node *node);
void btsrv_update_remote_dev_info(const bd_addr_t *addr, uint32_t dev_hdl, int32_t cod);
void btsrv_update_remote_device_name(uint32_t dev_hdl);
void btsrv_update_remote_device_name_cfm(const uint8_t *addr, const char *name, uint16_t name_len);

bool btsrv_get_pair_state(uint32_t dev_hdl);

/* 维测：打印远端设备信息链表 */
void btsrv_dfx_show_remote_dev_list(void);

/* 配置BLE安全连接标记 */
void btsrv_set_ble_sc_flag(bool is_enable);
/* 获取BLE安全连接标记 */
bool btsrv_get_ble_sc_flag(void);
/* 配置BLE一键双连标记 */
void btsrv_set_ble_ctkd_flag(bool is_enable);
/* 获取BLE一键双连标记 */
bool btsrv_get_ble_ctkd_flag(void);
 
#endif /* __REMOTE_DEVICE_MANAGER_H__ */
