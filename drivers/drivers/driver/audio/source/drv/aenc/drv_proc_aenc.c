/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: source file for drv aenc proc
 * Author: audio
 * Create: 2021-08-08
 */

#include "securec.h"
#include "audio_struct.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "drv_audio_proc.h"
#include "drv_audio_osal.h"
#include "drv_proc_aenc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef SAP_PROC_SUPPORT

/* SBC codec info */
typedef union {
    struct {
        unsigned int chnl_mode : 4;        /* [3..0] */
        unsigned int sample_frequency : 4; /* [7..4] */
        unsigned int alloc_method : 2;     /* [9..8] */
        unsigned int subband : 2;          /* [11..10] */
        unsigned int block_length : 4;     /* [15..12] */
        unsigned int min_bitpool : 8;      /* [23..16] */
        unsigned int max_bitpool : 8;      /* [31..24] */
    } bits;

    unsigned int u32;
} a2dp_sbc_codec_caps;

/* Channel modes */
typedef enum {
    A2DP_SBC_CHANNELS_JOINT_STEREO = 1,
    A2DP_SBC_CHANNELS_STEREO = 2,
    A2DP_SBC_CHANNELS_DUAL = 4,
    A2DP_SBC_CHANNELS_MONO = 8,
} a2dp_sbc_channels;

typedef struct {
    a2dp_sbc_channels channel_mode;
    const td_char *name;
} sbc_channel_mode_name_item;

static const sbc_channel_mode_name_item g_sbc_channel_mode_name_list[] = {
    {A2DP_SBC_CHANNELS_JOINT_STEREO, "joint"},
    {A2DP_SBC_CHANNELS_STEREO, "stereo"},
    {A2DP_SBC_CHANNELS_DUAL, "dual"},
    {A2DP_SBC_CHANNELS_MONO, "mono"},
};

/* Sampling Frequency */
typedef enum {
    A2DP_SBC_SAMPLE_FREQ_48K = 1,
    A2DP_SBC_SAMPLE_FREQ_44K = 2,
    A2DP_SBC_SAMPLE_FREQ_32K = 4,
    A2DP_SBC_SAMPLE_FREQ_16K = 8,
} a2dp_sbc_sample_freq;

typedef struct {
    a2dp_sbc_sample_freq sample_freq;
    td_u32 sample_rate;
} sbc_sample_freq_item;

static const sbc_sample_freq_item g_sbc_sample_freq_list[] = {
    {A2DP_SBC_SAMPLE_FREQ_48K, 48000},
    {A2DP_SBC_SAMPLE_FREQ_44K, 44100},
    {A2DP_SBC_SAMPLE_FREQ_32K, 32000},
    {A2DP_SBC_SAMPLE_FREQ_16K, 16000},
};

/* Bit allocation methods */
typedef enum {
    A2DP_SBC_ALLOC_METHOD_LOUDNESS = 1,
    A2DP_SBC_ALLOC_METHOD_SNR = 2,
} a2dp_sbc_alloc_method;

typedef struct {
    a2dp_sbc_alloc_method alloc_method;
    const td_char *name;
} sbc_alloc_method_name_item;

static const sbc_alloc_method_name_item g_sbc_alloc_method_name_list[] = {
    {A2DP_SBC_ALLOC_METHOD_LOUDNESS, "loudness"},
    {A2DP_SBC_ALLOC_METHOD_SNR, "snr"},
};

/* Sub_bands */
typedef enum {
    A2DP_SBC_SUB_BANDS_8 = 1,
    A2DP_SBC_SUB_BANDS_4 = 2,
} a2dp_sbc_sub_bands;

typedef struct {
    a2dp_sbc_sub_bands sub_bands;
    td_u32 sub_bands_num;
} sbc_sub_bands_item;

static const sbc_sub_bands_item g_sbc_sub_bands_list[] = {
    {A2DP_SBC_SUB_BANDS_8, 8},
    {A2DP_SBC_SUB_BANDS_4, 4},
};

/* Block lengths */
typedef enum {
    A2DP_SBC_BLOCKS_16 = 1,
    A2DP_SBC_BLOCKS_12 = 2,
    A2DP_SBC_BLOCKS_08 = 4,
    A2DP_SBC_BLOCKS_04 = 8,
} a2dp_sbc_blocks;

typedef struct {
    a2dp_sbc_blocks blocks;
    td_u32 blocks_num;
} sbc_blocks_item;

static const sbc_blocks_item g_sbc_blocks_list[] = {
    {A2DP_SBC_BLOCKS_16, 16},
    {A2DP_SBC_BLOCKS_12, 12},
    {A2DP_SBC_BLOCKS_08, 8},
    {A2DP_SBC_BLOCKS_04, 4},
};

static const td_char *get_sbc_channel_mode_name(a2dp_sbc_channels channel_mode)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_sbc_channel_mode_name_list) / sizeof(g_sbc_channel_mode_name_list[0])); i++) {
        if (channel_mode == g_sbc_channel_mode_name_list[i].channel_mode) {
            return g_sbc_channel_mode_name_list[i].name;
        }
    }
    return "unknown";
}

static td_u32 get_sbc_sample_rate(a2dp_sbc_sample_freq sample_freq)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_sbc_sample_freq_list) / sizeof(g_sbc_sample_freq_list[0])); i++) {
        if (sample_freq == g_sbc_sample_freq_list[i].sample_freq) {
            return g_sbc_sample_freq_list[i].sample_rate;
        }
    }
    return 0;
}

static const td_char *get_sbc_alloc_method_name(a2dp_sbc_alloc_method alloc_method)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_sbc_alloc_method_name_list) / sizeof(g_sbc_alloc_method_name_list[0])); i++) {
        if (alloc_method == g_sbc_alloc_method_name_list[i].alloc_method) {
            return g_sbc_alloc_method_name_list[i].name;
        }
    }
    return "unknown";
}

static td_u32 get_sbc_sub_bands_num(a2dp_sbc_sub_bands sub_bands)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_sbc_sub_bands_list) / sizeof(g_sbc_sub_bands_list[0])); i++) {
        if (sub_bands == g_sbc_sub_bands_list[i].sub_bands) {
            return g_sbc_sub_bands_list[i].sub_bands_num;
        }
    }
    return 0;
}

static td_u32 get_sbc_blocks_num(a2dp_sbc_blocks blocks)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_sbc_blocks_list) / sizeof(g_sbc_blocks_list[0])); i++) {
        if (blocks == g_sbc_blocks_list[i].blocks) {
            return g_sbc_blocks_list[i].blocks_num;
        }
    }
    return 0;
}

static td_void drv_aenc_proc_show_attr(td_void *file, const aenc_proc_item *proc)
{
    drv_audio_proc_print_title(file, "attr");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "acodec name", drv_audio_proc_get_acodec_name(proc->codec_id),
        "in_buf_dur_ms", proc->ibuf_dur_ms,
        "out_buf_dur_ms", proc->obuf_dur_ms,
        "max_bit_out_size", proc->max_bit_out_size);

    if (proc->codec_id == HA_CODEC_ID_AMRWB) {
        /* line 2 */
        drv_audio_osal_proc_print(file,
            "%-16s: %-10d |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
            "bit_rate", proc->bit_rate,
            "", "",
            "", "",
            "", "");
    }
}

static td_void drv_aenc_proc_show_sbc_private_data(td_void *file, const aenc_proc_item *proc)
{
    td_s32 ret;
    a2dp_sbc_codec_caps data;

    if (proc->private_data_size == 0) {
        return;
    }

    ret = memcpy_s(&data, sizeof(a2dp_sbc_codec_caps), proc->private_data, proc->private_data_size);
    if (ret != EOK) {
        sap_err_log_fun(memcpy_s, ret);
        return;
    }
    drv_audio_proc_print_title(file, "private data");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10d |" "%-16s: %-10s |" "%-16s: %-10d |" "\n",
        "channel_mode", get_sbc_channel_mode_name(data.bits.chnl_mode),
        "sample_rate", get_sbc_sample_rate(data.bits.sample_frequency),
        "alloc_method", get_sbc_alloc_method_name(data.bits.alloc_method),
        "subband", get_sbc_sub_bands_num(data.bits.subband));

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
        "block_length", get_sbc_blocks_num(data.bits.block_length),
        "min_bitpool", data.bits.min_bitpool,
        "max_bitpool", data.bits.max_bitpool,
        "", "");
}

static td_void drv_aenc_proc_show_in_pcm_format(td_void *file, const aenc_proc_item *proc)
{
    drv_audio_proc_print_title(file, "in pcm format");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "channels", proc->pcm_attr.channels,
        "bit_depth", proc->pcm_attr.bit_depth,
        "sample_rate", proc->pcm_attr.sample_rate,
        "pcm_samples", proc->pcm_attr.samples_per_frame);
}

static td_void drv_aenc_proc_show_basic_status(td_void *file, td_handle aenc, const aenc_proc_item *proc)
{
    drv_audio_proc_print_title(file, "basic status");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "handle", aenc,
        "state", drv_audio_proc_get_state((audio_state)(proc->state)),
        "bypass", drv_audio_proc_get_bool(proc->bypass),
        "eos", drv_audio_proc_get_bool(proc->eos));

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "frame_pms", proc->frame_pms,
        "frame_size", proc->frame_size,
        "proc_data_size", proc->proc_data_count,
        "underrun_cnt", proc->not_enough_data_cnt);
}

static td_void drv_aenc_proc_show_in_buf(td_void *file, const audio_ring_buf *rbuf_in, td_u32 frame_pms)
{
    td_u32 data_size;
    td_u32 data_ratio, data_ms;

    data_size = drv_audio_proc_query_rbuf_busy(rbuf_in->size, rbuf_in->read_pos, rbuf_in->write_pos);
    data_ratio = data_size * 100 / rbuf_in->size; /* 100: percent(%) */
    data_ms = drv_audio_proc_get_data_ms(data_size, frame_pms);

    drv_audio_proc_print_title(file, "in buf");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "buf_addr", rbuf_in->buf_addr,
        "buf_size", rbuf_in->size,
        "read_pos", rbuf_in->read_pos,
        "write_pos", rbuf_in->write_pos);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "tail_size", rbuf_in->tail_size,
        "data_size", data_size,
        "data_ratio", data_ratio,
        "data_ms", data_ms);

    /* line 3 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
        "info_addr", rbuf_in->info_addr,
        "info_num", rbuf_in->info_num,
        "info_used", rbuf_in->info_used,
        "", "");
}

static td_void drv_aenc_proc_show_out_buf(td_void *file, td_u8 buf_index, const audio_ring_buf *rbuf_out)
{
    td_u32 data_size;
    td_u32 data_ratio;

    data_size = drv_audio_proc_query_rbuf_busy(rbuf_out->size, rbuf_out->read_pos, rbuf_out->write_pos);
    data_ratio = data_size * 100 / rbuf_out->size; /* 100: percent(%) */

    drv_audio_proc_print_title(file, "out buf[%d]", buf_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "buf_addr", rbuf_out->buf_addr,
        "buf_size", rbuf_out->size,
        "read_pos", rbuf_out->read_pos,
        "write_pos", rbuf_out->write_pos);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10u |" "\n",
        "tail_size", rbuf_out->tail_size,
        "data_size", data_size,
        "data_ratio", data_ratio,
        "overrun", rbuf_out->overrun_cnt);

    /* line 3 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
        "info_addr", rbuf_out->info_addr,
        "info_num", rbuf_out->info_num,
        "info_used", rbuf_out->info_used,
        "", "");
}

td_s32 drv_proc_aenc_show(td_void *file, td_handle aenc, const aenc_proc_item *proc)
{
    td_u8 i;
    drv_audio_proc_print_head(file, "aenc%d", drv_audio_proc_get_mod_index(aenc));

    drv_aenc_proc_show_attr(file, proc);

    if (proc->codec_id == HA_CODEC_ID_SBC) {
        drv_aenc_proc_show_sbc_private_data(file, proc);
    }

    drv_aenc_proc_show_basic_status(file, aenc, proc);

    /* input */
    drv_aenc_proc_show_in_pcm_format(file, proc);
    if (proc->rbuf_in.handle != 0) {
        drv_aenc_proc_show_in_buf(file, &(proc->rbuf_in), proc->frame_pms);
    }

    /* output */
    for (i = 0; i < AENC_RBUF_MAX_NUM; i++) {
        if (proc->rbuf_out[i].handle != 0) {
            drv_aenc_proc_show_out_buf(file, i, &(proc->rbuf_out[i]));
        }
    }

    drv_audio_proc_print_tail(file);

    return AUDIO_SUCCESS;
}

#ifndef SW_UART_DEBUG
td_s32 drv_diag_log_proc_aenc_show(td_handle aenc, const aenc_proc_item *proc)
{
    td_u8 i;
    td_u32 data_size;

    uapi_diag_info_log3(0, "aenc_handle = 0x%x, codec = 0x%x, proc_data_size = %u\n",
        aenc,
        proc->codec_id,
        proc->proc_data_count);

    data_size = drv_audio_proc_query_rbuf_busy(proc->rbuf_in.size, proc->rbuf_in.read_pos, proc->rbuf_in.write_pos);
    uapi_diag_info_log4(0, "aenc input: buf_size = %u, buf_wptr = %u, buf_rptr = %u, data_size = %u\n",
        proc->rbuf_in.size,
        proc->rbuf_in.write_pos,
        proc->rbuf_in.read_pos,
        data_size);

    for (i = 0; i < AENC_RBUF_MAX_NUM; i++) {
        if (proc->rbuf_out[i].handle != 0) {
            data_size = drv_audio_proc_query_rbuf_busy(proc->rbuf_out[i].size,
                proc->rbuf_out[i].read_pos, proc->rbuf_out[i].write_pos);
            uapi_diag_info_log4(0, "aenc output: buf_size = %u, buf_wptr = %u, buf_rptr = %u, data_size = %u\n",
                proc->rbuf_out[i].size,
                proc->rbuf_out[i].write_pos,
                proc->rbuf_out[i].read_pos,
                data_size);
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
