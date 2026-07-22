/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: api of drv audio proc
 * Author: audio
 */

#include "securec.h"
#include "drv_audio_osal.h"
#include "audio_debug.h"
#include "drv_audio_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct {
    ha_codec_id acodec_id;
    const td_char *acodec_name;
} acodec_proc_name_item;

typedef struct {
    audio_format format;
    const td_char *name;
} format_proc_name_item;

static const acodec_proc_name_item g_acodec_proc_name_list[] = {
    {HA_CODEC_ID_PCM, "pcm"},
    {HA_CODEC_ID_MP3, "mp3"},
    {HA_CODEC_ID_AAC, "aac"},
    {HA_CODEC_ID_SBC, "sbc"},
    {HA_CODEC_ID_MSBC, "msbc"},
    {HA_CODEC_ID_OPUS, "opus"},
    {HA_CODEC_ID_FLAC, "flac"},
    {HA_CODEC_ID_L2HC, "l2hc"},
    {HA_CODEC_ID_LC3, "lc3"},
    {HA_CODEC_ID_SILK, "silk"},
    {HA_CODEC_ID_VORBIS, "vorbis"},
    {HA_CODEC_ID_AMRWB, "amrwb"},
};

static const format_proc_name_item g_format_proc_name_list[] = {
    {AUDIO_FORMAT_STREAM, "stream"},
    {AUDIO_FORMAT_PCM, "pcm"},
    {AUDIO_FORMAT_AC3, "ac3"},
    {AUDIO_FORMAT_MPEG1, "mpeg1"},
    {AUDIO_FORMAT_MP3, "mp3"},
    {AUDIO_FORMAT_MPEG2, "mpeg2"},
    {AUDIO_FORMAT_AAC_LC, "aac_lc"},
    {AUDIO_FORMAT_DTS, "dts"},
    {AUDIO_FORMAT_ATRAC, "atrac"},
    {AUDIO_FORMAT_DSD, "dsd"},
    {AUDIO_FORMAT_EAC3, "eac3"},
    {AUDIO_FORMAT_DTS_HD, "dts_hd"},
    {AUDIO_FORMAT_MAT, "mat"},
    {AUDIO_FORMAT_DST, "dst"},
    {AUDIO_FORMAT_WMA_PRO, "wma_pro"},
    {AUDIO_FORMAT_CXT, "cxt"},
};

td_char *drv_audio_proc_get_i2s_mode(audio_i2s_mode mode)
{
    if (mode == I2S_STD_MODE) {
        return "i2s_std";
    } else if (mode == I2S_PCM_MODE) {
        return "i2s_pcm";
    } else if (mode == I2S_TDM_MODE) {
        return "i2s_tdm";
    } else if (mode == I2S_PDM_MODE) {
        return "i2s_pdm";
    } else {
        return "null";
    }
}

td_char *drv_audio_proc_get_i2s_mclk(audio_i2s_mclk mclk)
{
    if (mclk == I2S_MCLK_128_FS) {
        return "128*fs";
    } else if (mclk == I2S_MCLK_256_FS) {
        return "256*fs";
    } else if (mclk == I2S_MCLK_384_FS) {
        return "384*fs";
    } else if (mclk == I2S_MCLK_512_FS) {
        return "512*fs";
    } else if (mclk == I2S_MCLK_768_FS) {
        return "768*fs";
    } else if (mclk == I2S_MCLK_1024_FS) {
        return "1024*fs";
    } else if (mclk == I2S_MCLK_1536_FS) {
        return "1536*fs";
    } else {
        return "null";
    }
}

#ifdef SAP_PROC_SUPPORT
td_s32 drv_audio_proc_print_head(td_void *file, const td_char *format, ...)
{
    td_s32 i;
    td_s32 len;
    va_list args;

    td_char title[PROC_MAX_TITLE_FORMAT_LEN + 1] = {0};
    td_char head_title_format[] = {
        [0 ... PROC_MAX_TITLE_FORMAT_LEN - 1] = '=', '\n', '\0'
    };

    va_start(args, format);
    len = vsnprintf_s(title, PROC_MAX_TITLE_FORMAT_LEN, PROC_MAX_TITLE_FORMAT_LEN - 1, format, args);
    va_end(args);

    if (len < 0) {
        sap_err_log_fun(vsnprintf_s, len);
        return AUDIO_FAILURE;
    }

    for (i = PROC_TITLE_PREFIX_LEN; i < PROC_TITLE_PREFIX_LEN + len; i++) {
        head_title_format[i] = title[i - PROC_TITLE_PREFIX_LEN];
    }

    drv_audio_osal_proc_print(file, "%s", head_title_format);

    return AUDIO_SUCCESS;
}

td_void drv_audio_proc_print_tail(td_void *file)
{
    td_s32 i;
    td_char title_format[PROC_MAX_TITLE_FORMAT_LEN + 2] = {'\0'}; /* 2: add '\n'&'\0' */

    for (i = 0; i < (PROC_MAX_TITLE_FORMAT_LEN - 1); i += 2) { /* 2: one '=' + one space */
        title_format[i] = '=';
        title_format[i + 1] = ' ';
    }
    title_format[PROC_MAX_TITLE_FORMAT_LEN] = '\n';

    drv_audio_osal_proc_print(file, "%s", title_format);
}

td_s32 drv_audio_proc_print_title(td_void *file, const td_char *format, ...)
{
    td_s32 i;
    td_s32 len;
    va_list args;

    td_char title[PROC_MAX_TITLE_FORMAT_LEN + 1] = {0};
    td_char title_format[] = {
        [0 ... PROC_MAX_TITLE_FORMAT_LEN - 1] = '-', '\n', '\0'
    };

    va_start(args, format);
    len = vsnprintf_s(title, PROC_MAX_TITLE_FORMAT_LEN, PROC_MAX_TITLE_FORMAT_LEN - 1, format, args);
    va_end(args);

    if (len < 0) {
        sap_err_log_fun(vsnprintf_s, len);
        return AUDIO_FAILURE;
    }

    for (i = PROC_TITLE_PREFIX_LEN; i < PROC_TITLE_PREFIX_LEN + len; i++) {
        title_format[i] = title[i - PROC_TITLE_PREFIX_LEN];
    }

    drv_audio_osal_proc_print(file, "%s", title_format);

    return AUDIO_SUCCESS;
}
#endif

const td_char *drv_audio_proc_get_acodec_name(ha_codec_id id)
{
    td_u32 i;

    for (i = 0; i < (sizeof(g_acodec_proc_name_list) / sizeof(g_acodec_proc_name_list[0])); i++) {
        if (id == g_acodec_proc_name_list[i].acodec_id) {
            return g_acodec_proc_name_list[i].acodec_name;
        }
    }

    return "unknown";
}

const td_char *drv_audio_proc_get_format_name(audio_format format)
{
    td_u32 i;

    for (i = 0; i < (sizeof(g_format_proc_name_list) / sizeof(g_format_proc_name_list[0])); i++) {
        if (format == g_format_proc_name_list[i].format) {
            return g_format_proc_name_list[i].name;
        }
    }

    return "unknown";
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
