/*
 * Copyright (c) CompanyNameMagicTag 2019-2021. All rights reserved.
 * Description: drv intf
 * Author: audio
 */

#include "securec.h"
#include "mem_addr.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "audio_msg.h"
#include "core_msg.h"
#include "drv_audio_osal.h"
#include "drv_audio_dev.h"
#include "drv_sap_ioctl.h"
#include "drv_sap_msg.h"
#include "drv_sap_proc.h"
#include "drv_sys.h"
#include "drv_adp.h"
#include "drv_ai.h"
#include "drv_ao.h"
#include "drv_track.h"
#include "drv_adec.h"
#include "drv_aenc.h"
#include "drv_vad.h"
#include "drv_sea.h"
#include "drv_anc.h"
#include "drv_haid.h"
#include "drv_ab.h"
#include "drv_dpm.h"

#include "time_consume.h"

typedef struct {
    td_s32 (*register_device)(td_void);
    td_void (*unregister_device)(td_void);
} sap_device_ops;

static td_s32 sap_drv_open(td_void *private_data)
{
    audio_func_enter();
    audio_func_exit();
    audio_unused(private_data);
    return AUDIO_SUCCESS;
}

static td_s32 sap_drv_release(td_void *private_data)
{
    audio_func_enter();
    audio_func_exit();
    audio_unused(private_data);
    return AUDIO_SUCCESS;
}

static td_s32 sap_drv_suspend(td_void *private_data)
{
    audio_func_enter();
    audio_func_exit();
    audio_unused(private_data);
    return AUDIO_SUCCESS;
}

static td_s32 sap_drv_resume(td_void *private_data)
{
    audio_func_enter();
    audio_func_exit();
    audio_unused(private_data);
    return AUDIO_SUCCESS;
}

/* dev register for sys start */
static drv_audio_osal_ioctl_cmd g_sap_sys_cmd_list[CMD_SYS_ID_MAX] = {
    [CMD_SYS_INIT] = {CMD_IOCTL_SYS_INIT, sap_ioctl_sys_init},
    [CMD_SYS_DEINIT] = {CMD_IOCTL_SYS_DEINIT, sap_ioctl_sys_deinit},
    [CMD_SYS_SET_DEBUG_CFG] = {CMD_IOCTL_SYS_SET_DEBUG_CFG, sap_ioctl_sys_set_debug_cfg},
    [CMD_SYS_GET_DEBUG_CFG] = {CMD_IOCTL_SYS_GET_DEBUG_CFG, sap_ioctl_sys_get_debug_cfg},
    [CMD_SYS_SET_PRODUCT_FORM] = {CMD_IOCTL_SYS_SET_PRODUCT_FORM, sap_ioctl_sys_set_product_form},
    [CMD_SYS_SET_TWS_MONO_MODE] = {CMD_IOCTL_SYS_SET_TWS_MONO_MODE, sap_ioctl_sys_set_tws_mono_mode},
    [CMD_SYS_SET_TWS_MODE] = {CMD_IOCTL_SYS_SET_TWS_MODE, sap_ioctl_sys_set_tws_mode},
    [CMD_SYS_SET_TWS_ROLE] = {CMD_IOCTL_SYS_SET_TWS_ROLE, sap_ioctl_sys_set_tws_role},
    [CMD_SYS_SET_DSP_LOG_LEVEL] = {CMD_IOCTL_SYS_SET_DSP_LOG_LEVEL, sap_ioctl_sys_set_dsp_log_level},
};

static drv_audio_osal_fileops g_sap_sys_fops = {
    .open = sap_drv_open,
    .read = NULL,
    .write = NULL,
    .llseek = NULL,
    .release = sap_drv_release,
    .cmd_list = g_sap_sys_cmd_list,
    .poll = NULL,
    .mmap = NULL,
    .cmd_cnt = sizeof(g_sap_sys_cmd_list) / sizeof(g_sap_sys_cmd_list[0]),
};

static drv_audio_osal_pmops g_sap_sys_drvops = {
    .pm_suspend = sap_drv_suspend,
    .pm_resume = sap_drv_resume,
    .pm_lowpower_enter = NULL,
    .pm_lowpower_exit = NULL,
    .pm_poweroff = NULL,
    .private_data = NULL,
};

static drv_audio_osal_dev g_sap_sys_dev_node;

static td_s32 sap_register_device_sys(td_void)
{
    td_s32 ret;

    ret = strncpy_s(g_sap_sys_dev_node.name, sizeof(g_sap_sys_dev_node.name), UMAP_DEVNAME_SYS, DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        return ret;
    }

    sap_trace_log_str(g_sap_sys_dev_node.name);

    g_sap_sys_dev_node.minor = UMAP_MIN_MINOR_SYS;
    g_sap_sys_dev_node.dev = TD_NULL;
    g_sap_sys_dev_node.fops = &g_sap_sys_fops;
    g_sap_sys_dev_node.pmops = &g_sap_sys_drvops;

    ret = drv_audio_osal_dev_register(&g_sap_sys_dev_node);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_dev_register, ret);
        return AUDIO_FAILURE;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device_sys(td_void)
{
    drv_audio_osal_dev_unregister(&g_sap_sys_dev_node);
}
/* dev register for sys end */

/* dev register for adp start */
static drv_audio_osal_ioctl_cmd g_sap_adp_cmd_list[CMD_ADP_ID_MAX] = {
    [CMD_ADP_INIT] = {CMD_IOCTL_ADP_INIT, sap_ioctl_adp_init},
    [CMD_ADP_DEINIT] = {CMD_IOCTL_ADP_DEINIT, sap_ioctl_adp_deinit},
    [CMD_ADP_GET_DEF_ATTR] = {CMD_IOCTL_ADP_GET_DEF_ATTR, sap_ioctl_adp_get_def_attr},
    [CMD_ADP_OPEN] = {CMD_IOCTL_ADP_OPEN, sap_ioctl_adp_open},
    [CMD_ADP_CLOSE] = {CMD_IOCTL_ADP_CLOSE, sap_ioctl_adp_close},
    [CMD_ADP_ALLOC_SHRB] = {CMD_IOCTL_ADP_ALLOC_SHRB, sap_ioctl_adp_alloc_shrb},
    [CMD_ADP_FREE_SHRB] = {CMD_IOCTL_ADP_FREE_SHRB, sap_ioctl_adp_free_shrb},
    [CMD_ADP_MAP] = {CMD_IOCTL_ADP_MAP, sap_ioctl_adp_map},
    [CMD_ADP_UNMAP] = {CMD_IOCTL_ADP_UNMAP, sap_ioctl_adp_unmap},
    [CMD_ADP_GET_ATTR] = {CMD_IOCTL_ADP_GET_ATTR, sap_ioctl_adp_get_attr},
    [CMD_ADP_SET_ATTR] = {CMD_IOCTL_ADP_SET_ATTR, sap_ioctl_adp_set_attr},
    [CMD_ADP_ATTACH_OUTPUT] = {CMD_IOCTL_ADP_ATTACH_OUTPUT, sap_ioctl_adp_attach_output},
    [CMD_ADP_DETACH_OUTPUT] = {CMD_IOCTL_ADP_DETACH_OUTPUT, sap_ioctl_adp_detach_output},
    [CMD_ADP_REG_EVENT_PROC] = {CMD_IOCTL_ADP_REG_EVENT_PROC, sap_ioctl_adp_register_event_proc},
};

static drv_audio_osal_fileops g_sap_adp_fops = {
    .open = sap_drv_open,
    .read = NULL,
    .write = NULL,
    .llseek = NULL,
    .release = sap_drv_release,
    .cmd_list = g_sap_adp_cmd_list,
    .poll = NULL,
    .mmap = NULL,
    .cmd_cnt = sizeof(g_sap_adp_cmd_list) / sizeof(g_sap_adp_cmd_list[0]),
};

static drv_audio_osal_pmops g_sap_adp_drvops = {
    .pm_suspend = sap_drv_suspend,
    .pm_resume = sap_drv_resume,
    .pm_lowpower_enter = NULL,
    .pm_lowpower_exit = NULL,
    .pm_poweroff = NULL,
    .private_data = NULL,
};

static drv_audio_osal_dev g_sap_adp_dev_node;

static td_s32 sap_register_device_adp(td_void)
{
    td_s32 ret;

    ret = strncpy_s(g_sap_adp_dev_node.name, sizeof(g_sap_adp_dev_node.name), UMAP_DEVNAME_ADP, DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        return ret;
    }

    sap_trace_log_str(g_sap_adp_dev_node.name);

    g_sap_adp_dev_node.minor = UMAP_MIN_MINOR_ADP;
    g_sap_adp_dev_node.dev = TD_NULL;
    g_sap_adp_dev_node.fops = &g_sap_adp_fops;
    g_sap_adp_dev_node.pmops = &g_sap_adp_drvops;

    ret = drv_audio_osal_dev_register(&g_sap_adp_dev_node);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_dev_register, ret);
        return AUDIO_FAILURE;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device_adp(td_void)
{
    drv_audio_osal_dev_unregister(&g_sap_adp_dev_node);
}
/* dev register for adp end */

#ifndef SOC_PLF_TYPE_PROT
/* dev register for ai start */
static drv_audio_osal_ioctl_cmd g_sap_ai_cmd_list[CMD_AI_ID_MAX] = {
    [CMD_AI_INIT] = {CMD_IOCTL_AI_INIT, sap_ioctl_ai_init},
    [CMD_AI_DEINIT] = {CMD_IOCTL_AI_DEINIT, sap_ioctl_ai_deinit},
    [CMD_AI_GET_DEF_ATTR] = {CMD_IOCTL_AI_GET_DEF_ATTR, sap_ioctl_ai_get_def_attr},
    [CMD_AI_OPEN] = {CMD_IOCTL_AI_OPEN, sap_ioctl_ai_open},
    [CMD_AI_CLOSE] = {CMD_IOCTL_AI_CLOSE, sap_ioctl_ai_close},
    [CMD_AI_REG_EVENT_PROC] = {CMD_IOCTL_AI_REG_EVENT_PROC, sap_ioctl_ai_register_event_proc},
    [CMD_AI_START] = {CMD_IOCTL_AI_START, sap_ioctl_ai_start},
    [CMD_AI_STOP] = {CMD_IOCTL_AI_STOP, sap_ioctl_ai_stop},
    [CMD_AI_GET_ATTR] = {CMD_IOCTL_AI_GET_ATTR, sap_ioctl_ai_get_attr},
    [CMD_AI_SET_ATTR] = {CMD_IOCTL_AI_SET_ATTR, sap_ioctl_ai_set_attr},
    [CMD_AI_SET_BYPASS] = {CMD_IOCTL_AI_SET_BYPASS, sap_ioctl_ai_set_bypass},
    [CMD_AI_RESET_VAD] = {CMD_IOCTL_AI_RESET_VAD, sap_ioctl_ai_reset_vad},
    [CMD_AI_GET_GAIN] = {CMD_IOCTL_AI_GET_GAIN, sap_ioctl_ai_get_gain},
    [CMD_AI_SET_GAIN] = {CMD_IOCTL_AI_SET_GAIN, sap_ioctl_ai_set_gain},
    [CMD_AI_GET_MUTE] = {CMD_IOCTL_AI_GET_MUTE, sap_ioctl_ai_get_mute},
    [CMD_AI_SET_MUTE] = {CMD_IOCTL_AI_SET_MUTE, sap_ioctl_ai_set_mute},
    [CMD_AI_SET_MIC_CALI_VOLUME] = {CMD_IOCTL_AI_SET_MIC_CALI_VOLUME, sap_ioctl_ai_set_mic_cali_volume},
    [CMD_AI_ATTACH_OUTPUT] = {CMD_IOCTL_AI_ATTACH_OUTPUT, sap_ioctl_ai_attach_output},
    [CMD_AI_DETACH_OUTPUT] = {CMD_IOCTL_AI_DETACH_OUTPUT, sap_ioctl_ai_detach_output},
};

static drv_audio_osal_fileops g_sap_ai_fops = {
    .open = sap_drv_open,
    .read = NULL,
    .write = NULL,
    .llseek = NULL,
    .release = sap_drv_release,
    .cmd_list = g_sap_ai_cmd_list,
    .poll = NULL,
    .mmap = NULL,
    .cmd_cnt = sizeof(g_sap_ai_cmd_list) / sizeof(g_sap_ai_cmd_list[0]),
};

static drv_audio_osal_pmops g_sap_ai_drvops = {
    .pm_suspend = sap_drv_suspend,
    .pm_resume = sap_drv_resume,
    .pm_lowpower_enter = NULL,
    .pm_lowpower_exit = NULL,
    .pm_poweroff = NULL,
    .private_data = NULL,
};

static drv_audio_osal_dev g_sap_ai_dev_node;

static td_s32 sap_register_device_ai(td_void)
{
    td_s32 ret;

    ret = strncpy_s(g_sap_ai_dev_node.name, sizeof(g_sap_ai_dev_node.name), UMAP_DEVNAME_AI, DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        return ret;
    }

    sap_trace_log_str(g_sap_ai_dev_node.name);

    g_sap_ai_dev_node.minor = UMAP_MIN_MINOR_AI;
    g_sap_ai_dev_node.dev = TD_NULL;
    g_sap_ai_dev_node.fops = &g_sap_ai_fops;
    g_sap_ai_dev_node.pmops = &g_sap_ai_drvops;

    ret = drv_audio_osal_dev_register(&g_sap_ai_dev_node);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_dev_register, ret);
        return AUDIO_FAILURE;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device_ai(td_void)
{
    drv_audio_osal_dev_unregister(&g_sap_ai_dev_node);
}
/* dev register for ai end */

/* dev register for vad start */
static drv_audio_osal_ioctl_cmd g_sap_vad_cmd_list[CMD_VAD_ID_MAX] = {
    [CMD_VAD_INIT] = {CMD_IOCTL_VAD_INIT, sap_ioctl_vad_init},
    [CMD_VAD_DEINIT] = {CMD_IOCTL_VAD_DEINIT, sap_ioctl_vad_deinit},
    [CMD_VAD_GET_DEF_ATTR] = {CMD_IOCTL_VAD_GET_DEF_ATTR, sap_ioctl_vad_get_def_attr},
    [CMD_VAD_OPEN] = {CMD_IOCTL_VAD_OPEN, sap_ioctl_vad_open},
    [CMD_VAD_CLOSE] = {CMD_IOCTL_VAD_CLOSE, sap_ioctl_vad_close},
    [CMD_VAD_REG_EVENT_PROC] = {CMD_IOCTL_VAD_REG_EVENT_PROC, sap_ioctl_vad_register_event_proc},
    [CMD_VAD_RESET] = {CMD_IOCTL_VAD_RESET, sap_ioctl_vad_reset},
};

static drv_audio_osal_fileops g_sap_vad_fops = {
    .open = sap_drv_open,
    .read = NULL,
    .write = NULL,
    .llseek = NULL,
    .release = sap_drv_release,
    .cmd_list = g_sap_vad_cmd_list,
    .poll = NULL,
    .mmap = NULL,
    .cmd_cnt = sizeof(g_sap_vad_cmd_list) / sizeof(g_sap_vad_cmd_list[0]),
};

static drv_audio_osal_pmops g_sap_vad_drvops = {
    .pm_suspend = sap_drv_suspend,
    .pm_resume = sap_drv_resume,
    .pm_lowpower_enter = NULL,
    .pm_lowpower_exit = NULL,
    .pm_poweroff = NULL,
    .private_data = NULL,
};

static drv_audio_osal_dev g_sap_vad_dev_node;

static td_s32 sap_register_device_vad(td_void)
{
    td_s32 ret;

    ret = strncpy_s(g_sap_vad_dev_node.name, sizeof(g_sap_vad_dev_node.name), UMAP_DEVNAME_VAD, DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        return ret;
    }

    sap_trace_log_str(g_sap_vad_dev_node.name);

    g_sap_vad_dev_node.minor = UMAP_MIN_MINOR_VAD;
    g_sap_vad_dev_node.dev = TD_NULL;
    g_sap_vad_dev_node.fops = &g_sap_vad_fops;
    g_sap_vad_dev_node.pmops = &g_sap_vad_drvops;

    ret = drv_audio_osal_dev_register(&g_sap_vad_dev_node);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_dev_register, ret);
        return AUDIO_FAILURE;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device_vad(td_void)
{
    drv_audio_osal_dev_unregister(&g_sap_vad_dev_node);
}
/* dev register for vad end */

/* dev register for sea start */
static drv_audio_osal_ioctl_cmd g_sap_sea_cmd_list[CMD_SEA_ID_MAX] = {
    [CMD_SEA_INIT] = {CMD_IOCTL_SEA_INIT, sap_ioctl_sea_init},
    [CMD_SEA_DEINIT] = {CMD_IOCTL_SEA_DEINIT, sap_ioctl_sea_deinit},
    [CMD_SEA_LOAD]   = {CMD_IOCTL_SEA_LOAD_ENG, sap_ioctl_sea_load_engine},
    [CMD_SEA_UNLOAD] = {CMD_IOCTL_SEA_UNLOAD_ENG, sap_ioctl_sea_unload_engine},
    [CMD_SEA_GET_ENG_CAPS] = {CMD_IOCTL_SEA_GET_ENG_CAPS, sap_ioctl_sea_get_eng_caps},
    [CMD_SEA_GET_DEF_ATTR] = {CMD_IOCTL_SEA_GET_DEF_ATTR, sap_ioctl_sea_get_def_attr},
    [CMD_SEA_CREATE]  = {CMD_IOCTL_SEA_CREATE, sap_ioctl_sea_create},
    [CMD_SEA_DESTROY] = {CMD_IOCTL_SEA_DESTROY, sap_ioctl_sea_destroy},
    [CMD_SEA_REG_EVENT_PROC] = {CMD_IOCTL_SEA_REG_EVENT_PROC, sap_ioctl_sea_register_event_proc},
    [CMD_SEA_START] = {CMD_IOCTL_SEA_START, sap_ioctl_sea_start},
    [CMD_SEA_STOP] = {CMD_IOCTL_SEA_STOP, sap_ioctl_sea_stop},
    [CMD_SEA_GET_ENG_ATTR] = {CMD_IOCTL_SEA_GET_ENG_ATTR, sap_ioctl_sea_get_eng_attr},
    [CMD_SEA_SET_ENG_ATTR] = {CMD_IOCTL_SEA_SET_ENG_ATTR, sap_ioctl_sea_set_eng_attr},
    [CMD_SEA_GET_PCM_ATTR] = {CMD_IOCTL_SEA_GET_PCM_ATTR, sap_ioctl_sea_get_pcm_attr},
    [CMD_SEA_SET_PCM_ATTR] = {CMD_IOCTL_SEA_SET_PCM_ATTR, sap_ioctl_sea_set_pcm_attr},
    [CMD_SEA_GET_PARAM] = {CMD_IOCTL_SEA_GET_PARAM, sap_ioctl_sea_get_param},
    [CMD_SEA_SET_PARAM] = {CMD_IOCTL_SEA_SET_PARAM, sap_ioctl_sea_set_param},
    [CMD_SEA_SET_BYPASS] = {CMD_IOCTL_SEA_SET_BYPASS, sap_ioctl_sea_set_bypass},
    [CMD_SEA_GET_ITEM_COUNT] = {CMD_IOCTL_SEA_GET_ITEM_COUNT, sap_ioctl_sea_get_item_count},
    [CMD_SEA_GET_ITEM_SETS] = {CMD_IOCTL_SEA_GET_ITEM_SETS, sap_ioctl_sea_get_item_sets},
    [CMD_SEA_ADD_ITEM] = {CMD_IOCTL_SEA_ADD_ITEM, sap_ioctl_sea_add_item},
    [CMD_SEA_REMOVE_ITEM] = {CMD_IOCTL_SEA_REMOVE_ITEM, sap_ioctl_sea_remove_item},
    [CMD_SEA_ENABLE_VID_ENROLL] = {CMD_IOCTL_SEA_ENABLE_VID_ENROLL, sap_ioctl_sea_enable_vid_enroll},
    [CMD_SEA_ATTACH_OUTPUT] = {CMD_IOCTL_SEA_ATTACH_OUTPUT, sap_ioctl_sea_attach_output},
    [CMD_SEA_DETACH_OUTPUT] = {CMD_IOCTL_SEA_DETACH_OUTPUT, sap_ioctl_sea_detach_output},
};

static drv_audio_osal_fileops g_sap_sea_fops = {
    .open = sap_drv_open,
    .read = NULL,
    .write = NULL,
    .llseek = NULL,
    .release = sap_drv_release,
    .cmd_list = g_sap_sea_cmd_list,
    .poll = NULL,
    .mmap = NULL,
    .cmd_cnt = sizeof(g_sap_sea_cmd_list) / sizeof(g_sap_sea_cmd_list[0]),
};

static drv_audio_osal_pmops g_sap_sea_drvops = {
    .pm_suspend = sap_drv_suspend,
    .pm_resume = sap_drv_resume,
    .pm_lowpower_enter = NULL,
    .pm_lowpower_exit = NULL,
    .pm_poweroff = NULL,
    .private_data = NULL,
};

static drv_audio_osal_dev g_sap_sea_dev_node;

static td_s32 sap_register_device_sea(td_void)
{
    td_s32 ret;

    ret = strncpy_s(g_sap_sea_dev_node.name, sizeof(g_sap_sea_dev_node.name), UMAP_DEVNAME_SEA, DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        return ret;
    }

    sap_trace_log_str(g_sap_sea_dev_node.name);

    g_sap_sea_dev_node.minor = UMAP_MIN_MINOR_SEA;
    g_sap_sea_dev_node.dev = TD_NULL;
    g_sap_sea_dev_node.fops = &g_sap_sea_fops;
    g_sap_sea_dev_node.pmops = &g_sap_sea_drvops;

    ret = drv_audio_osal_dev_register(&g_sap_sea_dev_node);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_dev_register, ret);
        return AUDIO_FAILURE;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device_sea(td_void)
{
    drv_audio_osal_dev_unregister(&g_sap_sea_dev_node);
}
/* dev register for sea end */

/* dev register for adec start */
static drv_audio_osal_ioctl_cmd g_sap_adec_cmd_list[CMD_ADEC_ID_MAX] = {
    [CMD_ADEC_INIT] = {CMD_IOCTL_ADEC_INIT, sap_ioctl_adec_init},
    [CMD_ADEC_DEINIT] = {CMD_IOCTL_ADEC_DEINIT, sap_ioctl_adec_deinit},
    [CMD_ADEC_LOAD] = {CMD_IOCTL_ADEC_LOAD_CODEC, sap_ioctl_adec_load_codec},
    [CMD_ADEC_UNLOAD] = {CMD_IOCTL_ADEC_UNLOAD_CODEC, sap_ioctl_adec_unload_codec},
    [CMD_ADEC_OPEN] = {CMD_IOCTL_ADEC_OPEN, sap_ioctl_adec_open},
    [CMD_ADEC_CLOSE] = {CMD_IOCTL_ADEC_CLOSE, sap_ioctl_adec_close},
    [CMD_ADEC_REG_EVENT_PROC] = {CMD_IOCTL_ADEC_REG_EVENT_PROC, sap_ioctl_adec_register_event_proc},
    [CMD_ADEC_START] = {CMD_IOCTL_ADEC_START, sap_ioctl_adec_start},
    [CMD_ADEC_STOP] = {CMD_IOCTL_ADEC_STOP, sap_ioctl_adec_stop},
    [CMD_ADEC_GET_ATTR] = {CMD_IOCTL_ADEC_GET_ATTR, sap_ioctl_adec_get_attr},
    [CMD_ADEC_SET_ATTR] = {CMD_IOCTL_ADEC_SET_ATTR, sap_ioctl_adec_set_attr},
    [CMD_ADEC_SET_BYPASS] = {CMD_IOCTL_ADEC_SET_BYPASS, sap_ioctl_adec_set_bypass},
    [CMD_ADEC_ATTACH_OUTPUT] = {CMD_IOCTL_ADEC_ATTACH_OUTPUT, sap_ioctl_adec_attach_output},
    [CMD_ADEC_DETACH_OUTPUT] = {CMD_IOCTL_ADEC_DETACH_OUTPUT, sap_ioctl_adec_detach_output},
};

static drv_audio_osal_fileops g_sap_adec_fops = {
    .open = sap_drv_open,
    .read = NULL,
    .write = NULL,
    .llseek = NULL,
    .release = sap_drv_release,
    .cmd_list = g_sap_adec_cmd_list,
    .poll = NULL,
    .mmap = NULL,
    .cmd_cnt = sizeof(g_sap_adec_cmd_list) / sizeof(g_sap_adec_cmd_list[0]),
};

static drv_audio_osal_pmops g_sap_adec_drvops = {
    .pm_suspend = sap_drv_suspend,
    .pm_resume = sap_drv_resume,
    .pm_lowpower_enter = NULL,
    .pm_lowpower_exit = NULL,
    .pm_poweroff = NULL,
    .private_data = NULL,
};

static drv_audio_osal_dev g_sap_adec_dev_node;

static td_s32 sap_register_device_adec(td_void)
{
    td_s32 ret;

    ret = strncpy_s(g_sap_adec_dev_node.name, sizeof(g_sap_adec_dev_node.name), UMAP_DEVNAME_ADEC,
        DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        return ret;
    }

    sap_trace_log_str(g_sap_adec_dev_node.name);

    g_sap_adec_dev_node.minor = UMAP_MIN_MINOR_ADEC;
    g_sap_adec_dev_node.dev = TD_NULL;
    g_sap_adec_dev_node.fops = &g_sap_adec_fops;
    g_sap_adec_dev_node.pmops = &g_sap_adec_drvops;

    ret = drv_audio_osal_dev_register(&g_sap_adec_dev_node);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_dev_register, ret);
        return AUDIO_FAILURE;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device_adec(td_void)
{
    drv_audio_osal_dev_unregister(&g_sap_adec_dev_node);
}
/* dev register for adec end */

/* dev register for aenc start */
static drv_audio_osal_ioctl_cmd g_sap_aenc_cmd_list[CMD_AENC_ID_MAX] = {
    [CMD_AENC_INIT] = {CMD_IOCTL_AENC_INIT, sap_ioctl_aenc_init},
    [CMD_AENC_DEINIT] = {CMD_IOCTL_AENC_DEINIT, sap_ioctl_aenc_deinit},
    [CMD_AENC_LOAD] = {CMD_IOCTL_AENC_LOAD_CODEC, sap_ioctl_aenc_load_codec},
    [CMD_AENC_UNLOAD] = {CMD_IOCTL_AENC_UNLOAD_CODEC, sap_ioctl_aenc_unload_codec},
    [CMD_AENC_OPEN] = {CMD_IOCTL_AENC_OPEN, sap_ioctl_aenc_open},
    [CMD_AENC_CLOSE] = {CMD_IOCTL_AENC_CLOSE, sap_ioctl_aenc_close},
    [CMD_AENC_REG_EVENT_PROC] = {CMD_IOCTL_AENC_REG_EVENT_PROC, sap_ioctl_aenc_register_event_proc},
    [CMD_AENC_START] = {CMD_IOCTL_AENC_START, sap_ioctl_aenc_start},
    [CMD_AENC_STOP] = {CMD_IOCTL_AENC_STOP, sap_ioctl_aenc_stop},
    [CMD_AENC_GET_ATTR] = {CMD_IOCTL_AENC_GET_ATTR, sap_ioctl_aenc_get_attr},
    [CMD_AENC_SET_ATTR] = {CMD_IOCTL_AENC_SET_ATTR, sap_ioctl_aenc_set_attr},
    [CMD_AENC_SET_BYPASS] = {CMD_IOCTL_AENC_SET_BYPASS, sap_ioctl_aenc_set_bypass},
    [CMD_AENC_ATTACH_OUTPUT] = {CMD_IOCTL_AENC_ATTACH_OUTPUT, sap_ioctl_aenc_attach_output},
    [CMD_AENC_DETACH_OUTPUT] = {CMD_IOCTL_AENC_DETACH_OUTPUT, sap_ioctl_aenc_detach_output},
    [CMD_AENC_SET_PARAM] = {CMD_IOCTL_AENC_SET_PARAM, sap_ioctl_aenc_set_param},
};

static drv_audio_osal_fileops g_sap_aenc_fops = {
    .open = sap_drv_open,
    .read = NULL,
    .write = NULL,
    .llseek = NULL,
    .release = sap_drv_release,
    .cmd_list = g_sap_aenc_cmd_list,
    .poll = NULL,
    .mmap = NULL,
    .cmd_cnt = sizeof(g_sap_aenc_cmd_list) / sizeof(g_sap_aenc_cmd_list[0]),
};

static drv_audio_osal_pmops g_sap_aenc_drvops = {
    .pm_suspend = sap_drv_suspend,
    .pm_resume = sap_drv_resume,
    .pm_lowpower_enter = NULL,
    .pm_lowpower_exit = NULL,
    .pm_poweroff = NULL,
    .private_data = NULL,
};

static drv_audio_osal_dev g_sap_aenc_dev_node;

static td_s32 sap_register_device_aenc(td_void)
{
    td_s32 ret;

    ret = strncpy_s(g_sap_aenc_dev_node.name, sizeof(g_sap_aenc_dev_node.name), UMAP_DEVNAME_AENC,
        DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        return ret;
    }

    sap_trace_log_str(g_sap_aenc_dev_node.name);

    g_sap_aenc_dev_node.minor = UMAP_MIN_MINOR_AENC;
    g_sap_aenc_dev_node.dev = TD_NULL;
    g_sap_aenc_dev_node.fops = &g_sap_aenc_fops;
    g_sap_aenc_dev_node.pmops = &g_sap_aenc_drvops;

    ret = drv_audio_osal_dev_register(&g_sap_aenc_dev_node);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_dev_register, ret);
        return AUDIO_FAILURE;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device_aenc(td_void)
{
    drv_audio_osal_dev_unregister(&g_sap_aenc_dev_node);
}
/* dev register for aenc end */

/* dev register for track start */
static drv_audio_osal_ioctl_cmd g_sap_track_cmd_list[CMD_TRACK_ID_MAX] = {
    [CMD_TRACK_INIT] = {CMD_IOCTL_TRACK_INIT, sap_ioctl_track_init},
    [CMD_TRACK_DEINIT] = {CMD_IOCTL_TRACK_DEINIT, sap_ioctl_track_deinit},
    [CMD_TRACK_GET_DEF_ATTR] = {CMD_IOCTL_TRACK_GET_DEF_ATTR, sap_ioctl_track_get_def_attr},
    [CMD_TRACK_OPEN] = {CMD_IOCTL_TRACK_OPEN, sap_ioctl_track_open},
    [CMD_TRACK_CLOSE] = {CMD_IOCTL_TRACK_CLOSE, sap_ioctl_track_close},
    [CMD_TRACK_REG_EVENT_PROC] = {CMD_IOCTL_TRACK_REG_EVENT_PROC, sap_ioctl_track_register_event_proc},
    [CMD_TRACK_GET_STATE] = {CMD_IOCTL_TRACK_GET_STATE, sap_ioctl_track_get_state},
    [CMD_TRACK_SET_STATE] = {CMD_IOCTL_TRACK_SET_STATE, sap_ioctl_track_set_state},
    [CMD_TRACK_GET_ATTR] = {CMD_IOCTL_TRACK_GET_ATTR, sap_ioctl_track_get_attr},
    [CMD_TRACK_SET_ATTR] = {CMD_IOCTL_TRACK_SET_ATTR, sap_ioctl_track_set_attr},
    [CMD_TRACK_GET_MUTE] = {CMD_IOCTL_TRACK_GET_MUTE, sap_ioctl_track_get_mute},
    [CMD_TRACK_SET_MUTE] = {CMD_IOCTL_TRACK_SET_MUTE, sap_ioctl_track_set_mute},
    [CMD_TRACK_GET_VOLUME] = {CMD_IOCTL_TRACK_GET_VOLUME, sap_ioctl_track_get_volume},
    [CMD_TRACK_SET_VOLUME] = {CMD_IOCTL_TRACK_SET_VOLUME, sap_ioctl_track_set_volume},
    [CMD_TRACK_SET_BYPASS] = {CMD_IOCTL_TRACK_SET_BYPASS, sap_ioctl_track_set_bypass},
    [CMD_TRACK_ATTACH_OUTPUT] = {CMD_IOCTL_TRACK_ATTACH_OUTPUT, sap_ioctl_track_attach_output},
    [CMD_TRACK_DETACH_OUTPUT] = {CMD_IOCTL_TRACK_DETACH_OUTPUT, sap_ioctl_track_detach_output},
    [CMD_TRACK_GET_PTS] = {CMD_IOCTL_TRACK_GET_PTS, sap_ioctl_track_get_pts},
};

static drv_audio_osal_fileops g_sap_track_fops = {
    .open = sap_drv_open,
    .read = NULL,
    .write = NULL,
    .llseek = NULL,
    .release = sap_drv_release,
    .cmd_list = g_sap_track_cmd_list,
    .poll = NULL,
    .mmap = NULL,
    .cmd_cnt = sizeof(g_sap_track_cmd_list) / sizeof(g_sap_track_cmd_list[0]),
};

static drv_audio_osal_pmops g_sap_track_drvops = {
    .pm_suspend = sap_drv_suspend,
    .pm_resume = sap_drv_resume,
    .pm_lowpower_enter = NULL,
    .pm_lowpower_exit = NULL,
    .pm_poweroff = NULL,
    .private_data = NULL,
};

static drv_audio_osal_dev g_sap_track_dev_node;

static td_s32 sap_register_device_track(td_void)
{
    td_s32 ret;

    ret = strncpy_s(g_sap_track_dev_node.name, sizeof(g_sap_track_dev_node.name), UMAP_DEVNAME_TRACK,
        DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        return ret;
    }

    sap_trace_log_str(g_sap_track_dev_node.name);

    g_sap_track_dev_node.minor = UMAP_MIN_MINOR_TRACK;
    g_sap_track_dev_node.dev = TD_NULL;
    g_sap_track_dev_node.fops = &g_sap_track_fops;
    g_sap_track_dev_node.pmops = &g_sap_track_drvops;

    ret = drv_audio_osal_dev_register(&g_sap_track_dev_node);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_dev_register, ret);
        return AUDIO_FAILURE;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device_track(td_void)
{
    drv_audio_osal_dev_unregister(&g_sap_track_dev_node);
}
/* dev register for track end */

/* dev register for ao start */
static drv_audio_osal_ioctl_cmd g_sap_ao_cmd_list[CMD_AO_ID_MAX] = {
    [CMD_AO_INIT] = {CMD_IOCTL_AO_INIT, sap_ioctl_ao_init},
    [CMD_AO_DEINIT] = {CMD_IOCTL_AO_DEINIT, sap_ioctl_ao_deinit},
    [CMD_AO_GET_DEF_ATTR] = {CMD_IOCTL_AO_GET_DEF_ATTR, sap_ioctl_ao_get_def_attr},
    [CMD_AO_OPEN] = {CMD_IOCTL_AO_OPEN, sap_ioctl_ao_open},
    [CMD_AO_CLOSE] = {CMD_IOCTL_AO_CLOSE, sap_ioctl_ao_close},
    [CMD_AO_REG_EVENT_PROC] = {CMD_IOCTL_AO_REG_EVENT_PROC, sap_ioctl_ao_register_event_proc},
    [CMD_AO_GET_PORT_ENABLE] = {CMD_IOCTL_AO_GET_PORT_ENABLE, sap_ioctl_ao_get_port_enable},
    [CMD_AO_SET_PORT_ENABLE] = {CMD_IOCTL_AO_SET_PORT_ENABLE, sap_ioctl_ao_set_port_enable},
    [CMD_AO_GET_MUTE] = {CMD_IOCTL_AO_GET_MUTE, sap_ioctl_ao_get_mute},
    [CMD_AO_SET_MUTE] = {CMD_IOCTL_AO_SET_MUTE, sap_ioctl_ao_set_mute},
    [CMD_AO_GET_VOLUME] = {CMD_IOCTL_AO_GET_VOLUME, sap_ioctl_ao_get_volume},
    [CMD_AO_SET_VOLUME] = {CMD_IOCTL_AO_SET_VOLUME, sap_ioctl_ao_set_volume},
    [CMD_AO_GET_TRACK_MODE] = {CMD_IOCTL_AO_GET_TRACK_MODE, sap_ioctl_ao_get_track_mode},
    [CMD_AO_SET_TRACK_MODE] = {CMD_IOCTL_AO_SET_TRACK_MODE, sap_ioctl_ao_set_track_mode},
    [CMD_AO_GET_ALL_TRACK_MUTE] = {CMD_IOCTL_AO_GET_ALL_TRACK_MUTE, sap_ioctl_ao_get_all_track_mute},
    [CMD_AO_SET_ALL_TRACK_MUTE] = {CMD_IOCTL_AO_SET_ALL_TRACK_MUTE, sap_ioctl_ao_set_all_track_mute},
    [CMD_AO_SET_BYPASS] = {CMD_IOCTL_AO_SET_BYPASS, sap_ioctl_ao_set_bypass},
    [CMD_AO_ATTACH_OUTPUT] = {CMD_IOCTL_AO_ATTACH_OUTPUT, sap_ioctl_ao_attach_output},
    [CMD_AO_DETACH_OUTPUT] = {CMD_IOCTL_AO_DETACH_OUTPUT, sap_ioctl_ao_detach_output},
    [CMD_AO_GET_AEF_PARAM] = {CMD_IOCTL_AO_GET_AEF_PARAM, sap_ioctl_ao_get_aef_param},
    [CMD_AO_SET_AEF_PARAM] = {CMD_IOCTL_AO_SET_AEF_PARAM, sap_ioctl_ao_set_aef_param},
    [CMD_AO_GET_PORT_AEF_ENABLE] = {CMD_IOCTL_AO_GET_PORT_AEF_ENABLE, sap_ioctl_ao_get_port_aef_enable},
    [CMD_AO_SET_PORT_AEF_ENABLE] = {CMD_IOCTL_AO_SET_PORT_AEF_ENABLE, sap_ioctl_ao_set_port_aef_enable},
    [CMD_AO_SET_AEF_PROFILE] = {CMD_IOCTL_AO_SET_AEF_PROFILE, sap_ioctl_ao_set_aef_profile},
    [CMD_AO_SET_SPK_CALI] = {CMD_IOCTL_AO_SET_SPK_CALI, sap_ioctl_ao_set_spk_cali},
};

static drv_audio_osal_fileops g_sap_ao_fops = {
    .open = sap_drv_open,
    .read = NULL,
    .write = NULL,
    .llseek = NULL,
    .release = sap_drv_release,
    .cmd_list = g_sap_ao_cmd_list,
    .poll = NULL,
    .mmap = NULL,
    .cmd_cnt = sizeof(g_sap_ao_cmd_list) / sizeof(g_sap_ao_cmd_list[0]),
};

static drv_audio_osal_pmops g_sap_ao_drvops = {
    .pm_suspend = sap_drv_suspend,
    .pm_resume = sap_drv_resume,
    .pm_lowpower_enter = NULL,
    .pm_lowpower_exit = NULL,
    .pm_poweroff = NULL,
    .private_data = NULL,
};

static drv_audio_osal_dev g_sap_ao_dev_node;

static td_s32 sap_register_device_ao(td_void)
{
    td_s32 ret;

    ret = strncpy_s(g_sap_ao_dev_node.name, sizeof(g_sap_ao_dev_node.name), UMAP_DEVNAME_AO, DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        return ret;
    }

    sap_trace_log_str(g_sap_ao_dev_node.name);

    g_sap_ao_dev_node.minor = UMAP_MIN_MINOR_AO;
    g_sap_ao_dev_node.dev = TD_NULL;
    g_sap_ao_dev_node.fops = &g_sap_ao_fops;
    g_sap_ao_dev_node.pmops = &g_sap_ao_drvops;

    ret = drv_audio_osal_dev_register(&g_sap_ao_dev_node);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_dev_register, ret);
        return AUDIO_FAILURE;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device_ao(td_void)
{
    drv_audio_osal_dev_unregister(&g_sap_ao_dev_node);
}
/* dev register for ao end */

#if defined(SAP_ANC_SUPPORT)
/* dev register for anc start */
static drv_audio_osal_ioctl_cmd g_sap_anc_cmd_list[CMD_ANC_ID_MAX] = {
    [CMD_ANC_SET_CONFIG] = {CMD_IOCTL_ANC_SET_CONFIG, sap_ioctl_anc_set_config},
    [CMD_ANC_GET_CONFIG] = {CMD_IOCTL_ANC_GET_CONFIG, sap_ioctl_anc_get_config},
};

static drv_audio_osal_fileops g_sap_anc_fops = {
    .open = sap_drv_open,
    .read = NULL,
    .write = NULL,
    .llseek = NULL,
    .release = sap_drv_release,
    .cmd_list = g_sap_anc_cmd_list,
    .poll = NULL,
    .mmap = NULL,
    .cmd_cnt = sizeof(g_sap_anc_cmd_list) / sizeof(g_sap_anc_cmd_list[0]),
};

static drv_audio_osal_pmops g_sap_anc_drvops = {
    .pm_suspend = sap_drv_suspend,
    .pm_resume = sap_drv_resume,
    .pm_lowpower_enter = NULL,
    .pm_lowpower_exit = NULL,
    .pm_poweroff = NULL,
    .private_data = NULL,
};

static drv_audio_osal_dev g_sap_anc_dev_node = {
    .minor = UMAP_MIN_MINOR_ANC,
    .dev = TD_NULL,
    .fops = &g_sap_anc_fops,
    .pmops = &g_sap_anc_drvops,
};

static td_s32 sap_register_device_anc(td_void)
{
    td_s32 ret;

    ret = strncpy_s(g_sap_anc_dev_node.name, sizeof(g_sap_anc_dev_node.name), UMAP_DEVNAME_ANC, DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        return ret;
    }

    ret = drv_audio_osal_dev_register(&g_sap_anc_dev_node);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_dev_register, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device_anc(td_void)
{
    drv_audio_osal_dev_unregister(&g_sap_anc_dev_node);
}
/* dev register for anc end */
#endif /* SAP_ANC_SUPPORT */

#if defined(SAP_HAID_SUPPORT)
/* dev register for haid start */
static drv_audio_osal_ioctl_cmd g_sap_haid_cmd_list[CMD_HAID_ID_MAX] = {
    [CMD_HAID_GET_DEFAULT_ATTR] = {CMD_IOCTL_HAID_GET_DEFAULT_ATTR, sap_ioctl_haid_get_default_attr},
    [CMD_HAID_CREATE] = {CMD_IOCTL_HAID_CREATE, sap_ioctl_haid_create},
    [CMD_HAID_DESTROY] = {CMD_IOCTL_HAID_DESTROY, sap_ioctl_haid_destroy},
    [CMD_HAID_SET_ENABLE] = {CMD_IOCTL_HAID_SET_ENABLE, sap_ioctl_haid_set_enable},
    [CMD_HAID_GET_ENABLE] = {CMD_IOCTL_HAID_GET_ENABLE, sap_ioctl_haid_get_enable},
    [CMD_HAID_REGISTER_EVENT_PROC] = {CMD_IOCTL_HAID_REGISTER_EVENT_PROC, sap_ioctl_haid_register_event_proc},
    [CMD_HAID_SET_VOLUME] = {CMD_IOCTL_HAID_SET_VOLUME, sap_ioctl_haid_set_volume},
    [CMD_HAID_GET_VOLUME] = {CMD_IOCTL_HAID_GET_VOLUME, sap_ioctl_haid_get_volume},
    [CMD_HAID_SET_CONFIG] = {CMD_IOCTL_HAID_SET_CONFIG, sap_ioctl_haid_set_config},
    [CMD_HAID_GET_CONFIG] = {CMD_IOCTL_HAID_GET_CONFIG, sap_ioctl_haid_get_config},
    [CMD_HAID_SET_HARDWARE_CONFIG] = {CMD_IOCTL_HAID_SET_HARDWARE_CONFIG, sap_ioctl_haid_set_hardware_config},
    [CMD_HAID_SET_ALL_CONFIG] = {CMD_IOCTL_HAID_SET_ALL_CONFIG, sap_ioctl_haid_set_all_config},
    [CMD_HAID_GET_ALL_CONFIG] = {CMD_IOCTL_HAID_GET_ALL_CONFIG, sap_ioctl_haid_get_all_config},
    [CMD_HAID_SINUS_TONE_START] = {CMD_IOCTL_HAID_SINUS_TONE_START, sap_ioctl_haid_sinus_tone_start},
    [CMD_HAID_SINUS_TONE_STOP] = {CMD_IOCTL_HAID_SINUS_TONE_STOP, sap_ioctl_haid_sinus_tone_stop},
    [CMD_HAID_SET_SCENE] = {CMD_IOCTL_HAID_SET_SCENE, sap_ioctl_haid_set_scene},
    [CMD_HAID_GET_SCENE] = {CMD_IOCTL_HAID_GET_SCENE, sap_ioctl_haid_get_scene},
    [CMD_HAID_SET_SPK_CALI] = {CMD_IOCTL_HAID_SET_SPK_CALI, sap_ioctl_haid_set_spk_cali},
    [CMD_HAID_SET_MIC_CALI_VOLUME] = {CMD_IOCTL_HAID_SET_MIC_CALI_VOLUME, sap_ioctl_haid_set_mic_cali_volume},
};

static drv_audio_osal_fileops g_sap_haid_fops = {
    .open = sap_drv_open,
    .read = NULL,
    .write = NULL,
    .llseek = NULL,
    .release = sap_drv_release,
    .cmd_list = g_sap_haid_cmd_list,
    .poll = NULL,
    .mmap = NULL,
    .cmd_cnt = sizeof(g_sap_haid_cmd_list) / sizeof(g_sap_haid_cmd_list[0]),
};

static drv_audio_osal_pmops g_sap_haid_drvops = {
    .pm_suspend = sap_drv_suspend,
    .pm_resume = sap_drv_resume,
    .pm_lowpower_enter = NULL,
    .pm_lowpower_exit = NULL,
    .pm_poweroff = NULL,
    .private_data = NULL,
};

static drv_audio_osal_dev g_sap_haid_dev_node = {
    .minor = UMAP_MIN_MINOR_HAID,
    .dev = TD_NULL,
    .fops = &g_sap_haid_fops,
    .pmops = &g_sap_haid_drvops,
};

static td_s32 sap_register_device_haid(td_void)
{
    td_s32 ret;

    ret = strncpy_s(g_sap_haid_dev_node.name, sizeof(g_sap_haid_dev_node.name),
        UMAP_DEVNAME_HAID, DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        return ret;
    }

    ret = drv_audio_osal_dev_register(&g_sap_haid_dev_node);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_dev_register, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device_haid(td_void)
{
    drv_audio_osal_dev_unregister(&g_sap_haid_dev_node);
}
/* dev register for haid end */
#endif /* SAP_HAID_SUPPORT */

#if defined(SAP_DPM_SUPPORT)
/* dev register for dpm start */
static drv_audio_osal_ioctl_cmd g_sap_dpm_cmd_list[CMD_DPM_ID_MAX] = {
    [CMD_DPM_INIT] = {CMD_IOCTL_DPM_INIT, sap_ioctl_dpm_init},
    [CMD_DPM_DEINIT] = {CMD_IOCTL_DPM_DEINIT, sap_ioctl_dpm_deinit},
    [CMD_DPM_LOAD] = {CMD_IOCTL_DPM_LOAD_LIB, sap_ioctl_dpm_load_lib},
    [CMD_DPM_UNLOAD] = {CMD_IOCTL_DPM_UNLOAD_LIB, sap_ioctl_dpm_unload_lib},
    [CMD_DPM_OPEN] = {CMD_IOCTL_DPM_OPEN, sap_ioctl_dpm_open},
    [CMD_DPM_CLOSE] = {CMD_IOCTL_DPM_CLOSE, sap_ioctl_dpm_close},
    [CMD_DPM_REG_EVENT_PROC] = {CMD_IOCTL_DPM_REG_EVENT_PROC, sap_ioctl_dpm_register_event_proc},
    [CMD_DPM_START] = {CMD_IOCTL_DPM_START, sap_ioctl_dpm_start},
    [CMD_DPM_STOP] = {CMD_IOCTL_DPM_STOP, sap_ioctl_dpm_stop},
    [CMD_DPM_GET_ATTR] = {CMD_IOCTL_DPM_GET_ATTR, sap_ioctl_dpm_get_attr},
    [CMD_DPM_SET_ATTR] = {CMD_IOCTL_DPM_SET_ATTR, sap_ioctl_dpm_set_attr},
    [CMD_DPM_SET_BYPASS] = {CMD_IOCTL_DPM_SET_BYPASS, sap_ioctl_dpm_set_bypass},
    [CMD_DPM_ATTACH_OUTPUT] = {CMD_IOCTL_DPM_ATTACH_OUTPUT, sap_ioctl_dpm_attach_output},
    [CMD_DPM_DETACH_OUTPUT] = {CMD_IOCTL_DPM_DETACH_OUTPUT, sap_ioctl_dpm_detach_output},
};

static drv_audio_osal_fileops g_sap_dpm_fops = {
    .open = sap_drv_open,
    .read = NULL,
    .write = NULL,
    .llseek = NULL,
    .release = sap_drv_release,
    .cmd_list = g_sap_dpm_cmd_list,
    .poll = NULL,
    .mmap = NULL,
    .cmd_cnt = sizeof(g_sap_dpm_cmd_list) / sizeof(g_sap_dpm_cmd_list[0]),
};

static drv_audio_osal_pmops g_sap_dpm_drvops = {
    .pm_suspend = sap_drv_suspend,
    .pm_resume = sap_drv_resume,
    .pm_lowpower_enter = NULL,
    .pm_lowpower_exit = NULL,
    .pm_poweroff = NULL,
    .private_data = NULL,
};

static drv_audio_osal_dev g_sap_dpm_dev_node;

static td_s32 sap_register_device_dpm(td_void)
{
    td_s32 ret;

    ret = strncpy_s(g_sap_dpm_dev_node.name, sizeof(g_sap_dpm_dev_node.name), UMAP_DEVNAME_DPM,
        DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        return ret;
    }

    sap_trace_log_str(g_sap_dpm_dev_node.name);

    g_sap_dpm_dev_node.minor = UMAP_MIN_MINOR_DPM;
    g_sap_dpm_dev_node.dev = TD_NULL;
    g_sap_dpm_dev_node.fops = &g_sap_dpm_fops;
    g_sap_dpm_dev_node.pmops = &g_sap_dpm_drvops;

    ret = drv_audio_osal_dev_register(&g_sap_dpm_dev_node);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_dev_register, ret);
        return AUDIO_FAILURE;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device_dpm(td_void)
{
    drv_audio_osal_dev_unregister(&g_sap_dpm_dev_node);
}
/* dev register for dpm end */
#endif /* SAP_DPM_SUPPORT */

static const sap_device_ops sap_device_ops_acore[] = {
    {sap_register_device_vad, sap_unregister_device_vad},
    {sap_register_device_ai, sap_unregister_device_ai},
    {sap_register_device_sea, sap_unregister_device_sea},
    {sap_register_device_adec, sap_unregister_device_adec},
    {sap_register_device_aenc, sap_unregister_device_aenc},
    {sap_register_device_track, sap_unregister_device_track},
    {sap_register_device_ao, sap_unregister_device_ao},

#if defined(SAP_ANC_SUPPORT)
    {sap_register_device_anc, sap_unregister_device_anc},
#endif /* SAP_ANC_SUPPORT */

#if defined(SAP_HAID_SUPPORT)
    {sap_register_device_haid, sap_unregister_device_haid},
#endif /* SAP_HAID_SUPPORT */

#if defined(SAP_DPM_SUPPORT)
    {sap_register_device_dpm, sap_unregister_device_dpm},
#endif /* SAP_DPM_SUPPORT */
};
#endif /* SOC_PLF_TYPE_PROT */

static const sap_device_ops sap_device_ops_common[] = {
    {sap_register_device_sys, sap_unregister_device_sys},
    {sap_register_device_adp, sap_unregister_device_adp},
};

/* dev register start */
static td_s32 sap_register_device(td_void)
{
    td_u32 i;
    td_s32 ret;
    td_size_t count;

    audio_func_enter();

    count = sizeof(sap_device_ops_common) / sizeof(sap_device_ops_common[0]);
    for (i = 0; i < count; i++) {
        ret = sap_device_ops_common[i].register_device();
        if (ret != AUDIO_SUCCESS) {
            sap_fatal_log_u32(i);
            sap_fatal_log_ret(ret);
        }
    }

#ifndef SOC_PLF_TYPE_PROT
    count = sizeof(sap_device_ops_acore) / sizeof(sap_device_ops_acore[0]);
    for (i = 0; i < count; i++) {
        ret = sap_device_ops_acore[i].register_device();
        if (ret != AUDIO_SUCCESS) {
            sap_fatal_log_u32(i);
            sap_fatal_log_ret(ret);
        }
    }
#endif /* SOC_PLF_TYPE_PROT */

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_void sap_unregister_device(td_void)
{
    td_u32 i;
    td_size_t count;

    audio_func_enter();

#ifndef SOC_PLF_TYPE_PROT
    count = sizeof(sap_device_ops_acore) / sizeof(sap_device_ops_acore[0]);
    for (i = 0; i < count; i++) {
        sap_device_ops_acore[i].unregister_device();
    }
#endif /* SOC_PLF_TYPE_PROT */

    count = sizeof(sap_device_ops_common) / sizeof(sap_device_ops_common[0]);
    for (i = 0; i < count; i++) {
        sap_device_ops_common[i].unregister_device();
    }

    audio_func_exit();
}
/* dev register end */

#ifdef SAP_PROC_SUPPORT
typedef td_s32 (*read_proc_function)(td_void *file, td_void *private);

typedef struct {
    const td_char *name;
    read_proc_function func;
} audio_reg_proc_item;

static const audio_reg_proc_item g_audio_reg_proc_list[] = {
    {UMAP_DEVNAME_SYS, sys_read_proc},
    {UMAP_DEVNAME_ADEC, adec_read_proc},
    {UMAP_DEVNAME_AO, ao_read_proc},
    {UMAP_DEVNAME_AI, ai_read_proc},
    {UMAP_DEVNAME_AENC, aenc_read_proc},
    {UMAP_DEVNAME_SEA, sea_read_proc},
    {UMAP_DEVNAME_AB, ab_read_proc},
    {UMAP_DEVNAME_ADP, adp_read_proc},
#if defined(SAP_HAID_SUPPORT)
    {UMAP_DEVNAME_HAID, haid_read_proc},
#endif
#if defined(SAP_DPM_SUPPORT)
    {UMAP_DEVNAME_DPM, dpm_read_proc},
#endif
};

static td_s32 sap_register_module_proc(const td_char *name, read_proc_function function)
{
    td_s32 ret;
    td_char buf[DRV_AUDIO_OSAL_PROC_NAME_LENGTH] = {0};
    drv_audio_osal_proc_entry *item = TD_NULL;

    /* create proc */
    ret = strncpy_s(buf, sizeof(buf), name, DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        sap_err_log_fun(strncpy_s, ret);
        return ret;
    }

    item = drv_audio_osal_proc_add(buf, strlen(buf));
    if (item == TD_NULL) {
        sap_err_log_fun(drv_audio_osal_proc_add, AUDIO_FAILURE);
        sap_trace_log_str(name);
        return AUDIO_FAILURE;
    }

    /* set functions */
    item->read = function;

    return AUDIO_SUCCESS;
}

static td_void sap_unregister_module_proc(const td_char *name)
{
    td_s32 ret;
    td_char buf[DRV_AUDIO_OSAL_PROC_NAME_LENGTH] = {0};

    ret = strncpy_s(buf, sizeof(buf), name, DRV_AUDIO_DEV_NAME_LEN);
    if (ret != EOK) {
        sap_err_log_fun(strncpy_s, ret);
        return;
    }

    drv_audio_osal_proc_remove(buf, strlen(buf));
}

static td_s32 sap_register_proc(td_void)
{
    td_s32 ret;
    td_u32 i;
    const audio_reg_proc_item *item = TD_NULL;

    audio_func_enter();

    for (i = 0; i < (sizeof(g_audio_reg_proc_list) / sizeof(g_audio_reg_proc_list[0])); i++) {
        item = &(g_audio_reg_proc_list[i]);
        ret = sap_register_module_proc(item->name, item->func);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(sap_register_module_proc, ret);
            sap_err_log_str(item->name);
        }
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_void sap_unregister_proc(td_void)
{
    td_u32 i;
    audio_func_enter();

    for (i = 0; i < (sizeof(g_audio_reg_proc_list) / sizeof(g_audio_reg_proc_list[0])); i++) {
        sap_unregister_module_proc(g_audio_reg_proc_list[i].name);
    }

    audio_func_exit();
}
#else
static td_s32 sap_register_proc(td_void)
{
    return AUDIO_SUCCESS;
}

static td_void sap_unregister_proc(td_void)
{
    return;
}
#endif /* SAP_PROC_SUPPORT */

#if (SAP_OS_TYPE != os_type_linux)
#define FILE_PATH_LEN_MAX 32
#define SAP_DEVNAME_SYS "/dev/" UMAP_DEVNAME_SYS
#define SAP_DEVNAME_ADP "/dev/" UMAP_DEVNAME_ADP
#define SAP_DEVNAME_AI "/dev/" UMAP_DEVNAME_AI
#define SAP_DEVNAME_VAD "/dev/" UMAP_DEVNAME_VAD
#define SAP_DEVNAME_SEA "/dev/" UMAP_DEVNAME_SEA
#define SAP_DEVNAME_ADEC "/dev/" UMAP_DEVNAME_ADEC
#define SAP_DEVNAME_AENC "/dev/" UMAP_DEVNAME_AENC
#define SAP_DEVNAME_TRACK "/dev/" UMAP_DEVNAME_TRACK
#define SAP_DEVNAME_AO "/dev/" UMAP_DEVNAME_AO
#define SAP_DEVNAME_ANC "/dev/" UMAP_DEVNAME_ANC
#define SAP_DEVNAME_HAID "/dev/" UMAP_DEVNAME_HAID
#define SAP_DEVNAME_DPM "/dev/" UMAP_DEVNAME_DPM

typedef struct {
    char file_path[FILE_PATH_LEN_MAX];
    drv_audio_osal_ioctl_cmd *cmd_list;
} ioctl_module_man;

/* This array is storing all modules with their cmd list and dev file path.
 * we use dev file path to get the module index.
 * module index is defined in enum.
 */
static ioctl_module_man module_list[MODULE_ID_MAX] = {
    [MODULE_SYS_IDX]   = {SAP_DEVNAME_SYS, g_sap_sys_cmd_list},
    [MODULE_ADP_IDX]   = {SAP_DEVNAME_ADP, g_sap_adp_cmd_list},
#ifndef SOC_PLF_TYPE_PROT
    [MODULE_AI_IDX]    = {SAP_DEVNAME_AI, g_sap_ai_cmd_list},
    [MODULE_VAD_IDX]   = {SAP_DEVNAME_VAD, g_sap_vad_cmd_list},
    [MODULE_SEA_IDX]   = {SAP_DEVNAME_SEA, g_sap_sea_cmd_list},
    [MODULE_ADEC_IDX]  = {SAP_DEVNAME_ADEC, g_sap_adec_cmd_list},
    [MODULE_AENC_IDX]  = {SAP_DEVNAME_AENC, g_sap_aenc_cmd_list},
    [MODULE_TRACK_IDX] = {SAP_DEVNAME_TRACK, g_sap_track_cmd_list},
    [MODULE_AO_IDX]    = {SAP_DEVNAME_AO, g_sap_ao_cmd_list},

#if defined(SAP_ANC_SUPPORT)
    [MODULE_ANC_IDX]  = {SAP_DEVNAME_ANC, g_sap_anc_cmd_list},
#endif /* SAP_ANC_SUPPORT */

#if defined(SAP_HAID_SUPPORT)
    [MODULE_HAID_IDX]  = {SAP_DEVNAME_HAID, g_sap_haid_cmd_list},
#endif /* SAP_HAID_SUPPORT */

#if defined(SAP_DPM_SUPPORT)
    [MODULE_DPM_IDX]  = {SAP_DEVNAME_DPM, g_sap_dpm_cmd_list},
#endif /* SAP_DPM_SUPPORT */
#endif /* SOC_PLF_TYPE_PROT */
};

/* In LiteOs, we accomplish this open interface to compat with open dev in linux.
 * This interface return the module list index of this module if this module exists in module_list.
 * This index is used to find destination function list of different module.
 */
td_s32 audio_open(const td_char *path_name, td_s32 flags, td_s32 mode)
{
    td_s32 module_idx;

    audio_func_enter();
    if (path_name == TD_NULL) {
        sap_err_log_ret(ERR_SYS_INVALID_PARAM);
        return AUDIO_FAILURE;
    }
    sap_trace_log_str(path_name);

    if (strlen(path_name) >= FILE_PATH_LEN_MAX) {
        sap_err_log_ret(ERR_SYS_INVALID_PARAM);
        return AUDIO_FAILURE;
    }

    /* We get the related module index by searching for the dev file name. */
    for (module_idx = 0; module_idx < MODULE_ID_MAX; ++module_idx) {
        if (strcmp(path_name, module_list[module_idx].file_path) == 0) {
            audio_func_exit();
            return module_idx;
        }
    }

    sap_err_log_ret(ERR_SYS_INVALID_ID);
    audio_unused(flags);
    audio_unused(mode);
    return AUDIO_FAILURE;
}

/* In mpi layer, we call ioctl with cmd id and module index which is got from open interface above
 * to call the reference function of this cmd id.
 */
td_s32 audio_ioctl(td_s32 module_idx, td_ulong ioctl_cmd, td_void *para)
{
    /*
     Get the cmd id from ioctl command encoding variable.
     the cmd id is stored in the lower 8 bits of ioctl_cmd.
     The lower 8 bits of ioctl_cmd are the NR bits.
     */
    td_s32 ret;
    td_u32 cmd_idx = ioctl_cmd & _IOC_NRMASK;

    time_consume_start();
    ret = module_list[module_idx].cmd_list[cmd_idx].handler(cmd_idx, para, TD_NULL);
    time_consume_stop("A_DRV", module_idx, (td_u32)cmd_idx);

    return ret;
}

/* We don't need to close anything because we don't create things in open. */
td_s32 audio_close(td_s32 module_idx)
{
    audio_func_enter();
    audio_func_exit();
    audio_unused(module_idx);
    return AUDIO_SUCCESS;
}

#endif

td_s32 sap_drv_mod_init(td_void)
{
    td_s32 ret;

    audio_func_enter();

    ret = sap_register_device();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(sap_register_device, ret);
        return ret;
    }

    ret = sap_register_proc();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(sap_register_proc, ret);
        sap_unregister_device();
        return ret;
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_void sap_drv_mod_exit(td_void)
{
    audio_func_enter();

    sap_unregister_proc();
    sap_unregister_device();

    audio_func_exit();
    return;
}

drv_audio_osal_module_init(sap_drv_mod_init);
drv_audio_osal_module_exit(sap_drv_mod_exit);

DRV_AUDIO_OSAL_MODULE_LICENSE("GPL");
