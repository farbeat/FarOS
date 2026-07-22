/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: source file for drv adp proc
 * Author: audio
 * Create: 2023-07-01
 */

#include "drv_proc_adp.h"
#include "audio_struct.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "drv_audio_proc.h"
#include "drv_audio_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef SAP_PROC_SUPPORT
typedef struct {
    audio_adp_type type;
    const td_char *name;
} adp_type_proc_name_item;

static const adp_type_proc_name_item g_adp_type_proc_name_list[] = {
    {ADP_INPUT, "input"},
    {ADP_OUTPUT, "output"},
    {ADP_CONNECTOR, "connect"},
};

static const td_char *get_adp_type_name(audio_adp_type type)
{
    td_u8 i;
    for (i = 0; i < (sizeof(g_adp_type_proc_name_list) / sizeof(g_adp_type_proc_name_list[0])); i++) {
        if (type == g_adp_type_proc_name_list[i].type) {
            return g_adp_type_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static td_void drv_adp_proc_show_basic_status(td_void *file, td_handle adp, const adp_proc_item *proc)
{
    drv_audio_proc_print_title(file, "basic status");

    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10s |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "handle", adp,
        "adp_type", get_adp_type_name(proc->adp_type),
        "busy_size", proc->busy_size,
        "free_size", proc->free_size);
}

static td_void drv_adp_proc_show_attr(td_void *file, const adp_proc_item *proc)
{
    drv_audio_proc_print_title(file, "attr");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
        "data_buf_size", proc->adp_attr.rbuf_size,
        "data_tail_size", proc->adp_attr.tail_size,
        "info_buf_size", proc->adp_attr.info_buf_size,
        "", "");
}

static td_void show_spend_status(td_void *file, const spend_status_info *spend)
{
    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10d |" "%-16s: %-10u |" "\n",
        "cur_interval", spend->cur_interval,
        "max_interval", spend->max_interval,
        "min_interval", spend->min_interval,
        "avg_interval", spend->avg_interval);
}

static td_void proc_show_send_frame_status(td_void *file, const adp_proc_item *proc)
{
    drv_audio_proc_print_title(file, "send frame status");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "send_success", proc->send_frm_status.send_success_cnt,
        "send_fail", proc->send_frm_status.send_fail_cnt,
        "", "",
        "", "");

    show_spend_status(file, &proc->send_frm_status.spend_status);
}

static td_void proc_show_send_stream_status(td_void *file, const adp_proc_item *proc)
{
    drv_audio_proc_print_title(file, "send stream status");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "send_success", proc->send_strm_status.send_success_cnt,
        "send_fail", proc->send_strm_status.send_fail_cnt,
        "", "",
        "", "");

    show_spend_status(file, &proc->send_strm_status.spend_status);
}

static td_void proc_show_get_frame_status(td_void *file, const adp_proc_item *proc)
{
    drv_audio_proc_print_title(file, "get frame status");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "get_success", proc->get_frm_status.get_success_cnt,
        "get_fail", proc->get_frm_status.get_fail_cnt,
        "release_success", proc->get_frm_status.release_success_cnt,
        "release_fail", proc->get_frm_status.release_fail_cnt);

    show_spend_status(file, &proc->get_frm_status.spend_status);
}

static td_void proc_show_get_stream_status(td_void *file, const adp_proc_item *proc)
{
    drv_audio_proc_print_title(file, "get stream status");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "get_success", proc->get_strm_status.get_success_cnt,
        "get_fail", proc->get_strm_status.get_fail_cnt,
        "release_success", proc->get_strm_status.release_success_cnt,
        "release_fail", proc->get_strm_status.release_fail_cnt);

    show_spend_status(file, &proc->get_strm_status.spend_status);
}

static td_void proc_show_get_buf_status(td_void *file, const adp_proc_item *proc)
{
    drv_audio_proc_print_title(file, "get buf status");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "%-16s: %-10u |" "\n",
        "get_success", proc->get_buf_status.get_success_cnt,
        "get_fail", proc->get_buf_status.get_fail_cnt,
        "release_success", proc->get_buf_status.release_success_cnt,
        "release_fail", proc->get_buf_status.release_fail_cnt);

    show_spend_status(file, &proc->get_buf_status.spend_status);
}

td_s32 drv_proc_adp_show(td_void *file, td_handle adp, const adp_proc_item *proc)
{
    td_u8 i;

    drv_audio_proc_print_head(file, "adp%d", drv_audio_proc_get_mod_index(adp));

    /* basic status */
    drv_adp_proc_show_basic_status(file, adp, proc);

    /* attr */
    drv_adp_proc_show_attr(file, proc);

    if (proc->adp_type != ADP_OUTPUT) {
        /* input status */
        proc_show_send_frame_status(file, proc);
        proc_show_send_stream_status(file, proc);
    }

    if (proc->adp_type != ADP_INPUT) {
        /* output status */
        proc_show_get_frame_status(file, proc);
        proc_show_get_stream_status(file, proc);
        proc_show_get_buf_status(file, proc);
    }

    drv_audio_proc_print_tail(file);

    return AUDIO_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
