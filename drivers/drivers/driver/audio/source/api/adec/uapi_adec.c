/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: adec user api
 * Author: Audio
 */

#include "soc_uapi_adec.h"
#include "mpi_adec.h"
#include "mpi_adp.h"
#include "audio_type.h"
#include "core_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 uapi_adec_init(td_void)
{
    return mpi_adec_init();
}

td_s32 uapi_adec_deinit(td_void)
{
    return mpi_adec_deinit();
}

td_s32 uapi_adec_create(td_handle *adec)
{
    return mpi_adec_open(adec);
}

td_s32 uapi_adec_destroy(td_handle adec)
{
    return mpi_adec_close(adec);
}

td_s32 uapi_adec_register_event_proc(td_handle adec, uapi_adec_event_proc event_proc, td_void *context)
{
    return mpi_adec_register_event_proc(adec, (audio_event_proc)(td_uintptr_t)event_proc, context);
}

td_s32 uapi_adec_start(td_handle adec)
{
    return mpi_adec_start(adec);
}

td_s32 uapi_adec_stop(td_handle adec)
{
    return mpi_adec_stop(adec);
}

td_s32 uapi_adec_get_attr(td_handle adec, uapi_adec_attr *attr)
{
    return mpi_adec_get_attr(adec, (adec_attr *)(td_void *)attr);
}

td_s32 uapi_adec_set_attr(td_handle adec, const uapi_adec_attr *attr)
{
    return mpi_adec_set_attr(adec, (const adec_attr *)(td_void *)attr);
}

td_s32 uapi_adec_attach_output(td_handle adec, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (adec == 0 || output == 0) {
        return ERR_ADEC_INVALID_HANDLE;
    }

    mod_id.u32 = output;
    if (mod_id.bits.type == SAP_MOD_ADP) {
        ret = mpi_adp_alloc_output_shrb(output, adec, 0);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    }

    return mpi_adec_attach_output(adec, output);
}

td_s32 uapi_adec_detach_output(td_handle adec, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (adec == 0 || output == 0) {
        return ERR_ADEC_INVALID_HANDLE;
    }

    if (get_sap_mod_type(adec) != SAP_MOD_ADEC) {
        return ERR_ADEC_INVALID_HANDLE;
    }

    ret = mpi_adec_detach_output(adec, output);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    mod_id.u32 = (td_u32)output;
    if (mod_id.bits.type == SAP_MOD_ADP) {
        return mpi_adp_free_shrb(output);
    }

    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
