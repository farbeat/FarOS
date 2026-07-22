/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: unf anc
 * Author: Audio
 */

#include "soc_uapi_anc.h"
#include "mpi_anc.h"
#include "audio_type.h"
#include "anc_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 uapi_anc_init(td_void)
{
    return mpi_anc_init();
}

td_s32 uapi_anc_deinit(td_void)
{
    return mpi_anc_deinit();
}

td_s32 uapi_anc_set_config(uapi_aha_mode mode, const td_s8 *config, td_u32 config_len)
{
    return mpi_anc_set_config((aha_mode)(td_u32)mode, config, config_len);
}

td_s32 uapi_anc_get_config(uapi_aha_mode *mode, td_s8 *config, td_u32 *config_len)
{
    td_s32 ret;
    aha_mode tmp_mode = AHA_MODE_MAX;

    if (mode == TD_NULL) {
        return ERR_ANC_NULL_PTR;
    }

    ret = mpi_anc_get_config(&tmp_mode, config, config_len);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    *mode = (td_u32)tmp_mode;
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
