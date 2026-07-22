/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: gpu proc by diag
 */

#include "diag_gpu_proc.h"
#include <stdio.h>
#include <string.h>
#include "errcode.h"
#include "diag.h"
#include "soc_diag_cmd_id.h"
#include "td_base.h"
#include "dfx_adapt_layer.h"
#include "drv_gpu_dfx.h"

#define GMMU_PARAM_CNT 2

#ifdef CONFIG_DIAG_GPU_PROC_SUPPORT
STATIC errcode_t diag_vau_dfx_read(uint16_t cmd_id, diag_option_t *option)
{
    errcode_t ret = (errcode_t)vau_dfx_proc_read(0, NULL);
    uapi_diag_report_packet(cmd_id, option, (uint8_t *)("vau_proc_read_succ\r\n"),
                            (uint16_t)strlen("vau_proc_read_succ\r\n") + 1, true);
    return ret;
}

STATIC errcode_t diag_jpeg_dfx_read(uint16_t cmd_id, diag_option_t *option)
{
    errcode_t ret = ERRCODE_SUCC;
#ifdef SUPPORT_GPU_JPEG
    ret = (errcode_t)jpeg_dfx_proc_read(0, NULL);
#endif
    uapi_diag_report_packet(cmd_id, option, (uint8_t *)("jpeg_proc_read_succ\r\n"),
                            (uint16_t)strlen("jpeg_proc_read_succ\r\n") + 1, true);
    return ret;
}

STATIC errcode_t diag_dpu_dfx_read(uint16_t cmd_id, diag_option_t *option, void *cmd_param)
{
    errcode_t ret = ERRCODE_SUCC;
    if (cmd_param == NULL) {
        printf(" invalid dpu_proc cmd: should input one param\n");
        return ERRCODE_FAIL;
    }
    char *dpu_param[1] = { NULL };
    const td_u8 str_len = 5;
    dpu_param[0] = (char *)dfx_malloc(0, str_len);
    if (dpu_param[0] == NULL) {
        return ERRCODE_MALLOC;
    }

    if (cmd_id == DIAG_CMD_GPU_DPU_PROC) {
        uint32_t dpu_layer_id = *(uint32_t *)cmd_param;
        if (sprintf_s(dpu_param[0], str_len, "%u\n", dpu_layer_id) == -1) {
            dfx_free(0, dpu_param[0]);
            return ERRCODE_FAIL;
        }
        ret = (errcode_t)dfx_get_proc_info(1, (const td_char **)dpu_param);
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)("dpu_proc_read_succ\r\n"),
                                (uint16_t)strlen("dpu_proc_read_succ\r\n") + 1, true);
    } else {
        uint32_t dpu_print_en = *(uint32_t *)cmd_param;
        if (sprintf_s(dpu_param[0], str_len, "%u\n", dpu_print_en) == -1) {
            dfx_free(0, dpu_param[0]);
            return ERRCODE_FAIL;
        }
        ret = (errcode_t)dfx_dpu_print(1, (const td_char **)dpu_param);
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)("dpu_print_read_succ\r\n"),
                                (uint16_t)strlen("dpu_print_read_succ\r\n") + 1, true);
    }
    dfx_free(0, dpu_param[0]);
    return ret;
}

STATIC errcode_t diag_gmmu_dfx_read(uint16_t cmd_id, diag_option_t *option, void *cmd_param)
{
    errcode_t ret = ERRCODE_SUCC;
    if (cmd_param == NULL) {
        printf(" invalid gmmu_proc cmd: should input 2 params\n");
        return ERRCODE_FAIL;
    }
    char *gmmu_param[GMMU_PARAM_CNT] = { NULL };
    const td_u8 str_len = 5;
    for (td_u32 i = 0; i < GMMU_PARAM_CNT; i++) {
        gmmu_param[i] = (char *)dfx_malloc(0, str_len);
        if (gmmu_param[i] == NULL) {
            if (i == 1) {
                dfx_free(0, gmmu_param[0]);
            }
            return ERRCODE_MALLOC;
        }
    }

    gpu_gmmu_proc_info_t *gmmu_proc_param = (gpu_gmmu_proc_info_t *)cmd_param;
    if (sprintf_s(gmmu_param[0], str_len, "%u\n", gmmu_proc_param->proc_en) == -1) {
        dfx_free(0, gmmu_param[0]);
        return ERRCODE_FAIL;
    }
    if (sprintf_s(gmmu_param[1], str_len, "%u\n", gmmu_proc_param->lut_en) == -1) {
        dfx_free(0, gmmu_param[0]);
        dfx_free(0, gmmu_param[1]);
        return ERRCODE_FAIL;
    }
#ifdef SUPPORT_GPU_GMMU
    ret = (errcode_t)gmmu_dfx_print_proc_info(GMMU_PARAM_CNT, (const td_char **)gmmu_param);
#endif
    uapi_diag_report_packet(cmd_id, option, (uint8_t *)("gmmu_proc_read_succ\r\n"),
                            (uint16_t)strlen("gmmu_proc_read_succ\r\n") + 1, true);
    dfx_free(0, gmmu_param[0]);
    dfx_free(0, gmmu_param[1]);
    return ret;
}

STATIC errcode_t gpu_proc_read(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    unused(cmd_param_size);
    errcode_t ret = ERRCODE_SUCC;

    switch (cmd_id) {
        case DIAG_CMD_GPU_VAU_PROC:
            return diag_vau_dfx_read(cmd_id, option);
        case DIAG_CMD_GPU_JPEG_PROC:
            return diag_jpeg_dfx_read(cmd_id, option);
        case DIAG_CMD_GPU_DPU_PROC:
        case DIAG_CMD_GPU_DPU_PRINT:
            return diag_dpu_dfx_read(cmd_id, option, cmd_param);
        case DIAG_CMD_GPU_GMMU_PROC:
            return diag_gmmu_dfx_read(cmd_id, option, cmd_param);
        default:
            printf("gpu_proc_read：invalid cmd id:%#x.\r\n",  cmd_id);
            ret = ERRCODE_FAIL;
            break;
    }
    return ret;
}

diag_cmd_reg_obj_t g_diag_gpu_proc_cmd_tbl[] = {
    {DIAG_CMD_GPU_VAU_PROC, DIAG_CMD_GPU_DPU_PRINT, gpu_proc_read},
};

errcode_t diag_gpu_proc_init(void)
{
    return uapi_diag_register_cmd(g_diag_gpu_proc_cmd_tbl,
        sizeof(g_diag_gpu_proc_cmd_tbl) / sizeof(g_diag_gpu_proc_cmd_tbl[0]));
}
#endif
