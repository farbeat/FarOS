/**
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides at cmd manager source\n
 * Author: CompanyName \n
 * History: \n
 * 2022-11-17， Create file. \n
 */
#include "debug_print.h"
#include "securec.h"
#include "soc_osal.h"
#include "common_def.h"
#include "mc_bt_manager.h"
#include "app_at_process.h"
#include "bt_manager_tx_process.h"
#include "bt_manager_cmd_manager.h"

#define BSP_MEM_ALLOC(y) osal_kmalloc((y), OSAL_GFP_KERNEL)
#define BSP_MEM_FREE(y) osal_kfree(y)
#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))
#define MAX_PARAM_COUNT             128
#define ONE_ADDR_LEN                2
#define CMD_PAYLOAD_MAX             (MAX_PARAM_COUNT-9)
#define CMD_FORMAT_LEN              3
#define CMD_HEAD_LEN                4
#define CMD_PARAM_OFFSET            (CMD_FORMAT_LEN+CMD_HEAD_LEN)
#define BT_ADDR_LEN                 6
#define AT_DISC_PHONE_FLAG          1
#define BT_NAME_LEN_MAX             32
#define CMD_CONTROL_OFFSET          (CMD_HEAD_LEN)
#define CMD_SID_OFFSET              (CMD_HEAD_LEN+1)
#define CMD_CID_OFFSET              (CMD_HEAD_LEN+2)

/* 处理AT命令对应命令序列的处理接口，将命令序列字符串strcmd处理成数组param */
static void ParseCmdParam(char* strcmd, uint8_t* param, uint32_t str_len)
{
    char* p;
    uint8_t* strtemp;
    int len;
    int ret;
    int i;
    len = (str_len + 1) / 3;     //命令序列长度，+1为结束符，/3为每个十六进制数字符加空格长度
    if (len > MAX_PARAM_COUNT || strcmd == NULL) {
        wstp_print("[AT][ParseCmdParam]cmd len=%d too long\r\n", len);
        return;
    }
    strtemp = (uint8_t*)BSP_MEM_ALLOC(str_len + 1);
    memset_s(strtemp, str_len + 1, 0, str_len + 1);
    ret = memcpy_s(strtemp, str_len, strcmd, str_len);
    if (ret != EOK) {
        BSP_MEM_FREE(strtemp);
        wstp_print("[AT][ParseCmdParam]memcpy= error!\r\n");
        return;
    }

    //处理十六进制命令序列转数字数组
    p = strtok((char*)strtemp, " ");
    for (i = 0; i < len; i++) {
        param[i] = strtol(p, NULL, 16);
        p = strtok(NULL, " ");
    }
    BSP_MEM_FREE(strtemp);
}

/* 处理参数为001122334455这类地址，转成数组param */
static void ParseATCmdAddrParam(char* strCmd, uint8_t* param, uint32_t count)
{
    char temp[ONE_ADDR_LEN + 1] = {0};
    char* p;
    int ret;
    p = strCmd;

    for (uint32_t i = 0; i < count; i++) {
        ret = memcpy_s(temp, ONE_ADDR_LEN + 1, p, ONE_ADDR_LEN);
        if (ret != EOK) {
            wstp_print("[AT][ParseATCmdAddrParam]memcpy= error!\r\n");
            return;
        }
        temp[ONE_ADDR_LEN] = '\0';
        param[i] = strtol(temp, NULL, 16);
        p = p + ONE_ADDR_LEN;
    }
}

/* 处理参数为MACBT,001e10112233类型的AT^PHYNUM设置蓝牙地址命令，转成数组param */
static void ParseATCmdSetAddrParam(char* strCmd, uint8_t* param)
{
    char* strKey = "MACBT," ;
    char* p = strCmd + strlen(strKey);
    char temp[ONE_ADDR_LEN + 1] = {0};
    int i;
    int ret;

    for (i = BT_ADDR_LEN - 1; i >= 0; i--) {
        ret = memcpy_s(temp, ONE_ADDR_LEN + 1, p, ONE_ADDR_LEN);
        if (ret != EOK) {
            wstp_print("[AT][ParseATCmdAddrParam]memcpy= error!\r\n");
            return;
        }
        temp[ONE_ADDR_LEN] = '\0';
        param[i] = strtol(temp, NULL, 16);
        p = p + ONE_ADDR_LEN;
    }

    return;
}

/* 处理参数为15800001234这类地址，转成数组param */
static void ParseATCmdCallNumParam(char* strCmd, uint32_t para_len, uint8_t* param)
{
    char temp[2] = {0};     //2为一个十进制数字，一个结束符
    char* p;
    uint32_t i;
    p = strCmd;
    /* 减去结束符长度 */
    uint32_t param_len = para_len - 1;

    for (i = 0; i < param_len; i++) {
        if (memcpy_s(temp, 2, p, 1) != EOK) {
            wstp_print("[AT][ParseATCmdCallNumParam]memcpy= error!\r\n");
            return;
        }
        temp[1] = '\0';
        param[i] = strtol(temp, NULL, 10) + '0';
        p++;
    }
}

/* 处理param为一个数字的接口 */
static uint32_t ParseSingleNumParam(char* para, uint32_t len)
{
    char* strtemp;
    uint32_t ret;
    if (para == NULL) {
        wstp_print("[AT][ParseSingleNumParam]para err!\r\n");
        return 0;
    }

    // 设定读取数值最大长度为6
    if (len > 6) {
        wstp_print("[AT][ParseSingleNumParam]num error!max num is 999999\r\n");
        return 0;
    }

    strtemp = (char*)BSP_MEM_ALLOC(len + 1);
    if (strtemp == NULL) {
        wstp_print("[AT][ParseSingleNumParam] malloc fail\r\n");
        return 0;
    }
    memset_s(strtemp, len + 1, 0, len + 1);
    ret = memcpy_s(strtemp, len + 1, para, len);
    if (ret != EOK) {
        BSP_MEM_FREE(strtemp);
        wstp_print("[AT][ParseSingleNumParam]memcpy= error!\r\n");
        return 0;
    }
    strtemp[len] = '\0';

    ret = strtol(strtemp, NULL, 10);
    BSP_MEM_FREE(strtemp);
    return ret;
}

static void package_common_cmd_to_bt(char* strCmd, char* inParam, ParamTypeIdE paramType, uint32_t para_len)
{
    uint8_t param[MAX_PARAM_COUNT] = {0};
    commu_inter_info_t* interInfo = NULL;
    uint32_t infoLen;
    int payloadLen;
    int ret;

    ParseCmdParam(strCmd, param, strlen(strCmd));
    payloadLen = para_len;
    wstp_print("package_common_cmd_to_bt strCmd %s inParam %s para_len %d\n", strCmd, inParam, para_len);
    if (payloadLen > CMD_PAYLOAD_MAX || payloadLen < 0) {
        wstp_print("[AT][package_common_cmd_to_bt]payloadLen=%d err!\r\n", payloadLen);
        return;
    }

    infoLen = sizeof(commu_inter_info_t) + payloadLen;
    interInfo = (commu_inter_info_t*)BSP_MEM_ALLOC(infoLen);
    if (interInfo == NULL) {
        wstp_print("[AT][package_common_cmd_to_bt] malloc=%d fail\r\n", infoLen);
        return;
    }

    memset_s(interInfo, infoLen, 0, infoLen);
    if (paramType == PARAM_TYPE_ADDR) {
        ParseATCmdAddrParam(inParam, &param[CMD_PARAM_OFFSET], BT_ADDR_LEN);
    } else if (paramType == PARAM_TYPE_SET_ADDR) {
        ParseATCmdSetAddrParam(inParam, &param[CMD_PARAM_OFFSET]);
    } else if (paramType == PARAM_TYPE_CALL_NUM) {
        ParseATCmdCallNumParam(inParam, para_len, &param[CMD_PARAM_OFFSET]);
    } else if (paramType == PARAM_TYPE_DISC_PHONE) {
        ParseATCmdAddrParam(inParam, &param[CMD_PARAM_OFFSET], BT_ADDR_LEN);
        param[CMD_PARAM_OFFSET + BT_ADDR_LEN] = AT_DISC_PHONE_FLAG;
    } else if (paramType == PARAM_TYPE_HIGH_POWER) {
        ParseATCmdAddrParam(inParam, &param[CMD_PARAM_OFFSET], BT_ADDR_LEN);
        param[CMD_PARAM_OFFSET + BT_ADDR_LEN] = ParseSingleNumParam(&inParam[ONE_ADDR_LEN * BT_ADDR_LEN], 1);
    } else if (paramType == PARAM_TYPE_SET_VALUE) {
        ParseCmdParam(inParam, &param[CMD_PARAM_OFFSET], para_len);
        payloadLen = (payloadLen + 1) / 3;
    }

    if (payloadLen > 0) {
        ret = memcpy_s(&interInfo->payload[0], payloadLen, &param[CMD_PARAM_OFFSET], payloadLen);
        if (ret != EOK) {
            wstp_print("[AT][package_common_cmd_to_bt]memcpy error!\r\n");
            BSP_MEM_FREE(interInfo);
            return;
        }
    }

    interInfo->interHeader.control = param[CMD_CONTROL_OFFSET];
    interInfo->interHeader.devType = 0;
    interInfo->interHeader.appID[0] = 0;
    interInfo->interHeader.serviceID = param[CMD_SID_OFFSET];
    interInfo->interHeader.commandID = param[CMD_CID_OFFSET];
    interInfo->payloadLength = payloadLen;
    interInfo->tranIDSrc = 0;
    wstp_print("commandID = 0x%x payload_len=%d\n", interInfo->interHeader.commandID, payloadLen);
    bt_manager_msg_send((uint8_t *)interInfo, infoLen);
    BSP_MEM_FREE(interInfo);
}

/* 无参数AT命令处理接口 */
static uint32_t parse_no_para_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    package_common_cmd_to_bt(str_cmd, (char*)NULL, PARAM_TYPE_NULL, para_len);
    return 0;
}

/* 有固定长度参数，参数为地址数据接口 */
static uint32_t parse_para_addr_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    package_common_cmd_to_bt(str_cmd, (char*)para, PARAM_TYPE_ADDR, para_len);
    return 0;
}

/* 断开指定蓝牙地址 */
static uint32_t parse_para_discp_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    package_common_cmd_to_bt(str_cmd, (char*)para, PARAM_TYPE_DISC_PHONE, para_len);
    return 0;
}

/* 设置电话号码 */
static uint32_t parse_para_call_num_handle(uint8_t *para, uint32_t para_len, char* str_cmd)
{
    package_common_cmd_to_bt(str_cmd, (char*)para, PARAM_TYPE_CALL_NUM, para_len);
    return 0;
}

/* 设置变长数值 */
static uint32_t parse_para_set_value_handle(uint8_t *para, uint32_t para_len, char* str_cmd)
{
    package_common_cmd_to_bt(str_cmd, (char*)para, PARAM_TYPE_SET_VALUE, para_len);
    return 0;
}

/* 设置高功率开关 */
static uint32_t parse_para_set_high_power_handle(uint8_t *para, uint32_t paraLen, char* strCommonCmd)
{
    package_common_cmd_to_bt(strCommonCmd, (char*)para, PARAM_TYPE_HIGH_POWER, paraLen);
    return 0;
}

static void package_name_param_cmd_to_bt(char* strCmd, char* inParam, uint32_t paraLen, ParamTypeIdE paramType)
{
    unused(paramType);
    commu_inter_info_t* interInfo = NULL;
    uint8_t param[MAX_PARAM_COUNT] = {0};
    uint32_t infoLen;
    int payloadLen;
    int ret;

    payloadLen = paraLen + 1;     //+1多为结束符申请
    ParseCmdParam(strCmd, param, strlen(strCmd));
    if (payloadLen > BT_NAME_LEN_MAX || payloadLen <= 1) {
        //设备不能为空，并且长度小于等于32
        wstp_print("[AT][package_name_param_cmd_to_bt]len=%d err!\r\n", payloadLen);
        return;
    }
    infoLen = sizeof(commu_inter_info_t) + payloadLen;
    interInfo = (commu_inter_info_t*)BSP_MEM_ALLOC(infoLen);
    if (interInfo == NULL) {
        wstp_print("[AT][package_name_param_cmd_to_bt] malloc=%d fail\r\n", infoLen);
        return;
    }

    memset_s(interInfo, infoLen, 0, infoLen);
    ret = memcpy_s(&interInfo->payload[0], BT_NAME_LEN_MAX, inParam, paraLen);
    if (ret != EOK) {
        wstp_print("[AT][package_name_param_cmd_to_bt]memcpy error!\r\n");
        BSP_MEM_FREE(interInfo);
        return;
    }
    interInfo->payload[paraLen] = '\0';
    interInfo->interHeader.tranID = 0;
    interInfo->interHeader.control = param[CMD_CONTROL_OFFSET];
    interInfo->interHeader.devType = 0;
    interInfo->interHeader.appID[0] = 0;
    interInfo->interHeader.serviceID = param[CMD_SID_OFFSET];
    interInfo->interHeader.commandID = param[CMD_CID_OFFSET];
    interInfo->payloadLength = payloadLen;
    interInfo->tranIDSrc = 0;
    wstp_print("commandID = 0x%x payload_len=%d\n", interInfo->interHeader.commandID, payloadLen);
    bt_manager_msg_send((uint8_t *)interInfo, infoLen);
    BSP_MEM_FREE(interInfo);
}

/* 有不固定长度参数接口 */
static uint32_t parse_para_set_name_handle(uint8_t *para, uint32_t para_len, char* str_cmd)
{
    package_name_param_cmd_to_bt(str_cmd, (char*)para, para_len, PARAM_TYPE_NULL);
    return 0;
}

/* bt_service_sample_cmd接口适配 */
static uint32_t parse_para_bt_test_handle(uint8_t *para, uint32_t para_len, char* str_cmd)
{
    unused(str_cmd);
    int32_t i;
    int32_t len;
    int32_t ret;
    int32_t argc = 0;
    uint8_t *strTemp;
    char *p = NULL;
    char *msg_argv[MAX_AT_CMD_NAME_LEN] = {NULL};

    if (para == NULL) {
        return ERRCODE_INVALID_PARAM;
    }
    wstp_print("[at cmd] test"NEWLINE);
    strTemp = (uint8_t*)malloc(para_len + 1); /* AT送过来的是命令序列，不是字符串，所以没有结束符 需要手动加上结束符 */
    if (strTemp == NULL) {
        free(strTemp);
        wstp_print("bt test:parse_para_bt_test_handle:%d\r\n", __LINE__);
        return ERRCODE_FAIL;
    }

    ret = memcpy_s(strTemp, para_len, para, para_len);
    if (ret != EOK) {
        free(strTemp);
        wstp_print("bt test:parse_para_bt_test_handle:%d\r\n", __LINE__);
        return ERRCODE_FAIL;
    }

    strTemp[para_len] = '\0'; /* 给命令序列加上结束符， 方便下面做字符串处理 */

    p = strtok((char*)strTemp, " ");
    for (i = 0; p != NULL; i++) {
        len = strlen(p) + 1;

        msg_argv[i] = (char*)malloc(len);
        if (msg_argv[i] == NULL) {
            free(strTemp);
            wstp_print("bt test:parse_para_bt_test_handle:%d\r\n", __LINE__);
            return ERRCODE_FAIL;
        }

        ret = memcpy_s(msg_argv[i], len, p, len);
        if (ret != EOK) {
            free(strTemp);
            free(msg_argv[i]);
            wstp_print("bt test:parse_para_bt_test_handle:%d\r\n", __LINE__);
            return ERRCODE_FAIL;
        }

        argc++;
        if (argc >= MAX_AT_CMD_NAME_LEN) {
            free(strTemp);
            free(msg_argv[i]);
            wstp_print("bt test:parse_para_bt_test_handle:%d\r\n", __LINE__);
            return ERRCODE_FAIL;
        }

        p = strtok(NULL, " ");
    }

    free(strTemp);

    bt_service_sample_cmd(argc, msg_argv);

    for (i = 0; i < argc; i++) {
        if (msg_argv[i] != NULL) {
            free(msg_argv[i]);
        }
    }

    return ERRCODE_SUCC;
}

/* these commands can be used in end_user version */
static const at_cmd_table_t g_bt_manager_cmd[] = {
/* 无参数无特殊处理AT命令 */
    { "AT^BTSPPREG",              parse_no_para_handle,         "36 5A 00 03 00 09 00 9A 20" },
    { "AT^BTENABLE",              parse_no_para_handle,         "36 5A 00 03 00 09 01 9A 20" },
    { "AT^BTDISABLE",             parse_no_para_handle,         "36 5A 00 03 00 09 02 9A 20" },
    { "AT^BTADDR?",               parse_no_para_handle,         "36 5A 00 03 00 09 05 BA 62" },
    { "AT^BTNAME?",               parse_no_para_handle,         "36 5A 00 03 00 09 07 9A 20" },
    { "AT^BTINQSCANOPEN",         parse_no_para_handle,         "36 5A 00 03 00 09 08 59 BC" },
    { "AT^BTINQOPEN",             parse_no_para_handle,         "36 5A 00 03 00 09 0B A2 89" },
    { "AT^BTINQCLOSE",            parse_no_para_handle,         "36 5A 00 03 00 09 0C B2 A8" },
    { "AT^BTINQSCANCLOSE",        parse_no_para_handle,         "36 5A 00 03 00 09 09 E8 D7" },
    { "AT^BTPSCANOPEN",           parse_no_para_handle,         "36 5A 00 03 00 09 08 59 BC" },
    { "AT^BTPSCANCLOSE",          parse_no_para_handle,         "36 5A 00 03 00 09 09 E8 D7" },
    { "AT^BTINQINFO?",            parse_no_para_handle,         "36 5A 00 03 00 09 0D A2 89" },
    { "AT^BTCONNINFO?",           parse_no_para_handle,         "36 5A 00 03 00 09 0E A2 89" },
    { "AT^BTREMOTEINFO?",         parse_no_para_handle,         "36 5A 00 03 00 09 0F A2 89" },
    { "AT^BTPAIRNUM?",            parse_no_para_handle,         "36 5A 00 03 00 09 10 A2 89" },
    { "AT^BTPAIRINFO?",           parse_no_para_handle,         "36 5A 00 03 00 09 11 A2 89" },
    { "AT^BTPAIRSTATE",           parse_para_addr_handle,       "36 5A 00 03 00 09 12 A2 89" },
    { "AT^BTHFPPICKUP",           parse_no_para_handle,         "36 5A 00 04 00 09 68 A2 89" },
    { "AT^BTHFPREJECT",           parse_no_para_handle,         "36 5A 00 04 00 09 69 A2 89" },
    { "AT^BTHFPHANGUP",           parse_no_para_handle,         "36 5A 00 04 00 09 6A A2 89" },
    { "AT^BTHFPHOLD",             parse_no_para_handle,         "36 5A 00 04 00 09 6B A2 89" },
    { "AT^BLEADVEN",              parse_no_para_handle,         "36 5A 00 03 00 09 33 28 6D" },
    { "AT^BLEADVDIS",             parse_no_para_handle,         "36 5A 00 03 00 09 34 28 6D" },
    { "AT^BLEENTERSCAN",          parse_para_set_value_handle,  "36 5A 00 09 00 09 35" },
    { "AT^FTYRESET",              parse_no_para_handle,         "36 5A 00 03 00 09 0A 00 00" },
    { "AT^BTLOGTYPELEVEL",        parse_para_set_value_handle,  "36 5A 00 03 00 09 8C 00 00" },

/* GATT server命令  */
    { "AT^BTSGATTCREATESERVER",         parse_no_para_handle,        "36 5A 00 03 00 09 13 00 00" },
    { "AT^BTSGATTCREATESERVICE",        parse_para_set_value_handle, "36 5A 00 09 00 09 14" },
    { "AT^BTSGATTADDINCLUDESERVICE",    parse_para_set_value_handle, "36 5A 00 09 00 09 15" },
    { "AT^BTSGATTADDCHARACTER",         parse_para_set_value_handle, "36 5A 00 09 00 09 16" },
    { "AT^BTSGATTADDDESC",              parse_para_set_value_handle, "36 5A 00 09 00 09 17" },
    { "AT^BTSGATTSTARTSERVICE",         parse_para_set_value_handle, "36 5A 00 09 00 09 18" },
    { "AT^BTSGATTSENDRESPONSE",         parse_para_set_value_handle, "36 5A 00 09 00 09 19" },
    { "AT^BTSGATTUPDATECHARACTERVALUE", parse_para_set_value_handle, "36 5A 00 09 00 09 1A" },
    { "AT^BTSGATTUPDATEDESCVALUE",      parse_para_set_value_handle, "36 5A 00 09 00 09 1B" },
    { "AT^BTSGATTGETGAPATTRIBUTE",      parse_para_set_value_handle, "36 5A 00 09 00 09 1C" },
    { "AT^BTSGATTSETGAPATTRIBUTE",      parse_para_set_value_handle, "36 5A 00 09 00 09 1D" },
    { "AT^BTSGATTMULTIPLEVALUENTF",     parse_para_set_value_handle, "36 5A 00 09 00 09 1E" },
    { "AT^BTSGATTMTUSETPARAM",          parse_para_set_value_handle, "36 5A 00 09 00 09 1F" },

/* GATT client命令 */
    { "AT^BTSGATTDISCOVERYSERVICE",     parse_para_set_value_handle, "36 5A 00 09 00 09 20" },
    { "AT^BTSGATTDISCOVERYINCLUDE",     parse_para_set_value_handle, "36 5A 00 09 00 09 21" },
    { "AT^BTSGATTDISCOVERYCHARACTER",   parse_para_set_value_handle, "36 5A 00 09 00 09 22" },
    { "AT^BTSGATTDISCHARBYUUID",        parse_para_set_value_handle, "36 5A 00 09 00 09 23" },
    { "AT^BTSGATTDISCOVERYDESC",        parse_para_set_value_handle, "36 5A 00 09 00 09 24" },
    { "AT^BTSGATTDISDESCBYHANDLE",      parse_para_set_value_handle, "36 5A 00 09 00 09 25" },
    { "AT^BTSGATTREADCHARACTER",        parse_para_set_value_handle, "36 5A 00 09 00 09 26" },
    { "AT^BTSGATTREADCHARBYUUID",       parse_para_set_value_handle, "36 5A 00 09 00 09 27" },
    { "AT^BTSGATTREADMULTIPLECHAR",     parse_para_set_value_handle, "36 5A 00 09 00 09 28" },
    { "AT^BTSGATTMULTVALUEREADCHAR",    parse_para_set_value_handle, "36 5A 00 09 00 09 29" },
    { "AT^BTSGATTWRITECHARACTER",       parse_para_set_value_handle, "36 5A 00 09 00 09 2A" },
    { "AT^BTSGATTRELIABLEWRITECHAR",    parse_para_set_value_handle, "36 5A 00 09 00 09 2B" },
    { "AT^BTSGATTREADDESC",             parse_para_set_value_handle, "36 5A 00 09 00 09 2C" },
    { "AT^BTSGATTWRITEDESC",            parse_para_set_value_handle, "36 5A 00 09 00 09 2D" },
    { "AT^BTSGATTMTUCONFIG",            parse_para_set_value_handle, "36 5A 00 09 00 09 2E" },
    { "AT^BTSGATTVALUENOTIFICATION",    parse_para_set_value_handle, "36 5A 00 09 00 09 2F" },
    { "AT^BTSGATTGETHIDDEVICERPT",      parse_para_set_value_handle, "36 5A 00 09 00 09 30" },
    { "AT^BTSGATTSETATTMINKEYSIZE",     parse_para_set_value_handle, "36 5A 00 09 00 09 31" },
    { "AT^BTSGATTSETATTCONFIGPARAM",    parse_para_set_value_handle, "36 5A 00 09 00 09 32" },

/* 有固定参数处理AT命令 */
    { "AT^BTADDR",                parse_para_addr_handle,             "36 5A 00 09 00 09 04" },
#ifndef MEMORY_MINI
    { "AT^BTCONNADDR",            parse_para_addr_handle,             "36 5A 00 09 00 09 36" },
#endif
    { "AT^CANCALPAIR",            parse_para_addr_handle,             "36 5A 00 09 00 09 37" },
    { "AT^BTISCONN?",             parse_para_addr_handle,             "36 5A 00 09 00 09 38" },
    { "AT^BTDISCONN",             parse_para_addr_handle,             "36 5A 00 09 00 09 39" },
    { "AT^SETSPKVOL",             parse_para_set_high_power_handle,   "36 5A 00 0A 00 09 65" },
    { "AT^SETMICVOL",             parse_para_set_high_power_handle,   "36 5A 00 0A 00 09 66" },
    { "AT^BTHFPDAIL",             parse_para_call_num_handle,         "36 5A 00 0A 00 09 67" },
/* 无固定参数处理AT命令 */
    { "AT^BTNAME",                parse_para_set_name_handle,         "36 5A 00 09 00 09 06" },
    { "AT^BTTESTCMD",             parse_para_bt_test_handle,          "" },

/* AVRCP BTSRV_WSTP_AT_AVRCP_REG = 0x8A10, */
    { "AT^AVRCPREG",                parse_no_para_handle,         "36 5A 00 09 00 09 42" },
    { "AT^AVRCPDEREG",              parse_no_para_handle,         "36 5A 00 09 00 09 43" },
    { "AT^AVRCPTGCONN",             parse_para_addr_handle,       "36 5A 00 09 00 09 44" },
    { "AT^AVRCPTGDISCONN",          parse_para_addr_handle,       "36 5A 00 09 00 09 45" },
    { "AT^AVRCPTGGETCONNSTAT",      parse_para_addr_handle,       "36 5A 00 09 00 09 46" },
    { "AT^AVRCPTGNOTIFYSTAT",       parse_para_set_value_handle,  "36 5A 00 09 00 09 47" },
    { "AT^AVRCPTGSETLOCINF",        parse_para_set_value_handle,  "36 5A 00 09 00 09 48" },
    { "AT^AVRCPCTCONN",             parse_para_addr_handle,       "36 5A 00 09 00 09 49" },
    { "AT^AVRCPCTDISCONN",          parse_para_addr_handle,       "36 5A 00 09 00 09 4A" },
    { "AT^AVRCPCTPRESSBUTTON",      parse_para_set_name_handle,   "36 5A 00 09 00 09 4B" },
    { "AT^AVRCPCTSETVOLUME",        parse_para_set_name_handle,   "36 5A 00 09 00 09 4C" },
    { "AT^AVRCPCTGETTGPLAYSTAT",    parse_para_addr_handle,       "36 5A 00 09 00 09 4D" },
    { "AT^AVRCPCTGETELEMTATTRI",    parse_para_addr_handle,       "36 5A 00 09 00 09 4E" },
    { "AT^AVRCPCTGETSUPPORTEVENT",  parse_para_addr_handle,       "36 5A 00 09 00 09 4F" },
    { "AT^AVRCPCTREGSUPPORTEVENT",  parse_para_set_name_handle,   "36 5A 00 09 00 09 50" },
    { "AT^AVRCPCTGETCONNSTAT",      parse_para_addr_handle,       "36 5A 00 09 00 09 51" },
    { "AT^AVRCPCTGETSUPPORTCOMPANY",parse_para_addr_handle,       "36 5A 00 09 00 09 52" },

/* AVRCP BTSRV_WSTP_AT_A2DP_SRC_REG = 0x8A20, */
    { "AT^A2DPREG",                 parse_no_para_handle,         "36 5A 00 09 00 09 53" },
    { "AT^A2DPDEREG",               parse_no_para_handle,         "36 5A 00 09 00 09 54" },
#ifndef MEMORY_MINI
    { "AT^A2DPGETCONNSTAT",         parse_para_addr_handle,       "36 5A 00 09 00 09 55" },
    { "AT^A2DPGETPLAYSTAT",         parse_para_addr_handle,       "36 5A 00 09 00 09 56" },
    { "AT^A2DPCONN",                parse_para_addr_handle,       "36 5A 00 09 00 09 57" },
    { "AT^A2DPDISCONN",             parse_para_addr_handle,       "36 5A 00 09 00 09 58" },
    { "AT^A2DPGETACTIVEDEV",        parse_no_para_handle,         "36 5A 00 09 00 09 59" },
    { "AT^A2DPPLAYSTART",           parse_para_addr_handle,       "36 5A 00 09 00 09 5A" },
    { "AT^A2DPPLAYSUSPEND",         parse_para_addr_handle,       "36 5A 00 09 00 09 5B" },
    { "AT^A2DPPLAYSTOP",            parse_para_addr_handle,       "36 5A 00 09 00 09 5C" },
#endif
    { "AT^A2DPSNKREG",              parse_no_para_handle,         "36 5A 00 09 00 09 8E" },
    { "AT^A2DPSNKDEREG",            parse_no_para_handle,         "36 5A 00 09 00 09 8F" },
#ifndef MEMORY_MINI
    { "AT^A2DPSNKGETCONNSTAT",      parse_para_addr_handle,       "36 5A 00 09 00 09 90" },
    { "AT^A2DPSNKGETPLAYSTAT",      parse_para_addr_handle,       "36 5A 00 09 00 09 91" },
    { "AT^A2DPSNKCONN",             parse_para_addr_handle,       "36 5A 00 09 00 09 92" },
    { "AT^A2DPSNKDISCONN",          parse_para_addr_handle,       "36 5A 00 09 00 09 93" },
    { "AT^A2DPSNKGETACTIVEDEV",     parse_no_para_handle,         "36 5A 00 09 00 09 94" },
    { "AT^A2DPSNKPLAYSTART",        parse_para_addr_handle,       "36 5A 00 09 00 09 95" },
    { "AT^A2DPSNKPLAYSUSPEND",      parse_para_addr_handle,       "36 5A 00 09 00 09 96" },
    { "AT^A2DPSNKPLAYSTOP",         parse_para_addr_handle,       "36 5A 00 09 00 09 97" },
#endif
    { "AT^BTSGATTCBKREG",           parse_no_para_handle,         "36 5A 00 09 00 09 5D" },
/* PBAP BTSRV_WSTP_AT_PBAP_REG = 0x8A00, */
    { "AT^PBAPREG",                 parse_no_para_handle,         "36 5A 00 09 00 09 3A" },
    { "AT^PBAPCONN",                parse_para_addr_handle,       "36 5A 00 09 00 09 3B" },
    { "AT^PBAPDISCONN",             parse_para_addr_handle,       "36 5A 00 09 00 09 3C" },
    { "AT^PBAPPULLPB",              parse_para_set_name_handle,   "36 5A 00 09 00 09 3D" },
    { "AT^PBAPPULLVCL",             parse_para_set_name_handle,   "36 5A 00 09 00 09 3E" },
    { "AT^PBAPPULLVCE",             parse_para_set_name_handle,   "36 5A 00 09 00 09 3F" },
    { "AT^PBAPSTOPTRANS",           parse_no_para_handle,         "36 5A 00 09 00 09 40" },
    { "AT^PBAPGETCONNSTATE",        parse_no_para_handle,         "36 5A 00 09 00 09 41" },
/* HFP BTSRV_WSTP_AT_HFP_REG = 0x1303, */
    { "AT^HFPREG",                  parse_no_para_handle,         "36 5A 00 03 00 09 5E 9A 20" },
    { "AT^HFPCONN",                 parse_para_addr_handle,       "36 5A 00 09 00 09 5F" },
    { "AT^HFPDISCONN",              parse_para_addr_handle,       "36 5A 00 09 00 09 60" },
    { "AT^HFPCONNSCO",              parse_para_addr_handle,       "36 5A 00 09 00 09 61" },
    { "AT^HFPDISCONNSCO",           parse_para_addr_handle,       "36 5A 00 09 00 09 62" },
    { "AT^HFPGETCONNSTATE",         parse_para_addr_handle,       "36 5A 00 09 00 09 63" },
    { "AT^HFPGETSCOCONNSTATE",      parse_para_addr_handle,       "36 5A 00 09 00 09 64" },
/* SPP */
    { "AT^SPPSVRCREATE",            parse_para_set_name_handle,   "36 5A 00 09 00 09 6C" },
    { "AT^SPPSVRACCEPT",            parse_para_set_name_handle,   "36 5A 00 09 00 09 6D" },
    { "AT^SPPSVRCLOSE",             parse_para_set_name_handle,   "36 5A 00 09 00 09 6E" },
    { "AT^SPPCONNECT",              parse_para_set_name_handle,   "36 5A 00 09 00 09 6F" },
    { "AT^SPPDISCONNECT",           parse_para_set_name_handle,   "36 5A 00 09 00 09 70" },
    { "AT^SPPISCONNECT?",           parse_para_set_name_handle,   "36 5A 00 09 00 09 71" },
    { "AT^SPPGETRMTADDR",           parse_para_set_name_handle,   "36 5A 00 09 00 09 72" },
    { "AT^SPPREADDATA",             parse_para_set_name_handle,   "36 5A 00 09 00 09 73" },
    { "AT^SPPWRITEDATA",            parse_para_set_name_handle,   "36 5A 00 09 00 09 74" },
    { "AT^SPPREADFILE",             parse_para_set_name_handle,   "36 5A 00 09 00 09 75" },
    { "AT^SPPWRITEFILE",            parse_para_set_name_handle,   "36 5A 00 09 00 09 76" },
    { "AT^SPPTESTRCVDELAY",         parse_para_set_name_handle,   "36 5A 00 09 00 09 77" },
    { "AT^SPPTESTSENDDELAY",        parse_para_set_name_handle,   "36 5A 00 09 00 09 78" },
/* DFT */
    { "AT^BTDFTREG",                parse_no_para_handle,         "36 5A 00 09 00 09 79" },
    { "AT^BTSETTXPOWER",            parse_para_set_name_handle,   "36 5A 00 09 00 09 7A" },
    { "AT^BTDFTSETDUTMODE",         parse_para_set_name_handle,   "36 5A 00 09 00 09 7B" },
    { "AT^BTLO",                    parse_para_set_name_handle,   "36 5A 00 09 00 09 7C" },
    { "AT^BTDUTMODE",               parse_para_set_name_handle,   "36 5A 00 09 00 09 7D" },
    { "AT^BTTX",                    parse_para_set_name_handle,   "36 5A 00 09 00 09 7E" },
    { "AT^BTRX",                    parse_para_set_name_handle,   "36 5A 00 09 00 09 7F" },
    { "AT^BLETX",                   parse_para_set_name_handle,   "36 5A 00 09 00 09 80" },
    { "AT^BLERX",                   parse_para_set_name_handle,   "36 5A 00 09 00 09 81" },
    { "AT^BTDFTRESETRXCOUNT",       parse_no_para_handle,         "36 5A 00 09 00 09 82" },
    { "AT^BTDFTREADRXCOUNT",        parse_no_para_handle,         "36 5A 00 09 00 09 83" },
#ifdef FT_DONGLE
    { "AT^CTBTTX",                  parse_para_set_name_handle,   "36 5A 00 09 00 09 84" },
    { "AT^CTBTRX",                  parse_para_set_name_handle,   "36 5A 00 09 00 09 85" },
    { "AT^CTBLETX",                 parse_para_set_name_handle,   "36 5A 00 09 00 09 86" },
    { "AT^CTBLERX",                 parse_para_set_name_handle,   "36 5A 00 09 00 09 87" },
    { "AT^BTSETCTDELAY",            parse_para_set_name_handle,   "36 5A 00 09 00 09 88" },
    { "AT^BTSETDONGLEADDR",         parse_para_set_name_handle,   "36 5A 00 09 00 09 89" },
#endif
    { "AT^BTPWR",                   parse_para_set_name_handle,   "36 5A 00 09 00 09 8A" },
    { "AT^BTLINKTYPE?",             parse_no_para_handle,         "36 5A 00 03 00 09 8B A2 89" },
    { "AT^BLECONNLIST?",            parse_no_para_handle,         "36 5A 00 03 00 09 8D A2 89" },
};

at_cmd_table_t *get_bt_manager_cmd_table(uint32_t *cmd_count)
{
    *cmd_count = ARRAY_COUNT(g_bt_manager_cmd);
    return (at_cmd_table_t *)g_bt_manager_cmd;
}