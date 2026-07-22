/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: product None config
 * Author: @CompanyNameTag
 * Create:  2020-10-23
 */
#ifndef PRODUCT_NONE_CONFIG_H
#define PRODUCT_NONE_CONFIG_H

/**
 * @addtogroup connectivity_config_product
 * @{
 */
#define BTH_WITH_SMART_WEAR                 NO
#define BTC_WITH_SMART_WEAR                 NO
#define USE_COMPRESS_LOG_INSTEAD_OF_SDT_LOG NO
#define COMPRESS_LOG_TRIGGER_THRESHOLD      0
#define COMPRESS_LOG_COUNT_THRESHOLD        0xFFFFFFFF
#define BTC_TWS_MONITOR                     YES
#define BTC_ICO_SUPPORT                     YES
#define BTC_DUAL_ANT_SWITCH_SUPPORT         NO
#define BTC_BLE_VOICEID_SUPPORT             YES
#define BTC_DFX_LOG_HELP_SUPPORT            YES
#define BTH_WEAR_ENABLE_CONNECT_MANAGER     NO
#define BTH_WEAR_ENABLE_BLE_FEATURES        NO
#define BTH_WEAR_ENABLE_HFP_FEATURES        NO
#define EXCEPTION_TEST_ENABLE               NO
#define APP_WRITE_OTP_ENABLE                NO
#define USE_FLASH_SUSPEND_AND_RESUME        NO
#define WAIT_APPS_DUMP_FOREVER              NO
#define TWS_MPU_XIP_PROTECT                 NO
#define BTSNOOP_ENABLE                      YES
#define SYS_DEBUG_MODE_ENABLE               YES
#define WEAR_USER_CONFIG                    NO
#define LOG_LEVEL_APP_DEFAULT_CONFIG        3   // Info level
#define LOG_LEVEL_GNSS_DEFAULT_CONFIG       3   // Info level
#define LOG_LEVEL_BT_DEFAULT_CONFIG         3   // Info level
#define LOG_LEVEL_DSP_DEFAULT_CONFIG        3   // Info level
#define LOG_LEVEL_SEC_DEFAULT_CONFIG        3   // Info level
#define FACTORY_CALI                        NO
#define BCPU_HEAP_MININUM_SIZE              0xA000 // 40KB
#define NON_OS_CRITICAL_RECORD              NO
#define BTH_WEAR_ENABLE_AUDIO_SINK          NO
#define BTH_WEAR_ENABLE_AUDIO_GATEWAY       NO
#define BTH_WEAR_BREDR_DOUBLE_CONNECT       NO
#define ENABLE_MASSDATA_RECORD              NO
#define TWS_USER_CHR_RECORD                 NO
#define ENABLE_LOW_POWER                    NO
#define DTB_IMAGE_PAGES                     0
#define RECOVERY_IMAGE_PAGES                24
#define RESERVE_IMAGE_PAGES                 0
#define BT_IMAGE_PAGES                      112
#define HIFI0_IMAGE_PAGES                   112
#define APP_IMAGE_PAGES                     1353
#define FOTA_IMAGE_START                    0x3C2000
#define SSB_FOTA_OFFSET                     0x0
#define XO_CTRIM_VALUR_DEFAULT              0x56
#define DSP1_IPC_ENABLE                     NO
#define BTC_SMART_WEAR_WLCOEX               YES
#define APP_BTN_TASK_MONITOR_ENABLE         NO
#define BTC_SWITCH_COEX                     NO
#define SSB_FOTA_MODE_TWS                   NO
#define DSP_EXTEND_OCRAM_SIZE               0
#define BT_MIPS_DEBUG                       NO
#define BTC_GET_EMC_FROM_PLATFORM           YES  // 后续耳机产品通过HCI传递NV数据实现后，该选项需要置为NO
#define OSC_EN_CALLBACK_BY_PLT              YES
#define RC_CLOCK_ON                         NO
#define SMART_WATCH_BT_COEX_PIN_SWITCH      NO   // pinmux switch in the Coexistence of BT and WiFi of smart watch
#define BTH_WEAR_ENABLE_HICARRY_2           NO
#define CAP_SENSOR_BUFF_LEN                 128
#define ENABLE_BT_XO_16M                    NO

/********************display config********************/
#define DOUBLE_BUFFER                     1
#if defined(QSPI_DISPLAY)
#define RESOLUTION_WIDTH                  194
#define RESOLUTION_HEIGHT                 368
#elif defined(MEMORY_MINI)
#define RESOLUTION_WIDTH                  240
#define RESOLUTION_HEIGHT                 296
#elif defined(SUPPORT_SOFT_SEND_FRM)
#define RESOLUTION_WIDTH                  368
#define RESOLUTION_HEIGHT                 448
#else
#define RESOLUTION_WIDTH                  454
#define RESOLUTION_HEIGHT                 454
#endif
/**
 * @} end of group PRODUCT
 */
#endif
