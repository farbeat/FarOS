/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: adp core
 * Author: audio
 */

#ifdef PLATFORM_DRIVER_SUPPORT
#include "platform.h"
#endif
#include "securec.h"
#include "audio_struct.h"
#include "audio_debug.h"
#include "mem_addr.h"
#include "core_module.h"
#include "core_rbuf.h"
#include "core_shrb.h"
#include "core_adp.h"
#ifdef SAP_PROC_SUPPORT
#include "audio_osal.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ADP_ALIGN_BYTES 128

typedef struct {
    sap_mod_head head;

    shrb_handle shrb;
    shrb_buffer shrb_buf;
    audio_adp_type adp_type;
    td_bool shrb_alloc_flag;

    td_u32 data_buf_size;
    td_u32 data_buf_tail_size;
    td_u32 info_buf_size;

    connector_handle connector;

    td_void *context;
    audio_event_proc event_proc;

#ifdef SAP_PROC_SUPPORT
    /* send frame */
    td_u32 last_send_frm_time;
    td_u64 total_send_frm_interval;
    adp_proc_send_status send_frm_status;

    /* send stream */
    td_u32 last_send_strm_time;
    td_u64 total_send_strm_interval;
    adp_proc_send_status send_strm_status;

    /* get frame */
    td_u32 last_get_frm_time;
    td_u64 total_get_frm_interval;
    adp_proc_get_status get_frm_status;

    /* get stream */
    td_u32 last_get_strm_time;
    td_u64 total_get_strm_interval;
    adp_proc_get_status get_strm_status;

    /* get buf */
    td_u32 last_get_buf_time;
    td_u64 total_get_buf_interval;
    adp_proc_get_status get_buf_status;
#endif
} core_adp_inst;

static td_u32 g_adp_init_cnt = 0;
static sap_mod_pool_handle g_adp_pool = TD_NULL;

AUDIO_STATIC sap_mod_handle core_adp_get_module_handle(td_handle adp)
{
    td_s32 ret;
    sap_mod_handle adp_handle = TD_NULL;

    ret = core_mod_get_module(g_adp_pool, (td_u32)adp, &adp_handle);
    if (ret != AUDIO_SUCCESS) {
        audio_log_info("Call core_mod_get_module Failed, Error Code: [%#x]", ret);
        sap_trace_log_h32(adp);
        return TD_NULL;
    }

    return adp_handle;
}

#define core_adp_get_inst(adp_inst, adp)                                 \
    do {                                                                 \
        (adp_inst) = (core_adp_inst *)core_adp_get_module_handle((adp)); \
        if ((adp_inst) == TD_NULL) {                                     \
            return ERR_ADP_INVALID_HANDLE;                               \
        }                                                                \
    } while (0)

#ifdef SAP_PROC_SUPPORT
static inline td_void update_interval_limit(spend_status_info *spend_status, td_u32 interval)
{
    spend_status->cur_interval = interval;
    if (interval > spend_status->max_interval) {
        spend_status->max_interval = interval;
    }
    if (interval < spend_status->min_interval) {
        spend_status->min_interval = interval;
    }
}

static td_void update_send_time_interval(adp_proc_send_status *status, td_u32 *last_time,
    td_u64 *total_interval, td_u32 cur_time)
{
    if (status->send_success_cnt > 1) {
        /* 第1次无须计算帧间隔，否则会导致数据异常 */
        td_u32 time_diff = audio_osal_cali_time_diff_us(*last_time, cur_time);
        update_interval_limit(&status->spend_status, time_diff);
        *total_interval += time_diff;
        status->spend_status.avg_interval = (td_u32)((*total_interval) / (td_u64)(status->send_success_cnt - 1));
    }

    status->send_success_cnt++;
    *last_time = cur_time;
}

static td_void update_get_time_interval(adp_proc_get_status *status, td_u32 *last_time,
    td_u64 *total_interval, td_u32 cur_time)
{
    if (status->get_success_cnt > 1) {
        /* 第1次无须计算帧间隔，否则会导致数据异常 */
        td_u32 time_diff = audio_osal_cali_time_diff_us(*last_time, cur_time);
        update_interval_limit(&status->spend_status, time_diff);
        *total_interval += time_diff;
        status->spend_status.avg_interval = (td_u32)((*total_interval) / (td_u64)(status->get_success_cnt - 1));
    }

    status->get_success_cnt++;
    *last_time = cur_time;
}
#endif

static inline td_void adp_create(core_adp_inst *adp_inst, audio_adp_type adp_type, const audio_adp_attr *adp_attr)
{
    adp_inst->adp_type = adp_type;
    adp_inst->data_buf_size = audio_align_up(adp_attr->rbuf_size, ADP_ALIGN_BYTES);
    adp_inst->data_buf_tail_size = audio_align_up(adp_attr->tail_size, ADP_ALIGN_BYTES);
    adp_inst->info_buf_size = 0;
    adp_inst->shrb_alloc_flag = TD_FALSE;

#ifdef SAP_PROC_SUPPORT
    adp_inst->send_frm_status.spend_status.min_interval = 0xFFFFFFFF;
    adp_inst->send_strm_status.spend_status.min_interval = 0xFFFFFFFF;
    adp_inst->get_frm_status.spend_status.min_interval = 0xFFFFFFFF;
    adp_inst->get_strm_status.spend_status.min_interval = 0xFFFFFFFF;
    adp_inst->get_buf_status.spend_status.min_interval = 0xFFFFFFFF;
#endif
}

static inline td_s32 adp_destroy(const core_adp_inst *adp_inst)
{
    if (adp_inst->shrb_alloc_flag == TD_TRUE) {
        return ERR_ADP_INST_BUSY;
    }
    return AUDIO_SUCCESS;
}

static td_s32 adp_event_proc(td_void *handle, td_s32 event, td_void *param, td_void *context)
{
    core_adp_inst *adp_inst = (core_adp_inst *)handle;
    adp_check_null_ptr_return(adp_inst);

    switch (event) {
        case AUDIO_EVENT_GET_EOS: /* direct return */
            return core_shrb_get_eos(adp_inst->shrb, (td_bool *)param);

        case AUDIO_EVENT_REPORT_EOS: /* direct return */
            sap_trace_log_h32(core_mod_get_handle(adp_inst));
            sap_trace_log_info("adp eos event process");
            if (adp_inst->event_proc != TD_NULL) {
                adp_inst->event_proc((td_handle)core_mod_get_handle(adp_inst), ADP_EVENT_EOS, param, adp_inst->context);
            }
            return AUDIO_SUCCESS;

        case AUDIO_EVENT_REPORT_INPUT_EMPTY:
            if (adp_inst->event_proc != TD_NULL) {
                adp_inst->event_proc((td_handle)core_mod_get_handle(adp_inst), ADP_EVENT_EMPTY,
                    param, adp_inst->context);
            }
            return AUDIO_SUCCESS;

        case AUDIO_EVENT_GET_DELAY: /* direct return */
            return AUDIO_SUCCESS;

        default:
            break;
    }

    audio_unused(context);
    return AUDIO_SUCCESS;
}

static td_s32 adp_attach_input_connector(sap_mod_handle module, connector_handle connector)
{
    core_adp_inst *adp_inst = (core_adp_inst *)module;
    adp_check_null_ptr_return(adp_inst);

    /* adp only support one input */
    if (adp_inst->connector != TD_NULL) {
        return ERR_ADP_NOT_SUPPORTED;
    }

    adp_inst->connector = connector;
    return AUDIO_SUCCESS;
}

static td_s32 adp_detach_input_connector(sap_mod_handle module, const connector_handle connector)
{
    core_adp_inst *adp_inst = (core_adp_inst *)module;
    adp_check_null_ptr_return(adp_inst);

    if (adp_inst->connector != connector) {
        return ERR_ADP_INVALID_PARAM;
    }

    adp_inst->connector = TD_NULL;
    return AUDIO_SUCCESS;
}

static inline td_void adp_detach_all_output(core_adp_inst *adp_inst)
{
    adp_inst->connector = TD_NULL;
}

static td_s32 adp_detach_output_connector(sap_mod_handle module, const connector_handle connector, td_u32 data_source)
{
    core_adp_inst *adp_inst = (core_adp_inst *)module;
    adp_check_null_ptr_return(adp_inst);

    if (adp_inst->connector != connector) {
        sap_err_log_u32(adp_inst->adp_type);
        return ERR_ADP_INVALID_PARAM;
    }

    adp_inst->connector = TD_NULL;
    audio_unused(data_source);
    return AUDIO_SUCCESS;
}

static td_s32 adp_attach_output_connector(sap_mod_handle module, connector_handle connector, td_u32 data_source)
{
    core_adp_inst *adp_inst = (core_adp_inst *)module;
    adp_check_null_ptr_return(adp_inst);

    /* adp only support one output */
    if (adp_inst->connector != TD_NULL) {
        sap_err_log_u32(adp_inst->adp_type);
        return ERR_ADP_NOT_SUPPORTED;
    }

    audio_unused(data_source);
    adp_inst->connector = connector;
    return AUDIO_SUCCESS;
}

static const sap_mod_ops g_core_adp_ops = {
    .attach_input_connector = adp_attach_input_connector,
    .detach_input_connector = adp_detach_input_connector,
    .detach_output_connector = adp_detach_output_connector,
    .attach_output_connector = adp_attach_output_connector,
};

td_s32 core_adp_init(td_void)
{
    td_s32 ret;

    audio_func_enter();
    if (g_adp_init_cnt > 0) {
        g_adp_init_cnt++;
        audio_func_exit();
        return AUDIO_SUCCESS;
    }

    ret = core_mod_init(&g_adp_pool, SAP_MOD_ADP, sizeof(core_adp_inst), MOD_ADP_MAX_NUM);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_mod_init, ret);
        return ret;
    }

    g_adp_init_cnt++;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_adp_deinit(td_void)
{
    td_s32 ret;

    audio_func_enter();
    if (g_adp_init_cnt == 0) {
        audio_func_exit();
        return AUDIO_SUCCESS;
    }
    if (g_adp_init_cnt > 1) {
        g_adp_init_cnt--;
        audio_func_exit();
        return AUDIO_SUCCESS;
    }

    ret = core_mod_deinit(g_adp_pool);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_mod_deinit, ret);
        return ret;
    }
    g_adp_pool = TD_NULL;

    g_adp_init_cnt--;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_bool core_adp_check_no_inst(td_void)
{
    if (g_adp_pool == TD_NULL) {
        return TD_TRUE;
    }

    return (g_adp_pool->used_num > 0 ? TD_FALSE : TD_TRUE);
}

td_s32 core_adp_get_def_attr(audio_adp_type type, audio_adp_attr *attr)
{
    if (type >= ADP_TYPE_MAX) {
        return ERR_ADP_INVALID_PARAM;
    }

    adp_check_null_ptr_return(attr);

    return memset_s(attr, sizeof(audio_adp_attr), 0, sizeof(audio_adp_attr));
}

td_s32 core_adp_create(td_handle *adp, audio_adp_type type, const audio_adp_attr *attr)
{
    td_s32 ret;
    sap_mod_handle adp_handle = TD_NULL;
    core_adp_inst *adp_inst = TD_NULL;

    audio_func_enter();

    adp_check_null_ptr_return(adp);
    adp_check_null_ptr_return(attr);

    if ((attr->info_buf_size + attr->rbuf_size + attr->tail_size) >= SHM0_SIZE) {
        return ERR_ADP_INVALID_PARAM;
    }

    ret = core_mod_alloc(g_adp_pool, &adp_handle);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_mod_alloc, ret);
        return ret;
    }
    adp_handle->ops = &g_core_adp_ops;
    adp_inst = (core_adp_inst *)adp_handle;

    adp_create(adp_inst, type, attr);

    core_mod_register_event_proc(adp_handle, adp_event_proc, TD_NULL);

    *adp = (td_handle)core_mod_get_handle(adp_inst);
#if (SAP_CHIP_TYPE != socmn1)
    sap_alert_log_h32(*adp);
#endif
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_adp_clone(td_handle adp, audio_adp_type type)
{
    td_s32 ret;
    sap_mod_handle adp_handle = TD_NULL;
    audio_adp_attr adp_attr;
    core_adp_inst *adp_inst = TD_NULL;

    audio_func_enter();

    ret = core_mod_alloc_by_id(g_adp_pool, (td_u32)adp, &adp_handle);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_mod_alloc_by_id, ret);
        return ret;
    }
    adp_handle->ops = &g_core_adp_ops;
    adp_inst = (core_adp_inst *)adp_handle;

    adp_attr.rbuf_size = 0;
    adp_attr.tail_size = 0;
    adp_attr.info_buf_size = 0;
    adp_create(adp_inst, type, &adp_attr);

    core_mod_register_event_proc(adp_handle, adp_event_proc, TD_NULL);

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_adp_destroy(td_handle adp)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    audio_func_enter();
#if (SAP_CHIP_TYPE != socmn1)
    sap_alert_log_h32(adp);
#endif

    ret = adp_destroy(adp_inst);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(adp_destroy, ret);
        return ret;
    }

    adp_detach_all_output(adp_inst);

    ret = core_mod_free(g_adp_pool, &adp_inst->head);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_mod_free, ret);
        return ret;
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_adp_get_attr(td_handle adp, audio_adp_attr *attr)
{
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    audio_func_enter();

    adp_check_null_ptr_return(attr);

    attr->rbuf_size = adp_inst->data_buf_size;
    attr->tail_size = adp_inst->data_buf_tail_size;
    attr->info_buf_size = adp_inst->info_buf_size;

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_adp_set_attr(td_handle adp, const audio_adp_attr *attr)
{
    core_adp_inst *adp_inst = TD_NULL;

    audio_func_enter();
    adp_check_null_ptr_return(attr);

    if ((attr->info_buf_size + attr->rbuf_size + attr->tail_size) >= SHM0_SIZE) {
        return ERR_ADP_INVALID_PARAM;
    }

    core_adp_get_inst(adp_inst, adp);

    adp_inst->data_buf_size = audio_align_up(attr->rbuf_size, ADP_ALIGN_BYTES);
    adp_inst->data_buf_tail_size = audio_align_up(attr->tail_size, ADP_ALIGN_BYTES);
    adp_inst->info_buf_size = audio_align_up(attr->info_buf_size, ADP_ALIGN_BYTES);

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_adp_alloc_shrb(td_handle adp)
{
    td_s32 ret;
    shrb_attr attr;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    audio_func_enter();

    attr.data_buf_size = adp_inst->data_buf_size;
    attr.data_buf_tail_size = adp_inst->data_buf_tail_size;
    attr.info_buf_size = adp_inst->info_buf_size;
    ret = core_shrb_alloc(&adp_inst->shrb, (td_u32)adp, &attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_alloc, ret);
        return ret;
    }

    ret = core_shrb_attach(adp_inst->shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_attach, ret);
        (td_void)core_shrb_free(adp_inst->shrb);
        return ret;
    }

    ret = core_shrb_get_rbuf(adp_inst->shrb, &adp_inst->shrb_buf);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_get_rbuf, ret);
        return ret;
    }

    adp_inst->shrb_alloc_flag = TD_TRUE;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_adp_free_shrb(td_handle adp)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    audio_func_enter();

    if (adp_inst->shrb_alloc_flag != TD_TRUE) {
        return ERR_ADP_NOT_SUPPORTED;
    }

    ret = core_shrb_detach(adp_inst->shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_detach, ret);
        return ret;
    }

    ret = core_shrb_free(adp_inst->shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_free, ret);
        return ret;
    }

    adp_inst->shrb = TD_NULL;
    adp_inst->shrb_alloc_flag = TD_FALSE;
    adp_inst->shrb_buf.data_buf = TD_NULL;
    adp_inst->shrb_buf.info_buf = TD_NULL;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_adp_attach_shrb(td_handle adp)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    audio_func_enter();

    ret = core_shrb_get_id(adp, &adp_inst->shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_get_id, ret);
        return ret;
    }

    ret = core_shrb_attach(adp_inst->shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_attach, ret);
        return ret;
    }

    ret = core_shrb_get_rbuf(adp_inst->shrb, &adp_inst->shrb_buf);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_get_rbuf, ret);
        return ret;
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_adp_detach_shrb(td_handle adp)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    audio_func_enter();

    ret = core_shrb_detach(adp_inst->shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_detach, ret);
        return ret;
    }

    adp_inst->shrb_buf.data_buf = TD_NULL;
    adp_inst->shrb_buf.info_buf = TD_NULL;

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_adp_get_rbuf(td_handle adp, rbuf_handle *data_buf, rbuf_handle *info_buf)
{
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    adp_check_null_ptr_return(data_buf);
    adp_check_null_ptr_return(info_buf);

    *data_buf = adp_inst->shrb_buf.data_buf;
    *info_buf = adp_inst->shrb_buf.info_buf;
    return AUDIO_SUCCESS;
}

td_s32 core_adp_query_free(td_handle adp, td_u32 *free_size)
{
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    return core_shrb_query_free(&adp_inst->shrb_buf, free_size);
}

td_s32 core_adp_query_busy(td_handle adp, td_u32 *busy_size)
{
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    return core_shrb_query_busy(&adp_inst->shrb_buf, busy_size);
}

td_s32 core_adp_get_buffer(td_handle adp, audio_stream_buf *stream)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    adp_check_null_ptr_return(stream);

    ret = core_shrb_get_buffer(&adp_inst->shrb_buf, stream);
    if (ret != AUDIO_SUCCESS) {
        goto fail;
    }

    if (stream->data == TD_NULL || stream->size == 0) {
        ret = ERR_ADP_BUF_FULL;
        goto fail;
    }

#ifdef SAP_PROC_SUPPORT
    update_get_time_interval(&adp_inst->get_buf_status, &adp_inst->last_get_buf_time,
        &adp_inst->total_get_buf_interval, audio_osal_gettime_us());
#endif

    return AUDIO_SUCCESS;

fail:
#ifdef SAP_PROC_SUPPORT
    adp_inst->get_buf_status.get_fail_cnt++;
#endif
    return ret;
}

td_s32 core_adp_put_buffer(td_handle adp, const audio_stream_buf *stream)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    adp_check_null_ptr_return(stream);
    adp_check_null_ptr_return(stream->data);

    ret = core_check_bool(stream->eos);
    if (ret != AUDIO_SUCCESS) {
        return ERR_ADP_INVALID_PARAM;
    }

    ret = core_check_bool(stream->pkg_loss);
    if (ret != AUDIO_SUCCESS) {
        return ERR_ADP_INVALID_PARAM;
    }

    ret = core_shrb_put_buffer(&adp_inst->shrb_buf, stream);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_put_buffer, ret);
        sap_err_log_h32(adp);
        goto fail;
    }

    ret = core_shrb_set_eos(adp_inst->shrb, stream->eos);
    if (ret != AUDIO_SUCCESS) {
        goto fail;
    }

#ifdef SAP_PROC_SUPPORT
    adp_inst->get_buf_status.release_success_cnt++;
#endif

    return AUDIO_SUCCESS;

fail:
#ifdef SAP_PROC_SUPPORT
    adp_inst->get_buf_status.release_fail_cnt++;
#endif
    return ret;
}

td_s32 core_adp_send_stream(td_handle adp, const audio_stream_buf *stream)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    ret = core_shrb_send_stream(&adp_inst->shrb_buf, stream);
    if (ret != AUDIO_SUCCESS) {
        goto fail;
    }

    ret = core_shrb_set_eos(adp_inst->shrb, stream->eos);
    if (ret != AUDIO_SUCCESS) {
        goto fail;
    }

#ifdef SAP_PROC_SUPPORT
    update_send_time_interval(&adp_inst->send_strm_status, &adp_inst->last_send_strm_time,
        &adp_inst->total_send_strm_interval, audio_osal_gettime_us());
#endif

    return AUDIO_SUCCESS;

fail:
#ifdef SAP_PROC_SUPPORT
    adp_inst->send_strm_status.send_fail_cnt++;
#endif
    return ret;
}

td_s32 core_adp_acquire_stream(td_handle adp, audio_stream_buf *stream)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    adp_check_null_ptr_return(stream);

    stream->pts = 0LL;
    stream->data = TD_NULL;
    stream->size = 0;
    stream->eos = TD_FALSE;
    stream->pkg_loss = TD_FALSE;
    ret = core_shrb_acquire_stream(&adp_inst->shrb_buf, stream);
    if (ret != AUDIO_SUCCESS) {
        goto fail;
    }

    if (stream->data == TD_NULL || stream->size == 0) {
        ret = ERR_ADP_BUF_EMPTY;
        goto fail;
    }

#ifdef SAP_PROC_SUPPORT
    update_get_time_interval(&adp_inst->get_strm_status, &adp_inst->last_get_strm_time,
        &adp_inst->total_get_strm_interval, audio_osal_gettime_us());
#endif

    return AUDIO_SUCCESS;

fail:
#ifdef SAP_PROC_SUPPORT
    adp_inst->get_strm_status.get_fail_cnt++;
#endif
    return ret;
}

td_s32 core_adp_release_stream(td_handle adp, const audio_stream_buf *stream)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    ret = core_shrb_release_stream(&adp_inst->shrb_buf, stream);
#ifdef SAP_PROC_SUPPORT
    if (ret != AUDIO_SUCCESS) {
        adp_inst->get_strm_status.release_fail_cnt++;
    } else {
        adp_inst->get_strm_status.release_success_cnt++;
    }
#endif

    return ret;
}

td_s32 core_adp_send_frame(td_handle adp, const audio_frame *frame)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    ret = core_shrb_send_frame(&adp_inst->shrb_buf, frame);
    if (ret != AUDIO_SUCCESS) {
        goto fail;
    }

    ret = core_shrb_set_eos(adp_inst->shrb, frame->eos);
    if (ret != AUDIO_SUCCESS) {
        goto fail;
    }

#ifdef SAP_PROC_SUPPORT
    update_send_time_interval(&adp_inst->send_frm_status, &adp_inst->last_send_frm_time,
        &adp_inst->total_send_frm_interval, audio_osal_gettime_us());
#endif

    return AUDIO_SUCCESS;

fail:
#ifdef SAP_PROC_SUPPORT
    adp_inst->send_frm_status.send_fail_cnt++;
#endif
    return ret;
}

td_s32 core_adp_acquire_frame(td_handle adp, audio_frame *frame)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    adp_check_null_ptr_return(frame);

    ret = core_shrb_acquire_frame(&adp_inst->shrb_buf, frame);
    if (ret != AUDIO_SUCCESS) {
        goto fail;
    }

    if (frame->bits_buffer == TD_NULL || frame->bits_bytes == 0) {
        ret = ERR_ADP_BUF_EMPTY;
        goto fail;
    }

#ifdef SAP_PROC_SUPPORT
    update_get_time_interval(&adp_inst->get_frm_status, &adp_inst->last_get_frm_time,
        &adp_inst->total_get_frm_interval, audio_osal_gettime_us());
#endif

    return AUDIO_SUCCESS;

fail:
#ifdef SAP_PROC_SUPPORT
    adp_inst->get_strm_status.get_fail_cnt++;
#endif
    return ret;
}

td_s32 core_adp_release_frame(td_handle adp, const audio_frame *frame)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    ret = core_shrb_release_frame(&adp_inst->shrb_buf, frame);
#ifdef SAP_PROC_SUPPORT
    if (ret != AUDIO_SUCCESS) {
        adp_inst->get_frm_status.release_fail_cnt++;
    } else {
        adp_inst->get_frm_status.release_success_cnt++;
    }
#endif

    return ret;
}

td_s32 core_adp_register_event_proc(td_handle adp, audio_event_proc event_proc, td_void *context)
{
    core_adp_inst *adp_inst = TD_NULL;
    core_adp_get_inst(adp_inst, adp);

    adp_inst->event_proc = event_proc;
    adp_inst->context = context;

    return AUDIO_SUCCESS;
}

static td_s32 adp_get_proc_info(const core_adp_inst *adp_inst, adp_proc_item *proc)
{
    /* attr */
    proc->adp_type = adp_inst->adp_type;
    proc->adp_attr.rbuf_size = adp_inst->data_buf_size;
    proc->adp_attr.tail_size = adp_inst->data_buf_tail_size;
    proc->adp_attr.info_buf_size = adp_inst->info_buf_size;

    /* buf status */
    (td_void)core_shrb_query_busy(&adp_inst->shrb_buf, &(proc->busy_size));
    (td_void)core_shrb_query_free(&adp_inst->shrb_buf, &(proc->free_size));

#ifdef SAP_PROC_SUPPORT
    /* input status */
    (td_void)memcpy_s(&proc->send_frm_status, sizeof(proc->send_frm_status),
        &adp_inst->send_frm_status, sizeof(adp_inst->send_frm_status));
    (td_void)memcpy_s(&proc->send_strm_status, sizeof(proc->send_strm_status),
        &adp_inst->send_strm_status, sizeof(adp_inst->send_strm_status));

    /* output status */
    (td_void)memcpy_s(&proc->get_frm_status, sizeof(proc->get_frm_status),
        &adp_inst->get_frm_status, sizeof(adp_inst->get_frm_status));
    (td_void)memcpy_s(&proc->get_strm_status, sizeof(proc->get_strm_status),
        &adp_inst->get_strm_status, sizeof(adp_inst->get_strm_status));
    (td_void)memcpy_s(&proc->get_buf_status, sizeof(proc->get_buf_status),
        &adp_inst->get_buf_status, sizeof(adp_inst->get_buf_status));
#endif

    return AUDIO_SUCCESS;
}

td_s32 core_adp_get_proc_info(td_handle adp, adp_proc_item *proc)
{
    td_s32 ret;
    core_adp_inst *adp_inst = TD_NULL;

    audio_func_enter();
    adp_check_null_ptr_return(proc);

    core_adp_get_inst(adp_inst, adp);

    (td_void)memset_s(proc, sizeof(*proc), 0, sizeof(*proc));
    ret = adp_get_proc_info(adp_inst, proc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(adp_get_proc_info, ret);
        return ret;
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

#ifdef PLATFORM_DRIVER_SUPPORT
static platform_driver g_core_adp_driver = {
    .module_id = SAP_MOD_ADP,
    .init = core_adp_init,
    .deinit = core_adp_deinit,
    .suspend = TD_NULL,
    .resume = TD_NULL,
    .get_mod_handle = core_adp_get_module_handle,
};
module_driver(g_core_adp_driver);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
