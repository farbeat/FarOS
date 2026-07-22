/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: dpm user api
 * Author: Audio
 */

#include "soc_uapi_dpm.h"
#include "mpi_dpm.h"
#include "mpi_adp.h"
#include "audio_type.h"
#include "core_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 uapi_dpm_init(td_void)
{
    return mpi_dpm_init();
}

td_s32 uapi_dpm_deinit(td_void)
{
    return mpi_dpm_deinit();
}

td_s32 uapi_dpm_create(td_handle *dpm)
{
    return mpi_dpm_open(dpm);
}

td_s32 uapi_dpm_destroy(td_handle dpm)
{
    return mpi_dpm_close(dpm);
}

td_s32 uapi_dpm_register_event_proc(td_handle dpm, uapi_dpm_event_proc event_proc, td_void *context)
{
    return mpi_dpm_register_event_proc(dpm, (audio_event_proc)(td_uintptr_t)event_proc, context);
}

td_s32 uapi_dpm_start(td_handle dpm)
{
    return mpi_dpm_start(dpm);
}

td_s32 uapi_dpm_stop(td_handle dpm)
{
    return mpi_dpm_stop(dpm);
}

td_s32 uapi_dpm_get_attr(td_handle dpm, uapi_dpm_attr *attr)
{
    return mpi_dpm_get_attr(dpm, (dpm_attr *)(td_void *)attr);
}

td_s32 uapi_dpm_set_attr(td_handle dpm, const uapi_dpm_attr *attr)
{
    return mpi_dpm_set_attr(dpm, (const dpm_attr *)(td_void *)attr);
}

td_s32 uapi_dpm_attach_output(td_handle dpm, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (dpm == 0 || output == 0) {
        return ERR_DPM_INVALID_HANDLE;
    }

    mod_id.u32 = output;
    if (mod_id.bits.type == SAP_MOD_ADP) {
        ret = mpi_adp_alloc_output_shrb(output, dpm, 0);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    }

    ret = mpi_dpm_attach_output(dpm, output);
    if (ret != AUDIO_SUCCESS) {
        (td_void)mpi_adp_free_shrb(output);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_dpm_detach_output(td_handle dpm, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (dpm == 0 || output == 0) {
        return ERR_DPM_INVALID_HANDLE;
    }

    ret = mpi_dpm_detach_output(dpm, output);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    mod_id.u32 = output;
    if (mod_id.bits.type == SAP_MOD_ADP) {
        ret = mpi_adp_free_shrb(output);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    }

    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
