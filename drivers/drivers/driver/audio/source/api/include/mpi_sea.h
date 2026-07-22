/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: head file for mpi sea
 * Author: audio
 */

#ifndef __MPI_SEA_H__
#define __MPI_SEA_H__

#include "sea_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_sea_init(td_void);

td_s32 mpi_sea_deinit(td_void);

td_s32 mpi_sea_load_engine(sea_lib_id lib_id, const td_char *lib_name);

td_s32 mpi_sea_unload_engine(sea_lib_id lib_id, const td_char *lib_name);

td_s32 mpi_sea_get_engine_caps(sea_lib_id lib_id, sea_eng_caps *eng_caps);

td_s32 mpi_sea_get_def_attr(const sea_eng_sel *eng_sel, sea_pcm_attr *attr);

td_s32 mpi_sea_create(td_handle *sea, const sea_eng_sel *eng_sel, const sea_pcm_attr *attr);

td_s32 mpi_sea_destroy(td_handle sea);

td_s32 mpi_sea_register_event_proc(td_handle sea, audio_event_proc proc, td_void *context);

td_s32 mpi_sea_start(td_handle sea);

td_s32 mpi_sea_stop(td_handle sea);

td_s32 mpi_sea_get_eng_attr(td_handle sea, td_u8 type, sea_eng_attr *attr);

td_s32 mpi_sea_set_eng_attr(td_handle sea, td_u8 type, const sea_eng_attr *attr);

td_s32 mpi_sea_get_pcm_attr(td_handle sea, sea_pcm_attr *attr);

td_s32 mpi_sea_set_pcm_attr(td_handle sea, const sea_pcm_attr *attr);

td_s32  mpi_sea_get_param(td_handle sea, sea_afe_type type, td_void *param, td_u32 param_size);

td_s32  mpi_sea_set_param(td_handle sea, sea_afe_type type, const td_void *param, td_u32 param_size);

td_s32 mpi_sea_set_bypass(td_handle sea, td_bool enable);

td_s32 mpi_sea_get_item_count(td_handle sea, sea_item_type type, td_u32 *count);

td_s32 mpi_sea_get_item_sets(td_handle sea, sea_item_type type, sea_item_sets *sets);

td_s32 mpi_sea_add_item(td_handle sea, sea_item_type type, const sea_info_item *item);

td_s32 mpi_sea_remove_item(td_handle sea, sea_item_type type, const sea_info_item *item);

td_s32 mpi_sea_enable_vid_enroll(td_handle sea, td_bool enable);

td_s32 mpi_sea_attach_output(td_handle sea, sea_output_type type, td_handle output);

td_s32 mpi_sea_detach_output(td_handle sea, sea_output_type type, td_handle output);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_SEA_H__ */
