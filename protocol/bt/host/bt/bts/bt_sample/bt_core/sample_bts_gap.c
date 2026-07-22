/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test function of GAP module.
 */

#include "sample_bts_gap.h"
#include <osal_list.h>
#include <soc_osal.h>
#include <stdlib.h>
#include "securec.h"
#include "soc_errno.h"
#include "bts_br_gap.h"
#include "bts_le_gap.h"
#include "bts_feature.h"
#include "bts_a2dp_source.h"
#include "bts_br_gap.h"
#include "bts_avrcp_controller.h"
#include "bts_spp.h"
#include "sample_common.h"
#include "debug_print.h"
#include "ble_ams_client.h"
#include "ble_ancs_client.h"
#include "ble_fmp_locator.h"
#include "ble_bas_client.h"
#include "ble_pro_dis_client.h"

#define BTSRV_LOG_UART_INFO          0xFF2
/* adapter service test cmd id */
typedef enum {
    BTSRV_SAMPLE_GAP_ENABLE_BLE,      /* 0 打开BLE */
    BTSRV_SAMPLE_GAP_DISABLE_BLE,     /* 1 关闭BLE */
    BTSRV_SAMPLE_GAP_ENABLE_BT,       /* 2 打开BT */
    BTSRV_SAMPLE_GAP_DISABLE_BT,      /* 3 关闭BT */
    BTSRV_SAMPLE_GAP_GET_LOCAL_ADDR,  /* 4 获取本地设备地址 */

    BTSRV_SAMPLE_GAP_GET_LOCAL_NAME,  /* 5 获取本地设备名称 */
    BTSRV_SAMPLE_GAP_SET_LOCAL_NAME,  /* 6 设置本地设备名称 */
    BTSRV_SAMPLE_GAP_FACTORY_RESET,   /* 7 恢复出厂设置 */
    BTSRV_SAMPLE_GAP_GET_BT_SCAN_MODE, /* 8 获取扫描模式 */
    BTSRV_SAMPLE_GAP_SET_BT_SCAN_MODE, /* 9 设置扫描模式 */

    BTSRV_SAMPLE_GAP_GET_PAIRED_DEVICE_NUM,   /* 10 获取已配对设备数量 */
    BTSRV_SAMPLE_GAP_GET_PAIRED_DEVICE_LIST,  /* 11 获取已配对设备列表 */
    BTSRV_SAMPLE_GAP_GET_PAIR_STATE,          /* 12 获取指定设备配对状态 */
    BTSRV_SAMPLE_GAP_REMOVE_PAIR,             /* 13 删除配对 */
    BTSRV_SAMPLE_GAP_REMOVE_ALL_PAIRS,        /* 14 删除所有配对 */

    BTSRV_SAMPLE_GAP_IS_ACL_CONNECTED,        /* 15 检查ACL是否连接 */
    BTSRV_SAMPLE_GAP_DISCONNECT_REMOTE_DEVICE, /* 16 断开设备连接 */
    BTSRV_SAMPLE_GAP_CONNECT_REMOTE_DEVICE,   /* 17 连接设备 */
    BTSRV_SAMPLE_GAP_REGISTER_CALLBACKS,      /* 18 注册GAP回调 */
    BTSRV_SAMPLE_GAP_UPDATE_CONNECT_PARAM,    /* 19 BLE 连接参数更新 */

    BTSRV_SAMPLE_GAP_STOP_BT_DISCOVERY,       /* 20 停止BT发现流程 */
    BTSRV_SAMPLE_GAP_IS_BT_DISCOVERY,         /* 21 是否正在BT发现流程中 */
    BTSRV_SAMPLE_GAP_READ_REMOTE_RSSI_VALUE,  /* 22 读取远端设备RSSI值 */
    BTSRV_SAMPLE_GAP_GET_DEVICE_NAME,         /* 23 获取远端设备名称 */
    BTSRV_SAMPLE_GAP_GET_DEVICE_CLASS,        /* 24 获取远端设备class */

    BTSRV_SAMPLE_GAP_PAIR_REQUEST_RELY,       /* 25 配对请求回复 */
    BTSRV_SAMPLE_GAP_SET_DEVICE_PIN,          /* 26 设置设备PIN-CODE */
    BTSRV_SAMPLE_GAP_SET_DEVICE_PAIRING_CONFIRMATION, /* 27 设置设备配对确认 */
    BTSRV_SAMPLE_GAP_SET_DEVICE_PASSKEY,      /* 28 设置设备passkey */
    BTSRV_SAMPLE_GAP_UPDATE_FOUND_LIST,       /* 29 刷新发现设备列表 */

    BTSRV_SAMPLE_GAP_SHOW_FOUND_LIST,         /* 30 展示发现设备列表 */
    BTSRV_SAMPLE_GAP_SET_CONN_REQ_ANSWER,     /* 31 设置连接请求应答 */
    BTSRV_SAMPLE_GAP_SET_FEATURE_CONFIG,      /* 32 蓝牙特性配置 */
    BTSRV_SAMPLE_GAP_GET_FEATURE_CONFIG,      /* 33 蓝牙特性获取 */
    BTSRV_SAMPLE_GAP_RECONNECT_PHONE,         /* 34 重连手机 */

    BTSRV_SAMPLE_GAP_SET_INQUIRY_PARAM,         /* 35 设置inquiry参数 */
    BTSRV_SAMPLE_GAP_SET_SERVICE_TASK_PRIORITY, /* 36 设置BT_SERVICE_TASK优先级 */
    BTSRV_SAMPLE_GAP_SET_FLOW_LIMIT,            /* 37 设置流控限制字节数 */
    BTSRV_SAMPLE_GAP_SET_SPP_WRITE_RETRY_MAX,   /* 38 设置spp写入重试最大次数 */
} btsrv_gap_sample_cmd_id;

typedef enum {
    ACL_EXT_MODE_DEFAULT,       /* 默认值，无任何连带操作 */
    ACL_EXT_MODE_CONNECT_A2DP,  /* ACL连接建立后，连带建立A2DP连接 */
} acl_conn_ext_mode;

/* 发现设备链表节点 */
typedef struct {
    struct osal_list_head node;

    bd_addr_t bd_addr;
    uint8_t name[BD_NAME_LEN];
    int32_t cod;
    int32_t rssi;
} sample_found_dev_list_node;

#define is_limied_discoverable_mode(cod)    (((cod) & 0x2000) != 0)
bool g_sample_found_list_init = false;
OSAL_LIST_HEAD(g_sample_found_dev_list_head);

/* 重置发现设备链表 */
void reset_found_dev_list(void)
{
    if (!g_sample_found_list_init) {
        g_sample_found_list_init = true;
        OSAL_INIT_LIST_HEAD(&g_sample_found_dev_list_head);
        return;
    }

    while (osal_list_empty(&g_sample_found_dev_list_head) == false) {
        struct osal_list_head *node = g_sample_found_dev_list_head.next;
        osal_list_del(node);
        bts_sample_mem_free(node);
    }
}

/* 新增节点 */
static void add_node_to_found_dev_list(const bd_addr_t *bd_addr, const uint8_t *name, int32_t cod, int32_t rssi)
{
    sample_found_dev_list_node *node =
        (sample_found_dev_list_node *)(void *)bts_sample_mem_new(sizeof(sample_found_dev_list_node));
    if (node == NULL) {
        return;
    }
    (void)memset_s(node, sizeof(sample_found_dev_list_node), 0, sizeof(sample_found_dev_list_node));

    if (memcpy_s(&node->bd_addr, sizeof(node->bd_addr), bd_addr, sizeof(bd_addr_t)) != EOK) {
        bts_sample_mem_free(node);
        return;
    }

    if (memcpy_s(node->name, sizeof(node->name), name, BD_NAME_LEN) != EOK) {
        bts_sample_mem_free(node);
        return;
    }

    node->cod = cod;
    node->rssi = rssi;
    osal_list_add_tail((struct osal_list_head *)node, &g_sample_found_dev_list_head);
}

/* 更新RSSI */
static void update_found_dev_node_rssi(const bd_addr_t *bd_addr, int32_t rssi)
{
    struct osal_list_head *list_head = &g_sample_found_dev_list_head;
    struct osal_list_head *node = list_head->next;

    while (node != NULL && node != list_head) {
        sample_found_dev_list_node *dev_info = (sample_found_dev_list_node *)node;
        if (memcmp(dev_info->bd_addr.addr, bd_addr->addr, BD_ADDR_LEN) == 0) {
            dev_info->rssi = rssi;
            return;
        }

        node = node->next;
    }

    bts_sample_log("dev not found addr: ****%02x%02x%02x%02x\n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0]); /* addr下标 0 1 2 3 */
}

/* 刷新发现设备列表 */
void update_found_dev_list(void)
{
    struct osal_list_head *list_head = &g_sample_found_dev_list_head;
    struct osal_list_head *node = list_head->next;

    while (node != NULL && node != list_head) {
        sample_found_dev_list_node *dev_info = (sample_found_dev_list_node *)node;

        uint8_t len = sizeof(dev_info->name);
        if (!gap_get_device_name(&dev_info->bd_addr, dev_info->name, &len)) {
            bts_sample_log("get name fail\n");
        }

        dev_info->cod = gap_get_device_class(&dev_info->bd_addr);
        if (dev_info->cod == 0) {
            bts_sample_log("get cod fail\n");
        }

        if (!gap_read_remote_rssi_value(&dev_info->bd_addr)) {
            bts_sample_log("get rssi fail\n");
        }

        node = node->next;
    }
}

/* 对发现设备列表排序 */
static void sort_found_dev_list(void)
{
    struct osal_list_head *list_head = &g_sample_found_dev_list_head;

    for (struct osal_list_head *node1 = list_head->next; node1 != NULL && node1 != list_head; node1 = node1->next) {
        sample_found_dev_list_node *dev_info1 = (sample_found_dev_list_node *)node1;
        for (struct osal_list_head *node2 = node1->next; node2 != NULL && node2 != list_head; node2 = node2->next) {
            sample_found_dev_list_node *dev_info2 = (sample_found_dev_list_node *)node2;
            /* node1是limited模式，node2不是，node1在前，无需交换位置 */
            if (is_limied_discoverable_mode((uint32_t)dev_info1->cod) &&
                !is_limied_discoverable_mode((uint32_t)dev_info2->cod)) {
                continue;
            }
            /* 同为limited或非limited模式时，node1的rssi比node2大，rssi大的在前，无需交换位置 */
            if (((is_limied_discoverable_mode((uint32_t)dev_info1->cod) &&
                is_limied_discoverable_mode((uint32_t)dev_info2->cod)) ||
                (!is_limied_discoverable_mode((uint32_t)dev_info1->cod) &&
                !is_limied_discoverable_mode((uint32_t)dev_info2->cod))) &&
                dev_info1->rssi >= dev_info2->rssi) {
                continue;
            }

            /* 交换两个节点的信息 */
            sample_found_dev_list_node tmp_node;
            uint32_t ret =
                (uint32_t)memcpy_s(&tmp_node, sizeof(tmp_node), dev_info1, sizeof(sample_found_dev_list_node));
            ret |= (uint32_t)memcpy_s(&dev_info1->bd_addr,
                sizeof(sample_found_dev_list_node) - sizeof(struct osal_list_head),
                &dev_info2->bd_addr,
                sizeof(sample_found_dev_list_node) - sizeof(struct osal_list_head));
            ret |= (uint32_t)memcpy_s(&dev_info2->bd_addr,
                sizeof(sample_found_dev_list_node) - sizeof(struct osal_list_head),
                &tmp_node.bd_addr,
                sizeof(sample_found_dev_list_node) - sizeof(struct osal_list_head));
            if (ret != EOK) {
                bts_sample_log("swap fail %x\n", ret);
            }
        }
    }
}

/* 展示发现设备列表 */
void show_found_dev_list(void)
{
    /* 对发现设备列表排序 */
    sort_found_dev_list();

    struct osal_list_head *list_head = &g_sample_found_dev_list_head;
    /* 展示发现设备列表 */
    int32_t i = 0;
    for (struct osal_list_head *node = list_head->next; node != NULL && node != list_head; node = node->next) {
        sample_found_dev_list_node *dev_info = (sample_found_dev_list_node *)node;
        bts_sample_log("found_device[%d]: addr: ****%02x%02x%02x%02x name: \"%s\" cod: %x rssi: %d limited_mdoe: %d\n",
            i,
            dev_info->bd_addr.addr[3], dev_info->bd_addr.addr[2], /* addr下标 3 2 */
            dev_info->bd_addr.addr[1], dev_info->bd_addr.addr[0], /* addr下标 1 0 */
            dev_info->name,
            dev_info->cod,
            dev_info->rssi,
            (is_limied_discoverable_mode((uint32_t)dev_info->cod) ? 1 : 0));
        i++;
    }
}

static void btsrv_sample_cmd_enable_ble(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
#if defined(SW_UART_DEBUG) || defined(SW_RTT_DEBUG)
    btsrv_set_log_type_level(BTSRV_LOG_UART_INFO);
#endif
    errcode_t ret = enable_ble();
    bts_sample_log("enable_ble ret(errcode_t): %x\n", ret);
}

static void btsrv_sample_cmd_disable_ble(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
    ble_dis_client_clear_remote_all();
    ble_bas_client_clear_remote_all();
    ble_fmp_locator_clear_remote_all();
    ble_ancs_client_clear_remote_all();
    ble_ams_client_clear_remote_all();
    bool ret = disable_ble();
    bts_sample_log("disable_ble ret(errcode_t): %x\n", ret);
}

static void btsrv_sample_cmd_enable_bt(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);

    errcode_t ret = enable_bt_stack();
    bts_sample_log("enable_bt_stack ret(errcode_t): %x\n", ret);
}

static void btsrv_sample_cmd_disable_bt(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);

    errcode_t ret = disable_bt_stack();
    bts_sample_log("disable_bt_stack ret(errcode_t): %x\n", ret);
}

/* 获取本地设备地址 params: sample_mode len */
static void btsrv_sample_cmd_get_local_addr(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("btsrv_sample_cmd_get_local_addr params_cnt error! need: sample_mode len\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    int len = atoi(params[1]);

    unsigned char *mac = NULL;
    if ((sample_mode & 0x1) == 0 && len > 0) {
        mac = (unsigned char *)(void *)bts_sample_mem_new(len);
        if (mac == NULL) {
            bts_sample_log("btsrv_sample_cmd_get_local_addr malloc error!\n");
            return;
        }
        (void)memset_s(mac, (size_t)len, 0, (size_t)len);
    }
    if (mac != NULL) {
        errcode_t ret = bluetooth_get_local_addr(mac, (unsigned int)len);
        bts_sample_log("bluetooth_get_local_addr ret(errcode_t): %x mac: ", ret);
        if (ret == ERRCODE_BT_SUCCESS) {
            for (int i = len - 1; i >= 0; i--) {
                bts_sample_log("%02x", mac[i]);
            }
        }
        bts_sample_log("\n");
        bts_sample_mem_free(mac);
    }
}

/* 获取本地设备名称 params: sample_mode length */
static void btsrv_sample_cmd_get_local_name(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("btsrv_sample_cmd_get_local_name params_cnt error! need: sample_mode length\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    unsigned char len = (unsigned char)atoi(params[1]);

    unsigned char *name = NULL;
    if (len > 0) {
        name = (unsigned char *)(void *)bts_sample_mem_new(len);
        if (name == NULL) {
            bts_sample_log("btsrv_sample_cmd_get_local_name malloc error!\n");
            return;
        }
        (void)memset_s(name, len, 0, len);
    }

    errcode_t ret;
    if ((sample_mode & 0x3) != 0) {
        ret = bluetooth_get_local_name(NULL, NULL);
    } else if ((sample_mode & 0x1) != 0) {
        ret = bluetooth_get_local_name(NULL, &len);
    } else if ((sample_mode & 0x2) != 0) {
        ret = bluetooth_get_local_name(name, NULL);
    } else {
        ret = bluetooth_get_local_name(name, &len);
    }

    bts_sample_log("bluetooth_get_local_name ret(errcode_t): %x length: %u name: \"", ret, len);
    if (ret == ERRCODE_BT_SUCCESS) {
        bts_sample_log("%s", name);
    }
    bts_sample_log("\"\n");

    if (name != NULL) {
        bts_sample_mem_free(name);
    }
}

/* 设置本地设备名称 params: sample_mode name length */
static void btsrv_sample_cmd_set_local_name(const char **params, int32_t params_cnt)
{
    if (params_cnt != 3) { /* 3：参数个数 */
        bts_sample_log("btsrv_sample_cmd_set_local_name params_cnt error! need: sample_mode name length\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    const unsigned char *name = (const unsigned char *)params[1];
    int len = atoi(params[2]); /* 2: 第2个参数len */

    errcode_t ret;
    if ((sample_mode & 0x1) != 0) {
        ret = bluetooth_set_local_name(NULL, (unsigned char)len);
    } else {
        ret = bluetooth_set_local_name(name, (unsigned char)len);
    }

    bts_sample_log("bluetooth_set_local_name inputName: \"%s\" inputLen: %d ret(errcode_t): %x\n", name, len, ret);
}

/* 恢复出厂设置 params: 无 */
static void btsrv_sample_cmd_factory_reset(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);

    bool ret = bluetooth_factory_reset();
    bts_sample_log("bluetooth_factory_reset ret(bool): %d\n", ret);
}

/* 获取BT扫描模式 */
static void btsrv_sample_cmd_get_bt_scan_mode(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);

    int mode = gap_br_get_scan_mode();
    bts_sample_log("gap_br_get_scan_mode mode: %d\n", mode);
}

/* 设置BT扫描模式 params: mode duration */
static void btsrv_sample_cmd_set_bt_scan_mode(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("btsrv_sample_cmd_set_bt_scan_mode params_cnt error! need: mode duration\n");
        return;
    }

    int mode = atoi(params[0]);
    int duration = atoi(params[1]);

    bool ret = gap_br_set_bt_scan_mode(mode, duration);

    bts_sample_log("gap_br_set_bt_scan_mode inputMode: %d inputDuration: %d ret(bool): %d\n", mode, duration, ret);
}

/* 获取已配对设备数量 params: sample_mode */
static void btsrv_sample_cmd_get_paired_devices_num(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    if (params_cnt != 1) { /* 1：参数个数 */
        bts_sample_log("btsrv_sample_cmd_get_paired_device_num params_cnt error! need: sample_mode\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }

    uint16_t number = 0;
    errcode_t ret;
    if ((sample_mode & 0x1) != 0) {
        ret = gap_ble_get_paired_devices_num(NULL);
    } else {
        ret = gap_ble_get_paired_devices_num(&number);
    }

    bts_sample_log("gap_get_paired_devices_num ret(errcode_t): %x, num of paired devices:%u\n", ret, number);
}

/* 获取已配对设备数量 */
void wstp_get_paired_devices_num(void)
{
    unsigned int number = 0;
    bool ret = gap_get_paired_devices_num(&number);
    bts_sample_log("gap_get_paired_devices_num ret(bool): %d, num of paired devices:%u\n", ret, number);
}

/* 获取已配对设备列表 params: sample_mode number */
static void btsrv_sample_cmd_get_paired_devices_list(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("btsrv_sample_cmd_get_paired_devices_list params_cnt error! need: sample_mode number\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    int number = atoi(params[1]);
    gap_paired_device_info_t *paired_list = NULL;
    if (number > 0) {
        paired_list =
            (gap_paired_device_info_t *)(void *)bts_sample_mem_new((uint32_t)number * sizeof(gap_paired_device_info_t));
        if (paired_list == NULL) {
            bts_sample_log("btsrv_sample_cmd_get_paired_devices_list malloc error!\n");
            return;
        }
        (void)memset_s(paired_list,
            (size_t)number * sizeof(gap_paired_device_info_t),
            0,
            (size_t)number * sizeof(gap_paired_device_info_t));
    }

    bool ret;
    if ((sample_mode & 0x3) == 0x1) {
        ret = gap_get_paired_devices_list(NULL, &number);
    } else if ((sample_mode & 0x3) == 0x2) {
        ret = gap_get_paired_devices_list(paired_list, NULL);
    } else if ((sample_mode & 0x3) == 0x3) {
        ret = gap_get_paired_devices_list(NULL, NULL);
    } else {
        ret = gap_get_paired_devices_list(paired_list, &number);
    }

    bts_sample_log("gap_get_paired_devices_list ret(bool): %d\n", ret);
    if (ret && paired_list) {
        bts_sample_log("number: %d paired_list:\n", number);
        for (int i = 0; i < number; i++) {
            bts_sample_log("\t%d name: \"%s\" addr: ****%02x%02x%02x%02x\n", i, paired_list[i].device_name,
                paired_list[i].addr.addr[3], paired_list[i].addr.addr[2], /* 3 2 idx */
                /* 1 0 idx */
                paired_list[i].addr.addr[1], paired_list[i].addr.addr[0]);
        }
    }

    if (paired_list) {
        bts_sample_mem_free(paired_list);
    }
}

/* 获取已配对设备列表 */
void wstp_get_paired_devices_list(void)
{
    bool ret;
    int number = 0;
    gap_paired_device_info_t *paired_list = NULL;

    ret = gap_get_paired_devices_num((unsigned int *)&number);
    bts_sample_log("gap_get_paired_devices_num ret(bool): %d, num of paired devices:%d\n", ret, number);
    if (ret == false || number <= 0) {
        return;
    }

    paired_list = (gap_paired_device_info_t *)bts_sample_mem_new((uint32_t)number * sizeof(gap_paired_device_info_t));
    if (paired_list == NULL) {
        bts_sample_log("btsrv_sample_cmd_get_paired_devices_list malloc error!\n");
        return;
    }
    (void)memset_s(paired_list,
        (size_t)number * sizeof(gap_paired_device_info_t),
        0,
        (size_t)number * sizeof(gap_paired_device_info_t));

    ret = gap_get_paired_devices_list(paired_list, &number);
    bts_sample_log("gap_get_paired_devices_list ret(bool): %d\n", ret);
    if (ret) {
        bts_sample_log("show paired list:\n");
        bts_sample_log("| %-4s | %-32s | %-12s |\n", "idx", "name", "address");
        for (int i = 0; i < number; i++) {
            bts_sample_log("| %-4d | %-32s | ****%02x%02x%02x%02x |\n", i, paired_list[i].device_name,
                paired_list[i].addr.addr[3], paired_list[i].addr.addr[2], /* idx 3 2 */
                paired_list[i].addr.addr[1], paired_list[i].addr.addr[0]); /* idx 1 0 */
        }
        bts_sample_log("show paired list end!\n");
    }

    if (paired_list) {
        bts_sample_mem_free(paired_list);
    }
}

/* 获取配对状态 params: sample_mode addr */
static void btsrv_sample_cmd_get_pair_state(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("btsrv_sample_cmd_get_pair_state params_cnt error! need: sample_mode addr\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("btsrv_sample_cmd_get_pair_state decode addr error!\n");
        return;
    }

    errcode_t ret;
    gap_ble_pair_state_t status;
    if ((sample_mode & 0x1) != 0) {
        ret = gap_ble_get_pair_state(NULL, &status);
    } else {
        ret = gap_ble_get_pair_state(&bd_addr, &status);
    }

    bts_sample_log("gap_get_pair_state ret(errcode_t): %x status: %d\n", ret, status);
}

/* 获取配对状态 params: addr */
void wstp_get_pair_state(const bd_addr_t *bd_addr)
{
    int ret = gap_get_pair_state(bd_addr);
    bts_sample_log("gap_get_pair_state ret(int): %d\n", ret);
}

/* 删除配对 params: sample_mode addr */
static void btsrv_sample_cmd_remove_pair(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("btsrv_sample_cmd_remove_pair params_cnt error! need: sample_mode addr\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("btsrv_sample_cmd_get_pair_state decode addr error!\n");
        return;
    }

    bool ret;
    if ((sample_mode & 0x1) != 0) {
        ret = gap_remove_pair(NULL);
    } else {
        ret = gap_remove_pair(&bd_addr);
    }

    bts_sample_log("gap_remove_pair ret(bool): %d\n", ret);
}

/* 删除所有配对 */
static void btsrv_sample_cmd_remove_all_pairs(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);

    bool ret = gap_remove_all_pairs();
    bts_sample_log("gap_remove_all_pairs ret(bool): %d\n", ret);
}

/* 检查ACL是否连接 params: sample_mode addr */
static void btsrv_sample_cmd_is_acl_connected(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("btsrv_sample_cmd_is_acl_connected params_cnt error! need: sample_mode addr\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("btsrv_sample_cmd_is_acl_connected decode addr error!\n");
        return;
    }

    bool ret;
    if ((sample_mode & 0x1) != 0) {
        ret = bt_is_acl_connected(NULL);
    } else {
        ret = bt_is_acl_connected(&bd_addr);
    }

    bts_sample_log("bt_is_acl_connected ret(bool): %d\n", ret);
}

/* 断开与设备连接 params: addr addr_type */
static void btsrv_sample_cmd_disconnect_remote_device(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("btsrv_sample_cmd_disconnect_remote_device params_cnt error! need: addr addr_type.\n");
        return;
    }

    uint32_t addr_type = 0;
    if (sscanf_s(params[1], "%x", &addr_type) <= 0) {
        bts_sample_log("get addr_type fail\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("btsrv_sample_cmd_disconnect_remote_device decode addr error!\n");
        return;
    }
    bd_addr.type = (uint8_t)addr_type;

    errcode_t ret = gap_ble_disconnect_remote_device(&bd_addr);
    bts_sample_log("gap_disconnect_remote_device ret(bool): %u\n", ret);
}

bd_addr_t g_remote_connect_addr = {0};
acl_conn_ext_mode g_acl_conn_ext_mode = ACL_EXT_MODE_CONNECT_A2DP;
/* 连接设备 params: addr addr_type */
static void btsrv_sample_cmd_connect_remote_device(const char **params, int32_t params_cnt)
{
    if (params_cnt < 2) { /* 2：参数个数 */
        bts_sample_log("btsrv_sample_cmd_connect_remote_device params_cnt error! need: sample_mode addr\n");
        return;
    }

    uint8_t addr_type = 0;
    if (sscanf_s(params[1], "%x", &addr_type) <= 0) {
        bts_sample_log("get addr_type fail\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("btsrv_sample_cmd_connect_remote_device decode addr error!\n");
        return;
    }
    bd_addr.type = addr_type;
    if (params_cnt >= 3) { /* 3：包含extMode时，参数个数>= 3 */
        g_acl_conn_ext_mode = atoi(params[2]); /* 2：extMode下标 */
    }
    errcode_t ret = gap_ble_connect_remote_device(&bd_addr);
    bts_sample_log("gap_connect_remote_device ret(bool): %u\n", ret);
}

/* 连接设备，存在其他音频设备先断开在连接新的设备 */
bool wstp_connect_remote_device(const bd_addr_t *bd_addr)
{
    if (bd_addr == NULL) {
        bts_sample_log("param id null\n");
        return false;
    }
    errcode_t ret = gap_connect_remote_device(bd_addr);
    (void)memcpy_s(&g_remote_connect_addr, sizeof(bd_addr_t), bd_addr, sizeof(bd_addr_t));
    bts_sample_log("gap_connect_remote_device ret(errcode_t): %x\n", ret);
    return (ret == ERRCODE_BT_SUCCESS) ? true : false;
}

static void sample_gap_state_changed_callback(const int transport, const int status)
{
    bts_sample_log("gap_state_changed_callback transport: %d status: %d\n", transport, status);
}

#ifndef MEMORY_MINI
/* 主动发起连接 */
static bool is_active_connection(const bd_addr_t *bd_addr)
{
    if (memcmp(g_remote_connect_addr.addr, bd_addr->addr, BD_ADDR_LEN) == 0) {
        (void)memset_s(g_remote_connect_addr.addr, sizeof(bd_addr_t), 0, sizeof(bd_addr_t));
        return true;
    } else {
        return false;
    }
}
#endif

static void sample_gap_acl_state_changed_callback(const bd_addr_t *bd_addr, gap_acl_state_t state, unsigned int reason)
{
    bts_sample_log("gap_acl_state_changed_callback addr: ");
    for (int i = BD_ADDR_LEN - 1; i >= 0; i--) {
        bts_sample_log("%02x", bd_addr->addr[i]);
    }
    int device_class = 0;
    if (state == GAP_ACL_STATE_CONNECTED) {
        /* 不扫描获取不到耳机COD，但是手机在连接时可获取到COD。 */
        device_class = gap_get_device_class(bd_addr);
        bts_sample_log(" state: %d reason: %x cod: %x \n", state, reason, device_class);
    }
#ifndef MEMORY_MINI
#if (BTH_BREDR_ONLY == YES) || (BTH_BREDR_LE_DOUBLE == YES)
    if (state == GAP_ACL_STATE_CONNECTED && ((uint32_t)device_class & 0x200) == 0 && is_active_connection(bd_addr)) {
        /* 只要是非手机设备，连接成功后就先初始化audio manager为音乐播放 */
        bts_sample_log("sample_gap_acl_state_changed_callback call AudioManagerInit - AUDIO_STREAM_MUSIC\n");

        if (avrcp_ct_get_device_connect_state(bd_addr) == PROFILE_STATE_DISCONNECTED) {
            /* 连接建立成功后，主动建立AVRCP连接 */
            bts_sample_log("call avrcp_ct_connect\n");
            int conn_ret = avrcp_ct_connect(bd_addr);
            bts_sample_log("avrcp_ct_connect ret(int): %x\n", conn_ret);
        }
        /* 设置扩展模式为连带连接A2DP时，对非手机设备，连带主动触发建立A2DP连接。
         * 但特别注意：
         *     实测发现有的音箱不接受被动建立A2DP连接，需要由音箱主动，如小爱音箱play。
         *     这种情况下，不能设置连带建立A2DP连接模式，只建立ACL连接即可，音箱会主动触发A2DP连接。
         */
        if (g_acl_conn_ext_mode == ACL_EXT_MODE_CONNECT_A2DP &&
            a2dp_src_get_device_connect_state(bd_addr) == PROFILE_STATE_DISCONNECTED) {
            bts_sample_log("sample_gap_acl_state_changed_callback call a2dp_src_connect\n");
            int conn_ret = a2dp_src_connect(bd_addr);
            bts_sample_log("sample_gap_acl_state_changed_callback a2dp_src_connect ret(int): %x\n", conn_ret);
        }
    }
#endif
#endif
}

static void sample_gap_discovery_state_changed_callback(int status)
{
    bts_sample_log("gap_discovery_state_changed_callback status: %d\n", status);
}

static void sample_gap_discovery_result_callback(const bd_addr_t *bd_addr)
{
    unsigned char length = BD_NAME_LEN;
    unsigned char *device_name = (unsigned char *)(void *)bts_sample_mem_new(length);
    if (device_name == NULL) {
        bts_sample_log("gap_discovery_result_callback malloc error!\n");
        return;
    }
    (void)memset_s(device_name, length, 0, length);

    bool ret1 = gap_get_device_name(bd_addr, device_name, &length);
    int cod = gap_get_device_class(bd_addr);
    bool ret2 = gap_read_remote_rssi_value(bd_addr);

    bts_sample_log("gap_discovery_result_callback addr: ****%02x%02x%02x%02x ",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0]); /* addr下标 0 1 2 3 */
    bts_sample_log("cod: %x getNameRet: %x getRssiRet: %x ", cod, ret1, ret2);
    if (ret1) {
        bts_sample_log("nameLen: %u name: \"%s\"", length, device_name);
    }
    bts_sample_log("\n");

    add_node_to_found_dev_list(bd_addr, device_name, cod, 0);
    bts_sample_mem_free(device_name);
}

static void sample_gap_pair_requested_callback(const bd_addr_t *bd_addr)
{
    bts_sample_log("gap_pair_requested_callback addr: ****%02x%02x%02x%02x\n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0]); /* addr下标 0 1 2 3 */
}

static void sample_gap_pair_confirmed_callback(const bd_addr_t *bd_addr, int req_type, int number)
{
    bts_sample_log("gap_pair_confirmed_callback addr: ****%02x%02x%02x%02x req_type: %d number: %d\n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0], /* addr下标 0 1 2 3 */
        req_type, number);
}

static void sample_gap_scan_mode_changed_callback(int mode)
{
    bts_sample_log("gap_scan_mode_changed_callback mode: %d\n", mode);
}

static void sample_gap_local_device_name_changed_callback(const unsigned char *device_name, unsigned char length)
{
    bts_sample_log("gap_local_device_name_changed_callback length: %u name: \"%s\"\n", length, device_name);
}

static void sample_gap_local_device_addr_changed_callback(const bd_addr_t *bd_addr)
{
    bts_sample_log("gap_local_device_addr_changed_callback addr: ****%02x%02x%02x%02x\n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0]); /* addr下标 0 1 2 3 */
}

static void sample_gap_pair_status_changed_callback(const bd_addr_t *bd_addr, int status)
{
    bts_sample_log("gap_pair_status_changed_callback addr: ****%02x%02x%02x%02x status: %d\n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0], status); /* addr下标 0 1 2 3 */
}

static void sample_gap_remote_uuid_changed_callback(const bd_addr_t *bd_addr, bt_uuid_t uuid)
{
    bts_sample_log("gap_remote_uuid_changed_callback addr: ****%02x%02x%02x%02x uuid: ",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0]); /* addr下标 0 1 2 3 */
    for (int i = 0; i < uuid.uuid_len; i++) {
        bts_sample_log("%02x", uuid.uuid[i]);
    }
    bts_sample_log("\n");
}

static void sample_gap_remote_name_changed_callback(const bd_addr_t *bd_addr,
    const unsigned char *device_name, unsigned char length)
{
    bts_sample_log("gap_remote_name_changed_callback addr: ****%02x%02x%02x%02x length: %u device_name: \"%s\"\n",
        bd_addr->addr[3], bd_addr->addr[2], /* addr下标 2 3 */
        bd_addr->addr[1], bd_addr->addr[0], length, device_name); /* addr下标 0 1 */
}

static void sample_gap_remote_alias_changed_callback(const bd_addr_t *bd_addr,
    const unsigned char *alias, unsigned char length)
{
    bts_sample_log("gap_remote_alias_changed_callback addr: ****%02x%02x%02x%02x length: %u alias: \"%s\"\n",
        bd_addr->addr[3], bd_addr->addr[2], /* addr下标 2 3 */
        bd_addr->addr[1], bd_addr->addr[0], length, alias); /* addr下标 0 1 */
}

static void sample_gap_remote_cod_changed_callback(const bd_addr_t *bd_addr, int cod)
{
    bts_sample_log("gap_remote_cod_changed_callback addr: ****%02x%02x%02x%02x cod: %d\n",
        bd_addr->addr[3], bd_addr->addr[2], /* addr下标 2 3 */
        bd_addr->addr[1], bd_addr->addr[0], cod); /* addr下标 0 1 */
}

static void sample_gap_remote_battery_level_changed_callback(const bd_addr_t *bd_addr, int battery_level)
{
    bts_sample_log("gap_remote_battery_level_changed_callback addr: ****%02x%02x%02x%02x battery_level: %d\n",
        bd_addr->addr[3], bd_addr->addr[2], /* addr下标 2 3 */
        bd_addr->addr[1], bd_addr->addr[0], battery_level); /* addr下标 0 1 */
}

static void sample_gap_read_remote_rssi_event_callback(const bd_addr_t *bd_addr, int rssi, int status)
{
    bts_sample_log("gap_read_remote_rssi_event_callback addr: ****%02x%02x%02x%02x rssi: %d status: %d\n",
        bd_addr->addr[3], bd_addr->addr[2], /* addr下标 2 3 */
        bd_addr->addr[1], bd_addr->addr[0], rssi, status); /* addr下标 0 1 */

    if (status == ERRCODE_BT_SUCCESS) {
        update_found_dev_node_rssi(bd_addr, rssi);
    }
}

static bool g_sample_gap_accept_conn = true;

static void sample_gap_is_accept_conn_on_safe_mode_callback(const bd_addr_t *bd_addr, bool *res)
{
    bts_sample_log("is_accept_conn_on_safe_mode_callback addr: ****%02x%02x%02x%02x set *res true\n",
        bd_addr->addr[3], bd_addr->addr[2], /* addr下标 2 3 */
        bd_addr->addr[1], bd_addr->addr[0]); /* addr下标 0 1 */

    *res = g_sample_gap_accept_conn;
}

/* 注册回调 params: sample_mode fill_callbacks */
void btsrv_at_cmd_gap_register_callbacks(void)
{
    gap_call_backs_t cb = {
        sample_gap_state_changed_callback,
        sample_gap_acl_state_changed_callback,
        sample_gap_scan_mode_changed_callback,
        sample_gap_pair_status_changed_callback,
        sample_gap_local_device_name_changed_callback,
        sample_gap_local_device_addr_changed_callback,
        sample_gap_pair_requested_callback,
        sample_gap_pair_confirmed_callback,
        sample_gap_discovery_state_changed_callback,
        sample_gap_discovery_result_callback,
        sample_gap_remote_uuid_changed_callback,
        sample_gap_remote_name_changed_callback,
        sample_gap_remote_alias_changed_callback,
        sample_gap_remote_cod_changed_callback,
        sample_gap_remote_battery_level_changed_callback,
        sample_gap_read_remote_rssi_event_callback,
        sample_gap_is_accept_conn_on_safe_mode_callback
    };
    int ret = gap_register_callbacks(&cb);
    bts_sample_log("gap reg cbk ret %x\n", ret);
}

/* 注册回调 params: sample_mode fill_callbacks */
static void btsrv_sample_cmd_gap_register_callbacks(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("gap_register_callbacks params_cnt error! need: sample_mode fill_callbacks\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    int fill_callbacks = atoi(params[1]);

    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = gap_register_callbacks(NULL);
    } else if (fill_callbacks != 0) {
        gap_call_backs_t cb = {
            sample_gap_state_changed_callback,
            sample_gap_acl_state_changed_callback,
            sample_gap_scan_mode_changed_callback,
            sample_gap_pair_status_changed_callback,
            sample_gap_local_device_name_changed_callback,
            sample_gap_local_device_addr_changed_callback,
            sample_gap_pair_requested_callback,
            sample_gap_pair_confirmed_callback,
            sample_gap_discovery_state_changed_callback,
            sample_gap_discovery_result_callback,
            sample_gap_remote_uuid_changed_callback,
            sample_gap_remote_name_changed_callback,
            sample_gap_remote_alias_changed_callback,
            sample_gap_remote_cod_changed_callback,
            sample_gap_remote_battery_level_changed_callback,
            sample_gap_read_remote_rssi_event_callback,
            sample_gap_is_accept_conn_on_safe_mode_callback
        };
        ret = gap_register_callbacks(&cb);
        bts_sample_log("gap_register_callbacks with funcs ret(int): %x\n", ret);
    } else {
        gap_call_backs_t cb = { 0, };
        ret = gap_register_callbacks(&cb);
        bts_sample_log("gap_register_callbacks test ret(int): %x\n", ret);
    }
}

/* BLE 连接参数更新 param: conn_handle interval_min interval_max slave_latency timeout_multiplier */
static void btsrv_sample_cmd_gap_ble_connect_param_update(const char **params, int32_t params_cnt)
{
    if (params_cnt != 5) { /* 5：参数个数 */
        bts_sample_log("gap ble update connection parameters params_cnt error! need: conn_handle int_min int_max"
            "slave_latency timeout_multiplier.\r\n");
        return;
    }

    gap_conn_param_update_t conn_param = { 0 };
    conn_param.conn_handle = (uint16_t)atoi(params[0]);    /* param 0: conn_handle */
    conn_param.interval_min = (uint16_t)atoi(params[1]);    /* param 1: interval_min */
    conn_param.interval_max = (uint16_t)atoi(params[2]);    /* param 2: interval_max */
    conn_param.slave_latency = (uint16_t)atoi(params[3]);    /* param 3: slave_latency */
    conn_param.timeout_multiplier = (uint16_t)atoi(params[4]);    /* param 4: timeout_multiplier */
    errcode_t ret = gap_ble_connect_param_update(&conn_param);
    bts_sample_log("gap ble update connection parameters ret: %u\n", ret);
}

static void btsrv_sample_cmd_cancel_bt_discovery(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);

    bool ret = gap_br_cancel_discovery();
    bts_sample_log("gap_br_cancel_discovery ret(bool): %d\n", ret);
}

/* 检查是否正在BT扫描中 params: transport */
static void btsrv_sample_cmd_is_bt_discovering(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) { /* 1：参数个数 */
        bts_sample_log("is_bt_discovering params_cnt error! need: mode duration\n");
        return;
    }

    int transport = atoi(params[0]);

    bool ret = is_bt_discovering(transport);

    bts_sample_log("is_bt_discovering transport: %d ret(bool): %d\n", transport, ret);
}

/* 获取远端设备RSSI params: sample_mode addr */
static void btsrv_sample_cmd_read_remote_rssi_value(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("gap_read_remote_rssi_value params_cnt error! need: sample_mode addr\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("gap_read_remote_rssi_value decode addr error!\n");
        return;
    }

    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = gap_read_remote_rssi_value(NULL);
    } else {
        ret = gap_read_remote_rssi_value(&bd_addr);
    }

    bts_sample_log("gap_read_remote_rssi_value ret(bool): %d\n", ret);
}

/* 获取远端设备名称 params: sample_mode addr length */
static void btsrv_sample_cmd_get_device_name(const char **params, int32_t params_cnt)
{
    if (params_cnt != 3) { /* 3：参数个数 */
        bts_sample_log("btsrv_sample_cmd_get_device_name params_cnt error! need: sample_mode length\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("btsrv_sample_cmd_get_device_name decode addr error!\n");
        return;
    }
    unsigned char len = (unsigned char)atoi(params[2]); /* 2: 参数下标 */

    unsigned char *name = NULL;
    if ((sample_mode & 0x2) == 0 && len > 0) {
        name = (unsigned char *)(void *)bts_sample_mem_new(len);
        if (name == NULL) {
            bts_sample_log("btsrv_sample_cmd_get_device_name malloc error!\n");
            return;
        }
        (void)memset_s(name, len, 0, len);
    }

    bd_addr_t *bt_addr_tmp = ((sample_mode & 0x1) == 0 ? &bd_addr : NULL);
    unsigned char *name_tmp = ((sample_mode & 0x2) == 0 ? name : NULL);
    unsigned char *length_tmp = ((sample_mode & 0x4) == 0 ? &len : NULL);
    bool ret = gap_get_device_name(bt_addr_tmp, name_tmp, length_tmp);

    bts_sample_log("gap_get_device_name ret(bool): %d name: \"", ret);
    if (ret) {
        bts_sample_log("%s", name);
    }
    bts_sample_log("\" len: %u\n", len);

    if (name != NULL) {
        bts_sample_mem_free(name);
    }
}

/* 获取远端设备COD params: sample_mode addr */
static void btsrv_sample_cmd_get_device_class(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("gap_get_device_class params_cnt error! need: sample_mode addr\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("gap_get_device_class decode addr error!\n");
        return;
    }

    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = gap_get_device_class(NULL);
    } else {
        ret = gap_get_device_class(&bd_addr);
    }

    bts_sample_log("gap_get_device_class device_class: %d\n", ret);
}

static void btsrv_sample_cmd_set_filter_device_class(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("gap_br_set_inquiry_paramters params_cnt error! need: device_class max_number\n");
        return;
    }

    uint32_t dev_class = (uint32_t)atoi(params[0]);
    uint8_t max_num = (uint8_t)atoi(params[1]);
    int ret = gap_br_set_inquiry_paramters(dev_class, max_num);

    bts_sample_log("gap_br_set_inquiry_paramters dev_class:0x%x max_num:0x%x ret: %d\n",
        dev_class, max_num, ret);
}

static void btsrv_sample_cmd_update_found_list(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);

    update_found_dev_list();
}

static void btsrv_sample_cmd_show_found_list(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);

    show_found_dev_list();
}

static void btsrv_sample_cmd_set_connect_req_answer(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) { /* 1：参数个数 */
        bts_sample_log("btsrv_sample_cmd_set_connect_req_answer params_cnt error! need: accept\n");
        return;
    }

    g_sample_gap_accept_conn = (bool)atoi(params[0]);
}

static void btsrv_sample_cmd_set_feature_config(const char **params, int32_t params_cnt)
{
    bts_feature_t feature;
    if (params_cnt != 1) { /* 1：参数个数 */
        bts_sample_log("btsrv_sample_cmd_set_feature_config params_cnt error! need: feature_config\n");
        return;
    }
    *(int *)&feature = atoi(params[0]);
    bts_sample_log("feature:%d bt_auto_reconnect_closed:%d\n", *(int *)&feature, feature.bt_auto_reconnect_closed);
    errcode_t ret = bts_set_features(&feature);
    bts_sample_log("bts set features ret:%d\n", ret);
}

static void btsrv_sample_cmd_get_feature_config(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
    bts_feature_t feature = bts_get_features();
    bts_sample_log("bts get features feature: bt_auto_reconnect_closed:%d.\n", feature.bt_auto_reconnect_closed);
}

static void btsrv_sample_cmd_reconnect_phone(const char **params, int32_t params_cnt)
{
    errcode_t ret;
    if (params_cnt != 1) { /* 1：参数个数 */
        bts_sample_log("reconnect phone param error! need: bd_addr.\n");
        return;
    }

    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("reconnect phone decode addr error!\n");
        return;
    }
    ret = gap_reconnect_media_device(&bd_addr);
    bts_sample_log("reconnect phone ret:%d.\n", ret);
}

static void btsrv_sample_cmd_set_service_task_priority(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) { /* 1：参数个数 */
        bts_sample_log("params_cnt error! need one param!\n");
        return;
    }
    uint16_t task_priority = (uint16_t)atoi(params[0]);
    errcode_t ret = bts_set_service_task_priority(task_priority);
    bts_sample_log("bts set service task priority ret:0x%x priority:%d\n", ret, task_priority);
}

static void btsrv_sample_cmd_set_flow_limit(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) { /* 1：参数个数 */
        bts_sample_log("params_cnt error! need one param!\n");
        return;
    }
    uint16_t flow_limit = (uint16_t)atoi(params[0]);
    errcode_t ret = gap_bt_set_flow_limit(flow_limit);
    bts_sample_log("bts set flow limit ret:0x%x limit:%d\n", ret, flow_limit);
}

static void btsrv_sample_cmd_set_spp_write_retry_max(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) { /* 1：参数个数 */
        bts_sample_log("params_cnt error! need one param!\n");
        return;
    }
    uint32_t retry_max = (uint32_t)atoi(params[0]);
    errcode_t ret = spp_write_retry_times_config(retry_max);
    bts_sample_log("bts set spp write retry max ret:0x%x max:%u\n", ret, retry_max);
}

btsrv_sample_cmd_hdl_info g_btsrv_gap_sample_hdls[] = {
    /* 0 */
    { BTSRV_SAMPLE_GAP_ENABLE_BLE,  btsrv_sample_cmd_enable_ble },
    { BTSRV_SAMPLE_GAP_DISABLE_BLE,  btsrv_sample_cmd_disable_ble },
    { BTSRV_SAMPLE_GAP_ENABLE_BT,  btsrv_sample_cmd_enable_bt },
    { BTSRV_SAMPLE_GAP_DISABLE_BT,  btsrv_sample_cmd_disable_bt },
    { BTSRV_SAMPLE_GAP_GET_LOCAL_ADDR, btsrv_sample_cmd_get_local_addr },

    /* 5 */
    { BTSRV_SAMPLE_GAP_GET_LOCAL_NAME, btsrv_sample_cmd_get_local_name },
    { BTSRV_SAMPLE_GAP_SET_LOCAL_NAME, btsrv_sample_cmd_set_local_name },
    { BTSRV_SAMPLE_GAP_FACTORY_RESET, btsrv_sample_cmd_factory_reset },
    { BTSRV_SAMPLE_GAP_GET_BT_SCAN_MODE, btsrv_sample_cmd_get_bt_scan_mode },
    { BTSRV_SAMPLE_GAP_SET_BT_SCAN_MODE, btsrv_sample_cmd_set_bt_scan_mode },

    /* 10 */
    { BTSRV_SAMPLE_GAP_GET_PAIRED_DEVICE_NUM, btsrv_sample_cmd_get_paired_devices_num },
    { BTSRV_SAMPLE_GAP_GET_PAIRED_DEVICE_LIST, btsrv_sample_cmd_get_paired_devices_list },
    { BTSRV_SAMPLE_GAP_GET_PAIR_STATE, btsrv_sample_cmd_get_pair_state },
    { BTSRV_SAMPLE_GAP_REMOVE_PAIR,  btsrv_sample_cmd_remove_pair},
    { BTSRV_SAMPLE_GAP_REMOVE_ALL_PAIRS, btsrv_sample_cmd_remove_all_pairs },

    /* 15 */
    { BTSRV_SAMPLE_GAP_IS_ACL_CONNECTED, btsrv_sample_cmd_is_acl_connected },
    { BTSRV_SAMPLE_GAP_DISCONNECT_REMOTE_DEVICE, btsrv_sample_cmd_disconnect_remote_device },
    { BTSRV_SAMPLE_GAP_CONNECT_REMOTE_DEVICE, btsrv_sample_cmd_connect_remote_device },
    { BTSRV_SAMPLE_GAP_REGISTER_CALLBACKS, btsrv_sample_cmd_gap_register_callbacks },
    { BTSRV_SAMPLE_GAP_UPDATE_CONNECT_PARAM, btsrv_sample_cmd_gap_ble_connect_param_update },

    /* 20 */
    { BTSRV_SAMPLE_GAP_STOP_BT_DISCOVERY, btsrv_sample_cmd_cancel_bt_discovery },
    { BTSRV_SAMPLE_GAP_IS_BT_DISCOVERY, btsrv_sample_cmd_is_bt_discovering },
    { BTSRV_SAMPLE_GAP_READ_REMOTE_RSSI_VALUE, btsrv_sample_cmd_read_remote_rssi_value },
    { BTSRV_SAMPLE_GAP_GET_DEVICE_NAME, btsrv_sample_cmd_get_device_name },
    { BTSRV_SAMPLE_GAP_GET_DEVICE_CLASS, btsrv_sample_cmd_get_device_class },

    /* 25 */
    { BTSRV_SAMPLE_GAP_PAIR_REQUEST_RELY, NULL },
    { BTSRV_SAMPLE_GAP_SET_DEVICE_PIN, NULL },
    { BTSRV_SAMPLE_GAP_SET_DEVICE_PAIRING_CONFIRMATION, NULL },
    { BTSRV_SAMPLE_GAP_SET_DEVICE_PASSKEY, NULL },
    /* 29 */
    { BTSRV_SAMPLE_GAP_UPDATE_FOUND_LIST, btsrv_sample_cmd_update_found_list },

    /* 30 */
    { BTSRV_SAMPLE_GAP_SHOW_FOUND_LIST, btsrv_sample_cmd_show_found_list },
    { BTSRV_SAMPLE_GAP_SET_CONN_REQ_ANSWER, btsrv_sample_cmd_set_connect_req_answer },
    { BTSRV_SAMPLE_GAP_SET_FEATURE_CONFIG, btsrv_sample_cmd_set_feature_config },
    { BTSRV_SAMPLE_GAP_GET_FEATURE_CONFIG, btsrv_sample_cmd_get_feature_config },
    { BTSRV_SAMPLE_GAP_RECONNECT_PHONE, btsrv_sample_cmd_reconnect_phone },

    /* 35 */
    { BTSRV_SAMPLE_GAP_SET_INQUIRY_PARAM, btsrv_sample_cmd_set_filter_device_class },
    { BTSRV_SAMPLE_GAP_SET_SERVICE_TASK_PRIORITY, btsrv_sample_cmd_set_service_task_priority},
    { BTSRV_SAMPLE_GAP_SET_FLOW_LIMIT, btsrv_sample_cmd_set_flow_limit},
    { BTSRV_SAMPLE_GAP_SET_SPP_WRITE_RETRY_MAX, btsrv_sample_cmd_set_spp_write_retry_max},
};

void btsrv_gap_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    uint32_t hdl_cnt = sizeof(g_btsrv_gap_sample_hdls) / sizeof(btsrv_sample_cmd_hdl_info);

    for (uint32_t i = 0; i < hdl_cnt; i++) {
        if (g_btsrv_gap_sample_hdls[i].id == cmd_id && g_btsrv_gap_sample_hdls[i].func != NULL) {
            g_btsrv_gap_sample_hdls[i].func(params, params_cnt);
            return;
        }
    }

    bts_sample_log("btsrv_gap_sample_cmd cmd_id %d not support now!\n", cmd_id);
}
