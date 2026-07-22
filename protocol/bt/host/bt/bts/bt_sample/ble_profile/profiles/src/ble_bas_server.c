/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BT Battery Service Profile Server Module.
 */
#include <string.h>
#include "errcode.h"
#include "osal_addr.h"
#include "bts_def.h"
#include "sample_common.h"
#include "bts_gatt_server.h"
#include "ble_bas.h"
#include "ble_bas_server.h"

#define BLE_BAS_SERVER_ID 1
#define BLE_BAS_SINGLE_LINK_CONNECT_ID 1
#define bas_16_low_byte(uuid) ((uint8_t)((uuid) & 0x00FF))
#define bas_16_high_byte(uuid) ((uint8_t)(((uuid) & 0xFF00) >> 8))

ble_bas_data_t g_ble_bas_data = {0};

static uint16_t g_bas_character_value_handle = 0; /* 特征值句柄，GATT规范里的句柄 */

/* 添加特征格式描述符 */
static void ble_bas_add_desc_character_format(uint8_t server_id, uint16_t srvc_handle)
{
    /* 特征格式描述符：格式：0x04;指数：0x00;单位：0x27AD(percentage);命名空间：0x01（Bluetooth SIG）;
     * 描述：0x010F（internal）
     */
    /* 小端传输 */
    uint8_t bas_character_format_val[] = {
        BLE_BAS_CHARACTERISTIC_FORMAT_FORMAT,
        BLE_BAS_CHARACTERISTIC_FORMAT_EXPONENT,
        bas_16_low_byte(BLE_BAS_CHARACTERISTIC_FORMAT_UNIT_UUID),
        bas_16_high_byte(BLE_BAS_CHARACTERISTIC_FORMAT_UNIT_UUID),
        BLE_BAS_CHARACTERISTIC_FORMAT_NAME_SPACE,
        bas_16_low_byte(BLE_BAS_CHARACTERISTIC_FORMAT_DESCRIPTION_UUID),
        bas_16_high_byte(BLE_BAS_CHARACTERISTIC_FORMAT_DESCRIPTION_UUID)
    };
    bt_uuid_t character_format_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_BAS_CHARACTERISTIC_FORMAT_UUID, &character_format_uuid);
    gatts_add_desc_info_t descriptor;
    descriptor.desc_uuid = character_format_uuid;
    descriptor.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
    descriptor.value_len = (uint16_t)sizeof(bas_character_format_val);
    descriptor.value = bas_character_format_val;
    uint16_t character_format_handle = 0;
    gatts_add_descriptor_sync(server_id, srvc_handle, &descriptor, &character_format_handle);
    bts_sample_info_log("[ACore] bas server add cfd handle:%hu.\r\n", character_format_handle);
    if (character_format_handle == 0) {
        return;
    }
}

/* 添加客户端特征配置描述符 */
static void ble_bas_add_desc_client_character_configuration(uint8_t server_id, uint16_t srvc_handle)
{
    /* 客户端特征配置描述符：通知，小端传输 */
    uint8_t bas_ccc_val[] = {
        bas_16_low_byte(BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION),
        bas_16_high_byte(BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION)
    };
    bt_uuid_t ccc_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_UUID, &ccc_uuid);
    gatts_add_desc_info_t descriptor;
    descriptor.desc_uuid = ccc_uuid;
    descriptor.permissions = GATT_ATTRIBUTE_PERMISSION_READ | GATT_ATTRIBUTE_PERMISSION_WRITE;
    descriptor.value_len = (uint16_t)sizeof(bas_ccc_val);
    descriptor.value = bas_ccc_val;
    uint16_t ccc_handle = 0;
    gatts_add_descriptor_sync(server_id, srvc_handle, &descriptor, &ccc_handle);
    bts_sample_info_log("[ACore] bas server add cccd handle:%hu.\r\n", ccc_handle);
    if (ccc_handle == 0) {
        return;
    }
}

/* 添加电量特征 */
static void ble_bas_add_battery_level(uint8_t server_id, uint16_t srvc_handle)
{
    bt_uuid_t bas_battery_level_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_BAS_BATTERY_LEVEL_UUID, &bas_battery_level_uuid);
    gatts_add_chara_info_t character;
    character.chara_uuid = bas_battery_level_uuid;
    character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
    character.properties = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_NOTIFY;
    character.value_len = 1;
    character.value = &(g_ble_bas_data.battery_level);
    gatts_add_character_result_t result = {0};
    gatts_add_characteristic_sync(server_id, srvc_handle, &character, &result);

    bts_sample_info_log("[ACore] bas server add character handle:%hu value_handle:%hu.\r\n",
        result.handle, result.value_handle);
    if (result.handle == 0 || result.value_handle == 0) {
        return;
    }
    g_bas_character_value_handle = result.value_handle;

    /* 添加描述符 */
    ble_bas_add_desc_character_format(server_id, srvc_handle);
    ble_bas_add_desc_client_character_configuration(server_id, srvc_handle);
}

/* 添加特征和描述符 */
static void ble_bas_add_characters_and_descriptors(uint8_t server_id, uint16_t srvc_handle)
{
    ble_bas_add_battery_level(server_id, srvc_handle);
}

/* 创建服务 */
static void ble_bas_add_service(void)
{
    bt_uuid_t bas_service_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_BAS_SERVICE_UUID, &bas_service_uuid);
    /* 添加服务 */
    gatts_add_service_sync(BLE_BAS_SERVER_ID, &bas_service_uuid, true, &(g_ble_bas_data.start_hdl));
    bts_sample_info_log("[ACore] bas server add service handle:%hu.\r\n", g_ble_bas_data.start_hdl);
    if (g_ble_bas_data.start_hdl == 0) {
        return;
    }
    /* 添加特征和描述符 */
    ble_bas_add_characters_and_descriptors(BLE_BAS_SERVER_ID, g_ble_bas_data.start_hdl);
    /* 启动服务 */
    gatts_start_service(BLE_BAS_SERVER_ID, g_ble_bas_data.start_hdl);
}

void ble_bas_server_init(ble_bas_func_result_cbk_t cbk)
{
    bts_sample_info_log("[ACore] bas server beginning add service\r\n");
    g_ble_bas_data.battery_level = BLE_BAS_BATTERY_LEVEL_DEFAULT;
    g_ble_bas_data.cbk = cbk;
    g_ble_bas_data.start_hdl = 0;
    g_bas_character_value_handle = 0;
    ble_bas_add_service();
}

uint8_t ble_bas_server_battery_level_get(void)
{
    return g_ble_bas_data.battery_level;
}

errcode_t ble_bas_server_battery_level_notify(uint8_t value)
{
    if (value > BLE_BAS_BATTERY_LEVEL_MAX) {
        bts_sample_error_log("bas server notify fail, battery level is out of range [0-100].\r\n");
        return ERRCODE_BT_FAIL;
    }
    if (g_bas_character_value_handle == 0) {
        bts_sample_error_log("bas notify fail, bas service does not exist.\r\n");
        return ERRCODE_BT_FAIL;
    }
    if (value != g_ble_bas_data.battery_level) {
        g_ble_bas_data.battery_level = value;
        gatts_ntf_ind_t param = {0};
        param.attr_handle = g_bas_character_value_handle;
        param.value_len = 1;
        param.value = &value;
        gatts_notify_indicate(BLE_BAS_SERVER_ID, BLE_BAS_SINGLE_LINK_CONNECT_ID, &param);
        return ERRCODE_BT_SUCCESS;
    }
    bts_sample_error_log("bas server notify fail, battery level is not changed\r\n");
    return ERRCODE_BT_FAIL;
}

void ble_bas_start_service_cbk(uint8_t server_id, uint16_t handle)
{
    if (server_id == 1 && handle == g_ble_bas_data.start_hdl) {
        bts_sample_info_log("[ACore] bas start service success,service_att_handle:%hu\r\n", handle);
        if (g_ble_bas_data.cbk != NULL) {
            g_ble_bas_data.cbk(ERRCODE_BT_SUCCESS);
        }
    }
}