/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: sdk gatt
 * Author: @CompanyNameTag
 * Create: 2022-03-03
 */
#ifndef BLE_ADVERTISING_H__
#define BLE_ADVERTISING_H__
#include <stdint.h>
#include <bth_stack_gatt.h>

#ifdef __cplusplus
extern "C" {
#endif
#define decode2byte_little(_ptr)     (uint16_t)(*(uint8_t *)(_ptr) | (*(uint8_t *)((_ptr) + 1) << 8))
#define decode4byte_little(_ptr) \
    (uint32_t)(*(uint8_t *)(_ptr) | (*(uint8_t *)((_ptr) + 1) << 8) \
    | (*(uint8_t *)((_ptr) + 2) << 16) | (*(uint8_t *)((_ptr) + 3) << 24))
#define encode2byte_little(_ptr, data) \
do { \
    *(uint8_t *)((_ptr) + 1) = (uint8_t)((data) >> 8); \
    *(uint8_t *)(_ptr) = (uint8_t)(data); \
} while (0)
#define encode4byte_little(_ptr, data) \
do { \
    *(uint8_t *)((_ptr) + 3) = (uint8_t)((data) >> 24); \
    *(uint8_t *)((_ptr) + 2) = (uint8_t)((data) >> 16); \
    *(uint8_t *)((_ptr) + 1) = (uint8_t)((data) >> 8); \
    *(uint8_t *)(_ptr) = (uint8_t)(data); \
} while (0)

#define ATT_DECODE2BYTE             decode2byte_little
#define ATT_DECODE4BYTE             decode4byte_little
#define ATT_ENCODE2BYTE             encode2byte_little
#define ATT_ENCODE4BYTE             encode4byte_little

#define ATT_VALUE_LEN_DEFAULT 4
#define att_uuid_compare(u1_, u2_)    memcmp((u1_), (u2_), sizeof(bth_att_uuid))

typedef void (sdk_gatt_cbk_func)(uint16_t event, void *arg);

uint8_t uapi_gatts_add_server(void);
uint8_t uapi_gatts_add_service(bth_gatt_add_server *param);
uint8_t uapi_gatts_add_characteristic(bth_gatt_add_characteristic *param);
uint8_t uapi_gatts_add_descriptor(bth_gatt_add_descriptor *descriptor_param);
uint8_t uapi_gatts_send_response(bth_gatt_send_response *respones_req);
uint8_t uapi_gatts_start_service(bth_gatt_start_service *start_param);
uint8_t uapi_gatts_send_value_by_uuid(bth_gatt_notify_characteristic_by_uuid *param);
uint8_t uapi_gatts_add_include_service(bth_gatt_add_include *include_param);
uint8_t uapi_gatts_get_value(uint16_t att_handle, const uint16_t *len, const uint8_t *data);
uint8_t uapi_gatts_stop_service(const bth_gatt_add_server *service_param);
uint8_t uapi_gatts_delete_service(const bth_gatt_add_server *service_param);
uint8_t uapi_gatts_mtu_reply(uint16_t conn_hdl, uint16_t client_rx_mtu);
uint8_t uapi_gatts_callback_regist(uint16_t event, const void *func);
uint8_t uapi_gatts_callback_unregist(uint16_t event);
uint8_t uapi_gatts_service_cbk_reg(sdk_gatt_cbk_func cbk);
uint8_t uapi_gatts_set_default_mtu_value(bth_le_att_mtu_param *mtu_param);
uint8_t uapi_gatts_exchange_mtu_req(bth_gatt_mtu_config *param);
#ifdef __cplusplus
}
#endif

#endif // BLE_ADVERTISING_H__

/** @} */
