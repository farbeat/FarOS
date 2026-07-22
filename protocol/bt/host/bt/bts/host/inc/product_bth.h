/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef _PRODUCT_BTH_H_
#define _PRODUCT_BTH_H_
#include "chip_definitions.h"

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO 0
#endif


#if ((defined BS25_PRODUCT_EVB) || (defined BS25_PRODUCT_FPGA) || (defined BS25_PRODUCT_HID))
#define BT_SDK_CONFIG_SYNC_FLASH            YES
#define BTH_BLE_CONNECT_MAX 16
#define BTH_BLE_PAIRING_MAX 4
#define BTH_BTS_AT_CMD YES
#define BTH_BTS_WHITE_ADDR_COUNT_MAX 16
/* The platform provides the register address for storing the bypass mode flag. */
#define BT_BYPASS_MODE_ADDR 0x57000010
#else
#define BT_SDK_CONFIG_SYNC_FLASH            YES
#define BTH_BLE_CONNECT_MAX 16
#define BTH_BLE_PAIRING_MAX 4
#define BTH_BTS_AT_CMD NO
#define BTH_BTS_WHITE_ADDR_COUNT_MAX 16
#endif

#endif
