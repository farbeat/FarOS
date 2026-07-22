/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv adp
 * Author: audio
 */

#include "audio_osal.h"
#include "audio_debug.h"
#include "audio_struct.h"
#include "core_shrb.h"
#include "core_adp.h"
#include "drv_audio_osal.h"
#include "drv_sap_ioctl.h"
#include "drv_msg_adp.h"
#include "core_module.h"
#include "core_msg_struct.h"
#include "core_msg.h"
#include "securec.h"
#include "drv_proc_adp.h"
#include "drv_adp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define DRV_ADP_ALLOC_BY_A_CORE 1

typedef struct {
    td_handle drv_adp;
    td_u64    context;
    td_u64    event_proc;
} drv_adp_inst;

static drv_adp_inst g_drv_adp_insts[MOD_ADP_MAX_NUM];

#define drv_adp_check_inst(drv_adp, adp)    \
    do {                                    \
        if ((drv_adp) != (adp)) {           \
            sap_err_log_h32(adp);           \
            sap_err_log_h32(drv_adp);       \
            return ERR_ADP_INVALID_HANDLE;  \
        }                                   \
    } while (0)

static td_s32 drv_adp_get_inst(drv_adp_inst **adp_inst, td_handle adp)
{
    td_u8 adp_ind;
    sap_mod_id mod_id;

    mod_id.u32 = (td_u32)adp;
    adp_ind = mod_id.bits.index;

    if (adp_ind >= MOD_ADP_MAX_NUM) {
        sap_err_log_h32(adp);
        return ERR_ADP_INVALID_HANDLE;
    }

    *adp_inst = &g_drv_adp_insts[adp_ind];
    return AUDIO_SUCCESS;
}

static td_s32 drv_adp_find_inst(drv_adp_inst **adp_inst, td_handle adp)
{
    drv_adp_inst *adp_inst_p = TD_NULL;
    td_s32 ret = drv_adp_get_inst(&adp_inst_p, adp);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_adp_get_inst, ret);
        return ret;
    }

    drv_adp_check_inst(adp_inst_p->drv_adp, adp);

    *adp_inst = adp_inst_p;
    return AUDIO_SUCCESS;
}

static td_s32 drv_adp_alloc_inst(drv_adp_inst **adp_inst, td_handle adp)
{
    drv_adp_inst *adp_inst_p = TD_NULL;
    td_s32 ret = drv_adp_get_inst(&adp_inst_p, adp);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_adp_get_inst, ret);
        return ret;
    }

    if (adp_inst_p->drv_adp != 0) {
        sap_err_log_h32(adp);
        sap_err_log_h32(adp_inst_p->drv_adp);
        return ERR_ADP_NOT_AVAILABLE;
    }

    adp_inst_p->drv_adp = adp;
    *adp_inst = adp_inst_p;
    return AUDIO_SUCCESS;
}

static td_s32 drv_adp_free_inst(td_handle adp)
{
    drv_adp_inst *adp_inst_p = TD_NULL;
    td_s32 ret = drv_adp_get_inst(&adp_inst_p, adp);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_adp_get_inst, ret);
        return ret;
    }

    drv_adp_check_inst(adp_inst_p->drv_adp, adp);

    (td_void)memset_s(adp_inst_p, sizeof(*adp_inst_p), 0, sizeof(drv_adp_inst));

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_init(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    shrb_init_cfg init_cfg;
    sap_adp_init *adp_init = (sap_adp_init *)arg;

    audio_unused(cmd);
    audio_unused(file);

    ret = drv_msg_adp_init(adp_init->core_cid, adp_init->init_reg);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_init, ret);
        return ret;
    }

    ret = core_adp_init();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_adp_init, ret);
        drv_msg_adp_deinit();
        return ret;
    }

    core_shrb_get_init_cfg(adp_init->init_reg, &init_cfg);
    adp_init->mem_type = init_cfg.mem_type;
    adp_init->cache    = init_cfg.cache;
    adp_init->size     = init_cfg.size;
    adp_init->fd       = init_cfg.fd;
    adp_init->phys_addr = init_cfg.phys_addr;

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_deinit(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);

    ret = core_adp_deinit();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_adp_deinit, ret);
        return ret;
    }

    ret = drv_msg_adp_deinit();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_deinit, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_get_def_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_adp_open *adp_open = (sap_adp_open *)arg;

    audio_unused(cmd);
    audio_unused(file);

    ret = drv_msg_adp_get_def_attr(adp_open->type, &adp_open->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_get_def_attr, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_open(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_adp_open *adp_info = (sap_adp_open *)arg;
    drv_adp_inst *adp_inst = TD_NULL;

    audio_unused(cmd);
    audio_unused(file);

    ret = drv_msg_adp_open(&adp_info->h_mod, adp_info->type, &adp_info->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_open, ret);
        return ret;
    }

    ret = core_adp_clone(adp_info->h_mod, adp_info->type);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_adp_clone, ret);
        drv_msg_adp_close(adp_info->h_mod);
        return ret;
    }

    ret = drv_adp_alloc_inst(&adp_inst, adp_info->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_adp_alloc_inst, ret);
        drv_msg_adp_close(adp_info->h_mod);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_close(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_drv_handle *adp_info = (sap_drv_handle *)arg;

    audio_unused(cmd);
    audio_unused(file);

    ret = drv_msg_adp_close(adp_info->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_close, ret);
        return ret;
    }

    ret = core_adp_destroy(adp_info->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_adp_destroy, ret);
        return ret;
    }

    ret = drv_adp_free_inst(adp_info->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_adp_free_inst, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_alloc_shrb(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_drv_attach_para *param = (sap_drv_attach_para *)arg;

    audio_unused(cmd);
    audio_unused(file);

#ifdef DRV_ADP_ALLOC_BY_A_CORE
    audio_adp_attr adp_attr;

    ret = drv_msg_adp_get_attr(param->h_src, &adp_attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_get_attr, ret);
        return ret;
    }

    if (adp_attr.rbuf_size == 0) {
        /* need to get attr from other module when use default size */
        ret = drv_msg_adp_sync_attr(param->h_src, param->h_sink, param->adp_type, param->data_port_id, &adp_attr);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(drv_msg_adp_sync_attr, ret);
            return ret;
        }
    }

    ret = core_adp_set_attr(param->h_src, &adp_attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_adp_set_attr, ret);
        return ret;
    }

    ret = core_adp_alloc_shrb(param->h_src);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_adp_alloc_shrb, ret);
        return ret;
    }

    ret = drv_msg_adp_attach_shrb(param->h_src);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_attach_shrb, ret);
        core_adp_free_shrb(param->h_src);
        return ret;
    }
#else
    ret = drv_msg_adp_alloc_shrb(param->h_src, param->h_sink, param->adp_type, param->data_port_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_alloc_shrb, ret);
        return ret;
    }

    ret = core_adp_attach_shrb(param->h_src);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_adp_attach_shrb, ret);
        drv_msg_adp_free_shrb(param->h_src);
        return ret;
    }
#endif

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_free_shrb(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_drv_handle *adp_info = (sap_drv_handle *)arg;

    audio_unused(cmd);
    audio_unused(file);

#ifdef DRV_ADP_ALLOC_BY_A_CORE
    ret = drv_msg_adp_detach_shrb(adp_info->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_detach_shrb, ret);
        return ret;
    }

    ret = core_adp_free_shrb(adp_info->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_adp_free_shrb, ret);
        return ret;
    }
#else
    ret = core_adp_detach_shrb(adp_info->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_adp_detach_shrb, ret);
        return ret;
    }

    ret = drv_msg_adp_free_shrb(adp_info->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_free_shrb, ret);
        return ret;
    }
#endif

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_map(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_drv_handle *adp_info = (sap_drv_handle *)arg;

    audio_unused(cmd);
    audio_unused(file);

    ret = core_adp_attach_shrb(adp_info->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_adp_attach_shrb, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_unmap(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_drv_handle *adp_info = (sap_drv_handle *)arg;

    audio_unused(cmd);
    audio_unused(file);

    ret = core_adp_detach_shrb(adp_info->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_adp_detach_shrb, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_get_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_adp_attr *adp_info = (sap_adp_attr *)arg;

    audio_unused(cmd);
    audio_unused(file);

    ret = drv_msg_adp_get_attr(adp_info->h_mod, &adp_info->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_get_attr, ret);
        return ret;
    }

    (td_void)core_adp_set_attr(adp_info->h_mod, &adp_info->attr);

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_set_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_adp_attr *adp_info = (sap_adp_attr *)arg;

    audio_unused(cmd);
    audio_unused(file);

    ret = drv_msg_adp_set_attr(adp_info->h_mod, &adp_info->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_set_attr, ret);
        return ret;
    }

    (td_void)core_adp_set_attr(adp_info->h_mod, &adp_info->attr);

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_attach_output(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_drv_attach_para *attach_para = (sap_drv_attach_para *)arg;

    audio_unused(cmd);
    audio_unused(file);

    ret = drv_msg_adp_attach_output(attach_para->h_src, attach_para->h_sink);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_attach_output, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_detach_output(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_drv_attach_para *attach_para = (sap_drv_attach_para *)arg;

    audio_unused(cmd);
    audio_unused(file);

    ret = drv_msg_adp_detach_output(attach_para->h_src, attach_para->h_sink);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_detach_output, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adp_register_event_proc(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_adp;
    sap_drv_event *adp_event = TD_NULL;
    drv_adp_inst *adp_inst = TD_NULL;

    audio_unused(cmd);
    audio_unused(file);

    adp_event = (sap_drv_event *)arg;
    drv_adp = adp_event->h_mod;

    ret = drv_adp_find_inst(&adp_inst, drv_adp);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_adp_find_inst, ret);
        return ret;
    }

    adp_inst->event_proc = adp_event->event_proc;
    adp_inst->context = adp_event->context;

    return AUDIO_SUCCESS;
}

static td_s32 drv_msg_adp_report_event(core_msg_data *msg_data)
{
    td_s32 ret;

    msg_data_adp_event_info *adp_event = (msg_data_adp_event_info *)msg_data->input_data;
    td_handle adp_handle = msg_data->handle;
    drv_adp_inst *adp_inst = TD_NULL;
    audio_event_proc event_proc = TD_NULL;

    ret = drv_adp_find_inst(&adp_inst, adp_handle);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_adp_find_inst, ret);
        return ret;
    }

    event_proc = (audio_event_proc)(td_uintptr_t)(adp_inst->event_proc);
    if (event_proc != TD_NULL) {
        event_proc(adp_handle, adp_event->event, &(adp_event->param), (td_void *)(td_uintptr_t)adp_inst->context);
    }

    return AUDIO_SUCCESS;
}

static const core_msg_module_func g_drv_msg_adp_func[MSG_DRV_ADP_CMD_MAX] = {
    [MSG_DRV_ADP_REPORT_EVENT] = {drv_msg_adp_report_event}
};

static td_s32 drv_msg_adp_callback(core_msg_data *msg_data)
{
    if (msg_data->cmd >= MSG_DRV_ADP_CMD_MAX) {
        sap_err_log_u32(msg_data->cmd);
        return ERR_ADP_INVALID_PARAM;
    }

    return g_drv_msg_adp_func[msg_data->cmd].func(msg_data);
}

td_s32 drv_msg_adp_module_init(audio_core_id core_id)
{
    td_s32 ret;

    ret = core_msg_register_module(core_id, MSG_CORE_ADP_MODULE, drv_msg_adp_callback);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_msg_register_module, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

#ifdef SAP_PROC_SUPPORT
td_s32 adp_read_proc(td_void *file, td_void *private)
{
    td_s32 ret;
    td_u8 adp_id;
    drv_adp_inst *adp_inst = TD_NULL;
    adp_proc_item proc;

    audio_unused(private);

    for (adp_id = 0; adp_id < MOD_ADP_MAX_NUM; adp_id++) {
        adp_inst = &g_drv_adp_insts[adp_id];
        if (adp_inst->drv_adp == 0) {
            continue;
        }

        (td_void)memset_s(&proc, sizeof(proc), 0, sizeof(proc));
        ret = core_adp_get_proc_info(adp_inst->drv_adp, &proc);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(core_adp_get_proc_info, ret);
            continue;
        }

        ret = drv_proc_adp_show(file, adp_inst->drv_adp, &proc);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(drv_proc_adp_show, ret);
            continue;
        }
    }

    return AUDIO_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
