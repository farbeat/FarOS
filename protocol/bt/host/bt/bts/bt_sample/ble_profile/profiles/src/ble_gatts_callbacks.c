/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 */
#include "securec.h"
#include "osal_addr.h"
#include "bts_def.h"
#include "sample_common.h"
#include "bts_gatt_server.h"
#include "bts_gatt_client.h"
#include "ble_pro_dis.h"
#include "ble_hid_take_photo_server.h"
#include "ble_pro_dis_server.h"
#include "ble_fmp_target.h"
#include "ble_bas_client.h"
#include "ble_bas_server.h"
#include "ble_pro_dis_client.h"
#include "ble_fmp_locator.h"
#include "ble_ancs_client.h"
#include "ble_ams_client.h"
#include "ble_gatts_callbacks.h"

uint32_t g_ble_gatt_mtu_size = 23;

/* 服务添加回调 */
static void ble_server_service_add_cbk(uint8_t server_id, bt_uuid_t *uuid, uint16_t handle, errcode_t status)
{
    bts_sample_info_log("[BLE]ble_server_service_add_cbk server: %u status: %u srv_handle: %hu uuid_len: %u \
        uuid: %02x%02x", server_id, status, handle, uuid->uuid_len, uuid->uuid[0], uuid->uuid[1]);
    if (status != ERRCODE_BT_SUCCESS) {
        return;
    }
    bt_uuid_t hid_service_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_UUID_HUMAN_INTERFACE_DEVICE, &hid_service_uuid);

    bt_uuid_t dis_service_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_PRO_DIS_SERVICE_UUID16, &dis_service_uuid);

    bt_uuid_t fmp_service_uuid = {0};
    bts_sample_data_to_uuid_len2(FMP_IMMEDIATE_ALERT_SERVICE_UUID, &fmp_service_uuid);

    if (bts_sample_bt_gatts_cmp_uuid(uuid, &hid_service_uuid) == true) {
        ble_hid_add_characters_and_descriptors(server_id, handle);
        gatts_start_service(server_id, handle);
    } else if (bts_sample_bt_gatts_cmp_uuid(uuid, &dis_service_uuid) == true) {
        ble_pro_dis_add_characters(server_id, handle);
        gatts_start_service(server_id, handle);
    } else if (bts_sample_bt_gatts_cmp_uuid(uuid, &fmp_service_uuid) == true) {
        fmp_target_add_alc_characteristic(server_id, handle);
        gatts_start_service(server_id, handle);
    } else {
        bts_sample_error_log("[BLE] server add service cbk, service uuid is not matched.\n");
    }
}

/* 特征添加回调 */
static void  ble_server_characteristic_add_cbk(uint8_t server_id, bt_uuid_t *uuid, uint16_t service_handle,
    gatts_add_character_result_t *result, errcode_t status)
{
    bts_sample_info_log("[BLE]server add characteristic cbk server_id: %u status: %u srv_hdl: %hu "
        "char_hdl: %hu value_handle: %hu uuid_len: %u uuid: %02x%02x",
        server_id, status, service_handle, result->handle, result->value_handle,
        uuid->uuid_len, uuid->uuid[0], uuid->uuid[1]);
    if (status != ERRCODE_BT_SUCCESS) {
        return;
    }

    bt_uuid_t hid_report_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_UUID_REPORT, &hid_report_uuid);

    bt_uuid_t fmp_alc_uuid = {0};
    bts_sample_data_to_uuid_len2(FMP_ALERT_LEVEL_CHARACTERISTIC_UUID, &fmp_alc_uuid);

    if (bts_sample_bt_gatts_cmp_uuid(uuid, &hid_report_uuid) == true) {
        ble_hid_add_characters_input_report_cbk(service_handle);
    } else if (bts_sample_bt_gatts_cmp_uuid(uuid, &fmp_alc_uuid) == true) {
        fmp_target_add_characteristic_cbk(service_handle, result->handle, result->value_handle);
    }
}

/* 描述符添加回调 */
static void  ble_server_descriptor_add_cbk(uint8_t server_id, bt_uuid_t *uuid, uint16_t service_handle,
    uint16_t handle, errcode_t status)
{
    bts_sample_info_log("[BLE]server add descriptor server: %u status: %u srv_hdl: %hu desc_hdl: %hu "
        "uuid_len:%u, uuid: ", server_id, status, service_handle, handle, uuid->uuid_len);
}

/* 开始服务回调 */
static void ble_server_service_start_cbk(uint8_t server_id, uint16_t handle, errcode_t status)
{
    bts_sample_info_log("[BLE]server start service server: %u status: %u srv_hdl: %hu\n",
        server_id, status, handle);
    if (status != ERRCODE_BT_SUCCESS) {
        return;
    }
    ble_bas_start_service_cbk(server_id, handle);
    fmp_target_start_service_cbk(server_id, handle);
}

static void ble_server_req_read_cbk(uint8_t server_id, uint16_t conn_id, gatts_req_read_cb_t *cb_para,
    errcode_t status)
{
    bts_sample_info_log("[BLE]server read req server: %u status: %u conn_id:%hu handle: %hu \n",
        server_id, status, conn_id, cb_para->handle);
}

static void ble_server_req_write_cbk(uint8_t server_id, uint16_t conn_id, gatts_req_write_cb_t *cb_para,
    errcode_t status)
{
    bts_sample_info_log("[BLE]server write req server: %u status: %u conn_id:%hu handle: %hu \n",
        server_id, status, conn_id, cb_para->handle);
    if (status != ERRCODE_BT_SUCCESS) {
        return;
    }
    fmp_target_write_req_cbk(server_id, cb_para->handle, *(cb_para->value));
}

static void ble_server_service_stop_cbk(uint8_t server_id, uint16_t handle, errcode_t status)
{
    bts_sample_info_log("[BLE]server stop service server: %u status: %u handle: %hu \n",
        server_id, status, handle);
}

static void ble_server_service_delete_cbk(uint8_t server_id, errcode_t status)
{
    bts_sample_info_log("[BLE]server delete service server: %u status: %u \n",
        server_id, status);
}

static void ble_server_mtu_change_cbk(uint8_t server_id, uint16_t conn_id, uint16_t mtu_size, errcode_t status)
{
    bts_sample_info_log("[BLE]server mtu change server: %u status: %u conn_id: %u mtu_size: %u \n",
        server_id, status, conn_id, mtu_size);
}

errcode_t ble_server_register_gatt_callbacks(void)
{
    errcode_t ret = ERRCODE_BT_SUCCESS;
    gatts_callbacks_t cb = {0};
    cb.add_service_cb = ble_server_service_add_cbk;
    cb.add_characteristic_cb = ble_server_characteristic_add_cbk;
    cb.add_descriptor_cb = ble_server_descriptor_add_cbk;
    cb.start_service_cb = ble_server_service_start_cbk;
    cb.read_request_cb = ble_server_req_read_cbk;
    cb.write_request_cb = ble_server_req_write_cbk;
    cb.stop_service_cb = ble_server_service_stop_cbk;
    cb.delete_service_cb = ble_server_service_delete_cbk;
    cb.mtu_changed_cb = ble_server_mtu_change_cbk;
    ret = gatts_register_callbacks(&cb);
    return ret;
}


static void ble_client_discovery_service_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_service_result_t *service, errcode_t status)
{
    bts_sample_info_log("[BLE]client discovery service cbk client_id: %u conn_id: %hu status: %u \
        uuid: 0x%02x%02x \n", client_id, conn_id, status, service->uuid.uuid[0], service->uuid.uuid[1]);
    if (status != ERRCODE_BT_SUCCESS) {
        return;
    }

    bt_uuid_t bas_service_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_BAS_SERVICE_UUID, &bas_service_uuid);

    bt_uuid_t dis_service_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_PRO_DIS_SERVICE_UUID16, &dis_service_uuid);

    bt_uuid_t fmp_service_uuid = {0};
    bts_sample_data_to_uuid_len2(FMP_IMMEDIATE_ALERT_SERVICE_UUID, &fmp_service_uuid);
    if (bts_sample_bt_gatts_cmp_uuid(&service->uuid, &bas_service_uuid) == true) {
        ble_bas_client_discovery_service_cbk(client_id, conn_id, service);
    } else if (bts_sample_bt_gatts_cmp_uuid(&service->uuid, &dis_service_uuid) == true) {
        ble_dis_client_discovery_service_cbk(client_id, conn_id, service);
    } else if (bts_sample_bt_gatts_cmp_uuid(&service->uuid, &fmp_service_uuid) == true) {
        fmp_locator_discovery_service_cbk(client_id, conn_id, service);
    } else if (is_ble_ancs_service(&service->uuid)) {
        ble_ancs_client_discovery_service_cbk(client_id, conn_id, service);
    } else if (is_ble_ams_service(&service->uuid)) {
        ble_ams_client_discovery_service_cbk(client_id, conn_id, service);
    }
}

static void ble_client_discovery_character_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_result_t *character, errcode_t status)
{
    bts_sample_info_log("[BLE]client discovery character cbk client_id: %u conn_id: %hu status: %u \
        uuid: 0x%02x%02x \n", client_id, conn_id, status, character->uuid.uuid[0], character->uuid.uuid[1]);
    if (status != ERRCODE_BT_SUCCESS) {
        return;
    }

    bt_uuid_t battery_level_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_BAS_BATTERY_LEVEL_UUID, &battery_level_uuid);

    bt_uuid_t fmp_alc_uuid = {0};
    bts_sample_data_to_uuid_len2(FMP_ALERT_LEVEL_CHARACTERISTIC_UUID, &fmp_alc_uuid);

    if (bts_sample_bt_gatts_cmp_uuid(&character->uuid, &battery_level_uuid) == true) {
        ble_bas_client_discovery_character_cbk(client_id, conn_id, character);
    } else if (bts_sample_bt_gatts_cmp_uuid(&character->uuid, &fmp_alc_uuid) == true) {
        fmp_locator_discovery_characteristic_cbk(client_id, conn_id, character);
    } else {
        ble_dis_client_discovery_character_cbk(client_id, conn_id, character);
        ble_ancs_client_discovery_character_cbk(client_id, conn_id, character);
        ble_ams_client_discovery_character_cbk(client_id, conn_id, character);
    }
}

static void ble_client_discovery_descriptor_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_descriptor_result_t *descriptor, errcode_t status)
{
    bts_sample_info_log("[BLE]client discovery descriptor cbk client_id: %u conn_id: %hu status: %u \
        uuid: 0x%02x%02x \n", client_id, conn_id, status, descriptor->uuid.uuid[0], descriptor->uuid.uuid[1]);
    if (status != ERRCODE_BT_SUCCESS) {
        return;
    }
    ble_bas_client_discovery_descriptor_cbk(client_id, conn_id, descriptor);
    ble_ancs_client_discovery_descriptor_cbk(client_id, conn_id, descriptor);
    ble_ams_client_discovery_descriptor_cbk(client_id, conn_id, descriptor);
}

static void ble_client_read_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *read_result,
    gatt_status_t status)
{
    bts_sample_info_log("[BLE]client read cbk client_id: %u conn_id: %hu status: 0x%x handle: %lu \n",
        client_id, conn_id, status, read_result->handle);
    ble_bas_client_read_cbk(client_id, conn_id, read_result, status);
    ble_dis_client_read_cbk(client_id, conn_id, read_result, status);
    ble_ams_client_read_cbk(client_id, conn_id, read_result, status);
}

static void ble_client_write_cbk(uint8_t client_id, uint16_t conn_id, uint16_t handle, gatt_status_t status)
{
    bts_sample_info_log("[BLE]client write cbk client_id: %u conn_id: %hu status: %d handle: %hu \n",
        client_id, conn_id, status, handle);

    ble_ancs_client_write_cbk(client_id, conn_id, handle, status);
    ble_ams_client_write_cbk(client_id, conn_id, handle, status);
}

static void ble_client_notification_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data,
    errcode_t status)
{
    bts_sample_info_log("[BLE]client notification cbk client_id: %u conn_id: %hu status: %u handle: %hu \n",
        client_id, conn_id, status, data->handle);
    if (status != ERRCODE_BT_SUCCESS) {
        return;
    }
    ble_bas_client_notify_characteristic_cbk(client_id, conn_id, data);
    ble_ancs_client_notify_cbk(client_id, conn_id, data);
    ble_ams_client_notify_cbk(client_id, conn_id, data);
}

static void ble_client_mtu_exchange_result_cbk(uint8_t client_id, uint16_t conn_id, uint16_t mtu_size, errcode_t status)
{
    bts_sample_info_log("[BLE]client MTU changed cbk: status: %u client_id: %u conn_id: %u mtu:%u.\r\n",
        status, client_id, conn_id, mtu_size);
    g_ble_gatt_mtu_size = mtu_size;
}

errcode_t ble_client_register_gatt_callbacks(void)
{
    errcode_t ret = ERRCODE_BT_SUCCESS;
    gattc_callbacks_t cb = {0};
    cb.discovery_svc_cb = ble_client_discovery_service_cbk;
    cb.discovery_chara_cb = ble_client_discovery_character_cbk;
    cb.discovery_desc_cb = ble_client_discovery_descriptor_cbk;
    cb.read_cb = ble_client_read_cbk;
    cb.write_cb = ble_client_write_cbk;
    cb.notification_cb = ble_client_notification_cbk;
    cb.mtu_changed_cb = ble_client_mtu_exchange_result_cbk;
    ret = gattc_register_callbacks(&cb);
    return ret;
}

uint32_t ble_gatt_mtu_exchange_result_get(void)
{
    return g_ble_gatt_mtu_size;
}