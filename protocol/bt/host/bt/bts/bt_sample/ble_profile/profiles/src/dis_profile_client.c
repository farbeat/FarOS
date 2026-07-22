/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */
#include <string.h>
#include "osal_addr.h"
#include "sample_common.h"
#include "ble_pro_dis_client.h"

OSAL_LIST_HEAD(g_dis_client_rmt_dev_list);  /* ble_dis_client_remote_dev_t */

static uint16_t g_dis_character_uuid[CHAR_BUTT] = {
    BLE_PRO_DIS_SYSTEM_ID_UUID16,
    BLE_PRO_DIS_MODEL_NUMBER_UUID16,
    BLE_PRO_DIS_SERIAL_NUMBER_UUID16,
    BLE_PRO_DIS_FIRMWARE_REVISION_UUID16,
    BLE_PRO_DIS_HARDWARE_REVISION_UUID16,
    BLE_PRO_DIS_SOFTWARE_REVISION_UUID16,
    BLE_PRO_DIS_MANUFACTURER_NAME_UUID16,
    BLE_PRO_DIS_IEEE_REGULATORY_CERTIFICATION_DATA_LIST_UUID16,
    BLE_PRO_DIS_PNP_ID_UUID16,
};

static ble_dis_client_remote_dev_t* dis_client_get_dev_by_conn(uint8_t client_id, uint16_t conn_id)
{
    struct osal_list_head *list_head = &g_dis_client_rmt_dev_list;
    struct osal_list_head *node = list_head->next;
    while (node != NULL && node != list_head) {
        ble_dis_client_remote_dev_t *dev_node = (ble_dis_client_remote_dev_t *)node;
        if ((dev_node->client_id == client_id) && (dev_node->conn_id == conn_id)) {
            return dev_node;
        }
        node = node->next;
    }

    return NULL;
}

static uint8_t ble_pro_dis_client_get_character_id_by_uuid(uint16_t short_uuid)
{
    for (uint8_t i = CHAR_SYSTEM_ID; i < CHAR_BUTT; i++) {
        if (g_dis_character_uuid[i] == short_uuid) {
            return i;
        }
    }
    return CHAR_BUTT;
}

static uint8_t ble_dis_client_get_character_id_by_uuid(bt_uuid_t uuid)
{
    for (uint8_t i = CHAR_SYSTEM_ID; i < CHAR_BUTT; i++) {
        bt_uuid_t bt_uuid = {0};
        bts_sample_data_to_uuid_len2(g_dis_character_uuid[i], &bt_uuid);
        if (bts_sample_bt_gatts_cmp_uuid(&bt_uuid, &uuid) == true) {
            return i;
        }
    }
    return CHAR_BUTT;
}

static dis_client_character_status ble_dis_client_read_character(ble_dis_client_remote_dev_t *dev,
    uint16_t short_uuid)
{
    uint8_t character_id;
    character_id = ble_pro_dis_client_get_character_id_by_uuid(short_uuid);
    if (character_id >= CHAR_BUTT) {
        bts_sample_error_log("Invalid UUID:0x%x\r\n", short_uuid);
        return CHARACTER_NOT_SUPPORT;
    }

    if (dev->character[character_id].handle == 0) {
        bts_sample_error_log("Character 0x%04x is not support.\r\n", short_uuid);
        return CHARACTER_NOT_SUPPORT;
    }

    if (dev->character[character_id].status == CHARACTER_NOT_READABLE) {
        bts_sample_error_log("Character 0x%04x is not readable.\r\n", short_uuid);
        return CHARACTER_NOT_READABLE;
    }

    errcode_t ret = gattc_read_req_by_handle(dev->client_id, dev->conn_id, dev->character[character_id].handle);
    if (ret == ERRCODE_BT_SUCCESS) {
        return CHARACTER_READ_OK;
    } else {
        return CHARACTER_READ_TIMEOUT;
    }
}

errcode_t ble_dis_client_discovery_service(uint8_t client_id, uint16_t conn_id)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev != NULL) {
        bts_sample_error_log("dis client remote service is already exist.\r\n");
        return ERRCODE_BT_FAIL;
    }

    /* 查找服务 */
    bt_uuid_t dis_service_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_PRO_DIS_SERVICE_UUID16, &dis_service_uuid);
    return gattc_discovery_service(client_id, conn_id, &dis_service_uuid);
}

dis_client_character_status ble_pro_dis_read_remote_manufacturer_name(uint8_t client_id, uint16_t conn_id,
                                                                      dis_client_read_character_cbk cbk)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("[BLE] dis read character error,client_id:%u conn_id:%hu.\r\n",
            client_id, conn_id);
        return CHARACTER_NOT_SUPPORT;
    }
    dev->character[CHAR_MANUFACTURER_NAME].cbk = cbk;
    return ble_dis_client_read_character(dev, g_dis_character_uuid[CHAR_MANUFACTURER_NAME]);
}

dis_client_character_status ble_pro_dis_read_remote_model_number(uint8_t client_id, uint16_t conn_id,
                                                                 dis_client_read_character_cbk cbk)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("[BLE] dis read character error,client_id:%u conn_id:%hu.\r\n",
            client_id, conn_id);
        return CHARACTER_NOT_SUPPORT;
    }
    dev->character[CHAR_MODEL_NUMBER].cbk = cbk;
    return ble_dis_client_read_character(dev, g_dis_character_uuid[CHAR_MODEL_NUMBER]);
}

dis_client_character_status ble_pro_dis_read_remote_serial_number(uint8_t client_id, uint16_t conn_id,
                                                                  dis_client_read_character_cbk cbk)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("[BLE] dis read character error,client_id:%u conn_id:%hu.\r\n",
            client_id, conn_id);
        return CHARACTER_NOT_SUPPORT;
    }
    dev->character[CHAR_SERIAL_NUMBER].cbk = cbk;
    return ble_dis_client_read_character(dev, g_dis_character_uuid[CHAR_SERIAL_NUMBER]);
}

dis_client_character_status ble_pro_dis_read_remote_hardware_revision(uint8_t client_id, uint16_t conn_id,
                                                                      dis_client_read_character_cbk cbk)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("[BLE] dis read character error,client_id:%u conn_id:%hu.\r\n",
            client_id, conn_id);
        return CHARACTER_NOT_SUPPORT;
    }
    dev->character[CHAR_HARDWARE_REVISION].cbk = cbk;
    return ble_dis_client_read_character(dev, g_dis_character_uuid[CHAR_HARDWARE_REVISION]);
}

dis_client_character_status ble_pro_dis_read_remote_firmware_revision(uint8_t client_id, uint16_t conn_id,
                                                                      dis_client_read_character_cbk cbk)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("[BLE] dis read character error,client_id:%u conn_id:%hu.\r\n",
            client_id, conn_id);
        return CHARACTER_NOT_SUPPORT;
    }
    dev->character[CHAR_FIRMWARE_REVISION].cbk = cbk;
    return ble_dis_client_read_character(dev, g_dis_character_uuid[CHAR_FIRMWARE_REVISION]);
}

dis_client_character_status ble_pro_dis_read_remote_software_revision(uint8_t client_id, uint16_t conn_id,
                                                                      dis_client_read_character_cbk cbk)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("[BLE] dis read character error,client_id:%u conn_id:%hu.\r\n",
            client_id, conn_id);
        return CHARACTER_NOT_SUPPORT;
    }
    dev->character[CHAR_SOFTWARE_REVISION].cbk = cbk;
    return ble_dis_client_read_character(dev, g_dis_character_uuid[CHAR_SOFTWARE_REVISION]);
}

dis_client_character_status ble_pro_dis_read_remote_system_id(uint8_t client_id, uint16_t conn_id,
                                                              dis_client_read_character_cbk cbk)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("[BLE] dis read character error,client_id:%u conn_id:%hu.\r\n",
            client_id, conn_id);
        return CHARACTER_NOT_SUPPORT;
    }
    dev->character[CHAR_SYSTEM_ID].cbk = cbk;
    return ble_dis_client_read_character(dev, g_dis_character_uuid[CHAR_SYSTEM_ID]);
}

dis_client_character_status ble_pro_dis_read_remote_ieee_rcdl(uint8_t client_id, uint16_t conn_id,
                                                              dis_client_read_character_cbk cbk)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("[BLE] dis read character error,client_id:%u conn_id:%hu.\r\n",
            client_id, conn_id);
        return CHARACTER_NOT_SUPPORT;
    }
    dev->character[CHAR_IEEE_RCDL].cbk = cbk;
    return ble_dis_client_read_character(dev, g_dis_character_uuid[CHAR_IEEE_RCDL]);
}

dis_client_character_status ble_pro_dis_read_remote_pnp_id(uint8_t client_id, uint16_t conn_id,
                                                           dis_client_read_character_cbk cbk)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("[BLE] dis read character error,client_id:%u conn_id:%hu.\r\n",
            client_id, conn_id);
        return CHARACTER_NOT_SUPPORT;
    }
    dev->character[CHAR_PNP_ID].cbk = cbk;
    return ble_dis_client_read_character(dev, g_dis_character_uuid[CHAR_PNP_ID]);
}

void ble_dis_client_clear_remote_by_conn(uint8_t client_id, uint16_t conn_id)
{
    ble_dis_client_remote_dev_t *node = NULL;
    node = dis_client_get_dev_by_conn(client_id, conn_id);
    if (node != NULL) {
        osal_list_del(node);
        bts_sample_mem_free(node);
    }
}

void ble_dis_client_clear_remote_all(void)
{
    while (osal_list_empty(&g_dis_client_rmt_dev_list) == false) {
        struct osal_list_head *node = g_dis_client_rmt_dev_list.next;
        osal_list_del(node);
        bts_sample_mem_free(node);
    }
}

/* 发现服务回调函数 */
void ble_dis_client_discovery_service_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_service_result_t *service)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev != NULL) {
        bts_sample_error_log("dis client remote service is already exist.\r\n");
        return;
    }
    dev = bts_sample_mem_new(sizeof(ble_dis_client_remote_dev_t));
    if (dev == NULL) {
        bts_sample_error_log("dis client remote node create fail.\r\n");
        return;
    }
    dev->client_id = client_id;
    dev->conn_id = conn_id;
    dev->start_hdl = service->start_hdl;
    dev->end_hdl = service->end_hdl;
    osal_list_add_tail((struct osal_list_head *)dev, &g_dis_client_rmt_dev_list);

    /* 发现特征 */
    gattc_discovery_character_param_t param = {0};
    param.service_handle = dev->start_hdl;
    errcode_t ret = gattc_discovery_character(client_id, conn_id, &param);
    bts_sample_info_log("[BLE]dis client discovery character ret:%u \r\n", ret);
}

/* 发现特征回调函数 */
void ble_dis_client_discovery_character_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_result_t *character)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("[BLE]dis client remote service is not exist,client_id:%u conn_id:%hu.\r\n",
            client_id, conn_id);
        return;
    }
    uint8_t character_id;
    character_id = ble_dis_client_get_character_id_by_uuid(character->uuid);
    if (character_id >= CHAR_BUTT) {
        bts_sample_error_log("[ACore] dis client discovery character cbk character UUID is not matched, \
            UUID:0x%02x%02x\r\n", character->uuid.uuid[0], character->uuid.uuid[1]);
        return;
    }
    if ((character->properties & GATT_CHARACTER_PROPERTY_BIT_READ) == 0) {
        dev->character[character_id].status = CHARACTER_NOT_READABLE;
    }
    dev->character[character_id].handle = character->value_handle;
    bts_sample_info_log("[BLE]dis client discovery character cbk, uuid:0x%02x%02x, value_handle:%hu, \
        properties: %u", character->uuid.uuid[0], character->uuid.uuid[1], character->value_handle,
        character->properties);
}

/* 读回调函数 */
void ble_dis_client_read_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *read_result,
    gatt_status_t status)
{
    ble_dis_client_remote_dev_t *dev = dis_client_get_dev_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("[BLE]dis client remote service is not exist,client_id:%u conn_id:%hu.\r\n",
            client_id, conn_id);
        return;
    }

    for (uint8_t i = CHAR_SYSTEM_ID; i < CHAR_BUTT; i++) {
        if (dev->character[i].handle == read_result->handle) {
            if (dev->character[i].cbk != NULL) {
                dev->character[i].cbk(status, g_dis_character_uuid[i],
                    read_result->data_len, read_result->data);
            }
            return;
        }
    }
    bts_sample_error_log("character handle:0x%x is not dis character.\r\n", read_result->handle);
}
