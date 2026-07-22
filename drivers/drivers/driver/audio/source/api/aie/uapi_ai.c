/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: unf ai
 * Author: Audio
 */

#include "soc_uapi_ai.h"
#include "mpi_ai.h"
#include "mpi_adp.h"
#include "audio_type.h"
#include "ai_struct.h"
#include "core_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 uapi_ai_init(td_void)
{
    return mpi_ai_init();
}

td_s32 uapi_ai_deinit(td_void)
{
    return mpi_ai_deinit();
}

td_s32 uapi_ai_get_default_attr(uapi_ai_port port, uapi_ai_attr *attr)
{
    return mpi_ai_get_def_attr((audio_ai_port)port, (audio_ai_attr *)(td_void *)attr);
}

td_s32 uapi_ai_open(td_handle *ai, uapi_ai_port port, const uapi_ai_attr *attr)
{
    return mpi_ai_open(ai, (audio_ai_port)port, (const audio_ai_attr *)(td_void *)attr);
}

td_s32 uapi_ai_close(td_handle ai)
{
    return mpi_ai_close(ai);
}

td_s32 uapi_ai_register_event_proc(td_handle ai, uapi_ai_event_proc proc, td_void *context)
{
    return mpi_ai_register_event_proc(ai, (audio_event_proc)(td_void *)proc, context);
}

td_s32 uapi_ai_start(td_handle ai)
{
    return mpi_ai_start(ai);
}

td_s32 uapi_ai_stop(td_handle ai)
{
    return mpi_ai_stop(ai);
}

td_s32 uapi_ai_reset_vad(td_handle ai)
{
    return mpi_ai_reset_vad(ai);
}

td_s32 uapi_ai_get_attr(td_handle ai, uapi_ai_attr *attr)
{
    return mpi_ai_get_attr(ai, (audio_ai_attr *)(td_void *)attr);
}

td_s32 uapi_ai_set_attr(td_handle ai, const uapi_ai_attr *attr)
{
    return mpi_ai_set_attr(ai, (const audio_ai_attr *)(td_void *)attr);
}

td_s32 uapi_ai_get_mute(td_handle ai, td_bool *mute)
{
    return mpi_ai_get_mute(ai, mute);
}

td_s32 uapi_ai_set_mute(td_handle ai, td_bool mute)
{
    return mpi_ai_set_mute(ai, mute);
}

td_s32 uapi_ai_get_volume(td_handle ai, uapi_ai_gain *gain)
{
    return mpi_ai_get_gain(ai, (ai_gain *)(td_void *)gain);
}

td_s32 uapi_ai_set_volume(td_handle ai, const uapi_ai_gain *gain)
{
    return mpi_ai_set_gain(ai, (const ai_gain *)(td_void *)gain);
}

td_s32 uapi_ai_set_mic_cali_volume(td_handle ai, const uapi_ai_cali_volume *cali_volume)
{
    return mpi_ai_set_mic_cali_volume(ai, (const ai_cali_volume *)(td_void *)cali_volume);
}

td_s32 uapi_ai_attach_output(td_handle ai, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (ai == 0 || output == 0) {
        return ERR_AI_INVALID_HANDLE;
    }

    mod_id.u32 = (td_u32)output;
    if (mod_id.bits.type == SAP_MOD_ADP) {
        ret = mpi_adp_alloc_output_shrb(output, ai, 0);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    }

    ret = mpi_ai_attach_output(ai, output);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_ai_detach_output(td_handle ai, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (ai == 0 || output == 0) {
        return ERR_AI_INVALID_HANDLE;
    }

    if (get_sap_mod_type(ai) != SAP_MOD_AI) {
        return ERR_AI_INVALID_HANDLE;
    }

    ret = mpi_ai_detach_output(ai, output);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    mod_id.u32 = (td_u32)output;
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
