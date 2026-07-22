/*
 * Copyright (c) CompanyNameMagicTag 2012-2020. All rights reserved.
 * Description: common define of dsp
 * Author: audio
 */

#ifndef __COMMON_DSP_H__
#define __COMMON_DSP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SIZE_01K 0x00000400
#define SIZE_02K 0x00000800
#define SIZE_04K 0x00001000
#define SIZE_08K 0x00002000
#define SIZE_16K 0x00004000
#define SIZE_32K 0x00008000
#define SIZE_64K 0x00010000
#define SIZE_128K 0x00020000
#define SIZE_256K 0x00040000
#define SIZE_512K 0x00080000
#define SIZE_768K 0x000C0000
#define SIZE_1M 0x00100000
#define SIZE_2M 0x00200000
#define SIZE_3M 0x00300000
#define SIZE_4M 0x00400000
#define SIZE_8M 0x00800000
#define SIZE_512M 0x20000000

#define DELAY_100US 100
#define CNT_100US_PS 10000
#define WAIT_10S (10 * CNT_100US_PS)
#define DELAY_1MS 1

#define RD_DATA_SIZE 0x140   /* 320B/10ms/1ch */
#define READ_DATA_SIZE 0xa00 /* 80ms */

typedef enum {
    SAP_SHM_SYS_CMD_SIZE = 0x0008, /* shmem of sys cmd */
    SAP_SHM_PROBE_STATUS_SIZE = 0x0100, /* shmem of probe status */
    SAP_SHM_ADP_IDS_SIZE = 0x0010, /* shmem of adp handle */
    SAP_SHM_SHRB_CFG_SIZE = 0x0040, /* shmem of shrb config */
    SAP_SHM_TRACK_PTS_SIZE = 0x0010, /* shmem of track pts */
    SAP_SHM_TWS_CONFIG_SIZE = 0x0010, /* shmem of tws config */
} sap_shm_size;

typedef enum {
    SAP_SHM_SYS_CMD_OFFSET = 0x0000, /* shmem of sys cmd */
    SAP_SHM_PROBE_STATUS_OFFSET = (SAP_SHM_SYS_CMD_OFFSET + SAP_SHM_SYS_CMD_SIZE), /* shmem of heart rate */
    SAP_SHM_ADP_IDS_OFFSET = (SAP_SHM_PROBE_STATUS_OFFSET + SAP_SHM_PROBE_STATUS_SIZE), /* shmem of adp handle */
    SAP_SHM_SHRB_CFG_OFFSET = (SAP_SHM_ADP_IDS_OFFSET + SAP_SHM_ADP_IDS_SIZE), /* shmem of shrb config */
    SAP_SHM_TRACK_PTS_OFFSET = (SAP_SHM_SHRB_CFG_OFFSET + SAP_SHM_SHRB_CFG_SIZE), /* shmem of track pts */
    SAP_SHM_TWS_CONFIG_OFFSET = (SAP_SHM_TRACK_PTS_OFFSET + SAP_SHM_TRACK_PTS_SIZE), /* shmem of track pts */
    SOC_SHM_SHRB_OFFSET = (SAP_SHM_TWS_CONFIG_OFFSET + SAP_SHM_TWS_CONFIG_SIZE), /* shmem of share ring buffer */
} sap_shm_offset;

#define SAP_SHM_BASE_SIZE SOC_SHM_SHRB_OFFSET

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __COMMON_DSP_H__ */
