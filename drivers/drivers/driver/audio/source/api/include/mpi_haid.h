/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: head file for mpi haid
 * Author: audio
 */

#ifndef __MPI_HAID_H__
#define __MPI_HAID_H__

#include "haid_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_haid_init(td_void);

td_s32 mpi_haid_deinit(td_void);

td_s32 mpi_haid_get_default_attr(haid_attr *attr);

td_s32 mpi_haid_create(td_handle *haid, const haid_attr *attr);

td_s32 mpi_haid_destroy(td_handle haid);

td_s32 mpi_haid_set_enable(td_handle haid, td_bool enable);

td_s32 mpi_haid_get_enable(td_handle haid, td_bool *enable);

td_s32 mpi_haid_register_event_proc(td_handle haid, audio_event_proc proc, td_void *context);

td_s32 mpi_haid_set_volume(td_handle haid, const haid_volume *volume);

td_s32 mpi_haid_get_volume(td_handle haid, haid_volume *volume);

td_s32 mpi_haid_set_config(td_handle haid, const td_s8 *config, td_u32 config_len);

td_s32 mpi_haid_get_config(td_handle haid, td_s8 *config, td_u32 *config_len);

td_s32 mpi_haid_set_hardware_config(td_handle haid, const td_s8 *config, td_u32 config_len);

td_s32 mpi_haid_set_all_config(td_handle haid, const td_s8 *config, td_u32 config_len);

td_s32 mpi_haid_get_all_config(td_handle haid, td_s8 *config, td_u32 *config_len);

td_s32 mpi_haid_sinus_tone_start(td_handle haid, td_u32 frequence, const haid_volume *volume);

td_s32 mpi_haid_sinus_tone_stop(td_handle haid);

td_s32 mpi_haid_set_scene(td_handle haid, haid_scene scene);

td_s32 mpi_haid_get_scene(td_handle haid, haid_scene *scene);

td_s32 mpi_haid_set_spk_cali_coef(td_handle haid, const td_u8 *filter_coefs, td_u32 coef_len);

td_s32 mpi_haid_set_mic_cali_volume(td_handle haid, const haid_cali_volume *cali_volume);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_HAID_H__ */
