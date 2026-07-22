/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */
#include <stdint.h>
#include <stddef.h>
#include "osal_addr.h"
#include "common_def.h"
#include "sample_common.h"
#include "ble_pro_dis_client.h"
#include "ble_pro_dis_server.h"
#include "ble_gatts_callbacks.h"
#include "sample_ble_dis_profile.h"

static void bt_at_cmd_dis_srv_register_cbks(int argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    ble_server_register_gatt_callbacks();
    return;
}

static void bt_at_cmd_dis_srv_init(int argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    ble_pro_dis_server_init();
    return;
}

static void bt_at_cmd_dis_srv_get_manu_name(int argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    char *data = NULL;
    data = ble_pro_dis_server_manufacturer_name();
    if (data == NULL) {
        bts_sample_error_log("local manufacturer name is null\r\n");
        return;
    }
    bts_sample_info_log("local manufacturer name is %s\r\n", data);
    return;
}

static void bt_at_cmd_dis_srv_set_manu_name(int argc, char *argv[])
{
    uint8_t ret = 0;
    uint16_t len = 0;
    char *data = NULL;
    bts_sample_info_log("bt_at_cmd_dis_srv_set_manu_name argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_srv_set_manu_name invalid param number:%d\r\n", argc);
        return;
    }
    len = (uint16_t)atoi(argv[0]);
    data = bts_sample_mem_new(sizeof(char) * (len + 1));
    if (memcpy_s(data, sizeof(char) * (len + 1), argv[1], len) != EOK) {
        bts_sample_error_log("bts_at_cmd_dis_srv_set_manu_name memcpy_s failed\r\n");
        return;
    }
    data[len] = '\0';
    (void)ble_pro_dis_server_manufacturer_name_set(data);
    return;
}

static void bt_at_cmd_dis_srv_get_model_num(int argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    char *data = NULL;
    data = ble_pro_dis_server_model_number();
    if (data == NULL) {
        bts_sample_error_log("local model number is null\r\n");
        return;
    }
    bts_sample_info_log("local model number is %s\r\n", data);
    return;
}

static void bt_at_cmd_dis_srv_set_model_num(int argc, char *argv[])
{
    uint8_t ret = 0;
    uint16_t len = 0;
    char *data = NULL;
    bts_sample_info_log("bt_at_cmd_dis_srv_get_model_num argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_srv_set_model_num invalid param number:%d\r\n", argc);
        return;
    }
    len = (uint16_t)atoi(argv[0]);
    data = bts_sample_mem_new(sizeof(char) * len + 1);
    if (memcpy_s(data, sizeof(char) * (len + 1), argv[1], len) != EOK) {
        bts_sample_error_log("bts_at_cmd_dis_srv_set_model_num memcpy_s failed\r\n");
        return;
    }
    data[len] = '\0';
    (void)ble_pro_dis_server_model_number_set(data);
    return;
}

static void bt_at_cmd_dis_srv_get_serial_num(int argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    char *data = NULL;
    data = ble_pro_dis_server_serial_number();
    if (data == NULL) {
        bts_sample_error_log("local serial number is null\r\n");
        return;
    }
    bts_sample_info_log("local serial number is %s\r\n", data);
    return;
}

static void bt_at_cmd_dis_srv_set_serial_num(int argc, char *argv[])
{
    uint8_t ret = 0;
    uint16_t len = 0;
    char *data = NULL;
    bts_sample_info_log("bt_at_cmd_dis_srv_set_serial_num argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_srv_set_serial_num invalid param number:%d\r\n", argc);
        return;
    }
    len = (uint16_t)atoi(argv[0]);
    data = bts_sample_mem_new(sizeof(char) * len + 1);
    if (memcpy_s(data, sizeof(char) * (len + 1), argv[1], len) != EOK) {
        bts_sample_error_log("bts_at_cmd_dis_srv_set_serial_num memcpy_s failed\r\n");
        return;
    }
    data[len] = '\0';
    (void)ble_pro_dis_server_serial_number_set(data);
    return;
}

static void bt_at_cmd_dis_srv_get_hw_revision(int argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    char *data = NULL;
    data = ble_pro_dis_server_hardware_revision();
    if (data == NULL) {
        bts_sample_error_log("local hardware revision is null\r\n");
        return;
    }
    bts_sample_info_log("local hardware revision is %s\r\n", data);
    return;
}

static void bt_at_cmd_dis_srv_set_hw_revision(int argc, char *argv[])
{
    uint8_t ret = 0;
    uint16_t len = 0;
    char *data = NULL;
    bts_sample_info_log("bt_at_cmd_dis_srv_set_hw_revision argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_srv_set_hw_revision invalid param number:%d\r\n", argc);
        return;
    }
    len = (uint16_t)atoi(argv[0]);
    data = bts_sample_mem_new(sizeof(char) * len + 1);
    if (memcpy_s(data, sizeof(char) * (len + 1), argv[1], len) != EOK) {
        bts_sample_error_log("bts_at_cmd_dis_srv_get_manu_name memcpy_s failed\r\n");
        return;
    }
    data[len] = '\0';
    (void)ble_pro_dis_server_hardware_revision_set(data);
    return;
}

static void bt_at_cmd_dis_srv_get_fw_revision(int argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    char *data = NULL;
    data = ble_pro_dis_server_firmware_revision();
    if (data == NULL) {
        bts_sample_error_log("local firmware revision is null\r\n");
        return;
    }
    bts_sample_info_log("local firmware revision is %s\r\n", data);
    return;
}

static void bt_at_cmd_dis_srv_set_fw_revision(int argc, char *argv[])
{
    uint8_t ret = 0;
    uint16_t len = 0;
    char *data = NULL;
    bts_sample_info_log("bt_at_cmd_dis_srv_set_fw_revision argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_srv_set_fw_revision invalid param number:%d\r\n", argc);
        return;
    }
    len = (uint16_t)atoi(argv[0]);
    data = bts_sample_mem_new(sizeof(char) * len + 1);
    if (memcpy_s(data, sizeof(char) * (len + 1), argv[1], len) != EOK) {
        bts_sample_error_log("bts_at_cmd_dis_srv_set_fw_revision memcpy_s failed\r\n");
        return;
    }
    data[len] = '\0';
    (void)ble_pro_dis_server_firmware_revision_set(data);
    return;
}

static void bt_at_cmd_dis_srv_get_sw_revision(int argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    char *data = NULL;
    data = ble_pro_dis_server_software_revision();
    if (data == NULL) {
        bts_sample_error_log("local software revision is null\r\n");
        return;
    }
    bts_sample_info_log("local software revision is %s\r\n", data);
    return;
}

static void bt_at_cmd_dis_srv_set_sw_revision(int argc, char *argv[])
{
    uint8_t ret = 0;
    uint16_t len = 0;
    char *data = NULL;
    bts_sample_info_log("bt_at_cmd_dis_srv_set_sw_revision argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_srv_set_sw_revision invalid param number:%d\r\n", argc);
        return;
    }
    len = (uint16_t)atoi(argv[0]);
    data = bts_sample_mem_new(sizeof(char) * len + 1);
    if (memcpy_s(data, sizeof(char) * (len + 1), argv[1], len) != EOK) {
        bts_sample_error_log("bts_at_cmd_dis_srv_set_sw_revision memcpy_s failed\r\n");
        return;
    }
    data[len] = '\0';
    (void)ble_pro_dis_server_software_revision_set(data);
    return;
}

static void bt_at_cmd_dis_srv_get_system_id(int argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    char *data = NULL;
    data = ble_pro_dis_server_system_id();
    if (data == NULL) {
        bts_sample_error_log("local system id is null\r\n");
        return;
    }
    bts_sample_info_log("local system id is %s\r\n", data);
    return;
}

static void bt_at_cmd_dis_srv_set_system_id(int argc, char *argv[])
{
    uint8_t ret = 0;
    uint16_t len = 0;
    char *data = NULL;
    bts_sample_info_log("bt_at_cmd_dis_srv_set_system_id argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_srv_set_system_id invalid param number:%d\r\n", argc);
        return;
    }
    len = (uint16_t)atoi(argv[0]);
    data = bts_sample_mem_new(sizeof(char) * len + 1);
    if (memcpy_s(data, sizeof(char) * (len + 1), argv[1], len) != EOK) {
        bts_sample_error_log("bts_at_cmd_dis_srv_set_system_id memcpy_s failed\r\n");
        return;
    }
    data[len] = '\0';
    (void)ble_pro_dis_server_system_id_set(data);
    return;
}

static void bt_at_cmd_dis_srv_get_ieee_rcdl(int argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    char *data = NULL;
    data = ble_pro_dis_server_ieee_regulatory_certification_data_list();
    if (data == NULL) {
        bts_sample_error_log("local ieee regulatory certification data list is null\r\n");
        return;
    }
    bts_sample_info_log("local ieee regulatory certification data list is %s\r\n", data);
    return;
}

static void bt_at_cmd_dis_srv_set_ieee_rcdl(int argc, char *argv[])
{
    uint8_t ret = 0;
    uint16_t len = 0;
    char *data = NULL;
    bts_sample_info_log("bt_at_cmd_dis_srv_set_ieee_rcdl argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_srv_set_ieee_rcdl invalid param number:%d\r\n", argc);
        return;
    }
    len = (uint16_t)atoi(argv[0]);
    data = bts_sample_mem_new(sizeof(char) * len + 1);
    if (memcpy_s(data, sizeof(char) * (len + 1), argv[1], len) != EOK) {
        bts_sample_error_log("bts_at_cmd_dis_srv_set_ieee_rcdl memcpy_s failed\r\n");
        return;
    }
    data[len] = '\0';
    (void)ble_pro_dis_server_ieee_regulatory_certification_data_list_set(data);
    return;
}

static void bt_at_cmd_dis_srv_get_pnp_id(int argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    ble_pro_dis_pnp_id *data = NULL;
    data = ble_pro_dis_server_pnp_id();
    if (data == NULL) {
        bts_sample_error_log("local pnp id is null\r\n");
        return;
    }
    bts_sample_info_log("pnp id info:\r\nvendor_id_source:0x%x\r\nvendor_id:0x%x\r\nproduct_id:0x%x\r\n"
        "product_revision:0x%x\r\n", data->vendor_id_source, data->vendor_id, data->product_id, data->product_revision);
    return;
}

static void bt_at_cmd_dis_srv_set_pnp_id(int argc, char *argv[])
{
    uint8_t ret = 0;
    uint8_t vendor_id_source;
    uint16_t vendor_id, product_id, product_revision;
    bts_sample_info_log("bt_at_cmd_dis_srv_set_pnp_id argc:%d\r\n", argc);
    if (argc != BT_NUM4) {
        bts_sample_error_log("bt_at_cmd_dis_srv_set_pnp_id invalid param number:%d\r\n", argc);
        return;
    }
    vendor_id_source = (uint8_t)atoi(argv[0]);
    vendor_id = (uint16_t)atoi(argv[BT_NUM1]);
    product_id = (uint16_t)atoi(argv[BT_NUM2]);
    product_revision = (uint16_t)atoi(argv[BT_NUM3]);
    (void)ble_pro_dis_server_pnp_id_set(vendor_id_source, vendor_id, product_id, product_revision);
    return;
}

/*
 * Input: 2 params
 * <client_id> <conn_id>
 */
static void bt_at_cmd_dis_cln_discovery_service(int argc, char *argv[])
{
    bts_sample_info_log("bt_at_cmd_dis_cln_discovery_service argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_cln_discovery_service argc:%d\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);
    errcode_t ret = ble_dis_client_discovery_service(client_id, conn_id);
    bts_sample_info_log("dis client discovery service result:%u\r\n", ret);
    return;
}

static void dis_cln_read_character_callback(uint8_t err_code, uint16_t uuid, uint16_t val_len, uint8_t *val)
{
    uint8_t *chara_val = NULL;
    bts_sample_info_log("DIS Client read remote character of uuid:0x%x error_code:%u\r\n", uuid, err_code);
    if (val == NULL || val_len == 0) {
        bts_sample_error_log("character value len:%hu\r\n", val_len);
        return;
    }
    chara_val = bts_sample_mem_new(val_len + 1);
    if (chara_val == NULL) {
        bts_sample_error_log("character value malloc fail, len:%hu\r\n", val_len + 1);
        return;
    }
    if (memcpy_s(chara_val, val_len + 1, val, val_len) != EOK) {
        bts_sample_error_log("character value memcpy_s fail\r\n");
        return;
    }
    chara_val[val_len] = '\0';
    bts_sample_info_log("character value len:%hu val:%s\r\n", val_len, chara_val);
    bts_sample_mem_free(chara_val);
}

static void dis_cln_read_pnp_id_callback(uint8_t err_code, uint16_t uuid, uint16_t val_len, uint8_t *val)
{
    uint8_t vendor_id_source, low, high;
    uint16_t vendor_id, product_id, product_revision;
    uint16_t idx = 0;
    bts_sample_info_log("DIS Client read remote character of uuid:0x%x error_code:%u\r\n", uuid, err_code);
    if (val == NULL || val_len == 0) {
        bts_sample_error_log("character value len:%hu\r\n", val_len);
        return;
    }
    if (val_len != sizeof(vendor_id_source) + sizeof(vendor_id) + sizeof(product_id) + sizeof(product_revision)) {
        bts_sample_error_log("Invalid Pnp_id value len:%hu.\r\n", val_len);
        return;
    }
    vendor_id_source = val[idx++];
    low = val[idx++];
    high = val[idx++];
    vendor_id = makeu16(low, high);
    low = val[idx++];
    high = val[idx++];
    product_id = makeu16(low, high);
    low = val[idx++];
    high = val[idx++];
    product_revision = makeu16(low, high);
    bts_sample_info_log("vendor_id_source:0x%02x vendor_id:0x%04x product_id:0x%04x "
        "product_revision:0x%04x\r\n", vendor_id_source, vendor_id, product_id, product_revision);
}

/*
 * Input: 2 params
 * <client_id> <conn_id>
 */
static void bt_at_cmd_dis_cln_rd_rmt_manu_name(int argc, char *argv[])
{
    dis_client_character_status result;
    bts_sample_info_log("bt_at_cmd_dis_cln_rd_rmt_manu_name argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_cln_rd_rmt_manu_name argc:%d\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);

    result = ble_pro_dis_read_remote_manufacturer_name(client_id, conn_id, dis_cln_read_character_callback);
    switch (result) {
        case CHARACTER_READ_OK:
            bts_sample_info_log("remote manufacturer name read ok\r\n");
            break;
        case CHARACTER_NOT_SUPPORT:
            bts_sample_info_log("remote manufacturer name not support\r\n");
            break;
        case CHARACTER_NOT_READABLE:
            bts_sample_info_log("remote manufacturer name not readable\r\n");
            break;
        case CHARACTER_READ_TIMEOUT:
            bts_sample_info_log("remote manufacturer name read timeout\r\n");
            break;
    }

    return;
}

/*
 * Input: 2 params
 * <client_id> <conn_id>
 */
static void bt_at_cmd_dis_cln_rd_rmt_model_num(int argc, char *argv[])
{
    dis_client_character_status result;

    bts_sample_info_log("bt_at_cmd_dis_cln_rd_rmt_model_num argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_cln_rd_rmt_model_num argc:%d\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);

    result = ble_pro_dis_read_remote_model_number(client_id, conn_id, dis_cln_read_character_callback);
    switch (result) {
        case CHARACTER_READ_OK:
            bts_sample_info_log("remote model number read ok\r\n");
            break;
        case CHARACTER_NOT_SUPPORT:
            bts_sample_info_log("remote model number not support\r\n");
            break;
        case CHARACTER_NOT_READABLE:
            bts_sample_info_log("remote model number not readable\r\n");
            break;
        case CHARACTER_READ_TIMEOUT:
            bts_sample_info_log("remote model number read timeout\r\n");
            break;
    }

    return;
}

/*
 * Input: 2 params
 * <client_id> <conn_id>
 */
static void bt_at_cmd_dis_cln_rd_rmt_serial_num(int argc, char *argv[])
{
    dis_client_character_status result;

    bts_sample_info_log("bt_at_cmd_dis_cln_rd_rmt_serial_num argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_cln_rd_rmt_serial_num argc:%d\r\n", argc);
        return;
    }

    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);

    result = ble_pro_dis_read_remote_serial_number(client_id, conn_id, dis_cln_read_character_callback);

    switch (result) {
        case CHARACTER_READ_OK:
            bts_sample_info_log("remote serial number read ok\r\n");
            break;
        case CHARACTER_NOT_SUPPORT:
            bts_sample_info_log("remote serial number not support\r\n");
            break;
        case CHARACTER_NOT_READABLE:
            bts_sample_info_log("remote serial number not readable\r\n");
            break;
        case CHARACTER_READ_TIMEOUT:
            bts_sample_info_log("remote serial number read timeout\r\n");
            break;
    }
    return;
}

/*
 * Input: 2 params
 * <client_id> <conn_id>
 */
static void bt_at_cmd_dis_cln_rd_rmt_hw_revision(int argc, char *argv[])
{
    dis_client_character_status result;

    bts_sample_info_log("bt_at_cmd_dis_cln_rd_rmt_hw_revision argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_cln_rd_rmt_hw_revision argc:%d\r\n", argc);
        return;
    }

    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);

    result = ble_pro_dis_read_remote_hardware_revision(client_id, conn_id, dis_cln_read_character_callback);
    switch (result) {
        case CHARACTER_READ_OK:
            bts_sample_info_log("remote hardware revision read ok\r\n");
            break;
        case CHARACTER_NOT_SUPPORT:
            bts_sample_info_log("remote hardware revision not support\r\n");
            break;
        case CHARACTER_NOT_READABLE:
            bts_sample_info_log("remote hardware revision not readable\r\n");
            break;
        case CHARACTER_READ_TIMEOUT:
            bts_sample_info_log("remote hardware revision read timeout\r\n");
            break;
    }
    return;
}

/*
 * Input: 2 params
 * <client_id> <conn_id>
 */
static void bt_at_cmd_dis_cln_rd_rmt_fw_revision(int argc, char *argv[])
{
    dis_client_character_status result;

    bts_sample_info_log("bt_at_cmd_dis_cln_rd_rmt_fw_revision argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_cln_rd_rmt_fw_revision argc:%d\r\n", argc);
        return;
    }

    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);

    result = ble_pro_dis_read_remote_firmware_revision(client_id, conn_id, dis_cln_read_character_callback);
    switch (result) {
        case CHARACTER_READ_OK:
            bts_sample_info_log("remote firmware revision read ok\r\n");
            break;
        case CHARACTER_NOT_SUPPORT:
            bts_sample_info_log("remote firmware revision not support\r\n");
            break;
        case CHARACTER_NOT_READABLE:
            bts_sample_info_log("remote firmware revision not readable\r\n");
            break;
        case CHARACTER_READ_TIMEOUT:
            bts_sample_info_log("remote firmware revision read timeout\r\n");
            break;
    }

    return;
}

/*
 * Input: 2 params
 * <client_id> <conn_id>
 */
static void bt_at_cmd_dis_cln_rd_rmt_sw_revision(int argc, char *argv[])
{
    dis_client_character_status result;

    bts_sample_info_log("bt_at_cmd_dis_cln_rd_rmt_sw_revision argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_cln_rd_rmt_sw_revision argc:%d\r\n", argc);
        return;
    }

    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);

    result = ble_pro_dis_read_remote_software_revision(client_id, conn_id, dis_cln_read_character_callback);
    switch (result) {
        case CHARACTER_READ_OK:
            bts_sample_info_log("remote software revision read ok\r\n");
            break;
        case CHARACTER_NOT_SUPPORT:
            bts_sample_info_log("remote software revision not support\r\n");
            break;
        case CHARACTER_NOT_READABLE:
            bts_sample_info_log("remote software revision not readable\r\n");
            break;
        case CHARACTER_READ_TIMEOUT:
            bts_sample_info_log("remote software revision read timeout\r\n");
            break;
    }

    return;
}

/*
 * Input: 2 params
 * <client_id> <conn_id>
 */
static void bt_at_cmd_dis_cln_rd_rmt_system_id(int argc, char *argv[])
{
    dis_client_character_status result;

    bts_sample_info_log("bt_at_cmd_dis_cln_rd_rmt_system_id argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_cln_rd_rmt_system_id argc:%d\r\n", argc);
        return;
    }

    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);

    result = ble_pro_dis_read_remote_system_id(client_id, conn_id, dis_cln_read_character_callback);
    switch (result) {
        case CHARACTER_READ_OK:
            bts_sample_info_log("remote system id read ok\r\n");
            break;
        case CHARACTER_NOT_SUPPORT:
            bts_sample_info_log("remote system id not support\r\n");
            break;
        case CHARACTER_NOT_READABLE:
            bts_sample_info_log("remote system id not readable\r\n");
            break;
        case CHARACTER_READ_TIMEOUT:
            bts_sample_info_log("remote system id read timeout\r\n");
            break;
    }

    return;
}

/*
 * Input: 2 params
 * <client_id> <conn_id>
 */
static void bt_at_cmd_dis_cln_rd_rmt_ieee_rcdl(int argc, char *argv[])
{
    dis_client_character_status result;

    bts_sample_info_log("bt_at_cmd_dis_cln_rd_rmt_ieee_rcdl argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_cln_rd_rmt_ieee_rcdl argc:%d\r\n", argc);
        return;
    }

    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);

    result = ble_pro_dis_read_remote_ieee_rcdl(client_id, conn_id, dis_cln_read_character_callback);
    switch (result) {
        case CHARACTER_READ_OK:
            bts_sample_info_log("remote ieee regulatory certification data list read ok\r\n");
            break;
        case CHARACTER_NOT_SUPPORT:
            bts_sample_info_log("remote ieee regulatory certification data list not support\r\n");
            break;
        case CHARACTER_NOT_READABLE:
            bts_sample_info_log("remote ieee regulatory certification data list not readable\r\n");
            break;
        case CHARACTER_READ_TIMEOUT:
            bts_sample_info_log("remote ieee regulatory certification data list read timeout\r\n");
            break;
    }

    return;
}

/*
 * Input: 2 params
 * <client_id> <conn_id>
 */
static void bt_at_cmd_dis_cln_rd_rmt_pnp_id(int argc, char *argv[])
{
    dis_client_character_status result;

    bts_sample_info_log("bt_at_cmd_dis_cln_rd_rmt_pnp_id argc:%d\r\n", argc);
    if (argc != BT_NUM2) {
        bts_sample_error_log("bt_at_cmd_dis_cln_rd_rmt_pnp_id argc:%d\r\n", argc);
        return;
    }

    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);

    result = ble_pro_dis_read_remote_pnp_id(client_id, conn_id, dis_cln_read_pnp_id_callback);
    switch (result) {
        case CHARACTER_READ_OK:
            bts_sample_info_log("remote pnp id read ok\r\n");
            break;
        case CHARACTER_NOT_SUPPORT:
            bts_sample_info_log("remote pnp id not support\r\n");
            break;
        case CHARACTER_NOT_READABLE:
            bts_sample_info_log("remote pnp id not readable\r\n");
            break;
        case CHARACTER_READ_TIMEOUT:
            bts_sample_info_log("remote pnp id read timeout\r\n");
            break;
    }

    return;
}

typedef void (*ble_at_dis_cmd_handle_func)(int argc, char **argv);

typedef enum {
    BTSRV_SAMPLE_BLE_DIS_SRV_REG = 0,              /* 0 dis server callback reg */
    BTSRV_SAMPLE_BLE_DIS_SRV_INIT,                 /* 1 dis server init */
    BTSRV_SAMPLE_BLE_DIS_SRV_GET_MANU_NAME,        /* 2 dis server get manufacter name */
    BTSRV_SAMPLE_BLE_DIS_SRV_SET_MANU_NAME,        /* 3 dis server set manufacter name */
    BTSRV_SAMPLE_BLE_DIS_SRV_GET_MODEL_NUM,        /* 4 dis server get model number */
    BTSRV_SAMPLE_BLE_DIS_SRV_SET_MODEL_NUM = 5,    /* 5 dis server set model number */
    BTSRV_SAMPLE_BLE_DIS_SRV_GET_SERIAL_NUM,       /* 6 dis server get serial number */
    BTSRV_SAMPLE_BLE_DIS_SRV_SET_SERIAL_NUM,       /* 7 dis server set serial number */
    BTSRV_SAMPLE_BLE_DIS_SRV_GET_HW_REVISION,      /* 8 dis server get hardware revision */
    BTSRV_SAMPLE_BLE_DIS_SRV_SET_HW_REVISION,      /* 9 dis server set hardware revision */
    BTSRV_SAMPLE_BLE_DIS_SRV_GET_FW_REVISION = 10, /* 10 dis server get firmware revision */
    BTSRV_SAMPLE_BLE_DIS_SRV_SET_FW_REVISION,      /* 11 dis server set firmware revision */
    BTSRV_SAMPLE_BLE_DIS_SRV_GET_SW_REVISION,      /* 12 dis server get software revision */
    BTSRV_SAMPLE_BLE_DIS_SRV_SET_SW_REVISION,      /* 13 dis server set software revision */
    BTSRV_SAMPLE_BLE_DIS_SRV_GET_SYSTEM_ID,        /* 14 dis server get system id */
    BTSRV_SAMPLE_BLE_DIS_SRV_SET_SYSTEM_ID = 15,   /* 15 dis server set system id */
    BTSRV_SAMPLE_BLE_DIS_SRV_GET_IEEE_RCDL,        /* 16 dis server get ieee regulatory certification data list */
    BTSRV_SAMPLE_BLE_DIS_SRV_SET_IEEE_RCDL,        /* 17 dis server set ieee regulatory certification data list */
    BTSRV_SAMPLE_BLE_DIS_SRV_GET_PNP_ID,           /* 18 dis server get pnp id */
    BTSRV_SAMPLE_BLE_DIS_SRV_SET_PNP_ID,           /* 19 dis server set pnp id */
} ble_at_dis_srv_cmd_id;

typedef struct {
    int id;
    ble_at_dis_cmd_handle_func func;
} ble_at_dis_srv_cmd_hdl_info;

static ble_at_dis_srv_cmd_hdl_info g_ble_at_dis_srv_cmd_hdls[] = {
    { BTSRV_SAMPLE_BLE_DIS_SRV_REG, bt_at_cmd_dis_srv_register_cbks },
    { BTSRV_SAMPLE_BLE_DIS_SRV_INIT, bt_at_cmd_dis_srv_init },
    { BTSRV_SAMPLE_BLE_DIS_SRV_GET_MANU_NAME, bt_at_cmd_dis_srv_get_manu_name },
    { BTSRV_SAMPLE_BLE_DIS_SRV_SET_MANU_NAME, bt_at_cmd_dis_srv_set_manu_name },
    { BTSRV_SAMPLE_BLE_DIS_SRV_GET_MODEL_NUM, bt_at_cmd_dis_srv_get_model_num },
    { BTSRV_SAMPLE_BLE_DIS_SRV_SET_MODEL_NUM, bt_at_cmd_dis_srv_set_model_num },
    { BTSRV_SAMPLE_BLE_DIS_SRV_GET_SERIAL_NUM, bt_at_cmd_dis_srv_get_serial_num },
    { BTSRV_SAMPLE_BLE_DIS_SRV_SET_SERIAL_NUM, bt_at_cmd_dis_srv_set_serial_num },
    { BTSRV_SAMPLE_BLE_DIS_SRV_GET_HW_REVISION, bt_at_cmd_dis_srv_get_hw_revision },
    { BTSRV_SAMPLE_BLE_DIS_SRV_SET_HW_REVISION, bt_at_cmd_dis_srv_set_hw_revision },
    { BTSRV_SAMPLE_BLE_DIS_SRV_GET_FW_REVISION, bt_at_cmd_dis_srv_get_fw_revision },
    { BTSRV_SAMPLE_BLE_DIS_SRV_SET_FW_REVISION, bt_at_cmd_dis_srv_set_fw_revision },
    { BTSRV_SAMPLE_BLE_DIS_SRV_GET_SW_REVISION, bt_at_cmd_dis_srv_get_sw_revision },
    { BTSRV_SAMPLE_BLE_DIS_SRV_SET_SW_REVISION, bt_at_cmd_dis_srv_set_sw_revision },
    { BTSRV_SAMPLE_BLE_DIS_SRV_GET_SYSTEM_ID, bt_at_cmd_dis_srv_get_system_id },
    { BTSRV_SAMPLE_BLE_DIS_SRV_SET_SYSTEM_ID, bt_at_cmd_dis_srv_set_system_id },
    { BTSRV_SAMPLE_BLE_DIS_SRV_GET_IEEE_RCDL, bt_at_cmd_dis_srv_get_ieee_rcdl },
    { BTSRV_SAMPLE_BLE_DIS_SRV_SET_IEEE_RCDL, bt_at_cmd_dis_srv_set_ieee_rcdl },
    { BTSRV_SAMPLE_BLE_DIS_SRV_GET_PNP_ID, bt_at_cmd_dis_srv_get_pnp_id },
    { BTSRV_SAMPLE_BLE_DIS_SRV_SET_PNP_ID, bt_at_cmd_dis_srv_set_pnp_id },
};

void ble_at_dis_srv_cmd(int cmd_id, char **params, int params_cnt)
{
    uint32_t hdl_cnt = sizeof(g_ble_at_dis_srv_cmd_hdls) / sizeof(ble_at_dis_srv_cmd_hdl_info);
    for (uint32_t i = 0; i < hdl_cnt; i++) {
        if (g_ble_at_dis_srv_cmd_hdls[i].id == cmd_id && g_ble_at_dis_srv_cmd_hdls[i].func != NULL) {
            g_ble_at_dis_srv_cmd_hdls[i].func(params_cnt, params);
            return;
        }
    }
}

typedef enum {
    BTSRV_SAMPLE_BLE_DIS_CLN_DIS_SVC = 1,           /* 1 dis client discovery service */
    BTSRV_SAMPLE_BLE_DIS_CLN_RD_MANU_NAME,          /* 2 dis client read remote manufacter name */
    BTSRV_SAMPLE_BLE_DIS_CLN_RD_MODEL_NUM,          /* 3 dis client read remote model number */
    BTSRV_SAMPLE_BLE_DIS_CLN_RD_SERIAL_NUM,         /* 4 dis client read remote serial number */
    BTSRV_SAMPLE_BLE_DIS_CLN_RD_HW_REVISION = 5,    /* 5 dis client read remote hardware revision */
    BTSRV_SAMPLE_BLE_DIS_CLN_RD_FW_REVISION,        /* 6 dis client read remote firmware revision */
    BTSRV_SAMPLE_BLE_DIS_CLN_RD_SW_REVISION,        /* 7 dis client read remote software revision */
    BTSRV_SAMPLE_BLE_DIS_CLN_RD_SYSTEM_ID,          /* 8 dis client read remote system id */
    BTSRV_SAMPLE_BLE_DIS_CLN_RD_IEEE_RCDL = 9,      /* 9 dis client read
                                                        remote ieee regulatory certification data list */
    BTSRV_SAMPLE_BLE_DIS_CLN_RD_PNP_ID,             /* 10 dis client read remote pnp id */
} ble_at_dis_cln_cmd_id;

typedef struct {
    int id;
    ble_at_dis_cmd_handle_func func;
} ble_at_dis_cln_cmd_hdl_info;

static ble_at_dis_cln_cmd_hdl_info g_ble_at_dis_cln_cmd_hdls[] = {
    { BTSRV_SAMPLE_BLE_DIS_CLN_DIS_SVC, bt_at_cmd_dis_cln_discovery_service },
    { BTSRV_SAMPLE_BLE_DIS_CLN_RD_MANU_NAME, bt_at_cmd_dis_cln_rd_rmt_manu_name },
    { BTSRV_SAMPLE_BLE_DIS_CLN_RD_MODEL_NUM, bt_at_cmd_dis_cln_rd_rmt_model_num },
    { BTSRV_SAMPLE_BLE_DIS_CLN_RD_SERIAL_NUM, bt_at_cmd_dis_cln_rd_rmt_serial_num },
    { BTSRV_SAMPLE_BLE_DIS_CLN_RD_HW_REVISION, bt_at_cmd_dis_cln_rd_rmt_hw_revision },
    { BTSRV_SAMPLE_BLE_DIS_CLN_RD_FW_REVISION, bt_at_cmd_dis_cln_rd_rmt_fw_revision },
    { BTSRV_SAMPLE_BLE_DIS_CLN_RD_SW_REVISION, bt_at_cmd_dis_cln_rd_rmt_sw_revision },
    { BTSRV_SAMPLE_BLE_DIS_CLN_RD_SYSTEM_ID, bt_at_cmd_dis_cln_rd_rmt_system_id },
    { BTSRV_SAMPLE_BLE_DIS_CLN_RD_IEEE_RCDL, bt_at_cmd_dis_cln_rd_rmt_ieee_rcdl },
    { BTSRV_SAMPLE_BLE_DIS_CLN_RD_PNP_ID, bt_at_cmd_dis_cln_rd_rmt_pnp_id },
};

void ble_at_dis_cln_cmd(int cmd_id, char **params, int params_cnt)
{
    uint32_t hdl_cnt = sizeof(g_ble_at_dis_cln_cmd_hdls) / sizeof(ble_at_dis_cln_cmd_hdl_info);
    for (uint32_t i = 0; i < hdl_cnt; i++) {
        if (g_ble_at_dis_cln_cmd_hdls[i].id == cmd_id && g_ble_at_dis_cln_cmd_hdls[i].func != NULL) {
            g_ble_at_dis_cln_cmd_hdls[i].func(params_cnt, params);
            return;
        }
    }
}

