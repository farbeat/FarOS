/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: This module include test function of BT PBAP Service module.
 */

#include "sample_pbap_service.h"
#include <soc_osal.h>
#include <td_base.h>
#include "securec.h"
#include "errcode.h"
#include "bts_br_gap.h"
#include "bts_pbap.h"
#include "sample_common.h"
#include "debug_print.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define SAMPLE_PBAP_TASK_STACK_SIZE 0x400
#define PBAP_APP_FOLDER_MAXLENGTH   40
static bd_addr_t g_pbap_curr_addr;

typedef enum {
    PBAP_VCF_PB = 0,
    PBAP_VCF_ICH,
    PBAP_VCF_OCH,
    PBAP_VCF_MCH,
    PBAP_VCF_CCH,
    PBAP_VCF_SPD,
    PBAP_VCF_FAV,
} pbap_vcf_files;

static void sample_pbap_print_addr(const bd_addr_t *bd_addr)
{
    if (bd_addr != NULL) {
        bts_sample_log("addr: ****%02x%02x%02x%02x ",
            bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0]); /* addr 下标 3 2 1 0 */
    }
}

static void sample_pbap_conn_state(const bd_addr_t *bd_addr, profile_connect_state_t state)
{
    sample_pbap_print_addr(bd_addr);
    bts_sample_log("%d\n", state);
    return;
}

static void sample_pbap_receive_data(const bd_addr_t *bd_addr, const uint8_t* data, int32_t len)
{
    sample_pbap_print_addr(bd_addr);
    bts_sample_log("receive %d bytes(%s)\n", len, data);
    return;
}

static void sample_pbap_transfer_complete(const bd_addr_t *bd_addr)
{
    sample_pbap_print_addr(bd_addr);
    bts_sample_log("\n");
    return;
}

static void sample_pbap_get_phonebook_size(const bd_addr_t *bd_addr, uint16_t size)
{
    sample_pbap_print_addr(bd_addr);
    bts_sample_log("%u\n", size);
    return;
}

typedef struct {
    uint32_t bts_code;
    td_char print_message[20]; /* 字符串长度20 */
} sample_pbap_code_print;

static sample_pbap_code_print g_pbap_error_pair[] = {
    {PBAP_ERROR_OK, "OK"},
    {PBAP_ERROR_BAD_REQUEST, "BAD REQUEST"},
    {PBAP_ERROR_NOT_IMPLEMENTED, "NOT IMPLEMENTED"},
    {PBAP_ERROR_UNAUTHORIZED, "UNAUTHORIZED"},
    {PBAP_ERROR_PRECONDITION_FAILED, "PRECONDITION FAILED"},
    {PBAP_ERROR_NOT_FOUND, "NOT FOUND"},
    {PBAP_ERROR_NOT_ACCEPTABLE, "NOT ACCEPTABLE"},
    {PBAP_ERROR_SERVICE_UNAVAILABLE, "SERVICE UNAVAILABLE"},
    {PBAP_ERROR_FORBIDDEN, "FORBIDDEN"},
};

static td_char* sample_pbap_print_error(uint32_t code)
{
    for (uint32_t i = 0; i < sizeof(g_pbap_error_pair) / sizeof(g_pbap_error_pair[0]); i++) {
        if (code == g_pbap_error_pair[i].bts_code) {
            return g_pbap_error_pair[i].print_message;
        }
    }
    return NULL;
}

static void sample_pbap_handle_error_code(const bd_addr_t *bd_addr, int32_t code)
{
    sample_pbap_print_addr(bd_addr);
    bts_sample_log("0x%x %s\n", code, sample_pbap_print_error((uint32_t)code));
}

static bool sample_pbap_is_null_pointer(uint16_t sample_mode, uint8_t index)
{
    return ((sample_mode & (1 << index)) != 0);
}

/* 蓝牙电话簿注册回调 */
void wstp_pbap_pce_reg_callback(void)
{
    errcode_t ret = ERRCODE_BT_SUCCESS;
    pbap_pce_callbacks_t callbacks = {
        sample_pbap_conn_state,
        sample_pbap_get_phonebook_size,
        sample_pbap_transfer_complete,
        sample_pbap_receive_data,
        sample_pbap_handle_error_code,
    };
    ret = (errcode_t)pbap_pce_register_callbacks(&callbacks);
    bts_sample_log("ret=0x%x\n", ret);
}

static void sample_pbap_cmd_reg_callback(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bool fill_callbacks = atoi(params[1]);
    uint32_t ret = ERRCODE_BT_SUCCESS;
    if (sample_pbap_is_null_pointer((uint16_t)sample_mode, 0)) {
        ret = (uint32_t)pbap_pce_register_callbacks(NULL);
        return;
    }
    if (fill_callbacks != 0) {
        pbap_pce_callbacks_t callbacks = {
            sample_pbap_conn_state,
            sample_pbap_get_phonebook_size,
            sample_pbap_transfer_complete,
            sample_pbap_receive_data,
            sample_pbap_handle_error_code,
        };
        ret = (uint32_t)pbap_pce_register_callbacks(&callbacks);
    } else {
        pbap_pce_callbacks_t callbacks = { 0 };
        ret = (uint32_t)pbap_pce_register_callbacks(&callbacks);
    }
    bts_sample_log("ret: 0x%x\n", ret);
}

static bd_addr_t* sample_pbap_parse_addr(const char* addr_str)
{
    if (decode_bd_addr_from_str(addr_str, g_pbap_curr_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("decode addr error!\n");
        return NULL;
    }
    return &g_pbap_curr_addr;
}

static int32_t sample_pbap_connect_task_body(void *bd_addr)
{
    uint32_t result = (uint32_t)pbap_pce_connect((bd_addr_t*)bd_addr);
    if (result != ERRCODE_BT_SUCCESS) {
        bts_sample_log("Connect to PBAP service failed\n");
    }
    bts_sample_log("[Line: %d] finish, result: 0x%x!\n", __LINE__, result);
    return 0;
}

static void sample_pbap_create_conn_task(const bd_addr_t *bd_addr)
{
    osal_task *task = osal_kthread_create(sample_pbap_connect_task_body, (void *)bd_addr, "sample_pbap_connect_task",
        SAMPLE_PBAP_TASK_STACK_SIZE);
    if (task == NULL) {
        bts_sample_log("create task fail\n");
        return;
    }
}

/* start a pbap client */
static void sample_pbap_cmd_start_client(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_pbap_is_null_pointer((uint16_t)sample_mode, 0)) {
        bd_addr = sample_pbap_parse_addr(params[1]);
    }
    sample_pbap_create_conn_task(bd_addr);
}

/* stop a pbap client */
static void sample_pbap_cmd_stop_client(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_pbap_is_null_pointer((uint16_t)sample_mode, 0)) {
        bd_addr = sample_pbap_parse_addr(params[1]);
    }

    uint32_t ret = (uint32_t)pbap_pce_disconnect(bd_addr);
    bts_sample_log("[Line: %d] finish, ret: %u\n", __LINE__, ret);
}

static int32_t sample_pbap_set_folder_vcf(uint8_t* name, uint8_t vcf, bool is_pull_phone_book)
{
    if (name == NULL) {
        return -1;
    }
    switch (vcf) {
        case PBAP_VCF_PB: /* Main Phone book */
            (void)strcat_s((char *)name, PBAP_APP_FOLDER_MAXLENGTH, "pb");
            break;
        case PBAP_VCF_ICH: /* Incoming Calls History */
            (void)strcat_s((char *)name, PBAP_APP_FOLDER_MAXLENGTH, "ich");
            break;
        case PBAP_VCF_OCH: /* Outgoing Calls History */
            (void)strcat_s((char *)name, PBAP_APP_FOLDER_MAXLENGTH, "och");
            break;
        case PBAP_VCF_MCH: /* Missed Calls History */
            (void)strcat_s((char *)name, PBAP_APP_FOLDER_MAXLENGTH, "mch");
            break;
        case PBAP_VCF_CCH: /* Combined Calls History */
            (void)strcat_s((char *)name, PBAP_APP_FOLDER_MAXLENGTH, "cch");
            break;
        case PBAP_VCF_SPD: /* Speed Dial */
            (void)strcat_s((char *)name, PBAP_APP_FOLDER_MAXLENGTH, "spd");
            break;
        case PBAP_VCF_FAV: /* Favorite Contacts */
            (void)strcat_s((char *)name, PBAP_APP_FOLDER_MAXLENGTH, "fav");
            break;
        default:
            bts_sample_log("no vcf file be seleted.\n");
            return -1;
    }
    if (is_pull_phone_book) {
        (void)strcat_s((char *)name, PBAP_APP_FOLDER_MAXLENGTH, ".vcf");
    }
    return 0;
}

static void sample_pbap_filter_composer(uint8_t filter[PBAP_MASK_LEN], uint8_t flag)
{
    uint8_t *temp = NULL;
    uint8_t bytes = 0;
    uint8_t bits = 0;

    if (filter && (flag < 64)) { /* flag最大值64 */
        bytes = (uint8_t)(flag >> 3); /* 右移3位判断填入哪个字节 */
        bits = (uint8_t)(flag & 0x07); /* 求余数判断填入哪一位 */
        temp = filter + PBAP_MASK_LEN - 1 - bytes;
        *temp |= (1 << bits);
    }
}

static void sample_pbap_set_property(uint8_t pro_arr[PBAP_MASK_LEN], uint32_t property)
{
    uint8_t cur_bit = 0;
    uint32_t temp = property;
    while (temp != 0) {
        if ((temp & 0x01) != 0) {
            sample_pbap_filter_composer(pro_arr, cur_bit);
        }
        temp = temp >> 1;
        cur_bit++;
    }
}

static bool sample_pbap_set_pull_phone_book_name(uint8_t folder, uint8_t vcf, uint8_t* name)
{
    if (name == NULL) {
        bts_sample_log("empty name.\n");
        return false;
    }
    if (folder == PBAP_REPOSITORY_LOCAL) {
        (void)strcpy_s((char *)name, PBAP_APP_FOLDER_MAXLENGTH, "telecom/");
    } else if (folder == PBAP_REPOSITORY_SIM) {
        (void)strcpy_s((char *)name, PBAP_APP_FOLDER_MAXLENGTH, "SIM1/telecom/");
    } else {
        bts_sample_log("no folder be seleted.\n");
        return false;
    }

    if (sample_pbap_set_folder_vcf(name, vcf, true) != 0) {
        return false;
    }
    return true;
}

static bool wstp_pbap_check_param_valid(
    uint8_t folder, uint8_t vcf, uint16_t max_list_count, uint16_t list_offset, uint8_t format)
{
    bts_unused(max_list_count);
    bts_unused(list_offset);
    bool res = true;
    if (folder == 1) { /* folder 1 */
        if (vcf > 6) { /* vcf num 6 */
            res = false;
        }
    } else if (folder == 2) { /* folder 2 */
        if (vcf > 4) { /* vcf num 4 */
            res = false;
        }
    } else {
        res = false;
    }

    if (format > 1) {
        res = false;
    }

    return res;
}

/* get phone book */
void wstp_pbap_cmd_pull_phonebook(
    uint8_t folder, uint8_t vcf, uint16_t max_list_count, uint16_t list_offset, uint8_t format)
{
    if (!wstp_pbap_check_param_valid(folder, vcf, max_list_count, list_offset, format)) {
        bts_sample_log("pbap param invalid\n");
        return;
    }

    pbap_pce_pull_phone_book_param_t *thr_par =
        (pbap_pce_pull_phone_book_param_t*)(void *)bts_sample_mem_new(sizeof(pbap_pce_pull_phone_book_param_t));
    if (thr_par == NULL) {
        bts_sample_log("pbap param malloc failed\n");
        return;
    }
    (void)memset_s(thr_par, sizeof(pbap_pce_pull_phone_book_param_t), 0, sizeof(pbap_pce_pull_phone_book_param_t));
    thr_par->name = (unsigned char *)(void *)bts_sample_mem_new(PBAP_APP_FOLDER_MAXLENGTH);
    if (thr_par->name == NULL) {
        bts_sample_log("pbap name malloc failed\n");
        bts_sample_mem_free(thr_par);
        return;
    }
    (void)memset_s(thr_par->name, PBAP_APP_FOLDER_MAXLENGTH, 0, PBAP_APP_FOLDER_MAXLENGTH);
    /* 为0时代表查询对应记录大小 */
    thr_par->max_list_count = max_list_count;
    thr_par->list_offset = list_offset;
    thr_par->format = format;
    uint32_t property = 0;
    sample_pbap_set_property(thr_par->property_selector, property);
    uint8_t filter = 0;
    if (filter != 0) {
        sample_pbap_filter_composer(thr_par->property_selector, PBAP_PROPERTY_MASK_FILTER);
    }

    if (sample_pbap_set_pull_phone_book_name(folder, vcf, thr_par->name)) {
        bts_sample_log("pbap_pce_pull_phone_book name=%s\n", thr_par->name);
        pbap_pce_pull_phone_book(NULL, thr_par);
    }
    bts_sample_mem_free(thr_par->name);
    bts_sample_mem_free(thr_par);
}

/* get phone book */
static void sample_pbap_cmd_pull_phonebook(const char **params, int32_t params_cnt)
{
    if (params_cnt != 9) { /* 9个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_pbap_is_null_pointer((uint16_t)sample_mode, 0)) {
        bd_addr = sample_pbap_parse_addr(params[1]);
    }

    if (pbap_pce_get_device_connect_state(bd_addr) != PROFILE_STATE_CONNECTED) {
        bts_sample_log("No PBAP connection selected!\n");
        return;
    }
    pbap_pce_pull_phone_book_param_t *thr_par =
        (pbap_pce_pull_phone_book_param_t*)(void *)bts_sample_mem_new(sizeof(pbap_pce_pull_phone_book_param_t));
    if (thr_par == NULL) {
        bts_sample_log("pbap param malloc failed\n");
        return;
    }
    (void)memset_s(thr_par, sizeof(pbap_pce_pull_phone_book_param_t), 0, sizeof(pbap_pce_pull_phone_book_param_t));
    thr_par->name = (unsigned char *)(void *)bts_sample_mem_new(PBAP_APP_FOLDER_MAXLENGTH);
    if (thr_par->name == NULL) {
        bts_sample_log("pbap name malloc failed\n");
        bts_sample_mem_free(thr_par);
        return;
    }
    (void)memset_s(thr_par->name, PBAP_APP_FOLDER_MAXLENGTH, 0, PBAP_APP_FOLDER_MAXLENGTH);
    thr_par->max_list_count = (unsigned short)atoi(params[4]); /* index = 4 */
    thr_par->list_offset = (unsigned short)atoi(params[5]); /* index = 5 */
    thr_par->format = (unsigned char)atoi(params[6]); /* index = 6 */
    uint32_t property = (uint32_t)atoi(params[7]); /* index = 7 */
    sample_pbap_set_property(thr_par->property_selector, property);
    int filter = atoi(params[8]); /* index = 8 */
    if (filter != 0) {
        sample_pbap_filter_composer(thr_par->property_selector, PBAP_PROPERTY_MASK_FILTER);
    }

    uint8_t folder = (uint8_t)atoi(params[2]); /* index = 2 */
    uint8_t vcf = (uint8_t)atoi(params[3]); /* index = 3 */
    if (sample_pbap_set_pull_phone_book_name(folder, vcf, thr_par->name)) {
        pbap_pce_pull_phone_book(bd_addr, thr_par);
    }
    bts_sample_mem_free(thr_par->name);
    bts_sample_mem_free(thr_par);
}

static bool sample_pbap_set_vcard_listing_name(bd_addr_t* bd_addr, uint8_t folder, uint8_t vcf, uint8_t* name)
{
    bts_sample_log("folder=%u vcf=%u\n", folder, vcf);
    uint8_t tel_name[8] = "telecom";
    uint8_t sim_name[5] = "SIM1";
    if (name == NULL) {
        bts_sample_log("empty name.\n");
        return false;
    }
    uint32_t ret = (uint32_t)pbap_pce_set_phone_book(bd_addr, NULL);
    if (ret != ERRCODE_BT_SUCCESS) {
        bts_sample_log("0x%x %s\n", ret, sample_pbap_print_error(ret));
        return false;
    }
    if (folder == PBAP_REPOSITORY_LOCAL) {
        ret = (uint32_t)pbap_pce_set_phone_book(bd_addr, tel_name);
        if (ret != ERRCODE_BT_SUCCESS) {
            bts_sample_log("0x%x %s\n", ret, sample_pbap_print_error(ret));
            return false;
        }
    } else if (folder == PBAP_REPOSITORY_SIM) {
        ret = (uint32_t)pbap_pce_set_phone_book(bd_addr, sim_name);
        if (ret != ERRCODE_BT_SUCCESS) {
            bts_sample_log("0x%x %s\n", ret, sample_pbap_print_error(ret));
            return false;
        }
        ret = (uint32_t)pbap_pce_set_phone_book(bd_addr, tel_name);
        if (ret != ERRCODE_BT_SUCCESS) {
            bts_sample_log("0x%x %s\n", ret, sample_pbap_print_error(ret));
            return false;
        }
    } else {
        bts_sample_log("no folder be seleted.\n");
        return false;
    }
    if (sample_pbap_set_folder_vcf(name, vcf, false)) {
        return false;
    }
    return true;
}

/* get phone vCard-Listing */
void wstp_pbap_cmd_pull_vcard_listing(const char **params, int32_t params_cnt)
{
    if (params_cnt != 5 && params_cnt != 7) { /* 5个参数不带过滤，7个参数带过滤 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }
    uint8_t folder = (uint8_t)atoi(params[0]); /* index = 0 */
    uint8_t vcf = (uint8_t)atoi(params[1]); /* index = 1 */
    uint16_t max_list_count = (uint16_t)atoi(params[2]); /* index = 2 */
    uint16_t list_offset = (uint16_t)atoi(params[3]); /* index = 3 */
    if (!wstp_pbap_check_param_valid(folder, vcf, max_list_count, list_offset, 0)) {
        bts_sample_log("pbap param invalid\n");
        return;
    }

    bd_addr_t* bd_addr = NULL;

    pbap_pce_pull_vcard_listing_param_t *thr_par =
        (pbap_pce_pull_vcard_listing_param_t*)(void *)bts_sample_mem_new(sizeof(pbap_pce_pull_vcard_listing_param_t));
    if (thr_par == NULL) {
        bts_sample_log("pbap param malloc failed\n");
        return;
    }

    (void)memset_s(
        thr_par, sizeof(pbap_pce_pull_vcard_listing_param_t), 0, sizeof(pbap_pce_pull_vcard_listing_param_t));
    thr_par->name = (uint8_t *)(void *)bts_sample_mem_new(PBAP_APP_FOLDER_MAXLENGTH);
    if (thr_par->name == NULL) {
        bts_sample_log("pbap param name malloc failed\n");
        bts_sample_mem_free(thr_par);
        return;
    }
    (void)memset_s(thr_par->name, PBAP_APP_FOLDER_MAXLENGTH, 0, PBAP_APP_FOLDER_MAXLENGTH);
    thr_par->max_list_count = max_list_count;
    thr_par->list_offset = list_offset; /* index = 5 */
    /* 0:索引序 1:字典序 */
    thr_par->order = (uint8_t)atoi(params[4]); /* index = 4 */
    if (params_cnt == 7) { /* 7个参数带过滤 */
        thr_par->search_value = (unsigned char *)params[5]; /* index = 5 */
        thr_par->search_property = (uint8_t)atoi(params[6]); /* index = 6 */
    }
    bts_sample_log("folder=%d vcf=%d max_list_count=%d list_offset=%d order=%d search_value=%s property=%d\n",
        folder, vcf,  max_list_count, list_offset, thr_par->order, thr_par->search_value, thr_par->search_property);
    if (sample_pbap_set_vcard_listing_name(bd_addr, folder, vcf, thr_par->name)) {
        bts_sample_log("pbap_pce_pull_vcard_listing name=%s\n", thr_par->name);
        pbap_pce_pull_vcard_listing(bd_addr, thr_par);
    }
    bts_sample_mem_free(thr_par->name);
    bts_sample_mem_free(thr_par);
}

/* get phone vCard-Listing */
static void sample_pbap_cmd_pull_vcard_listing(const char **params, int32_t params_cnt)
{
    if (params_cnt != 7) { /* 7个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_pbap_is_null_pointer((uint16_t)sample_mode, 0)) {
        bd_addr = sample_pbap_parse_addr(params[1]);
    }

    if (pbap_pce_get_device_connect_state(bd_addr) != PROFILE_STATE_CONNECTED) {
        bts_sample_log("No PBAP connection selected!\n");
        return;
    }

    pbap_pce_pull_vcard_listing_param_t *thr_par =
        (pbap_pce_pull_vcard_listing_param_t*)(void *)bts_sample_mem_new(sizeof(pbap_pce_pull_vcard_listing_param_t));
    if (thr_par == NULL) {
        bts_sample_log("pbap param malloc failed\n");
        return;
    }

    (void)memset_s(
        thr_par, sizeof(pbap_pce_pull_vcard_listing_param_t), 0, sizeof(pbap_pce_pull_vcard_listing_param_t));
    thr_par->name = (uint8_t *)(void *)bts_sample_mem_new(PBAP_APP_FOLDER_MAXLENGTH);
    if (thr_par->name == NULL) {
        bts_sample_log("pbap param name malloc failed\n");
        bts_sample_mem_free(thr_par);
        return;
    }
    (void)memset_s(thr_par->name, PBAP_APP_FOLDER_MAXLENGTH, 0, PBAP_APP_FOLDER_MAXLENGTH);
    uint8_t folder = (uint8_t)atoi(params[2]); /* index = 2 */
    uint8_t vcf = (uint8_t)atoi(params[3]); /* index = 3 */
    thr_par->max_list_count = (unsigned short)atoi(params[4]); /* index = 4 */
    thr_par->list_offset = (unsigned short)atoi(params[5]); /* index = 5 */
    thr_par->order = (unsigned char)atoi(params[6]); /* index = 6 */

    if (sample_pbap_set_vcard_listing_name(bd_addr, folder, vcf, thr_par->name)) {
        pbap_pce_pull_vcard_listing(bd_addr, thr_par);
    }
    bts_sample_mem_free(thr_par->name);
    bts_sample_mem_free(thr_par);
}

static bool sample_pbap_set_vcard_entry_name(
    bd_addr_t *bd_addr, uint8_t folder, uint8_t vcf, const td_char *vcf_name, uint8_t *name)
{
    if (name == NULL) {
        bts_sample_log("empty name.\n");
        return false;
    }
    uint8_t tel_name[8] = "telecom";
    uint8_t sim_name[5] = "SIM1";
    uint32_t ret = (uint32_t)pbap_pce_set_phone_book(bd_addr, NULL);
    if (ret != ERRCODE_BT_SUCCESS) {
        bts_sample_log("0x%x %s\n", ret, sample_pbap_print_error(ret));
        return false;
    }
    if (folder == PBAP_REPOSITORY_LOCAL) {
        ret = (uint32_t)pbap_pce_set_phone_book(bd_addr, tel_name);
        if (ret != ERRCODE_BT_SUCCESS) {
            bts_sample_log("0x%x %s\n", ret, sample_pbap_print_error(ret));
            return false;
        }
    } else if (folder == PBAP_REPOSITORY_SIM) {
        ret = (uint32_t)pbap_pce_set_phone_book(bd_addr, sim_name);
        if (ret != ERRCODE_BT_SUCCESS) {
            bts_sample_log("0x%x %s\n", ret, sample_pbap_print_error(ret));
            return false;
        }
        ret = (uint32_t)pbap_pce_set_phone_book(bd_addr, tel_name);
        if (ret != ERRCODE_BT_SUCCESS) {
            bts_sample_log("0x%x %s\n", ret, sample_pbap_print_error(ret));
            return false;
        }
    } else {
        bts_sample_log("no folder be seleted.\n");
        return false;
    }
    if (sample_pbap_set_folder_vcf(name, vcf, false)) {
        return false;
    } else {
        ret = (uint32_t)pbap_pce_set_phone_book(bd_addr, name);
        if (ret != ERRCODE_BT_SUCCESS) {
            bts_sample_log("0x%x %s\n", ret, sample_pbap_print_error(ret));
            return false;
        }
    }

    if (strcpy_s((char *)name, PBAP_APP_FOLDER_MAXLENGTH, vcf_name) != EOK) {
        bts_sample_log("strcpy_s vcf_name fail\n");
        return false;
    }
    return true;
}

/* get phone vCard-Entry */
void wstp_pbap_cmd_pull_vcard_entry(uint8_t folder, uint8_t vcf, td_char *vcf_name, uint8_t format)
{
    bd_addr_t* bd_addr = NULL;

    if (!wstp_pbap_check_param_valid(folder, vcf, 0, 0, format)) {
        bts_sample_log("pbap param invalid\n");
        return;
    }

    pbap_pce_pull_vcard_entry_param_t *thr_par =
        (pbap_pce_pull_vcard_entry_param_t*)(void *)bts_sample_mem_new(sizeof(pbap_pce_pull_vcard_entry_param_t));
    if (thr_par == NULL) {
        bts_sample_log("pbap param malloc failed\n");
        return;
    }
    (void)memset_s(thr_par, sizeof(pbap_pce_pull_vcard_entry_param_t), 0, sizeof(pbap_pce_pull_vcard_entry_param_t));
    thr_par->name = (uint8_t*)(void *)bts_sample_mem_new(PBAP_APP_FOLDER_MAXLENGTH);
    if (thr_par->name == NULL) {
        bts_sample_log("pbap param name malloc failed\n");
        bts_sample_mem_free(thr_par);
        return;
    }
    (void)memset_s(thr_par->name, PBAP_APP_FOLDER_MAXLENGTH, 0, PBAP_APP_FOLDER_MAXLENGTH);
    thr_par->format = format; /* index = 5 */
    uint32_t property = 0; /* index = 6 */
    sample_pbap_set_property(thr_par->property_selector, property);
    uint8_t filter = 0; /* index = 7 */
    if (filter != 0) {
        sample_pbap_filter_composer(thr_par->property_selector, PBAP_PROPERTY_MASK_FILTER);
    }

    if (sample_pbap_set_vcard_entry_name(bd_addr, folder, vcf, vcf_name, thr_par->name)) {
        pbap_pce_pull_vcard_entry(bd_addr, thr_par);
    }
    bts_sample_mem_free(thr_par->name);
    bts_sample_mem_free(thr_par);
}

/* get phone vCard-Entry */
static void sample_pbap_cmd_pull_vcard_entry(const char **params, int32_t params_cnt)
{
    if (params_cnt != 8) { /* 8个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_pbap_is_null_pointer((uint16_t)sample_mode, 0)) {
        bd_addr = sample_pbap_parse_addr(params[1]);
    }

    if (pbap_pce_get_device_connect_state(bd_addr) != PROFILE_STATE_CONNECTED) {
        bts_sample_log("No PBAP connection selected!\n");
        return;
    }

    pbap_pce_pull_vcard_entry_param_t *thr_par =
        (pbap_pce_pull_vcard_entry_param_t*)(void *)bts_sample_mem_new(sizeof(pbap_pce_pull_vcard_entry_param_t));
    if (thr_par == NULL) {
        bts_sample_log("pbap param malloc failed\n");
        return;
    }
    (void)memset_s(thr_par, sizeof(pbap_pce_pull_vcard_entry_param_t), 0, sizeof(pbap_pce_pull_vcard_entry_param_t));
    thr_par->name = (uint8_t*)(void *)bts_sample_mem_new(PBAP_APP_FOLDER_MAXLENGTH);
    if (thr_par->name == NULL) {
        bts_sample_log("pbap param name malloc failed\n");
        bts_sample_mem_free(thr_par);
        return;
    }
    (void)memset_s(thr_par->name, PBAP_APP_FOLDER_MAXLENGTH, 0, PBAP_APP_FOLDER_MAXLENGTH);
    thr_par->format = (unsigned char)atoi(params[5]); /* index = 5 */
    uint32_t property = (uint32_t)atoi(params[6]); /* index = 6 */
    sample_pbap_set_property(thr_par->property_selector, property);
    int filter = atoi(params[7]); /* index = 7 */
    if (filter != 0) {
        sample_pbap_filter_composer(thr_par->property_selector, PBAP_PROPERTY_MASK_FILTER);
    }
    uint8_t folder = (uint8_t)atoi(params[2]); /* index = 2 */
    uint8_t vcf = (uint8_t)atoi(params[3]); /* index = 3 */
    const td_char* vcf_name = params[4]; /* index = 4 */

    if (sample_pbap_set_vcard_entry_name(bd_addr, folder, vcf, vcf_name, thr_par->name)) {
        pbap_pce_pull_vcard_entry(bd_addr, thr_par);
    }
    bts_sample_mem_free(thr_par->name);
    bts_sample_mem_free(thr_par);
}

/* pbap abort operation */
static void sample_pbap_cmd_stop_trans(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_pbap_is_null_pointer((uint16_t)sample_mode, 0)) {
        bd_addr = sample_pbap_parse_addr(params[1]);
    }

    if (pbap_pce_get_device_connect_state(bd_addr) != PROFILE_STATE_CONNECTED) {
        bts_sample_log("No PBAP connection selected!\n");
        return;
    }
    uint32_t result = (uint32_t)pbap_pce_stop_trans(bd_addr);
    bts_sample_log("[Line: %d] finish, ret: 0x%x\n", __LINE__, result);
}

/* pbap get connect status */
static void sample_pbap_amd_get_conn_state(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_pbap_is_null_pointer((uint16_t)sample_mode, 0)) {
        bd_addr = sample_pbap_parse_addr(params[1]);
    }

    uint32_t result = (uint32_t)pbap_pce_get_device_connect_state(bd_addr);
    bts_sample_log("[Line: %d] finish, conn state: 0x%x\n", __LINE__, result);
}

/* PBAP service test cmd id */
typedef enum {
    SAMPLE_PBAP_CMD_REG_CALLBACK,       /* 注册回调 */
    SAMPLE_PBAP_CMD_START_CLIENT,       /* 启动PBAP客户端 */
    SAMPLE_PBAP_CMD_STOP_CLIENT,        /* 关闭PBAP客户端 */
    SAMPLE_PBAP_CMD_PULL_PHONEBOOK,     /* 下载电话簿 */
    SAMPLE_PBAP_CMD_PULL_VCARD_LISTING, /* 下载vcard列表 */
    SAMPLE_PBAP_CMD_PULL_VCARD_ENTRY,   /* 下载vcard条目 */
    SAMPLE_PBAP_CMD_STOP_TRANS,         /* 中断传输 */
    SAMPLE_PBAP_CMD_GET_CONN_STATE,     /* 获取连接状态 */
} sample_pbap_cmd_id;

typedef void (*pbap_sample_cmd_handle_func)(const char **params, int32_t params_cnt);

typedef struct {
    sample_pbap_cmd_id id;
    pbap_sample_cmd_handle_func func;
} sample_pbap_hdl_info;

sample_pbap_hdl_info g_sample_pbap_hdls[] = {
    { SAMPLE_PBAP_CMD_REG_CALLBACK, sample_pbap_cmd_reg_callback },
    { SAMPLE_PBAP_CMD_START_CLIENT, sample_pbap_cmd_start_client },
    { SAMPLE_PBAP_CMD_STOP_CLIENT, sample_pbap_cmd_stop_client },
    { SAMPLE_PBAP_CMD_PULL_PHONEBOOK, sample_pbap_cmd_pull_phonebook },
    { SAMPLE_PBAP_CMD_PULL_VCARD_LISTING, sample_pbap_cmd_pull_vcard_listing },
    { SAMPLE_PBAP_CMD_PULL_VCARD_ENTRY, sample_pbap_cmd_pull_vcard_entry },
    { SAMPLE_PBAP_CMD_STOP_TRANS, sample_pbap_cmd_stop_trans },
    { SAMPLE_PBAP_CMD_GET_CONN_STATE, sample_pbap_amd_get_conn_state },
};

void bt_pbap_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    bts_sample_log("id: %u\n", cmd_id);
    uint32_t hdl_cnt = sizeof(g_sample_pbap_hdls) / sizeof(sample_pbap_hdl_info);
    for (uint32_t i = 0; i < hdl_cnt; i++) {
        if (g_sample_pbap_hdls[i].id == cmd_id) {
            g_sample_pbap_hdls[i].func(params, params_cnt);
            return;
        }
    }
    bts_sample_log("invalid cmd_id = %d \n", cmd_id);
}

#ifdef  __cplusplus
}
#endif
