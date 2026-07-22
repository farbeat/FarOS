/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: source file for drv ao proc
 * Author: audio
 * Create: 2021-07-01
 */

#include "drv_proc_ao.h"
#include "audio_struct.h"
#include "audio_aef.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "drv_audio_proc.h"
#include "drv_audio_osal.h"
#include "core_util.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef SAP_PROC_SUPPORT

typedef struct {
    ao_port_id port_id;
    const td_char *name;
} ao_port_proc_name_item;

static const ao_port_proc_name_item g_ao_port_proc_name_list[] = {
    {AO_PORT_DAC0, "DAC0"},
    {AO_PORT_DAC1, "DAC1"},
    {AO_PORT_DAC2, "DAC2"},
    {AO_PORT_DAC3, "DAC3"},

    {AO_PORT_I2S0, "I2S0"},
    {AO_PORT_I2S1, "I2S1"},
    {AO_PORT_I2S2, "I2S2"},
    {AO_PORT_I2S3, "I2S3"},

    {AO_PORT_SPDIF0, "SPDIF0"},

    {AO_PORT_HDMI0, "HDMI0"},
    {AO_PORT_HDMI1, "HDMI1"},

    {AO_PORT_ARC0, "ARC0"},
    {AO_PORT_ARC1, "ARC1"},

    {AO_PORT_USB0, "USB0"},
    {AO_PORT_USB1, "USB1"},

    {AO_PORT_BT0, "BT0"},
    {AO_PORT_BT1, "BT1"},

    {AO_PORT_CAST0, "CAST0"},
    {AO_PORT_CAST1, "CAST1"},

    {AO_PORT_ALL, "ALL"},
};

typedef struct {
    aef_type type;
    const td_char *name;
} aef_type_name_item;

static const aef_type_name_item g_aef_type_name_list[] = {
    {AEF_TYPE_NULL, "none"},
    {AEF_TYPE_SWS1, "sws1"},
    {AEF_TYPE_SMARTPA, "smart_pa"},
};

static const td_char *drv_ao_proc_aef_type_name(aef_type type)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_aef_type_name_list) / sizeof(g_aef_type_name_list[0])); i++) {
        if (type == g_aef_type_name_list[i].type) {
            return g_aef_type_name_list[i].name;
        }
    }

    return "unknown";
}

static const td_char *drv_ao_proc_get_port_name(ao_port_id port_id)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_ao_port_proc_name_list) / sizeof(g_ao_port_proc_name_list[0])); i++) {
        if (port_id == g_ao_port_proc_name_list[i].port_id) {
            return g_ao_port_proc_name_list[i].name;
        }
    }

    return "unknown";
}

typedef struct {
    ao_aef_profile profile;
    const td_char *name;
} aef_profile_name_item;

static const aef_profile_name_item g_aef_profile_name_list[] = {
    {AO_AEF_PROFILE_NONE, "none"},
    {AO_AEF_PROFILE_MUSIC, "music"},
    {AO_AEF_PROFILE_VOIP, "voip"},
    {AO_AEF_PROFILE_RINGTONE, "RINGTONE"},
};

static const td_char *drv_ao_proc_aef_profile_name(ao_aef_profile profile)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_aef_profile_name_list) / sizeof(g_aef_profile_name_list[0])); i++) {
        if (profile == g_aef_profile_name_list[i].profile) {
            return g_aef_profile_name_list[i].name;
        }
    }

    return "unknown";
}

static td_char *drv_ao_proc_get_ao_track_mode(ao_track_mode mode)
{
    switch (mode) {
        case AO_TRACK_STEREO:
            return "stereo";
        case AO_TRACK_DOUBLE_MONO:
            return "dual_mono";
        case AO_TRACK_DOUBLE_LEFT:
            return "dual_left";
        case AO_TRACK_DOUBLE_RIGHT:
            return "dual_right";
        case AO_TRACK_EXCHANGE:
            return "exchange";
        case AO_TRACK_ONLY_RIGHT:
            return "only_right";
        case AO_TRACK_ONLY_LEFT:
            return "only_left";
        case AO_TRACK_MUTE:
            return "mute";
        default:
            return "unknown";
    }
}

static td_void drv_ao_proc_show_port_attr(td_void *file, td_u8 port_index,
    const ao_out_port_attr *port_attr)
{
    if ((port_attr->out_port >= AO_PORT_I2S0) && (port_attr->out_port <= AO_PORT_I2S4)) {
        drv_audio_proc_print_title(file, "port[%d] attr", port_index);

        /* line 1 */
        drv_audio_osal_proc_print(file,
            "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10d |" "\n",
            "master", drv_audio_proc_get_bool(port_attr->u.i2s_attr.attr.master),
            "i2s_mode", drv_audio_proc_get_i2s_mode((audio_i2s_mode)port_attr->u.i2s_attr.attr.i2s_mode),
            "mclk", drv_audio_proc_get_i2s_mclk((audio_i2s_mclk)port_attr->u.i2s_attr.attr.mclk),
            "bclk", port_attr->u.i2s_attr.attr.bclk);

        /* line 2 */
        drv_audio_osal_proc_print(file,
            "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "%-16s: %-10d |" "\n",
            "bit_depth", port_attr->u.i2s_attr.attr.bit_depth,
            "channels", port_attr->u.i2s_attr.attr.channels,
            "pcm_rise_edge", drv_audio_proc_get_pcm_rise_edge(port_attr->u.i2s_attr.attr.sample_rise_edge),
            "pcm_delay_cycle", port_attr->u.i2s_attr.attr.pcm_delay_cycle);
    }
}

static td_void drv_ao_proc_show_port_buf(td_void *file, td_u8 port_index, const aop_proc_item *proc)
{
    td_u32 data_size, frame_pms;
    td_u32 data_ratio, data_ms;

    frame_pms = calc_frame_size_per_ms(&proc->pcm_attr);
    data_size = drv_audio_proc_query_rbuf_busy(proc->tx_buf.buff_size, proc->tx_buf.buff_rptr, proc->tx_buf.buff_wptr);
    data_ratio = data_size * 100 / proc->tx_buf.buff_size; /* 100: percent(%) */
    data_ms = drv_audio_proc_get_data_ms(data_size, frame_pms);

    drv_audio_proc_print_title(file, "port[%d] buf", port_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "buf_addr", proc->tx_buf.buff_addr,
        "buf_size", proc->tx_buf.buff_size,
        "buf_wptr", proc->tx_buf.buff_wptr,
        "buf_rptr", proc->tx_buf.buff_rptr);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "period_buf_size", proc->tx_buf.period_buf_size,
        "threshold_size", proc->tx_buf.threshold_size,
        "data_size", data_size,
        "data_ratio", data_ratio);

    /* line 3 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "data_ms", data_ms,
        "", "",
        "", "",
        "", "");
}

static td_void drv_ao_proc_show_port_dma_status(td_void *file, td_u8 port_index, const aop_proc_item *proc)
{
    drv_audio_proc_print_title(file, "port[%d] dma status", port_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "dma_cnt", proc->tx_status.dma_cnt,
        "bus_time_out", proc->tx_status.bus_time_out_cnt,
        "total_byte_write", proc->tx_status.total_byte_write,
        "buf_empty_cnt", proc->tx_status.buf_empty_cnt);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "buf_empty_warn", proc->tx_status.buf_empty_warning_cnt,
        "bus_fifo_empty", proc->tx_status.bus_fifo_empty_cnt,
        "inf_fifo_empty", proc->tx_status.inf_fifo_empty_cnt,
        "inf_empty_real", proc->tx_status.inf_empty_cnt_real);
}

static td_void drv_ao_proc_show_port_run_status(td_void *file, td_u8 port_index, const aop_proc_item *proc)
{
    drv_audio_proc_print_title(file, "port[%d] run status", port_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "xrun_stage_cnt", proc->run_status.xrun_stage_cnt,
        "", "",
        "", "",
        "", "");

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "max_xrun_1_cnt", proc->run_status.max_xrun[0].xrun_cnt,
        "max_xrun_2_cnt", proc->run_status.max_xrun[1].xrun_cnt,
        "max_xrun_3_cnt", proc->run_status.max_xrun[2].xrun_cnt, /* 2:index */
        "max_xrun_4_cnt", proc->run_status.max_xrun[3].xrun_cnt); /* 3:index */

    /* line 3 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "max_xrun_1_pos", proc->run_status.max_xrun[0].start_pos,
        "max_xrun_2_pos", proc->run_status.max_xrun[1].start_pos,
        "max_xrun_3_pos", proc->run_status.max_xrun[2].start_pos, /* 2:index */
        "max_xrun_4_pos", proc->run_status.max_xrun[3].start_pos); /* 3:index */

    /* line 4 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "last_xrun_1_cnt", proc->run_status.last_xrun[0].xrun_cnt,
        "last_xrun_2_cnt", proc->run_status.last_xrun[1].xrun_cnt,
        "last_xrun_3_cnt", proc->run_status.last_xrun[2].xrun_cnt, /* 2:index */
        "last_xrun_4_cnt", proc->run_status.last_xrun[3].xrun_cnt); /* 3:index */

    /* line 5 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "last_xrun_1_pos", proc->run_status.last_xrun[0].start_pos,
        "last_xrun_2_pos", proc->run_status.last_xrun[1].start_pos,
        "last_xrun_3_pos", proc->run_status.last_xrun[2].start_pos, /* 2:index */
        "last_xrun_4_pos", proc->run_status.last_xrun[3].start_pos); /* 3:index */
}

static td_void drv_ao_proc_show_engine_basic_status(td_void *file, td_handle ao, const ao_proc_item *proc)
{
    drv_audio_proc_print_title(file, "engine basic status");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "handle", ao,
        "frame_size", proc->engine_proc.frame_size,
        "aip_num", proc->engine_proc.aip_num,
        "aop_num", proc->engine_proc.aop_num);
}

static td_void drv_ao_proc_show_engine_pcm_format(td_void *file, const ao_proc_item *proc)
{
    drv_audio_proc_print_title(file, "engine pcm format");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "channels", proc->engine_proc.pcm_attr.channels,
        "bit_depth", proc->engine_proc.pcm_attr.bit_depth,
        "sample_rate", proc->engine_proc.pcm_attr.sample_rate,
        "pcm_samples", proc->engine_proc.pcm_attr.samples_per_frame);
}

static td_void drv_ao_proc_show_aip_status(td_void *file, td_u8 aip_index, const aip_proc_item *proc)
{
    drv_audio_proc_print_title(file, "aip[%d] status", aip_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "handle", proc->handle,
        "state", drv_audio_proc_get_state((audio_state)proc->state),
        "bypass", drv_audio_proc_get_bool(proc->bypass),
        "eos", drv_audio_proc_get_bool(proc->eos));

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "input_pts", proc->input_pts,
        "play_pts", proc->play_pts,
        "volume_integer", proc->gain.integer,
        "volume_decimal", proc->gain.decimal);
}

static td_void drv_ao_proc_show_aip_in_pcm_format(td_void *file, td_u8 aip_index, const aip_proc_item *proc)
{
    drv_audio_proc_print_title(file, "aip[%d] in pcm format", aip_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "channels", proc->in_pcm_attr.channels,
        "bit_depth", proc->in_pcm_attr.bit_depth,
        "sample_rate", proc->in_pcm_attr.sample_rate,
        "pcm_samples", proc->in_pcm_attr.samples_per_frame);
}

static td_void drv_ao_proc_show_aip_out_pcm_format(td_void *file, td_u8 aip_index, const aip_proc_item *proc)
{
    drv_audio_proc_print_title(file, "aip[%d] out pcm format", aip_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "channels", proc->out_pcm_attr.channels,
        "bit_depth", proc->out_pcm_attr.bit_depth,
        "sample_rate", proc->out_pcm_attr.sample_rate,
        "pcm_samples", proc->out_pcm_attr.samples_per_frame);
}

static td_void drv_ao_proc_show_aip_in_buf(td_void *file, td_u8 aip_index, const aip_proc_item *proc)
{
    td_u32 data_size;
    td_u32 data_ratio, data_ms;

    data_size = drv_audio_proc_query_rbuf_busy(proc->rbuf_in.size, proc->rbuf_in.read_pos, proc->rbuf_in.write_pos);
    data_ratio = data_size * 100 / proc->rbuf_in.size; /* 100: percent(%) */
    data_ms = drv_audio_proc_get_data_ms(data_size, proc->in_frame_pms);

    drv_audio_proc_print_title(file, "aip[%d] in buf", aip_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "buf_addr", proc->rbuf_in.buf_addr,
        "buf_size", proc->rbuf_in.size,
        "read_pos", proc->rbuf_in.read_pos,
        "write_pos", proc->rbuf_in.write_pos);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "tail_size", proc->rbuf_in.tail_size,
        "data_size", data_size,
        "data_ratio", data_ratio,
        "data_ms", data_ms);

    /* line 3 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
        "info_addr", proc->rbuf_in.info_addr,
        "info_num", proc->rbuf_in.info_num,
        "info_used", proc->rbuf_in.info_used,
        "", "");
}

static td_void drv_ao_proc_show_aip_out_buf(td_void *file, td_u8 aip_index, td_u8 buf_index,
    const audio_ring_buf *rbuf_out, td_u32 frame_pms)
{
    td_u32 data_size;
    td_u32 data_ratio, data_ms;

    data_size = drv_audio_proc_query_rbuf_busy(rbuf_out->size, rbuf_out->read_pos, rbuf_out->write_pos);
    data_ratio = data_size * 100 / rbuf_out->size; /* 100: percent(%) */
    data_ms = drv_audio_proc_get_data_ms(data_size, frame_pms);

    drv_audio_proc_print_title(file, "aip[%d] out buf[%d]", aip_index, buf_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "buf_addr", rbuf_out->buf_addr,
        "buf_size", rbuf_out->size,
        "read_pos", rbuf_out->read_pos,
        "write_pos", rbuf_out->write_pos);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "tail_size", rbuf_out->tail_size,
        "data_size", data_size,
        "data_ratio", data_ratio,
        "data_ms", data_ms);

    /* line 3 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "overrun", rbuf_out->overrun_cnt,
        "info_addr", rbuf_out->info_addr,
        "info_num", rbuf_out->info_num,
        "info_used", rbuf_out->info_used);
}

static td_void drv_ao_proc_show_port_status(td_void *file, td_u8 port_index, const aop_proc_item *proc)
{
    drv_audio_proc_print_title(file, "port[%d] status", port_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "port_name", drv_ao_proc_get_port_name(proc->port),
        "state", drv_audio_proc_get_state((audio_state)proc->state),
        "bypass", drv_audio_proc_get_bool(proc->bypass),
        "port_enable", drv_audio_proc_get_bool(proc->port_enable));

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
        "mute", drv_audio_proc_get_bool(proc->mute),
        "volume_integer", proc->gain.integer,
        "volume_decimal", proc->gain.decimal,
        "track_mode", drv_ao_proc_get_ao_track_mode((ao_track_mode)proc->track_mode));
}

static td_void drv_ao_proc_show_port_aef_status(td_void *file, td_u8 port_index, const aop_proc_item *proc)
{
    drv_audio_proc_print_title(file, "port[%d] aef status", port_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10d |" "\n",
        "effect_type", drv_ao_proc_aef_type_name(proc->effect_type),
        "effect_profile", drv_ao_proc_aef_profile_name(proc->aef_profile),
        "effect_bypass", drv_audio_proc_get_bool(proc->aef_bypass),
        "effect_fail", proc->effect_fail);
}

static td_void drv_ao_proc_show_port_said_status(td_void *file, td_u8 port_index, const aop_proc_item *proc)
{
    drv_audio_proc_print_title(file, "port[%d] said status", port_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10d |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "said_enable", drv_audio_proc_get_bool(proc->said_enable),
        "said_beat", proc->said_beat,
        "", "",
        "", "");
}

static td_void drv_ao_proc_show_port_out_pcm_format(td_void *file, td_u8 port_index, const aop_proc_item *proc)
{
    drv_audio_proc_print_title(file, "port[%d] out pcm format", port_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "channels", proc->pcm_attr.channels,
        "bit_depth", proc->pcm_attr.bit_depth,
        "sample_rate", proc->pcm_attr.sample_rate,
        "pcm_samples", proc->pcm_attr.samples_per_frame);
}

td_s32 drv_proc_ao_show(td_void *file, td_handle ao, const ao_proc_item *proc)
{
    td_u8 i, j;

    drv_audio_proc_print_head(file, "sound%d", proc->snd_id);

    /* AIP(track) */
    for (i = 0; i < proc->engine_proc.aip_num; i++) {
        drv_ao_proc_show_aip_status(file, i, &(proc->aip_proc[i]));
        drv_ao_proc_show_aip_in_pcm_format(file, i, &(proc->aip_proc[i]));
        drv_ao_proc_show_aip_in_buf(file, i, &(proc->aip_proc[i]));
        drv_ao_proc_show_aip_out_pcm_format(file, i, &(proc->aip_proc[i]));
        for (j = 0; j < TRACK_RBUF_MAX_NUM; j++) {
            if (proc->aip_proc[i].rbuf_out[j].handle != 0) {
                drv_ao_proc_show_aip_out_buf(file, i, j,
                    &(proc->aip_proc[i].rbuf_out[j]), proc->aip_proc[i].out_frame_pms);
            }
        }
    }

    /* engine */
    drv_ao_proc_show_engine_basic_status(file, ao, proc);
    drv_ao_proc_show_engine_pcm_format(file, proc);

    /* AOP(port) */
    for (i = 0; i < proc->engine_proc.aop_num; i++) {
        drv_ao_proc_show_port_status(file, i, &(proc->aop_proc[i]));
        drv_ao_proc_show_port_aef_status(file, i, &(proc->aop_proc[i]));
        drv_ao_proc_show_port_said_status(file, i, &(proc->aop_proc[i]));
        drv_ao_proc_show_port_attr(file, i, &proc->attr.port_attr[i]);
        drv_ao_proc_show_port_dma_status(file, i, &proc->aop_proc[i]);
        drv_ao_proc_show_port_run_status(file, i, &proc->aop_proc[i]);
        drv_ao_proc_show_port_out_pcm_format(file, i, &(proc->aop_proc[i]));
        drv_ao_proc_show_port_buf(file, i, &proc->aop_proc[i]);
    }

    drv_audio_proc_print_tail(file);

    return AUDIO_SUCCESS;
}

#ifndef SW_UART_DEBUG
td_s32 drv_diag_log_proc_ao_show(td_handle ao, const ao_proc_item *proc)
{
    td_u8 i;
    td_u8 j;
    td_u32 data_size;

    /* AIP(track) */
    for (i = 0; i < proc->engine_proc.aip_num; i++) {
        uapi_diag_info_log4(0, "track_handle = 0x%x, volume = %u, play_pts = %u, eos = %u\n",
            proc->aip_proc[i].handle,
            proc->aip_proc[i].gain.integer,
            proc->aip_proc[i].play_pts,
            proc->aip_proc[i].eos);

        data_size = drv_audio_proc_query_rbuf_busy(proc->aip_proc[i].rbuf_in.size,
            proc->aip_proc[i].rbuf_in.read_pos, proc->aip_proc[i].rbuf_in.write_pos);
        uapi_diag_info_log4(0, "track input: buf_size = %u, buf_wptr = %u, buf_rptr = %u, data_size = %u\n",
            proc->aip_proc[i].rbuf_in.size,
            proc->aip_proc[i].rbuf_in.write_pos,
            proc->aip_proc[i].rbuf_in.read_pos,
            data_size);

        for (j = 0; j < TRACK_RBUF_MAX_NUM; j++) {
            if (proc->aip_proc[i].rbuf_out[j].handle != 0) {
                data_size = drv_audio_proc_query_rbuf_busy(proc->aip_proc[i].rbuf_out[j].size,
                    proc->aip_proc[i].rbuf_out[j].read_pos, proc->aip_proc[i].rbuf_out[j].write_pos);
                uapi_diag_info_log4(0, "track output: buf_size = %u, buf_wptr = %u, buf_rptr = %u, data_size = %u\n",
                    proc->aip_proc[i].rbuf_out[j].size,
                    proc->aip_proc[i].rbuf_out[j].write_pos,
                    proc->aip_proc[i].rbuf_out[j].read_pos,
                    data_size);
            }
        }
    }

    uapi_diag_info_log3(0, "ao_handle = 0x%x, aip_num = %u, aop_num = %u\n",
        ao,
        proc->engine_proc.aip_num,
        proc->engine_proc.aop_num);

    /* AOP(port) */
    for (i = 0; i < proc->engine_proc.aop_num; i++) {
        uapi_diag_info_log4(0, "prot_name = 0x%x, volume = %u, dma_cnt = %u, buf_empty_cnt = %u\n",
            proc->aop_proc[i].port,
            proc->aop_proc[i].gain.integer,
            proc->aop_proc[i].tx_status.dma_cnt,
            proc->aop_proc[i].tx_status.buf_empty_cnt);

        data_size = drv_audio_proc_query_rbuf_busy(proc->aop_proc[i].tx_buf.buff_size,
            proc->aop_proc[i].tx_buf.buff_rptr, proc->aop_proc[i].tx_buf.buff_wptr);
        uapi_diag_info_log4(0, "port output: buf_size = %u, buf_wptr = %u, buf_rptr = %u, data_size = %u\n",
            proc->aop_proc[i].tx_buf.buff_size,
            proc->aop_proc[i].tx_buf.buff_wptr,
            proc->aop_proc[i].tx_buf.buff_rptr,
            data_size);
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
