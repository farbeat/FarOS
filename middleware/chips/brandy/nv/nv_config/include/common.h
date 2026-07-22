/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: NV common header file.
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include "stdint.h"
/* 修改此文件后需要先编译A核任意版本生成中间文件application.etypes后才能在编译nv.bin时生效 */

/* 基础类型无需在此文件中定义，直接引用即可，对应app.json中的sample0 */

/* 普通结构体，对应app.json中的sample1 */
typedef struct {
    int8_t param1;
    int8_t param2;
    uint32_t param3;
    uint32_t param4;
    uint8_t param5;
} sample1_type_t;

/* 普通数组，对应app.json中的sample2 */
typedef struct {
    uint32_t param1[5];
} sample2_type_t;

/* 多类型结构嵌套，对应app.json中的sample3 */
typedef struct {
    uint32_t param1[5];
    uint32_t param2;
    uint8_t param3;
    sample1_type_t param4;
} sample3_type_t;

/* 更为复杂的结构体嵌套，对应app.json中的sample4 */
typedef struct {
    sample1_type_t param1;
    sample2_type_t param2;
    sample3_type_t param3;
} sample4_type_t;

typedef struct {
    uint8_t conn_vertify[384]; /* 128 byte */
    uint8_t conn_salt[20];     /* 20 count */
    uint32_t salt_len;
} ext_nv_secure_conn_t;

#define BD_ADDR_LEN 6
#define BTSDK_DEVNAME_LEN 64

/* 本地设备信息 */
typedef struct {
    uint8_t bd_addr[BD_ADDR_LEN];    /* 本地设备地址 */
    uint8_t reserve;                 /* 本地设备保留 */
    uint8_t device_mode;             /* 本地设备设备模式 */
    uint8_t name[BTSDK_DEVNAME_LEN]; /* 本地设备名称 */
} btsrv_local_device_info_t;

#define BT_RF_POWER_BAND_NUM 8
#define TX_POWER_CALI_COFF_NUM 3
/* TX POWER信息 */
typedef struct {
    uint8_t nv_tx_power_cali_num;                               // 功率校准频点个数
    int16_t nv_tx_power_cali_pref_offset[BT_RF_POWER_BAND_NUM]; // 对应频点pref
    uint8_t nv_tx_power_cali_freq[BT_RF_POWER_BAND_NUM];        // 需要校准的频点
    int16_t nv_tx_power_coff[TX_POWER_CALI_COFF_NUM];           // 二项式系数
    uint32_t nv_cali_switch_all;                                // 校准总开关
    uint16_t nv_ble_1m_max_power_val;                           // BLE 1m定制化功率，此处入参为最大档的功率
    uint16_t nv_ble_2m_max_power_val;                           // BLE 2m定制化功率，此处入参为最大档的功率
    uint16_t nv_br_max_power_val;                               // BR定制化功率，此处入参为最大档的功率
    uint16_t nv_edr_gfsk_max_power_val;                         // EDR的gfsk定制化功率，此处入参为最大档的功率
    uint8_t nv_customized_feature;                              // 定制化特性开关
    uint8_t nv_fcc_ind;                                         // FCC产品标示0xFC:指示为FCC产品 others:非FCC产品
    uint8_t nv_tws_monitor_switch;                              // 监听开关
    uint8_t nv_tws_dual_switch;                                 // 双发开关
} btsrv_calibration_config_t;

#define PAIRED_DEVICE_DATA_MAX 960
#define PAIRED_DEVICE_RESERVE_LEN 3

/* 已配对设备持久化结构 */
typedef struct {
    uint8_t device_num;                         /* 设备信息个数 */
    uint8_t reserve[PAIRED_DEVICE_RESERVE_LEN]; /* 设备信息保留 */
    uint8_t info[PAIRED_DEVICE_DATA_MAX];       /* 设备信息 */
} btsrv_paired_device_info_t;

/* HOST NV配置数据长度 */
#define BT_CONFIG_DATA_LEN 584

/* HOST NV配置结构 */
typedef struct {
    uint8_t reserve;
    uint8_t config_data[BT_CONFIG_DATA_LEN];
} bth_product_data_config_t;

/* 3A prams */
#define NV_AUDIO_VQE_PARAM_SIZE 48
typedef struct {
    uint32_t size;
    uint32_t data[NV_AUDIO_VQE_PARAM_SIZE];
} audio_vqe_param_t;

#endif /* __COMMON_H__ */
