#ifndef BTSRV_GATT_SERVER_DEFAULT_SERVICE
#define BTSRV_GATT_SERVER_DEFAULT_SERVICE
void btsrv_gatts_set_eatt_support(bool support);

/**
 * Set device name .
 * @param name Device name string.
 * @param len The length of device name string.
 * @param is_enable BT/BLE enable is or not.
 */
void btsrv_gatts_set_device_name_value(const uint8_t *name, const uint8_t len, bool is_enable);
void btsrv_gatts_set_device_appearance_value(uint16_t appearance);
void btsrv_gatts_register_gatt_service(void);
void btsrv_gatts_register_gap_service(void);
#endif
