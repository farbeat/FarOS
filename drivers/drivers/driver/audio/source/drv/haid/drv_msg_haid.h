/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: header file for drv haid msg
 * Author: audio
 */

#ifndef __DRV_MSG_HAID_H__
#define __DRV_MSG_HAID_H__

#include "haid_struct.h"
#include "audio_msg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 drv_msg_haid_get_default_attr(haid_attr *attr);

td_s32 drv_msg_haid_create(td_handle *haid, const haid_attr *attr);

td_s32 drv_msg_haid_destroy(td_handle haid);

td_s32 drv_msg_haid_load_lib(td_handle haid);

td_s32 drv_msg_haid_set_enable(td_handle haid, td_bool enable);

td_s32 drv_msg_haid_get_enable(td_handle haid, td_bool *enable);

td_s32 drv_msg_haid_set_volume(td_handle haid, const haid_volume *volume);

td_s32 drv_msg_haid_get_volume(td_handle haid, haid_volume *volume);

td_s32 drv_msg_haid_set_config(td_handle haid, const td_s8 *config, td_u32 config_len);

td_s32 drv_msg_haid_get_config(td_handle haid, td_s8 *config, td_u32 *config_len);

td_s32 drv_msg_haid_set_hardware_config(td_handle haid, const td_s8 *config, td_u32 config_len);

td_s32 drv_msg_haid_set_all_config(td_handle haid, const td_s8 *config, td_u32 config_len);

td_s32 drv_msg_haid_get_all_config(td_handle haid, td_s8 *config, td_u32 *config_len);

td_s32 drv_msg_haid_sinus_tone_start(td_handle haid, td_u32 frequence, const haid_volume *volume);

td_s32 drv_msg_haid_sinus_tone_stop(td_handle haid);

td_s32 drv_msg_haid_set_scene(td_handle haid, haid_scene scene);

td_s32 drv_msg_haid_get_scene(td_handle haid, haid_scene *scene);

td_s32 drv_msg_haid_set_spk_cali(td_handle haid, const audio_spk_cali *coefs);

td_s32 drv_msg_haid_set_mic_cali_volume(td_handle haid, const haid_cali_volume *cali_volume);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_MSG_HAID_H__ */
