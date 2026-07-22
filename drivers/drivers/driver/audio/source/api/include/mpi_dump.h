/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: head file for mpi dump
 * Author: audio
 */

#ifndef __MPI_DUMP_H__
#define __MPI_DUMP_H__

#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_dump_open(td_handle h_mod, td_u32 mod_out_type, const td_char *file_name, td_u32 file_size);

td_s32 mpi_dump_close(td_handle h_mod);

td_s32 mpi_dump_set_mod_mask(td_u8 dump_mask);

td_void mpi_dump_get_mod_mask(td_u8 *dump_mask);

td_bool mpi_dump_get_mod_flag(td_handle h_mod);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_DUMP_H__ */
