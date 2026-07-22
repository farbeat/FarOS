/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides at cmd register func \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-20， Create file. \n
 */
#ifndef APP_MSG_MANAGER_H
#define APP_MSG_MANAGER_H

#include "stdint.h"
#include <stdlib.h>
#include "securec.h"
#include "at_cmd_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define DIAG_MSG_CID_DEFAULT 0xFF
#define DIAG_CONTROL_TRUE    1
#define DIAG_CONTROL_FALSE   0
#define MCU_RET_SUCCESS      "\r\nOK\r\n"
#define MCU_RET_FAIL         "\r\nERROR\r\n"

typedef enum {
    DIAG_COMMON_MSG     = 0x0,
    RESET               = 0x1,
    FSTEST              = 0x2,
    FSFORMAT            = 0x3,
    FSRWTEST            = 0x4,
    FSSETCLK            = 0x5,
    FSREADCLK           = 0x6,
    FSWRITE             = 0x7,
    FSCHECK             = 0x8,
    FSRM                = 0x9,
    GETWSTPVER          = 0xA,
    LOGWRITE            = 0xB,
    HEAPINFO            = 0xC,
    STACKINFO           = 0xD,
    TESTDTCMBW          = 0xE,
    TESTL2MBW           = 0xF,
    TESTMMCBW           = 0x10,
    TESTPSRAMBW         = 0x11,
    TESTNORFLASHBW      = 0x12,
    TESTNANDFLASHBW     = 0x13,
    GETFSINFO           = 0x14,
    PMRMVOTE            = 0x15,
    PMADDVOTE           = 0x16,
    LCDON               = 0x17,
    LCDOFF              = 0x18,
    SHIPMODE            = 0x19,
    PMMODESET           = 0x1a,
    PMMODEUSR           = 0x1b,
    PMMODEGET           = 0x1c,
    GETCURSEC           = 0x1d,
    LCDFILL             = 0x1e,
    LCDLIGHT            = 0x1f,
    GETIDS              = 0x20,
    RESUMETOUCH         = 0x21,
    MCUDYMPW            = 0x22,
    COREMARKTEST        = 0x23,
    PMCLDO1PD           = 0x24,
#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
    ADCAUTOSAMPLE       = 0x25,
    ADCCLOSEAUTOSAMPLE  = 0X26,
    ADCSHOWDATA         = 0X27,
#endif
    NANDREAD            = 0x28,
    NANDWRITE           = 0x29,
    NANDERASE           = 0x2a,
    NANDMARK            = 0x2b,
    NANDCHECK           = 0x2c,
    NANDSETMODE         = 0x2d,
    BUTTON              = 0x2e,
    SETAPPDELAY         = 0x2f,
    POWERKEY            = 0x30,
    SETCLDO1VOL         = 0x31,
    NANDAGINGTEST       = 0x32,
    FLASHAGINGTEST      = 0x33,
    SETSCREENOFFFUNC    = 0x34,
    SETSCREENOFFTIME    = 0x35,
    SETSCREENSTATE      = 0x36,
    GETSCREENSTATE      = 0x37,
    SETPINPULL          = 0x38,
    GETPINPULL          = 0x39,
    AESDECRYPT          = 0x3a,
    AESENCRYPT          = 0x3b,
    AESSETGID           = 0x3c,
    TIMESET             = 0x3d,
    TIMEGET             = 0x3e,
    DONGLECONNECTED     = 0x40,
    FTDONGLERETURN      = 0x41,
    I2CWRITE            = 0x42,
    I2CREAD             = 0x43,
    SETCALENDAR         = 0x44,
    GETCALENDAR         = 0x45,
    ALIPAYSECTEST       = 0x46,
    ALIPAYDEBUG         = 0x47,
    FILESPLIT           = 0x48,
    FILECOMBINE         = 0x49,
    ALIPAYVERIFY        = 0x4A,
    ALIPAYRESET         = 0x4B,
    FORCEDEEPSLEEP      = 0x4C,
    GETTRANSCHAN        = 0x4D,
    SETTRANSCHAN        = 0x4E,
    AUTOOTABLOCK        = 0x4F,
    AUTOOTARUN          = 0x50,
    AUTOOTADELMAC       = 0x51,
    AUTOOTAADDMAC       = 0x52,
    AUTOOTASHOWMAC      = 0x53,
    PERFSTART           = 0x54,
    PERFSTOP            = 0x55,
    SCHEDSTART          = 0x56,
    SCHEDSTOP           = 0x57,
    MSG_ID_MAX_BUTT,
} app_msg_id;

typedef enum {
    TID_LOCAL_ADDRESS   = 0x00,
    TID_MCU             = 0x02,
    TID_BT              = 0x03,
    TID_PHONE           = 0x04,
    TID_PC              = 0x06,
    TID_MAX_NUM,  /* NOTE: Add New Transaction ID above this item, Tran ID should less than 0x0F */
} diag_msg_tranid;

typedef enum {
    SID_DEV_STATUS      = 0x1,
    SID_OTA_SVR         = 0x2,
    SID_CALL_SVR        = 0x3,
    SID_NOTIFY_SVR      = 0x4,
    SID_MAINTENANCE_SVR = 0x5,
    SID_FILE_TRANS_SVR  = 0x6,
    SID_FACTORY_TEST    = 0x7,
    SID_APP_AT_SVR      = 0x8,
    SID_BT_SVR          = 0x9,
    SID_MEDIA_SVR       = 0xA,
    SID_GRAPHIC_SVR     = 0XB,
    SID_PLAT_BUTT,

    SID_BT_FACT_TEST    = 0x13,
    SID_BT_DEV_MGR_SVR  = 0x81,
    SID_BT_PHONE_MGR_SVR = 0x82,
    SID_CONNECT_SVR     = 0x83,
    SID_BLE_CONNECT_SVR = 0x85,
    SID_BT_AUDIO_SVR    = 0x8A,
} diag_msg_sid;

/**
 * @brief at cmd msg information struct.
 *
 */
typedef struct {
    uint8_t id;                     /* app msg id */
    uint8_t control;                /* bit0 1:ack 0:no ack*/
    uint32_t payload_len;           /* The Length of Payload */
    uint8_t payload[0];
} app_msg_info_t;

typedef struct {
    uint8_t start_flag;                 /* start flag 0xAF */
    uint8_t control;                    /* control 0x3 */
    uint16_t len;                       /* msg length */
    uint8_t tid;                        /* transmit ID */
    uint8_t sid;                        /* service ID */
    uint8_t cid;                        /* cmd ID */
    uint8_t payload[0];
} diag_commom_msg_t;

void print_str_uart(char* str);
void print_at_cmd_ret(int ret);
void app_at_task_queue_init(void);
unsigned long get_app_at_queue_id(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
