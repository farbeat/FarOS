/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */
#ifndef BLE_PRO_DIS_CLIENT_H
#define BLE_PRO_DIS_CLIENT_H
#include "errcode.h"
#include "osal_list.h"
#include "bts_gatt_client.h"
#include "ble_pro_dis.h"
#ifdef __cplusplus
extern "C" {
#endif

#define BLE_PRO_DIS_RMT_DEV_MAX 16
#define BLE_PRO_DIS_CHARACTER_VALUE_LEN_MAX 128

typedef void (*dis_client_read_character_cbk)(uint8_t error_code, uint16_t uuid, uint16_t val_len, uint8_t *val);

typedef enum {
    CHAR_SYSTEM_ID,
    CHAR_MODEL_NUMBER,
    CHAR_SERIAL_NUMBER,
    CHAR_FIRMWARE_REVISION,
    CHAR_HARDWARE_REVISION,
    CHAR_SOFTWARE_REVISION,
    CHAR_MANUFACTURER_NAME,
    CHAR_IEEE_RCDL,
    CHAR_PNP_ID,
    CHAR_BUTT,
} dis_client_character_id_t;

typedef enum {
    CHARACTER_NOT_SUPPORT,    /* 不支持 */
    CHARACTER_NOT_READABLE,   /* 不支持读取 */
    CHARACTER_READ_OK,        /* 读取正常 */
    CHARACTER_READ_TIMEOUT,   /* 读取超时 */
} dis_client_character_status;

typedef struct dis_client_character_info {
    uint8_t status;
    uint16_t handle;
    dis_client_read_character_cbk cbk;
} dis_client_character_info_t;

typedef struct ble_dis_client_remote_dev {
    struct osal_list_head node;
    uint8_t client_id;
    uint16_t conn_id;
    uint16_t att_mtu;
    uint16_t start_hdl;
    uint16_t end_hdl;
    dis_client_character_info_t character[CHAR_BUTT];
} ble_dis_client_remote_dev_t;

/**
 * @brief read manufacturer name of peer device
 * @param [in] client_id gatt client注册返回client id。
 * @param [in] conn_id   执行BLE连接时回调返回的连接ID。
 * @param [in] cbk callback of response
 */
dis_client_character_status ble_pro_dis_read_remote_manufacturer_name(uint8_t client_id, uint16_t conn_id,
                                                                      dis_client_read_character_cbk cbk);

/**
 * @brief read module number of peer device
 * @param [in] client_id gatt client注册返回client id。
 * @param [in] conn_id   执行BLE连接时回调返回的连接ID。
 * @param [in] cbk callback of response
 */
dis_client_character_status ble_pro_dis_read_remote_model_number(uint8_t client_id, uint16_t conn_id,
                                                                 dis_client_read_character_cbk cbk);

/**
 * @brief read serial number of peer device
 * @param [in] client_id gatt client注册返回client id。
 * @param [in] conn_id   执行BLE连接时回调返回的连接ID。
 * @param [in] cbk callback of response
 */
dis_client_character_status ble_pro_dis_read_remote_serial_number(uint8_t client_id, uint16_t conn_id,
                                                                  dis_client_read_character_cbk cbk);

/**
 * @brief read hardware revision of peer device
 * @param [in] client_id gatt client注册返回client id。
 * @param [in] conn_id   执行BLE连接时回调返回的连接ID。
 * @param [in] cbk callback of response
 */
dis_client_character_status ble_pro_dis_read_remote_hardware_revision(uint8_t client_id, uint16_t conn_id,
                                                                      dis_client_read_character_cbk cbk);

/**
 * @brief read firmware revision of peer device
 * @param [in] client_id gatt client注册返回client id。
 * @param [in] conn_id   执行BLE连接时回调返回的连接ID。
 * @param [in] cbk callback of response
 */
dis_client_character_status ble_pro_dis_read_remote_firmware_revision(uint8_t client_id, uint16_t conn_id,
                                                                      dis_client_read_character_cbk cbk);

/**
 * @brief read software revision of peer device
 * @param [in] client_id gatt client注册返回client id。
 * @param [in] conn_id   执行BLE连接时回调返回的连接ID。
 * @param [in] cbk callback of response
 */
dis_client_character_status ble_pro_dis_read_remote_software_revision(uint8_t client_id, uint16_t conn_id,
                                                                      dis_client_read_character_cbk cbk);

/**
 * @brief read system id of peer device
 * @param [in] client_id gatt client注册返回client id。
 * @param [in] conn_id   执行BLE连接时回调返回的连接ID。
 * @param [in] cbk callback of response
 */
dis_client_character_status ble_pro_dis_read_remote_system_id(uint8_t client_id, uint16_t conn_id,
                                                              dis_client_read_character_cbk cbk);

/**
 * @brief read ieee regulatory certification data list of peer device
 * @param [in] client_id gatt client注册返回client id。
 * @param [in] conn_id   执行BLE连接时回调返回的连接ID。
 * @param [in] cbk callback of response
 */
dis_client_character_status ble_pro_dis_read_remote_ieee_rcdl(uint8_t client_id, uint16_t conn_id,
                                                              dis_client_read_character_cbk cbk);

/**
 * @brief read pnp id of peer device
 * @param [in] client_id gatt client注册返回client id。
 * @param [in] conn_id   执行BLE连接时回调返回的连接ID。
 * @param [in] cbk callback of response
 */
dis_client_character_status ble_pro_dis_read_remote_pnp_id(uint8_t client_id, uint16_t conn_id,
                                                           dis_client_read_character_cbk cbk);

/**
 * @brief discovery dis service of peer device
 * @param [in] client_id gatt client注册返回client id。
 * @param [in] conn_id   执行BLE连接时回调返回的连接ID。
 */
errcode_t ble_dis_client_discovery_service(uint8_t client_id, uint16_t conn_id);

/* 根据连接ID清理dis服务连接设备信息 */
void ble_dis_client_clear_remote_by_conn(uint8_t client_id, uint16_t conn_id);

/* 清理dis服务连接设备信息 */
void ble_dis_client_clear_remote_all(void);

/* 发现dis服务回调函数 */
void ble_dis_client_discovery_service_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_service_result_t *service);

/* 发现dis特征回调函数 */
void ble_dis_client_discovery_character_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_result_t *character);

/* 读DIS回调函数 */
void ble_dis_client_read_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *read_result,
    gatt_status_t status);

#ifdef __cplusplus
}
#endif
#endif  /* end of ble_pro_dis_client.h */
