/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: head file for mpi anc
 * Author: audio
 */

#ifndef __MPI_ANC_H__
#define __MPI_ANC_H__

#include "anc_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_anc_init(td_void);
td_s32 mpi_anc_deinit(td_void);
td_s32 mpi_anc_set_config(aha_mode mode, const td_s8 *config, td_u32 config_len);
td_s32 mpi_anc_get_config(aha_mode *mode, td_s8 *config, td_u32 *config_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_ADP_H__ */
