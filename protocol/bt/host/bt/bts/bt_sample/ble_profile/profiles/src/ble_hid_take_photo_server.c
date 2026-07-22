/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BT HID Service Server Module.
 */
#include "securec.h"
#include "osal_addr.h"
#include "bts_def.h"
#include "chip_definitions.h"
#include "sample_common.h"
#include "bts_gatt_stru.h"
#include "bts_gatt_server.h"
#include "ble_gatts_callbacks.h"
#include "ble_hid_take_photo_server.h"

/* HID information flag remote wakeup */
#define BLE_HID_INFO_FLAG_REMOTE_WAKE_UP_MSK 0x01
/* HID information flag normally connectable */
#define BLE_HID_INFO_FLAG_NORMALLY_CONNECTABLE_MSK 0x02
/* HID information country code */
#define BLE_HID_INFO_COUNTRY_CODE 0x00
/* HID spec version 1.11 */
#define BLE_HID_VERSION  0x0101
/* HID input report id */
#define BLE_HID_REPORT_ID 1
/* HID input report type */
#define BLE_REPORT_REFERENCE_REPORT_TYPE_INPUT_REPORT 1
/* HID output report type */
#define BLE_REPORT_REFERENCE_REPORT_TYPE_OUTPUT_REPORT 2
/* HID gatt server id */
#define BLE_HID_SERVER_ID 1
/* HID ble connect id */
#define BLE_SINGLE_LINK_CONNECT_ID 1
/* HID gatt attribute num */
#define BLE_HID_ATTRIBUTE_NUM 10
/* HID gatt notification flag, do not need confirm */
#define GATT_NOTIFICATION_FLAG 0
/* HID gatt indication flag, need confirm */
#define GATT_INDICATION_FLAG 1
#define INVALID_SERVER_ID 0

/* gatt server ID */
static uint8_t g_server_id = INVALID_SERVER_ID;

enum {
    /* HID information characteristic properties */
    HID_INFORMATION_PROPERTIES   = GATT_CHARACTER_PROPERTY_BIT_READ,
    /* HID protocol mode characteristic properties */
    HID_PROTOCOL_MODE_PROPERTIES = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP,
    /* HID report map characteristic properties */
    HID_REPORT_MAP_PROPERTIES    = GATT_CHARACTER_PROPERTY_BIT_READ,
    /* HID input report characteristic properties */
    HID_INPUT_REPORT_PROPERTIES  = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_NOTIFY |
                                   GATT_CHARACTER_PROPERTY_BIT_WRITE,
    /* HID output report characteristic properties */
    HID_OUTPUT_REPORT_PROPERTIES = GATT_CHARACTER_PROPERTY_BIT_READ | GATT_CHARACTER_PROPERTY_BIT_WRITE |
                                   GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP,
    /* HID control point characteristic properties */
    HID_CONTROL_POINT_PROPERTIES = GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP,
};

#define uint16_to_byte(n) ((uint8_t)(n)), ((uint8_t)((n) >> 8))

/* hid input report att handle */
uint32_t g_hid_input_report_att_hdl = 0;

uint32_t g_hid_service_handle = 0;

__attribute__((unused)) static errcode_t ble_hid_server_register_gatt_callbacks(void);

#if defined(CHIP_BRANDY)
/* Hid Report Map (Descriptor) */
uint8_t g_srv_hid_report_map[] = {
    0x05, 0x0C, /* Usage Page (Consumer) */
    0x09, 0x01, /* Usage (Consumer Control) */
    0xA1, 0x01, /* Collection (Application) */
    0x85, 0x01, /* Report ID (1) */
    0x09, 0xE9, /* Usage (Volume Increment) */
    0x09, 0xEA, /* Usage (Volume Decrement) */
    0x09, 0xE2, /* Usage (Mute) */
    0x09, 0x30, /* Usage (Power) */
    0x15, 0x01, /* Logical minimum (1) */
    0x25, 0x0C, /* Logical maximum (12) */
    0x75, 0x10, /* Report Size (16) */
    0x95, 0x01, /* Report Count (1) */
    0x81, 0x00, /* INPUT (Data,Array,Absolute,Bit Field) */
    0xC0,       /* End Collection */
};
#else
/* Hid Report Map (Descriptor) */
uint8_t g_srv_hid_report_map[] = {
    0x05, 0x01,       /* Usage Page (Generic Desktop) */
    0x09, 0x06,       /* Usage (Keyboard) */
    0xA1, 0x01,       /* Collection (Application) */
    0x85, 0x01,       /* Report Id (1) */
    0x05, 0x07,       /* Usage Page (Keyboard) */
    0x19, 0xE0,       /* Usage Minimum (Keyboard Left Control) */
    0x29, 0xE7,       /* Usage Maximum (Keyboard Right GUI) */
    0x15, 0x00,       /* Logical minimum (0) */
    0x25, 0x01,       /* Logical maximum (1) */
    0x75, 0x01,       /* Report Size (1) */
    0x95, 0x08,       /* Report Count (8) */
    0x81, 0x02,       /* Input (data,Value,Absolute,Bit Field) */
    0x95, 0x05,       /* Report Count (5) */
    0x05, 0x08,       /* Usage Page (LEDs) */
    0x19, 0x01,       /* Usage Minimum (Num Lock) */
    0x29, 0x05,       /* Usage Maximum (Kana) */
    0x91, 0x02,       /* Output (data,Value,Absolute,Non-volatile,Bit Field) */
    0x95, 0x01,       /* Report Count (1) */
    0x75, 0x03,       /* Report Size (3) */
    0x91, 0x01,       /* Output (Constant,Array,Absolute,Non-volatile,Bit Field) */
    0x95, 0x06,       /* Report Count (6) */
    0x75, 0x08,       /* Report Size (8) */
    0x15, 0x00,       /* Logical minimum (0) */
    0x26, 0xA4, 0x00, /* Logical maximum (164) */
    0x05, 0x07,       /* Usage Page (Keyboard) */
    0x19, 0x00,       /* Usage Minimum (No event indicated) */
    0x2A, 0xA4, 0x00, /* Usage Maximum (Keyboard ExSel) */
    0x81, 0x00,       /* Input (data,Array,Absolute,Bit Field) */
    0xC0,             /* End Collection */
};
#endif
/* 创建服务 */
static void ble_hid_add_service(void)
{
    bt_uuid_t hid_service_uuid = {0};
    bts_sample_data_to_uuid_len2(BLE_UUID_HUMAN_INTERFACE_DEVICE, &hid_service_uuid);
    gatts_add_service(BLE_HID_SERVER_ID, &hid_service_uuid, true);
}

/* 添加特征：HID information */
static void ble_hid_add_character_hid_information(uint8_t server_id, uint16_t srvc_handle)
{
    /* HID information value for test */
    static uint8_t hid_information_val[] = {uint16_to_byte(BLE_HID_VERSION),
        BLE_HID_INFO_COUNTRY_CODE,
        BLE_HID_INFO_FLAG_REMOTE_WAKE_UP_MSK | BLE_HID_INFO_FLAG_NORMALLY_CONNECTABLE_MSK};
    gatts_add_chara_info_t character;
    bts_sample_data_to_uuid_len2(BLE_UUID_HID_INFORMATION, &character.chara_uuid);
    character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
    character.properties = HID_INFORMATION_PROPERTIES;
    character.value_len = (uint16_t)sizeof(hid_information_val);
    character.value = hid_information_val;
    gatts_add_characteristic(server_id, srvc_handle, &character);
}

/* 添加特征：HID report map */
static void ble_hid_add_character_report_map(uint8_t server_id, uint16_t srvc_handle)
{
    gatts_add_chara_info_t character;
    bts_sample_data_to_uuid_len2(BLE_UUID_REPORT_MAP, &character.chara_uuid);
    character.permissions = GATT_ATTRIBUTE_PERMISSION_READ | GATT_ATTRIBUTE_PERMISSION_AUTHENTICATION_NEED;
    character.properties = HID_REPORT_MAP_PROPERTIES;
    character.value_len = (uint16_t)sizeof(g_srv_hid_report_map);
    character.value = g_srv_hid_report_map;
    gatts_add_characteristic(server_id, srvc_handle, &character);
}

/* 添加特征：HID control point */
static void ble_hid_add_character_hid_control_point(uint8_t server_id, uint16_t srvc_handle)
{
    /* HID control point value for test */
    static uint8_t control_point_val[] = {0x00, 0x00};
    gatts_add_chara_info_t character;
    bts_sample_data_to_uuid_len2(BLE_UUID_HID_CONTROL_POINT, &character.chara_uuid);
    character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
    character.properties = HID_CONTROL_POINT_PROPERTIES;
    character.value_len = (uint16_t)sizeof(control_point_val);
    character.value = control_point_val;
    gatts_add_characteristic(server_id, srvc_handle, &character);
}

/* 添加描述符：客户端特性配置 */
static void ble_hid_add_descriptor_ccc(uint8_t server_id, uint16_t srvc_handle)
{
    /* HID client characteristic configuration value for test */
    static uint8_t ccc_val[] = {0x00, 0x00};
    gatts_add_desc_info_t descriptor;
    bts_sample_data_to_uuid_len2(BLE_UUID_CLIENT_CHARACTERISTIC_CONFIGURATION, &descriptor.desc_uuid);
    descriptor.permissions = GATT_ATTRIBUTE_PERMISSION_READ | GATT_ATTRIBUTE_PERMISSION_WRITE;
    descriptor.value_len = (uint16_t)sizeof(ccc_val);
    descriptor.value = ccc_val;
    gatts_add_descriptor(server_id, srvc_handle, &descriptor);
}

/* 添加描述符：HID report reference */
static void ble_hid_add_descriptor_report_reference(uint8_t server_id, uint16_t srvc_handle, bool is_input_flag)
{
    /* HID output report reference value for test [report id 1, output] */
    static uint8_t report_reference_val_output[] = {BLE_HID_REPORT_ID, BLE_REPORT_REFERENCE_REPORT_TYPE_OUTPUT_REPORT};
    /* HID input report reference value for test  [report id 1, input] */
    static uint8_t report_reference_val_input[] = {BLE_HID_REPORT_ID, BLE_REPORT_REFERENCE_REPORT_TYPE_INPUT_REPORT};
    gatts_add_desc_info_t descriptor;
    bts_sample_data_to_uuid_len2(BLE_UUID_REPORT_REFERENCE, &descriptor.desc_uuid);
    descriptor.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
    if (is_input_flag) {
        descriptor.value = report_reference_val_input;
        descriptor.value_len = (uint16_t)sizeof(report_reference_val_input);
    } else {
        descriptor.value = report_reference_val_output;
        descriptor.value_len = (uint16_t)sizeof(report_reference_val_output);
    }
    gatts_add_descriptor(server_id, srvc_handle, &descriptor);
}

/* 添加特征：HID input report(device to host) */
static void ble_hid_add_character_input_report(uint8_t server_id, uint16_t srvc_handle)
{
    /* HID input report value  for test
    * input report format:
    * data0 | data1 | data2 | data3 | data4 | data5 | data6
    * E0~E7 | key   | key   | key   | key   | key   | key
    */
    static uint8_t input_report_value[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    gatts_add_chara_info_t character;
    bts_sample_data_to_uuid_len2(BLE_UUID_REPORT, &character.chara_uuid);
    character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
    character.properties = HID_INPUT_REPORT_PROPERTIES;
    character.value_len = (uint16_t)sizeof(input_report_value);
    character.value = input_report_value;
    gatts_add_characteristic(server_id, srvc_handle, &character);

    ble_hid_add_descriptor_ccc(server_id, srvc_handle);
    ble_hid_add_descriptor_report_reference(server_id, srvc_handle, true);
}

/* 添加特征：HID output report(host to device) */
static void ble_hid_add_character_output_report(uint8_t server_id, uint16_t srvc_handle)
{
    /* HID output report value for test */
    static uint8_t output_report_value[] = {0x00};
    gatts_add_chara_info_t character;
    bts_sample_data_to_uuid_len2(BLE_UUID_REPORT, &character.chara_uuid);
    character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
    character.properties = HID_OUTPUT_REPORT_PROPERTIES;
    character.value_len = (uint16_t)sizeof(output_report_value);
    character.value = output_report_value;
    gatts_add_characteristic(server_id, srvc_handle, &character);

    ble_hid_add_descriptor_report_reference(server_id, srvc_handle, false);
}

/* 添加特征：HID protocol mode */
static void ble_hid_add_character_protocol_mode(uint8_t server_id, uint16_t srvc_handle)
{
    /* HID protocol mode value for test */
    static uint8_t protocol_mode_val[] = {0x00, 0x00};
    gatts_add_chara_info_t character;
    bts_sample_data_to_uuid_len2(BLE_UUID_PROTOCOL_MODE, &character.chara_uuid);
    character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
    character.properties = HID_PROTOCOL_MODE_PROPERTIES;
    character.value_len = (uint16_t)sizeof(protocol_mode_val);
    character.value = protocol_mode_val;
    gatts_add_characteristic(server_id, srvc_handle, &character);
}

/* 添加HID服务的所有特征和描述符 */
void ble_hid_add_characters_and_descriptors(uint8_t server_id, uint16_t srvc_handle)
{
    g_hid_service_handle = srvc_handle;
    /* HID Information */
    ble_hid_add_character_hid_information(server_id, srvc_handle);
    /* Report Map */
    ble_hid_add_character_report_map(server_id, srvc_handle);
    /* Protocol Mode */
    ble_hid_add_character_protocol_mode(server_id, srvc_handle);
    /* Input Report */
    ble_hid_add_character_input_report(server_id, srvc_handle);
    /* Output Report */
    ble_hid_add_character_output_report(server_id, srvc_handle);
    /* HID Control Point */
    ble_hid_add_character_hid_control_point(server_id, srvc_handle);
}

/* 初始化HID device */
void ble_hid_server_init(void)
{
    /* HID server app uuid for test */
    static char server_app_uuid_for_test[] = {0x00, 0x00};
    ble_server_register_gatt_callbacks();
    bt_uuid_t app_uuid = {0};
    app_uuid.uuid_len = (uint8_t)sizeof(server_app_uuid_for_test);
    if (memcpy_s(app_uuid.uuid, BT_UUID_MAX_LEN,
        server_app_uuid_for_test, sizeof(server_app_uuid_for_test)) != EOK) {
        bts_sample_error_log("[hid][ERROR] uuid data to len2 memcpy fail.\n");
        return;
    }
    bts_sample_info_log("[hid][INFO]beginning add server\r\n");
    gatts_register_server(&app_uuid, &g_server_id); /* 注册gatt server */
    bts_sample_info_log("[hid][INFO]server_id:%d\r\n", g_server_id);
    bts_sample_info_log("[hid][INFO]beginning add service\r\n");
    ble_hid_add_service(); /* 添加HID服务 */
}

/* device向host发送数据：input report */
errcode_t ble_hid_server_send_input_report(const uint8_t *data, uint8_t len)
{
    gatts_ntf_ind_t param = {0};
    param.attr_handle = (uint16_t)g_hid_input_report_att_hdl;
    param.value_len = len;
    param.value = bts_sample_mem_new(len);
    if (param.value == NULL) {
        bts_sample_error_log("[hid][ERROR]send input report new fail\r\n");
        return ERRCODE_BT_MALLOC_FAIL;
    }
    if (memcpy_s(param.value, param.value_len, data, len) != EOK) {
        bts_sample_error_log("[hid][ERROR]send input report memcpy fail\r\n");
        bts_sample_mem_free(param.value);
        return ERRCODE_BT_FAIL;
    }
    gatts_notify_indicate(g_server_id, BLE_SINGLE_LINK_CONNECT_ID, &param);
    bts_sample_mem_free(param.value);
    return ERRCODE_BT_SUCCESS;
}

void ble_hid_add_characters_input_report_cbk(uint16_t srvc_handle)
{
    if ((srvc_handle == 0) || (g_hid_service_handle != srvc_handle)) {
        bts_sample_error_log("[hid][ERROR]add report error, service_handle:%d\r\n", srvc_handle);
        return;
    }
    g_hid_input_report_att_hdl = srvc_handle + BLE_HID_CHARACTER_INPUT_REPORT_VALUE_HANDLE_OFFSET;
}