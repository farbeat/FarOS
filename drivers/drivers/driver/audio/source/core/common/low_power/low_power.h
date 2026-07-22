/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: low power message between app and dsp.
 * Author: audio
 */

#ifndef __LOW_POWER_H__
#define __LOW_POWER_H__

typedef enum {
    DSP_POWER_OFF = 0,
    DSP_POWER_ON = 1
} dsp_status;

typedef enum {
    FREQ_ADJUST_START = 0,
    FREQ_ADJUST_END = 1,
    FREQ_PON_INIT = 2
} dsp_freq_adjust_cmd;

typedef struct {
    td_u32 action;
    td_u32 slave_freq;
} ipc_payload_adjust_slave_freq;

typedef struct {
    td_u32 status;
} msg_audio2app_dsp_status_rsp;

td_s32 dsp_status_msg_sent(dsp_status state);
#endif /* __LOW_POWER_H__ */

