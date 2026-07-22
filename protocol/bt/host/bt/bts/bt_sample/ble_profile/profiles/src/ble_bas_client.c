/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BT Battery Service Profile Client Module.
 */
#include <string.h>
#include "errcode.h"
#include "osal_addr.h"
#include "sample_common.h"
#include "bts_le_gap.h"
#include "ble_bas_client.h"

#define bas_16_low_byte(uuid) ((uint8_t)((uuid) & 0x00FF))
#define bas_16_high_byte(uuid) ((uint8_t)(((uuid) & 0xFF00) >> 8))
#define bas_byte_to_16(low_byte, high_byte) (((uint16_t)(*(low_byte))) | (((uint16_t)(*(high_byte))) << 8))
#define BLE_BAS_CFD_DESC_LENGTH 7
#define BLE_BAS_CFD_DESC_EXPONENT_IDX 1
#define BLE_BAS_CFD_DESC_UNIT_LOW_IDX 2
#define BLE_BAS_CFD_DESC_UNIT_HIGH_IDX 3
#define BLE_BAS_CFD_DESC_NAME_SPACE_IDX 4
#define BLE_BAS_CFD_DESC_DESCRIPTION_LOW_IDX 5
#define BLE_BAS_CFD_DESC_DESCRIPTION_HIGH_IDX 6

OSAL_LIST_HEAD(g_ble_bas_client_remote_list); /* ble_bas_client_t */

ble_bas_func_result_cbk_t g_discovery_result_t = NULL;

static ble_bas_client_t* bas_client_get_dev_by_conn(uint8_t client_id, uint16_t conn_id)
{
    struct osal_list_head *list_head = &g_ble_bas_client_remote_list;
    struct osal_list_head *node = list_head->next;
    while (node != NULL && node != list_head) {
        ble_bas_client_t *svc_node = (ble_bas_client_t *)node;
        if ((svc_node->client_id == client_id) && (svc_node->conn_id == conn_id)) {
            return svc_node;
        }
        node = node->next;
    }

    return NULL;
}

/* 读特征格式描述符回调处理 */
static void ble_bas_client_read_cfd_descriptor_cbk(ble_bas_client_t *dev, uint16_t len, uint8_t *value)
{
    if (len != BLE_BAS_CFD_DESC_LENGTH) {
        bts_sample_error_log("[ACore]bas client read descriptor cbk cfd length error.\r\n");
        return;
    }
    dev->character.cfd_info.cfd.format = *value;
    dev->character.cfd_info.cfd.exponent = *(value + BLE_BAS_CFD_DESC_EXPONENT_IDX);
    dev->character.cfd_info.cfd.unit.uuid_len = 2;    // 2: short uuid length
    dev->character.cfd_info.cfd.unit.uuid[0] = *(value + BLE_BAS_CFD_DESC_UNIT_LOW_IDX);
    dev->character.cfd_info.cfd.unit.uuid[1] = *(value + BLE_BAS_CFD_DESC_UNIT_HIGH_IDX);
    dev->character.cfd_info.cfd.name_space = *(value + BLE_BAS_CFD_DESC_NAME_SPACE_IDX);
    dev->character.cfd_info.cfd.description.uuid_len = 2;    // 2: short uuid length
    dev->character.cfd_info.cfd.description.uuid[0] = *(value + BLE_BAS_CFD_DESC_DESCRIPTION_LOW_IDX);
    dev->character.cfd_info.cfd.description.uuid[1] = *(value + BLE_BAS_CFD_DESC_DESCRIPTION_HIGH_IDX);
    if (dev->character.cfd_info.cbk != NULL) {
        dev->character.cfd_info.cbk(dev->character.cfd_info.cfd);
    }
}

/* 读客户端特征配置描述符回调处理 */
static void ble_bas_client_read_ccc_descriptor_cbk(ble_bas_client_t *dev, uint16_t len, uint8_t *value)
{
    bts_unused(len);
    uint16_t ccd_value = BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_DEFAULT;
    if ((*value) == 1) {
        ccd_value = BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION;
        dev->character.cccd_info.value = BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION;
    }
    dev->character.cccd_info.value = ccd_value;
    if (dev->character.cccd_info.cbk != NULL) {
        dev->character.cccd_info.cbk(ccd_value);
    }
}

void ble_bas_client_clear_remote_by_conn(uint8_t client_id, uint16_t conn_id)
{
    ble_bas_client_t *node = NULL;
    node = bas_client_get_dev_by_conn(client_id, conn_id);
    if (node != NULL) {
        osal_list_del(node);
        bts_sample_mem_free(node);
    }
}

void ble_bas_client_clear_remote_all(void)
{
    while (osal_list_empty(&g_ble_bas_client_remote_list) == false) {
        struct osal_list_head *node = g_ble_bas_client_remote_list.next;
        osal_list_del(node);
        bts_sample_mem_free(node);
    }
}

errcode_t ble_bas_client_discovery_service(uint8_t client_id, uint16_t conn_id, ble_bas_func_result_cbk_t cbk)
{
    ble_bas_client_t *dev = bas_client_get_dev_by_conn(client_id, conn_id);
    if (dev != NULL) {
        bts_sample_error_log("bas client remote service is already exist,client_id:%u, conn_id:%hu.\r\n",
            client_id, conn_id);
        return ERRCODE_BT_FAIL;
    }
    g_discovery_result_t = cbk;
    bt_uuid_t bas_service_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_BAS_SERVICE_UUID, &bas_service_uuid);
    return gattc_discovery_service(client_id, conn_id, &bas_service_uuid);
}

errcode_t ble_bas_client_read_battery_level(uint8_t client_id, uint16_t conn_id,
    ble_bas_character_read_cbk_t read_cbk, ble_bas_notify_cbk_t notify_cbk)
{
    ble_bas_client_t *dev = bas_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("bas client remote service is not exist,client_id:%u, conn_id:%hu.\r\n",
            client_id, conn_id);
        return ERRCODE_BT_FAIL;
    }
    if (dev->character.value_handle == 0) {
        bts_sample_error_log("bas client did not discovery remote service. \r\n");
        return ERRCODE_BT_NOT_READY;
    }
    if ((dev->character.properties & GATT_CHARACTER_PROPERTY_BIT_READ) == 0) {
        bts_sample_error_log("bas client remote battery level is not readable.");
        return ERRCODE_BT_UNSUPPORTED;
    }
    dev->character.read_cbk = read_cbk;
    dev->character.notify_cbk = notify_cbk;
    return gattc_read_req_by_handle(client_id, conn_id, dev->character.value_handle);
}

errcode_t ble_bas_client_read_format_descriptor(uint8_t client_id, uint16_t conn_id,
    ble_bas_cfd_read_cbk_t cfd_cbk)
{
    ble_bas_client_t *dev = bas_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("bas client remote service is not exist,client_id:%u, conn_id:%hu.\r\n",
            client_id, conn_id);
        return ERRCODE_BT_FAIL;
    }
    if (dev->character.cfd_info.descriptor_handle == 0) {
        bts_sample_error_log("bas client remote cfd is not exist. \r\n");
        return ERRCODE_BT_NOT_READY;
    }
    dev->character.cfd_info.cbk = cfd_cbk;
    return gattc_read_req_by_handle(client_id, conn_id, dev->character.cfd_info.descriptor_handle);
}

errcode_t ble_bas_client_read_ccc_descriptor(uint8_t client_id, uint16_t conn_id,
    ble_bas_cccd_read_cbk_t cccd_cbk)
{
    ble_bas_client_t *dev = bas_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("bas client remote service is not exist,client_id:%u, conn_id:%hu.\r\n",
            client_id, conn_id);
        return ERRCODE_BT_FAIL;
    }
    if (dev->character.cccd_info.descriptor_handle == 0) {
        bts_sample_error_log("bas client remote cccd is not exist. \r\n");
        return ERRCODE_BT_NOT_READY;
    }
    dev->character.cccd_info.cbk = cccd_cbk;
    return gattc_read_req_by_handle(client_id, conn_id, dev->character.cccd_info.descriptor_handle);
}

errcode_t ble_bas_client_write_ccc_descriptor(uint8_t client_id, uint16_t conn_id,
    bas_client_cccd_notify_enable_t enable_flag)
{
    ble_bas_client_t *dev = bas_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("bas client remote service is not exist,client_id:%u, conn_id:%hu.\r\n",
            client_id, conn_id);
        return ERRCODE_BT_FAIL;
    }
    if (dev->character.cccd_info.descriptor_handle == 0) {
        bts_sample_error_log("bas client write cccd error, cccd is not exist.\n");
        return ERRCODE_BT_NOT_READY;
    }

    uint16_t ccc_value = BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_DEFAULT;
    if (enable_flag == BAS_CLIENT_CCCD_NOTIFY_ENABLE) {
        ccc_value = BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION;
    }
    uint8_t bas_ccc_val[] = {
        bas_16_low_byte(ccc_value),
        bas_16_high_byte(ccc_value)
    };
    gattc_handle_value_t value = {0};
    value.data_len = (uint16_t)sizeof(bas_ccc_val);
    value.data = bas_ccc_val;
    value.handle = dev->character.cccd_info.descriptor_handle;
    return gattc_write_req(client_id, conn_id, &value);
}

/* 发现服务回调函数 */
void ble_bas_client_discovery_service_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_service_result_t *service)
{
    ble_bas_client_t *dev = bas_client_get_dev_by_conn(client_id, conn_id);
    if (dev != NULL) {
        bts_sample_error_log("bas client remote service is already exist,client_id:%u, conn_id:%hu.\r\n",
            client_id, conn_id);
        return;
    }
    dev = (ble_bas_client_t *)bts_sample_mem_new(sizeof(ble_bas_client_t));
    if (dev == NULL) {
        bts_sample_error_log("bas client remote node create fail.\r\n");
        return;
    }
    dev->client_id = client_id;
    dev->conn_id = conn_id;
    dev->start_hdl = service->start_hdl;
    dev->end_hdl = service->end_hdl;
    dev->cbk = g_discovery_result_t;
    osal_list_add_tail((struct osal_list_head *)dev, &g_ble_bas_client_remote_list);
    if (dev->cbk != NULL) {
        dev->cbk(ERRCODE_BT_SUCCESS);
    }

    /* 发现特征 */
    gattc_discovery_character_param_t param = {0};
    param.service_handle = dev->start_hdl;
    bts_sample_data_to_uuid_len2(BLE_BAS_BATTERY_LEVEL_UUID, &param.uuid);
    errcode_t ret = gattc_discovery_character(client_id, conn_id, &param);
    bts_sample_info_log("bas client discovery battery level ret:%u.\r\n", ret);
}

/* 发现特征回调函数 */
void ble_bas_client_discovery_character_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_result_t *character)
{
    ble_bas_client_t *dev = bas_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("bas client remote service is not exist, client_id:%u conn_id:%hu\n",
            client_id, conn_id);
        return;
    }
    dev->character.declare_handle = character->declare_handle;
    dev->character.value_handle = character->value_handle;
    dev->character.properties = character->properties;

    /* 发现描述符 */
    gattc_discovery_descriptor(client_id, conn_id, dev->character.declare_handle);
}

/* 发现特征描述符回调函数 */
void ble_bas_client_discovery_descriptor_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_descriptor_result_t *descriptor)
{
    ble_bas_client_t *dev = bas_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("bas client remote service is not exist, client_id:%u conn_id:%hu\n",
            client_id, conn_id);
        return;
    }

    if ((descriptor->descriptor_hdl > dev->start_hdl) &&
        (descriptor->descriptor_hdl <= dev->end_hdl)) {
        /* 特征格式描述符 */
        bt_uuid_t character_format_uuid = {0};
        bts_sample_data_to_uuid_len2(BLE_BAS_CHARACTERISTIC_FORMAT_UUID, &character_format_uuid);

        /* 客户端特征配置描述符 */
        bt_uuid_t ccc_uuid = {0};
        bts_sample_data_to_uuid_len2(BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_UUID, &ccc_uuid);

        if (bts_sample_bt_gatts_cmp_uuid(&descriptor->uuid, &character_format_uuid) == true) {
            dev->character.cfd_info.descriptor_handle = descriptor->descriptor_hdl;
        } else if (bts_sample_bt_gatts_cmp_uuid(&descriptor->uuid, &ccc_uuid) == true) {
            dev->character.cccd_info.descriptor_handle = descriptor->descriptor_hdl;
        }
    }
}

/* 读回调函数 */
void ble_bas_client_read_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *read_result,
    gatt_status_t status)
{
    bts_unused(status);
    ble_bas_client_t *dev = bas_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        return;
    }

    if (read_result->handle == dev->character.value_handle) { /* 读特征的回调 */
        dev->character.value = *read_result->data;
        dev->character.read_cbk(*read_result->data);
    } else if (read_result->handle == dev->character.cfd_info.descriptor_handle) { /* 特征格式描述符 */
        ble_bas_client_read_cfd_descriptor_cbk(dev, read_result->data_len, read_result->data);
    } else if (read_result->handle == dev->character.cccd_info.descriptor_handle) { /* 客户端特征配置描述符 */
        ble_bas_client_read_ccc_descriptor_cbk(dev, read_result->data_len, read_result->data);
    }
}

/* 特征值通知回调函数 */
void ble_bas_client_notify_characteristic_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data)
{
    ble_bas_client_t *dev = bas_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        return;
    }

    if (data->handle == dev->character.value_handle) {
        dev->character.value = *data->data;
        if (dev->character.notify_cbk != NULL) {
            dev->character.notify_cbk(*data->data);
        }
    }
}
