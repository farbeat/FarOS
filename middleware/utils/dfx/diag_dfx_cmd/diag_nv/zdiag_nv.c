/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: software nv by zdiag
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "nv.h"
#include "nv_porting.h"
#include "diag.h"
#include "soc_diag_cmd_id.h"
#include "zdiag_adapt_layer.h"
#include "nv_reset.h"
#include "nv_nvregion.h"
#include "zdiag_nv.h"

STATIC void nv_set_done_callback_test(errcode_t result)
{
    printf("nv set done result = %x\r\n", result);
}

STATIC errcode_t diag_cmd_nv_read(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    unused(cmd_param_size);
    errcode_t res;
    nv_read_input_t *nv_read_input = (nv_read_input_t *)cmd_param;
    nv_read_output_t *nv_read_output = NULL;
    uint16_t klength = 0;
    nv_read_output = (nv_read_output_t *)dfx_malloc(0, sizeof(nv_read_output_t) + KVALUE_MAX_LENGTH);
    if (nv_read_output == NULL) {
        return ERRCODE_MALLOC;
    }
    nv_read_output->key = (uint16_t)nv_read_input->key;
    nv_read_output->ret =
        (uint16_t)uapi_nv_read(nv_read_output->key, KVALUE_MAX_LENGTH, &klength, nv_read_output->data);
    if (nv_read_output->ret != ERRCODE_SUCC) {
        dfx_free(0, nv_read_output);
        return nv_read_output->ret;
    }
    nv_read_output->length = (uint32_t)klength;
    res = uapi_diag_report_packet(cmd_id, option, (uint8_t *)nv_read_output,
        (uint16_t)(sizeof(nv_read_output_t) + nv_read_output->length), true);
    dfx_free(0, nv_read_output);
    return res;
}

STATIC errcode_t diag_cmd_nv_read_with_attr(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size,
                                            diag_option_t *option)
{
    unused(cmd_param_size);
    errcode_t res;
    nv_read_with_attr_output_t *nv_read_with_attr_output = NULL;
    nv_read_with_attr_output = (nv_read_with_attr_output_t *)dfx_malloc(0,
        sizeof(nv_read_with_attr_output_t) + KVALUE_MAX_LENGTH);
    if (nv_read_with_attr_output == NULL) {
        return ERRCODE_MALLOC;
    }
    (void)memset_s(nv_read_with_attr_output, sizeof(nv_read_with_attr_output_t) + KVALUE_MAX_LENGTH, 0,
        sizeof(nv_read_with_attr_output_t) + KVALUE_MAX_LENGTH);
    nv_read_with_attr_output->key = *(uint16_t *)cmd_param;
    res = uapi_nv_read_with_attr(nv_read_with_attr_output->key, KVALUE_MAX_LENGTH, &nv_read_with_attr_output->length,
        nv_read_with_attr_output->value, &nv_read_with_attr_output->attr);
    nv_read_with_attr_output->ret = res;
    if (res != ERRCODE_SUCC) {
        dfx_free(0, nv_read_with_attr_output);
        return res;
    }
    res = uapi_diag_report_packet(cmd_id, option, nv_read_with_attr_output,
        sizeof(nv_read_with_attr_output_t) + nv_read_with_attr_output->length, true);
    dfx_free(0, nv_read_with_attr_output);
    return res;
}

STATIC errcode_t diag_cmd_nv_write(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    unused(cmd_param_size);
    errcode_t res;
    nv_write_output_t nv_write_output = {0};
    nv_write_input_t *tmp = (nv_write_input_t *)cmd_param;
    nv_write_input_t *nv_write_input = NULL;
    /* hso传入的最大数据长度大于300重新分配内存 */
    if (tmp->length > 300) {
        uint32_t input_size = sizeof(nv_write_input_t) + (tmp->length * sizeof(uint8_t));
        nv_write_input = malloc(input_size);
        uint16_t copy_size = sizeof(nv_write_input_t) + 300;
        (void)memset_s(nv_write_input, input_size, 0, input_size);
        (void)memcpy_s(nv_write_input, input_size, cmd_param, copy_size);
    } else {
        nv_write_input = (nv_write_input_t *)cmd_param;
    }

    nv_write_output.key = (uint32_t)nv_write_input->key;
    nv_write_output.ret = uapi_nv_write(nv_write_input->key, nv_write_input->value,
        (uint16_t)nv_write_input->length);
    res = uapi_diag_report_packet(cmd_id, option, &nv_write_output, sizeof(nv_write_output), true);
    /* 如果传入的数据长度大于300，释放内存 */
    if (tmp->length > 300) {
        free(nv_write_input);
    }
    return res;
}

STATIC errcode_t diag_cmd_nv_write_with_attr(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size,
                                             diag_option_t *option)
{
    unused(cmd_param_size);
    errcode_t res;
    nv_write_with_attr_input_t *tmp = (nv_write_with_attr_input_t *)cmd_param;
    nv_write_with_attr_input_t *nv_write_with_attr_input = NULL;
    /* hso传入的最大数据长度大于300重新分配内存 */
    if (tmp->length > 300) {
        uint32_t input_size = sizeof(nv_write_with_attr_input_t) + (tmp->length * sizeof(uint8_t));
        nv_write_with_attr_input = malloc(input_size);
        uint16_t copy_size = sizeof(nv_write_with_attr_input_t) + 300;
        (void)memset_s(nv_write_with_attr_input, input_size, 0, input_size);
        (void)memcpy_s(nv_write_with_attr_input, input_size, cmd_param, copy_size);
    } else {
        nv_write_with_attr_input = (nv_write_with_attr_input_t *)cmd_param;
    }

    nv_write_with_attr_output_t nv_write_with_attr_output = {0};
    nv_key_attr_t attr = {0};
    attr.permanent = nv_write_with_attr_input->permanent;
    attr.encrypted = nv_write_with_attr_input->encrypted;
    attr.non_upgrade = nv_write_with_attr_input->non_upgrade;
    nv_write_with_attr_output.key = nv_write_with_attr_input->key;
    nv_write_with_attr_output.ret = uapi_nv_write_with_attr(nv_write_with_attr_input->key,
        nv_write_with_attr_input->value, nv_write_with_attr_input->length, &attr, nv_set_done_callback_test);
    res = uapi_diag_report_packet(cmd_id, option, &nv_write_with_attr_output, sizeof(nv_write_with_attr_output), true);
    /* 如果传入的数据长度大于300，释放内存 */
    if (tmp->length > 300) {
        free(nv_write_with_attr_input);
    }
    return res;
}

STATIC errcode_t diag_cmd_nv_get_store_status(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size,
                                              diag_option_t *option)
{
    unused(cmd_param_size);
    errcode_t res = ERRCODE_FAIL;
    nv_store_status_t status = {0};

    uint32_t flag = *(uint32_t *)cmd_param;

    if (flag == 0) {
        res = uapi_nv_get_store_status(&status);
        if (res == ERRCODE_SUCC) {
            return uapi_diag_report_packet(cmd_id, option, &status, sizeof(status), true);
        }
    } else {
        uint32_t pages_in_store;
        uint32_t page_index;

        (void)memset_s(&status, sizeof(nv_store_status_t), 0, sizeof(nv_store_status_t));

        pages_in_store = (uint32_t)kv_store_get_page_count(0);
        for (page_index = 0; page_index < pages_in_store; page_index++) {
            kv_page_handle_t page;
            kv_page_status_t page_status;
            if (kv_store_get_page_handle(0, page_index, &page) == ERRCODE_SUCC) {
                kv_page_get_status(&page, &page_status);
                status.total_space = page_status.total_space;
                status.used_space = page_status.used_space;
                status.reclaimable_space = page_status.reclaimable_space;
                status.corrupted_space = page_status.corrupted_space;
                status.max_key_space = page_status.max_key_space;
            }
            res = uapi_diag_report_packet(cmd_id, option, &status, sizeof(status), true);
        }
    }
    return res;
}

STATIC errcode_t diag_cmd_nv_backup(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    unused(cmd_param_size);
    nv_backup_mode_t *flag = (nv_backup_mode_t *)cmd_param;
    errcode_t res = uapi_nv_backup(flag);
    return uapi_diag_report_packet(cmd_id, option, (uint8_t *)&res, sizeof(res), true);
}

STATIC errcode_t diag_cmd_nv_set_restore_flag(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size,
    diag_option_t *option)
{
    unused(cmd_param_size);
    errcode_t res;
    nv_reset_mode_t *nv_reset_mode = (nv_reset_mode_t *)cmd_param;
    nv_reset_mode_out_t nv_reset_out = {0};
    if (memcpy_s(&nv_reset_out, sizeof(nv_reset_mode_t), nv_reset_mode, sizeof(nv_reset_mode_t)) != EOK) {
        return ERRCODE_MEMCPY;
    }
    if (nv_reset_mode->mode == 1) {
        res = uapi_nv_set_restore_mode_all();
    } else {
        nv_restore_mode_t restore_mode = {0};
        memcpy_s(&restore_mode, sizeof(nv_restore_mode_t), nv_reset_mode->region_flag, sizeof(nv_restore_mode_t));
        res = uapi_nv_set_restore_mode_partitial((const nv_restore_mode_t *)&restore_mode);
    }

    nv_reset_out.ret = res;
    return uapi_diag_report_packet(cmd_id, option, (uint8_t *)&nv_reset_out, sizeof(nv_reset_out), true);
}

STATIC errcode_t diag_cmd_nv_restore(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    unused(cmd_param_size);
    unused(cmd_param);
#if (CONFIG_NV_SUPPORT_BACKUP_RESTORE == NV_YES)
    errcode_t res = kv_restore_all_keys();
    if (res != ERRCODE_SUCC) {
        return res;
    }
    return uapi_diag_report_packet(cmd_id, option, (uint8_t *)&res, sizeof(res), true);
#else
    return ERRCODE_SUCC;
#endif
}

STATIC errcode_t diag_cmd_nv_get_page_usage(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size,
    diag_option_t *option)
{
    unused(cmd_param_size);
    unused(cmd_param);
    nv_page_usage_out page_usage_out = {0};

    kv_nvregion_map_t *nv_map = kv_nvregion_get_map();
    if (nv_map == NULL) {
        return ERRCODE_FAIL;
    }

    for (uint32_t i = 0; i < nv_map->num_entries; i++) {
        page_usage_out.store_id = nv_map->entries[i].details.store_id;
        page_usage_out.ver = nv_map->entries[i].details.ver;
        page_usage_out.page_index = nv_map->entries[i].details.page_index;
        page_usage_out.inverted_details_word = nv_map->entries[i].inverted_details_word;
        page_usage_out.sequence_number = nv_map->entries[i].sequence_number;
        page_usage_out.inverted_sequence_number = nv_map->entries[i].inverted_sequence_number;
        page_usage_out.used_space = nv_map->page_status_map[i].used_space;
        page_usage_out.reclaimable_space = nv_map->page_status_map[i].reclaimable_space;
        page_usage_out.corrupted_space = nv_map->page_status_map[i].corrupted_space;
        page_usage_out.total_space = KV_PAGE_SIZE - (uint32_t)sizeof(kv_page_header_t);
        page_usage_out.max_key_space =
            page_usage_out.total_space - page_usage_out.used_space + page_usage_out.reclaimable_space;
        page_usage_out.first_writable_location =
            nv_map->page_status_map[i].page_location + nv_map->page_status_map[i].first_writable_offset;

        (void)uapi_diag_report_packet(cmd_id, option, &page_usage_out, sizeof(nv_page_usage_out), true);
    }

    return ERRCODE_SUCC;
}

diag_cmd_reg_obj_t g_diag_nv_cmd_tbl[] = {
    {DIAG_CMD_NV_RD,                    DIAG_CMD_NV_RD,                     diag_cmd_nv_read},
    {DIAG_CMD_NV_RD_WITH_ATTR,          DIAG_CMD_NV_RD_WITH_ATTR,           diag_cmd_nv_read_with_attr},
    {DIAG_CMD_NV_WR,                    DIAG_CMD_NV_WR,                     diag_cmd_nv_write},
    {DIAG_CMD_NV_WR_WITH_ATTR,          DIAG_CMD_NV_WR_WITH_ATTR,           diag_cmd_nv_write_with_attr},
    {DIAG_CMD_NV_GET_STORE_STATUS,      DIAG_CMD_NV_GET_STORE_STATUS,       diag_cmd_nv_get_store_status},
    {DIAG_CMD_NV_BACKUP_ALL_KEYS,       DIAG_CMD_NV_BACKUP_ALL_KEYS,        diag_cmd_nv_backup},
    {DIAG_CMD_NV_RESTORE_ALL_KEYS,      DIAG_CMD_NV_RESTORE_ALL_KEYS,       diag_cmd_nv_restore},
    {DIAG_CMD_NV_GET_PAGE_USAGE,        DIAG_CMD_NV_GET_PAGE_USAGE,         diag_cmd_nv_get_page_usage},
    {DIAG_CMD_NV_SET_RESTORE_FLAG,      DIAG_CMD_NV_SET_RESTORE_FLAG,       diag_cmd_nv_set_restore_flag}
};

errcode_t zdiag_nv_init(void)
{
    return uapi_diag_register_cmd(g_diag_nv_cmd_tbl, sizeof(g_diag_nv_cmd_tbl) / sizeof(g_diag_nv_cmd_tbl[0]));
}

