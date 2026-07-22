/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: anc msg driver
 * Author: audio
 */

#include "audio_debug.h"
#include "core_msg_struct.h"
#include "core_msg.h"
#include "drv_msg_anc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define drv_anc_send_msg(msg)  core_msg_send(AUDIO_CORE_DSP0, MSG_CORE_ANC_MODULE, msg, MSG_SYNC_WAIT_TIME_MS)

td_s32 drv_msg_anc_set_config(aha_mode mode, const td_s8 *config, td_u32 config_len)
{
    td_s32 ret;
    anc_cfg anc_config;

    anc_config.aha_mode = mode;
    anc_config.config_len = config_len;

    core_msg_data msg_data = {
        .cmd = MSG_CORE_ANC_SET_CONFIG,
        .handle = 0,
        .input_data = &anc_config,
        .input_data_size = sizeof(anc_config),
        .output_data_size = 0,
    };

    audio_unused(config);
    return drv_anc_send_msg(&msg_data);
}

td_s32 drv_msg_anc_get_config(aha_mode *mode, td_s8 *config, td_u32 *config_len)
{
    td_s32 ret;
    anc_cfg anc_config;

    core_msg_data msg_data = {
        .cmd = MSG_CORE_ANC_GET_CONFIG,
        .handle = 0,
        .input_data = TD_NULL,
        .input_data_size = 0,
        .output_data = &anc_config,
        .output_data_size = sizeof(anc_config),
    };

    ret = drv_anc_send_msg(&msg_data);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_anc_send_msg, ret);
        return ret;
    }

    *mode = anc_config.aha_mode;
    *config_len = anc_config.config_len;
    audio_unused(config);
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
