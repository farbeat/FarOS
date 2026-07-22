/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: aenc user api
 * Author: Audio
 */

#include "soc_uapi_aenc.h"
#include "mpi_aenc.h"
#include "mpi_adp.h"
#include "audio_type.h"
#include "core_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 uapi_aenc_init(td_void)
{
    return mpi_aenc_init();
}

td_s32 uapi_aenc_deinit(td_void)
{
    return mpi_aenc_deinit();
}

td_s32 uapi_aenc_create(td_handle *aenc)
{
    return mpi_aenc_open(aenc);
}

td_s32 uapi_aenc_destroy(td_handle aenc)
{
    return mpi_aenc_close(aenc);
}

td_s32 uapi_aenc_register_event_proc(td_handle aenc, uapi_aenc_event_proc event_proc, td_void *context)
{
    return mpi_aenc_register_event_proc(aenc, (audio_event_proc)(td_uintptr_t)event_proc, context);
}

td_s32 uapi_aenc_start(td_handle aenc)
{
    return mpi_aenc_start(aenc);
}

td_s32 uapi_aenc_stop(td_handle aenc)
{
    return mpi_aenc_stop(aenc);
}

td_s32 uapi_aenc_get_attr(td_handle aenc, uapi_aenc_attr *attr)
{
    return mpi_aenc_get_attr(aenc, (aenc_attr *)(td_void *)attr);
}

td_s32 uapi_aenc_set_attr(td_handle aenc, const uapi_aenc_attr *attr)
{
    return mpi_aenc_set_attr(aenc, (const aenc_attr *)(td_void *)attr);
}

td_s32 uapi_aenc_attach_output(td_handle aenc, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (aenc == 0 || output == 0) {
        return ERR_AENC_INVALID_HANDLE;
    }

    mod_id.u32 = output;
    if (mod_id.bits.type == SAP_MOD_ADP) {
        ret = mpi_adp_alloc_output_shrb(output, aenc, 0);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    }

    ret = mpi_aenc_attach_output(aenc, output);
    if (ret != AUDIO_SUCCESS) {
        (td_void)mpi_adp_free_shrb(output);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_aenc_detach_output(td_handle aenc, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (aenc == 0 || output == 0) {
        return ERR_AENC_INVALID_HANDLE;
    }

    if (get_sap_mod_type(aenc) != SAP_MOD_AENC) {
        return ERR_AENC_INVALID_HANDLE;
    }

    ret = mpi_aenc_detach_output(aenc, output);
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

td_s32 uapi_aenc_set_param(td_handle aenc, uapi_aenc_param_type type, const td_void *param, td_u32 param_size)
{
    return mpi_aenc_set_param(aenc, (aenc_param_type)type, param, param_size);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
