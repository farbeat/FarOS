
/*
 * Copyright (c) @CompanyNameMagicTag Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description:  At cmd header file.
 * Create:
 */
#ifndef AT_CMD_API_H
#define AT_CMD_API_H

#include <stdint.h>
#include "errcode.h"

#define APPID_STRUCT_LEN 4
#define CALIBRATION_LENTH 256

/**
 * @brief at cmd handle function definition.
 */
typedef uint32_t (*at_cmd_handle)(uint8_t *para, uint32_t para_len, char *str_cmd);

/**
 * @brief At cmd table structure.
 */
typedef struct {
    char *str_cmd;
    at_cmd_handle cmd_handle;
    char *str_common;
} at_cmd_table_t;

/**
 * @brief at cmd msg information struct.
 *
 */
typedef struct {
    uint32_t payload_len;           /* The Length of Payload         */
    void *payload;                  /* The pointer of Payload buf    */
} at_cmd_msg_info_t;

/*
 * Inter Commu Header Structure definition
 */
typedef struct {
    uint16_t devType;
    uint8_t tranID;                    /* The inter commu header transaction id   */
    uint8_t control;                   /* The inter commu header control          */
    uint8_t serviceID;                 /* The inter commu header Service ID       */
    uint8_t commandID;                 /* The inter commu header Command ID       */
    uint8_t appID[APPID_STRUCT_LEN];
} commu_inter_header_t;

/*
 * Inter Commu Info Structure definition
 */
typedef struct {
    commu_inter_header_t interHeader;  /* The inter commu header        */
    uint16_t payloadLength;           /* The Length of Payload         */
    uint8_t tranIDSrc;                /* The Length of Payload         */
    uint8_t reserved;                 /* For the 4 Byte alignment      */
    uint8_t payload[0];               /* The pointer of Payload buf    */
} commu_inter_info_t;

/*
 * BT Calibration Config Info in IPC Structure definition
 */
typedef struct {
    uint32_t length;
    uint8_t params[CALIBRATION_LENTH];
} ipc_calibration_config_t;
/**
 * @brief Add module to at cmd list.
 *
 * @param module_cmd_list Module cmd array pointer.
 * @param cmd_count Module cmd array length.
 * @return errcode_t
 */
errcode_t at_cmd_add_moudle(at_cmd_table_t *module_cmd_list, uint32_t cmd_count);

/**
 * @brief At cmd thread process function.
 * @param param unused param.
 */
void at_cmd_task_process_handler(void *param);

/**
 * @brief At cmd message receive callback function.
 * @param data
 * @param data_len
 */
void at_cmd_msg_receive_handler(uint8_t *data, uint32_t data_len);

/**
 * @brief At cmd task inti message queue.
 */
void at_cmd_task_queue_init(void);

#ifdef FT_BT_UART
/**
 * @brief send at cmd result to bt cores.
 * @param result
 * @param result_len
 */
void at_cmd_send_abt_result_to_bt(uint8_t *result, uint32_t result_len);
void set_calibration_from_nv(void);
#endif

#endif