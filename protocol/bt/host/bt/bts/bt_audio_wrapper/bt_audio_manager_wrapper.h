/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: audio manager wrapper api header.
 */

#ifndef AUDIO_MANAGER_WRAPPER_H
#define AUDIO_MANAGER_WRAPPER_H

#include "audio_base_type.h"

#ifdef __cplusplus
extern "C" {
#endif

int audio_manager_init(AudioStreamType stream_type);

int audio_manager_set_device_connection_state(AudioDeviceType device_type, AudioDeviceConnectState connect_state);

int audio_stream_in_init(AudioLinkDirection link_dir, AudioCodecFormat codec);

int audio_manager_activate_interrupt(AudioStreamType stream_type);

int audio_stream_in_start(int instance_num);

int audio_stream_in_get_channel_id(int instance_num, uint32_t *shm_id);

int audio_stream_in_attach_backend(int instance_num, uint32_t backend_id);

int audio_stream_out_init(AudioLinkDirection link_dir, AudioCodecFormat codec);

int audio_stream_out_start(int instance_num);

int audio_stream_out_get_channel_id(int instance_num, uint32_t *shm_id);

int audio_stream_out_attach_frontend(int instance_num, uint32_t frontend_id);

int audio_stream_in_stop(int instance_num);

int audio_stream_out_stop(int instance_num);

int audio_stream_in_deinit(int instance_num);

int audio_stream_out_deinit(int instance_num);

int audio_manager_deactivate_interrupt(void);

int audio_manager_deinit(void);

bool audio_manager_set_volume(AudioStreamType stream_type, int32_t volume);
int32_t audio_manager_get_volume(AudioStreamType stream_type);

bool audio_manager_mute(AudioStreamType streamType);
bool audio_manager_unmute(AudioStreamType streamType);

#ifdef __cplusplus
}
#endif

#endif /* AUDIO_MANAGER_WRAPPER_H */
