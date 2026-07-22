/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: audio system user api
 * Author: Audio
 */

#include "mpi_sys.h"
#include "mpi_dump.h"
#include "audio_type.h"
#include "soc_uapi_audio_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 uapi_audio_init(td_void)
{
    return mpi_audio_init();
}

td_s32 uapi_audio_deinit(td_void)
{
    return mpi_audio_deinit();
}

td_s32 uapi_audio_set_debug_cfg(const uapi_audio_debug_cfg *cfg)
{
    td_s32 ret;
    sys_check_null_ptr_return(cfg);

    if (cfg->dump_mask >= AUDIO_DUMP_MASK_MAX || cfg->proc_mask >= AUDIO_PROC_MASK_MAX) {
        return ERR_SYS_INVALID_PARAM;
    }

#ifdef SAP_DUMP_SUPPORT
    ret = mpi_dump_set_mod_mask(cfg->dump_mask);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }
#else
    if (cfg->dump_mask != 0x0) {
        return ERR_SYS_NOT_SUPPORTED;
    }
    audio_unused(ret);
#endif

    return mpi_audio_set_debug_cfg((const audio_debug_cfg *)(td_void *)cfg);
}

td_s32 uapi_audio_get_debug_cfg(uapi_audio_debug_cfg *cfg)
{
    td_s32 ret;
    sys_check_null_ptr_return(cfg);

    ret = mpi_audio_get_debug_cfg((audio_debug_cfg *)(td_void *)cfg);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

#ifdef SAP_DUMP_SUPPORT
    mpi_dump_get_mod_mask(&(cfg->dump_mask));
#else
    cfg->dump_mask = 0x0;
#endif

    return AUDIO_SUCCESS;
}

td_s32 uapi_audio_set_product_form(uapi_audio_product_form product_form)
{
    return mpi_audio_set_product_form((audio_product_form)product_form);
}

td_s32 uapi_audio_set_tws_mono_mode(td_bool mono)
{
    return mpi_audio_set_tws_mono_mode(mono);
}

td_s32 uapi_audio_set_tws_mode(uapi_audio_tws_mode tws_mode)
{
    return mpi_audio_set_tws_mode((audio_tws_mode)tws_mode);
}

td_s32 uapi_audio_set_tws_role(uapi_audio_tws_role tws_role)
{
    return mpi_audio_set_tws_role((audio_tws_role)tws_role);
}

td_s32 uapi_audio_set_dsp_log_level(td_u32 log_level)
{
    return mpi_audio_set_dsp_log_level(log_level);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
