/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: source file for drv ai proc
 * Author: audio
 * Create: 2021-07-31
 */

#include "drv_proc_ai.h"
#include "audio_struct.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "drv_audio_proc.h"
#include "drv_audio_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef SAP_PROC_SUPPORT

typedef struct {
    audio_ai_port port_id;
    const td_char *name;
} ai_port_proc_name_item;

static const ai_port_proc_name_item g_ai_port_proc_name_list[] = {
    {AI_PORT_I2S0, "i2s0"},
    {AI_PORT_I2S1, "i2s1"},
    {AI_PORT_I2S2, "i2s2"},
    {AI_PORT_I2S3, "i2s3"},
    {AI_PORT_I2S4, "i2s4"},

    {AI_PORT_ADC0, "adc0"},
    {AI_PORT_ADC1, "adc1"},
    {AI_PORT_ADC2, "adc2"},
    {AI_PORT_ADC3, "adc3"},

    {AI_PORT_PDM0, "pdm0"},
    {AI_PORT_PDM1, "pdm1"},
    {AI_PORT_PDM2, "pdm2"},
    {AI_PORT_PDM3, "pdm3"},

    {AI_PORT_SIF0, "sif0"},

    {AI_PORT_HDMIRX0, "hdmi_rx0"},
    {AI_PORT_HDMIRX1, "hdmi_rx1"},
    {AI_PORT_HDMIRX2, "hdmi_rx2"},
    {AI_PORT_HDMIRX3, "hdmi_rx3"},

    {AI_PORT_LPADC0, "lpadc0"},
    {AI_PORT_LPADC1, "lpadc1"},
};

static const td_char *get_ai_port_name(audio_ai_port port_id)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_ai_port_proc_name_list) / sizeof(g_ai_port_proc_name_list[0])); i++) {
        if (port_id == g_ai_port_proc_name_list[i].port_id) {
            return g_ai_port_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static td_void drv_ai_proc_show_basic_status(td_void *file, td_handle ai, const ai_proc_item *proc)
{
    drv_audio_proc_print_title(file, "basic status");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10d |" "\n",
        "handle", ai,
        "ai_enable", drv_audio_proc_get_bool(proc->enable),
        "port_enable", drv_audio_proc_get_bool(proc->port_proc.enable),
        "frame_index", proc->port_proc.frame_index);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10s |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "chan_num", proc->port_proc.chan_num,
        "mute", drv_audio_proc_get_bool(proc->port_proc.mute),
        "gain_integer", proc->port_proc.gain.integer,
        "gain_decimal", proc->port_proc.gain.decimal);

    /* line 3 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "ref_enable", drv_audio_proc_get_bool(proc->port_proc.ai_attr.ref_attr.enable),
        "", "",
        "", "",
        "", "");
}

static td_void drv_ai_proc_show_mic_cali_volume(td_void *file, const ai_proc_item *proc)
{
    td_u32 i;
    td_u32 mic_num = proc->port_proc.cali_volume.mic_num;
    if (mic_num == 0 || mic_num > AI_MIC_NUM_MAX) {
        return;
    }

    drv_audio_proc_print_title(file, "mic calibration volume");

    for (i = 0; i < mic_num; i++) {
        drv_audio_osal_proc_print(file,
            "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
            "mic", i,
            "vol_integer", proc->port_proc.cali_volume.volume[i].integer,
            "vol_decimal", proc->port_proc.cali_volume.volume[i].decimal,
            "", "");
    }
}

static td_void drv_ai_proc_show_out_pcm_attr(td_void *file, const ai_port_proc_item *proc)
{
    td_u32 out_channel = proc->ai_attr.pcm_attr.channels;

    if (proc->ai_attr.ref_attr.enable == TD_TRUE) {
        out_channel++;
    }

    drv_audio_proc_print_title(file, "out pcm attr");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "channels", out_channel,
        "bit_depth", proc->ai_attr.pcm_attr.bit_depth,
        "sample_rate", proc->ai_attr.pcm_attr.sample_rate,
        "pcm_samples", proc->ai_attr.pcm_attr.samples_per_frame);
}

static td_void drv_ai_proc_show_out_buf(td_void *file, td_u8 buf_index, const audio_ring_buf *rbuf, td_u32 frame_pms)
{
    td_u32 data_size;
    td_u32 data_ratio, data_ms;

    data_size = drv_audio_proc_query_rbuf_busy(rbuf->size, rbuf->read_pos, rbuf->write_pos);
    data_ratio = data_size * 100 / rbuf->size; /* 100: percent(%) */
    data_ms = drv_audio_proc_get_data_ms(data_size, frame_pms);

    drv_audio_proc_print_title(file, "out buf[%d]", buf_index);

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

static td_void show_i2s_attr(td_void *file, const audio_i2s_attr *attr)
{
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10d |" "\n",
        "master", drv_audio_proc_get_bool(attr->master),
        "i2s_mode", drv_audio_proc_get_i2s_mode((audio_i2s_mode)attr->i2s_mode),
        "mclk", drv_audio_proc_get_i2s_mclk((audio_i2s_mclk)attr->mclk),
        "bclk", attr->bclk);

    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "%-16s: %-10d |" "\n",
        "bit_depth", attr->bit_depth,
        "channels", attr->channels,
        "pcm_rise_edge", drv_audio_proc_get_pcm_rise_edge(attr->sample_rise_edge),
        "pcm_delay_cycle", attr->pcm_delay_cycle);
}

static inline td_void drv_ai_proc_show_ai_adc_attr(td_void *file, const ai_adc_attr *adc_attr)
{
    audio_unused(file);
    audio_unused(adc_attr);
}

static inline td_void drv_ai_proc_show_ai_pdm_attr(td_void *file, const ai_pdm_attr *pdm_attr)
{
    show_i2s_attr(file, &pdm_attr->i2s_attr);
}

static inline td_void drv_ai_proc_show_ai_i2s_attr(td_void *file, const ai_i2s_attr *i2s_attr)
{
    show_i2s_attr(file, &i2s_attr->i2s_attr);
}

static td_void drv_ai_proc_show_port_attr(td_void *file, const ai_port_proc_item *proc)
{
    drv_audio_proc_print_title(file, "port attr - %s", get_ai_port_name(proc->port));

    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "channels", proc->ai_attr.pcm_attr.channels,
        "bit_depth", proc->ai_attr.pcm_attr.bit_depth,
        "sample_rate", proc->ai_attr.pcm_attr.sample_rate,
        "pcm_samples", proc->ai_attr.pcm_attr.samples_per_frame);

    if ((proc->port >= AI_PORT_I2S0) && (proc->port <= AI_PORT_I2S3)) {
        drv_ai_proc_show_ai_i2s_attr(file, &proc->ai_attr.port_attr.i2s);
    } else if ((proc->port >= AI_PORT_PDM0) && (proc->port <= AI_PORT_PDM3)) {
        drv_ai_proc_show_ai_pdm_attr(file, &proc->ai_attr.port_attr.pdm);
    } else if ((proc->port >= AI_PORT_ADC0) && (proc->port <= AI_PORT_ADC3)) {
        drv_ai_proc_show_ai_adc_attr(file, &proc->ai_attr.port_attr.adc);
    } else {
        return;
    }
}

static td_void drv_ai_proc_show_port_buf(td_void *file, const ai_port_proc_item *proc, td_u32 frame_pms)
{
    td_u32 data_size;
    td_u32 data_ratio, data_ms;

    data_size = drv_audio_proc_query_rbuf_busy(proc->port_status.buf_info.buff_size,
        proc->port_status.buf_info.buff_rptr, proc->port_status.buf_info.buff_wptr);
    data_ratio = data_size * 100 / proc->port_status.buf_info.buff_size; /* 100: percent(%) */
    data_ms = drv_audio_proc_get_data_ms(data_size, frame_pms);

    drv_audio_proc_print_title(file, "port buf - %s", get_ai_port_name(proc->port));

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "buf_addr", proc->port_status.buf_info.buff_addr,
        "buf_size", proc->port_status.buf_info.buff_size,
        "buf_wptr", proc->port_status.buf_info.buff_wptr,
        "buf_rptr", proc->port_status.buf_info.buff_rptr);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "period_buf_size", proc->port_status.buf_info.period_buf_size,
        "threshold_size", proc->port_status.buf_info.threshold_size,
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

static td_void drv_ai_proc_show_port_dma_status(td_void *file, const ai_port_proc_item *proc)
{
    drv_audio_proc_print_title(file, "port dma status - %s", get_ai_port_name(proc->port));

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "dma_cnt", proc->port_status.proc_status.dma_cnt,
        "bus_time_out", proc->port_status.proc_status.bus_time_out_cnt,
        "total_byte_read", proc->port_status.proc_status.total_byte_read,
        "buf_full_cnt", proc->port_status.proc_status.buf_full_cnt);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "buf_full_warn", proc->port_status.proc_status.buf_full_warning_cnt,
        "bus_fifo_full", proc->port_status.proc_status.bus_fifo_full_cnt,
        "inf_fifo_full", proc->port_status.proc_status.inf_fifo_full_cnt,
        "try_read_cnt", proc->port_status.proc_status.try_read_cnt);
}

static td_void drv_ai_proc_show_port_run_status(td_void *file, const ai_port_proc_item *proc)
{
    drv_audio_proc_print_title(file, "port run status - %s", get_ai_port_name(proc->port));

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "xrun_stage_cnt", proc->port_status.run_status.xrun_stage_cnt,
        "", "",
        "", "",
        "", "");

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "max_xrun_1_cnt", proc->port_status.run_status.max_xrun[0].xrun_cnt,
        "max_xrun_2_cnt", proc->port_status.run_status.max_xrun[1].xrun_cnt,
        "max_xrun_3_cnt", proc->port_status.run_status.max_xrun[2].xrun_cnt, /* 2:index */
        "max_xrun_4_cnt", proc->port_status.run_status.max_xrun[3].xrun_cnt); /* 3:index */

    /* line 3 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "max_xrun_1_pos", proc->port_status.run_status.max_xrun[0].start_pos,
        "max_xrun_2_pos", proc->port_status.run_status.max_xrun[1].start_pos,
        "max_xrun_3_pos", proc->port_status.run_status.max_xrun[2].start_pos, /* 2:index */
        "max_xrun_4_pos", proc->port_status.run_status.max_xrun[3].start_pos); /* 3:index */

    /* line 4 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "last_xrun_1_cnt", proc->port_status.run_status.last_xrun[0].xrun_cnt,
        "last_xrun_2_cnt", proc->port_status.run_status.last_xrun[1].xrun_cnt,
        "last_xrun_3_cnt", proc->port_status.run_status.last_xrun[2].xrun_cnt, /* 2:index */
        "last_xrun_4_cnt", proc->port_status.run_status.last_xrun[3].xrun_cnt); /* 3:index */

    /* line 5 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "last_xrun_1_pos", proc->port_status.run_status.last_xrun[0].start_pos,
        "last_xrun_2_pos", proc->port_status.run_status.last_xrun[1].start_pos,
        "last_xrun_3_pos", proc->port_status.run_status.last_xrun[2].start_pos, /* 2:index */
        "last_xrun_4_pos", proc->port_status.run_status.last_xrun[3].start_pos); /* 3:index */
}

td_s32 drv_proc_ai_show(td_void *file, td_handle ai, const ai_proc_item *proc)
{
    td_u8 i;

    drv_audio_proc_print_head(file, "ai%d", drv_audio_proc_get_mod_index(ai));

    /* basic */
    drv_ai_proc_show_basic_status(file, ai, proc);

    /* ai mic calibration volume */
    drv_ai_proc_show_mic_cali_volume(file, proc);

    /* port */
    drv_ai_proc_show_port_attr(file, &(proc->port_proc));
    drv_ai_proc_show_port_dma_status(file, &(proc->port_proc));
    drv_ai_proc_show_port_run_status(file, &(proc->port_proc));
    drv_ai_proc_show_port_buf(file, &(proc->port_proc), proc->frame_pms);

    /* output */
    drv_ai_proc_show_out_pcm_attr(file, &(proc->port_proc));

    for (i = 0; i < AI_RBUF_MAX_NUM; i++) {
        if (proc->rbuf[i].handle != 0) {
            drv_ai_proc_show_out_buf(file, i, &(proc->rbuf[i]), proc->frame_pms);
        }
    }

    drv_audio_proc_print_tail(file);

    return AUDIO_SUCCESS;
}

#ifndef SW_UART_DEBUG
td_s32 drv_diag_log_proc_ai_show(td_handle ai, const ai_proc_item *proc)
{
    td_u8 i;
    td_u32 data_size;

    uapi_diag_info_log4(0, "ai_handle = 0x%x, dma_cnt = %u, buf_full_cnt = %u, prot = 0x%x\n",
        ai,
        proc->port_proc.port_status.proc_status.dma_cnt,
        proc->port_proc.port_status.proc_status.buf_full_cnt,
        proc->port_proc.port);

    data_size = drv_audio_proc_query_rbuf_busy(proc->port_proc.port_status.buf_info.buff_size,
        proc->port_proc.port_status.buf_info.buff_rptr, proc->port_proc.port_status.buf_info.buff_wptr);
    uapi_diag_info_log4(0, "ai input: buf_size = %u, buf_wptr = %u, buf_rptr = %u, data_size = %u\n",
        proc->port_proc.port_status.buf_info.buff_size,
        proc->port_proc.port_status.buf_info.buff_wptr,
        proc->port_proc.port_status.buf_info.buff_rptr,
        data_size);

    for (i = 0; i < AI_RBUF_MAX_NUM; i++) {
        if (proc->rbuf[i].handle != 0) {
            data_size = drv_audio_proc_query_rbuf_busy(proc->rbuf[i].size,
                proc->rbuf[i].read_pos, proc->rbuf[i].write_pos);
            uapi_diag_info_log4(0, "ai output: buf_size = %u, buf_wptr = %u, buf_rptr = %u, data_size = %u\n",
                proc->rbuf[i].size,
                proc->rbuf[i].write_pos,
                proc->rbuf[i].read_pos,
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
