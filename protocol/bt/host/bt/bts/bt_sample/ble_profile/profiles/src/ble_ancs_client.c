/*
 * Copyright (c) @CompanyNameMagicTag 2024. All rights reserved.
 * Description: BLE Apple Notification Center Service Client module.
 */

#include <string.h>
#include "errcode.h"
#include "osal_addr.h"
#include "sample_common.h"
#include "bts_le_gap.h"
#include "ble_gatts_callbacks.h"
#include "ble_ancs_client.h"

#define BLE_CLIENT_CHARACTERISTIC_CONFIGURATION_UUID 0x2902
#define ANCS_NOTIFY_ATT_ID_OFFSET 5
#define ANCS_NOTIFY_ATT_MAX_LENGTH_BYTE 2
#define ANCS_NOTIFY_ACTION_ID_OFFSET 5
#define ANCS_NOTIFY_ATT_VALUE_OFFSET 3
#define BLE_ATT_HEADER_LEN 3
#define BLE_ANCS_DATA_MAX_LEN 512
#define ANCS_NOTIFY_UUID_BYTE_LENGTH 4
#define ANCS_NOTIFY_SOURCE_LENGTH 8
#define ANCS_NOTIFY_ATT_DATA_HEADER_LENGTH 8

#define u16_low_byte(data) ((uint8_t)((data) & 0x00FF))
#define u16_high_byte(data) ((uint8_t)(((data) & 0xFF00) >> 8))
#define little_u8_2_u16(p)   ((((uint16_t)(*((p) + 1))) << 8) | ((uint16_t)(*(p))))
#define u32_2_little_first_byte(uuid) ((uint8_t)((uuid) & 0x000000FF))
#define u32_2_little_second_byte(uuid) ((uint8_t)(((uuid) & 0x0000FF00) >> 8))
#define u32_2_little_three_byte(uuid) ((uint8_t)(((uuid) & 0x00FF0000) >> 16))
#define u32_2_little_four_byte(uuid) ((uint8_t)(((uuid) & 0xFF000000) >> 24))
#define little_byte_2_u32(data) (((uint32_t)(*(data))) | \
    ((uint32_t)(((uint32_t)*((data) + BT_NUM1)) << 8)) | \
    ((uint32_t)(((uint32_t)*((data) + BT_NUM2)) << 16)) | \
    ((uint32_t)(((uint32_t)*((data) + BT_NUM3)) << 24)))

/* ANCS服务UUID，小端序 */
uint8_t ancs_uuid[BT_UUID_MAX_LEN] = {
    0xD0, 0x00, 0x2D, 0x12, 0x1E, 0x4B, 0x0F, 0xA4, 0x99, 0x4E, 0xCE, 0xB5, 0x31, 0xF4, 0x05, 0x79};

/* ANCS服务特征UUID-Notification Source，小端序 */
uint8_t notify_src_uuid[BT_UUID_MAX_LEN] = {
    0xBD, 0x1D, 0xA2, 0x99, 0xE6, 0x25, 0x58, 0x8C, 0xD9, 0x42, 0x01, 0x63, 0x0D, 0x12, 0xBF, 0x9F};

/* ANCS服务特征UUID-Control Point，小端序 */
uint8_t ctl_point_uuid[BT_UUID_MAX_LEN] = {
    0xD9, 0xD9, 0xAA, 0xFD, 0xBD, 0x9B, 0x21, 0x98, 0xA8, 0x49, 0xE1, 0x45, 0xF3, 0xD8, 0xD1, 0x69};

/* ANCS服务特征UUID-Data Source，小端序 */
uint8_t data_src_uuid[BT_UUID_MAX_LEN] = {
    0xFB, 0x7B, 0x7C, 0xCE, 0x6A, 0xB3, 0x44, 0xBE, 0xB5, 0x4B, 0xD6, 0x24, 0xE9, 0xC6, 0xEA, 0x22};

ble_ancs_client_callbacks_t g_ancs_callbacks_t = {0};

OSAL_LIST_HEAD(g_ble_ancs_service_list);  /* ble_ancs_service_t */

/* 根据连接ID查询ANCS服务信息 */
static ble_ancs_service_t* ancs_service_remote_by_conn(uint8_t client_id, uint16_t conn_id)
{
    struct osal_list_head *list_head = &g_ble_ancs_service_list;
    struct osal_list_head *node = list_head->next;
    while (node != NULL && node != list_head) {
        ble_ancs_service_t *svc_node = (ble_ancs_service_t *)node;
        if ((svc_node->client_id == client_id) && (svc_node->conn_id == conn_id)) {
            return svc_node;
        }
        node = node->next;
    }

    return NULL;
}

/* ANCS服务特征匹配 */
static ancs_character_t ancs_char_match(bt_uuid_t *uuid)
{
    if (uuid->uuid_len != BT_UUID_MAX_LEN) {
        return ANCS_CHAR_INVALID;
    }
    bt_uuid_t char_uuid = {0};
    char_uuid.uuid_len = BT_UUID_MAX_LEN;
    if (memcpy_s(char_uuid.uuid, BT_UUID_MAX_LEN, notify_src_uuid, BT_UUID_MAX_LEN) != EOK) {
        bts_sample_error_log("[btsrv][ERROR] notify src uuid memcpy fail!\r\n");
        return ANCS_CHAR_INVALID;
    }
    if (memcmp(&char_uuid, uuid, sizeof(bt_uuid_t)) == 0) {
        return ANCS_CHAR_NOTIFY_SRC;
    }
    if (memcpy_s(char_uuid.uuid, BT_UUID_MAX_LEN, ctl_point_uuid, BT_UUID_MAX_LEN) != EOK) {
        bts_sample_error_log("[btsrv][ERROR] ctl point uuid memcpy fail!\r\n");
        return ANCS_CHAR_INVALID;
    }
    if (memcmp(&char_uuid, uuid, sizeof(bt_uuid_t)) == 0) {
        return ANCS_CHAR_CTRL_POINT;
    }
    if (memcpy_s(char_uuid.uuid, BT_UUID_MAX_LEN, data_src_uuid, BT_UUID_MAX_LEN) != EOK) {
        bts_sample_error_log("[btsrv][ERROR] data src uuid memcpy fail!\r\n");
        return ANCS_CHAR_INVALID;
    }
    if (memcmp(&char_uuid, uuid, sizeof(bt_uuid_t)) == 0) {
        return ANCS_CHAR_DATA_SRC;
    }
    return ANCS_CHAR_INVALID;
}

/* ANCS客户端发起服务建立 */
errcode_t ble_ancs_client_setup_service(uint8_t client_id, uint16_t conn_id)
{
    ble_ancs_service_t *remote = ancs_service_remote_by_conn(client_id, conn_id);
    if (remote != NULL) {
        bts_sample_error_log("ancs client remote service is already exist,client_id:%u, conn_id:%u.\r\n",
            client_id, conn_id);
        return ERRCODE_BT_DONE;
    }
    bt_uuid_t service_uuid = {0};
    service_uuid.uuid_len = BT_UUID_MAX_LEN;
    if (memcpy_s(service_uuid.uuid, BT_UUID_MAX_LEN, ancs_uuid, BT_UUID_MAX_LEN) != EOK) {
        bts_sample_error_log("[btsrv][ERROR] uuid memcpy fail!\r\n");
        return ERRCODE_BT_FAIL;
    }
    return gattc_discovery_service(client_id, conn_id, &service_uuid);
}

/* ANCS客户端发现服务回调处理函数 */
void ble_ancs_client_discovery_service_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_service_result_t *service)
{
    ble_ancs_service_t *remote_svc = ancs_service_remote_by_conn(client_id, conn_id);
    if (remote_svc != NULL) {
        bts_sample_error_log("ancs client remote service is already exist,client_id:%u, conn_id:%u.\r\n",
            client_id, conn_id);
        return;
    }
    remote_svc = (ble_ancs_service_t *)bts_sample_mem_new(sizeof(ble_ancs_service_t));
    if (remote_svc == NULL) {
        bts_sample_error_log("ancs client remote node create fail.\r\n");
        return;
    }
    remote_svc->client_id = client_id;
    remote_svc->conn_id = conn_id;
    remote_svc->start_handle = service->start_hdl;
    remote_svc->end_handle = service->end_hdl;
    osal_list_add_tail((struct osal_list_head *)remote_svc, &g_ble_ancs_service_list);

    /* 发现特征 */
    gattc_discovery_character_param_t param = {0};
    param.service_handle = remote_svc->start_handle;
    errcode_t ret = gattc_discovery_character(client_id, conn_id, &param);
    bts_sample_info_log("ancs client discovery character ret:%u.\r\n", ret);
}

/* ANCS客户端发现特征回调处理函数 */
void ble_ancs_client_discovery_character_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_result_t *character)
{
    ble_ancs_service_t *remote = ancs_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        return;
    }

    if ((character->declare_handle < remote->start_handle) ||
        (character->declare_handle > remote->end_handle)) {
        bts_sample_error_log("ancs client remote service did not have this character_handle:%u conn_id:%u\n",
            character->declare_handle, conn_id);
        return;
    }

    switch (ancs_char_match(&(character->uuid))) {
        case ANCS_CHAR_NOTIFY_SRC: {
            remote->notify_src.declare_handle = character->declare_handle;
            remote->notify_src.value_handle = character->value_handle;
            remote->notify_src.properties = character->properties;

            /* 发现描述符 */
            gattc_discovery_descriptor(client_id, conn_id, character->declare_handle);
            break;
        }

        case ANCS_CHAR_DATA_SRC: {
            remote->data_src.declare_handle = character->declare_handle;
            remote->data_src.value_handle = character->value_handle;
            remote->data_src.properties = character->properties;

            /* 发现描述符 */
            gattc_discovery_descriptor(client_id, conn_id, character->declare_handle);
            break;
        }

        case ANCS_CHAR_CTRL_POINT: {
            remote->ctl_point.declare_handle = character->declare_handle;
            remote->ctl_point.value_handle = character->value_handle;
            remote->ctl_point.properties = character->properties;
            break;
        }

        default:
            bts_sample_error_log("[btsrv][INFO] invalid character!\r\n");
            break;
    }
}

/* ANCS客户端发现描述符回调处理函数 */
void ble_ancs_client_discovery_descriptor_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_descriptor_result_t *descriptor)
{
    ble_ancs_service_t *remote = ancs_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        return;
    }

    if (descriptor->descriptor_hdl < remote->start_handle ||
        descriptor->descriptor_hdl > remote->end_handle) {
        bts_sample_error_log("ancs client remote service did not have this descriptor_handle:%u conn_id:%u\n",
            descriptor->descriptor_hdl, conn_id);
        return;
    }

    /* 客户端特征配置描述符 */
    bt_uuid_t ccc_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_CLIENT_CHARACTERISTIC_CONFIGURATION_UUID, &ccc_uuid);
    if (!bts_sample_bt_gatts_cmp_uuid(&ccc_uuid, &(descriptor->uuid))) {
        return;
    }

    uint16_t notify_ccc_max_handle = remote->end_handle;
    uint16_t data_ccc_max_handle = remote->end_handle;
    if ((remote->data_src.value_handle != 0) &&
        (remote->notify_src.value_handle != 0) &&
        (remote->notify_src.value_handle < remote->data_src.value_handle)) {
        notify_ccc_max_handle = remote->data_src.value_handle - 1;
    } else if ((remote->notify_src.value_handle != 0) &&
        (remote->data_src.value_handle != 0) &&
        (remote->notify_src.value_handle > remote->data_src.value_handle)) {
        data_ccc_max_handle = remote->notify_src.value_handle - 1;
    }

    if ((remote->notify_src.value_handle != 0) &&
        (descriptor->descriptor_hdl > remote->notify_src.value_handle) &&
        (descriptor->descriptor_hdl <= notify_ccc_max_handle)) {
        remote->notify_src_ccc_handle = descriptor->descriptor_hdl;
    } else if ((remote->data_src.value_handle != 0) &&
        (descriptor->descriptor_hdl > remote->data_src.value_handle) &&
        (descriptor->descriptor_hdl <= data_ccc_max_handle)) {
        remote->data_src_ccc_handle = descriptor->descriptor_hdl;
    }

    if ((remote->notify_src_ccc_handle != 0) &&
        (remote->data_src_ccc_handle != 0)) {
        if (g_ancs_callbacks_t.setup_cb != NULL) {
            g_ancs_callbacks_t.setup_cb(client_id, conn_id, ERRCODE_BT_SUCCESS);
        }
    }
}

/* ANCS客户端订阅通知源特征通知 */
errcode_t ble_ancs_client_subscribe_notification_source(uint8_t client_id, uint16_t conn_id,
    ancs_client_notify_enable_t enable_flag)
{
    ble_ancs_service_t *remote = ancs_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        bts_sample_error_log("ancs client remote service is not exist, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (remote->notify_src_ccc_handle == 0) {
        bts_sample_error_log("ancs client remote service did not discovery notify ccc, client_id:%u "
            "conn_id:%u\n", client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    uint16_t ccc_value = BLE_ANCS_CLIENT_CHARACTERISTIC_CONFIGURATION_DEFAULT;
    if (enable_flag == ANCS_CLIENT_NOTIFY_ENABLE) {
        ccc_value = BLE_ANCS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION;
    }
    uint8_t notify_ccc_val[] = {
        u16_low_byte(ccc_value),
        u16_high_byte(ccc_value)
    };
    gattc_handle_value_t value = {0};
    value.data_len = (uint16_t)sizeof(notify_ccc_val);
    value.data = notify_ccc_val;
    value.handle = remote->notify_src_ccc_handle;
    return gattc_write_req(client_id, conn_id, &value);
}

/* ANCS客户端订阅数据源特征通知 */
errcode_t ble_ancs_client_subscribe_data_source(uint8_t client_id, uint16_t conn_id,
    ancs_client_notify_enable_t enable_flag)
{
    ble_ancs_service_t *remote = ancs_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        bts_sample_error_log("ancs client remote service is not exist, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (remote->data_src_ccc_handle == 0) {
        bts_sample_error_log("ancs client remote service did not discovery data ccc, client_id:%u "
            "conn_id:%u\n", client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    uint16_t ccc_value = BLE_ANCS_CLIENT_CHARACTERISTIC_CONFIGURATION_DEFAULT;
    if (enable_flag == ANCS_CLIENT_NOTIFY_ENABLE) {
        ccc_value = BLE_ANCS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION;
    }
    uint8_t notify_ccc_val[] = {
        u16_low_byte(ccc_value),
        u16_high_byte(ccc_value)
    };
    gattc_handle_value_t value = {0};
    value.data_len = (uint16_t)sizeof(notify_ccc_val);
    value.data = notify_ccc_val;
    value.handle = remote->data_src_ccc_handle;
    return gattc_write_req(client_id, conn_id, &value);
}

/* u32转小端序字节流 */
static uint32_t convert_little_2_u32(uint8_t *data)
{
    return little_byte_2_u32(data);
}

/* 释放【通知属性响应数据】申请的内存 */
static void notify_att_rsp_free(ble_ancs_notification_attr_rsp_t *notify_att_rsp)
{
    while (osal_list_empty(&(notify_att_rsp->attr_rsp_lst)) == false) {
        struct osal_list_head *node = notify_att_rsp->attr_rsp_lst.next;
        osal_list_del(node);
        bts_sample_mem_free(((ble_ancs_attr_rsp_t *)node)->att);
        bts_sample_mem_free(node);
    }
    return;
}

/* 组装属性响应数据，兼容分包场景，只解析第一包 */
static uint16_t ancs_att_rsp_package(ble_ancs_attr_rsp_t *attr_rsp,
    uint8_t *data, uint16_t total_length, uint16_t offset_byte)
{
    if ((total_length - offset_byte) <= ANCS_NOTIFY_ATT_VALUE_OFFSET) { /* 最后一个属性只包含属性id或者属性长度 */
        attr_rsp->att_id = *data;
        attr_rsp->att_length = 0;
        attr_rsp->att = 0;
        return 0;
    }
    attr_rsp->att_id = *data;
    uint16_t data_length = little_u8_2_u16(data + 1);
    attr_rsp->att_length = data_length + 1; /* 字符串以0结尾 */
    if (data_length != 0) {
        uint8_t *att_data = bts_sample_mem_new(attr_rsp->att_length);
        if (memset_s(att_data, attr_rsp->att_length, 0, attr_rsp->att_length) != EOK) {
            bts_sample_mem_free(att_data);
            attr_rsp->att_length = 0;
            attr_rsp->att = 0;
            return data_length;
        }
        uint16_t copy_length = data_length;
        if ((total_length - offset_byte) < (data_length + ANCS_NOTIFY_ATT_VALUE_OFFSET)) { /* 最后一个属性 */
            copy_length = total_length - offset_byte - ANCS_NOTIFY_ATT_VALUE_OFFSET;
        }
        if (memcpy_s(att_data, attr_rsp->att_length, data + ANCS_NOTIFY_ATT_VALUE_OFFSET, copy_length) != EOK) {
            bts_sample_error_log("[btsrv][ERROR] addr memcpy fail!\r\n");
            bts_sample_mem_free(att_data);
            attr_rsp->att_length = 0;
            attr_rsp->att = 0;
            return data_length;
        }
        att_data[attr_rsp->att_length - 1] = 0; /* 字符串以0结尾 */
        attr_rsp->att = att_data;
    } else {
        attr_rsp->att_length = 0;
        attr_rsp->att = 0;
    }
    return data_length;
}

/* 解析属性响应数据 */
static uint16_t parse_att_rsp(uint8_t *data, uint16_t data_length, struct osal_list_head *attr_rsp_lst)
{
    uint16_t offset_byte = 0;
    uint16_t att_num = 0;
    uint16_t att_data_len = 0;
    while (offset_byte < data_length) {
        ble_ancs_attr_rsp_t *attr_rsp = (ble_ancs_attr_rsp_t *)bts_sample_mem_new(sizeof(ble_ancs_attr_rsp_t));
        if (attr_rsp != NULL) {
            att_data_len = ancs_att_rsp_package(attr_rsp, data, data_length, offset_byte);
            att_num += 1;
            osal_list_add_tail((struct osal_list_head *)attr_rsp, attr_rsp_lst);
            offset_byte = (uint16_t)(offset_byte + 1 + sizeof(uint16_t) + att_data_len);
            data = data + 1 + sizeof(uint16_t) + att_data_len;
        }
    }
    return att_num;
}

/* 解析通知属性响应数据，响应数据格式：cmd_id(1B)|notify_uid(4B)|att_id1(1B)|att_id1_length(2B)|att_id1_data(动态)|... */
static void notify_att_data_parse(uint8_t client_id, uint16_t conn_id, uint8_t *data, uint16_t data_length)
{
    ble_ancs_notification_attr_rsp_t *notify_att_rsp = NULL;
    errcode_t result = ERRCODE_BT_SUCCESS;
    if (data_length <= (ANCS_NOTIFY_ATT_DATA_HEADER_LENGTH - 1)) {
        result = ERRCODE_BT_PARAM_ERR;
    } else {
        notify_att_rsp = bts_sample_mem_new(sizeof(ble_ancs_notification_attr_rsp_t));
        notify_att_rsp->notification_uid = convert_little_2_u32(data);
        notify_att_rsp->att_num = 0;
        notify_att_rsp->attr_rsp_lst.prev = &(notify_att_rsp->attr_rsp_lst);
        notify_att_rsp->attr_rsp_lst.next = &(notify_att_rsp->attr_rsp_lst);
        data = data + ANCS_NOTIFY_UUID_BYTE_LENGTH;
        notify_att_rsp->att_num = parse_att_rsp(data,
            data_length - ANCS_NOTIFY_UUID_BYTE_LENGTH, &notify_att_rsp->attr_rsp_lst);
    }

    if (g_ancs_callbacks_t.notify_attr_cb != NULL) {
        g_ancs_callbacks_t.notify_attr_cb(client_id, conn_id, notify_att_rsp, result);
    }

    if (notify_att_rsp != NULL) {
        notify_att_rsp_free(notify_att_rsp);
        bts_sample_mem_free(notify_att_rsp);
    }
}

/* 释放【应用属性响应结果】申请的内存 */
static void app_att_rsp_free(ble_ancs_app_attr_rsp_t *app_att_rsp)
{
    while (osal_list_empty(&(app_att_rsp->attr_rsp_lst)) == false) {
        struct osal_list_head *node = app_att_rsp->attr_rsp_lst.next;
        osal_list_del(node);
        bts_sample_mem_free(((ble_ancs_attr_rsp_t *)node)->att);
        bts_sample_mem_free(node);
    }

    if (app_att_rsp->app_id.id != NULL) {
        bts_sample_mem_free(app_att_rsp->app_id.id);
    }

    return;
}

/* 解析应用编号 */
static void app_id_parse(ble_ancs_app_attr_rsp_t *app_att_rsp, uint8_t *data, uint16_t data_length)
{
    uint16_t app_id_len = 0;
    for (int i = 0; i < data_length - 1; i++) {
        app_id_len = app_id_len + 1;
        if (*(data + i) == 0) {
            break;
        }
    }
    if (app_id_len != 0) {
        app_att_rsp->app_id.len = app_id_len;
        uint8_t *id = bts_sample_mem_new(app_id_len);
        if (id != NULL) {
            if (memcpy_s(id, app_id_len, data, app_id_len) != EOK) {
                bts_sample_mem_free(id);
                app_att_rsp->app_id.id = NULL;
                return;
            }
            app_att_rsp->app_id.id = id;
            return;
        }
    }
    app_att_rsp->app_id.len = 0;
    app_att_rsp->app_id.id = NULL;
}

/* 解析应用属性响应结果，响应格式：cmd_id(1B)|app_id(动态长度，字符串字节流，以NULL结尾)|att_id1(1B)|att_id1_length(2B)|att_id1_data(动态)|... */
static void app_att_data_parse(uint8_t client_id, uint16_t conn_id, uint8_t *data, uint16_t data_length)
{
    ble_ancs_app_attr_rsp_t *app_att_rsp = NULL;
    errcode_t result = ERRCODE_BT_SUCCESS;
    if (data_length <= 0) {
        result = ERRCODE_BT_PARAM_ERR;
    } else {
        app_att_rsp = bts_sample_mem_new(sizeof(ble_ancs_app_attr_rsp_t));
        app_id_parse(app_att_rsp, data, data_length);
        if ((app_att_rsp->app_id.len == 0) || (app_att_rsp->app_id.id == NULL)) {
            result = ERRCODE_BT_PARAM_ERR;
        } else {
            app_att_rsp->att_num = 0;
            app_att_rsp->attr_rsp_lst.prev = &(app_att_rsp->attr_rsp_lst);
            app_att_rsp->attr_rsp_lst.next = &(app_att_rsp->attr_rsp_lst);
            uint16_t app_id_len = app_att_rsp->app_id.len;
            data = data + app_id_len;
            app_att_rsp->att_num = parse_att_rsp(data, data_length - app_id_len, &app_att_rsp->attr_rsp_lst);
        }
    }

    if (g_ancs_callbacks_t.app_attr_cb != NULL) {
        g_ancs_callbacks_t.app_attr_cb(client_id, conn_id, app_att_rsp, result);
    }

    if (app_att_rsp != NULL) {
        app_att_rsp_free(app_att_rsp);
        bts_sample_mem_free(app_att_rsp);
    }
}

/* ANCS客户端通知消息处理函数，消息格式：event_id(1B)|event_flags(1B)|category_id(1B)|category_count(1B)|notify_uuid(4B) */
static void notify_src_handle(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data)
{
    errcode_t result = ERRCODE_BT_SUCCESS;
    ble_ancs_notification_src_t *notify_src_data = NULL;
    if (data->data_len != ANCS_NOTIFY_SOURCE_LENGTH) {
        result = ERRCODE_BT_PARAM_ERR;
    } else {
        uint8_t *pt_data = data->data;
        notify_src_data = bts_sample_mem_new(sizeof(ble_ancs_notification_src_t));
        notify_src_data->event_id = *pt_data;
        notify_src_data->event_flags = *(pt_data + BT_NUM1);
        notify_src_data->category_id = *(pt_data + BT_NUM2);
        notify_src_data->category_count = *(pt_data + BT_NUM3);
        pt_data = pt_data + BT_NUM4;
        notify_src_data->notification_uid = convert_little_2_u32(pt_data);
    }

    if (g_ancs_callbacks_t.notify_source_cb != NULL) {
        g_ancs_callbacks_t.notify_source_cb(client_id, conn_id, notify_src_data, result);
    }
    if (notify_src_data != NULL) {
        bts_sample_mem_free(notify_src_data);
    }
}

/* ANCS客户端数据消息处理函数 */
static void data_src_handle(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data)
{
    uint8_t *pt_data = data->data;
    uint16_t data_length = data->data_len;
    if ((data_length == (ble_gatt_mtu_exchange_result_get() - BLE_ATT_HEADER_LEN)) ||
        (data_length == BLE_ANCS_DATA_MAX_LEN)) {
        bts_sample_error_log("response is larger than MTU, please limit attribute length, \
            client_id:%u conn_id:%u\n", client_id, conn_id);
    }
    uint8_t cmd_id = *pt_data;
    pt_data = pt_data + 1;
    switch (cmd_id) {
        /* 数据格式：cmd_id(1B)|notify_uid(4B)|att_id1(1B)|att_id1_length(2B)|att_id1_data(动态)|... */
        case ANCS_CMD_GET_NOTIFY_ATT:{
            notify_att_data_parse(client_id, conn_id, pt_data, data->data_len - 1);
            break;
        }
        /* 数据格式：cmd_id(1B)|app_id(动态长度，字符串字节流，以NULL结尾)|att_id1(1B)|att_id1_length(2B)|att_id1_data(动态)|... */
        case ANCS_CMD_GET_APP_ATT:{
            app_att_data_parse(client_id, conn_id, pt_data, data->data_len - 1);
            break;
        }
        default:
            break;
    }
}

/* ANCS客户端接收对端通知处理函数 */
void ble_ancs_client_notify_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data)
{
    ble_ancs_service_t *remote = ancs_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        return;
    }

    /* notification source */
    /* 数据格式:event_id(1B)|event_flags(1B)|category_id(1B)|category_count(1B)|notification_uid(4B) */
    if (data->handle == remote->notify_src.value_handle) {
        notify_src_handle(client_id, conn_id, data);
    /* data source */
    } else if (data->handle == remote->data_src.value_handle) {
        data_src_handle(client_id, conn_id, data);
    }
}

/* ANCS客户端订阅结果处理函数 */
static void ble_ancs_client_subscribe_result(uint8_t client_id, uint16_t conn_id,
    ancs_client_subscribe_type_t type, gatt_status_t status)
{
    if (g_ancs_callbacks_t.subscribe_cb != NULL) {
        g_ancs_callbacks_t.subscribe_cb(client_id, conn_id, type, status);
    }
}

/* ANCS客户端写请求响应结果处理函数 */
static void ble_ancs_client_write_result(uint8_t client_id, uint16_t conn_id, gatt_status_t status)
{
    if (g_ancs_callbacks_t.write_result_cb != NULL) {
        g_ancs_callbacks_t.write_result_cb(client_id, conn_id, status);
    }
}

/* ANCS客户端写请求响应处理函数 */
void ble_ancs_client_write_cbk(uint8_t client_id, uint16_t conn_id, uint16_t handle, gatt_status_t status)
{
    ble_ancs_service_t *remote = ancs_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        return;
    }

    if (handle == (remote->notify_src_ccc_handle)) {
        ble_ancs_client_subscribe_result(client_id, conn_id, ANCS_CLIENT_SUBSCRIBE_NOTIFY, status);
    } else if (handle == (remote->data_src_ccc_handle)) {
        ble_ancs_client_subscribe_result(client_id, conn_id, ANCS_CLIENT_SUBSCRIBE_DATA, status);
    } else if (handle == (remote->ctl_point.value_handle)) {
        ble_ancs_client_write_result(client_id, conn_id, status);
    }
}

/* 校验通知属性是否需要拼接【最大长度】参数 */
static bool is_followed_by_max_length_param(uint8_t att_id)
{
    bool is_followed = false;
    switch (att_id) {
        case ANCS_NOTIFY_ATT_TITLE:
        case ANCS_NOTIFY_ATT_SUBTITLE:
        case ANCS_NOTIFY_ATT_MESSAGE:
            is_followed = true;
            break;
        default:
            break;
    }
    return is_followed;
}

/* u32转小端序 */
static void u32_2_little(uint32_t notify_uid, uint8_t *pt_data)
{
    *(pt_data + BT_NUM1) = u32_2_little_first_byte(notify_uid);
    *(pt_data + BT_NUM2) = u32_2_little_second_byte(notify_uid);
    *(pt_data + BT_NUM3) = u32_2_little_three_byte(notify_uid);
    *(pt_data + BT_NUM4) = u32_2_little_four_byte(notify_uid);
}

/* 组装通知属性请求参数 */
static errcode_t package_notify_att_req_param(ble_ancs_notification_attr_req_t *notify_att_req, uint8_t *pt_data)
{
    /* command id */
    *pt_data = ANCS_CMD_GET_NOTIFY_ATT;

    /* notification uuid 小端 */
    u32_2_little(notify_att_req->notification_uid, pt_data);

    /* 拼接ATT ID */
    pt_data = pt_data + ANCS_NOTIFY_ATT_ID_OFFSET;
    for (int i = 0; i < notify_att_req->att_num; i++) {
        /* 校验ATT ID */
        if (notify_att_req->attr[i].att_id > ANCS_NOTIFY_ATT_NEGATIVE_LABEL) {
            return ERRCODE_BT_PARAM_ERR;
        }
        *(pt_data) = notify_att_req->attr[i].att_id;
        pt_data = pt_data + 1;

        /* 属性带有最大长度参数,小端传输 */
        if (is_followed_by_max_length_param(notify_att_req->attr[i].att_id)) {
            *(pt_data) = u16_low_byte(notify_att_req->attr[i].att_max_length);
            *(pt_data + 1) = u16_high_byte(notify_att_req->attr[i].att_max_length);
            pt_data = pt_data + ANCS_NOTIFY_ATT_MAX_LENGTH_BYTE;
        }
    }
    return ERRCODE_BT_SUCCESS;
}

/* 计算申请内存大小 */
static uint16_t calc_notify_att_req_param_length(ble_ancs_notification_attr_req_t *notify_att_req)
{
    uint16_t data_len = (uint16_t)(sizeof(uint8_t) + sizeof(uint32_t) +
        (notify_att_req->att_num) * sizeof(uint8_t));
    for (int i = 0; i < notify_att_req->att_num; i++) {
        /* 属性带有最大长度参数 */
        if (is_followed_by_max_length_param(notify_att_req->attr[i].att_id)) {
            data_len += (uint16_t)sizeof(uint16_t);
        }
    }
    return data_len;
}

/* 发送获取通知属性命令 */
errcode_t ble_ancs_client_send_get_notify_att_cmd(uint8_t client_id, uint16_t conn_id,
    ble_ancs_notification_attr_req_t *notify_att_req)
{
    ble_ancs_service_t *remote = ancs_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        bts_sample_error_log("ancs client remote service is not exist, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (remote->ctl_point.value_handle == 0) {
        bts_sample_error_log("ancs client remote service did not discovery character, client_id:%u "
            "conn_id:%u\n", client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (notify_att_req->att_num == 0) {
        bts_sample_error_log("ancs client att num is invalid, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_PARAM_ERR;
    }

    /* 计算请求数据长度 */
    uint16_t data_len = calc_notify_att_req_param_length(notify_att_req);

    /* 请求数据格式：cmd_id(1B)|notify_uid(4B)|att_id1(1B)|att_id2(1B)|att_id2_max_length(2B)|... */
    uint8_t *data = (uint8_t *)bts_sample_mem_new(data_len);

    /* 指向参数数据的指针，用于拼接参数移动指针 */
    uint8_t *pt_data = data;
    errcode_t ret = package_notify_att_req_param(notify_att_req, pt_data);
    if (ret != ERRCODE_BT_SUCCESS) {
        bts_sample_mem_free(data);
        return ret;
    }
    gattc_handle_value_t value = {0};
    value.handle = remote->ctl_point.value_handle;
    value.data_len = data_len;
    value.data = data;
    errcode_t result = gattc_write_req(client_id, conn_id, &value);
    bts_sample_mem_free(data);
    return result;
}

static errcode_t package_app_att_req_param(ble_ancs_app_attr_req_t *app_att_req, uint8_t *pt_data)
{
    /* command id */
    *pt_data = ANCS_CMD_GET_APP_ATT;

    /* app id ,字符串 */
    pt_data = pt_data + 1;
    if (memcpy_s(pt_data, app_att_req->app_id.len, app_att_req->app_id.id, app_att_req->app_id.len) != EOK) {
        bts_sample_error_log("[btsrv][ERROR] app id memcpy fail!\r\n");
        return ERRCODE_BT_MALLOC_FAIL;
    }

    /* 校验 att id */
    pt_data = pt_data + app_att_req->app_id.len;
    for (int i = 0; i < app_att_req->att_num; i++) {
        if (app_att_req->attr_id[i] > ANCS_APP_ATT_DISPLAY_NAME) {
            return ERRCODE_BT_PARAM_ERR;
        }
        *(pt_data) = app_att_req->attr_id[i];
        pt_data = pt_data + 1;
    }

    return ERRCODE_BT_SUCCESS;
}

/* 发送获取应用属性命令 */
errcode_t ble_ancs_client_send_get_app_att_cmd(uint8_t client_id, uint16_t conn_id,
    ble_ancs_app_attr_req_t *app_att_req)
{
    ble_ancs_service_t *remote = ancs_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        bts_sample_error_log("ancs client remote service is not exist, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (remote->ctl_point.value_handle == 0) {
        bts_sample_error_log("ancs client remote service did not discovery character, client_id:%u "
            "conn_id:%u\n", client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (app_att_req->att_num == 0) {
        bts_sample_error_log("ancs client att num is invalid, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_PARAM_ERR;
    }

    if (app_att_req->app_id.len != strlen((char *)app_att_req->app_id.id) + 1) {
        bts_sample_error_log("[ANCSClient][AppIdentifier]string must be NULL-terminated!\r\n");
        return ERRCODE_BT_PARAM_ERR;
    }

    /* 计算数据长度 */
    uint16_t data_len = (uint16_t)(sizeof(uint8_t) + app_att_req->app_id.len +
        (app_att_req->att_num) * sizeof(uint8_t));
    
    /* 数据格式：cmd_id(1B)|app_id(动态)|att_id1(1B)|att_id2(1B)|... */
    uint8_t *data = (uint8_t *)bts_sample_mem_new(data_len);
    /* 指向参数数据的指针，用于拼接参数移动指针 */
    uint8_t *pt_data = data;
    errcode_t ret = package_app_att_req_param(app_att_req, pt_data);
    if (ret != ERRCODE_BT_SUCCESS) {
        bts_sample_mem_free(data);
        return ret;
    }

    gattc_handle_value_t value = {0};
    value.handle = remote->ctl_point.value_handle;
    value.data_len = data_len;
    value.data = data;
    errcode_t result = gattc_write_req(client_id, conn_id, &value);
    bts_sample_mem_free(data);
    return result;
}

/* 发送执行通知行为命令 */
errcode_t ble_ancs_client_send_perform_notify_action_cmd(uint8_t client_id, uint16_t conn_id,
    ble_ancs_notification_action_req_t *action_req)
{
    ble_ancs_service_t *remote = ancs_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        bts_sample_error_log("ancs client remote service is not exist, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (remote->ctl_point.value_handle == 0) {
        bts_sample_error_log("ancs client remote service did not discovery character, client_id:%u "
            "conn_id:%u\n", client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }
    if (action_req->action_id >= ANCS_ACTION_RESERVED) {
        bts_sample_error_log("ancs client action id is invalid, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_PARAM_ERR;
    }

    /* 计算数据长度 */
    uint16_t data_len = (uint16_t)(sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint8_t));
    
    /* 数据格式：cmd_id(1B)|notify_uid(4B)|action_id(1B) */
    uint8_t *data = (uint8_t *)bts_sample_mem_new(data_len);
    /* 指向参数数据的指针，用于拼接参数移动指针 */
    uint8_t *pt_data = data;

    /* command id */
    *pt_data = ANCS_CMD_PERFORM_NOTIFY_ACTION;

    /* notification uuid 小端 */
    u32_2_little(action_req->notification_uid, pt_data);
    *(pt_data + ANCS_NOTIFY_ACTION_ID_OFFSET) = action_req->action_id;

    gattc_handle_value_t value = {0};
    value.handle = remote->ctl_point.value_handle;
    value.data_len = data_len;
    value.data = data;
    errcode_t result = gattc_write_req(client_id, conn_id, &value);
    bts_sample_mem_free(data);
    return result;
}

/* 注册ANCS客户端回调函数 */
errcode_t ble_ancs_client_register_callbacks(ble_ancs_client_callbacks_t *func)
{
    if (memcpy_s(&g_ancs_callbacks_t, sizeof(g_ancs_callbacks_t), func, sizeof(ble_ancs_client_callbacks_t)) != EOK) {
        return ERRCODE_BT_MEMCPY_FAIL;
    }
    return ERRCODE_BT_SUCCESS;
}

/* ANCS服务判断 */
bool is_ble_ancs_service(bt_uuid_t *uuid)
{
    bt_uuid_t ancs_service_uuid = {0};
    ancs_service_uuid.uuid_len = BT_UUID_MAX_LEN;
    if (memcpy_s(ancs_service_uuid.uuid, BT_UUID_MAX_LEN, ancs_uuid, BT_UUID_MAX_LEN) != EOK) {
        bts_sample_error_log("[BLE]client discovery ancs memcpy error! \n");
        return false;
    }
    if (ancs_service_uuid.uuid_len != uuid->uuid_len) {
        return false;
    }

    return (memcmp(uuid, &ancs_service_uuid, sizeof(bt_uuid_t)) == 0 ? true : false);
}

/* 根据连接ID清理本地存储的对端ANCS服务信息 */
void ble_ancs_client_clear_remote_by_conn(uint8_t client_id, uint16_t conn_id)
{
    ble_ancs_service_t *node = NULL;
    node = ancs_service_remote_by_conn(client_id, conn_id);
    if (node != NULL) {
        osal_list_del((struct osal_list_head *)node);
        bts_sample_mem_free(node);
    }
}

/* 清理本地存储的所有对端ANCS服务信息 */
void ble_ancs_client_clear_remote_all(void)
{
    while (osal_list_empty(&g_ble_ancs_service_list) == false) {
        struct osal_list_head *node = g_ble_ancs_service_list.next;
        osal_list_del(node);
        bts_sample_mem_free(node);
    }
}
