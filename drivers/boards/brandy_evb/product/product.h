/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:  BRANDY product config
 * Author: @CompanyNameTag
 * Create:  2020-10-23
 */
#ifndef PRODUCT_H
#define PRODUCT_H

#ifndef YES
#define YES (1)
#endif

#ifndef NO
#define NO (0)
#endif

#if (defined BRANDY_PRODUCT_NONE)
#define APPLICATION_VERSION_STRING "B650"
#include "product_none.h"
#endif

#if (defined BRANDY_PRODUCT_FPGA)
#define APPLICATION_VERSION_STRING "B650"
#include "product_fpga_standard.h"
#endif

#if (defined BRANDY_PRODUCT_EVB)
#define APPLICATION_VERSION_STRING "B650"
#include "product_evb_standard.h"
#endif

#if (defined BRANDY_PRODUCT_EVB4)
#define APPLICATION_VERSION_STRING "B650"
#include "product_evb4_standard.h"
#endif

#define TWS_EAR_WEAR_NOTICE_SUPP            NO
#define TWS_A2DP_TX_PWR_REDUCE_SUPP         NO
#define TWS_PAGE_TX_PWR_REDUCE_SUPP         NO
#define WITH_SDIO_HOST                      NO
#define USE_FN_PLL                          NO
#define BT_EXIST                            YES
#define APP_EXIST                           NO
#define DSP_EXIST                           YES
#define DUAL_DSP_EXIST                      NO
#define GNSS_EXIST                          NO
#define SECURITY_EXIST                      NO
#define WIFI_EXIST                          NO
#define UWB_EXIST                           NO
#define NFC_EXIST                           NO
#define CORE_NUMS                           3
#define IPC_SHARE_NUMS                      6
#define OTP_SET_CLK_PERIOD                  NO
#define SHA512_EXIST                        NO
#define SM4_EXIST                           NO
#define SHA_LAST_PACKAGE_IS_BIG_ENDIAN      NO
#define OTP_FIRST_REGION_BITS               2048
#define OTP_SECOND_REGION_BITS              2048
#define RPC_PAYLOAD_MAX_SIZE                3072
#define USE_RPC_MODE                        NO
#define SEC_NEW_RSA                         NO
#define EMBED_FLASH_EXIST                   NO
#define SSB_BACKUP                          YES
#define USE_LOAD_SWICH                      YES
#define USE_PMU_WDT                         NO
#define USE_PMU_COUL                        NO
#define PIO_DRIVE0_IS_LOWEST                YES
#define UART_RESET_REG_SUPP                 YES
#define MASTER_UART_PIO_DRIVE_VAL           HAL_PIO_DRIVE_15
#define SLAVE_UART_PIO_DRIVE_VAL            HAL_PIO_DRIVE_1
#define EMMC_SD_BUSWIDTH                    MMCSD_BUSWIDTH_8
#define PARTITION_EXIST                     YES
#define ALIPAY_SEC_I2C_INDEX                I2C_BUS_3
#define ALIPAY_SEC_GPIO                     S_AGPIO_L29
#endif
