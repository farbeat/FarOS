/*
 * Copyright (c) @CompanyNameMagicTag 2024. All rights reserved.
 * Description: BLE Apple Media Service Client module.
 */

#include <string.h>
#include "errcode.h"
#include "osal_addr.h"
#include "sample_common.h"
#include "bts_le_gap.h"
#include "ble_ams_client.h"

#define BLE_CLIENT_CHARACTERISTIC_CONFIGURATION_UUID 0x2902
#define AMS_ENTITY_UPDATE_NOTIFY_VALUE_OFFSET 3
#define AMS_ENTITY_UPDATE_NOTIFY_FLAGS_OFFSET 2
#define AMS_STRING_SUFFIX_ZERO_BYTE 1

#define u16_low_byte(uuid) ((uint8_t)((uuid) & 0x00FF))
#define u16_high_byte(uuid) ((uint8_t)(((uuid) & 0xFF00) >> 8))

/* AMS服务UUID，小端序 */
uint8_t ams_uuid[BT_UUID_MAX_LEN] = {
    0xDC, 0xF8, 0x55, 0xAD, 0x02, 0xC5, 0xF4, 0x8E, 0x3A, 0x43, 0x36, 0x0F, 0x2B, 0x50, 0xD3, 0x89};

/* AMS服务特征UUID-Remote Command，小端序 */
uint8_t rmt_cmd_uuid[BT_UUID_MAX_LEN] = {
    0xC2, 0x51, 0xCA, 0xF7, 0x56, 0x0E, 0xDF, 0xB8, 0x8A, 0x4A, 0xB1, 0x57, 0xD8, 0x81, 0x3C, 0x9B};

/* AMS服务特征UUID-Entity Update，小端序 */
uint8_t entity_update_uuid[BT_UUID_MAX_LEN] = {
    0x02, 0xC1, 0x96, 0xBA, 0x92, 0xBB, 0x0C, 0x9A, 0x1F, 0x41, 0x8D, 0x80, 0xCE, 0xAB, 0x7C, 0x2F};

/* AMS服务特征UUID-Entity Attribute，小端序 */
uint8_t entity_att_uuid[BT_UUID_MAX_LEN] = {
    0xD7, 0xD5, 0xBB, 0x70, 0xA8, 0xA3, 0xAB, 0xA6, 0xD8, 0x46, 0xAB, 0x23, 0x8C, 0xF3, 0xB2, 0xC6};

/* AMS客户端回调函数结构体 */
ble_ams_client_callbacks_t g_ams_callbacks_t = {0};

/* 对端AMS服务信息 */
OSAL_LIST_HEAD(g_ble_ams_service_list);

/* 根据连接ID查询存储的对端AMS服务信息 */
static ble_ams_service_t* ams_service_remote_by_conn(uint8_t client_id, uint16_t conn_id)
{
    struct osal_list_head *list_head = &g_ble_ams_service_list;
    struct osal_list_head *node = list_head->next;
    while (node != NULL && node != list_head) {
        ble_ams_service_t *svc_node = (ble_ams_service_t *)node;
        if ((svc_node->client_id == client_id) && (svc_node->conn_id == conn_id)) {
            return svc_node;
        }
        node = node->next;
    }

    return NULL;
}

/* AMS客户端发起服务建立 */
errcode_t ble_ams_client_setup_service(uint8_t client_id, uint16_t conn_id)
{
    ble_ams_service_t *remote = ams_service_remote_by_conn(client_id, conn_id);
    if (remote != NULL) {
        bts_sample_error_log("ams client remote service is already exist,client_id:%u, conn_id:%u.\r\n",
            client_id, conn_id);
        return ERRCODE_BT_DONE;
    }

    bt_uuid_t service_uuid = {0};
    service_uuid.uuid_len = BT_UUID_MAX_LEN;
    if (memcpy_s(service_uuid.uuid, BT_UUID_MAX_LEN, ams_uuid, BT_UUID_MAX_LEN) != EOK) {
        bts_sample_error_log("[btsrv][ERROR] uuid memcpy fail!\r\n");
        return ERRCODE_BT_FAIL;
    }
    return gattc_discovery_service(client_id, conn_id, &service_uuid);
}

/* 发现服务回调函数 */
void ble_ams_client_discovery_service_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_service_result_t *service)
{
    ble_ams_service_t *remote_svc = ams_service_remote_by_conn(client_id, conn_id);
    if (remote_svc != NULL) {
        bts_sample_error_log("ams client remote service is already exist,client_id:%u, conn_id:%u.\r\n",
            client_id, conn_id);
        return;
    }
    remote_svc = (ble_ams_service_t *)bts_sample_mem_new(sizeof(ble_ams_service_t));
    if (remote_svc == NULL) {
        bts_sample_error_log("ams client remote node create fail.\r\n");
        return;
    }
    remote_svc->client_id = client_id;
    remote_svc->conn_id = conn_id;
    remote_svc->start_handle = service->start_hdl;
    remote_svc->end_handle = service->end_hdl;
    osal_list_add_tail((struct osal_list_head *)remote_svc, &g_ble_ams_service_list);

    /* 发现特征 */
    gattc_discovery_character_param_t param = {0};
    param.service_handle = remote_svc->start_handle;
    errcode_t ret = gattc_discovery_character(client_id, conn_id, &param);
    bts_sample_info_log("ams client discovery character ret:%u.\r\n", ret);
}

/* AMS服务特征匹配 */
static ams_character_t ams_char_match(bt_uuid_t *uuid)
{
    if (uuid->uuid_len != BT_UUID_MAX_LEN) {
        return AMS_CHAR_INVALID;
    }
    bt_uuid_t char_uuid = {0};
    char_uuid.uuid_len = BT_UUID_MAX_LEN;
    if (memcpy_s(char_uuid.uuid, BT_UUID_MAX_LEN, rmt_cmd_uuid, BT_UUID_MAX_LEN) != EOK) {
        bts_sample_error_log("[btsrv][ERROR] remote cmd uuid memcpy fail!\r\n");
        return AMS_CHAR_INVALID;
    }
    if (memcmp(&char_uuid, uuid, sizeof(bt_uuid_t)) == 0) {
        return AMS_CHAR_REMOTE_CMD;
    }
    if (memcpy_s(char_uuid.uuid, BT_UUID_MAX_LEN, entity_update_uuid, BT_UUID_MAX_LEN) != EOK) {
        bts_sample_error_log("[btsrv][ERROR] entity update uuid memcpy fail!\r\n");
        return AMS_CHAR_INVALID;
    }
    if (memcmp(&char_uuid, uuid, sizeof(bt_uuid_t)) == 0) {
        return AMS_CHAR_ENTITY_UPDATE;
    }
    if (memcpy_s(char_uuid.uuid, BT_UUID_MAX_LEN, entity_att_uuid, BT_UUID_MAX_LEN) != EOK) {
        bts_sample_error_log("[btsrv][ERROR] entity att uuid memcpy fail!\r\n");
        return AMS_CHAR_INVALID;
    }
    if (memcmp(&char_uuid, uuid, sizeof(bt_uuid_t)) == 0) {
        return AMS_CHAR_ENTITY_ATTR;
    }
    return AMS_CHAR_INVALID;
}

/* 发现特征回调函数 */
void ble_ams_client_discovery_character_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_result_t *character)
{
    ble_ams_service_t *remote = ams_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        return;
    }

    if ((character->declare_handle < remote->start_handle) ||
        (character->declare_handle > remote->end_handle)) {
        bts_sample_error_log("ams client remote service did not have this character_handle:%u conn_id:%u\n",
            character->declare_handle, conn_id);
        return;
    }

    switch (ams_char_match(&(character->uuid))) {
        case AMS_CHAR_REMOTE_CMD: {
            remote->remote_cmd.declare_handle = character->declare_handle;
            remote->remote_cmd.value_handle = character->value_handle;
            remote->remote_cmd.properties = character->properties;

            /* 发现描述符 */
            gattc_discovery_descriptor(client_id, conn_id, character->declare_handle);
            break;
        }

        case AMS_CHAR_ENTITY_UPDATE: {
            remote->entity_update.declare_handle = character->declare_handle;
            remote->entity_update.value_handle = character->value_handle;
            remote->entity_update.properties = character->properties;

            /* 发现描述符 */
            gattc_discovery_descriptor(client_id, conn_id, character->declare_handle);
            break;
        }

        case AMS_CHAR_ENTITY_ATTR: {
            remote->entity_attr.declare_handle = character->declare_handle;
            remote->entity_attr.value_handle = character->value_handle;
            remote->entity_attr.properties = character->properties;
            break;
        }

        default:
            bts_sample_error_log("[btsrv][INFO] invalid character!\r\n");
            break;
    }
}

/* 发现描述符回调函数 */
void ble_ams_client_discovery_descriptor_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_descriptor_result_t *descriptor)
{
    ble_ams_service_t *remote = ams_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        return;
    }

    if (descriptor->descriptor_hdl < remote->start_handle ||
        descriptor->descriptor_hdl > remote->end_handle) {
        bts_sample_error_log("ams client remote service did not have this descriptor_handle:%u conn_id:%u\n",
            descriptor->descriptor_hdl, conn_id);
        return;
    }

    /* 客户端特征配置描述符 */
    bt_uuid_t ccc_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_CLIENT_CHARACTERISTIC_CONFIGURATION_UUID, &ccc_uuid);
    if (!bts_sample_bt_gatts_cmp_uuid(&ccc_uuid, &(descriptor->uuid))) {
        return;
    }

    uint16_t cmd_ccc_max_handle = remote->end_handle;
    uint16_t entity_ccc_max_handle = remote->end_handle;

    if ((remote->entity_update.value_handle != 0) &&
        (remote->remote_cmd.value_handle != 0) &&
        (remote->remote_cmd.value_handle < remote->entity_update.value_handle)) {
        cmd_ccc_max_handle = remote->entity_update.value_handle - 1;
    } else if ((remote->remote_cmd.value_handle != 0) &&
        (remote->entity_update.value_handle != 0) &&
        (remote->remote_cmd.value_handle > remote->entity_update.value_handle)) {
        entity_ccc_max_handle = remote->remote_cmd.value_handle - 1;
    }

    if ((remote->remote_cmd.value_handle != 0) &&
        (descriptor->descriptor_hdl > remote->remote_cmd.value_handle) &&
        (descriptor->descriptor_hdl <= cmd_ccc_max_handle)) {
        remote->remote_cmd_ccc_handle = descriptor->descriptor_hdl;
    } else if ((remote->entity_update.value_handle != 0) &&
        (descriptor->descriptor_hdl > remote->entity_update.value_handle) &&
        (descriptor->descriptor_hdl <= entity_ccc_max_handle)) {
        remote->entity_update_ccc_handle = descriptor->descriptor_hdl;
    }

    if ((remote->remote_cmd_ccc_handle != 0) &&
        (remote->entity_update_ccc_handle != 0)) {
        if (g_ams_callbacks_t.setup_cb != NULL) {
            g_ams_callbacks_t.setup_cb(client_id, conn_id, ERRCODE_BT_SUCCESS);
        }
    }
}

/* 处理远端命令上报信息 */
static void remote_cmd_rpt_handle(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data)
{
    errcode_t status = ERRCODE_BT_SUCCESS;
    ble_ams_remote_cmd_rpt_t *rpt_data = bts_sample_mem_new(sizeof(ble_ams_remote_cmd_rpt_t) + data->data_len);
    rpt_data->cmd_num = data->data_len;
    if (memcpy_s(rpt_data->remote_cmd, rpt_data->cmd_num, data->data, rpt_data->cmd_num) != EOK) {
        bts_sample_error_log("[btsrv][ERROR] remote cmd rpt memcpy fail!\r\n");
        status = ERRCODE_BT_MEMCPY_FAIL;
    }

    if (g_ams_callbacks_t.remote_cmd_cb != NULL) {
        g_ams_callbacks_t.remote_cmd_cb(client_id, conn_id, rpt_data, status);
    }
    bts_sample_mem_free(rpt_data);
}

/* 处理实体更新上报信息 */
static void entity_update_rpt_handle(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data)
{
    errcode_t status = ERRCODE_BT_SUCCESS;
    uint16_t value_len = data->data_len - AMS_ENTITY_UPDATE_NOTIFY_VALUE_OFFSET;
    ble_ams_entity_update_rpt_t *rpt_data = bts_sample_mem_new(
        sizeof(ble_ams_entity_update_rpt_t) + value_len + AMS_STRING_SUFFIX_ZERO_BYTE);
    rpt_data->entity_id = *(data->data);
    rpt_data->att_id = *(data->data + sizeof(uint8_t));
    rpt_data->entity_update_flags = *(data->data + AMS_ENTITY_UPDATE_NOTIFY_FLAGS_OFFSET);
    rpt_data->value_length = value_len + AMS_STRING_SUFFIX_ZERO_BYTE;
    if (memcpy_s(rpt_data->value, rpt_data->value_length,
        (data->data + AMS_ENTITY_UPDATE_NOTIFY_VALUE_OFFSET), value_len) != EOK) {
        bts_sample_error_log("[btsrv][ERROR] entity update rpt memcpy fail!\r\n");
        status = ERRCODE_BT_MEMCPY_FAIL;
    }
    rpt_data->value[rpt_data->value_length - 1] = 0; /* 字符串拼接末尾0 */

    if (g_ams_callbacks_t.entity_update_cb != NULL) {
        g_ams_callbacks_t.entity_update_cb(client_id, conn_id, rpt_data, status);
    }
    bts_sample_mem_free(rpt_data);
}

/* 特征通知回调函数 */
void ble_ams_client_notify_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data)
{
    ble_ams_service_t *remote = ams_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        return;
    }

    /* Remote Command */
    /* 数据格式:RemoteCommandID 1(1B)|RemoteCommandID 2(1B)|... */
    if (data->handle == remote->remote_cmd.value_handle) {
        remote_cmd_rpt_handle(client_id, conn_id, data);

    /* Entity Update */
    /* 数据格式:EntityID(1B)|AttributeID(1B)|EntityUpdateFlags(1B)|Value（动态） */
    } else if (data->handle == remote->entity_update.value_handle) {
        entity_update_rpt_handle(client_id, conn_id, data);
    }
}

/* AMS客户端订阅结果处理函数 */
static void ble_ams_client_subscribe_result(uint8_t client_id, uint16_t conn_id,
    ams_client_subscribe_type_t type, gatt_status_t status)
{
    if (g_ams_callbacks_t.subscribe_cb != NULL) {
        g_ams_callbacks_t.subscribe_cb(client_id, conn_id, type, status);
    }
}

/* 注册感兴趣实体属性响应结果处理 */
static void ble_ams_client_write_character_rsp(uint8_t client_id, uint16_t conn_id,
    ams_client_write_character_type_t type,
    gatt_status_t status)
{
    if (g_ams_callbacks_t.write_rsp_cb != NULL) {
        g_ams_callbacks_t.write_rsp_cb(client_id, conn_id, type, status);
    }
}

/* 写请求响应回调函数 */
void ble_ams_client_write_cbk(uint8_t client_id, uint16_t conn_id, uint16_t handle, gatt_status_t status)
{
    ble_ams_service_t *remote = ams_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        return;
    }

    if (handle == (remote->remote_cmd_ccc_handle)) {
        ble_ams_client_subscribe_result(client_id, conn_id, AMS_CLIENT_SUBSCRIBE_REMOTE_CMD, status);
    } else if (handle == (remote->entity_update_ccc_handle)) {
        ble_ams_client_subscribe_result(client_id, conn_id, AMS_CLIENT_SUBSCRIBE_ENTITY_UPDATE, status);
    } else if (handle == (remote->entity_update.value_handle)) {
        ble_ams_client_write_character_rsp(client_id, conn_id, AMS_CLIENT_WRITE_ENTITY_UPDATE, status);
    } else if (handle == (remote->remote_cmd.value_handle)) {
        ble_ams_client_write_character_rsp(client_id, conn_id, AMS_CLIENT_WRITE_REMOTE_CMD, status);
    } else if (handle == (remote->entity_attr.value_handle)) {
        ble_ams_client_write_character_rsp(client_id, conn_id, AMS_CLIENT_WRITE_ENTITY_ATTRIBUTE, status);
    }
}

/* 读请求响应回调函数 */
void ble_ams_client_read_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *read_result,
    gatt_status_t status)
{
    ble_ams_service_t *remote = ams_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        return;
    }

    if (read_result->handle != remote->entity_attr.value_handle) {
        return;
    }

    ble_ams_full_attr_value_t *full_value =
        bts_sample_mem_new(sizeof(ble_ams_full_attr_value_t) + read_result->data_len + 1);
    full_value->value_len = read_result->data_len + 1;

    if (read_result->data_len != 0) {
        if (memcpy_s(full_value->value, full_value->value_len, read_result->data, read_result->data_len) != EOK) {
            bts_sample_error_log("[btsrv][ERROR] read result memcpy fail!\r\n");
            bts_sample_mem_free(full_value);
            return;
        }
    }
    *(full_value->value + full_value->value_len - 1) = 0; /* 字符串末尾拼接0 */
    if (g_ams_callbacks_t.read_full_attr_result_cb != NULL) {
        g_ams_callbacks_t.read_full_attr_result_cb(client_id, conn_id, full_value, status);
    }
    bts_sample_mem_free(full_value);
}

/* 订阅远端命令特征通知 */
errcode_t ble_ams_client_subscribe_remote_cmd(uint8_t client_id, uint16_t conn_id,
    ams_client_notify_enable_t enable_flag)
{
    ble_ams_service_t *remote = ams_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        bts_sample_error_log("ams client remote service is not exist, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (remote->remote_cmd_ccc_handle == 0) {
        bts_sample_error_log("ams client remote service did not discovery notify ccc, client_id:%u "
            "conn_id:%u\n", client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    uint16_t ccc_value = BLE_AMS_CLIENT_CHARACTERISTIC_CONFIGURATION_DEFAULT;
    if (enable_flag == AMS_CLIENT_NOTIFY_ENABLE) {
        ccc_value = BLE_AMS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION;
    }
    uint8_t notify_ccc_val[] = {
        u16_low_byte(ccc_value),
        u16_high_byte(ccc_value)
    };
    gattc_handle_value_t value = {0};
    value.data_len = (uint16_t)sizeof(notify_ccc_val);
    value.data = notify_ccc_val;
    value.handle = remote->remote_cmd_ccc_handle;
    return gattc_write_req(client_id, conn_id, &value);
}

/* 订阅实体更新特征通知 */
errcode_t ble_ams_client_subscribe_entity_update(uint8_t client_id, uint16_t conn_id,
    ams_client_notify_enable_t enable_flag)
{
    ble_ams_service_t *remote = ams_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        bts_sample_error_log("ams client remote service is not exist, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (remote->entity_update_ccc_handle == 0) {
        bts_sample_error_log("ams client remote service did not discovery notify ccc, client_id:%u "
            "conn_id:%u\n", client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    uint16_t ccc_value = BLE_AMS_CLIENT_CHARACTERISTIC_CONFIGURATION_DEFAULT;
    if (enable_flag == AMS_CLIENT_NOTIFY_ENABLE) {
        ccc_value = BLE_AMS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION;
    }
    uint8_t notify_ccc_val[] = {
        u16_low_byte(ccc_value),
        u16_high_byte(ccc_value)
    };
    gattc_handle_value_t value = {0};
    value.data_len = (uint16_t)sizeof(notify_ccc_val);
    value.data = notify_ccc_val;
    value.handle = remote->entity_update_ccc_handle;
    return gattc_write_req(client_id, conn_id, &value);
}

/* 发送远端命令 */
errcode_t ble_ams_client_send_remote_cmd(uint8_t client_id, uint16_t conn_id,
    ams_remote_cmd_id_t cmd_id)
{
    ble_ams_service_t *remote = ams_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        bts_sample_error_log("ams client remote service is not exist, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (remote->remote_cmd.value_handle == 0) {
        bts_sample_error_log("ams client did not discovery character, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    uint8_t data = (uint8_t)cmd_id;
    gattc_handle_value_t value = {0};
    value.data_len = (uint16_t)sizeof(uint8_t);
    value.data = &data;
    value.handle = remote->remote_cmd.value_handle;
    return gattc_write_req(client_id, conn_id, &value);
}

/* 注册感兴趣的实体属性 */
errcode_t ble_ams_client_register_interested_entity_attribute(uint8_t client_id, uint16_t conn_id,
    ble_ams_interested_entity_attr_req_t *entity_attr_req)
{
    ble_ams_service_t *remote = ams_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        bts_sample_error_log("ams client remote service is not exist, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (remote->entity_update.value_handle == 0) {
        bts_sample_error_log("ams client did not discovery character, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (entity_attr_req->att_num == 0) {
        bts_sample_error_log("ams client attr req param error, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_PARAM_ERR;
    }

    uint16_t data_length = (uint16_t)(sizeof(uint8_t) + (entity_attr_req->att_num) * sizeof(uint8_t));
    uint8_t *data = bts_sample_mem_new(data_length);
    uint8_t *pt_data = data;
    *pt_data = entity_attr_req->entity_id;
    pt_data = pt_data + 1;
    for (int i = 0; i < entity_attr_req->att_num; i++) {
        *(pt_data + i) = entity_attr_req->att_id[i];
    }

    gattc_handle_value_t value = {0};
    value.data_len = data_length;
    value.data = data;
    value.handle = remote->entity_update.value_handle;
    errcode_t result = gattc_write_req(client_id, conn_id, &value);
    bts_sample_mem_free(data);
    return result;
}

/* 获取完整的实体属性 */
errcode_t ble_ams_client_retrieve_full_entity_attribute(uint8_t client_id, uint16_t conn_id,
    ble_ams_full_entity_attr_cmd_t *full_attr_req)
{
    ble_ams_service_t *remote = ams_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        bts_sample_error_log("ams client remote service is not exist, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (remote->entity_attr.value_handle == 0) {
        bts_sample_error_log("ams client did not discovery character, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    gattc_handle_value_t value = {0};
    value.data_len = (uint16_t)sizeof(ble_ams_full_entity_attr_cmd_t);
    value.data = (uint8_t *)full_attr_req;
    value.handle = remote->entity_attr.value_handle;
    return gattc_write_req(client_id, conn_id, &value);
}

/* 读取完整的实体属性 */
errcode_t ble_ams_client_read_full_entity_attribute(uint8_t client_id, uint16_t conn_id)
{
    ble_ams_service_t *remote = ams_service_remote_by_conn(client_id, conn_id);
    if (remote == NULL) {
        bts_sample_error_log("ams client remote service is not exist, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }

    if (remote->entity_attr.value_handle == 0) {
        bts_sample_error_log("ams client did not discovery character, client_id:%u conn_id:%u\n",
            client_id, conn_id);
        return ERRCODE_BT_NOT_READY;
    }
    return gattc_read_req_by_handle(client_id, conn_id, remote->entity_attr.value_handle);
}

/* 注册AMS客户端回调函数 */
errcode_t ble_ams_client_register_callbacks(ble_ams_client_callbacks_t *func)
{
    if (memcpy_s(&g_ams_callbacks_t, sizeof(g_ams_callbacks_t), func, sizeof(ble_ams_client_callbacks_t)) != EOK) {
        return ERRCODE_BT_MEMCPY_FAIL;
    }
    return ERRCODE_BT_SUCCESS;
}

/* AMS服务判断 */
bool is_ble_ams_service(bt_uuid_t *uuid)
{
    bt_uuid_t ams_service_uuid = {0};
    ams_service_uuid.uuid_len = BT_UUID_MAX_LEN;
    if (memcpy_s(ams_service_uuid.uuid, BT_UUID_MAX_LEN, ams_uuid, BT_UUID_MAX_LEN) != EOK) {
        bts_sample_error_log("[BLE]service uuid memcpy error! \n");
        return false;
    }
    if (ams_service_uuid.uuid_len != uuid->uuid_len) {
        return false;
    }

    return (memcmp(uuid, &ams_service_uuid, sizeof(bt_uuid_t)) == 0 ? true : false);
}

/* 根据连接ID清理存储的对端AMS服务信息 */
void ble_ams_client_clear_remote_by_conn(uint8_t client_id, uint16_t conn_id)
{
    ble_ams_service_t *node = NULL;
    node = ams_service_remote_by_conn(client_id, conn_id);
    if (node != NULL) {
        osal_list_del((struct osal_list_head *)node);
        bts_sample_mem_free(node);
    }
}

/* 清理所有存储的对端AMS服务信息 */
void ble_ams_client_clear_remote_all(void)
{
    while (osal_list_empty(&g_ble_ams_service_list) == false) {
        struct osal_list_head *node = g_ble_ams_service_list.next;
        osal_list_del(node);
        bts_sample_mem_free(node);
    }
}
