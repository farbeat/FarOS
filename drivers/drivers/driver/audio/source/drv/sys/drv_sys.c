/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv adp
 * Author: audio
 */

#include <pthread.h>
#include "securec.h"
#include "mem_addr.h"
#include "reg_addr.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "audio_struct.h"
#include "sys_struct.h"
#include "drv_audio_osal.h"
#include "drv_sap_ioctl.h"
#include "drv_sap_msg.h"
#include "drv_msg_sys.h"
#include "drv_hal.h"
#include "drv_overlay_load.h"
#include "drv_proc_sys.h"
#include "core_module.h"
#include "core_msg_struct.h"
#include "core_msg.h"
#include "core_shrb.h"
#include "sys_cmd.h"
#include "drv_ai.h"
#include "drv_sea.h"
#include "drv_aenc.h"
#include "drv_adec.h"
#include "drv_ao.h"
#include "drv_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef td_s32 (*drv_sys_fsm_exec)(audio_core_id core_id);

/* ap(application processor) core id */
static audio_core_id g_ap_core_id = AUDIO_CORE_CPU_K;

static pthread_mutex_t g_drv_sys_mutex = PTHREAD_MUTEX_INITIALIZER;

static audio_debug_cfg g_sys_debug_cfg = {0};

static td_s32 drv_msg_sys_active(audio_core_id to);

static td_void drv_sys_lock(td_void)
{
    (td_void)pthread_mutex_lock(&g_drv_sys_mutex);
}

static td_void drv_sys_unlock(td_void)
{
    (td_void)pthread_mutex_unlock(&g_drv_sys_mutex);
}

static td_s32 drv_shm_init(td_void)
{
    td_u32 i;
    volatile td_u8 *aoe_reg = TD_NULL;

    audio_func_enter();

    aoe_reg = (volatile td_u8 *)audio_osal_address_map(SOC_SYS_SHM_BASE_ADDR, SOC_SYS_SHM_BASE_SIZE);
    if (aoe_reg == TD_NULL) {
        sap_err_log_fun(audio_osal_address_map, TD_NULL);
        return ERR_SYS_MEM_ALLOC;
    }

    for (i = 0; i < SOC_SYS_SHM_BASE_SIZE; i++) {
        *(aoe_reg + i) = 0x0;
    }

    audio_osal_address_unmap(SOC_SYS_SHM_BASE_ADDR, (td_void *)aoe_reg);

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_void drv_ipc_shrb_deinit(td_void)
{
    td_s32 ret;

    ret = core_shrb_deinit();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_deinit, ret);
    }

    sys_cmd_deinit();

    sys_audio_tws_config_deinit();
}

static td_s32 drv_ipc_shrb_init(audio_core_id drv_cid)
{
    td_s32 ret;
    shrb_init_cfg shrb_cfg;

    sys_audio_tws_config_init();

    ret = sys_cmd_init();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(sys_cmd_init, ret);
        return ret;
    }

    (td_void)memset_s(&shrb_cfg, sizeof(shrb_cfg), 0, sizeof(shrb_cfg));

    ret = core_shrb_init(drv_cid, &shrb_cfg);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_init, ret);
        sys_cmd_deinit();
        return ret;
    }

    core_shrb_set_init_cfg(SOC_IPC_SHM_BASE_ADDR, &shrb_cfg);
    return AUDIO_SUCCESS;
}

static td_s32 drv_ipc_init(audio_core_id drv_cid, audio_core_id core_cid)
{
    td_s32 ret;
    audio_func_enter();

    ret = core_msg_init(drv_cid, core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_msg_init, ret);
        return ret;
    }

    ret = core_msg_init(core_cid, drv_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_msg_init, ret);
        core_msg_deinit(drv_cid, core_cid);
        return ret;
    }

    (td_void)core_msg_register_sys_active(core_cid, drv_msg_sys_active);

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_void drv_ipc_deinit(audio_core_id drv_cid, audio_core_id core_cid)
{
    td_s32 ret;
    audio_func_enter();

    ret = core_msg_deinit(core_cid, drv_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_msg_deinit, ret);
    }

    ret = core_msg_deinit(drv_cid, core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_msg_deinit, ret);
    }

    audio_func_exit();
}

#ifndef SW_UART_DEBUG
static td_void drv_sys_offline_proc(td_void)
{
    td_bool offline_flag = TD_TRUE;

    ai_read_proc(TD_NULL, &offline_flag);
    sea_read_proc(TD_NULL, &offline_flag);
    aenc_read_proc(TD_NULL, &offline_flag);
    adec_read_proc(TD_NULL, &offline_flag);
    ao_read_proc(TD_NULL, &offline_flag);
}
#endif

static td_s32 drv_sys_load_image(audio_core_id core_cid)
{
    td_s32 ret;
    audio_func_enter();

    ret = hal_dsp_boot_prepare(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(hal_dsp_boot_prepare, ret);
        hal_dsp_set_power_on(core_cid, TD_FALSE);
        return ret;
    }

    ret = drv_overlay_load_main_program(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_load_main_program, ret);
        hal_dsp_set_power_on(core_cid, TD_FALSE);
        return ret;
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 drv_sys_init(audio_core_id drv_cid, audio_core_id core_cid)
{
    td_s32 ret;
    audio_func_enter();

    ret = drv_ipc_init(drv_cid, core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_ipc_init, ret);
        return ret;
    }

    ret = hal_dsp_init(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(hal_dsp_init, ret);
        drv_ipc_deinit(drv_cid, core_cid);
        return ret;
    }

    ret = drv_sap_msg_start(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_sap_msg_start, ret);
        hal_dsp_reset(core_cid);
        hal_dsp_deinit(core_cid);
        drv_ipc_deinit(drv_cid, core_cid);
        return ret;
    }

#if (SAP_CHIP_TYPE != socmn1)
    hal_dsp_set_power_on(core_cid, TD_TRUE);

    ret = drv_sys_load_image(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_sys_load_image, ret);
        drv_sap_msg_stop(core_cid);
        hal_dsp_reset(core_cid);
        hal_dsp_deinit(core_cid);
        drv_ipc_deinit(drv_cid, core_cid);
        return ret;
    }

    drv_overlay_backup_tcm(core_cid);

    hal_dsp_set_power_on(core_cid, TD_FALSE);
#endif

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 drv_sys_deinit(audio_core_id drv_cid, audio_core_id core_cid)
{
    audio_func_enter();

    drv_sap_msg_stop(core_cid);
    hal_dsp_reset(core_cid);
    hal_dsp_deinit(core_cid);
    drv_ipc_deinit(drv_cid, core_cid);

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_void drv_sys_l2ram_remap(td_void)
{
    td_u32 src_start = SOC_L2RAM_AUDIO_REMAP_ADDR;
    td_u32 dest_start = SOC_L2RAM_AUDIO_PHYS_ADDR;
    td_u32 remap_size = SOC_L2RAM_SIZE;
    td_u32 value;

    value = (src_start >> 12) & 0x000fffff;   // 12, 0x000fffff : get high 20 bits addess, and move to low 20 bits
    audio_reg_write(R_DSP0_REMAPPING_SRC_BASE_ADDR, value);
    value = (dest_start >> 12) & 0x000fffff;  // 12, 0x000fffff : get high 20 bits addess, and move to low 20 bits
    audio_reg_write(R_DSP0_REMAPPING_DES_BASE_ADDR, value);
    value = (remap_size >> 12) & 0x1ff;       // 12, 0x1ff : get 12-20 bits size, and move to low 9 bits
    value = value > 0 ? value - 1 : 0;       // 1 : register value need minus 1
    value = 0x200 | value;                   // 0x200 : set bit9 for enable
    audio_reg_write(R_DSP_REMAPPING_EN1, value);
}

static td_s32 drv_sys_pon(audio_core_id core_cid)
{
    td_s32 ret;
    audio_func_enter();

    hal_dsp_set_power_on(core_cid, TD_TRUE);

    ret = hal_dsp_boot_prepare(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(hal_dsp_boot_prepare, ret);
        hal_dsp_set_power_on(core_cid, TD_FALSE);
        return ret;
    }

    drv_sys_l2ram_remap();

    ret = drv_overlay_load_main_program(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_load_main_program, ret);
        hal_dsp_set_power_on(core_cid, TD_FALSE);
        return ret;
    }

    sys_cmd_set_state(core_cid, AUDIO_SYS_ACTIVE);

    (td_void)hal_dsp_boot_up(core_cid);

    ret = sys_cmd_check_cmd_done(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(sys_cmd_check_cmd_done, ret);
        return AUDIO_SUCCESS;
    }

#if (SAP_CHIP_TYPE != socmn1)
    audio_log_alert("DSP%d power on", (td_s32)core_cid - (td_s32)AUDIO_CORE_DSP0);
    hal_dsp_set_clock_attr();
#endif

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 drv_sys_pd(audio_core_id core_cid)
{
    td_s32 ret;
    audio_func_enter();

    sys_cmd_set_state(core_cid, AUDIO_SYS_DEEP_STANDBY);
    ret = sys_cmd_check_cmd_done(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_warn_log_fun(sys_cmd_check_cmd_done, ret);
        return ERR_SYS_INST_BUSY;
    }

    ret = hal_dsp_reset(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(hal_dsp_reset, ret);
        return ret;
    }

    ret = drv_overlay_backup_tcm(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_backup_tcm, ret);
    }

    hal_dsp_set_power_on(core_cid, TD_FALSE);

#if (SAP_CHIP_TYPE != socmn1)
    audio_log_alert("DSP%d power down", (td_s32)core_cid - (td_s32)AUDIO_CORE_DSP0);
#endif

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 drv_sys_suspend(audio_core_id core_cid)
{
    td_s32 ret;
    audio_func_enter();

    sys_cmd_set_state(core_cid, AUDIO_SYS_ACTIVE_STANDBY);
    ret = sys_cmd_check_cmd_done(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(sys_cmd_check_cmd_done, ret);
        return ERR_SYS_INST_BUSY;
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 drv_sys_resume(audio_core_id core_cid)
{
    td_s32 ret;
    audio_func_enter();

    sys_cmd_set_state(core_cid, AUDIO_SYS_ACTIVE);
    ret = sys_cmd_check_cmd_done(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(sys_cmd_check_cmd_done, ret);
        return ERR_SYS_INST_BUSY;
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static drv_sys_fsm_exec g_drv_sys_fsm_exec[AUDIO_SYS_STATE_MAX][AUDIO_SYS_STATE_MAX] = {
    {TD_NULL, drv_sys_pon, TD_NULL},
    {drv_sys_pd, TD_NULL, drv_sys_suspend},
    {drv_sys_pd, drv_sys_resume, TD_NULL},
};

static td_s32 drv_sys_set_state(audio_core_id core_cid, audio_sys_state state)
{
    td_s32 ret;
    audio_sys_state curr_state;
    audio_sys_state next_state = state;
    drv_sys_fsm_exec fsm_exec = TD_NULL;

    drv_sys_lock();
    sys_cmd_get_state(core_cid, &curr_state);
    if (curr_state == next_state) {
        drv_sys_unlock();
        return AUDIO_SUCCESS;
    }

    audio_func_enter();
    fsm_exec = g_drv_sys_fsm_exec[curr_state][next_state];
    if (fsm_exec == TD_NULL) {
        sap_err_log_u32(curr_state);
        sap_err_log_u32(next_state);
        sap_err_log_fun(g_drv_sys_fsm_exec, ERR_SYS_NOT_SUPPORTED);
        drv_sys_unlock();
        return ERR_SYS_NOT_SUPPORTED;
    }

    ret = fsm_exec(core_cid);
    if (ret != AUDIO_SUCCESS) {
        sap_warn_log_fun(fsm_exec, ret);
        drv_sys_unlock();
        return ret;
    }

    drv_sys_unlock();
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_sys_init(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_u32 i;
    audio_core_id core_id;
    const td_u32 core_num = hal_dsp_get_core_num();

    ret = drv_shm_init();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_shm_init, ret);
        return ret;
    }

    ret = drv_ipc_shrb_init(g_ap_core_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_ipc_shrb_init, ret);
        return ret;
    }

    for (i = 0; i < core_num; i++) {
        core_id = AUDIO_CORE_DSP0 + i;
        if (core_id >= AUDIO_CORE_MAX) {
            continue;
        }

        ret = drv_sys_init(g_ap_core_id, core_id);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(drv_sys_init, ret);
            return ret;
        }
    }

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_sys_deinit(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_u32 i;
    audio_core_id core_id;
    const td_u32 core_num = hal_dsp_get_core_num();

    for (i = 0; i < core_num; i++) {
        core_id = AUDIO_CORE_DSP0 + i;
        if (core_id >= AUDIO_CORE_MAX) {
            continue;
        }

        ret = drv_sys_deinit(g_ap_core_id, core_id);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(drv_sys_deinit, ret);
            return ret;
        }
    }

    drv_ipc_shrb_deinit();

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_sys_set_debug_cfg(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    audio_debug_cfg *cfg = (audio_debug_cfg *)arg;

    ret = drv_audio_osal_proc_set_mask(cfg->proc_mask);
    if (ret != 0) {
        sap_err_log_fun(drv_audio_osal_proc_set_mask, ret);
        return ERR_SYS_NOT_SUPPORTED;
    }

    (td_void)memcpy_s(&g_sys_debug_cfg, sizeof(g_sys_debug_cfg), cfg, sizeof(*cfg));

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_sys_get_debug_cfg(td_u32 cmd, td_void *arg, td_void *file)
{
    audio_debug_cfg *cfg = (audio_debug_cfg *)arg;

    (td_void)memcpy_s(cfg, sizeof(*cfg), &g_sys_debug_cfg, sizeof(g_sys_debug_cfg));

    drv_audio_osal_proc_get_mask(&cfg->proc_mask);

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_sys_set_product_form(td_u32 cmd, td_void *arg, td_void *file)
{
    audio_product_form product_form = *(td_u8 *)arg;
    audio_unused(cmd);
    audio_unused(file);

    return sys_audio_set_product_form(product_form);
}

td_s32 sap_ioctl_sys_set_tws_mono_mode(td_u32 cmd, td_void *arg, td_void *file)
{
    td_bool mono = *(td_bool *)arg;
    audio_unused(cmd);
    audio_unused(file);

    return sys_audio_set_tws_mono_mode(mono);
}

td_s32 sap_ioctl_sys_set_tws_mode(td_u32 cmd, td_void *arg, td_void *file)
{
    audio_tws_mode tws_mode = *(td_u8 *)arg;
    audio_unused(cmd);
    audio_unused(file);

    return sys_audio_set_tws_mode(tws_mode);
}

td_s32 sap_ioctl_sys_set_tws_role(td_u32 cmd, td_void *arg, td_void *file)
{
    audio_tws_role tws_role = *(td_u8 *)arg;
    audio_unused(cmd);
    audio_unused(file);

    return sys_audio_set_tws_role(tws_role);
}

td_s32 sap_ioctl_sys_set_dsp_log_level(td_u32 cmd, td_void *arg, td_void *file)
{
    td_u32 log_level = *(td_u32 *)arg;
    audio_unused(cmd);
    audio_unused(file);

    return drv_msg_sys_set_dsp_log_level(log_level);
}

static td_s32 drv_msg_sys_active(audio_core_id to)
{
    return drv_sys_set_state(to, AUDIO_SYS_ACTIVE);
}

static td_s32 drv_msg_sys_report_event(core_msg_data *msg_data)
{
    msg_data_sys_event_info *event = (msg_data_sys_event_info *)msg_data->input_data;

    sap_trace_log_u32(event->core_id);
    sap_trace_log_u32(event->event);
    switch (event->event) {
        case SYS_EVENT_NO_SERVICE:
            drv_sys_set_state(event->core_id, AUDIO_SYS_DEEP_STANDBY);
            break;

        case SYS_EVENT_NO_STREAM:
            drv_sys_set_state(event->core_id, AUDIO_SYS_ACTIVE_STANDBY);
            break;

        case SYS_EVENT_CPS_CHANGE:
            sap_trace_log_u32(event->param);
            break;

#ifndef SW_UART_DEBUG
        case SYS_EVENT_OFFLINE_PROC:
            drv_sys_offline_proc();
            break;
#endif

        default:
            break;
    }

    return AUDIO_SUCCESS;
}

static const core_msg_module_func g_drv_msg_sys_func[MSG_DRV_SYS_CMD_MAX] = {
    [MSG_DRV_SYS_REPORT_EVENT] = {drv_msg_sys_report_event}
};

static td_s32 drv_msg_sys_callback(core_msg_data *msg_data)
{
    if (msg_data->cmd >= MSG_DRV_SYS_CMD_MAX) {
        sap_err_log_u32(msg_data->cmd);
        return ERR_SYS_INVALID_PARAM;
    }

    return g_drv_msg_sys_func[msg_data->cmd].func(msg_data);
}

td_s32 drv_msg_sys_module_init(audio_core_id core_id)
{
    td_s32 ret;

    ret = core_msg_register_module(core_id, MSG_CORE_SYS_MODULE, drv_msg_sys_callback);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_msg_register_module, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

#ifdef SAP_PROC_SUPPORT
td_s32 sys_read_proc(td_void *file, td_void *private)
{
    td_s32 ret;
    sys_proc_item *proc = TD_NULL;

    audio_unused(private);

    proc = (sys_proc_item *)audio_osal_malloc(sizeof(sys_proc_item));
    if (proc == TD_NULL) {
        return ERR_SYS_MEM_ALLOC;
    }

    (td_void)memset_s(proc, sizeof(*proc), 0, sizeof(*proc));

    ret = drv_msg_sys_get_proc_info(0, proc);
    if (ret != AUDIO_SUCCESS) {
        audio_osal_free(proc);
        sap_err_log_fun(drv_msg_sys_get_proc_info, ret);
        return ret;
    }

    (td_void)memcpy_s(&proc->debug_cfg, sizeof(proc->debug_cfg), &g_sys_debug_cfg, sizeof(g_sys_debug_cfg));
    drv_audio_osal_proc_get_mask(&proc->debug_cfg.proc_mask);

    ret = drv_proc_sys_show(file, proc);
    if (ret != AUDIO_SUCCESS) {
        audio_osal_free(proc);
        sap_err_log_fun(drv_proc_sys_show, ret);
        return ret;
    }

    audio_osal_free(proc);
    return AUDIO_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
