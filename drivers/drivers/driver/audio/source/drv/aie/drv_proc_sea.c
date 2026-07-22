/*
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: source file for drv sea proc
 * Author: audio
 * Create: 2022-01-08
 */

#include "securec.h"
#include "audio_struct.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "drv_audio_proc.h"
#include "drv_audio_osal.h"
#include "drv_proc_sea.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef SAP_PROC_SUPPORT

#define SEA_VERSION_LEN 11
#define SEA_NAME_LEN 4

typedef struct {
    sea_afe_anr_type type;
    const td_char *name;
} anr_type_proc_name_item;

static const anr_type_proc_name_item g_anr_type_proc_name_list[] = {
    {SEA_AFE_ANR_SCNR, "scnr"},
    {SEA_AFE_ANR_DMNR, "dmnr"},
};

typedef struct {
    sea_afe_aec_type type;
    const td_char *name;
} aec_type_proc_name_item;

static const aec_type_proc_name_item g_aec_type_proc_name_list[] = {
    {SEA_AFE_AEC_MONO, "mono"},
    {SEA_AFE_AEC_STEREO, "stereo"},
    {SEA_AFE_AEC_MULTI_CHANNEL, "multi"},
    {SEA_AFE_AEC_AI, "ai"},
};

typedef struct {
    sea_afe_aec_ref ref;
    const td_char *name;
} aec_ref_proc_name_item;

static const aec_ref_proc_name_item g_aec_ref_proc_name_list[] = {
    {SEA_AFE_AEC_REF_TX_DAC, "tx_dac"},
    {SEA_AFE_AEC_REF_TX_HDMI, "tx_hdmi"},
    {SEA_AFE_AEC_REF_ADAPTIVE, "adaptive"},
};

typedef struct {
    sea_afe_agc_type type;
    const td_char *name;
} agc_type_proc_name_item;

static const agc_type_proc_name_item g_agc_type_proc_name_list[] = {
    {SEA_AFE_AGC_RX, "rx"},
    {SEA_AFE_AGC_TX, "tx"},
};

typedef struct {
    sea_afe_type type;
    const td_char *name;
} afe_type_proc_name_item;

static const afe_type_proc_name_item g_afe_type_proc_name_list[] = {
    {SEA_AFE_NULL, "null"},
    {SEA_AFE_SEE, "see"},
    {SEA_AFE_VQE, "vqe"},
    {SEA_AFE_AHE, "ahe"},
};

typedef struct {
    sea_aai_type type;
    const td_char *name;
} aai_type_proc_name_item;

static const aai_type_proc_name_item g_aai_type_proc_name_list[] = {
    {SEA_AAI_NULL, "null"},
    {SEA_AAI_KWS, "kws"},
    {SEA_AAI_ASR, "asr"},
    {SEA_AAI_VID, "vid"},
    {SEA_AAI_ASD, "asd"},
};

typedef struct {
    sea_work_mode mode;
    const td_char *name;
} sea_mode_proc_name_item;

static const sea_mode_proc_name_item g_sea_mode_proc_name_list[] = {
    {SEA_NORM_MODE, "normal"},
    {SEA_LP_MODE, "lp"},
    {SEA_HP_MODE, "hp"},
};

typedef struct {
    sea_output_type type;
    const td_char *name;
} output_type_proc_name_item;

static const output_type_proc_name_item g_output_type_proc_name_list[] = {
    {SEA_OUTPUT_REC_SRC, "record"},
    {SEA_OUTPUT_ASR_SRC, "asr"},
    {SEA_OUTPUT_VC_SRC, "vc"},
    {SEA_OUTPUT_AH_SRC, "ah"},
    {SEA_OUTPUT_KWS_SRC, "kws"},
};

typedef struct {
    sea_asr_domain domain;
    const td_char *name;
} asr_domain_proc_name_item;

static const asr_domain_proc_name_item g_asr_domain_proc_name_list[] = {
    {SEA_ASR_DM_NULL, "null"},
    {SEA_ASR_DM_CHAT, "chat"},
    {SEA_ASR_DM_IOT, "iot"},
    {SEA_ASR_DM_APP, "app"},
    {SEA_ASR_DM_MUSIC, "music"},
    {SEA_ASR_DM_MOVIE, "movie"},
    {SEA_ASR_DM_FOOD, "food"},
    {SEA_ASR_DM_SPORT, "sport"},
    {SEA_ASR_DM_CONTACT, "contact"},
    {SEA_ASR_DM_WEATHER, "weather"},
    {SEA_ASR_DM_CLOTHING, "clothing"},
    {SEA_ASR_DM_NAVIGATION, "navigation"},
    {SEA_ASR_DM_MEDICAL, "medical"},
};

typedef struct {
    td_char name[SEA_NAME_LEN];
    td_char version[SEA_VERSION_LEN];
} version_info_str;

typedef struct {
    sea_lib_id lib_id;
    const td_char *name;
} sea_lib_proc_name_item;

static const sea_lib_proc_name_item g_sea_lib_proc_name_list[] = {
    {SEA_LIB_ID_SEE, "see"},
    {SEA_LIB_ID_KWS, "kws"},
    {SEA_LIB_ID_ASR, "asr"},
};

static const td_char *get_anr_type_name(sea_afe_anr_type type)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_anr_type_proc_name_list) / sizeof(g_anr_type_proc_name_list[0])); i++) {
        if (type == g_anr_type_proc_name_list[i].type) {
            return g_anr_type_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_aec_type_name(sea_afe_aec_type type)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_aec_type_proc_name_list) / sizeof(g_aec_type_proc_name_list[0])); i++) {
        if (type == g_aec_type_proc_name_list[i].type) {
            return g_aec_type_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_aec_ref_name(sea_afe_aec_ref ref)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_aec_ref_proc_name_list) / sizeof(g_aec_ref_proc_name_list[0])); i++) {
        if (ref == g_aec_ref_proc_name_list[i].ref) {
            return g_aec_ref_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_agc_type_name(sea_afe_agc_type type)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_agc_type_proc_name_list) / sizeof(g_agc_type_proc_name_list[0])); i++) {
        if (type == g_agc_type_proc_name_list[i].type) {
            return g_agc_type_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_afe_type_name(sea_afe_type type)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_afe_type_proc_name_list) / sizeof(g_afe_type_proc_name_list[0])); i++) {
        if (type == g_afe_type_proc_name_list[i].type) {
            return g_afe_type_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_aai_type_name(sea_aai_type type)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_aai_type_proc_name_list) / sizeof(g_aai_type_proc_name_list[0])); i++) {
        if (type == g_aai_type_proc_name_list[i].type) {
            return g_aai_type_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_sea_mode_name(sea_work_mode mode)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_sea_mode_proc_name_list) / sizeof(g_sea_mode_proc_name_list[0])); i++) {
        if (mode == g_sea_mode_proc_name_list[i].mode) {
            return g_sea_mode_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_output_type_name(sea_output_type type)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_output_type_proc_name_list) / sizeof(g_output_type_proc_name_list[0])); i++) {
        if (type == g_output_type_proc_name_list[i].type) {
            return g_output_type_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_asr_domain_name(sea_asr_domain domain)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_asr_domain_proc_name_list) / sizeof(g_asr_domain_proc_name_list[0])); i++) {
        if (domain == g_asr_domain_proc_name_list[i].domain) {
            return g_asr_domain_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_sea_name(sea_lib_id lib_id)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_sea_lib_proc_name_list) / sizeof(g_sea_lib_proc_name_list[0])); i++) {
        if (lib_id == g_sea_lib_proc_name_list[i].lib_id) {
            return g_sea_lib_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static td_void afe_show_option(td_void *file, td_u32 option)
{
    drv_audio_osal_proc_print(file,
        "%-5s: %-8s |" "%-5s: %-4s |" "%-5s: %-8s |" "%-5s: %-4s |"
        "%-5s: %-8s |" "%-5s: %-4s |" "%-5s: %-8s |" "%-5s: %-4s |"  "\n",
        "anr", drv_audio_proc_get_bool(check_option_valid(option, SEA_AFE_OPT_ANR)),
        "aec", drv_audio_proc_get_bool(check_option_valid(option, SEA_AFE_OPT_AEC)),
        "agc", drv_audio_proc_get_bool(check_option_valid(option, SEA_AFE_OPT_AGC)),
        "wnr", drv_audio_proc_get_bool(check_option_valid(option, SEA_AFE_OPT_WNR)),
        "bfm", drv_audio_proc_get_bool(check_option_valid(option, SEA_AFE_OPT_BFM)),
        "bss", drv_audio_proc_get_bool(check_option_valid(option, SEA_AFE_OPT_BSS)),
        "drb", drv_audio_proc_get_bool(check_option_valid(option, SEA_AFE_OPT_DRB)),
        "limit", drv_audio_proc_get_bool(check_option_valid(option, SEA_AFE_OPT_LMT)));

    drv_audio_osal_proc_print(file,
        "%-5s: %-8s |" "%-5s: %-4s |" "%-5s: %-8s |" "%-5s: %-4s |"
        "%-5s: %-8s |" "%-5s: %-4s |" "%-5s: %-8s |" "%-5s: %-4s |"  "\n",
        "eq", drv_audio_proc_get_bool(check_option_valid(option, SEA_AFE_OPT_EQ)),
        "drc", drv_audio_proc_get_bool(check_option_valid(option, SEA_AFE_OPT_DRC)),
        "", "",
        "", "",
        "", "",
        "", "",
        "", "",
        "", "");
}

static td_void show_basic_status(td_void *file, td_handle sea, const sea_proc_item *proc)
{
    drv_audio_proc_print_title(file, "basic status");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10u |" "\n",
        "handle", sea,
        "enable", drv_audio_proc_get_bool(proc->enable),
        "event_proc", drv_audio_proc_get_bool((proc->event_proc != TD_NULL) ? TD_TRUE : TD_FALSE),
        "send_try", proc->send_try_cnt);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "send_empty", proc->send_empty_cnt,
        "send_fail", proc->send_fail_cnt,
        "send_ok", proc->send_ok_cnt,
        "org_match", proc->org_match_cnt);

    /* line 3 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "user_match", proc->user_match_cnt,
        "", "",
        "", "",
        "", "");
}

static td_void afe_show_anr_cfg(td_void *file, const sea_afe_anr_cfg *cfg)
{
    drv_audio_proc_print_title(file, "anr cfg");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "enable", drv_audio_proc_get_bool(cfg->enable),
        "type", get_anr_type_name(cfg->type),
        "", "",
        "", "");
}

static td_void afe_show_aec_cfg(td_void *file, const sea_afe_aec_cfg *cfg)
{
    drv_audio_proc_print_title(file, "aec cfg");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10d |" "\n",
        "enable", drv_audio_proc_get_bool(cfg->enable),
        "type", get_aec_type_name(cfg->type),
        "ref_type", get_aec_ref_name(cfg->ref_type),
        "latency", cfg->latency);
}

static td_void afe_show_agc_cfg(td_void *file, const sea_afe_agc_cfg *cfg)
{
    drv_audio_proc_print_title(file, "agc cfg");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "enable", drv_audio_proc_get_bool(cfg->enable),
        "type", get_agc_type_name(cfg->type),
        "", "",
        "", "");
}

static td_void afe_show_see_attr(td_void *file, const sea_afe_see_attr *attr)
{
    afe_show_option(file, attr->options);

    if (check_option_valid(attr->options, SEA_AFE_OPT_ANR) == TD_TRUE) {
        afe_show_anr_cfg(file, &(attr->anr));
    }

    if (check_option_valid(attr->options, SEA_AFE_OPT_AEC) == TD_TRUE) {
        afe_show_aec_cfg(file, &(attr->aec));
    }

    if (check_option_valid(attr->options, SEA_AFE_OPT_AGC) == TD_TRUE) {
        afe_show_agc_cfg(file, &(attr->agc));
    }
}

static td_void afe_show_vqe_attr(td_void *file, const sea_afe_vqe_attr *attr)
{
    afe_show_option(file, attr->options);
}

static td_void afe_show_ahe_attr(td_void *file, const sea_afe_ahe_attr *attr)
{
    afe_show_option(file, attr->options);

    if (check_option_valid(attr->options, SEA_AFE_OPT_ANR) == TD_TRUE) {
        afe_show_anr_cfg(file, &(attr->anr));
    }

    if (check_option_valid(attr->options, SEA_AFE_OPT_AGC) == TD_TRUE) {
        afe_show_agc_cfg(file, &(attr->agc));
    }
}

static td_void show_afe_attr(td_void *file, const sea_eng_attr *eng_attr)
{
    drv_audio_proc_print_title(file, "afe attr [%u]", eng_attr->type);

    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "type", get_afe_type_name(eng_attr->type),
        "mode", get_sea_mode_name(eng_attr->mode),
        "", "",
        "", "");

    switch (eng_attr->type) {
        case SEA_AFE_SEE:
            afe_show_see_attr(file, &(eng_attr->u.see));
            break;

        case SEA_AFE_VQE:
            afe_show_vqe_attr(file, &(eng_attr->u.vqe));
            break;

        case SEA_AFE_AHE:
            afe_show_ahe_attr(file, &(eng_attr->u.ahe));
            break;

        default:
            break;
    }
}

static td_void aai_show_kws_attr(td_void *file, const sea_eng_attr *eng_attr)
{
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "type", get_aai_type_name(eng_attr->type),
        "mode", get_sea_mode_name(eng_attr->mode),
        "sensitivity", eng_attr->u.kws.sensitivity,
        "max_dura_ms", eng_attr->u.kws.max_dura_ms);

    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-40s |" "\n",
        "vad_bgn_thres", eng_attr->u.kws.vad_bgn_thres,
        "vad_end_thres", eng_attr->u.kws.vad_end_thres,
        "key_word", eng_attr->u.kws.keyword);
}

static td_void aai_show_asr_attr(td_void *file, const sea_eng_attr *eng_attr)
{
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "type", get_aai_type_name(eng_attr->type),
        "mode", get_sea_mode_name(eng_attr->mode),
        "nlp_enable", eng_attr->u.asr.nlp_enable,
        "max_dura_ms", eng_attr->u.asr.max_dura_ms);

    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
        "vad_bgn_thres", eng_attr->u.asr.vad_bgn_thres,
        "vad_end_thres", eng_attr->u.asr.vad_end_thres,
        "word_number", eng_attr->u.asr.word_number,
        "domains", get_asr_domain_name(eng_attr->u.asr.domains));
}

static td_void aai_show_vid_attr(td_void *file, const sea_eng_attr *eng_attr)
{
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "type", get_aai_type_name(eng_attr->type),
        "mode", get_sea_mode_name(eng_attr->mode),
        "sensitivity", eng_attr->u.vid.sensitivity,
        "min_dura_ms", eng_attr->u.vid.min_dura_ms);

    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "vid_count", eng_attr->u.vid.vid_count,
        "", "",
        "", "",
        "", "");
}

static td_void aai_show_asd_attr(td_void *file, const sea_eng_attr *eng_attr)
{
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
        "type", get_aai_type_name(eng_attr->type),
        "mode", get_sea_mode_name(eng_attr->mode),
        "latency", eng_attr->u.asd.latency,
        "", "");
}

static td_void show_aai_attr(td_void *file, const sea_eng_attr *eng_attr)
{
    drv_audio_proc_print_title(file, "aai attr [%u]", eng_attr->type);

    switch (eng_attr->type) {
        case SEA_AAI_KWS:
            aai_show_kws_attr(file, eng_attr);
            break;

        case SEA_AAI_ASR:
            aai_show_asr_attr(file, eng_attr);
            break;

        case SEA_AAI_VID:
            aai_show_vid_attr(file, eng_attr);
            break;

        case SEA_AAI_ASD:
            aai_show_asd_attr(file, eng_attr);
            break;

        default:
            break;
    }
}

static td_void show_in_pcm_format(td_void *file, const sea_pcm_format *in_pcm)
{
    drv_audio_proc_print_title(file, "in pcm format");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "channels", in_pcm->channels,
        "bit_depth", in_pcm->bit_depth,
        "sample_rate", in_pcm->sample_rate,
        "pcm_samples", in_pcm->samples_per_frame);
}

static td_void show_ref_pcm_format(td_void *file, const sea_pcm_format *ref_pcm)
{
    drv_audio_proc_print_title(file, "ref pcm format");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "channels", ref_pcm->channels,
        "bit_depth", ref_pcm->bit_depth,
        "sample_rate", ref_pcm->sample_rate,
        "pcm_samples", ref_pcm->samples_per_frame);
}

static td_void show_out_pcm_format(td_void *file, sea_output_type type, const sea_pcm_format *out_pcm)
{
    drv_audio_proc_print_title(file, "out pcm format [%s]", get_output_type_name(type));

    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "channels", out_pcm->channels,
        "bit_depth", out_pcm->bit_depth,
        "sample_rate", out_pcm->sample_rate,
        "pcm_samples", out_pcm->samples_per_frame);
}

static td_void show_input_buf(td_void *file, const audio_ring_buf *rbuf, td_u32 frame_pms)
{
    td_u32 data_size;
    td_u32 data_ratio, data_ms;

    data_size = drv_audio_proc_query_rbuf_busy(rbuf->size, rbuf->read_pos, rbuf->write_pos);
    data_ratio = data_size * 100 / rbuf->size; /* 100: percent(%) */
    data_ms = drv_audio_proc_get_data_ms(data_size, frame_pms);

    drv_audio_proc_print_title(file, "input buf");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "buf_addr", rbuf->buf_addr,
        "buf_size", rbuf->size,
        "read_pos", rbuf->read_pos,
        "write_pos", rbuf->write_pos);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "tail_size", rbuf->tail_size,
        "data_size", data_size,
        "data_ratio", data_ratio,
        "data_ms", data_ms);

    /* line 3 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
        "info_addr", rbuf->info_addr,
        "info_num", rbuf->info_num,
        "info_used", rbuf->info_used,
        "", "");
}

static td_void show_output_buf(td_void *file, sea_output_type type, td_u8 buf_index,
    const audio_ring_buf *rbuf, td_u32 frame_pms)
{
    td_u32 data_size;
    td_u32 data_ratio, data_ms;

    data_size = drv_audio_proc_query_rbuf_busy(rbuf->size, rbuf->read_pos, rbuf->write_pos);
    data_ratio = data_size * 100 / rbuf->size; /* 100: percent(%) */
    data_ms = drv_audio_proc_get_data_ms(data_size, frame_pms);

    drv_audio_proc_print_title(file, "output buf [%s][%d]", get_output_type_name(type), buf_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "buf_addr", rbuf->buf_addr,
        "buf_size", rbuf->size,
        "read_pos", rbuf->read_pos,
        "write_pos", rbuf->write_pos);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "tail_size", rbuf->tail_size,
        "data_size", data_size,
        "data_ratio", data_ratio,
        "data_ms", data_ms);

    /* line 3 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "overrun", rbuf->overrun_cnt,
        "info_addr", rbuf->info_addr,
        "info_num", rbuf->info_num,
        "info_used", rbuf->info_used);
}

static td_void show_sea_version(td_void *file, sea_version_info *ver_info)
{
    td_u8 i;
    version_info_str sea_ver_info[SEA_ECH_MAX_NUM] = { 0 };

    drv_audio_proc_print_title(file, "sea version");

    for (i = 0; i < SEA_ECH_MAX_NUM; i++) {
        if (ver_info[i].sea_version.u32 != 0) {
            (td_void)snprintf_s(sea_ver_info[i].version, SEA_VERSION_LEN, SEA_VERSION_LEN - 1, "%u.%u.%u.%u",
                ver_info[i].sea_version.bits.version_major, ver_info[i].sea_version.bits.version_minor,
                ver_info[i].sea_version.bits.revision, ver_info[i].sea_version.bits.step);

            (td_void)memcpy_s(sea_ver_info[i].name, SEA_NAME_LEN,
                get_sea_name(ver_info[i].sea_id), SEA_NAME_LEN);
        }
    }

    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        sea_ver_info[0].name, sea_ver_info[0].version,
        sea_ver_info[1].name, sea_ver_info[1].version, /* 1:first element */
        sea_ver_info[2].name, sea_ver_info[2].version, /* 2:second element */
        sea_ver_info[3].name, sea_ver_info[3].version); /* 3:third element */
}

td_s32 drv_proc_sea_show(td_void *file, td_handle sea, const sea_proc_item *proc)
{
    td_u8 i, j;
    const sea_eng_attr *sea_eng = TD_NULL;

    drv_audio_proc_print_head(file, "sea%d", drv_audio_proc_get_mod_index(sea));

    show_sea_version(file, (sea_version_info *)&(proc->version_info[0]));

    show_basic_status(file, sea, proc);

    for (i = 0; i < SEA_ECH_MAX_NUM; i++) {
        sea_eng = &proc->eng_attr[i];
        if (sea_eng->type > SEA_AFE_NULL && sea_eng->type < SEA_AFE_MAX) {
            show_afe_attr(file, sea_eng);
        } else if (sea_eng->type > SEA_AAI_NULL && sea_eng->type < SEA_AAI_MAX) {
            show_aai_attr(file, sea_eng);
        } else {
        }
    }

    /* input */
    show_in_pcm_format(file, &(proc->pcm_attr.in_pcm));
    show_ref_pcm_format(file, &(proc->pcm_attr.ref_pcm));
    if (proc->rbuf_in.handle != 0) {
        show_input_buf(file, &(proc->rbuf_in), proc->in_frame_pms);
    }

    /* output */
    for (i = 0; i < SEA_OUTPUT_MAX; i++) {
        show_out_pcm_format(file, i, &(proc->pcm_attr.out_pcm[i]));
        for (j = 0; j < SEA_RBUF_MAX_NUM; j++) {
            if (proc->rbuf_out[i][j].handle != 0) {
                show_output_buf(file, i, j, &(proc->rbuf_out[i][j]), proc->out_frame_pms[i]);
            }
        }
    }

    drv_audio_proc_print_tail(file);

    return AUDIO_SUCCESS;
}

#ifndef SW_UART_DEBUG
td_s32 drv_diag_log_proc_sea_show(td_handle sea, const sea_proc_item *proc)
{
    td_u8 i;
    td_u8 j;
    td_u32 data_size;

    uapi_diag_info_log4(0, "sea_handle = 0x%x, anr_enable = %u, aec_enable = %u, agc_enable = %u\n",
        sea,
        check_option_valid(proc->eng_attr[0].u.see.options, SEA_AFE_OPT_ANR),
        check_option_valid(proc->eng_attr[0].u.see.options, SEA_AFE_OPT_AEC),
        check_option_valid(proc->eng_attr[0].u.see.options, SEA_AFE_OPT_AGC));

    data_size = drv_audio_proc_query_rbuf_busy(proc->rbuf_in.size, proc->rbuf_in.read_pos, proc->rbuf_in.write_pos);
    uapi_diag_info_log4(0, "sea input: buf_size = %u, buf_wptr = %u, buf_rptr = %u, data_size = %u\n",
        proc->rbuf_in.size,
        proc->rbuf_in.write_pos,
        proc->rbuf_in.read_pos,
        data_size);

    for (i = 0; i < SEA_OUTPUT_MAX; i++) {
        for (j = 0; j < SEA_RBUF_MAX_NUM; j++) {
            if (proc->rbuf_out[i][j].handle != 0) {
                data_size = drv_audio_proc_query_rbuf_busy(proc->rbuf_out[i][j].size,
                    proc->rbuf_out[i][j].read_pos, proc->rbuf_out[i][j].write_pos);
                uapi_diag_info_log4(0, "sea output: buf_size = %u, buf_wptr = %u, buf_rptr = %u, data_size = %u\n",
                    proc->rbuf_out[i][j].size,
                    proc->rbuf_out[i][j].write_pos,
                    proc->rbuf_out[i][j].read_pos,
                    data_size);
            }
        }
    }

    return AUDIO_SUCCESS;
}
#endif /* SW_UART_DEBUG */
#endif /* SAP_PROC_SUPPORT */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
