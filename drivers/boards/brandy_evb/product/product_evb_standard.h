/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: product EVB standard config
 * Author: @CompanyNameTag
 * Create:  2022-01-11
 */
#ifndef PRODUCT_EVB_STANDARD_CONFIG_H
#define PRODUCT_EVB_STANDARD_CONFIG_H

/**
 * @defgroup connectivity_config_product PRODUCT
 * @ingroup  connectivity_config
 * @{
 */
/* Platform board config, need check if it support or not carefully. */
#define BTH_WITH_SMART_WEAR                 YES
#define APP_WRITE_OTP_ENABLE                NO
#define APP_HEAP_SIZE                       0x18000    // the minimum APP heap size (96kB)
#define USE_FLASH_SUSPEND_AND_RESUME        NO
#define WAIT_APPS_DUMP_FOREVER              NO
#define TWS_MPU_XIP_PROTECT                 NO
#define SSB_IMAGE_PAGES                     27
#ifdef CONFIG_LOW_POWER_TEST
#define ENABLE_LOW_POWER                    YES
#else
#define ENABLE_LOW_POWER                    NO
#endif
#define DTB_IMAGE_PAGES                     0
#define RECOVERY_IMAGE_PAGES                64
#define RESERVE_IMAGE_PAGES                 0
#define BT_IMAGE_PAGES                      112
#define HIFI0_IMAGE_PAGES                   112
#define HIFI0_CODEC_PAGES                   320
#define APP_IMAGE_PAGES                     1353
#define NV_IMAGE_PAGES                      4
#define FOTA_IMAGE_PAGES                    3
#define FOTA_IMAGE_START                    0x770000
#define SSB_FOTA_OFFSET                     0x0
#define XO_CTRIM_VALUR_DEFAULT              0x56
#define DSP1_IPC_ENABLE                     NO
#define APP_BTN_TASK_MONITOR_ENABLE         NO
#define SSB_FOTA_MODE_TWS                   YES
#define DSP_EXTEND_OCRAM_SIZE               1310720 // Unit byte, = 1280 KB
#define OSC_EN_CALLBACK_BY_PLT              YES
#define BT_MIPS_DEBUG                       NO
#define NON_OS_CRITICAL_RECORD              NO
#define ENABLE_MASSDATA_RECORD              NO
#define USE_COMPRESS_LOG_INSTEAD_OF_SDT_LOG NO
#define COMPRESS_LOG_TRIGGER_THRESHOLD      0
#define COMPRESS_LOG_COUNT_THRESHOLD        0xFFFFFFFF
#define EXCEPTION_TEST_ENABLE               NO
#define LOG_LEVEL_APP_DEFAULT_CONFIG        3   // Info level
#define LOG_LEVEL_BT_DEFAULT_CONFIG         3   // Info level
#define LOG_LEVEL_DSP_DEFAULT_CONFIG        3   // Info level
#define BCPU_HEAP_MININUM_SIZE              0xA000 // 40KB
#define SYS_DEBUG_MODE_ENABLE               YES
#define PROMPT_TONE_REGION_LENGTH           0x2800
#define AUDIO_DATA_STREAM_REGION_LENGTH     0x2000
#define BT_MASSDATA_LENGTH                  0x400
#define APP_MASSDATA_LENGTH                 0x800
#define APP_LOGGING_LENGTH                  0x1C00
#define DSP_LOGGING_LENGTH                  0x2000
#define PSRAM_DENSITY_MB                    32
#define RC_CLOCK_ON                         NO
#define SMART_WATCH_BT_COEX_PIN_SWITCH      NO   // pinmux switch in the Coexistence of BT and WiFi of smart watch
#define CAP_SENSOR_BUFF_LEN                 128
#define BT_L2RAM_AREA_SIZE                  3    // 384k, Reference the enum Value of hal_b_memory_share_config_t
#define DSP_L2RAM_AREA_TOTAL                10
#define GPU_L2RAM_AREA_TOTAL                11
#ifndef CONFIG_PSRAM_SUPPORT
#define DSP_L2RAM_AREA_SIZE                 6    // 无PSRAM & 小型化场景分配给DSP 6块*128K=768KB 剩余512K系统使用
#else
#if defined(CONFIG_SEA_PHS_SUPPORT)
#define DSP_L2RAM_AREA_SIZE                 10   // 语音唤醒场景分配给DSP 10块*128K=1280KB DSP L2M不给系统使用
#else
#define DSP_L2RAM_AREA_SIZE                 8    // 正常场景分配给DSP 8块*128K=1024KB 剩余256K系统使用
#endif /* CONFIG_SEA_PHS_SUPPORT */
#endif /* CONFIG_PSRAM_SUPPORT */
#define PUYA_FLASH_SUPPORT                  YES

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

/********************Other module board config********************/
#define BTC_SWITCH_COEX                     YES
#define BTC_SMART_WEAR_WLCOEX               YES
#define BTC_WITH_SMART_WEAR                 YES
#define BTC_TWS_MONITOR                     YES
#define BTC_ICO_SUPPORT                     YES
#define BTC_DUAL_ANT_SWITCH_SUPPORT         NO
#define BTC_GET_EMC_FROM_PLATFORM           YES  // 后续耳机产品通过HCI传递NV数据实现后，该选项需要置为NO
#define BTSNOOP_ENABLE                      NO
#define TWS_USER_CHR_RECORD                 NO
#define BTH_WEAR_ENABLE_AUDIO_SINK          NO
#define BTH_WEAR_ENABLE_AUDIO_GATEWAY       NO
#define BTH_WEAR_BREDR_DOUBLE_CONNECT       NO
#define BTC_DFX_LOG_HELP_SUPPORT            NO
#define FACTORY_CALI                        NO
#define BTH_WEAR_ENABLE_CONNECT_MANAGER     NO
#define BTH_WEAR_ENABLE_BLE_FEATURES        YES
#define BTH_WEAR_ENABLE_HFP_FEATURES        NO
#define WEAR_USER_CONFIG                    NO
#define BTH_HIGH_POWER                      YES
#define MULTI_CONNECT                       YES
#define BTH_CONFIG_HDAP                     YES
#define DEVICE_MANAGE_FEATURE               YES
#define BTH_CALL_LC3_32K                    YES
#define BTH_ENABLE_LC3_CODEC                YES
#define ENABLE_CHANGE_DEVICE_NAME           YES
#define BTH_DIP_PRODUCT_ID                  0x4106
#define BTH_ENABLE_L2HC_CODEC               YES
#define ENABLE_BT_XO_16M                    NO
#define BTH_WSTP_CMD_SUPPORT                YES
#define PRODUCT_SUPPORT_MODE_SWITCH         YES
#define BT_MANAGER_DEPLOYED                 2
#define BT_CODEC_TID                        2
#define BTH_WEAR_ENABLE_AVRCP_TARGET        NO
#define BTH_WEAR_ENABLE_AVRCP_CONTROLLER    NO
#define BTH_WEAR_ENABLE_AUDIO_SOURCE        NO
#define BTH_WEAR_ALLOW_INQUIRY_SCAN         YES
#define BTH_WEAR_ENABLE_HALL_STATE          NO

// 音频内置输入设备
#define BUILTIN_AI_PORT                            0x50  // 参考 soc_uapi_ai.h 中 UAPI_AI_PORT_LPADC0 枚举
#define BUILTIN_AI_PORT_ATTR_ADC_RX_TYPE           0     // 参考 soc_uapi_ai.h 中 UAPI_AI_RX_DEFAULT 枚举
#define BUILTIN_AI_PCM_ATTR_AUDIO_CH               1     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_CHANNEL_1 枚举
#define BUILTIN_AI_PCM_ATTR_BIT_DEPTH              16    // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_BIT_DEPTH_16 枚举
#define BUILTIN_AI_PCM_ATTR_SAMPLE_RATE            16000 // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_SAMPLE_RATE_16K 枚举
#define BUILTIN_AI_PCM_ATTR_FRAME_PER_SEC          100   // 每秒帧数

// 音频内置输出设备
#define BUILTIN_SND_OUT_PORT                       0x12  // 参考 soc_uapi_sound.h 中 UAPI_SND_OUT_PORT_I2S2 枚举
#define BUILTIN_SND_OUT_PORT_NUM                   1     // 端口个数
#define BUILTIN_SND_OUT_AUDIO_CH                   2     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_CHANNEL_2 枚举
#define BUILTIN_SND_OUT_BIT_DEPTH                  16    // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_BIT_DEPTH_16 枚举
#define BUILTIN_SND_OUT_SAMPLE_RATE                48000 // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_SAMPLE_RATE_48K 枚举
#define BUILTIN_SND_OUT_I2S_ATTR_I2S_BCLK          8     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_I2S_BCLK_8_DIV 枚举
#define BUILTIN_SND_OUT_I2S_ATTR_BIT_DEPTH         16    // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_BIT_DEPTH_16 枚举
#define BUILTIN_SND_OUT_I2S_ATTR_AUDIO_CH          2     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_CHANNEL_2 枚举
#define BUILTIN_SND_OUT_I2S_ATTR_I2S_STD_MODE      0     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_I2S_STD_MODE 枚举
#define BUILTIN_SND_OUT_I2S_ATTR_MASTER            1     // 1:主设备,0:从设备
#define BUILTIN_SND_OUT_I2S_ATTR_I2S_MCLK          1     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_I2S_MCLK_256_FS 枚举
#define BUILTIN_SND_OUT_I2S_ATTR_I2S_PCM_DELAY     1     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_I2S_PCM_1_DELAY 枚举
#define BUILTIN_SND_OUT_I2S_ATTR_SAMPLE_RISE_EDGE  1     // 参考 soc_uapi_audio.h 中 pcm_sample_rise_edge 成员

// 音频外挂输入设备
#define EXTERNAL_AI_PORT                           1     // 参考 soc_uapi_ai.h 中 UAPI_AI_PORT_I2S1 枚举
#define EXTERNAL_AI_I2S_ATTR_MASTER                1     // 1:主设备,0:从设备
#define EXTERNAL_AI_I2S_ATTR_I2S_MODE              0     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_I2S_STD_MODE 枚举
#define EXTERNAL_AI_I2S_ATTR_MCLK                  4     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_I2S_MCLK_768_FS 枚举
#define EXTERNAL_AI_I2S_ATTR_BCLK                  24    // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_I2S_BCLK_24_DIV 枚举
#define EXTERNAL_AI_I2S_ATTR_BIT_DEPTH             16    // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_BIT_DEPTH_16 枚举
#define EXTERNAL_AI_I2S_ATTR_AUDIO_CH              2     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_CHANNEL_2 枚举
#define EXTERNAL_AI_I2S_ATTR_SAMPLE_RISE_EDGE      1     // 参考 soc_uapi_audio.h 中 pcm_sample_rise_edge 成员
#define EXTERNAL_AI_I2S_ATTR_PCM_DELAY_CYCLE       1     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_I2S_PCM_1_DELAY 枚举
#define EXTERNAL_AI_PCM_ATTR_CHANNELS              2     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_CHANNEL_2 枚举
#define EXTERNAL_AI_PCM_ATTR_BIT_DEPTH             16    // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_BIT_DEPTH_16 枚举
#define EXTERNAL_AI_PCM_ATTR_SAMPLE_RATE           16000 // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_SAMPLE_RATE_16K 枚举
#define EXTERNAL_AI_PCM_ATTR_FRAME_PER_SEC         100   // 每秒帧数

// 音频外挂输出设备
#define EXTERNAL_SND_OUT_PORT                      0x11  // 参考 soc_uapi_sound.h 中 UAPI_SND_OUT_PORT_I2S1 枚举
#define EXTERNAL_SND_OUT_AUDIO_CHANNEL             2     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_CHANNEL_2 枚举
#define EXTERNAL_SND_OUT_AUDIO_BIT_DEPTH           16    // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_BIT_DEPTH_16 枚举
#define EXTERNAL_SND_OUT_AUDIO_SAMPLE_RATE         16000 // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_SAMPLE_RATE_16K 枚举
#define EXTERNAL_SND_OUT_PORT_NUM                  1     // 端口个数
#define EXTERNAL_SND_OUT_I2S_ATTR_BIT_DEPTH        16    // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_BIT_DEPTH_16 枚举
#define EXTERNAL_SND_OUT_I2S_ATTR_CHANNEL          2     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_CHANNEL_2 枚举
#define EXTERNAL_SND_OUT_I2S_ATTR_I2S_MODE         0     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_I2S_STD_MODE 枚举
#define EXTERNAL_SND_OUT_I2S_ATTR_MCLK             4     // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_I2S_MCLK_768_FS 枚举
#define EXTERNAL_SND_OUT_I2S_ATTR_MASTER           1     // 1:主设备,0:从设备
#define EXTERNAL_SND_OUT_I2S_ATTR_SAMPLE_RISE_EDGE 1     // 参考 soc_uapi_audio.h 中 pcm_sample_rise_edge 成员
#define EXTERNAL_SND_OUT_I2S_ATTR_BCLK             24    // 参考 soc_uapi_audio.h 中 UAPI_AUDIO_I2S_BCLK_24_DIV 枚举

// Sleep option
#define CONFIG_PM_TICKLESS_EN               YES
#define CONFIG_PM_POWER_GATING_EN           YES
#define CONFIG_PM_SLEEP_RECORD_EN           YES
#define CONFIG_PM_VETO_TRACK_EN             NO
#define CONFIG_PM_SLEEP_INT_EN              NO

// Double-click the key
#define CONFIG_SUPPORT_BUTTON_DOUBLE_CLICK  NO

// LCD VCI_EN
#define VCI_LCD_GPIO                        S_MGPIO8
#define DISPLAY_TE_GPIO                     S_AGPIO_L22
#define RESET_LCD_GPIO                      S_AGPIO_R3

// TP CONFIG
#define ZTW523_I2C_BUS                      I2C_BUS_1
#define ZTW523_INT_GPIO                     S_AGPIO_R5
#define ZTW523_RESET_GPIO                   S_AGPIO_R4
#define TMA525B_I2C_BUS                     I2C_BUS_1
#define TMA525B_INT_GPIO                    S_AGPIO_R5
#define TMA525B_RESET_GPIO                  S_AGPIO_R4

// TRACK MODE
#define SND_TRACK_MODE_BT_MUSIC             0 // 参考 soc_uapi_sound.h 中 UAPI_SND_TRACK_STEREO 枚举
#define SND_TRACK_MODE_BT_SCO               1 // 参考 soc_uapi_sound.h 中 UAPI_SND_TRACK_DOUBLE_MONO 枚举
#define SND_TRACK_MODE_LOCAL_CAT1           1 // 参考 soc_uapi_sound.h 中 UAPI_SND_TRACK_DOUBLE_MONO 枚举
#define SND_TRACK_MODE_BT_CAT1              0 // 参考 soc_uapi_sound.h 中 UAPI_SND_TRACK_STEREO 枚举
#define SND_TRACK_MODE_LOCAL_MUSIC          1 // 参考 soc_uapi_sound.h 中 UAPI_SND_TRACK_DOUBLE_MONO 枚举

// AEF
#if defined(MEMORY_MINI)
#define SND_PORT_AEF_TYPE 					0x130 // 参考 soc_uapi_aef.h 中 UAPI_AEF_TYPE_SWS1 枚举
#else
#define SND_PORT_AEF_TYPE 					0x180 // 参考 soc_uapi_aef.h 中 UAPI_AEF_TYPE_SMARTPA 枚举
#endif
#define SND_PORT_AEF_ENABLE 				true // true代表开启aef算法，false代表关闭aef算法
/**
 * @}
 */
#endif
