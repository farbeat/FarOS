/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio input
 * Author: audio
 */

#ifndef __HAL_MAD_H__
#define __HAL_MAD_H__

#include "ai_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct hal_mad_inst* hal_mad_handle;

typedef enum {
    MAD_EVENT_NULL = 0,
    MAD_EVENT_DATA_READY,  /* *< Data ready interrupt */
    MAD_EVENT_AUTO_ACTIVE, /* *< ::mad_ctrl_cfg.active_time, or ::mad_ctrl_cfg.slp_time */
    MAD_EVENT_START_DONE,  /* *< ::mad_ctrl_cfg.analog_time */

    MAD_EVENT_VAD_ENABLE,  /* *< Vad valid enable */
    MAD_EVENT_VAD_DISABLE, /* *< Vad valid disable */
    MAD_EVENT_VAD_VALID,   /* *< Vad valid interrupt */
    MAD_EVENT_VAD_TIMEOUT, /* *< ::mad_ctrl_cfg.vad_time x ::mad_ctrl_cfg.num */

    MAD_EVENT_MAX
} mad_event;

typedef enum {
    MAD_BUF_MODE_MAD_CTRL = 0,
    MAD_BUF_MODE_SYS_CTRL,
    MAD_BUF_MODE_MAX
} mad_buf_mode;

typedef struct {
    td_u8 lp_mode;     /* LP mode, 2bis, Range:[0~3], default 0 */
    td_u8 aon_flag;    /* VAD always on, 1bit, 0:disable, 1:enable, default 0x1 */
    td_u8 si_frame;    /* SI frame number, 4bits, Range[1~15], default 0x4 */
    td_u8 si_alpha;    /* SI alpha factor, 4bits, Range[1~15], default 0x4 */
    td_u8 si_bypass;   /* SI bypass, 1bit, 0:no bypass, 1:is bypass, default 0 */
    td_u8 si_thre_sum; /* SI threshold, 8bits, default 0x4 */
    td_u16 si_thre;    /* Energe threshold, 16bits, Range[1000,65535], default 0x3E8 */

    td_u8 eng_init;     /* Energe initial mini value, 8bits, Range[1~255], default 0xC8 */
    td_u8 init_frame;   /* Initial frame number, 3bits, Range[1~7], default 0x4 */
    td_u8 min_chan_eng; /* Mini channel energe threshold, 8bits, Range[1~255], default 0x64 */
    td_u8 snr_min;      /* SNR min value, 5bits, Range[1~31], default 0x10 */
    td_u8 snr_thre;     /* SNR threshold, 6bits, Range[1~127], default 0x14 */
    td_u8 snr_thre_sum; /* SNR threshold and sum, 8bits, Range[1~127], default 0x55  */
    td_u8 band_thre;    /* Band number, 4bits, Range[1~15], default 0xB */
    td_u8 scale;        /* Scale, 3bits, Range[2~7], default 0x2 */
    td_u8 cnt_thre;     /* Vad delay threshold, 6bits, Range[1~31], default 0x4 */
    td_u8 ana_bpctrl;   /* MAD simulate bypass signal, 8bits, Range:[0~255], default 0xFF */
    td_u8 omit_samp;    /* Omit sample number, 8bits, Range:[0~255], default 0x10 */
    td_u8 vad_num;      /* VAD number in MAD work window, 8bits, Range[1~255], default 0x30 */
    td_u16 vad_time;    /* VAD frame length, 16bits, 16K Range[16~1600], 96K Range[96~9600], default:320 */
} mad_vad_cfg;

typedef struct {
    mad_buf_mode buf_mode;  /* ::mad_buf_mode */
    td_u32 ext_base_addr_high;
    td_u32 ext_base_addr_low;
    td_u32 ext_top_addr_high;
    td_u32 ext_top_addr_low;
} mad_sys_cfg;

typedef audio_i2s_attr mad_i2s_attr;

typedef struct {
    mad_buf_mode buf_mode;
    audio_i2s_attr i2s_attr;
} hal_mad_attr;

typedef td_void (*hal_mad_intr_proc)(hal_mad_handle handle, mad_event event, td_void *param, td_void *iproc_data);

td_s32 hal_mad_init(td_void);

td_void hal_mad_deinit(td_void);

td_s32 hal_mad_open(hal_mad_handle *handle, audio_ai_port port, const hal_mad_attr *mad_attr);

td_s32 hal_mad_close(hal_mad_handle handle);

td_void hal_mad_set_intr_proc(hal_mad_handle handle, hal_mad_intr_proc intr_proc, td_void *iproc_data);

td_s32 hal_mad_set_enable(hal_mad_handle handle, td_bool enable);

td_s32 hal_mad_reset_vad(hal_mad_handle handle);

td_s32 hal_mad_set_rx_enable(hal_mad_handle handle, td_bool enable);

td_s32 hal_mad_set_vad_enable(hal_mad_handle handle, td_bool enable);

td_s32 hal_mad_set_buf_mode(hal_mad_handle handle, mad_buf_mode buf_mode);

td_s32 hal_mad_get_sys_config(hal_mad_handle handle, mad_sys_cfg *sys_cfg);

td_s32 hal_mad_set_sys_config(hal_mad_handle handle, const mad_sys_cfg *sys_cfg);

td_s32 hal_mad_get_vad_config(hal_mad_handle handle, mad_vad_cfg *vad_cfg);

td_s32 hal_mad_set_vad_config(hal_mad_handle handle, const mad_vad_cfg *vad_cfg);

td_s32 hal_mad_get_attr(hal_mad_handle handle, hal_mad_attr *mad_attr);

td_s32 hal_mad_set_attr(hal_mad_handle handle, const hal_mad_attr *mad_attr);

td_s32 hal_mad_read(hal_mad_handle handle, td_u8 *buffer, td_u32 size);

td_s32 hal_mad_get_buf_pos(hal_mad_handle handle, td_u32 *read_pos, td_u32 *write_pos);

td_s32 hal_mad_get_port_status(hal_mad_handle handle, ai_port_stauts *port_status);

td_s32 hal_mad_rew_buf_rptr(hal_mad_handle handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __HAL_MAD_H__ */
