/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: data config
 * Author: @CompanyNameTag
 * Create: 2021-09-01
 */
#ifndef DATA_CONFIG_H
#define DATA_CONFIG_H

#include "custdefs.h"
#include "mpc_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    BT_CONFIG_STATUS_SUCESS,
    BT_CONFIG_STATUS_FAIL,
    BT_CONFIG_STATUS_TIMEOUT,
    BT_CONFIG_STATUS_WAIT_FINISH,
} bt_config_status;

typedef struct {
    uint8_t bd_addr[BD_ADDR_LEN];
    uint8_t phone_bd_addr[BD_ADDR_LEN];
    uint8_t bd_name[BD_NAME_MAX_LEN];
    uint8_t config_data[BD_CONFIG_SYNC_DATA_SIZE];
} product_data_config;

product_data_config* bt_acore_get_data_config(void);
void bt_acore_get_bt_addr(uint8_t *bdaddr, uint32_t len);
void bt_acore_get_phone_bt_addr(uint8_t *bdaddr);
void bt_acore_get_bt_name(uint8_t *bdname, uint32_t len);
void bt_acore_get_config_sync_global(uint16_t offset, uint8_t *data, uint32_t len);
void bt_acore_set_config_sync_global(uint16_t offset, const uint8_t *data, uint32_t len);
uint8_t bt_sync_config_from_file_to_global(void);
uint8_t bt_sync_config_from_global_to_file(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* DATA_CONFIG_H */
