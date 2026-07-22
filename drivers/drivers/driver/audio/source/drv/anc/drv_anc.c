/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: anc driver
 * Author: audio
 */

#include "audio_type.h"
#include "audio_debug.h"
#include "drv_msg_anc.h"
#include "drv_sap_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sap_ioctl_anc_set_config(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    const sap_anc_config *config = (sap_anc_config *)arg;

    audio_unused(cmd);
    audio_unused(file);

    ret = drv_msg_anc_set_config(config->h_mod, config->config, config->config_len);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_anc_set_config, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_anc_get_config(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_anc_config *config = (sap_anc_config *)arg;

    audio_unused(cmd);
    audio_unused(file);

    ret = drv_msg_anc_get_config(&config->h_mod, config->config, &config->config_len);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_anc_get_config, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
