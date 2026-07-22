/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: probe of sys status
 * Author: audio
 */

#ifndef __PROBE_H__
#define __PROBE_H__

#include "audio_type.h"
#include "audio_struct.h"

td_s32 probe_status_init(td_void);
td_void probe_status_deinit(td_void);

td_void probe_add_heartbeat(audio_core_id core_id);
td_u32 probe_get_heartbeat(audio_core_id core_id);

td_void probe_update_run_step(audio_core_id core_id, audio_run_step run_step);

td_void probe_update_sys_state(audio_core_id core_id, audio_sys_state sys_state);

td_void probe_add_log_success(audio_core_id core_id);
td_void probe_add_log_busy(audio_core_id core_id);
td_void probe_add_log_len(audio_core_id core_id, td_u32 log_len);

td_void probe_add_msg_success(audio_core_id core_id);
td_void probe_add_msg_illegal_param(audio_core_id core_id);
td_void probe_add_msg_ack_timeout(audio_core_id core_id);
td_void probe_add_msg_underflow(audio_core_id core_id);
td_void probe_add_msg_crc_fail(audio_core_id core_id);

td_void probe_add_msg_int_fail(audio_core_id core_id);
td_void probe_add_msg_int_retrigger(audio_core_id core_id);

#endif
