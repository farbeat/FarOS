/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */
#include <string.h>
#include "osal_addr.h"
#include "bts_def.h"
#include "sample_common.h"
#include "bts_gatt_stru.h"
#include "bts_gatt_server.h"
#include "ble_pro_dis_server.h"

ble_pro_dis_data g_ble_pro_dis_data = {
    .system_id = NULL,
    .model_number = NULL,
    .serial_number = NULL,
    .firmware_revision = NULL,
    .hardware_revision = NULL,
    .software_revision = NULL,
    .manufacturer_name = NULL,
    .ieee_regulatory_certification_data_list = NULL,
    .pnp_id = {0},
};

/* DIS gatt attribute num */
#define BLE_DIS_ATTRIBUTE_NUM 10

/* DIS gatt server id */
#define BLE_DIS_SERVER_ID 1

/* BT_NUM */
#define BT_NUM8 8

char *ble_pro_dis_server_manufacturer_name(void)
{
    return g_ble_pro_dis_data.manufacturer_name;
}

int ble_pro_dis_server_manufacturer_name_set(char *value)
{
    g_ble_pro_dis_data.manufacturer_name = value;
    return 0;
}

char *ble_pro_dis_server_model_number(void)
{
    return g_ble_pro_dis_data.model_number;
}

int ble_pro_dis_server_model_number_set(char *value)
{
    g_ble_pro_dis_data.model_number = value;
    return 0;
}

char *ble_pro_dis_server_serial_number(void)
{
    return g_ble_pro_dis_data.serial_number;
}

int ble_pro_dis_server_serial_number_set(char *value)
{
    g_ble_pro_dis_data.serial_number = value;
    return 0;
}

char *ble_pro_dis_server_hardware_revision(void)
{
    return g_ble_pro_dis_data.hardware_revision;
}

int ble_pro_dis_server_hardware_revision_set(char *value)
{
    g_ble_pro_dis_data.hardware_revision = value;
    return 0;
}

char *ble_pro_dis_server_firmware_revision(void)
{
    return g_ble_pro_dis_data.firmware_revision;
}

int ble_pro_dis_server_firmware_revision_set(char *value)
{
    g_ble_pro_dis_data.firmware_revision = value;
    return 0;
}

char *ble_pro_dis_server_software_revision(void)
{
    return g_ble_pro_dis_data.software_revision;
}

int ble_pro_dis_server_software_revision_set(char *value)
{
    g_ble_pro_dis_data.software_revision = value;
    return 0;
}

char *ble_pro_dis_server_system_id(void)
{
    return g_ble_pro_dis_data.system_id;
}

int ble_pro_dis_server_system_id_set(char *value)
{
    g_ble_pro_dis_data.system_id = value;
    return 0;
}

char *ble_pro_dis_server_ieee_regulatory_certification_data_list(void)
{
    return g_ble_pro_dis_data.ieee_regulatory_certification_data_list;
}

int ble_pro_dis_server_ieee_regulatory_certification_data_list_set(char *value)
{
    g_ble_pro_dis_data.ieee_regulatory_certification_data_list = value;
    return 0;
}

ble_pro_dis_pnp_id *ble_pro_dis_server_pnp_id(void)
{
    return &g_ble_pro_dis_data.pnp_id;
}

int ble_pro_dis_server_pnp_id_set(uint8_t vendor_id_source, uint16_t vendor_id, uint16_t product_id,
                                  uint16_t product_revision)
{
    g_ble_pro_dis_data.pnp_id.vendor_id_source = vendor_id_source;
    g_ble_pro_dis_data.pnp_id.vendor_id        = vendor_id;
    g_ble_pro_dis_data.pnp_id.product_id       = product_id;
    g_ble_pro_dis_data.pnp_id.product_revision = product_revision;
    return 0;
}

static void ble_pro_dis_add_service(void)
{
    bt_uuid_t dis_service_uuid;
    bts_sample_data_to_uuid_len2(BLE_PRO_DIS_SERVICE_UUID16, &dis_service_uuid);
    gatts_add_service(BLE_DIS_SERVER_ID, &dis_service_uuid, true);
}

static void ble_pro_dis_add_character_system_id(uint8_t server_id, uint16_t srvc_handle)
{
    size_t len = 0;
    if (g_ble_pro_dis_data.system_id == NULL) {
        bts_sample_error_log("dis server system id is not support!\r\n");
        return;
    }
    len = strlen(g_ble_pro_dis_data.system_id) + 1;
    if (len > 0) {
        gatts_add_chara_info_t character;
        bts_sample_data_to_uuid_len2(BLE_PRO_DIS_SYSTEM_ID_UUID16, &character.chara_uuid);
        character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
        character.properties = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP;
        character.value_len = (uint16_t)len;
        character.value = (uint8_t *)bts_sample_mem_new(sizeof(char) * len);
        if (character.value == NULL) {
            return;
        }
        if (memcpy_s(character.value, len, g_ble_pro_dis_data.system_id, len) != EOK) {
            bts_sample_error_log("memcpy_s fail.");
            return;
        }
        gatts_add_characteristic(server_id, srvc_handle, &character);
        bts_sample_mem_free(character.value);
    }
}

static void ble_pro_dis_add_character_model_number(uint8_t server_id, uint16_t srvc_handle)
{
    size_t len = 0;
    if (g_ble_pro_dis_data.model_number == NULL) {
        bts_sample_error_log("dis server model number is not support!\r\n");
        return;
    }
    len = strlen(g_ble_pro_dis_data.model_number) + 1;
    if (len > 0) {
        gatts_add_chara_info_t character;
        bts_sample_data_to_uuid_len2(BLE_PRO_DIS_MODEL_NUMBER_UUID16, &character.chara_uuid);
        character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
        character.properties = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP;
        character.value_len = (uint16_t)len;
        character.value = (uint8_t *)bts_sample_mem_new(sizeof(char) * len);
        if (character.value == NULL) {
            return;
        }
        if (memcpy_s(character.value, len, g_ble_pro_dis_data.model_number, len) != EOK) {
            bts_sample_error_log("memcpy_s fail.");
            return;
        }
        gatts_add_characteristic(server_id, srvc_handle, &character);
        bts_sample_mem_free(character.value);
    }
}

static void ble_pro_dis_add_character_serial_number(uint8_t server_id, uint16_t srvc_handle)
{
    size_t len = 0;
    if (g_ble_pro_dis_data.serial_number == NULL) {
        bts_sample_error_log("dis server serial number is not support!\r\n");
        return;
    }
    len = strlen(g_ble_pro_dis_data.serial_number) + 1;
    if (len > 0) {
        gatts_add_chara_info_t character;
        bts_sample_data_to_uuid_len2(BLE_PRO_DIS_SERIAL_NUMBER_UUID16, &character.chara_uuid);
        character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
        character.properties = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP;
        character.value_len = (uint16_t)len;
        character.value = (uint8_t *)bts_sample_mem_new(sizeof(char) * len);
        if (character.value == NULL) {
            return;
        }
        if (memcpy_s(character.value, len, g_ble_pro_dis_data.serial_number, len) != EOK) {
            bts_sample_error_log("memcpy_s fail.");
            return;
        }
        gatts_add_characteristic(server_id, srvc_handle, &character);
        bts_sample_mem_free(character.value);
    }
}

static void ble_pro_dis_add_character_firmware_revision(uint8_t server_id, uint16_t srvc_handle)
{
    size_t len = 0;
    if (g_ble_pro_dis_data.firmware_revision == NULL) {
        bts_sample_error_log("dis server firmware_revision is not support!\r\n");
        return;
    }
    len = strlen(g_ble_pro_dis_data.firmware_revision) + 1;
    if (len > 0) {
        gatts_add_chara_info_t character;
        bts_sample_data_to_uuid_len2(BLE_PRO_DIS_FIRMWARE_REVISION_UUID16, &character.chara_uuid);
        character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
        character.properties = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP;
        character.value_len = (uint16_t)len;
        character.value = (uint8_t *)bts_sample_mem_new(sizeof(char) * len);
        if (character.value == NULL) {
            return;
        }
        if (memcpy_s(character.value, len, g_ble_pro_dis_data.firmware_revision, len) != EOK) {
            bts_sample_error_log("memcpy_s fail.");
            return;
        }
        gatts_add_characteristic(server_id, srvc_handle, &character);
        bts_sample_mem_free(character.value);
    }
}

static void ble_pro_dis_add_character_hardware_revision(uint8_t server_id, uint16_t srvc_handle)
{
    size_t len = 0;
    if (g_ble_pro_dis_data.hardware_revision == NULL) {
        bts_sample_error_log("dis server hardware revision is not support!\r\n");
        return;
    }
    len = strlen(g_ble_pro_dis_data.hardware_revision) + 1;
    if (len > 0) {
        gatts_add_chara_info_t character;
        bts_sample_data_to_uuid_len2(BLE_PRO_DIS_HARDWARE_REVISION_UUID16, &character.chara_uuid);
        character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
        character.properties = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP;
        character.value_len = (uint16_t)len;
        character.value = (uint8_t *)bts_sample_mem_new(sizeof(char) * len);
        if (character.value == NULL) {
            return;
        }
        if (memcpy_s(character.value, len, g_ble_pro_dis_data.hardware_revision, len) != EOK) {
            bts_sample_error_log("memcpy_s fail.");
            return;
        }
        gatts_add_characteristic(server_id, srvc_handle, &character);
        bts_sample_mem_free(character.value);
    }
}

static void ble_pro_dis_add_character_software_revision(uint8_t server_id, uint16_t srvc_handle)
{
    size_t len = 0;
    if (g_ble_pro_dis_data.software_revision == NULL) {
        bts_sample_error_log("dis server software revision is not support!\r\n");
        return;
    }
    len = strlen(g_ble_pro_dis_data.software_revision) + 1;
    if (len > 0) {
        gatts_add_chara_info_t character;
        bts_sample_data_to_uuid_len2(BLE_PRO_DIS_SOFTWARE_REVISION_UUID16, &character.chara_uuid);
        character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
        character.properties = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP;
        character.value_len = (uint16_t)len;
        character.value = (uint8_t *)bts_sample_mem_new(sizeof(char) * len);
        if (character.value == NULL) {
            return;
        }
        if (memcpy_s(character.value, len, g_ble_pro_dis_data.software_revision, len) != EOK) {
            bts_sample_error_log("memcpy_s fail.");
            return;
        }
        gatts_add_characteristic(server_id, srvc_handle, &character);
        bts_sample_mem_free(character.value);
    }
}

static void ble_pro_dis_add_character_manufacturer_name(uint8_t server_id, uint16_t srvc_handle)
{
    size_t len = 0;
    if (g_ble_pro_dis_data.manufacturer_name == NULL) {
        bts_sample_error_log("dis server manufacturer name is not support!\r\n");
        return;
    }
    len = strlen(g_ble_pro_dis_data.manufacturer_name) + 1;
    if (len > 0) {
        gatts_add_chara_info_t character;
        bts_sample_data_to_uuid_len2(BLE_PRO_DIS_MANUFACTURER_NAME_UUID16, &character.chara_uuid);
        character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
        character.properties = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP;
        character.value_len = (uint16_t)len;
        character.value = (uint8_t *)bts_sample_mem_new(sizeof(char) * len);
        if (character.value == NULL) {
            return;
        }
        if (memcpy_s(character.value, len, g_ble_pro_dis_data.manufacturer_name, len) != EOK) {
            bts_sample_error_log("memcpy_s fail.");
            return;
        }
        gatts_add_characteristic(server_id, srvc_handle, &character);
        bts_sample_mem_free(character.value);
    }
}

static void ble_pro_dis_add_character_ieee_rcdl(uint8_t server_id, uint16_t srvc_handle)
{
    size_t len = 0;
    if (g_ble_pro_dis_data.ieee_regulatory_certification_data_list == NULL) {
        bts_sample_error_log("dis server ieee regulatory certification data list is not support!\r\n");
        return;
    }
    len = strlen(g_ble_pro_dis_data.ieee_regulatory_certification_data_list) + 1;
    if (len > 0) {
        gatts_add_chara_info_t character;
        bts_sample_data_to_uuid_len2(BLE_PRO_DIS_IEEE_REGULATORY_CERTIFICATION_DATA_LIST_UUID16, &character.chara_uuid);
        character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
        character.properties = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP;
        character.value_len = (uint16_t)len;
        character.value = (uint8_t *)bts_sample_mem_new(sizeof(char) * len);
        if (character.value == NULL) {
            return;
        }
        if (memcpy_s(character.value, len, g_ble_pro_dis_data.ieee_regulatory_certification_data_list, len) != EOK) {
            bts_sample_error_log("memcpy_s fail.");
            return;
        }
        gatts_add_characteristic(server_id, srvc_handle, &character);
        bts_sample_mem_free(character.value);
    }
}

static void ble_pro_dis_pnp_id_encode(const ble_pro_dis_pnp_id * pnp_id, uint8_t *buffer_encoded, uint8_t buff_len)
{
    uint8_t len = 0;
    if (pnp_id == NULL || buffer_encoded == NULL || buff_len < BLE_PRO_DIS_PNP_ID_LEN) {
        return;
    }
    buffer_encoded[len++] = pnp_id->vendor_id_source;
    buffer_encoded[len++] = (uint8_t)((pnp_id->vendor_id & 0x00FF) >> 0);
    buffer_encoded[len++] = (uint8_t)((pnp_id->vendor_id & 0xFF00) >> BT_NUM8);
    buffer_encoded[len++] = (uint8_t)((pnp_id->product_id & 0x00FF) >> 0);
    buffer_encoded[len++] = (uint8_t)((pnp_id->product_id & 0xFF00) >> BT_NUM8);
    buffer_encoded[len++] = (uint8_t)((pnp_id->product_revision & 0x00FF) >> 0);
    buffer_encoded[len++] = (uint8_t)((pnp_id->product_revision & 0xFF00) >> BT_NUM8);
}

static void ble_pro_dis_add_character_pnp_id(uint8_t server_id, uint16_t srvc_handle)
{
    gatts_add_chara_info_t character;
    bts_sample_data_to_uuid_len2(BLE_PRO_DIS_PNP_ID_UUID16, &character.chara_uuid);
    character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
    character.properties = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP;
    character.value_len = BLE_PRO_DIS_PNP_ID_LEN;
    character.value = (uint8_t *)bts_sample_mem_new(sizeof(char) * BLE_PRO_DIS_PNP_ID_LEN);
    if (character.value == NULL) {
        return;
    }
    ble_pro_dis_pnp_id_encode(&g_ble_pro_dis_data.pnp_id, character.value, BLE_PRO_DIS_PNP_ID_LEN);
    gatts_add_characteristic(server_id, srvc_handle, &character);
    bts_sample_mem_free(character.value);
}

void ble_pro_dis_add_characters(uint8_t server_id, uint16_t srvc_handle)
{
    ble_pro_dis_add_character_system_id(server_id, srvc_handle);
    ble_pro_dis_add_character_model_number(server_id, srvc_handle);
    ble_pro_dis_add_character_serial_number(server_id, srvc_handle);
    ble_pro_dis_add_character_firmware_revision(server_id, srvc_handle);
    ble_pro_dis_add_character_hardware_revision(server_id, srvc_handle);
    ble_pro_dis_add_character_software_revision(server_id, srvc_handle);
    ble_pro_dis_add_character_manufacturer_name(server_id, srvc_handle);
    ble_pro_dis_add_character_ieee_rcdl(server_id, srvc_handle);
    ble_pro_dis_add_character_pnp_id(server_id, srvc_handle);
}

void ble_pro_dis_server_init(void)
{
    bts_sample_info_log("[ACore] ble_pro_dis_server_init beginning add service\r\n");
    ble_pro_dis_add_service();
}
