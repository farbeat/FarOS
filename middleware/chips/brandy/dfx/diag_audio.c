/*
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 * Description: audio diag
 * Author: audio
 * Create: 2024-06-06
 */

#include "diag.h"
#ifdef CONFIG_NV_FEATURE_SUPPORT
#include "nv.h"
#endif
#ifdef SUPPORT_MEDIA_AUDIO_MANAGER
#include "audio_manager_c_wrapper.h"
#endif
#include "debug_print.h"
#include "soc_diag_cmd_id.h"
#include "diag_audio.h"

#define GET_SEE_PARAM_FIRST_POS 0
#define DEBUGKITS_READ_PARAM_TAG 0xff
#define PARSE_EFFECT_PARAM_FLAG_POS 0

#define audio_unused(var) (void)(var)

/*
 * Debugkit menu点击通知 读取/配置 3A算法参数
 *
 * 当点击Read时，读取当前3A算法参数
 * 当点击Write时，将界面中的3A算法参数配置下去，并使能到算法中
 */
#ifdef SUPPORT_MEDIA_AUDIO_MANAGER
errcode_t diag_see_param_trans(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    errcode_t ret;
    uint8_t *param = (uint8_t *)cmd_param;

    if (cmd_param_size != NV_AUDIO_VQE_PARAM_SIZE * sizeof(uint32_t)) {
        PRINT("[audio][diag] cmd_param_size invalid\r\n");
        return ERRCODE_FAIL;
    }

    if (param[GET_SEE_PARAM_FIRST_POS] != DEBUGKITS_READ_PARAM_TAG) {
        /* set param , this func return 0 when success */
        ret = AudioManagerSetSeaParam((uint8_t *)param, cmd_param_size);
        if (ret != ERRCODE_SUCC) {
            PRINT("[audio][diag] set param fail\r\n");
            return ERRCODE_FAIL;
        }
    } else {
        /* get param , this func return 0 when success */
        ret = AudioManagerGetSeaParam((uint8_t *)param, cmd_param_size);
        if (ret != ERRCODE_SUCC) {
            PRINT("[audio][diag] get param fail\r\n");
            return ERRCODE_FAIL;
        }
    }

    return uapi_diag_report_packet(cmd_id, option, (uint8_t *)cmd_param, (uint16_t)cmd_param_size, true);
}

static errcode_t audio_manager_save_sea_param(uint8_t *sea_param, uint16_t len)
{
    if (sea_param == NULL || len != sizeof(audio_vqe_param_t)) {
        PRINT("[audio_manager_save_sea_param]sea_param null or len invalid");
        return ERRCODE_INVALID_PARAM;
    }

#ifdef CONFIG_NV_FEATURE_SUPPORT
    errcode_t nv_op = uapi_nv_write((uint16_t)NV_ID_AUDIO_VQE_PARAM, sea_param, len);
    if (nv_op != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
#endif
    return ERRCODE_SUCC;
}

/* debugkit menu点击Save保存所有3A参数 */
errcode_t diag_see_param_save(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    audio_vqe_param_t *nv_data = (audio_vqe_param_t *)malloc(sizeof(audio_vqe_param_t));
    if (nv_data == NULL) {
        PRINT("[audio][diag] malloc fail\r\n");
        return ERRCODE_MALLOC;
    }
    (void)memset_s(nv_data, sizeof(audio_vqe_param_t), 0, sizeof(audio_vqe_param_t));

    nv_data->size = NV_AUDIO_VQE_PARAM_SIZE;
    errcode_t ret = AudioManagerGetSeaParam((uint8_t *)nv_data->data, NV_AUDIO_VQE_PARAM_SIZE * sizeof(uint32_t));
    if (ret != ERRCODE_SUCC) {
        free(nv_data);
        PRINT("[audio][diag] get param fail\r\n");
        return ERRCODE_FAIL;
    }

    errcode_t nv_op = audio_manager_save_sea_param((uint8_t *)nv_data, (uint16_t)sizeof(audio_vqe_param_t));
    if (nv_op != ERRCODE_SUCC) {
        free(nv_data);
        PRINT("[audio][diag] save param nv fail\r\n");
        return ERRCODE_FAIL;
    }
    free(nv_data);

    errcode_t op = uapi_diag_report_packet(cmd_id, option, &ret, sizeof(uint8_t), true);
    if (op != ERRCODE_SUCC) {
        PRINT("[audio][diag] read rpt fail %u\r\n", op);
    }

    audio_unused(cmd_param);
    audio_unused(cmd_param_size);
    return op;
}
#endif /* SUPPORT_MEDIA_AUDIO_MANAGER */