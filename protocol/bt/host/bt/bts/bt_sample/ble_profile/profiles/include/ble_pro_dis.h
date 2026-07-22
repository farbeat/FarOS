/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */
#ifndef BLE_PRO_DIS_H
#define BLE_PRO_DIS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_PRO_DIS_SERVICE_UUID16 0x180A
#define BLE_PRO_DIS_SYSTEM_ID_UUID16 0x2A23
#define BLE_PRO_DIS_MODEL_NUMBER_UUID16 0x2A24
#define BLE_PRO_DIS_SERIAL_NUMBER_UUID16 0x2A25
#define BLE_PRO_DIS_FIRMWARE_REVISION_UUID16 0x2A26
#define BLE_PRO_DIS_HARDWARE_REVISION_UUID16 0x2A27
#define BLE_PRO_DIS_SOFTWARE_REVISION_UUID16 0x2A28
#define BLE_PRO_DIS_MANUFACTURER_NAME_UUID16 0x2A29
#define BLE_PRO_DIS_IEEE_REGULATORY_CERTIFICATION_DATA_LIST_UUID16 0x2A2A
#define BLE_PRO_DIS_PNP_ID_UUID16 0x2A50

#define BLE_PRO_DIS_PNP_ID_LEN 7

typedef struct {
    uint8_t vendor_id_source;
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t product_revision;
} ble_pro_dis_pnp_id;

typedef struct {
    /* represent the System Id of the device */
    char *system_id;

    /* represent the model number assigned by the device vendor */
    char *model_number;

    /* represent the serial number for a particular instance of the device */
    char *serial_number;

    /* represent the firmware revision for the firmware within the device */
    char *firmware_revision;

    /* represent the hardware revision for the hardware within the device */
    char *hardware_revision;

    /* represent the software revision for the software within the device */
    char *software_revision;

    /* represent the name of the manufacturer of the device */
    char *manufacturer_name;

    char *ieee_regulatory_certification_data_list;

    ble_pro_dis_pnp_id pnp_id;
} ble_pro_dis_data;

#ifdef __cplusplus
}
#endif
#endif
