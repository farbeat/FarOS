/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: Provides at cmd manager source
 * Author:
 * Create:
 */
#include "debug_print.h"
#include "securec.h"
#include "at_cmd_api.h"
#include "cmsis_os.h"
#include "fcntl.h"
#include "chip_io.h"
#include <sys/stat.h>
#include "soc_osal.h"
#include "mc_bt_manager.h"
#include "connectivity_low_power.h"
#include "common_def.h"
#if defined(__LITEOS__)
#include "soc/mmc.h"
#endif
#include "unistd.h"
#if (CONFIG_BT_CALI_RXDC == FEATURE_ON)
#include "connectivity_calibration.h"
#include "clock_calibration.h"
#endif
#include "app_at_process.h"
#include "bt_manager_tx_process.h"
#include "bts_br_gap.h"
#include "mc_bt_manager.h"
#include "bt_manager_at_process.h"

#define BSP_MEM_ALLOC(y) osal_kmalloc((y), OSAL_GFP_KERNEL)
#define BSP_MEM_FREE(y) osal_kfree(y)
#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

#define OLD_FILE_LEN 378
#define NV_ID_CUST_LEN 8
#define NV_ID_CUST 0x0043

#define BT_MANAGER_FILE_POWER_OFFSET 35
#define BT_CONFIG_CALIBRATION_INFO_LEN 44
#define BT_MANAGER_FILE_PHONE_LINKKEY_OFFSET 114
#define BT_MANAGER_FILE_PHONE_LINKKEY_LENGTH 28
#define BT_MANAGER_FILE_PHONE_PROFILE_OFFSET 136
#define BT_LINKKEY_NUM 4
#define BT_LINKKEY_PROFILE_OFFSET 22
/* calibration info length is 44, so skip this info */
#define BT_CONFIG_FACTORY_RESET_DATA_OFFSET (BT_CONFIG_CALIBRATION_INFO_LEN + 1)
#ifndef PORT_LOCK_FLAG_MASK
#define PORT_LOCK_FLAG_MASK 0x0001
#endif
/* 4s: bt ack for reset protect */
#define BT_RESET_TIMER_TIMEOUT 3000
/* 4s: bt ack for restart protect */
#define BT_RESTART_TIMER_TIMEOUT 3000
#define BT_MANAGER_SELFIND_INFO_LEN 4
#define BT_MANAGER_MODEL_ID_LEN 2

#define BT_SRC_TRANID_OFFSET 0
#define BT_DES_TRANID_OFFSET 4
#define BT_SRC_TRANID_MSK 0x0F
#define BT_DES_TRANID_MSK 0xF0

#define BT_MAC_LEN 6
#define MC_SEC_AES_IV_LEN             16  /* length of aes iv */
#define MC_SEC_AES_MIN_LEN            16  /* minimum length of aes data */
#define MC_SEC_AES_ENC_INC_LEN        16  /* increase length of aes encrypt data */
#define MC_SEC_USER_INFO_LEN          4   /* length of C3 key  for generating K1 key */
#define BT_MAC_LEN_AND_IS_SNIFF 7
#define BT_MAX_CMD_STR_LEN 32

#define FS_FILE_PATH_BT_MANAGER_FILE "/user/config/bt_conf.bin"

#define CT_TEST_DELAY_DEFAULT 1000
#define DONGLE_RECONN_TIMEOUT 10000
#define BT_ADDR_LEN 12
#define BT_ADDR_FIXED_LEN 13
#define BT_ADDR_HEX_LEN 6
#define BTSRV_LOG_UART_INFO          0xFF2
#define ONE_HEX_LEN 2
#define DELIM_LEN 1
#define BASE_HEX_VAL 16

uint32_t g_coupling_test_delay_ms = CT_TEST_DELAY_DEFAULT;
uint8_t g_dongle_addr[BT_ADDR_FIXED_LEN] = "";
uint8_t g_dongle_client_id = 0;
uint16_t g_bt_dft_rx_count = 0;


enum {
    BT_MANAGER_FILE_OPERABLE,
    BT_MANAGER_FILE_LOCK,
};

typedef uint32_t NV_LENGTH_S;

typedef enum {
    NV_OK_E = 0,
    NV_FAIL_E,
    NV_PARA_ERROR_E,
    NV_INVALID_E
} NvErrorCode;

typedef enum  {
  osTimerRousesIgnore       =     0,       ///< timer can't wakeup system
  osTimerRousesAllow        =     1        ///< timer can wakeup system
} os_timer_rouses_type;

typedef enum  {
  osTimerAlignIgnore        =     0,       ///<timer no need to align
  osTimerAlignAllow         =     1        ///< timer need to align
} os_timer_align_type;

typedef struct
{
    osTimerAttr_t             attr;
    osTimerFunc_t             func;
    osTimerType_t             type;
    os_timer_rouses_type       rousesType;
    os_timer_align_type        alignType;
} TimerDefineInfo;

#if (CONFIG_BT_CALI_RXDC == FEATURE_ON)
typedef struct BtTrans {
    unsigned char operation;
    unsigned char dataId;
    unsigned char lengthL;
    unsigned char lengthH;
    unsigned char data[];
} BtTransStru;

typedef enum {
    XO_OPERA_WRITE,
    XO_OPERA_READ,
    XO_OPERA_SAVE,
} XoCapOperationE;

typedef enum {
    XO_CAP_VALUE,
    XO_CAP_CTRIM,
} XoCapTypeE;

typedef struct {
    uint8_t xoCtrim:3;
    uint8_t xoValue:4;
    uint8_t reserved:1;
} XoCapStruc;

typedef struct {
    uint32_t status;
    union {
        XoCapStruc xoCap;
        uint8_t value;
    };
} XoResStru;
#endif

static uint8_t g_fileStatus = BT_MANAGER_FILE_OPERABLE;
static int32_t g_fileLen = OLD_FILE_LEN;
static osTimerId_t g_btResetTimerId = NULL;
static osTimerId_t g_btRestartTimerId = NULL;
static uint8_t macHighPower[BT_MAC_LEN + 1] = { 0 };

diag_ser_header_t g_bt_diag_header;    // 用于保存diag回复包头内容
bool g_bt_diag_flag = false;           // 用于控制执行功能后是否回复diag消息

#if (CONFIG_BT_FILE_ENCRYPTION == FEATURE_ON)
/* |___________________________________________________________________________________________________| */
/* |_________________________________|_________16byte________|________16byte________|__2byte___________| */
/* |              data               |      encyption        |          iv          | encyptiondata Len| */
/* |_________________________________|_______________________|______________________|__________________| */
/* |<----------- BT_FILE_ENCY_DATA_LEN --------------------->|                                         | */
/* |<---------------------------BT_FILE_ENCY_IV_LEN-------------------------------->|                  | */
/* |<-------------------------------------BT_FILE_ENCY_TOTAL_LEN-------------------------------------->| */
static uint8_t *g_fileBuf = NULL;
#define BT_FILE_ENCYPTION_OUTPUT_LEN 2
#define BT_FILE_ENCY_DATA_LEN (g_fileLen + MC_SEC_AES_ENC_INC_LEN)
#define BT_FILE_ENCY_IV_LEN (BT_FILE_ENCY_DATA_LEN + MC_SEC_AES_IV_LEN)
#define BT_FILE_ENCY_TOTAL_LEN (BT_FILE_ENCY_IV_LEN + BT_FILE_ENCYPTION_OUTPUT_LEN)

#define BT_FILE_ENCY_IV_START (g_fileBuf + BT_FILE_ENCY_DATA_LEN)
#define BT_FILE_ENCYPTION_OUTPUTLEN_START (g_fileBuf + BT_FILE_ENCY_IV_LEN)
#endif

static void BtResetTimeoutCallback(void const *argument);
static void BtRestartTimeoutCallback(void const *argument);
#if ((defined CHIP) && (CHIP != CHIP_BRANDY))
#if (CONFIG_BT_FILE_ENCRYPTION == FEATURE_ON)
static int32_t BtWriteFileAndEncyption(const char *fileName, const void *buf, uint32_t nByte, int32_t position);
static int32_t BtReadFileAndDecryption(const char *fileName, void *buf, uint32_t nByte);
#endif
#endif
/* Declare Timer */
TimerDefineInfo g_btResetTimerDef = {
    { "btResetTimer", 0, NULL, 0 },
    (osTimerFunc_t)BtResetTimeoutCallback,
    osTimerOnce,
    osTimerRousesIgnore,
    osTimerAlignIgnore
};

TimerDefineInfo g_btRestartTimerDef = {
    { "btRestartTimer", 0, NULL, 0 },
    (osTimerFunc_t)BtRestartTimeoutCallback,
    osTimerOnce,
    osTimerRousesIgnore,
    osTimerAlignIgnore
};

uint32_t bt_diag_cmd_process(diag_ser_data_t *data)
{
    if (data == NULL) {
        return ERRCODE_FAIL;
    }

    commu_inter_info_t* interInfo = NULL;
    uint32_t infoLen;
    uint8_t* ptr;
    uint32_t paylen;
    uint32_t ret;
    diag_ser_frame_t* frame = (diag_ser_frame_t*)data->payload;

    parse_tlv_2value(frame->tlv, data->header.length, &ptr, &paylen);
    infoLen = sizeof(commu_inter_info_t) + paylen + 1;
    interInfo = (commu_inter_info_t*)BSP_MEM_ALLOC(infoLen);
    if (interInfo == NULL) {
        wstp_print("[AT][package_name_param_cmd_to_bt] malloc=%d fail\r\n", infoLen);
        return ERRCODE_FAIL;
    }

    if (paylen > 0) {
        ret = memcpy_s(&interInfo->payload[0], paylen, ptr, paylen);
        if (ret != EOK) {
            wstp_print("[AT][package_common_cmd_to_bt]memcpy error!\r\n");
            BSP_MEM_FREE(interInfo);
            return ERRCODE_FAIL;
        }
    }

    interInfo->payload[paylen] = '\0';
    interInfo->interHeader.tranID = data->header.dst;
    interInfo->interHeader.control = 0;
    interInfo->interHeader.devType = 0;
    interInfo->interHeader.appID[0] = 0;
    interInfo->interHeader.serviceID = data->header.ser_id;
    interInfo->interHeader.commandID = data->header.cmd_id;
    interInfo->payloadLength = paylen + 1;
    interInfo->tranIDSrc = data->header.src;
    g_bt_diag_flag = true;
    (void)memcpy_s(&g_bt_diag_header, sizeof(diag_ser_header_t), &(data->header), sizeof(diag_ser_header_t));
    wstp_print("commandID = 0x%x payload_len=%d\n", interInfo->interHeader.commandID, interInfo->payloadLength);
    bt_manager_msg_send((uint8_t *)interInfo, infoLen);

    return ERRCODE_SUCC;
}

static void BtResetTimeoutCallback(void const *argument)
{
    unused(argument);
    g_btResetTimerId = NULL;
}
#if ((defined CHIP) && (CHIP != CHIP_BRANDY))
static int32_t BtResetStartTimer(uint32_t timeout)
{
    if (g_btResetTimerId != NULL) {
        if (osTimerStart(g_btResetTimerId, timeout) != osOK) {
            wstp_print("fail_01 timeout: %d\r\n", timeout);
            return ERRCODE_FAIL;
        }
    } else {
        g_btResetTimerId = osTimerNew(g_btResetTimerDef.func, g_btResetTimerDef.type, NULL, &(g_btResetTimerDef.attr));
        // g_btResetTimerId = mc_osTimerCreate(&g_btResetTimerDef, NULL);
        if (g_btResetTimerId == NULL) {
            wstp_print("fail g_btResetTimerId is NULL\r\n");
            return ERRCODE_FAIL;
        }
        if (osTimerStart(g_btResetTimerId, timeout) != osOK) {
            wstp_print("fail_02 timeout: %d\r\n", timeout);
            return ERRCODE_FAIL;
        }
    }

    return ERRCODE_SUCC;
}

static void BtResetStopTimer(void)
{
    if (g_btResetTimerId != NULL) {
        if (osTimerStop(g_btResetTimerId) != osOK) {
            wstp_print("fail osTimerStop(g_btResetTimerId) != osOK\r\n");
        }
    }
}
#endif

static void BtRestartTimeoutCallback(void const *argument)
{
    unused(argument);
#if (CONFIG_SVR_PWM == FEATURE_ON)
    // (void)MC_PM_CurTaskShutDownCplt(MC_MAIL_BT_MANAGER);
#endif
    g_btRestartTimerId = NULL;
}

#if ((defined CHIP) && (CHIP != CHIP_BRANDY))
static int32_t BtRestartStartTimer(uint32_t timeout)
{
    if (g_btRestartTimerId != NULL) {
        if (osTimerStart(g_btRestartTimerId, timeout) != osOK) {
            wstp_print("fail_01 timeout: %d\r\n", timeout);
            return ERRCODE_FAIL;
        }
    } else {
        g_btRestartTimerId = osTimerNew(g_btRestartTimerDef.func, g_btRestartTimerDef.type, NULL, &(g_btRestartTimerDef.attr));
        //g_btRestartTimerId = mc_osTimerCreate(&g_btRestartTimerDef, NULL);
        if (g_btRestartTimerId == NULL) {
            wstp_print("fail g_btResetTimerId is NULL\r\n");
            return ERRCODE_FAIL;
        }
        if (osTimerStart(g_btRestartTimerId, timeout) != osOK) {
            wstp_print("fail_02 timeout: %d\r\n", timeout);
            return ERRCODE_FAIL;
        }
    }

    return ERRCODE_SUCC;
}

static void BtRestartStopTimer(void)
{
    if (g_btRestartTimerId != NULL) {
        if (osTimerStop(g_btRestartTimerId) != osOK) {
            wstp_print("osTimerStop(g_btRestartTimerId) != osOK\r\n");
        }
    }
}

static void BtManagerRespose(uint8_t tid, uint8_t serviceID, uint8_t commandID, int32_t sendLength, const uint8_t *data)
{
    CommuInterHeaderS sendHeader = {0}; /*lint !e522*/
    bt_manager_commu_inter_info_t *sendInfo = NULL;

    do {
        if (data == NULL) {
            wstp_print("data is NULL\r\n");
            break;
        }

        /* control filed invalid here, Because the value will be re-write at the CommuSendPkt() */
        sendHeader.control = 0;
        sendHeader.tranID = tid;
        sendHeader.serviceID = serviceID;
        sendHeader.commandID = commandID;
        sendInfo = CommuBuildInterInfo(&sendHeader, sendLength);
        if (sendInfo == NULL) {
            wstp_print("sendInfo is NULL\r\n");
            break;
        }

        if (memcpy_s(sendInfo->payload, sendLength, data, sendLength) != EOK) {
            wstp_print("memcpy err\r\n");
            break;
        }

        if (CommuSendPkt(sendInfo) != ERRCODE_SUCC) {
            wstp_print("CommuSendPkt() err\r\n");
        }
    } while (0);
}

static int32_t BtGetFileSize(const char *path)
{
    int32_t ret = ERRCODE_FAIL;
    int32_t fd = -1;
    struct stat finfo = {0};

    do {
        fd = open(path, O_RDWR | O_CREAT, S_IREAD | S_IWRITE);
        if (fd < 0) {
            wstp_print("open err\r\n");
            break;
        }

        ret = fstat(fd, &finfo);
        if (ret < 0) {
            wstp_print("fstat err\r\n");
            ret = ERRCODE_FAIL;
        }
    } while (0);

    if (fd >= 0) {
        close(fd);
    }
    return (ret == 0) ? finfo.st_size : ERRCODE_FAIL;
}

static int32_t BtReadFlash(const char *path, void *buf, uint32_t nByte, int32_t position)
{
    int32_t ret = ERRCODE_FAIL;
    int fd = -1;

    do {
        if ((path == NULL) || (buf == NULL)) {
            wstp_print("path is NULL Or buf is NULL\r\n");
            break;
        }

        fd = open(path, O_RDWR | O_CREAT, S_IREAD | S_IWRITE);
        if (fd < 0) {
            wstp_print("open err\r\n");
            break;
        }

        if (lseek(fd, position, SEEK_SET) < 0) {
            wstp_print("lseek err\r\n");
            break;
        }

        ret = read(fd, buf, nByte);
        ret = (ret < 0) ? (ERRCODE_FAIL) : (ERRCODE_SUCC);
    } while (0);

    if (fd >= 0) {
        close(fd);
    }
    return ret;
}

static int32_t BtWriteFlash(const char *path, const void *buf, uint32_t nByte, int32_t position)
{
    int32_t ret = ERRCODE_FAIL;
    int fd = -1;

    if (g_fileStatus == BT_MANAGER_FILE_LOCK) {
        wstp_print("file lock\r\n");
        return ret;
    }

    do {
        if ((path == NULL) || (buf == NULL)) {
            wstp_print("path is NULL Or buf is NULL\r\n");
            break;
        }
        fd = open(path, O_RDWR | O_CREAT, S_IREAD | S_IWRITE);
        if (fd < 0) {
            wstp_print("open err\r\n");
            break;
        }

        if (lseek(fd, position, SEEK_SET) < 0) {
            wstp_print("lseek err\r\n");
            break;
        }
        if (((uint32_t)(write(fd, buf, nByte))) != nByte) {
            wstp_print("write err\r\n");
            break;
        }
        ret = ERRCODE_SUCC;
    } while (0);

    if (fd >= 0) {
        close(fd);
    }
    return ret;
}
#endif
// bt nv debug need enable
#if ((defined CHIP) && (!CHIP_LIBRA) && (CHIP != CHIP_BRANDY))
static void BtManagerRfPowerSetByCustomizedArea(uint8_t *data, int32_t fileSize)
{
    errno_t res;
    NvErrorCode result;
    /* meas BLE 1M maxPower 12db, BLE 2M maxPower 12db, Br maxPower 14db, Edr maxPower 12db */
    uint8_t customizedAreaJapanKorea[] = { 12, 0, 12, 0, 14, 0, 12, 0 };
    /* meas BLE 1M maxPower 14db, BLE 2M maxPower 14db, Br maxPower 14db, Edr maxPower 12db */
    uint8_t customizedAreaOther[] = { 14, 0, 14, 0, 14, 0, 12, 0 };
    uint8_t flagValue[NV_ID_CUST_LEN] = {0};

    if ((data == NULL) || (fileSize < (int32_t)(sizeof(customizedAreaOther)))) { /*lint !e574*/
        wstp_print("param err\r\n");
        return;
    }
    result = MC_NVRead(NV_ID_CUST, (NV_LENGTH_S *)&flagValue); /*lint !e545*/
    /* 0xF4 is Japan or Korea */
    if ((result == NV_OK_E) && (flagValue[0] == 0xF4)) {
        res = memcpy_s(data, fileSize, customizedAreaJapanKorea, sizeof(customizedAreaJapanKorea));
    } else {
        res = memcpy_s(data, fileSize, customizedAreaOther, sizeof(customizedAreaOther));
    }
    if (res != EOK) {
        wstp_print("memcpy err\r\n");
    }
}

#if (CONFIG_BT_FILE_ENCRYPTION == FEATURE_ON)
static int32_t BtReadFileAndDecryption(const char *fileName, void *buf, uint32_t nByte)
{
    int32_t currentFileLen;
    uint16_t encyptionOutputLen;
    int32_t addFlashSize = 0;
    int32_t status = ERRCODE_FAIL;
    uint16_t outDataLen = g_fileLen;

    do {
        if (g_fileBuf == NULL) {
            g_fileBuf = malloc(BT_FILE_ENCY_TOTAL_LEN);
            //g_fileBuf = MC_SVR_MEM_ALLOC(BT_FILE_ENCY_TOTAL_LEN); /* g_fileBuf will never be free. */
            if (g_fileBuf == NULL) {
                wstp_print("malloc err\r\n");
                break;
            }
            if (memset_s(g_fileBuf, BT_FILE_ENCY_TOTAL_LEN, 0, BT_FILE_ENCY_TOTAL_LEN) != EOK) {
                wstp_print("memset err\r\n");
                break;
            }
        }
        currentFileLen = BtGetFileSize(fileName) -
            MC_SEC_AES_ENC_INC_LEN - MC_SEC_AES_IV_LEN - BT_FILE_ENCYPTION_OUTPUT_LEN;
        if (currentFileLen < 0) {
            currentFileLen = 0;
        }
        if (currentFileLen < g_fileLen) {
            addFlashSize = g_fileLen - currentFileLen;
        }
        /* 0 indicates that the entire file is read from byte 0. */
        status = BtReadFlash(fileName, g_fileBuf, BT_FILE_ENCY_TOTAL_LEN - addFlashSize, 0);
        if (((uint32_t)status) == ERRCODE_FAIL) {
            wstp_print("read flash err\r\n");
            break;
        }
        encyptionOutputLen = *(uint16_t *)(BT_FILE_ENCYPTION_OUTPUTLEN_START - addFlashSize);
        if ((encyptionOutputLen > 0) && (encyptionOutputLen < BT_FILE_ENCY_TOTAL_LEN)) {
            status = MC_Sec_AES_StoreDec(g_fileBuf, encyptionOutputLen,
                &outDataLen, BT_FILE_ENCY_IV_START - addFlashSize);
            if (status != ERRCODE_SUCC) {
                wstp_print("MC_Sec_AES_StoreDec err\r\n");
                break;
            }
        }
        if (addFlashSize > 0) {
            if (memset_s(g_fileBuf + currentFileLen, addFlashSize, 0, addFlashSize) != EOK) {
                wstp_print("memset err\r\n");
            }
            BtWriteFileAndEncyption(fileName, g_fileBuf, g_fileLen, 0);
        }
        if (memcpy_s(buf, nByte, g_fileBuf, outDataLen) != EOK) {
            status = ERRCODE_FAIL;
            wstp_print("memcpy err\r\n");
            break;
        }
    } while (0);

    return status;
}
#endif

static int32_t BtWriteFileAndEncyption(const char *fileName, const void *buf, uint32_t nByte, int32_t position)
{
    uint16_t outDataLen;
    int32_t status = ERRCODE_FAIL;
    uint16_t *encyptionOutputLen = (uint16_t *)BT_FILE_ENCYPTION_OUTPUTLEN_START;

    do {
        if (g_fileBuf == NULL) {
            wstp_print("g_fileBuf is NULL\r\n");
            break;
        }
        if (buf != g_fileBuf) {
            if (memcpy_s(g_fileBuf + position, BT_FILE_ENCY_TOTAL_LEN - position, buf, nByte) != EOK) {
                wstp_print("memcpy err\r\n");
                break;
            }
        }
        status = MC_Sec_AES_StoreEnc(g_fileBuf, g_fileLen, &outDataLen, BT_FILE_ENCY_IV_START);
        if (status != ERRCODE_SUCC) {
            wstp_print("MC_Sec_AES_StoreEnc err\r\n");
            break;
        }
        *encyptionOutputLen = outDataLen;
        status = BtWriteFlash(fileName, g_fileBuf, BT_FILE_ENCY_TOTAL_LEN, 0);
        if (status != ERRCODE_SUCC) {
            wstp_print("BtWriteFlash err\r\n");
            break;
        }
        status = MC_Sec_AES_StoreDec(g_fileBuf, outDataLen, &outDataLen, BT_FILE_ENCY_IV_START);
        if (status != ERRCODE_SUCC) {
            wstp_print("MC_Sec_AES_StoreDec err\r\n");
            break;
        }
    } while (0);

    return status;
}
#endif

// bt nv debug need enable
#if ((!CHIP_LIBRA)&& (!CHIP_BRANDY))
static void BtManagerRead(const bt_manager_commu_inter_info_t *interInfo)
{
    int32_t fileSize = 0;
    uint8_t *dataBuf = NULL;
    int status = ERRCODE_FAIL;

    do {
        if ((interInfo->payloadLength == 0) || (&interInfo->payload[0] == NULL)) {
            break;
        }
        if (interInfo->payloadLength == sizeof(uint8_t)) {
            g_fileLen = OLD_FILE_LEN;
        } else if (interInfo->payloadLength == sizeof(uint16_t)) {
            g_fileLen = BTOU16(interInfo->payload);
        }
        fileSize = BtGetFileSize(FS_FILE_PATH_BT_MANAGER_FILE);
        if (fileSize < BT_CONFIG_CALIBRATION_INFO_LEN) {
            fileSize = BT_CONFIG_CALIBRATION_INFO_LEN;
        } else {
            fileSize = (fileSize > g_fileLen) ? g_fileLen : fileSize;
        }
        /* mem layout {status, buf[0], buf[1], buf[2], ...buf[fileSize - 1]} */
        dataBuf = malloc(fileSize + 1);
        if (dataBuf == NULL) {
            fileSize = 0;
            break;
        }
        /* mem layout {status, buf[0], buf[1], buf[2], ...buf[fileSize - 1]} */
#if (CONFIG_BT_FILE_ENCRYPTION == FEATURE_ON)
        status = BtReadFileAndDecryption(FS_FILE_PATH_BT_MANAGER_FILE, dataBuf + 1, (uint32_t)fileSize);
#else
        status = BtReadFlash(FS_FILE_PATH_BT_MANAGER_FILE, dataBuf + 1, (uint32_t)fileSize, 0);
#endif
        if (status != ERRCODE_SUCC) {
            wstp_print("BtRead err\r\n");
        }
        dataBuf[0] = status;
    } while (0);
    if (dataBuf != NULL) {
        BtManagerRfPowerSetByCustomizedArea(dataBuf + sizeof(uint8_t) + BT_MANAGER_FILE_POWER_OFFSET,
            fileSize - sizeof(uint8_t) - BT_MANAGER_FILE_POWER_OFFSET);
    }

    if (dataBuf != NULL) {
        free(dataBuf);
    }
}

static void BtManagerWrite(const bt_manager_commu_inter_info_t *interInfo)
{
    uint16_t length;
    int32_t offset;
    bt_manager_default_t *dataBuf = NULL;
    int status = ERRCODE_FAIL;

    do {
        if (interInfo == NULL) {
            wstp_print("interInfo is NULL\r\n");
            break;
        }
        if ((interInfo->payloadLength != 0) && (&interInfo->payload[0] == NULL)) {
            wstp_print("payload err\r\n");
            break;
        }

        length = interInfo->payloadLength;
        dataBuf = (bt_manager_default_t *)interInfo->payload;
        offset = dataBuf->offset;

        if (length <= sizeof(uint16_t)) {
            wstp_print("length err\r\n");
            break;
        }
#if (CONFIG_BT_FILE_ENCRYPTION == FEATURE_ON)
        status = BtWriteFileAndEncyption(FS_FILE_PATH_BT_MANAGER_FILE, dataBuf->data,
            (length - sizeof(uint16_t)), offset);
#else
        status = BtWriteFlash(FS_FILE_PATH_BT_MANAGER_FILE, dataBuf->data,
                              (length - sizeof(uint16_t)), offset);
#endif
        if (status != ERRCODE_SUCC) {
            status = ERRCODE_FAIL;
            wstp_print("BtWrite err\r\n");
        }
    } while (0);

    // BtManagerSendDataToBt(BT_MANAGER_SVR, MC_BT_MANAGER_CHANGE_RSP, sizeof(status), (uint8_t *)&status);
}

static void BtManagerNotify(void)
{
    uint32_t bufLen = sizeof(bt_manager_svc_inter_info_t) + sizeof(macHighPower);
    bt_manager_svc_inter_info_t *buf = NULL;
    uint8_t *btMgrBuf = NULL;

    buf = (bt_manager_svc_inter_info_t *)malloc(bufLen);
    if (buf == NULL) {
        wstp_print("malloc(%d) err\r\n", bufLen);
        return;
    }
    btMgrBuf = (uint8_t *)&(buf->data[0]);

    if (memset_s(buf, bufLen, 0x0, bufLen) != EOK) {
        wstp_print("memset_s err\r\n");
        return;
    }

    // buf->cmdID = BT_MANAGER_SVC_UI_INFO_CHANGE;
    buf->length = sizeof(macHighPower);
    if (memcpy_s(btMgrBuf, sizeof(macHighPower), macHighPower, sizeof(macHighPower)) != EOK) {
        wstp_print("memcpy_s err\r\n");
        free(buf);
        buf = NULL;
        return;
    }

#if (CONFIG_FT_MODE == FEATURE_ON)
    free(buf);
#else
    int32_t ret;
    ret = MC_MsgSnd(MC_PROP_BT_MANAGER_MAC_INFO_CHANGE, 0, buf);
    if (ret == false) {
        free(buf);
        wstp_print("MC_MsgSnd err\r\n");
        return;
    }
#endif

    return;
}

static int32_t BtManagerHightPowerStatus(const bt_manager_commu_inter_info_t *interInfo)
{
    if ((interInfo == NULL) || (interInfo->payloadLength < BT_MAC_LEN_AND_IS_SNIFF)) {
        wstp_print("input interInfo err\r\n");
        return ERRCODE_FAIL;
    }

    if (memset_s(macHighPower, sizeof(macHighPower), 0x0, sizeof(macHighPower)) != EOK) {
        wstp_print("memset_s err\r\n");
        return ERRCODE_FAIL;
    }

    if (memcpy_s(macHighPower, BT_MAC_LEN_AND_IS_SNIFF, interInfo->payload,
                 BT_MAC_LEN_AND_IS_SNIFF) != EOK) {
        wstp_print("memcpy_s err\r\n");
        return ERRCODE_FAIL;
    }
    BtManagerNotify();

    return ERRCODE_SUCC;
}

static int32_t BtManagerPdlOperation(const bt_manager_commu_inter_info_t *interInfo)
{
    uint8_t uSrcTranId;

    uSrcTranId = interInfo->interHeader.tranID & BT_SRC_TRANID_MSK;
    /* if MC_AP == uSrcTranId then forward message to BT Core */
    // if (uSrcTranId == MC_AP) {
    //     BtManagerSendDataToBt(BT_MANAGER_SVR, MC_BT_MANAGER_PDL_OPERATION, interInfo->payloadLength,
    //                           interInfo->payload);
    // } else {
    //     wstp_print("not supported send to BT Core\r\n");
    //     return ERRCODE_FAIL;
    // }

    return ERRCODE_SUCC;
}

static int32_t BtManagerPowerControl(const bt_manager_commu_inter_info_t *interInfo)
{
    uint8_t powerOpt;
    if ((interInfo == NULL) || (interInfo->payloadLength < 1)) {
        wstp_print("input interInfo err\r\n");
        return ERRCODE_FAIL;
    }
        powerOpt = interInfo->payload[POS_0];
        if (powerOpt == BT_MANAGER_POWER_ON_BT) {
        low_power_control(LOW_POWER_BT_POWER_UP_AND_RUN);
        wstp_print("BT poweron\r\n");
    } else if (powerOpt == BT_MANAGER_POWER_OFF_BT) {
        (void)ipc_send_message(CORES_BT_CORE, IPC_ACTION_BT_POWER_OFF, NULL, 0, IPC_PRIORITY_LOWEST, false);
        tcxo_delay_ms(20ULL);
        low_power_control(LOW_POWER_BT_POWER_OFF);
        ipc_set_outbox_free(CORES_BT_CORE);
        wstp_print("BT poweroff\r\n");
    } else {
        wstp_print("input err(powerOpt=%d)\r\n", powerOpt);
        return ERRCODE_FAIL;
    }

    return ERRCODE_SUCC;
}

#if (CONFIG_BT_CALI_RXDC == FEATURE_ON)
static uint32_t BtSetXoCap(BtTransStru *trans, XoCapStruc *xoCap)
{
    do {
        if ((trans->lengthL != sizeof(uint8_t)) && (trans->lengthH > 0)) {
            wstp_print("input trans err\r\n");
            return ERRCODE_FAIL;
            break;
        }
        if (trans->dataId == XO_CAP_VALUE) {
            xoCap->xoValue = trans->data[0];
        } else if (trans->dataId == XO_CAP_CTRIM) {
            xoCap->xoCtrim = trans->data[0];
        } else {
            return ERRCODE_FAIL;
        }
        calibration_set_xo_core_ctrim(xoCap->xoValue, (xo_ctrim_decimal_t)xoCap->xoCtrim);
    } while (0);
    return ERRCODE_SUCC;
}

static uint32_t BtEfuseXoCap(XoCapStruc *xoCap)
{
    uint8_t optXoValue;
    calibration_save_xo_core_ctrim(xoCap->xoValue, (xo_ctrim_decimal_t)xoCap->xoCtrim);
    calibration_read_xo_core_ctrim(&optXoValue);
    if (optXoValue == *(uint8_t *)xoCap) {
        return ERRCODE_SUCC;
    } else {
        return ERRCODE_FAIL;
    }
}

static void BtProcXoCap(const bt_manager_commu_inter_info_t *interInfo)
{
    uint8_t optXoValue;
    BtTransStru *trans = (BtTransStru *)interInfo->payload;
    XoCapStruc *xoCap = NULL;
    static XoResStru resData = {
        .status = ERRCODE_SUCC,
        .xoCap.xoCtrim = 0x6, /* means 1.5pf step 0.25pf */
        .xoCap.xoValue = 0xE, /* means 14pf step 1pf */
        .xoCap.reserved = 0x0, /* reserved */
    };
    static bool initDone = false;

    if (!initDone) {
        calibration_read_xo_core_ctrim(&optXoValue);
        if (optXoValue != 0) {
            resData.value = optXoValue;
        }
        initDone = true;
    }
    xoCap = &resData.xoCap;
    if (trans->operation == XO_OPERA_WRITE) {
        resData.status = BtSetXoCap(trans, xoCap);
        // BtManagerRespose(interInfo->interHeader.tranID, MC_BT_FACTORY_TEST_SVR,
        //     MC_BT_FACTORY_XO_CAP, sizeof(resData.status), (uint8_t *)&resData.status);
    } else if (trans->operation == XO_OPERA_READ) {
        XoResStru readData = {0};
        readData.status = 0;
        if (trans->dataId == XO_CAP_VALUE) {
            readData.value = resData.xoCap.xoValue;
        } else if (trans->dataId == XO_CAP_CTRIM) {
            readData.value = resData.xoCap.xoCtrim;
        } else {
            readData.status = ERRCODE_FAIL;
        }
        // BtManagerRespose(interInfo->interHeader.tranID, MC_BT_FACTORY_TEST_SVR,
        //     MC_BT_FACTORY_XO_CAP, sizeof(readData), (uint8_t *)&readData);
    } else if (trans->operation == XO_OPERA_SAVE) {
        resData.status = BtEfuseXoCap(xoCap);
        // BtManagerRespose(interInfo->interHeader.tranID, MC_BT_FACTORY_TEST_SVR,
        //     MC_BT_FACTORY_XO_CAP, sizeof(resData.status), (uint8_t *)&resData.status);
    } else {
        resData.status = ERRCODE_FAIL;
        // BtManagerRespose(interInfo->interHeader.tranID, MC_BT_FACTORY_TEST_SVR,
        //     MC_BT_FACTORY_XO_CAP, sizeof(resData.status), (uint8_t *)&resData.status);
    }
}
#endif

bool McBtManagerFactoryReset(void)
{
    int32_t status = ERRCODE_FAIL;
    uint8_t *resetBuffer = NULL;

    do {
        resetBuffer = (uint8_t *)malloc(g_fileLen);
        if (resetBuffer == NULL) {
            wstp_print("malloc err\r\n");
            break;
        }
        if (memset_s(resetBuffer, g_fileLen, 0, g_fileLen) != EOK) {
            wstp_print("memset err\r\n");
            break;
        }

        g_fileStatus = BT_MANAGER_FILE_OPERABLE;
#if (CONFIG_BT_FILE_ENCRYPTION == FEATURE_ON)
        int32_t i;
        int32_t offset;
        if (memcpy_s(resetBuffer, BT_CONFIG_CALIBRATION_INFO_LEN, g_fileBuf, BT_CONFIG_CALIBRATION_INFO_LEN) != EOK) {
            wstp_print("memcpy err_01\r\n");
            break;
        }
        if (memcpy_s(resetBuffer + BT_MANAGER_FILE_PHONE_LINKKEY_OFFSET,
                        BT_MANAGER_FILE_PHONE_LINKKEY_LENGTH * BT_LINKKEY_NUM,
                        g_fileBuf + BT_MANAGER_FILE_PHONE_LINKKEY_OFFSET,
                        BT_MANAGER_FILE_PHONE_LINKKEY_LENGTH * BT_LINKKEY_NUM) != EOK) {
            wstp_print("memcpy err_02\r\n");
            break;
        }
        for (i = 0; i < BT_LINKKEY_NUM; i++) {
            offset = BT_MANAGER_FILE_PHONE_LINKKEY_OFFSET + (i * BT_MANAGER_FILE_PHONE_LINKKEY_LENGTH) +
                        BT_LINKKEY_PROFILE_OFFSET;
            resetBuffer[offset] = 0;
        }
        status = BtWriteFileAndEncyption(FS_FILE_PATH_BT_MANAGER_FILE, resetBuffer, g_fileLen, 0);
#else
        if (BtGetFileSize(FS_FILE_PATH_BT_MANAGER_FILE) < BT_CONFIG_CALIBRATION_INFO_LEN) {
            status = ERRCODE_SUCC;
            break;
        }
        status = BtReadFlash(FS_FILE_PATH_BT_MANAGER_FILE, resetBuffer, BT_CONFIG_CALIBRATION_INFO_LEN, SEEK_SET);
        if (status != ERRCODE_SUCC) {
            wstp_print("BtReadFlash err\r\n");
            break;
        }
        /* this step dont care return vale */
        (void)BtReadFlash(FS_FILE_PATH_BT_MANAGER_FILE, resetBuffer + BT_MANAGER_FILE_PHONE_LINKKEY_OFFSET,
                             BT_MANAGER_FILE_PHONE_LINKKEY_LENGTH, BT_MANAGER_FILE_PHONE_LINKKEY_OFFSET);
        resetBuffer[BT_MANAGER_FILE_PHONE_PROFILE_OFFSET] = 0;
        status = BtWriteFlash(FS_FILE_PATH_BT_MANAGER_FILE, resetBuffer, g_fileLen, SEEK_SET);
#endif
        g_fileStatus = BT_MANAGER_FILE_LOCK;
    } while (0);

    if (resetBuffer != NULL) {
        free(resetBuffer);
        resetBuffer = NULL;
    }
    if (status == ERRCODE_SUCC) {
        return true;
    } else {
        return false;
    }
}
bool McBtManagerCleanKeyReset(void)
{
    int32_t status = ERRCODE_FAIL;
    uint8_t *resetBuffer = NULL;
    uint8_t reserved = 0;
    do {
        resetBuffer = (uint8_t *)malloc(g_fileLen);
        if (resetBuffer == NULL) {
            wstp_print("malloc err\r\n");
            break;
        }
        if (memset_s(resetBuffer, g_fileLen, 0, g_fileLen) != EOK) {
            wstp_print("memset err\r\n");
            break;
        }

        g_fileStatus = BT_MANAGER_FILE_OPERABLE;
#if (CONFIG_BT_FILE_ENCRYPTION == FEATURE_ON)
        if (memcpy_s(resetBuffer, BT_CONFIG_CALIBRATION_INFO_LEN, g_fileBuf, BT_CONFIG_CALIBRATION_INFO_LEN) != EOK) {
            wstp_print("memcpy err\r\n");
            break;
        }
        status = BtWriteFileAndEncyption(FS_FILE_PATH_BT_MANAGER_FILE, resetBuffer, g_fileLen, 0);
#else
        if (BtGetFileSize(FS_FILE_PATH_BT_MANAGER_FILE) < BT_CONFIG_CALIBRATION_INFO_LEN) {
            status = ERRCODE_SUCC;
            break;
        }
        status = BtReadFlash(FS_FILE_PATH_BT_MANAGER_FILE, resetBuffer, BT_CONFIG_CALIBRATION_INFO_LEN, SEEK_SET);
        if (status != ERRCODE_SUCC) {
            wstp_print("BtReadFlash err\r\n");
            break;
        }
        status = BtWriteFlash(FS_FILE_PATH_BT_MANAGER_FILE, resetBuffer, g_fileLen, SEEK_SET);
#endif
        g_fileStatus = BT_MANAGER_FILE_LOCK;
    } while (0);

    if (resetBuffer != NULL) {
        free(resetBuffer);
        resetBuffer = NULL;
    }

    // BtManagerSendDataToBt(BT_MANAGER_SVR, MC_BT_MANAGER_FAC_RESET, sizeof(uint8_t), &reserved);
    BtResetStartTimer(BT_RESET_TIMER_TIMEOUT);
    if (status == ERRCODE_SUCC) {
        return true;
    } else {
        return false;
    }
}
void McBtManagerInit(void)
{
    return;
}
#endif

typedef uint32_t (*btsrv_at_cmd_handle_func)(const bt_manager_commu_inter_info_t *at_para);

typedef struct {
    uint32_t id;
    btsrv_at_cmd_handle_func func;
} btsrv_at_cmd_hdl_info;

void bt_manager_at_init(void)
{
#ifdef SUPPORT_DIAG_V2_PROTOCOL
    uapi_diag_service_register(DIAG_SER_BT_TEST, bt_diag_cmd_process);
#endif
}

uint32_t btsrv_wstp_at_test(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
#ifdef SUPPORT_BREDR
    sample_spp_register_callbacks();
#endif
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_enable_bt(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
#if defined(SW_UART_DEBUG) || defined(SW_RTT_DEBUG)
    btsrv_set_log_type_level(BTSRV_LOG_UART_INFO);
#endif
    btsrv_at_cmd_gap_register_callbacks();
#ifdef SUPPORT_BREDR
    wstp_a2dp_src_register_callbacks();
    wstp_a2dp_snk_register_callbacks();
    wstp_avrcp_register_callbacks();
#endif
    errcode_t res = enable_bt_stack();
    if (res != 0) {
        wstp_print("bt enable fail\r\n");
        return ERRCODE_FAIL;
    } else {
        wstp_print("bt enable ok\r\n");
        return ERRCODE_SUCC;
    }
}

uint32_t btsrv_wstp_at_disable_bt(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    ble_dis_client_clear_remote_all();
    ble_bas_client_clear_remote_all();
    ble_fmp_locator_clear_remote_all();
    ble_ancs_client_clear_remote_all();
    ble_ams_client_clear_remote_all();
    errcode_t res = disable_bt_stack();
    if (res != 0) {
        wstp_print("bt disable fail\r\n");
        return ERRCODE_FAIL;
    } else {
        wstp_print("bt disable ok\r\n");
        return ERRCODE_SUCC;
    }
}

#ifdef SUPPORT_BREDR
uint32_t btsrv_wstp_at_hfp_reg(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_hfp_hf_reg_callback();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_pbap_reg(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_pbap_pce_reg_callback();
    wstp_print("pbap service registered\n");
    return ERRCODE_SUCC;
}
#endif

uint32_t btsrv_wstp_at_set_local_addr(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }

    errcode_t res = bluetooth_set_local_addr(bd_addr->addr, (uint8_t)at_para->payloadLength);
    BSP_MEM_FREE((void *)bd_addr);
    if (res == 0) {
        wstp_print("bluetooth_set_local_addr OK\r\n");
        return ERRCODE_SUCC;
    } else {
        wstp_print("bluetooth_set_local_addr fail\r\n");
        return ERRCODE_FAIL;
    }
}

uint32_t btsrv_wstp_at_get_local_addr(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    unsigned int len = BT_MAC_LEN;
    unsigned char addr[BT_MAC_LEN] = {0};
    errcode_t res = bluetooth_get_local_addr(&addr[0], len);
    if (res == 0) {
        char str[64] = "^PHYNUM:MACBT,";
        strcat_s(str, strlen(str) + strlen((char*)addr), (char*)addr);
        wstp_print("%s\r\n", &str[0]);
        wstp_print("local_addr : ******%02x%02x%02x\n", addr[3], addr[4], addr[5]);
        return ERRCODE_SUCC;
    } else {
        wstp_print("bluetooth_get_local_addr err\r\n");
        return ERRCODE_FAIL;
    }
}

uint32_t btsrv_wstp_at_set_local_name(const bt_manager_commu_inter_info_t *at_para)
{
    /* 减去结束符长度 */
    uint8_t name_len = at_para->payloadLength - 1;
    if (name_len <= 0) {
        wstp_print("Invalid name length\r\n");
        return ERRCODE_FAIL;
    }
    unsigned char *bd_name = (unsigned char *)BSP_MEM_ALLOC(name_len * sizeof(unsigned char));
    int ret = memcpy_s(bd_name, name_len, &at_para->payload[POS_0], name_len);
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE((void *)bd_name);
        return ERRCODE_FAIL;
    }
    errcode_t res = bluetooth_set_local_name(bd_name, name_len);
    BSP_MEM_FREE((void *)bd_name);
    if (res == 0) {
        char str[] = "OK";
        wstp_print("%s\r\n", &str[0]);
        return ERRCODE_SUCC;
    } else {
        return ERRCODE_FAIL;
    }
}

uint32_t btsrv_wstp_at_get_local_name(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    unsigned char len = 248;
    unsigned char name[248] = {0};
    errcode_t res = bluetooth_get_local_name(name, &len);
    if (res == 0) {
        char str[256] = "^btname:";
        strcat_s(str, strlen(str) + strlen((char*)name), (char*)name);
        wstp_print("%s\r\n", &str[0]);
        wstp_print("name: %s, length: %d\n", name, len);
        return ERRCODE_SUCC;
    } else {
        return ERRCODE_FAIL;
    }
}

uint32_t btsrv_wstp_at_factory_reset(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    bool res = bluetooth_factory_reset();
    if (res) {
        char str[] = "OK";
        wstp_print("%s\r\n", &str[0]);
    }
    return (uint32_t)res;
}

uint32_t btsrv_wstp_at_bt_start_discovery(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    reset_found_dev_list();
    bool res = gap_br_start_discovery();
    if (res) {
        char str[] = "OK";
        wstp_print("%s\r\n", &str[0]);
    }

    return (uint32_t)res;
}

uint32_t btsrv_wstp_at_bt_cancel_discovery(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    bool res = gap_br_cancel_discovery();
    if (res) {
        char str[] = "OK";
        wstp_print("%s\r\n", &str[0]);
    }

    return (uint32_t)res;
}

uint32_t btsrv_wstp_at_bt_read_discovery_info(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    update_found_dev_list();
    show_found_dev_list();
    return ERRCODE_SUCC;
}

#ifdef SUPPORT_BREDR
uint32_t btsrv_wstp_at_hfp_pick_up(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    int retcode = OHOS_BT_STATUS_SUCCESS;
    retcode = hfp_hf_accept_incoming_call(NULL, OHOS_HFP_HF_ACCEPT_CALL_ACTION_NONE);
    wstp_print("hfp pick up call res=0x%x\n", retcode);

    return (uint32_t)retcode;
}

uint32_t btsrv_wstp_at_hfp_hang_up(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    int retcode = OHOS_BT_STATUS_SUCCESS;
    retcode = hfp_hf_finish_call(NULL);
    wstp_print("hfp hang up call res=0x%x\n", retcode);

    return (uint32_t)retcode;
}

uint32_t btsrv_wstp_at_hfp_hold_active_call(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    int retcode = OHOS_BT_STATUS_SUCCESS;
    retcode = hfp_hf_hold_active_call(NULL);
    wstp_print("hfp hold active call res=0x%x\n", retcode);

    return (uint32_t)retcode;
}

uint32_t btsrv_wstp_at_hfp_reject(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    int retcode = OHOS_BT_STATUS_SUCCESS;
    retcode = hfp_hf_reject_incoming_call(NULL);
    wstp_print("hfp reject call res=0x%x\n", retcode);

    return (uint32_t)retcode;
}

uint32_t btsrv_wstp_at_bt_open_inqscan(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    bool res = gap_br_set_bt_scan_mode(OHOS_GAP_SCAN_MODE_CONNECTABLE_GENERAL_DISCOVERABLE, 0);
    if (res) {
        char str[] = "OK";
        wstp_print("%s\r\n", &str[0]);
    }
    wstp_print("open inqscan res=0x%x\n", res);
    return (uint32_t)res;
}

uint32_t btsrv_wstp_at_bt_close_inqscan(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    bool res = gap_br_set_bt_scan_mode(OHOS_GAP_SCAN_MODE_NONE, 0);
    if (res) {
        char str[] = "OK";
        wstp_print("%s\r\n", &str[0]);
    }
    wstp_print("    close inqscan res=0x%x\n", res);
    return (uint32_t)res;
}

uint32_t btsrv_wstp_at_bt_conn_by_addr(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    errno_t ret = 0;
    do {
        if (at_para->reserved == true) {
            ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_9], sizeof(bd_addr->addr));
        } else {
            ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
        }
        if (ret != EOK) {
            wstp_print("memcpy err\r\n");
            ret = false;
            break;
        }
        wstp_print("addr=%02x%02x%02x%02x%02x%02x\n",
            bd_addr->addr[POS_0],
            bd_addr->addr[POS_1],
            bd_addr->addr[POS_2],
            bd_addr->addr[POS_3],
            bd_addr->addr[POS_4],
            bd_addr->addr[POS_5]);
        ret = wstp_connect_remote_device(bd_addr);
    } while (0);
    BSP_MEM_FREE((void *)bd_addr);

    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_bt_cancel_pair(const bt_manager_commu_inter_info_t *at_para)
{
    bd_addr_t *bd_addr = (bd_addr_t*)BSP_MEM_ALLOC(sizeof(bd_addr_t));
    wstp_print("addr=%02x%02x%02x%02x%02x%02x \n",
        at_para->payload[POS_0], at_para->payload[POS_1], at_para->payload[POS_2],
        at_para->payload[POS_3], at_para->payload[POS_4], at_para->payload[POS_5]);
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    bool res = gap_remove_pair(bd_addr);
    if (res) {
        char str[] = "OK";
        wstp_print("%s\r\n", &str[0]);
    }
    else {
        wstp_print("gap_remove_pair err\r\n");
    }
    BSP_MEM_FREE((void *)bd_addr);
    return (uint32_t)res;
}

uint32_t btsrv_wstp_at_bt_is_connected(const bt_manager_commu_inter_info_t *at_para)
{
    bd_addr_t *bd_addr = (bd_addr_t*)BSP_MEM_ALLOC(sizeof(bd_addr_t));
    errno_t ret = 0;
    wstp_print("addr=%02x%02x%02x%02x%02x%02x \n",
        at_para->payload[POS_0], at_para->payload[POS_1], at_para->payload[POS_2],
        at_para->payload[POS_3], at_para->payload[POS_4], at_para->payload[POS_5]);
    do {
        if (at_para->reserved == true) {
            ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_9], sizeof(bd_addr->addr));
        } else {
            ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
        }
        if (ret != EOK) {
            wstp_print("memcpy err\r\n");
            ret = false;
            break;
        }
        wstp_print("addr=%02x%02x%02x%02x%02x%02x\n",
            bd_addr->addr[POS_0],
            bd_addr->addr[POS_1],
            bd_addr->addr[POS_2],
            bd_addr->addr[POS_3],
            bd_addr->addr[POS_4],
            bd_addr->addr[POS_5]);
        ret = bt_is_acl_connected((const bd_addr_t *)bd_addr);
    } while (0);
    wstp_print("device connect status: %d\n", ret);
    BSP_MEM_FREE((void *)bd_addr);

    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_bt_disconnect(const bt_manager_commu_inter_info_t *at_para)
{
    bd_addr_t *bd_addr = (bd_addr_t*)BSP_MEM_ALLOC(sizeof(bd_addr_t));
    errno_t ret = 0;
    do {
        if (at_para->reserved == true) {
            ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_9], sizeof(bd_addr->addr));
        } else {
            ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
        }
        if (ret != EOK) {
            wstp_print("memcpy err\r\n");
            ret = false;
            break;
        }
        wstp_print("addr=%02x%02x%02x%02x%02x%02x\n",
            bd_addr->addr[POS_0],
            bd_addr->addr[POS_1],
            bd_addr->addr[POS_2],
            bd_addr->addr[POS_3],
            bd_addr->addr[POS_4],
            bd_addr->addr[POS_5]);
        ret = gap_disconnect_remote_device(bd_addr);
    } while (0);
    wstp_print("gap_disconnect_remote_device ret=0x%x\r\n", ret);
    BSP_MEM_FREE((void *)bd_addr);

    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_bt_set_spk_vol(const bt_manager_commu_inter_info_t *at_para)
{
    sample_hfp_hf_set_spk_volume(at_para->payload[POS_0]);
    int ret = hfp_hf_set_volume(NULL, OHOS_HFP_VOLUME_SPEAKER, at_para->payload[POS_0]);
    if (!ret) {
        char str[] = "OK";
        wstp_print("%s\r\n", &str[0]);
    }
    else {
        wstp_print("hfp_hf_set_volume err\r\n");
    }
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_bt_set_mic_vol(const bt_manager_commu_inter_info_t *at_para)
{
    int ret = hfp_hf_set_volume(NULL, OHOS_HFP_VOLUME_MIC, at_para->payload[POS_0]);
    if (!ret) {
        char str[] = "OK";
        wstp_print("%s\r\n", &str[0]);
    }
    else {
        wstp_print("hfp_hf_set_volume err\r\n");
    }
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_bt_start_dail(const bt_manager_commu_inter_info_t *at_para)
{
    uint8_t number[BT_HFP_MAX_NUM_LEN] = {0};
    int number_len = 0;
    int payload_pos = 0;
    /* 减去结束符长度 */
    number_len = at_para->payloadLength - 1;
    wstp_print("btsrv_wstp_at_bt_start_dail number_len=%d number=\n", number_len);
    if (number_len > BT_HFP_MAX_NUM_LEN || number_len < BT_HFP_MIN_NUM_LEN) {
        wstp_print("invalid phone number\n");
        return ERRCODE_FAIL;
    }
    number_len += payload_pos;
    for (int i = payload_pos; i < number_len; i++) {
        number[i - payload_pos] = at_para->payload[i];
        wstp_print("0x%x ", at_para->payload[i]);
    }
    wstp_print("\n");
    bth_bd_addr *bd_addr = NULL;
    uint32_t ret = hfp_hf_start_dial(bd_addr, number, at_para->payloadLength);
    if (!ret) {
        wstp_print("hfp_hf_start_dial ok\r\n");
        char str[] = "OK";
        wstp_print("%s\r\n", &str[0]);
    }
    else {
        wstp_print("hfp_hf_start_dial err\r\n");
    }
    return (uint32_t)ret;
}

/* AVRCP */
uint32_t btsrv_wstp_at_avrcp_reg(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    wstp_avrcp_register_callbacks();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_avrcp_disreg(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    wstp_avrcp_disregister_callbacks();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_avrcp_tg_conn(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    ret = wstp_avrcp_tg_connect(&bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_avrcp_tg_disconn(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    ret = wstp_avrcp_tg_disconnect(&bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_avrcp_tg_get_dev_conn_state(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    ret = wstp_avrcp_tg_get_dev_conn_state(&bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_avrcp_tg_notify_state(const bt_manager_commu_inter_info_t *at_para)
{
    wstp_avrcp_tg_notify_state(at_para->payload, at_para->payloadLength);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_avrcp_tg_set_loc_inf(const bt_manager_commu_inter_info_t *at_para)
{
    wstp_avrcp_tg_set_loc_inf(at_para->payload, at_para->payloadLength);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_avrcp_ct_connect(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    errno_t ret = EOK;
    if (at_para->reserved == true) {
        ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_9], sizeof(bd_addr.addr));
    } else {
        ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    }
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    wstp_avrcp_ct_conn_and_reg(&bd_addr);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_avrcp_ct_disconnect(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    ret = wstp_avrcp_ct_disconnect(&bd_addr);
    return (uint32_t)ret;
}

/* 处理参数为001122334455这类地址，转成数组param */
static void parse_str_to_hex(char** str_cmd, uint8_t* param, uint32_t count)
{
    char temp[ONE_HEX_LEN + DELIM_LEN] = {0};
    char* p;
    int ret;
    p = *str_cmd;

    for (uint32_t i = 0; i < count; i++) {
        ret = memcpy_s(temp, ONE_HEX_LEN + DELIM_LEN, p, ONE_HEX_LEN);
        if (ret != EOK) {
            wstp_print("[AT][ParseATCmdAddrParam]memcpy= error!\r\n");
            return;
        }
        temp[ONE_HEX_LEN] = '\0';
        param[i] = strtol(temp, NULL, BASE_HEX_VAL);
        p = p + ONE_HEX_LEN;
    }
    *str_cmd = p + DELIM_LEN; /* 自动偏移到分割符后面 */
}

uint32_t btsrv_wstp_at_avrcp_ct_press_button(const bt_manager_commu_inter_info_t *at_para)
{
    errno_t ret = 0;
    char *tmp_para = NULL;
    int32_t key_operation = 0;
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    if (bd_addr == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }

    if (at_para->reserved == true) {
        tmp_para = (char *)(at_para->payload + POS_9);
    } else {
        tmp_para = (char *)(at_para->payload);
    }

    parse_str_to_hex(&tmp_para, (uint8_t *)bd_addr, BT_ADDR_HEX_LEN);
    parse_str_to_hex(&tmp_para, (uint8_t *)&key_operation, 1);
    wstp_avrcp_ct_press_button(bd_addr, key_operation);
    BSP_MEM_FREE((void *)bd_addr);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_bt_avrcp_ct_set_absolute_volume(const bt_manager_commu_inter_info_t *at_para)
{
    errno_t ret = 0;
    char *tmp_para = NULL;
    uint8_t volume = 0;
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    if (bd_addr == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }

    tmp_para = (char *)(&at_para->payload);
    parse_str_to_hex(&tmp_para, (uint8_t *)bd_addr, BT_ADDR_HEX_LEN);
    parse_str_to_hex(&tmp_para, (uint8_t *)&volume, 1);
    ret = wstp_bts_avrcp_ct_set_abs_volume(bd_addr, volume);
    BSP_MEM_FREE((void *)bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_avrcp_ct_get_tg_player_stat(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    if (bd_addr == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    ret = wstp_avrcp_ct_get_tg_player_stat(bd_addr);
    BSP_MEM_FREE((void *)bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_avrcp_ct_get_element_attri(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    if (bd_addr == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    wstp_avrcp_ct_get_element_attributes(bd_addr);
    BSP_MEM_FREE((void *)bd_addr);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_wstp_avrcp_ct_get_support_event(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    if (bd_addr == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    ret = wstp_avrcp_ct_get_support_event(bd_addr);
    BSP_MEM_FREE((void *)bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_avrcp_ct_reg_support_event(const bt_manager_commu_inter_info_t *at_para)
{
    errno_t ret = 0;
    char *tmp_para = NULL;
    uint8_t event_id = 0;
    uint32_t interval = 0;
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    if (bd_addr == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }

    tmp_para = (char *)(&at_para->payload);
    parse_str_to_hex(&tmp_para, (uint8_t *)bd_addr, BT_ADDR_HEX_LEN);
    parse_str_to_hex(&tmp_para, (uint8_t *)&event_id, 1);
    parse_str_to_hex(&tmp_para, (uint8_t *)&interval, 1);
    wstp_avrcp_ct_reg_support_event(bd_addr, event_id, interval);
    BSP_MEM_FREE((void *)bd_addr);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_avrcp_get_conn_stat(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    if (bd_addr == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    ret = wstp_avrcp_ct_get_conn_stat(bd_addr);
    BSP_MEM_FREE((void *)bd_addr);
    return (uint32_t)ret;
}

/* A2DP */
uint32_t btsrv_wstp_at_a2dp_src_reg(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    int ret = wstp_a2dp_src_register_callbacks();
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_a2dp_src_dereg(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    int ret = wstp_a2dp_src_deregister_callbacks();
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_a2dp_src_get_device_connect_state(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    ret = wstp_a2dp_src_get_device_connect_state(&bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_a2dp_src_get_playing_state(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    ret = wstp_a2dp_src_get_playing_state(&bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_a2dp_src_connect(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    ret = wstp_a2dp_src_connect(&bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_a2dp_src_disconnect(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    ret = wstp_a2dp_src_disconnect(&bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_a2dp_src_get_active_device(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    wstp_a2dp_src_get_active_device();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_a2dp_src_start_playing(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    (void)a2dp_src_start_playing(&bd_addr);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_a2dp_src_suspend_playing(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bth_bd_addr), &at_para->payload[POS_0], sizeof(bth_bd_addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    (void)a2dp_src_suspend_playing(&bd_addr);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_a2dp_src_stop_playing(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    (void)a2dp_src_stop_playing(&bd_addr);
    return ERRCODE_SUCC;
}

/* A2DP SNK */
uint32_t btsrv_wstp_at_a2dp_snk_reg(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    int ret =  wstp_a2dp_snk_register_callbacks();
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_a2dp_snk_dereg(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    int ret =  wstp_a2dp_snk_deregister_callbacks();
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_a2dp_snk_get_device_connect_state(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    ret = wstp_a2dp_snk_get_device_connect_state(&bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_a2dp_snk_get_playing_state(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    ret = wstp_a2dp_snk_get_playing_state(&bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_a2dp_snk_connect(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    ret = wstp_a2dp_snk_connect(&bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_a2dp_snk_disconnect(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    ret = wstp_a2dp_snk_disconnect(&bd_addr);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_a2dp_snk_get_active_device(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    wstp_a2dp_snk_get_active_device();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_a2dp_snk_start_playing(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    (void)a2dp_snk_start_playing(&bd_addr);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_a2dp_snk_suspend_playing(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bth_bd_addr), &at_para->payload[POS_0], sizeof(bth_bd_addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    (void)a2dp_snk_suspend_playing(&bd_addr);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_a2dp_snk_stop_playing(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    (void)a2dp_snk_stop_playing(&bd_addr);
    return ERRCODE_SUCC;
}
#endif

#ifdef SUPPORT_BLE
uint32_t btsrv_wstp_at_ble_gatt_create_server(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    wstp_print("btsrv_wstp_at_ble_gatt_create_server entered\r\n");
    (void)mc_ble_gatt_create_server_req_atb();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_gatt_cmd_cbk_reg(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    wstp_print("btsrv_wstp_at_gatt_cmd_cbk_reg entered\r\n");
    //mc_ble_gatt_cmd_cbk_reg();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_start_adv(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_start_adv\r\n");
    bool res = ble_conn_gap_start_adv();
    if (res == false) {
        wstp_print("btsrv_wstp_at_ble_start_adv, res\r\n");
    } else {
        wstp_print("btsrv_wstp_at_ble_start_adv, OK\r\n");
    }
    return (uint32_t)res;
}

uint32_t btsrv_wstp_at_ble_stop_adv(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_stop_adv\r\n");
    bool res = ble_conn_gap_stop_adv();
    if (res == false) {
        wstp_print("btsrv_wstp_at_ble_stop_adv, res\r\n");
    } else {
        wstp_print("btsrv_wstp_at_ble_stop_adv, OK\r\n");
    }
    return (uint32_t)res;
}

uint32_t btsrv_wstp_at_ble_enter_scan(const bt_manager_commu_inter_info_t *at_para)
{
    wstp_print("btsrv_wstp_at_ble_enter_scan\r\n");
    (void)bt_at_cmd_ble_enter_scan(at_para->payloadLength,(char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_create_service(const bt_manager_commu_inter_info_t *at_para)
{
    wstp_print("btsrv_wstp_at_ble_gatt_create_service entered\r\n");
    (void)bts_at_cmd_gatt_create_service(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_add_include_service(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_add_include_service entered\r\n");
    (void)bts_at_cmd_gatt_add_include_service(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_add_characteristic(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_add_characteristic entered\r\n");
    (void)bts_at_cmd_gatt_add_characteristic(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_add_descriptor(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_add_descriptor entered\r\n");
    (void)bts_at_cmd_gatt_add_descriptor(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_start_service(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_start_service entered\r\n");
    (void)bts_at_cmd_gatt_start_service(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_send_response(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_send_response entered\r\n");
    (void)bts_at_cmd_gatt_send_response(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_update_characteristic_value(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_update_characteristic_value entered\r\n");
    (void)bts_at_cmd_gatt_notify_characteristic_changed(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_update_descriptor_value(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_update_descriptor_value entered\r\n");
    (void)bts_at_cmd_gatt_update_descriptor_value_changed(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_get_gap_attribute(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_get_gap_attribute entered\r\n");
    (void)bts_at_cmd_gatt_get_gap_ui_attributes(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_set_gap_attribute(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_set_gap_attribute entered\r\n");
    (void)bts_at_cmd_gatt_set_gap_ui_attributes(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_multiple_value_ntf(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_multiple_value_ntf entered\r\n");
    (void)bt_at_cmd_multiple_value_nfy(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_mtu_set_param(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_mtu_set_param entered\r\n");
    (void)bt_at_cmd_set_le_att_mtu_param(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_discovery_service(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_discovery_service entered\r\n");
    (void)bts_at_cmd_gatt_discovery_service(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_discovery_include(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_discovery_include entered\r\n");
    (void)bts_at_cmd_gatt_find_include_service(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_discovery_character(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_discovery_character entered\r\n");
    (void)bts_at_cmd_gatt_discovery_characteristics(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_discovery_character_by_uuid(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_discovery_character_by_uuid entered\r\n");
    (void)bts_at_cmd_gatt_discovery_characteristics_by_uuid(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_discovery_descriptor(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_discovery_descriptor entered\r\n");
    (void)bts_at_cmd_gatt_read_characteristics_by_uuid(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_discovery_descriptor_by_handle(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_discovery_descriptor_by_handle entered\r\n");
    (void)bts_at_cmd_gatt_discovery_descriptor_by_handle(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_read_character(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_read_character entered\r\n");
    (void)bt_at_cmd_read_character(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_read_character_by_uuid(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_read_character_by_uuid entered\r\n");
    (void)bts_at_cmd_gatt_read_characteristics_by_uuid(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_read_multiple_character(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_read_multiple_character entered\r\n");
    (void)bt_at_cmd_read_multiple_character(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_multiple_value_read_character(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_multiple_value_read_character entered\r\n");
    (void)bt_at_cmd_multiple_value_read_character(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_write_character(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_write_character entered\r\n");
    (void)bts_at_cmd_gatt_write_characteristic_value(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_reliable_write_character(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_reliable_write_character entered\r\n");
    (void)bts_at_cmd_gatt_reliable_write_characteristic_value(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_read_descriptor(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_read_descriptor entered\r\n");
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_write_descriptor(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_write_descriptor entered\r\n");
    (void)bts_at_cmd_gatt_write_descriptor_value(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_mtu_config(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_mtu_config entered\r\n");
    (void)bts_at_cmd_gatt_mtu_config(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_value_notification(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_value_notification entered\r\n");
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_get_hid_device_report(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_get_hid_device_report entered\r\n");
    (void)bts_at_cmd_gatt_get_report_map(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_set_attribute_min_key_size(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_set_attribute_min_key_size entered\r\n");
    (void)bts_at_cmd_set_att_key_size(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_ble_gatt_set_attribute_config_param(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    wstp_print("btsrv_wstp_at_ble_gatt_set_attribute_config_param entered\r\n");
    (void)bts_at_cmd_set_att_key_size(at_para->payloadLength, (char **)at_para->payload);
    return ERRCODE_SUCC;
}
#endif

uint32_t btsrv_wstp_at_bt_service_test_cmd(const bt_manager_commu_inter_info_t *at_para)
{
    unsigned char argv[BT_MAX_CMD_STR_LEN] = {0};
    int ret = memcpy_s(argv, BT_MAX_CMD_STR_LEN, &at_para->payload[POS_0], at_para->payloadLength);
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE((void *)at_para);
        return ERRCODE_FAIL;
    }
    bt_service_sample_cmd(at_para->payloadLength, (char **)argv);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_show_conn_list(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    btsrv_dfx_show_conn_list();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_show_ble_conn_list(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    btsrv_dfx_show_ble_conn_list();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_show_remote_dev_list(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    btsrv_dfx_show_remote_dev_list();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_get_paired_devices_num(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    wstp_get_paired_devices_num();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_get_paired_devices_list(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    wstp_get_paired_devices_list();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_get_pair_state(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr bd_addr = {0};
    int ret = memcpy_s(bd_addr.addr, sizeof(bd_addr.addr), &at_para->payload[POS_0], sizeof(bd_addr.addr));
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        return ERRCODE_FAIL;
    }
    wstp_get_pair_state(&bd_addr);
    return ERRCODE_SUCC;
}

#ifdef SUPPORT_BREDR
uint32_t btsrv_wstp_at_pbap_conn(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    wstp_print("addr=%02x%02x%02x%02x%02x%02x\n",
        at_para->payload[POS_0],
        at_para->payload[POS_1],
        at_para->payload[POS_2],
        at_para->payload[POS_3],
        at_para->payload[POS_4],
        at_para->payload[POS_5]);
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy error!\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    uint32_t res = pbap_pce_connect(bd_addr);
    wstp_print("pbap connect res=0x%x\n", res);
    BSP_MEM_FREE((void *)bd_addr);
    return (uint32_t)res;
}

uint32_t btsrv_wstp_at_pbap_disconn(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    wstp_print("addr=%02x%02x%02x%02x%02x%02x\n",
        at_para->payload[POS_0],
        at_para->payload[POS_1],
        at_para->payload[POS_2],
        at_para->payload[POS_3],
        at_para->payload[POS_4],
        at_para->payload[POS_5]);
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy error!\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    uint32_t res = pbap_pce_disconnect(bd_addr);
    wstp_print("pbap disconnect res=0x%x\n", res);
    BSP_MEM_FREE((void *)bd_addr);
    return (uint32_t)res;
}

uint32_t btsrv_wstp_at_pbap_pull_phonebook(const bt_manager_commu_inter_info_t *at_para)
{
    uint8_t *folder = NULL;
    uint8_t *vcf = NULL;
    uint16_t *max_list_count = NULL;
    uint16_t *list_offset = NULL;
    uint8_t *format = NULL;
    char *delim = ",";
    uint16_t *tmp_para = (uint16_t *)BSP_MEM_ALLOC(at_para->payloadLength * sizeof(uint16_t));
    int ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);

    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE((void *)tmp_para);
        return ERRCODE_FAIL;
    }

    folder = (uint8_t *)strtok((char*)tmp_para, delim);
    vcf = (uint8_t *)strtok(NULL, delim);
    max_list_count = (uint16_t *)strtok(NULL, delim);
    list_offset = (uint16_t *)strtok(NULL, delim);
    format = (uint8_t *)strtok(NULL, delim);

    if ((folder == NULL) || (vcf == NULL) || (max_list_count == NULL) || (list_offset == NULL) || (format == NULL)) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE((void *)tmp_para);
        return ERRCODE_FAIL;
    }

    *folder = (uint8_t)atoi((char*)folder);
    *vcf = (uint8_t)atoi((char*)vcf);
    *max_list_count = (uint16_t)atoi((char*)max_list_count);
    *list_offset = (uint16_t)atoi((char*)list_offset);
    *format = (uint8_t)atoi((char*)format);
    wstp_print("folder=%d vcf=%d max_list_count=%d list_offset=%d format=%d\n",
        *folder,
        *vcf,
        *max_list_count,
        *list_offset,
        *format);
    wstp_pbap_cmd_pull_phonebook(*folder, *vcf, *max_list_count, *list_offset, *format);
    BSP_MEM_FREE((void *)tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_pbap_pull_vcard_listing(const bt_manager_commu_inter_info_t *at_para)
{
    char **params[POS_7] = {NULL};
    int32_t params_cnt = 0;
    char *p = NULL;
    char *delim = ",";
    uint16_t *tmp_para = (uint16_t *)BSP_MEM_ALLOC(at_para->payloadLength * sizeof(uint16_t));
    int ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE((void *)tmp_para);
        return ERRCODE_FAIL;
    }
    p = strtok((char*)tmp_para, delim);
    for (int i = 0; i < POS_7 && p != NULL; i++) {
        params_cnt++;
        params[i] = p;
        p = strtok((char*)NULL, delim);
    }
    wstp_pbap_cmd_pull_vcard_listing(params, params_cnt);
    BSP_MEM_FREE((void *)tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_pbap_pull_vcard_entry(const bt_manager_commu_inter_info_t *at_para)
{
    uint8_t *folder = NULL;
    uint8_t *vcf = NULL;
    char *vcf_name = NULL;
    uint8_t *format = NULL;
    char *delim = ",";
    uint16_t *tmp_para = (uint16_t *)BSP_MEM_ALLOC(at_para->payloadLength * sizeof(uint16_t));
    int ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);

    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE((void *)tmp_para);
        return ERRCODE_FAIL;
    }
    folder = (uint8_t *)strtok((char*)tmp_para, delim);
    vcf = (uint8_t *)strtok(NULL, delim);
    vcf_name = strtok(NULL, delim);
    format = (uint8_t *)strtok(NULL, delim);

    if ((folder == NULL) || (vcf == NULL) || (format == NULL)) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE((void *)tmp_para);
        return ERRCODE_FAIL;
    }

    *folder = (uint8_t)atoi((char*)folder);
    *vcf = (uint8_t)atoi((char*)vcf);
    *format = (uint8_t)atoi((char*)format);
    wstp_print("folder=%d vcf=%d vcf_name=%s format=%d\n", *folder, *vcf, vcf_name, *format);
    wstp_pbap_cmd_pull_vcard_entry(*folder, *vcf, vcf_name, *format);
    BSP_MEM_FREE((void *)tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_pbap_stop_trans(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    uint32_t ret = pbap_pce_stop_trans(NULL);
    wstp_print("pbap stop stans result=0x%x", ret);
    return ret;
}

uint32_t btsrv_wstp_at_pbap_get_conn_state(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    uint32_t ret = pbap_pce_get_device_connect_state(NULL);
    wstp_print("connection state=0x%x\n", ret);
    return ret;
}

uint32_t btsrv_wstp_at_hfp_hf_conn(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    wstp_print("addr=%02x%02x%02x%02x%02x%02x\n",
        at_para->payload[POS_0],
        at_para->payload[POS_1],
        at_para->payload[POS_2],
        at_para->payload[POS_3],
        at_para->payload[POS_4],
        at_para->payload[POS_5]);
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy error!\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    uint32_t res = hfp_hf_connect(bd_addr);
    wstp_print("hfp connect res=0x%x\n", res);
    BSP_MEM_FREE((void *)bd_addr);
    return res;
}

uint32_t btsrv_wstp_at_hfp_hf_disconn(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    wstp_print("addr=%02x%02x%02x%02x%02x%02x\n",
        at_para->payload[POS_0],
        at_para->payload[POS_1],
        at_para->payload[POS_2],
        at_para->payload[POS_3],
        at_para->payload[POS_4],
        at_para->payload[POS_5]);
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy error!\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    uint32_t res = hfp_hf_disconnect(bd_addr);
    wstp_print("hfp connect res=0x%x\n", res);
    BSP_MEM_FREE((void *)bd_addr);
    return res;
}

uint32_t btsrv_wstp_at_hfp_hf_conn_sco(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    wstp_print("addr=%02x%02x%02x%02x%02x%02x\n",
        at_para->payload[POS_0],
        at_para->payload[POS_1],
        at_para->payload[POS_2],
        at_para->payload[POS_3],
        at_para->payload[POS_4],
        at_para->payload[POS_5]);
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy error!\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    uint32_t res = hfp_hf_connect_sco(bd_addr);
    wstp_print("connect SCO result=0x%x", res);
    BSP_MEM_FREE((void *)bd_addr);
    return res;
}

uint32_t btsrv_wstp_at_hfp_hf_disconn_sco(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    wstp_print("addr=%02x%02x%02x%02x%02x%02x\n",
        at_para->payload[POS_0],
        at_para->payload[POS_1],
        at_para->payload[POS_2],
        at_para->payload[POS_3],
        at_para->payload[POS_4],
        at_para->payload[POS_5]);
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy error!\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    uint32_t res = hfp_hf_disconnect_sco(bd_addr);
    wstp_print("disconnect SCO result=0x%x", res);
    BSP_MEM_FREE((void *)bd_addr);
    return res;
}

uint32_t btsrv_wstp_at_hfp_get_dev_conn_state(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    wstp_print("addr=%02x%02x%02x%02x%02x%02x\n",
        at_para->payload[POS_0],
        at_para->payload[POS_1],
        at_para->payload[POS_2],
        at_para->payload[POS_3],
        at_para->payload[POS_4],
        at_para->payload[POS_5]);
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy error!\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    uint32_t res = hfp_hf_get_device_connect_state(bd_addr);
    wstp_print("connection state=0x%x\n", res);
    BSP_MEM_FREE((void *)bd_addr);
    return (uint32_t)res;
}

uint32_t btsrv_wstp_at_hfp_get_sco_conn_state(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    wstp_print("addr=%02x%02x%02x%02x%02x%02x\n",
        at_para->payload[POS_0],
        at_para->payload[POS_1],
        at_para->payload[POS_2],
        at_para->payload[POS_3],
        at_para->payload[POS_4],
        at_para->payload[POS_5]);
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy error!\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    uint32_t res = hfp_hf_get_sco_connect_state(bd_addr);
    wstp_print("sco connect state=0x%x\n", res);
    BSP_MEM_FREE((void *)bd_addr);
    return res;
}

uint32_t btsrv_wstp_at_avrcp_ct_get_support_companies(const bt_manager_commu_inter_info_t *at_para)
{
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    if (bd_addr == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }
    int ret = memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), &at_para->payload[POS_0], sizeof(bd_addr->addr));
    if (ret != EOK) {
        wstp_print("memcpy error!\r\n");
        BSP_MEM_FREE((void *)bd_addr);
        return ERRCODE_FAIL;
    }
    ret = wstp_avrcp_ct_get_support_companies(bd_addr);
    wstp_print("avrct ct get support companies ret=0x%x\n", ret);
    BSP_MEM_FREE((void *)bd_addr);
    return ret;
}

uint32_t btsrv_wstp_at_spp_server_create(const bt_manager_commu_inter_info_t *at_para)
{
    char *uuid = NULL;
    char *name = NULL;
    char *delim = ",";
    uint8_t name_len = 0;
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);

    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }

    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);

    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    uuid = strtok((char*)tmp_para, delim);
    name = strtok(NULL, delim);
    name_len = strlen(name) + 1;

    wstp_print("create spp server uuid=%s name=%s name_len=%d\n", uuid, name, name_len);
    wstp_spp_cmd_server_create(uuid, name, name_len);
    BSP_MEM_FREE(tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_spp_server_accept(const bt_manager_commu_inter_info_t *at_para)
{
    uint16_t *server_id = NULL;
    char *delim = "\0";
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);

    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }

    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);

    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    server_id = (uint16_t *)strtok((char*)tmp_para, delim);

    if (server_id == NULL) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    *server_id = (uint16_t)atoi((char*)server_id);
    wstp_print("spp server accept server_id=%d\n", *server_id);
    wstp_spp_cmd_server_accept(*server_id);
    BSP_MEM_FREE(tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_spp_server_close(const bt_manager_commu_inter_info_t *at_para)
{
    uint16_t *server_id = NULL;
    char *delim = "\0";
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);

    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }

    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);

    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    server_id = (uint16_t *)strtok((char*)tmp_para, delim);

    if (server_id == NULL) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    *server_id = (uint16_t)atoi((char*)server_id);
    wstp_print("spp server close server_id=%d\n", *server_id);
    wstp_spp_cmd_server_close(*server_id);
    BSP_MEM_FREE(tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_spp_conn(const bt_manager_commu_inter_info_t *at_para)
{
    uint16_t *server_id = NULL;
    uint32_t *uuid = NULL;
    char *bd_addr = NULL;
    char *delim = ",";
    uint8_t addr_len = 0;
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);

    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }

    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);

    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    server_id = (uint16_t *)strtok((char*)tmp_para, delim);
    uuid = (uint32_t *)strtok(NULL, delim);
    bd_addr = strtok(NULL, delim);
    addr_len = strlen(bd_addr);

    if (server_id == NULL) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    *server_id = (uint16_t)atoi((char*)server_id);
    wstp_print("spp connect server_id=%d uuid=%s bd_addr=%s addr_len=%d\n", *server_id, uuid, bd_addr, addr_len);
    wstp_spp_cmd_connect((char *)uuid, bd_addr);
    BSP_MEM_FREE(tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_spp_disconn(const bt_manager_commu_inter_info_t *at_para)
{
    if (at_para->reserved == true) {
        wstp_print("[apk]spp disconnect client_id=%d\n", at_para->payload[POS_9]);
        wstp_spp_cmd_disconnect(at_para->payload[POS_9]);
        return ERRCODE_FAIL;
    }
    uint16_t *client_id= NULL;
    char *delim = "\0";
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);

    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }

    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);

    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    client_id = (uint16_t *)strtok((char*)tmp_para, delim);

    if (client_id == NULL) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    *client_id = (uint16_t)atoi((char*)client_id);
    wstp_print("spp disconnect client_id=%d\n", *client_id);
    wstp_spp_cmd_disconnect(*client_id);
    BSP_MEM_FREE(tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_spp_is_conn(const bt_manager_commu_inter_info_t *at_para)
{
    uint16_t *client_id= NULL;
    char *delim = "\0";
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);

    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }

    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);

    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    client_id = (uint16_t *)strtok((char*)tmp_para, delim);

    if (client_id == NULL) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    *client_id = (uint16_t)atoi((char*)client_id);
    wstp_print("spp is connect? client_id=%d\n", *client_id);
    wstp_spp_cmd_is_spp_connect(*client_id);
    BSP_MEM_FREE(tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_spp_get_rmt_addr(const bt_manager_commu_inter_info_t *at_para)
{
    uint16_t *client_id= NULL;
    char *delim = "\0";
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);

    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }

    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);

    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    client_id = (uint16_t *)strtok((char*)tmp_para, delim);

    if (client_id == NULL) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    *client_id = (uint16_t)atoi((char*)client_id);
    wstp_print("spp get rmt addr client_id=%d\n", *client_id);
    wstp_spp_cmd_get_remote_addr(*client_id);
    BSP_MEM_FREE(tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_spp_read_data(const bt_manager_commu_inter_info_t *at_para)
{
    uint16_t *client_id = NULL;
    uint32_t *buf_len = NULL;
    uint32_t *duration = NULL;
    char *delim = ",";
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);

    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }

    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);

    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    client_id = (uint16_t *)strtok((char*)tmp_para, delim);
    buf_len = (uint32_t *)strtok(NULL, delim);
    duration = (uint32_t *)strtok(NULL, delim);

    if ((client_id == NULL) || (buf_len == NULL) || (duration == NULL)) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }

    *client_id = (uint16_t)atoi((char*)client_id);
    *buf_len = (uint32_t)atoi((char*)buf_len);
    *duration = (uint32_t)atoi((char*)duration);

    wstp_print("spp read client_id=%d buf_len=%d duration=%d\n", *client_id, *buf_len, *duration);
    BSP_MEM_FREE(tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_spp_write_data(const bt_manager_commu_inter_info_t *at_para)
{
    uint16_t *client_id_s = NULL;
    uint32_t *repeat_times_s = NULL;
    uint32_t *cycle_num_s = NULL;
    uint32_t *delay_time_s = NULL;
    char *delim = ",";
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);
    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }
    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }
    client_id_s = (uint16_t *)strtok((char*)tmp_para, delim);
    repeat_times_s = (uint32_t *)strtok(NULL, delim);
    cycle_num_s = (uint32_t *)strtok(NULL, delim);
    delay_time_s = (uint32_t *)strtok(NULL, delim);
    if ((client_id_s == NULL) || (repeat_times_s == NULL) || (cycle_num_s == NULL) || (delay_time_s == NULL)) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }
    uint16_t client_id = (uint16_t)atoi((char*)client_id_s);
    uint32_t repeat_times = (uint32_t)atoi((char*)repeat_times_s);
    uint32_t cycle_num = (uint32_t)atoi((char*)cycle_num_s);
    uint32_t delay_time = (uint32_t)atoi((char*)delay_time_s);
    wstp_print("spp write client_id=%d repeat_times=%d cycle_num=%d delay_time=%d\r\n",
        client_id,
        repeat_times,
        cycle_num,
        delay_time);
    wstp_spp_cmd_write(client_id, repeat_times, cycle_num, delay_time);
    BSP_MEM_FREE(tmp_para);
    return ERRCODE_SUCC;
}
#endif
uint32_t btsrv_wstp_at_dft_reg(const bt_manager_commu_inter_info_t *at_para)
{
    (void)at_para;
    bt_dft_reg_callbacks();
    wstp_print("bt dft command registered\n");
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_bt_set_tx_power(const bt_manager_commu_inter_info_t *at_para)
{
    char *delim = ",";
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);
    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\r\n");
        return ERRCODE_FAIL;
    }
    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE((void *)tmp_para);
        return ERRCODE_FAIL;
    }
    uint8_t *addr = (uint8_t *)strtok((char*)tmp_para, delim);
    uint8_t *pwr_level = (uint8_t *)strtok(NULL, delim);
    if ((addr == NULL) || (pwr_level == NULL)) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE((void *)tmp_para);
        return ERRCODE_FAIL;
    }
    *pwr_level = (uint8_t)atoi((char*)pwr_level);
    bth_bd_addr bd_addr = {0};
    uint8_t temp[BT_ONE_ADDR_LEN + 1] = {0};
    uint8_t* p = addr;
    for (uint32_t i = 0; i < BT_MAC_LEN; i++) {
        ret = memcpy_s(temp, BT_ONE_ADDR_LEN + 1, p, BT_ONE_ADDR_LEN);
        if (ret != EOK) {
            wstp_print("memcpy= error!\r\n");
            BSP_MEM_FREE((void *)tmp_para);
            return ERRCODE_FAIL;
        }
        temp[BT_ONE_ADDR_LEN] = '\0';
        bd_addr.addr[i] = strtol((char*)temp, NULL, 16);
        p = p + BT_ONE_ADDR_LEN;
    }
    wstp_print("addr=%02x%02x%02x%02x%02x%02x \n",
        bd_addr.addr[POS_0],
        bd_addr.addr[POS_1],
        bd_addr.addr[POS_2],
        bd_addr.addr[POS_3],
        bd_addr.addr[POS_4],
        bd_addr.addr[POS_5]);
    ret = bt_set_tx_power(&bd_addr, *pwr_level);
    wstp_print("set bt tx power %d ret=0x%x\n", *pwr_level, ret);
    BSP_MEM_FREE((void *)tmp_para);
    return (uint32_t)ret;
}

uint32_t btsrv_wstp_at_bt_set_dut_mode(const bt_manager_commu_inter_info_t *at_para)
{
    uint8_t *mode = NULL;
    char *delim = "\0";
    char *strtok_state = NULL;
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);
    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }
    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }
    mode = (uint8_t *)strtok_r((char*)tmp_para, delim, &strtok_state);
    if (mode == NULL) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }
    *mode = (uint8_t)atoi((char*)mode);
    wstp_print("set dft dut mode=%d\n", *mode);
    bt_wstp_dft_set_dut_mode(*mode);
    BSP_MEM_FREE(tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_bt_set_sig(const bt_manager_commu_inter_info_t *at_para)
{
    uint8_t *freq_s = NULL;
    uint8_t *mode_s = NULL;
    uint8_t *switch_trx_s = NULL;
    char *delim = ",";
    char *strtok_state;
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);
    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }
    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }
    freq_s = (uint8_t *)strtok_r((char*)tmp_para, delim, &strtok_state);
    mode_s = (uint8_t *)strtok_r(NULL, delim, &strtok_state);
    switch_trx_s = (uint8_t *)strtok_r(NULL, delim, &strtok_state);
    if ((freq_s == NULL) || (mode_s == NULL) || (switch_trx_s == NULL)) {
        wstp_print("input err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }
    uint8_t freq = (uint8_t)atoi((char*)freq_s);
    uint8_t mode = (uint8_t)atoi((char*)mode_s);
    uint8_t switch_trx = (uint8_t)atoi((char*)switch_trx_s);
    wstp_print("sig cmd freq=%d mode=%d switch_trx=%d\r\n", freq, mode, switch_trx);
    bt_wstp_dft_set_sig_cmd(freq, mode, switch_trx);
    BSP_MEM_FREE(tmp_para);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_bt_dft_set_power_all(const bt_manager_commu_inter_info_t *at_para)
{
    uint16_t ocf = 0xFC55;
    uint8_t param[sizeof(uint16_t)] = {0};
    uint8_t *data = NULL;
    char *delim = ",";
    char *strtok_state;
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t *tmp_para = (uint8_t *)BSP_MEM_ALLOC(at_para->payloadLength);
    if (tmp_para == NULL) {
        wstp_print("memory alloc failed\n");
        return ERRCODE_FAIL;
    }
    errno_t ret = memcpy_s(tmp_para, at_para->payloadLength, &at_para->payload[POS_0], at_para->payloadLength);
    if (ret != EOK) {
        wstp_print("memcpy err\r\n");
        BSP_MEM_FREE(tmp_para);
        return ERRCODE_FAIL;
    }
    for (uint8_t i = 0; i < sizeof(uint16_t); i++) {
        if (i == 0) {
            data = (uint8_t *)strtok_r((char*)tmp_para, delim, &strtok_state);
        } else {
            data = (uint8_t *)strtok_r(NULL, delim, &strtok_state);
        }
        if (data == NULL) {
            wstp_print("input err\r\n");
            BSP_MEM_FREE(tmp_para);
            return ERRCODE_FAIL;
        }
        param[i] = (uint8_t)atoi((char*)data);
        wstp_print("param[%d]=%d\n", i, param[i]);
    }
    bt_dft_set_vendor_cmd(ocf, param, sizeof(uint16_t));
    BSP_MEM_FREE(tmp_para);
    return ERRCODE_SUCC;
}

void btsrv_wstp_at_bt_dft_rx_count_cbk(uint16_t rx_count)
{
    g_bt_dft_rx_count = rx_count;
#if defined(FT_BT_UART) || defined(FT_DONGLE)
    factory_test_print(FT_RETURN_SUCC,  "receive rx packet number is [%d].\r\n", g_bt_dft_rx_count);
#else
    wstp_print("receive rx packet number is [%d].\r\n", g_bt_dft_rx_count);
#endif
}

void btsrv_wstp_at_bt_dft_set_dut_mode(const uint8_t *param)
{
    bt_dft_set_dut_mode(param);
}

void btsrv_wstp_at_bt_dft_set_bredr_tx_mode(const uint8_t *param)
{
    bt_dft_set_bredr_tx_mode(param);
}

void btsrv_wstp_at_bt_dft_set_bredr_rx_mode(const uint8_t *param)
{
    bt_dft_rx_count_cbk_reg((void *)btsrv_wstp_at_bt_dft_rx_count_cbk);
    bt_dft_set_bredr_rx_mode(param);
}

void btsrv_wstp_at_bt_dft_set_ble_tx_mode(const uint8_t *param)
{
    bt_dft_set_ble_tx_mode(param);
}

void btsrv_wstp_at_bt_dft_set_ble_rx_mode(const uint8_t *param)
{
    bt_dft_rx_count_cbk_reg((void *)btsrv_wstp_at_bt_dft_rx_count_cbk);
    bt_dft_set_ble_rx_mode(param);
}

#ifdef FT_DONGLE
void btsrv_wstp_at_bt_set_ct_delay_ms(const bt_manager_commu_inter_info_t *at_para)
{
    char *ptr = (char *)at_para->payload;
    uint32_t delay_time = strtoul(ptr, NULL, 0);
    if (delay_time == 0) {
        delay_time = CT_TEST_DELAY_DEFAULT;
    }
    g_coupling_test_delay_ms = delay_time;
    factory_test_print(FT_RETURN_SUCC,  "coupling test set delay_time = %d.\r\n", delay_time);
}

void btsrv_wstp_at_bt_set_dongle_addr(const bt_manager_commu_inter_info_t *at_para)
{
    if (at_para->payloadLength < BT_ADDR_LEN) {
        wstp_print("addr err.\r\n");
    }
    int ret = memcpy_s(g_dongle_addr, BT_ADDR_LEN, at_para->payload, sizeof(g_dongle_addr) - 1);
    if (ret != EOK) {
        factory_test_print(FT_RETURN_FAIL, "dongle_addr memcpy err.\r\n");
    } else {
        g_dongle_addr[BT_ADDR_LEN] = '\0';
        factory_test_print(FT_RETURN_SUCC, "dongle_addr = %s.\r\n", g_dongle_addr);
    }
}

static void dongle_gap_acl_state_changed_callback(const bth_bd_addr *bd_addr, uint8_t state, unsigned int reason)
{
    char uuid[] = {"1101"};
    if (state == GAP_ACL_STATE_CONNECTED) {
        gap_register_callbacks(NULL);
        /* 发起ssp连接 */
        wstp_spp_cmd_connect(uuid, (char*)g_dongle_addr);
    } else if (state == GAP_ACL_STATE_DISCONNECTED) {
        /* 发起重连接 */
        wstp_connect_remote_device(bd_addr);
    }
}

gap_call_backs_t dongle_spp_callback_register = {
    NULL,
    dongle_gap_acl_state_changed_callback,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

void btsrv_wstp_at_copuling_test_spp_continue(void)
{
    char uuid[] = {"1101"};
    wstp_print("current delay_time = %d.\r\n", g_coupling_test_delay_ms);
    wstp_print("current dongle_addr = %s.\r\n", g_dongle_addr);
    uint8_t *dongle_addr_hex = (uint8_t*)malloc(BT_ADDR_HEX_LEN);
    if (dongle_addr_hex == NULL) {
        wstp_print("malloc fail.\r\n");
        return;
    }
    decode_bd_addr_from_str(g_dongle_addr, dongle_addr_hex, BT_ADDR_HEX_LEN);
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    if (memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), dongle_addr_hex, sizeof(bd_addr->addr)) != EOK) {
        BSP_MEM_FREE(bd_addr);
        free(dongle_addr_hex);
        wstp_print("memcpy fail.\r\n");
        return;
    }
    (void)gap_register_callbacks(&dongle_spp_callback_register);
    wstp_connect_remote_device(bd_addr);
    g_dongle_client_id = 1; // 第一次被动连接client id 0被占用，断连后主动连接默认使用1
    bool is_connected = false;
    uint64_t start_time = uapi_tcxo_get_ms();
    uint64_t current_time;
    while (is_connected == false) {
        is_connected = is_spp_connected(g_dongle_client_id);
        (void)osDelay(CT_TEST_DELAY_DEFAULT);
        wstp_print("[dongle reconnecting.\r\n]");
        current_time = uapi_tcxo_get_ms();
        if (current_time - start_time > DONGLE_RECONN_TIMEOUT || is_connected == true) {
            wstp_print("[dongle connect %s.\r\n]", is_connected == true ? "Succ" : "Fail");
            break;
        }
    }
    free(dongle_addr_hex);
    BSP_MEM_FREE(bd_addr);
}

void btsrv_wstp_at_copuling_test_spp_disconnect(void)
{
    wstp_spp_cmd_disconnect(g_dongle_client_id);
    uint8_t *dongle_addr_hex = (uint8_t*)malloc(BT_ADDR_HEX_LEN);
    if (dongle_addr_hex == NULL) {
        wstp_print("malloc fail.\r\n");
        return;
    }
    decode_bd_addr_from_str(g_dongle_addr, dongle_addr_hex, BT_ADDR_HEX_LEN);
    bth_bd_addr *bd_addr = (bth_bd_addr*)BSP_MEM_ALLOC(sizeof(bth_bd_addr));
    if (memcpy_s(bd_addr->addr, sizeof(bd_addr->addr), dongle_addr_hex, sizeof(bd_addr->addr)) != EOK) {
        BSP_MEM_FREE(bd_addr);
        free(dongle_addr_hex);
        wstp_print("memcpy fail.\r\n");
        return;
    }
    gap_remove_pair(bd_addr);
    (void)osDelay(CT_TEST_DELAY_DEFAULT);
}

void btsrv_wstp_at_bt_coupling_test_bt_tx(const uint8_t *param)
{
    btsrv_wstp_at_copuling_test_spp_disconnect();
    bt_dft_set_bredr_tx_mode(param);
    (void)osDelay(g_coupling_test_delay_ms);
    uint8_t param_stop[] = {"00"};
    bt_dft_set_bredr_tx_mode(param_stop);
    btsrv_wstp_at_copuling_test_spp_continue();
}

void btsrv_wstp_at_bt_coupling_test_bt_rx(const uint8_t *param)
{
    btsrv_wstp_at_copuling_test_spp_disconnect();
    bt_dft_reset_rx_count(g_dongle_client_id);
    bt_dft_rx_count_cbk_reg((void *)btsrv_wstp_at_bt_dft_rx_count_cbk);
    bt_dft_set_bredr_rx_mode(param);
    (void)osDelay(g_coupling_test_delay_ms);
    uint8_t param_stop[] = {"00"};
    bt_dft_set_bredr_rx_mode(param_stop);
    btsrv_wstp_at_copuling_test_spp_continue();
}

void btsrv_wstp_at_bt_coupling_test_ble_tx(const uint8_t *param)
{
    btsrv_wstp_at_copuling_test_spp_disconnect();
    bt_dft_set_ble_tx_mode(param);
    (void)osDelay(g_coupling_test_delay_ms);
    uint8_t param_stop[] = {"00"};
    bt_dft_set_ble_tx_mode(param_stop);
    btsrv_wstp_at_copuling_test_spp_continue();
}

void btsrv_wstp_at_bt_coupling_test_ble_rx(const uint8_t *param)
{
    btsrv_wstp_at_copuling_test_spp_disconnect();
    bt_dft_reset_rx_count();
    bt_dft_rx_count_cbk_reg((void *)btsrv_wstp_at_bt_dft_rx_count_cbk);
    bt_dft_set_ble_rx_mode(param);
    (void)osDelay(g_coupling_test_delay_ms);
    uint8_t param_stop[] = {"00"};
    bt_dft_set_ble_rx_mode(param_stop);
    btsrv_wstp_at_copuling_test_spp_continue();
}
#endif

void btsrv_wstp_at_bt_dft_reset_rx_count(const uint8_t *param)
{
    bt_dft_reset_rx_count(param);
}

void btsrv_wstp_at_bt_dft_read_rx_count(const uint8_t *param)
{
    unused(param);
    factory_test_print(FT_RETURN_SUCC,  "receive rx packet number is [%d].\r\n", g_bt_dft_rx_count);
}

typedef void (*dft_func)(const uint8_t *param);

typedef struct {
    uint32_t at_id;
    dft_func func;
} btsrv_at_cmd_2_dft_t;

static const btsrv_at_cmd_2_dft_t g_btsrv_at_cmd_2_dft[] = {
    {BTSRV_WSTP_AT_BT_DFT_ENTER_DUT_MODE, btsrv_wstp_at_bt_dft_set_dut_mode},
    {BTSRV_WSTP_AT_BT_DFT_BREDR_TX_MODE, btsrv_wstp_at_bt_dft_set_bredr_tx_mode},
    {BTSRV_WSTP_AT_BT_DFT_BREDR_RX_MODE, btsrv_wstp_at_bt_dft_set_bredr_rx_mode},
    {BTSRV_WSTP_AT_BT_DFT_BLE_TX_MODE, btsrv_wstp_at_bt_dft_set_ble_tx_mode},
    {BTSRV_WSTP_AT_BT_DFT_BLE_RX_MODE, btsrv_wstp_at_bt_dft_set_ble_rx_mode},
    {BTSRV_WSTP_AT_BT_DFT_RESET_RX_COUNT, btsrv_wstp_at_bt_dft_reset_rx_count},
    {BTSRV_WSTP_AT_BT_DFT_READ_RX_COUNT, btsrv_wstp_at_bt_dft_read_rx_count},
#ifdef FT_DONGLE
    {BTSRV_WSTP_AT_BT_COUPLING_TEST_BT_TX, btsrv_wstp_at_bt_coupling_test_bt_tx},
    {BTSRV_WSTP_AT_BT_COUPLING_TEST_BT_RX, btsrv_wstp_at_bt_coupling_test_bt_rx},
    {BTSRV_WSTP_AT_BT_COUPLING_TEST_BLE_TX, btsrv_wstp_at_bt_coupling_test_ble_tx},
    {BTSRV_WSTP_AT_BT_COUPLING_TEST_BLE_RX, btsrv_wstp_at_bt_coupling_test_ble_rx},
#endif
};

uint32_t btsrv_wstp_at_bt_dft(const bt_manager_commu_inter_info_t *at_para)
{
    wstp_print("rcv at cmd payloadLength=%d\r\n", at_para->payloadLength);
    uint8_t len = 3;
    uint8_t base = 16;
    uint8_t str[] = {0, 0, '\0'};
    uint16_t param_size = at_para->payloadLength / len;
    uint8_t *param = (uint8_t *)BSP_MEM_ALLOC(param_size + POS_2);
    if (param == NULL) {
        wstp_print("malloc fail\n");
        return ERRCODE_FAIL;
    }
    for (uint8_t i = 0; i < param_size; i++) {
        errno_t ret = memcpy_s(str, len - 1, ((uint8_t *)&at_para->payload[POS_0] + i * len), len - 1);
        if (ret != EOK) {
            BSP_MEM_FREE(param);
            wstp_print("memcpy_s ret 0x%x\n", ret);
            return ERRCODE_FAIL;
        }
        param[POS_2 + i] = strtol((char*)str, NULL, base);
    }
    uint32_t at_id = (at_para->interHeader.serviceID << 8) + at_para->interHeader.commandID;
    for (uint8_t i = 0; i < sizeof(g_btsrv_at_cmd_2_dft) / sizeof(btsrv_at_cmd_2_dft_t); i++) {
        if (g_btsrv_at_cmd_2_dft[i].at_id == at_id && g_btsrv_at_cmd_2_dft[i].func != NULL) {
            g_btsrv_at_cmd_2_dft[i].func(param);
            break;
        }
    }
    BSP_MEM_FREE(param);
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_bt_show_link_mode(const bt_manager_commu_inter_info_t *at_para)
{
    unused(at_para);
    btsrv_dfx_show_link_mode();
    return ERRCODE_SUCC;
}

uint32_t btsrv_wstp_at_bt_set_log_type_level(const bt_manager_commu_inter_info_t *at_para)
{
    btsrv_set_log_type_level(*(uint32_t *)at_para->payload);
    return ERRCODE_SUCC;
}

static const btsrv_at_cmd_hdl_info g_wstp_at_cmd_hdls[] = {
    { BTSRV_WSTP_AT_TEST, btsrv_wstp_at_test },
    { BTSRV_WSTP_AT_ENABLE_BT, btsrv_wstp_at_enable_bt },
    { BTSRV_WSTP_AT_DISABLE_BT, btsrv_wstp_at_disable_bt },
    { BTSRV_WSTP_AT_BT_SERVICE_TEST_CMD, btsrv_wstp_at_bt_service_test_cmd },
    { BTSRV_WSTP_AT_SET_LOCAL_ADDR, btsrv_wstp_at_set_local_addr },
    { BTSRV_WSTP_AT_GET_LOCAL_ADDR, btsrv_wstp_at_get_local_addr },
    { BTSRV_WSTP_AT_SET_LOCAL_NAME, btsrv_wstp_at_set_local_name },
    { BTSRV_WSTP_AT_GET_LOCAL_NAME, btsrv_wstp_at_get_local_name },
    { BTSRV_WSTP_AT_FACTORY_RESET, btsrv_wstp_at_factory_reset },
    { BTSRV_WSTP_AT_BT_START_DISCOVERY, btsrv_wstp_at_bt_start_discovery },
    { BTSRV_WSTP_AT_BT_CANCEL_DISCOVERY, btsrv_wstp_at_bt_cancel_discovery },
    { BTSRV_WSTP_AT_BT_READ_DISCOVERY_INFO, btsrv_wstp_at_bt_read_discovery_info},
    { BTSRV_WSTP_AT_BT_DFX_SHOW_CONN_LIST, btsrv_wstp_at_show_conn_list},
    { BTSRV_WSTP_AT_BT_DFX_SHOW_REMOTE_DEV_LIST, btsrv_wstp_at_show_remote_dev_list},
    { BTSRV_WSTP_AT_BT_GET_PAIRED_DEVICES_NUM, btsrv_wstp_at_get_paired_devices_num},
    { BTSRV_WSTP_AT_BT_GET_PAIRED_DEVICES_LIST, btsrv_wstp_at_get_paired_devices_list},
    { BTSRV_WSTP_AT_BT_GET_PAIR_STATE, btsrv_wstp_at_get_pair_state},

#ifdef SUPPORT_BLE
    { BTSRV_WSTP_AT_BLE_ENABLE_ADV,  btsrv_wstp_at_ble_start_adv },
    { BTSRV_WSTP_AT_BLE_DISABLE_ADV, btsrv_wstp_at_ble_stop_adv },
    { BTSRV_WSTP_AT_BLE_ENTER_SCAN,  btsrv_wstp_at_ble_enter_scan },
    { BTSRV_WSTP_AT_BLE_GATT_CREATE_SERVER, btsrv_wstp_at_ble_gatt_create_server },
    { BTSRV_WSTP_AT_BLE_GATT_CREATE_SERVICE, btsrv_wstp_at_ble_gatt_create_service },
    { BTSRV_WSTP_AT_BLE_GATT_ADD_INCLUDE_SERVICE, btsrv_wstp_at_ble_gatt_add_include_service },
    { BTSRV_WSTP_AT_BLE_GATT_ADD_CHARACTER, btsrv_wstp_at_ble_gatt_add_characteristic },
    { BTSRV_WSTP_AT_BLE_GATT_ADD_DESCRIPTOR, btsrv_wstp_at_ble_gatt_add_descriptor },
    { BTSRV_WSTP_AT_BLE_GATT_START_SERVICE,  btsrv_wstp_at_ble_gatt_start_service },
    { BTSRV_WSTP_AT_BLE_GATT_SEND_RESPONSE, btsrv_wstp_at_ble_gatt_send_response },
    { BTSRV_WSTP_AT_BLE_GATT_UPDATE_CHAR_VALUE, btsrv_wstp_at_ble_gatt_update_characteristic_value },
    { BTSRV_WSTP_AT_BLE_GATT_UPDATE_DESC_VALUE, btsrv_wstp_at_ble_gatt_update_descriptor_value },
    { BTSRV_WSTP_AT_BLE_GATT_GET_GAP_ATTRIBUTE, btsrv_wstp_at_ble_gatt_get_gap_attribute },
    { BTSRV_WSTP_AT_BLE_GATT_SET_GAP_ATTRIBUTE, btsrv_wstp_at_ble_gatt_set_gap_attribute },
    { BTSRV_WSTP_AT_BLE_GATT_MULTIPLE_VALUE_NTF, btsrv_wstp_at_ble_gatt_multiple_value_ntf },
    { BTSRV_WSTP_AT_BLE_GATT_MTU_SET_PARAM, btsrv_wstp_at_ble_gatt_mtu_set_param },
    { BTSRV_WSTP_AT_BLE_GATT_DISCOVERY_SERVICE, btsrv_wstp_at_ble_gatt_discovery_service },
    { BTSRV_WSTP_AT_BLE_GATT_DISCOVERY_INCLUDE, btsrv_wstp_at_ble_gatt_discovery_include },
    { BTSRV_WSTP_AT_BLE_GATT_DISCOVERY_CHAR, btsrv_wstp_at_ble_gatt_discovery_character },
    { BTSRV_WSTP_AT_BLE_GATT_DISCOVERY_CHAR_BY_UUID, btsrv_wstp_at_ble_gatt_discovery_character_by_uuid },
    { BTSRV_WSTP_AT_BLE_GATT_DISCOVERY_DESC, btsrv_wstp_at_ble_gatt_discovery_descriptor },
    { BTSRV_WSTP_AT_BLE_GATT_DISC_DESC_BY_HANDLE, btsrv_wstp_at_ble_gatt_discovery_descriptor_by_handle },
    { BTSRV_WSTP_AT_BLE_GATT_READ_CHARACTER, btsrv_wstp_at_ble_gatt_read_character },
    { BTSRV_WSTP_AT_BLE_GATT_READ_CHAR_BY_UUID, btsrv_wstp_at_ble_gatt_read_character_by_uuid },
    { BTSRV_WSTP_AT_BLE_GATT_READ_MULTIPLE_CHAR, btsrv_wstp_at_ble_gatt_read_multiple_character },
    { BTSRV_WSTP_AT_BLE_GATT_MULT_VALUE_READ_CHAR, btsrv_wstp_at_ble_gatt_multiple_value_read_character },
    { BTSRV_WSTP_AT_BLE_GATT_WRITE_CHARACTER, btsrv_wstp_at_ble_gatt_write_character },
    { BTSRV_WSTP_AT_BLE_GATT_RELIABLE_WRITE_CHAR, btsrv_wstp_at_ble_gatt_reliable_write_character },
    { BTSRV_WSTP_AT_BLE_GATT_READ_DESCRIPTOR, btsrv_wstp_at_ble_gatt_read_descriptor },
    { BTSRV_WSTP_AT_BLE_GATT_WRITE_DESCRIPTOR, btsrv_wstp_at_ble_gatt_write_descriptor },
    { BTSRV_WSTP_AT_BLE_GATT_MTU_CONFIG, btsrv_wstp_at_ble_gatt_mtu_config },
    { BTSRV_WSTP_AT_BLE_GATT_VALUE_NOTIFICATION, btsrv_wstp_at_ble_gatt_value_notification },
    { BTSRV_WSTP_AT_BLE_GATT_GET_HID_DEVICE_RPT, btsrv_wstp_at_ble_gatt_get_hid_device_report },
    { BTSRV_WSTP_AT_BLE_GATT_SET_ATT_MIN_KEY_SIZE, btsrv_wstp_at_ble_gatt_set_attribute_min_key_size },
    { BTSRV_WSTP_AT_BLE_GATT_SET_ATT_CONFIG_PARAM, btsrv_wstp_at_ble_gatt_set_attribute_config_param },
    { BTSRV_WSTP_AT_BLE_GATT_CALLBACK_REG, btsrv_wstp_at_gatt_cmd_cbk_reg},
#endif

#ifdef SUPPORT_BREDR
    { BTSRV_WSTP_AT_BT_OPEN_INQSCAN, btsrv_wstp_at_bt_open_inqscan },
    { BTSRV_WSTP_AT_BT_CLOSE_INQSCAN, btsrv_wstp_at_bt_close_inqscan },
    { BTSRV_WSTP_AT_BT_CONN_BY_ADDR, btsrv_wstp_at_bt_conn_by_addr },
    { BTSRV_WSTP_AT_BT_CANCEL_PAIR, btsrv_wstp_at_bt_cancel_pair },
    { BTSRV_WSTP_AT_BT_IS_CONNECTED, btsrv_wstp_at_bt_is_connected },
    { BTSRV_WSTP_AT_BT_DISCONNECT, btsrv_wstp_at_bt_disconnect },
    { BTSRV_WSTP_AT_AVRCP_REG, btsrv_wstp_at_avrcp_reg },
    { BTSRV_WSTP_AT_AVRCP_DEREG, btsrv_wstp_at_avrcp_disreg },
    { BTSRV_WSTP_AT_AVRCP_TG_CONN, btsrv_wstp_at_avrcp_tg_conn },
    { BTSRV_WSTP_AT_AVRCP_TG_DISCONN, btsrv_wstp_at_avrcp_tg_disconn },
    { BTSRV_WSTP_AT_AVRCP_TG_GET_DEV_CONN_STATE, btsrv_wstp_at_avrcp_tg_get_dev_conn_state },
    { BTSRV_WSTP_AT_AVRCP_TG_NOTIFY_STATE, btsrv_wstp_at_avrcp_tg_notify_state },
    { BTSRV_WSTP_AT_AVRCP_TG_SET_LOC_INF, btsrv_wstp_at_avrcp_tg_set_loc_inf },
    { BTSRV_WSTP_AT_AVRCP_CT_CONN, btsrv_wstp_at_avrcp_ct_connect },
    { BTSRV_WSTP_AT_AVRCP_CT_DISCONN, btsrv_wstp_at_avrcp_ct_disconnect },
    { BTSRV_WSTP_AT_AVRCP_CT_PRESS_BUTTON, btsrv_wstp_at_avrcp_ct_press_button },
    { BTSRV_WSTP_AT_AVRCP_CT_SET_ABS_VOLUME, btsrv_wstp_at_bt_avrcp_ct_set_absolute_volume },
    { BTSRV_WSTP_AT_AVRCP_CT_GET_TG_PLAY_STAT, btsrv_wstp_at_avrcp_ct_get_tg_player_stat },
    { BTSRV_WSTP_AT_AVRCP_CT_GET_ELEMENT_ATTRI, btsrv_wstp_at_avrcp_ct_get_element_attri },
    { BTSRV_WSTP_AT_AVRCP_CT_GET_SUPPORT_EVENT, btsrv_wstp_at_wstp_avrcp_ct_get_support_event },
    { BTSRV_WSTP_AT_AVRCP_CT_REG_SUPPORT_EVENT, btsrv_wstp_at_avrcp_ct_reg_support_event },
    { BTSRV_WSTP_AT_AVRCP_CT_GET_CONN_STAT, btsrv_wstp_at_avrcp_get_conn_stat },
    { BTSRV_WSTP_AT_A2DP_SRC_REG, btsrv_wstp_at_a2dp_src_reg },
    { BTSRV_WSTP_AT_A2DP_SRC_DEREG, btsrv_wstp_at_a2dp_src_dereg },
    { BTSRV_WSTP_AT_A2DP_SRC_GET_DEVICE_CONN_STATE, btsrv_wstp_at_a2dp_src_get_device_connect_state },
    { BTSRV_WSTP_AT_A2DP_SRC_GET_PLAYING_STATE, btsrv_wstp_at_a2dp_src_get_playing_state },
    { BTSRV_WSTP_AT_A2DP_SRC_CONN, btsrv_wstp_at_a2dp_src_connect },
    { BTSRV_WSTP_AT_A2DP_SRC_DISCONN, btsrv_wstp_at_a2dp_src_disconnect },
    { BTSRV_WSTP_AT_A2DP_SRC_GET_ACTIVE_DEVICE, btsrv_wstp_at_a2dp_src_get_active_device },
    { BTSRV_WSTP_AT_A2DP_SRC_SET_PLAY_START, btsrv_wstp_at_a2dp_src_start_playing },
    { BTSRV_WSTP_AT_A2DP_SRC_SET_PLAY_SUSPEND, btsrv_wstp_at_a2dp_src_suspend_playing },
    { BTSRV_WSTP_AT_A2DP_SRC_SET_PLAY_STOP, btsrv_wstp_at_a2dp_src_stop_playing },
    { BTSRV_WSTP_AT_A2DP_SNK_REG, btsrv_wstp_at_a2dp_snk_reg },
    { BTSRV_WSTP_AT_A2DP_SNK_DEREG, btsrv_wstp_at_a2dp_snk_dereg },
    { BTSRV_WSTP_AT_A2DP_SNK_GET_DEVICE_CONN_STATE, btsrv_wstp_at_a2dp_snk_get_device_connect_state },
    { BTSRV_WSTP_AT_A2DP_SNK_GET_PLAYING_STATE, btsrv_wstp_at_a2dp_snk_get_playing_state },
    { BTSRV_WSTP_AT_A2DP_SNK_CONN, btsrv_wstp_at_a2dp_snk_connect },
    { BTSRV_WSTP_AT_A2DP_SNK_DISCONN, btsrv_wstp_at_a2dp_snk_disconnect },
    { BTSRV_WSTP_AT_A2DP_SNK_GET_ACTIVE_DEVICE, btsrv_wstp_at_a2dp_snk_get_active_device },
    { BTSRV_WSTP_AT_A2DP_SNK_SET_PLAY_START, btsrv_wstp_at_a2dp_snk_start_playing },
    { BTSRV_WSTP_AT_A2DP_SNK_SET_PLAY_SUSPEND, btsrv_wstp_at_a2dp_snk_suspend_playing },
    { BTSRV_WSTP_AT_A2DP_SNK_SET_PLAY_STOP, btsrv_wstp_at_a2dp_snk_stop_playing },
    { BTSRV_WSTP_AT_PBAP_REG, btsrv_wstp_at_pbap_reg },
    { BTSRV_WSTP_AT_PBAP_CONN, btsrv_wstp_at_pbap_conn },
    { BTSRV_WSTP_AT_PBAP_DISCONN, btsrv_wstp_at_pbap_disconn },
    { BTSRV_WSTP_AT_PBAP_PULL_PHONEBOOK, btsrv_wstp_at_pbap_pull_phonebook },
    { BTSRV_WSTP_AT_PBAP_PULL_VCARD_LISTING, btsrv_wstp_at_pbap_pull_vcard_listing },
    { BTSRV_WSTP_AT_PBAP_PULL_VCARD_ENTRY, btsrv_wstp_at_pbap_pull_vcard_entry },
    { BTSRV_WSTP_AT_PBAP_STOP_TRANS, btsrv_wstp_at_pbap_stop_trans },
    { BTSRV_WSTP_AT_PBAP_GET_CONN_STATE, btsrv_wstp_at_pbap_get_conn_state },
    { BTSRV_WSTP_AT_HFP_REG, btsrv_wstp_at_hfp_reg },
    { BTSRV_WSTP_AT_HFP_PICKUP, btsrv_wstp_at_hfp_pick_up },
    { BTSRV_WSTP_AT_HFP_REJECT, btsrv_wstp_at_hfp_reject },
    { BTSRV_WSTP_AT_HFP_HANGUP, btsrv_wstp_at_hfp_hang_up },
    { BTSRV_WSTP_AT_HFP_HOLD, btsrv_wstp_at_hfp_hold_active_call },
    { BTSRV_WSTP_AT_HFP_SET_SPK_VOL, btsrv_wstp_at_bt_set_spk_vol },
    { BTSRV_WSTP_AT_HFP_SET_MIC_VOL, btsrv_wstp_at_bt_set_mic_vol },
    { BTSRV_WSTP_AT_HFP_START_DAIL, btsrv_wstp_at_bt_start_dail },
    { BTSRV_WSTP_AT_HFP_HF_CONN, btsrv_wstp_at_hfp_hf_conn },
    { BTSRV_WSTP_AT_HFP_HF_DISCONN, btsrv_wstp_at_hfp_hf_disconn },
    { BTSRV_WSTP_AT_HFP_HF_CONN_SCO, btsrv_wstp_at_hfp_hf_conn_sco },
    { BTSRV_WSTP_AT_HFP_HF_DISCONN_SCO, btsrv_wstp_at_hfp_hf_disconn_sco },
    { BTSRV_WSTP_AT_HFP_GET_DEV_CONN_STATE, btsrv_wstp_at_hfp_get_dev_conn_state },
    { BTSRV_WSTP_AT_HFP_GET_SCO_CONN_STATE, btsrv_wstp_at_hfp_get_sco_conn_state },
    { BTSRV_WSTP_AT_AVRCP_CT_GET_SUPPORT_COMPANIES, btsrv_wstp_at_avrcp_ct_get_support_companies },
    { BTSRV_WSTP_AT_SPP_SERVER_CREATE, btsrv_wstp_at_spp_server_create },
    { BTSRV_WSTP_AT_SPP_SERVER_ACCEPT, btsrv_wstp_at_spp_server_accept },
    { BTSRV_WSTP_AT_SPP_SERVER_CLOSE, btsrv_wstp_at_spp_server_close },
    { BTSRV_WSTP_AT_SPP_CONN, btsrv_wstp_at_spp_conn },
    { BTSRV_WSTP_AT_SPP_DISCONN, btsrv_wstp_at_spp_disconn },
    { BTSRV_WSTP_AT_SPP_ISCONN, btsrv_wstp_at_spp_is_conn },
    { BTSRV_WSTP_AT_SPP_GET_RMT_ADDR, btsrv_wstp_at_spp_get_rmt_addr },
    { BTSRV_WSTP_AT_SPP_READ_DATA, btsrv_wstp_at_spp_read_data },
    { BTSRV_WSTP_AT_SPP_WRITE_DATA, btsrv_wstp_at_spp_write_data },
#endif
    { BTSRV_WSTP_AT_DFT_REG, btsrv_wstp_at_dft_reg },
    { BTSRV_WSTP_AT_BT_SET_TX_POWER, btsrv_wstp_at_bt_set_tx_power },
    { BTSRV_WSTP_AT_BT_SET_DUT_MODE, btsrv_wstp_at_bt_set_dut_mode },
    { BTSRV_WSTP_AT_BT_SET_SIG, btsrv_wstp_at_bt_set_sig },
    { BTSRV_WSTP_AT_BT_DFT_SET_POWRE_ALL, btsrv_wstp_at_bt_dft_set_power_all },
    {BTSRV_WSTP_AT_BT_DFT_ENTER_DUT_MODE, btsrv_wstp_at_bt_dft},
    {BTSRV_WSTP_AT_BT_DFT_BREDR_TX_MODE, btsrv_wstp_at_bt_dft},
    {BTSRV_WSTP_AT_BT_DFT_BREDR_RX_MODE, btsrv_wstp_at_bt_dft},
    {BTSRV_WSTP_AT_BT_DFT_BLE_TX_MODE, btsrv_wstp_at_bt_dft},
    {BTSRV_WSTP_AT_BT_DFT_BLE_RX_MODE, btsrv_wstp_at_bt_dft},
    {BTSRV_WSTP_AT_BT_DFT_RESET_RX_COUNT, btsrv_wstp_at_bt_dft},
    {BTSRV_WSTP_AT_BT_DFT_READ_RX_COUNT, btsrv_wstp_at_bt_dft},
#ifdef FT_DONGLE
    {BTSRV_WSTP_AT_BT_COUPLING_TEST_BT_TX, btsrv_wstp_at_bt_dft},
    {BTSRV_WSTP_AT_BT_COUPLING_TEST_BT_RX, btsrv_wstp_at_bt_dft},
    {BTSRV_WSTP_AT_BT_COUPLING_TEST_BLE_TX, btsrv_wstp_at_bt_dft},
    {BTSRV_WSTP_AT_BT_COUPLING_TEST_BLE_RX, btsrv_wstp_at_bt_dft},
    {BTSRV_WSTP_AT_BT_SET_CT_DELAY_MS, btsrv_wstp_at_bt_set_ct_delay_ms},
    {BTSRV_WSTP_AT_BT_SET_DONGLE_ADDR, btsrv_wstp_at_bt_set_dongle_addr},
#endif
    {BTSRV_WSTP_AT_BT_DFX_SHOW_LINK_MODE, btsrv_wstp_at_bt_show_link_mode},
    {BTSRV_WSTP_AT_BT_LOG_TYPE_LEVEL, btsrv_wstp_at_bt_set_log_type_level },
    {BTSRV_WSTP_AT_BLE_SHOW_CONN_LIST, btsrv_wstp_at_show_ble_conn_list},
};

void bt_manager_at_process(uint8_t *data, uint32_t data_len)
{
    unused(data_len);
    if (data == NULL) {
        return;
    }

    uint32_t ret;
    uint8_t rsp_data[BT_APK_RSP_PAYLOAD_LEN] = {0};
    bt_manager_commu_inter_info_t *at_para = (bt_manager_commu_inter_info_t *)(data);
    /* 使用保留位标识AT指令是串口下发还是APK下发 */
    at_para->reserved = false;
    uint8_t svcId = at_para->interHeader.serviceID;
    uint8_t cmdId = at_para->interHeader.commandID;
    uint32_t atId = (svcId << 8) + cmdId;
    wstp_print("bt_manager_at_process cmdId=0x%x atId=0x%x\n", cmdId, atId);
    int hdl_cnt = sizeof(g_wstp_at_cmd_hdls) / sizeof(btsrv_at_cmd_hdl_info);
    for (int i = 0; i < hdl_cnt; i++) {
        if (g_wstp_at_cmd_hdls[i].id == atId && g_wstp_at_cmd_hdls[i].func != NULL) {
            ret = g_wstp_at_cmd_hdls[i].func(at_para);
            print_at_cmd_ret(ERRCODE_SUCC);
            break;
        }
    }

    /* APK指令，需要通过SPP给予答复 */
    if (g_bt_diag_flag) {
        rsp_data[POS_0] = BT_APK_TLV_80;
        rsp_data[POS_1] = BT_APK_TLV_82;
        rsp_data[POS_2] = BT_APK_TLV_0;
        rsp_data[POS_3] = (uint8_t)ret;
        ret = send_spp_diag_msg(g_bt_diag_flag, &g_bt_diag_header, rsp_data, BT_APK_RSP_PAYLOAD_LEN);
        wstp_print("send rsp msg to apk ret=0x%X\n", ret);
    }
    g_bt_diag_flag = false;
}