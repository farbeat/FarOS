/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: head file for mpi audio sys
 * Author: audio
 */

#ifndef __MPI_SYS_H__
#define __MPI_SYS_H__

#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_audio_init(td_void);

td_s32 mpi_audio_deinit(td_void);

td_s32 mpi_audio_set_debug_cfg(const audio_debug_cfg *cfg);

td_s32 mpi_audio_get_debug_cfg(audio_debug_cfg *cfg);

td_s32 mpi_audio_set_product_form(audio_product_form product_form);

td_s32 mpi_audio_set_tws_mono_mode(td_bool mono);

td_s32 mpi_audio_set_tws_mode(audio_tws_mode tws_mode);

td_s32 mpi_audio_set_tws_role(audio_tws_role tws_role);

td_s32 mpi_audio_set_dsp_log_level(td_u32 log_level);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_SYS_H__ */
