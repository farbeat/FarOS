/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description: ipc protocal common interface
 * Author: @CompanyNameTag
 * Create:
 */

#ifndef IPC_PROTOCOL_H
#define IPC_PROTOCOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DATA_MAX_LEN 0x8000
#define DATA_MIN_LEN 0x01

typedef enum {
    IPC_CMD_OPEN_BTAPP,
    IPC_CMD_CLOSE_BTAPP,
    IPC_CMD_CONFIG_BTAPP, /* bt data: ipc_bt_pkt_t */
    IPC_CMD_OPEN_AUDIO,
    IPC_CMD_CLOSE_AUDIO,
    IPC_CMD_CONFIG_AUDIO,
    IPC_CMD_AUDIO_DATA,   /* voice or audio data: ipc_audio_data_t */
    IPC_CMD_MCU_CMD,      /* control msg from MCU */
    IPC_CMD_BT_CMD,       /* control msg from BT */
    IPC_CMD_DSP_CMD,      /* control msg from DSP */
    IPC_CMD_BT_SPP_CMD,   /* send spp data status */
} ipc_cmd_id;

typedef enum {
    TRANS_ID_NA = 0x00,            /* 0:not used */
    TRANS_ID_AP,                   /* 1:ap */
    TRANS_ID_MCU,                  /* 2:mcu */
    TRANS_ID_BT,                   /* 3:bt */
    TRANS_ID_APP,                  /* 4:app */
    TRANS_ID_ZIGBEE,               /* 5:zigbee */
    TRANS_ID_PC,                   /* 6:pc */
    TRANS_ID_SERIAL,               /* 7:pc_serial */
    TRANS_ID_SECOND_APP,           /* 8:second app */
    TRANS_ID_SYNERGY = 0x09,       /* 9:iot */
    TRANS_ID_DSP = 0x0A,           /* 10:dsp/hifi */
    TRANS_ID_BT_CONTROLLER = 0x0B, /* 11:bt controller */
    TRANS_ID_MAX = 0x0F            /* reserved */
} trans_id;

typedef struct {
    uint8_t src_trans_id : 4; /* trans_id */
    uint8_t dst_trans_id : 4; /* trans_id */
    uint8_t cmd;              /* ipc_cmd_id */
    uint16_t length;          /* the length of data, no include header length */
} ipc_msg_header_t;

typedef struct {
    ipc_msg_header_t hdr;
    uint8_t data[];
} ipc_msg_t;

#define IPC_AUDIO_DATA_HAR_RESERVED 3

typedef struct {
    uint32_t cmd_id;
    uint8_t packet_loss; /* 00: packet loss; 01: no packet loss */
    uint8_t reserved[IPC_AUDIO_DATA_HAR_RESERVED];
    uint32_t timestamp; /* The Time Stamp reflects the sampling instant of the first octet in the media packet. */
    /* The sequence number increments by one for each media packet
    * sent, and may be used by the receiver to detect packet loss and
    * to restore packet sequence */
    uint16_t sequence_number;
    uint16_t length; /* the length of audio data */
} ipc_audio_data_hdr_t;

typedef struct {
    ipc_audio_data_hdr_t hdr;
    uint8_t data[];
} ipc_audio_data_t;

typedef struct {
    uint8_t id;
    uint8_t data[];
} ipc_mcu_cmd_t;

typedef enum {
    IPC_BT_DSP_CMD_SET_TS, /* set toggle length and first frame ts to dsp */
    IPC_BT_DSP_CMD_GET_TS,
    IPC_BT_DSP_CMD_SYNC_TS,
} ipc_bt_dst_cmd_e;

typedef struct {
    uint32_t cmd_id;      /* IPC_BT_DSP_CMD_SET_TS */
    uint32_t toggle_len;  /* toggle length in us */
    uint32_t toggle_ts;   /* ts of first toggle */
} ipc_bt_setts_cmd_t;

typedef struct {
    uint32_t cmd_id; /* IPC_BT_DSP_CMD_GET_TS */
} ipc_bt_getts_cmd_t;

typedef struct {
    uint32_t cmd_id;               /* IPC_BT_DSP_CMD_SYNC_TS */
    uint32_t toggle_cnt;           /* toggle count according to main ear */
    uint32_t toggle_ts;            /* dsp regenerate ts according toggle */
    uint32_t delay_toggle_cnt;     /* second ear set ts delay toggle count */
    uint32_t ts_flag;
    uint32_t toggle_cnt_slave;
    uint32_t ts_ratio;
    uint32_t last_sn;
    uint32_t last_original_ts;
} ipc_bt_sync_ts_cmd_t;

/* 0              1             2              3              4              5              6             7
 *  +--------+--------+--------+--------+--------+--------+--------+
 *  |     SOF         |         LENGTH           | Control         |  FSN       |   SID      |   CID      |
 *  +--------+--------+--------+--------+--------+--------+--------+
 *  9              10
 *  +--------+--/ /---+ +--------+--------+
 *    PAYLOAD   ...          | |            CRC            |
 *  +--------+--/ /---+ +--------+--------+
 */
typedef struct {
    uint8_t trans_id;
    uint8_t sof;
    uint16_t length;
    uint8_t control;
    uint8_t frame_sequense_number;
    uint16_t optcode;
    uint8_t data[]; /* payload + CRC */
} ipc_bt_pkt_t;

typedef enum {
    IPC_SEND_OK,
    IPC_SEND_BAD_PARAMS,
    IPC_SEND_MEMORY_ALLOC_FAIL,
    IPC_SEND_MESSAGE_PUT_FALI,
    IPC_SEND_FALI,
} ipc_send_return_id;

#ifdef __cplusplus
}
#endif
#endif
