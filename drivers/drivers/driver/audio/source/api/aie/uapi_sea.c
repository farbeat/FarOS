/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: unf sea
 * Author: Audio
 */

#include "securec.h"
#include "soc_uapi_sea.h"
#include "mpi_sea.h"
#include "mpi_adp.h"
#include "audio_type.h"
#include "sea_struct.h"
#include "core_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

static td_s32 sea_get_item_sets(td_handle sea, sea_item_type type, uapi_sea_item_sets *sets)
{
    td_s32 ret;
    td_u32 item_count;
    sea_item_sets item_sets;
    uapi_sea_item *item_offset = TD_NULL;

    if (sets == TD_NULL) {
        return ERR_SEA_NULL_PTR;
    }
    if (sets->size == 0 || sets->array == TD_NULL) {
        return ERR_SEA_INVALID_PARAM;
    }
    sets->count = 0;

    item_sets.index = 0;
    item_sets.total_count = 0;
    while (sets->count < sets->size) {
        ret = mpi_sea_get_item_sets(sea, type, &item_sets);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }

        item_offset = &sets->array[sets->count];
        item_count = sap_min(sets->size - sets->count, item_sets.output_count);
        if (item_count == 0) {
            break;
        }
        ret = memcpy_s(item_offset, sizeof(uapi_sea_item) * (sets->size - sets->count),
                       item_sets.items, sizeof(sea_info_item) * item_count);
        if (ret != EOK) {
            return ret;
        }

        sets->count += item_count;
        item_sets.index = sets->count;
        if (item_sets.index >= item_sets.total_count) {
            break;
        }
    }

    if (sets->size < item_sets.total_count) {
        return ERR_SEA_MEM_OVERLOAD;
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_sea_init(td_void)
{
    return mpi_sea_init();
}

td_s32 uapi_sea_deinit(td_void)
{
    return mpi_sea_deinit();
}

td_s32 uapi_sea_load_engine(uapi_sea_lib_id lib_id, const td_char *lib_name)
{
    return mpi_sea_load_engine((sea_lib_id)lib_id, lib_name);
}

td_s32 uapi_sea_unload_engine(uapi_sea_lib_id lib_id, const td_char *lib_name)
{
    return mpi_sea_unload_engine((sea_lib_id)lib_id, lib_name);
}

td_s32 uapi_sea_get_engine_caps(uapi_sea_lib_id lib_id, uapi_sea_eng_caps *eng_caps)
{
    td_s32 ret;
    td_u32 i;
    sea_eng_caps caps_temp;
    sea_eng_sel *eng_sel = TD_NULL;
    uapi_sea_eng_sel *sea_sel = TD_NULL;

    if (eng_caps == TD_NULL) {
        return ERR_SEA_NULL_PTR;
    }

    ret = mpi_sea_get_engine_caps((sea_lib_id)lib_id, &caps_temp);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    for (i = 0; i < caps_temp.eng_sel_cnt; i++) {
        eng_sel = &caps_temp.eng_sel[i];
        sea_sel = &eng_caps->eng_sel[i];
        sea_sel->afe.type = eng_sel->eng_ind[0].type;
        sea_sel->afe.lib_id = eng_sel->eng_ind[0].lib_id;
        sea_sel->aai[0].type = eng_sel->eng_ind[1].type;
        sea_sel->aai[0].lib_id = eng_sel->eng_ind[1].lib_id;
        sea_sel->aai[1].type = eng_sel->eng_ind[2].type; /* 2:index */
        sea_sel->aai[1].lib_id = eng_sel->eng_ind[2].lib_id; /* 2:index */
        sea_sel->aai[2].type = eng_sel->eng_ind[3].type; /* 2,3:index */
        sea_sel->aai[2].lib_id = eng_sel->eng_ind[3].lib_id; /* 2,3:index */
    }
    eng_caps->eng_sel_cnt = caps_temp.eng_sel_cnt;

    return AUDIO_SUCCESS;
}

static td_void sea_eng_sel_init(sea_eng_sel *eng_sel, const uapi_sea_eng_sel *sea_sel)
{
    eng_sel->eng_ind[SEA_ECH_AFE_IND].type = sea_sel->afe.type;
    eng_sel->eng_ind[SEA_ECH_AFE_IND].lib_id = sea_sel->afe.lib_id;
    eng_sel->eng_ind[SEA_ECH_AAI_IND].type = sea_sel->aai[0].type;
    eng_sel->eng_ind[SEA_ECH_AAI_IND].lib_id = sea_sel->aai[0].lib_id;
    eng_sel->eng_ind[SEA_ECH_AAI_IND + 1].type = sea_sel->aai[1].type;
    eng_sel->eng_ind[SEA_ECH_AAI_IND + 1].lib_id = sea_sel->aai[1].lib_id;
    eng_sel->eng_ind[SEA_ECH_AAI_IND + 2].type = sea_sel->aai[2].type; /* 2:index */
    eng_sel->eng_ind[SEA_ECH_AAI_IND + 2].lib_id = sea_sel->aai[2].lib_id; /* 2:index */
}

td_s32 uapi_sea_get_default_attr(const uapi_sea_eng_sel *sea_sel, uapi_sea_attr *sea_attr)
{
    td_s32 ret;
    sea_eng_sel eng_sel;
    sea_pcm_attr attr;

    if (sea_sel == TD_NULL || sea_attr == TD_NULL) {
        return ERR_SEA_NULL_PTR;
    }

    (td_void)memset_s(&eng_sel, sizeof(eng_sel), 0, sizeof(eng_sel));
    sea_eng_sel_init(&eng_sel, sea_sel);

    ret = mpi_sea_get_def_attr(&eng_sel, &attr);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    ret = memcpy_s(sea_attr, sizeof(uapi_sea_attr), &attr, sizeof(attr));
    if (ret != EOK) {
        return ret;
    }
    return AUDIO_SUCCESS;
}

td_s32 uapi_sea_create(td_handle *sea, const uapi_sea_eng_sel *sea_sel, const uapi_sea_attr *sea_attr)
{
    td_s32 ret;
    sea_eng_sel eng_sel;
    sea_pcm_attr attr;

    if (sea == TD_NULL || sea_sel == TD_NULL) {
        return ERR_SEA_NULL_PTR;
    }

    (td_void)memset_s(&eng_sel, sizeof(eng_sel), 0, sizeof(eng_sel));
    sea_eng_sel_init(&eng_sel, sea_sel);

    if (sea_attr == TD_NULL) {
        ret = mpi_sea_get_def_attr(&eng_sel, &attr);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    } else {
        ret = memcpy_s(&attr, sizeof(attr), sea_attr, sizeof(*sea_attr));
        if (ret != EOK) {
            return ret;
        }
    }

    ret = mpi_sea_create(sea, &eng_sel, &attr);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_sea_destroy(td_handle sea)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return mpi_sea_destroy(sea);
}

td_s32 uapi_sea_register_event_proc(td_handle sea, uapi_sea_event_proc proc, td_void *context)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return mpi_sea_register_event_proc(sea, (audio_event_proc)(td_uintptr_t)proc, context);
}

td_s32 uapi_sea_start(td_handle sea)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return mpi_sea_start(sea);
}

td_s32 uapi_sea_stop(td_handle sea)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return mpi_sea_stop(sea);
}

td_s32 uapi_sea_get_attr(td_handle sea, uapi_sea_attr *sea_attr)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return mpi_sea_get_pcm_attr(sea, (sea_pcm_attr *)(td_void *)sea_attr);
}

td_s32 uapi_sea_set_attr(td_handle sea, const uapi_sea_attr *sea_attr)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return mpi_sea_set_pcm_attr(sea, (const sea_pcm_attr *)(td_void *)sea_attr);
}

static td_s32 sea_get_afe_attr(uapi_sea_afe_attr *afe_attr, uapi_sea_afe_type type, const sea_eng_attr *eng_attr)
{
    switch (type) {
        case UAPI_SEA_AFE_SEE :
            afe_attr->u.see.options = eng_attr->u.see.options;
            break;
        case UAPI_SEA_AFE_VQE :
            afe_attr->u.vqe.options = eng_attr->u.vqe.options;
            break;
        case UAPI_SEA_AFE_AHE :
            afe_attr->u.ahe.volume = eng_attr->u.ahe.volume;
            break;
        default :
            return ERR_SEA_INVALID_PARAM;
    }

    afe_attr->type = type;
    afe_attr->mode = (uapi_sea_work_mode)(eng_attr->mode);
    return AUDIO_SUCCESS;
}

static td_s32 sea_get_aai_attr(uapi_sea_aai_attr *aai_attr, uapi_sea_aai_type type, const sea_eng_attr *eng_attr)
{
    td_s32 ret;
    switch (type) {
        case UAPI_SEA_AAI_KWS :
            aai_attr->u.kws.sensitivity = eng_attr->u.kws.sensitivity;
            aai_attr->u.kws.max_dura_ms = eng_attr->u.kws.max_dura_ms;
            aai_attr->u.kws.vad_bgn_thres = eng_attr->u.kws.vad_bgn_thres;
            aai_attr->u.kws.vad_end_thres = eng_attr->u.kws.vad_end_thres;
            ret = strcpy_s(aai_attr->u.kws.keyword, sizeof(aai_attr->u.kws.keyword), eng_attr->u.kws.keyword);
            if (ret != EOK) {
                return ERR_SEA_MEM_COPY;
            }
            break;
        case UAPI_SEA_AAI_ASR :
            aai_attr->u.asr.nlp_enable = eng_attr->u.asr.nlp_enable;
            aai_attr->u.asr.max_dura_ms = eng_attr->u.asr.max_dura_ms;
            aai_attr->u.asr.vad_bgn_thres = eng_attr->u.asr.vad_bgn_thres;
            aai_attr->u.asr.vad_end_thres = eng_attr->u.asr.vad_end_thres;
            break;
        case UAPI_SEA_AAI_VID :
            aai_attr->u.vid.sensitivity = eng_attr->u.vid.sensitivity;
            aai_attr->u.vid.min_dura_ms = eng_attr->u.vid.min_dura_ms;
            aai_attr->u.vid.vid_count = eng_attr->u.vid.vid_count;
            break;
        case UAPI_SEA_AAI_ASD :
            aai_attr->u.asd.latency = eng_attr->u.asd.latency;
            break;
        default :
            return ERR_SEA_INVALID_PARAM;
    }

    aai_attr->type = type;
    aai_attr->mode = (uapi_sea_work_mode)(eng_attr->mode);
    return AUDIO_SUCCESS;
}

td_s32 uapi_sea_get_afe_attr(td_handle sea, uapi_sea_afe_type type, uapi_sea_afe_attr *afe_attr)
{
    td_s32 ret;
    sea_eng_attr eng_attr;

    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    if (afe_attr == TD_NULL) {
        return ERR_SEA_NULL_PTR;
    }

    if (type <= UAPI_SEA_AFE_NULL || type >= UAPI_SEA_AFE_MAX) {
        return ERR_SEA_INVALID_PARAM;
    }

    ret = mpi_sea_get_eng_attr(sea, (td_u8)type, &eng_attr);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return sea_get_afe_attr(afe_attr, type, &eng_attr);
}

td_s32 uapi_sea_get_aai_attr(td_handle sea, uapi_sea_aai_type type, uapi_sea_aai_attr *aai_attr)
{
    td_s32 ret;
    sea_eng_attr eng_attr;

    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    if (aai_attr == TD_NULL) {
        return ERR_SEA_NULL_PTR;
    }

    if (type <= UAPI_SEA_AAI_NULL || type >= UAPI_SEA_AAI_MAX) {
        return ERR_SEA_INVALID_PARAM;
    }

    ret = mpi_sea_get_eng_attr(sea, (td_u8)type, &eng_attr);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return sea_get_aai_attr(aai_attr, type, &eng_attr);
}

static td_s32 sea_set_afe_attr(sea_eng_attr *eng_attr, uapi_sea_afe_type type, const uapi_sea_afe_attr *afe_attr)
{
    switch (type) {
        case UAPI_SEA_AFE_SEE :
            eng_attr->u.see.options = afe_attr->u.see.options;
            break;
        case UAPI_SEA_AFE_VQE :
            eng_attr->u.vqe.options = afe_attr->u.vqe.options;
            break;
        case UAPI_SEA_AFE_AHE :
            eng_attr->u.ahe.volume = afe_attr->u.ahe.volume;
            break;
        default :
            return ERR_SEA_INVALID_PARAM;
    }

    eng_attr->mode = (sea_work_mode)afe_attr->mode;
    eng_attr->type = (td_u8)type;
    return AUDIO_SUCCESS;
}

static td_s32 sea_set_aai_attr(sea_eng_attr *eng_attr, uapi_sea_aai_type type, const uapi_sea_aai_attr *aai_attr)
{
    td_s32 ret;
    switch (type) {
        case UAPI_SEA_AAI_KWS :
            eng_attr->u.kws.sensitivity = aai_attr->u.kws.sensitivity;
            eng_attr->u.kws.max_dura_ms = aai_attr->u.kws.max_dura_ms;
            eng_attr->u.kws.vad_bgn_thres = aai_attr->u.kws.vad_bgn_thres;
            eng_attr->u.kws.vad_end_thres = aai_attr->u.kws.vad_end_thres;
            ret = strcpy_s(eng_attr->u.kws.keyword, sizeof(eng_attr->u.kws.keyword), aai_attr->u.kws.keyword);
            if (ret != EOK) {
                return ERR_SEA_MEM_COPY;
            }
            break;
        case UAPI_SEA_AAI_ASR :
            eng_attr->u.asr.nlp_enable = aai_attr->u.asr.nlp_enable;
            eng_attr->u.asr.max_dura_ms = aai_attr->u.asr.max_dura_ms;
            eng_attr->u.asr.vad_bgn_thres = aai_attr->u.asr.vad_bgn_thres;
            eng_attr->u.asr.vad_end_thres = aai_attr->u.asr.vad_end_thres;
            break;
        case UAPI_SEA_AAI_VID :
            eng_attr->u.vid.sensitivity = aai_attr->u.vid.sensitivity;
            eng_attr->u.vid.min_dura_ms = aai_attr->u.vid.min_dura_ms;
            eng_attr->u.vid.vid_count = aai_attr->u.vid.vid_count;
            break;
        case UAPI_SEA_AAI_ASD :
            eng_attr->u.asd.latency = aai_attr->u.asd.latency;
            break;
        default :
            return ERR_SEA_INVALID_PARAM;
    }

    eng_attr->mode = (sea_work_mode)aai_attr->mode;
    eng_attr->type = (td_u8)type;
    return AUDIO_SUCCESS;
}

td_s32 uapi_sea_set_afe_attr(td_handle sea, uapi_sea_afe_type type, const uapi_sea_afe_attr *afe_attr)
{
    td_s32 ret;
    sea_eng_attr eng_attr;

    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    if (afe_attr == TD_NULL) {
        return ERR_SEA_NULL_PTR;
    }

    if (type <= UAPI_SEA_AFE_NULL || type >= UAPI_SEA_AFE_MAX) {
        return ERR_SEA_INVALID_PARAM;
    }

    ret = sea_set_afe_attr(&eng_attr, type, afe_attr);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return mpi_sea_set_eng_attr(sea, (td_u8)type, &eng_attr);
}

td_s32 uapi_sea_set_aai_attr(td_handle sea, uapi_sea_aai_type type, const uapi_sea_aai_attr *aai_attr)
{
    td_s32 ret;
    sea_eng_attr eng_attr;

    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    if (aai_attr == TD_NULL) {
        return ERR_SEA_NULL_PTR;
    }

    if (type <= UAPI_SEA_AAI_NULL || type >= UAPI_SEA_AAI_MAX) {
        return ERR_SEA_INVALID_PARAM;
    }

    ret = sea_set_aai_attr(&eng_attr, type, aai_attr);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return mpi_sea_set_eng_attr(sea, (td_u8)type, &eng_attr);
}

td_s32 uapi_sea_get_param(td_handle sea, uapi_sea_afe_type type, td_void *param, td_u32 param_size)
{
    return mpi_sea_get_param(sea, (sea_afe_type)type, param, param_size);
}

td_s32 uapi_sea_set_param(td_handle sea, uapi_sea_afe_type type, const td_void *param, td_u32 param_size)
{
    return mpi_sea_set_param(sea, (sea_afe_type)type, param, param_size);
}

td_s32 uapi_sea_attach_output(td_handle sea, uapi_sea_output_type type, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (sea == 0 || output == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    mod_id.u32 = (td_u32)output;
    if (mod_id.bits.type == SAP_MOD_ADP) {
        ret = mpi_adp_alloc_output_shrb(output, sea, (td_u32)type);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    }

    return mpi_sea_attach_output(sea, (sea_output_type)type, output);
}

td_s32 uapi_sea_detach_output(td_handle sea, uapi_sea_output_type type, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (sea == 0 || output == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    if (get_sap_mod_type(sea) != SAP_MOD_SEA) {
        return ERR_SEA_INVALID_HANDLE;
    }

    ret = mpi_sea_detach_output(sea, (sea_output_type)type, output);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    mod_id.u32 = (td_u32)output;
    if (mod_id.bits.type == SAP_MOD_ADP) {
        ret = mpi_adp_free_shrb(output);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_sea_get_phrase_count(td_handle sea, td_u32 *count)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return mpi_sea_get_item_count(sea, SEA_ITEM_PHRASE, count);
}

td_s32 uapi_sea_get_phrase_sets(td_handle sea, uapi_sea_item_sets *sets)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return sea_get_item_sets(sea, SEA_ITEM_PHRASE, sets);
}

td_s32 uapi_sea_get_entity_count(td_handle sea, td_u32 *count)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return mpi_sea_get_item_count(sea, SEA_ITEM_VOICE_ID, count);
}

td_s32 uapi_sea_get_entity_sets(td_handle sea, uapi_sea_item_sets *sets)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return sea_get_item_sets(sea, SEA_ITEM_VOICE_ID, sets);
}

td_s32 uapi_sea_start_vid_enroll(td_handle sea)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return mpi_sea_enable_vid_enroll(sea, TD_TRUE);
}

td_s32 uapi_sea_stop_vid_enroll(td_handle sea)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return mpi_sea_enable_vid_enroll(sea, TD_FALSE);
}

td_s32 uapi_sea_add_vid_entity(td_handle sea, const uapi_sea_item *item)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return mpi_sea_add_item(sea, SEA_ITEM_VOICE_ID, (const sea_info_item *)(td_void *)item);
}

td_s32 uapi_sea_remove_vid_entity(td_handle sea, const uapi_sea_item *item)
{
    if (sea == 0) {
        return ERR_SEA_INVALID_HANDLE;
    }

    return mpi_sea_remove_item(sea, SEA_ITEM_VOICE_ID, (const sea_info_item *)(td_void *)item);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
