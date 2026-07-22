/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: header file for audio drv proc
 * Author: audio
 * Create: 2021-06-18
 */

#ifndef __DRV_AUDIO_PROC_H__
#define __DRV_AUDIO_PROC_H__

#include "audio_struct.h"
#include "ha_codec.h"
#include "core_module.h"

/*
 * proc info example:
 * ------------------[ADEC] index = 0---------------------
 * A:11111      |B:22222      |C:33333      |D:44444      |
 * E:55555      |F:66666      |G:77777      |H:88888      |
 *
 * <---- 30 ----><---- 30 ----><---- 30 ----><---- 30 ---->
 */
#define PROC_MAX_TITLE_FORMAT_LEN 120
#define PROC_TITLE_PREFIX_LEN     40
#define PROC_MAX_TITLE_LEN        (PROC_MAX_TITLE_FORMAT_LEN - PROC_TITLE_PREFIX_LEN)

#define check_option_valid(option, mod) ((((option) & (mod)) != 0x0) ? TD_TRUE : TD_FALSE)

td_s32 drv_audio_proc_print_head(td_void *file, const td_char *format, ...);

td_void drv_audio_proc_print_tail(td_void *file);

td_s32 drv_audio_proc_print_title(td_void *file, const td_char *format, ...);

static inline td_char *drv_audio_proc_get_bool(td_bool enable)
{
    if (enable == TD_TRUE) {
        return "yes";
    } else if (enable == TD_FALSE) {
        return "no";
    } else {
        return "null";
    }
}

static inline td_char *drv_audio_proc_get_state(audio_state state)
{
    if (state == AUDIO_STATE_IDLE) {
        return "idle";
    } else if (state == AUDIO_STATE_RUNNING) {
        return "run";
    } else if (state == AUDIO_STATE_STOPPED) {
        return "stop";
    } else {
        return "null";
    }
}

static inline td_s32 drv_audio_proc_get_mod_index(td_handle handle)
{
    return (handle != 0) ? ((sap_mod_id)handle).bits.index : -1;
}

static inline td_s32 drv_audio_proc_get_mod_sub_index(td_handle handle)
{
    return (handle != 0) ? ((sap_mod_id)handle).bits.sub_id : -1;
}

const td_char *drv_audio_proc_get_acodec_name(ha_codec_id id);

static inline td_char *drv_audio_proc_get_pcm_rise_edge(td_s32 rise_edge)
{
    if (rise_edge == TD_TRUE) {
        return "positive";
    } else if (rise_edge == TD_FALSE) {
        return "negative";
    } else {
        return "null";
    }
}

static inline td_u32 drv_audio_proc_query_rbuf_free(td_u32 size, td_u32 read_pos, td_u32 write_pos)
{
    return (write_pos >= read_pos) ? (size + read_pos - write_pos) : (read_pos - write_pos);
}

static inline td_u32 drv_audio_proc_query_rbuf_busy(td_u32 size, td_u32 read_pos, td_u32 write_pos)
{
    return (write_pos >= read_pos) ? (write_pos - read_pos) : (write_pos + size - read_pos);
}

static inline td_u32 drv_audio_proc_get_data_ms(td_u32 data_size, td_u32 frame_pms)
{
    return (frame_pms == 0) ? 0 : (data_size / frame_pms);
}

const td_char *drv_audio_proc_get_format_name(audio_format format);

td_char *drv_audio_proc_get_i2s_mode(audio_i2s_mode mode);

td_char *drv_audio_proc_get_i2s_mclk(audio_i2s_mclk mclk);

#endif /* __DRV_AUDIO_PROC_H__ */
