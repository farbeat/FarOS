/*
 * Copyright (c) CompanyNameMagicTag 2019-2021. All rights reserved.
 * Description: hal interface
 * Author: audio
 */

#include "audio_osal.h"
#include "audio_debug.h"
#include "mem_addr.h"
#include "core_msg.h"
#include "drv_audio_osal.h"
#include "drv_audio_dev.h"
#include "drv_sys.h"
#include "drv_adp.h"
#include "drv_adec.h"
#include "drv_ao.h"
#include "drv_ai.h"
#include "drv_haid.h"
#include "drv_vad.h"
#include "drv_aenc.h"
#include "drv_sea.h"
#include "drv_ab.h"
#include "drv_hal.h"
#include "drv_overlay_load.h"
#include "drv_sap_msg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef SOC_SUPPORT_DSP_MSG_TO_APPS
#define SAP_MSG_TASK_PRIO 20
#define SAP_MSG_TASK_SIZE 0xB00
#define SAP_MSG_TASK_NAME "sap_msg"

#define SAP_MSG_EVENT_NUM 128
#define SAP_MSG_EVENT_SIZE sizeof(td_u16)
#define SAP_MSG_QUEUE_TIMEOUT 0 /* Set 0 for ISR */
#define SAP_MSG_WAIT_FOREVER 0xFFFFFFFF

#define sap_msg_gen_event_id(mod, event) ((((mod) << 8) & 0xFF00) | ((event) & 0x00FF))

typedef struct {
    dsp_int_cfg int_cfg;
    td_ulong msg_qid;
    osal_task *msg_task;
} dsp_int_mgr;

static dsp_int_mgr g_dsp_int_mgr = {
    .msg_qid = 0L,
    .msg_task = TD_NULL,
};

static td_void sap_isr_proc(td_u32 param)
{
    td_u32 raw_int;
    td_u32 sw_int;
    td_u16 msg_data = 0;
    dsp_int_mgr *int_mgr = &g_dsp_int_mgr;
    dsp_int_cfg *int_cfg = &int_mgr->int_cfg;

    audio_reg_read(int_cfg->reg_int, raw_int);

    if ((raw_int & int_cfg->int_msg_bit) != 0) {
        audio_reg_write(int_cfg->reg_int_clr, int_cfg->int_clr_bit);

        msg_data = 0;
        osal_msg_queue_write_copy(int_mgr->msg_qid, &msg_data, sizeof(msg_data), SAP_MSG_QUEUE_TIMEOUT);
    }

    if ((raw_int & int_cfg->int_vad_bit) != 0) {
        drv_vad_set_enable(TD_FALSE);

        msg_data = (td_u16)sap_msg_gen_event_id(MSG_CORE_AI_MODULE, VAD_EVENT_VAD_VALID);
        osal_msg_queue_write_copy(int_mgr->msg_qid, &msg_data, sizeof(msg_data), SAP_MSG_QUEUE_TIMEOUT);
    }

    audio_unused(param);
}

static td_s32 sap_register_int(dsp_int_cfg *int_cfg)
{
    td_s32 ret;
    td_u32 int_mask;

    ret = audio_osal_irq_request(int_cfg->irq_no,
        (osal_irq_handler)(td_uintptr_t)sap_isr_proc,
        TD_NULL, TD_NULL, TD_NULL);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(audio_osal_irq_request, ret);
        return ret;
    }
    audio_osal_irq_enable(int_cfg->irq_no);

    audio_reg_read(int_cfg->reg_int_mask, int_mask);
    int_mask |= int_cfg->int_msg_bit;
    audio_reg_write(int_cfg->reg_int_mask, int_mask);
    return AUDIO_SUCCESS;
}

static td_void sap_unreg_int(dsp_int_cfg *int_cfg)
{
    audio_osal_irq_disable(int_cfg->irq_no);
    audio_osal_irq_free(int_cfg->irq_no, TD_NULL);
}

static td_s32 sap_msg_proc(td_void *param)
{
    td_s32 ret;
    td_u16 vad_event = 0;
    td_u16 msg_data = 0;
    td_u32 msg_size = (td_u32)sizeof(msg_data);
    dsp_int_mgr *int_mgr = (dsp_int_mgr *)param;

    while (TD_TRUE) {
        ret = osal_msg_queue_read_copy(int_mgr->msg_qid, &msg_data, &msg_size, SAP_MSG_WAIT_FOREVER);
        if (ret != OSAL_SUCCESS) {
            continue;
        }

        if (msg_data == 0) {
            core_msg_process(AUDIO_CORE_DSP0);
        } else {
            vad_event = (td_u16)sap_msg_gen_event_id(MSG_CORE_AI_MODULE, VAD_EVENT_VAD_VALID);
            if (msg_data == vad_event) {
                drv_vad_intr_proc();
            }
        }
    }

    audio_unused(param);
    return AUDIO_SUCCESS;
}

static td_s32 sap_msg_task_create(dsp_int_mgr *int_mgr)
{
    td_s32 ret;

    osal_msg_queue_create(SAP_MSG_TASK_NAME, SAP_MSG_EVENT_NUM, &int_mgr->msg_qid, 0, SAP_MSG_EVENT_SIZE);
    int_mgr->msg_task = osal_kthread_create(sap_msg_proc, (td_void *)int_mgr, SAP_MSG_TASK_NAME, SAP_MSG_TASK_SIZE);
    if (int_mgr->msg_task == TD_NULL) {
        sap_err_log_fun(osal_kthread_create, TD_NULL);
        osal_msg_queue_delete(int_mgr->msg_qid);
        return AUDIO_FAILURE;
    }

#if (SAP_CHIP_TYPE != socmn1)
    (td_void)osal_kthread_set_priority(int_mgr->msg_task, SAP_MSG_TASK_PRIO);
#endif

    ret = sap_register_int(&int_mgr->int_cfg);
    if (ret != AUDIO_SUCCESS) {
        osal_kthread_destroy(int_mgr->msg_task, TD_TRUE);
        osal_msg_queue_delete(int_mgr->msg_qid);
        return ret;
    }

    return AUDIO_SUCCESS;
}

static td_void sap_msg_task_destroy(dsp_int_mgr *int_mgr)
{
    sap_unreg_int(&int_mgr->int_cfg);
    osal_kthread_destroy(int_mgr->msg_task, TD_TRUE);
    osal_msg_queue_delete(int_mgr->msg_qid);
    int_mgr->msg_task = TD_NULL;
    int_mgr->msg_qid = 0L;
}

#endif /* SOC_SUPPORT_DSP_MSG_TO_APPS */

td_s32 drv_sap_msg_start(audio_core_id core_id)
{
#ifdef SOC_SUPPORT_DSP_MSG_TO_APPS
    td_s32 ret;
    dsp_int_mgr *int_mgr = &g_dsp_int_mgr;

    ret = drv_msg_sys_module_init(core_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_sys_module_init, ret);
        return ret;
    }

    ret = drv_msg_ai_module_init(core_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_module_init, ret);
        return ret;
    }
#ifdef SAP_HAID_SUPPORT
    ret = drv_msg_haid_module_init(core_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_haid_module_init, ret);
        return ret;
    }
#endif
    ret = drv_msg_sea_module_init(core_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_sea_module_init, ret);
        return ret;
    }

    ret = drv_msg_adp_module_init(core_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adp_module_init, ret);
        return ret;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        hal_dsp_get_int_cfg(core_id, &int_mgr->int_cfg);
        ret = sap_msg_task_create(int_mgr);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(sap_msg_task_create, ret);
            return ret;
        }
    }
#endif /* SOC_SUPPORT_DSP_MSG_TO_APPS */
    audio_unused(core_id);
    return AUDIO_SUCCESS;
}

td_s32 drv_sap_msg_stop(audio_core_id core_id)
{
#ifdef SOC_SUPPORT_DSP_MSG_TO_APPS
    dsp_int_mgr *int_mgr = &g_dsp_int_mgr;

    if (core_id == AUDIO_CORE_DSP0) {
        sap_msg_task_destroy(int_mgr);
    }
#endif /* SOC_SUPPORT_DSP_MSG_TO_APPS */
    audio_unused(core_id);
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
