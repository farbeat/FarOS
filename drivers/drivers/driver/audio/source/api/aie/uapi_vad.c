/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: unf vad
 * Author: Audio
 */

#include "soc_uapi_vad.h"
#include "mpi_vad.h"
#include "audio_type.h"
#include "vad_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 uapi_vad_init(td_void)
{
    return mpi_vad_init();
}

td_s32 uapi_vad_deinit(td_void)
{
    return mpi_vad_deinit();
}

td_s32 uapi_vad_get_default_attr(uapi_vad_attr *attr)
{
    return mpi_vad_get_def_attr((audio_vad_attr *)(td_void *)attr);
}

td_s32 uapi_vad_open(td_handle *vad, const uapi_vad_attr *attr)
{
    return mpi_vad_open(vad, (const audio_vad_attr *)(td_void *)attr);
}

td_s32 uapi_vad_close(td_handle vad)
{
    return mpi_vad_close(vad);
}

td_s32 uapi_vad_register_event_proc(td_handle vad, uapi_vad_event_proc proc, td_void *context)
{
    return mpi_vad_register_event_proc(vad, (audio_event_proc)(td_uintptr_t)proc, context);
}

td_s32 uapi_vad_reset(td_handle vad)
{
    return mpi_vad_reset(vad);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
