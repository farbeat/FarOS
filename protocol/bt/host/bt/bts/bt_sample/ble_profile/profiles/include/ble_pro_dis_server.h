/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */
#ifndef BLE_PRO_DIS_SERVER_H
#define BLE_PRO_DIS_SERVER_H

#include <stdint.h>
#include "ble_pro_dis.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @breif  register basic callbacks for server
 */
void ble_pro_dis_register_callbacks(void);

/**
 * @breif  init DIS server and start DIS service
 */
void ble_pro_dis_server_init(void);

/**
 * @brief  get local manufacturer name
 * @return local manufacturer name
 */
char *ble_pro_dis_server_manufacturer_name(void);

/**
 * @brief  set local manufacturer name
 * @param  [in]  value, manufacturer name
 */
int ble_pro_dis_server_manufacturer_name_set(char *value);

/**
 * @brief  get local model number
 * @return local model number
 */
char *ble_pro_dis_server_model_number(void);

/**
 * @brief  set local model number
 * @param  [in]  value, model number
 */
int ble_pro_dis_server_model_number_set(char *value);

/**
 * @brief  get local serial number
 * @return local serial number
 */
char *ble_pro_dis_server_serial_number(void);

/**
 * @brief  set local serial number
 * @param  [in]  value, serial number
 */
int ble_pro_dis_server_serial_number_set(char *value);

/**
 * @brief  get local hardware revision
 * @return local hardware revision
 */
char *ble_pro_dis_server_hardware_revision(void);

/**
 * @brief  set local hardware revision
 * @param  [in]  value, hardware revision
 */
int ble_pro_dis_server_hardware_revision_set(char *value);

/**
 * @brief  get local firmware revision
 * @return local firmware revision
 */
char *ble_pro_dis_server_firmware_revision(void);

/**
 * @brief  set local firmware revision
 * @param  [in]  value, firmware revision
 */
int ble_pro_dis_server_firmware_revision_set(char *value);

/**
 * @brief  get local software revision
 * @return local software revision
 */
char *ble_pro_dis_server_software_revision(void);

/**
 * @brief  set local software revision
 * @param  [in]  value, software revision
 */
int ble_pro_dis_server_software_revision_set(char *value);

/**
 * @brief  get local system id
 * @return local system id
 */
char *ble_pro_dis_server_system_id(void);

/**
 * @brief  set local system id
 * @param  [in]  value, system id
 */
int ble_pro_dis_server_system_id_set(char *value);

/**
 * @brief  get local ieee regulatory certification data list
 * @return local ieee regulatory certification data list
 */
char *ble_pro_dis_server_ieee_regulatory_certification_data_list(void);

/**
 * @brief  set local regulatory certification data list
 * @param  [in]  value, regulatory certification data list
 */
int ble_pro_dis_server_ieee_regulatory_certification_data_list_set(char *value);

/**
 * @brief  get local pnp id
 * @return local pnp id
 */
ble_pro_dis_pnp_id *ble_pro_dis_server_pnp_id(void);

/**
 * @brief  set local pnp id
 * @param  [in]  vendor_id_source
 * @param  [in]  vendor_id
 * @param  [in]  product_id
 * @param  [in]  product_revision
 */
int ble_pro_dis_server_pnp_id_set(uint8_t vendor_id_source, uint16_t vendor_id, uint16_t product_id,
                                  uint16_t product_revision);

/**
 * @brief  dis server add characters and descriptors
 * @param  [in]  server_id
 * @param  [in]  srvc_handle
 */
void ble_pro_dis_add_characters(uint8_t server_id, uint16_t srvc_handle);

#ifdef __cplusplus
}
#endif
#endif
