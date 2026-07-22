/*
 * Copyright (c) @CompanyNameMagicTag 2023. All rights reserved.
 * Description: FMP Locator Module.
 */
#include "securec.h"
#include "errcode.h"
#include "osal_list.h"
#include "sample_common.h"
#include "bts_def.h"
#include "bts_gatt_stru.h"
#include "ble_fmp_locator.h"

typedef struct {
    struct osal_list_head node;
    uint8_t client_id;
    uint16_t conn_id;
    uint16_t srvc_hdl;
    uint16_t alc_hdl;
    uint16_t alc_value_hdl;
} fmp_locator_remote_dev_t;

OSAL_LIST_HEAD(g_fmp_locator_client_list);    /* list of structure fmp_locator_remote_dev_t */

static fmp_locator_remote_dev_t *fmp_locator_get_client_by_conn(uint8_t client_id, uint16_t conn_id)
{
    struct osal_list_head *list_head = &g_fmp_locator_client_list;
    struct osal_list_head *node = list_head->next;
    while (node != NULL && node != list_head) {
        fmp_locator_remote_dev_t *dev_node = (fmp_locator_remote_dev_t *)node;
        if ((dev_node->client_id == client_id) && (dev_node->conn_id == conn_id)) {
            return dev_node;
        }
        node = node->next;
    }

    return NULL;
}

void ble_fmp_locator_clear_remote_by_conn(uint8_t client_id, uint16_t conn_id)
{
    fmp_locator_remote_dev_t *node = NULL;
    node = fmp_locator_get_client_by_conn(client_id, conn_id);
    if (node != NULL) {
        osal_list_del(node);
        bts_sample_mem_free(node);
    }
}

void ble_fmp_locator_clear_remote_all(void)
{
    while (osal_list_empty(&g_fmp_locator_client_list) == false) {
        struct osal_list_head *node = g_fmp_locator_client_list.next;
        osal_list_del(node);
        bts_sample_mem_free(node);
    }
}

errcode_t fmp_locator_connect(uint8_t client_id, uint16_t conn_id)
{
    fmp_locator_remote_dev_t *dev = fmp_locator_get_client_by_conn(client_id, conn_id);
    if (dev != NULL) {
        bts_sample_error_log("fmp client remote service is already exist.\r\n");
        return ERRCODE_BT_FAIL;
    }

    /* 查找服务 */
    bt_uuid_t fmp_service_uuid = {0};
    bts_sample_data_to_uuid_len2(FMP_IMMEDIATE_ALERT_SERVICE_UUID, &fmp_service_uuid);
    return gattc_discovery_service(client_id, conn_id, &fmp_service_uuid);
}

errcode_t fmp_locator_send_alert(uint8_t client_id, uint16_t conn_id, uint8_t alert_level)
{
    fmp_locator_remote_dev_t *dev = fmp_locator_get_client_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("fmp client remote service is not exist.\r\n");
        return ERRCODE_BT_FAIL;
    }

    if (dev->alc_value_hdl == 0) {
        bts_sample_error_log("fmp client remote node character is not exist.\r\n");
        return ERRCODE_BT_FAIL;
    }

    uint8_t alert_level_value = alert_level;
    gattc_handle_value_t handle_value = {0};
    handle_value.handle = dev->alc_value_hdl;
    handle_value.data_len = (uint16_t)sizeof(uint8_t);
    handle_value.data = &alert_level_value;
    return gattc_write_cmd(client_id, conn_id, &handle_value);
}

/* 发现服务回调函数 */
void fmp_locator_discovery_service_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_service_result_t *service)
{
    fmp_locator_remote_dev_t *dev = fmp_locator_get_client_by_conn(client_id, conn_id);
    if (dev != NULL) {
        bts_sample_error_log("fmp client remote service is not exist,client_id:%u, conn_id:%hu.\r\n",
            client_id, conn_id);
        return;
    }
    dev = bts_sample_mem_new(sizeof(fmp_locator_remote_dev_t));
    if (dev == NULL) {
        bts_sample_error_log("fmp client remote node create fail.\r\n");
        return;
    }
    dev->client_id = client_id;
    dev->conn_id = conn_id;
    dev->srvc_hdl = service->start_hdl;
    osal_list_add_tail((struct osal_list_head *)dev, &g_fmp_locator_client_list);

    /* 发现特征 */
    gattc_discovery_character_param_t param = {0};
    param.service_handle = dev->srvc_hdl;
    bts_sample_data_to_uuid_len2(FMP_ALERT_LEVEL_CHARACTERISTIC_UUID, &param.uuid);
    errcode_t ret = gattc_discovery_character(client_id, conn_id, &param);
    bts_sample_info_log("[BLE]fmp client discovery character ret:%u \r\n", ret);
}

/* 发现特征回调函数 */
void fmp_locator_discovery_characteristic_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_result_t *character)
{
    fmp_locator_remote_dev_t *dev = fmp_locator_get_client_by_conn(client_id, conn_id);
    if (dev == NULL) {
        bts_sample_error_log("fmp client remote service is not exist,client_id:%u, conn_id:%hu.\r\n",
            client_id, conn_id);
        return;
    }
    if ((character->properties & GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP) == 0) {
        bts_sample_error_log("fmp client remote character not support write without response "
            "properties:%u.\r\n", character->properties);
        return;
    }
    dev->alc_hdl = character->declare_handle;
    dev->alc_value_hdl = character->value_handle;
    bts_sample_info_log("[BLE]fmp client discovery character cbk, declare_hdl:0x%x value_hdl:0x%x\r\n",
        character->declare_handle, character->value_handle);
}
