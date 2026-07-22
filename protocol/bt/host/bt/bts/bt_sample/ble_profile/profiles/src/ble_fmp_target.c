/*
 * Copyright (c) @CompanyNameMagicTag 2023. All rights reserved.
 * Description: FMP Locator Module.
 */
#include "errcode.h"
#include "soc_osal.h"
#include "sample_common.h"
#include "bts_gatt_stru.h"
#include "bts_gatt_server.h"
#include "bts_def.h"
#include "ble_fmp_target.h"

#define BLE_FMP_SERVER_ID 1

typedef struct fmp_target_ias {
    fmp_target_evt_handler_t evt_handler;
    uint32_t service_handle;
    uint16_t srvc_att_hdl;
    uint16_t chara_hdl;
    uint16_t chara_value_hdl;
    bool     is_srvc_started;
} fmp_target_ias_t;

static fmp_target_ias_t g_fmp_target_ias = {0};

static errcode_t fmp_target_create_ias_service(void)
{
    bt_uuid_t fmp_service_uuid = {0};
    bts_sample_data_to_uuid_len2(FMP_IMMEDIATE_ALERT_SERVICE_UUID, &fmp_service_uuid);
    return gatts_add_service(BLE_FMP_SERVER_ID, &fmp_service_uuid, true);
}

errcode_t fmp_target_init(fmp_target_evt_handler_t evt_hdl)
{
    if (evt_hdl == NULL) {
        bts_sample_error_log("Invalid parameter.\r\n");
        return ERRCODE_BT_PARAM_ERR;
    }

    g_fmp_target_ias.evt_handler = evt_hdl;
    g_fmp_target_ias.is_srvc_started = false;
    return fmp_target_create_ias_service();
}

errcode_t fmp_target_add_alc_characteristic(uint8_t server_id, uint16_t srvc_handle)
{
    errcode_t ret;
    bt_uuid_t fmp_alc_uuid = {0};
    bts_sample_data_to_uuid_len2(FMP_ALERT_LEVEL_CHARACTERISTIC_UUID, &fmp_alc_uuid);
    uint8_t default_value = FMP_ALERT_LEVEL_NO_ALERT;
    gatts_add_chara_info_t character;
    character.chara_uuid = fmp_alc_uuid;
    character.permissions = GATT_ATTRIBUTE_PERMISSION_READ;
    character.properties = GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP;
    character.value_len = (uint16_t)sizeof(uint8_t);
    character.value = &default_value;
    ret = gatts_add_characteristic(server_id, srvc_handle, &character);
    return ret;
}

void fmp_target_add_characteristic_cbk(uint16_t service_handle,
    uint16_t character_handle, uint16_t value_handle)
{
    bts_sample_info_log("fmp target add characteristic success, character_handle: 0x%x\r\n",
        character_handle);
    g_fmp_target_ias.srvc_att_hdl = service_handle;
    g_fmp_target_ias.chara_hdl = character_handle;
    g_fmp_target_ias.chara_value_hdl = value_handle;
}

void fmp_target_start_service_cbk(uint8_t server_id, uint16_t handle)
{
    if (server_id == 1 && handle == g_fmp_target_ias.srvc_att_hdl) {
        bts_sample_info_log("[ACore] fmp target start service success,service_att_handle:%hu\r\n", handle);
        g_fmp_target_ias.is_srvc_started = true;
    }
}

void fmp_target_write_req_cbk(uint8_t server_id, uint16_t handle, uint8_t value)
{
    if (server_id == 1 && handle == g_fmp_target_ias.chara_value_hdl) {
        bts_sample_info_log("[ACore] fmp target write req handle:%hu, value:%u\r\n", handle, value);
        if (g_fmp_target_ias.evt_handler != NULL) {
            g_fmp_target_ias.evt_handler(value);
        }
    }
}
