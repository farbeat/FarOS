/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: source file for drv dpm proc
 * Author: audio
 * Create: 2021-08-08
 */

#include "drv_proc_dpm.h"
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
    dpm_id dpm_lib_id;
    const td_char *dpm_name;
} dpm_proc_name_item;

static const dpm_proc_name_item g_dpm_proc_name_list[] = {
    {DPM_ID_FIR, "fir"},
};

static const td_char *drv_audio_proc_get_dpm_name(dpm_id id)
{
    td_u32 i;

    for (i = 0; i < (sizeof(g_dpm_proc_name_list) / sizeof(g_dpm_proc_name_list[0])); i++) {
        if (id == g_dpm_proc_name_list[i].dpm_lib_id) {
            return g_dpm_proc_name_list[i].dpm_name;
        }
    }

    return "unknown";
}

static td_void drv_dpm_proc_show_attr(td_void *file, const dpm_proc_item *proc)
{
    drv_audio_proc_print_title(file, "attr");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "dpm name", drv_audio_proc_get_dpm_name(proc->dpm_id),
        "", "",
        "", "",
        "", "");
}

static td_void drv_dpm_proc_show_basic_status(td_void *file, td_handle dpm, const dpm_proc_item *proc)
{
    drv_audio_proc_print_title(file, "basic status");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "handle", dpm,
        "state", drv_audio_proc_get_state((audio_state)(proc->state)),
        "bypass", drv_audio_proc_get_bool(proc->bypass),
        "eos", drv_audio_proc_get_bool(proc->eos));

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
        "frame_size", proc->frame_size,
        "proc_data_size", proc->proc_data_count,
        "underrun_cnt", proc->not_enough_data_cnt,
        "", "");
}

static td_void drv_dpm_proc_show_in_buf(td_void *file, const audio_ring_buf *rbuf_in)
{
    td_u32 data_size;
    td_u32 data_ratio;

    data_size = drv_audio_proc_query_rbuf_busy(rbuf_in->size, rbuf_in->read_pos, rbuf_in->write_pos);
    data_ratio = data_size * 100 / rbuf_in->size; /* 100: percent(%) */

    drv_audio_proc_print_title(file, "in buf");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "buf_addr", rbuf_in->buf_addr,
        "buf_size", rbuf_in->size,
        "read_pos", rbuf_in->read_pos,
        "write_pos", rbuf_in->write_pos);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
        "tail_size", rbuf_in->tail_size,
        "data_size", data_size,
        "data_ratio", data_ratio,
        "", "");

    /* line 3 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10s |" "\n",
        "info_addr", rbuf_in->info_addr,
        "info_num", rbuf_in->info_num,
        "info_used", rbuf_in->info_used,
        "", "");
}

static td_void drv_dpm_proc_show_out_buf(td_void *file, td_u8 buf_index, const audio_ring_buf *rbuf_out)
{
    td_u32 data_size;
    td_u32 data_ratio;

    data_size = drv_audio_proc_query_rbuf_busy(rbuf_out->size, rbuf_out->read_pos, rbuf_out->write_pos);
    data_ratio = data_size * 100 / rbuf_out->size; /* 100: percent(%) */

    drv_audio_proc_print_title(file, "out buf[%d]", buf_index);

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "buf_addr", rbuf_out->buf_addr,
        "buf_size", rbuf_out->size,
        "read_pos", rbuf_out->read_pos,
        "write_pos", rbuf_out->write_pos);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "tail_size", rbuf_out->tail_size,
        "data_size", data_size,
        "data_ratio", data_ratio,
        "overrun", rbuf_out->overrun_cnt);
}

td_s32 drv_proc_dpm_show(td_void *file, td_handle dpm, const dpm_proc_item *proc)
{
    td_u8 i;
    drv_audio_proc_print_head(file, "dpm%d", drv_audio_proc_get_mod_index(dpm));

    drv_dpm_proc_show_attr(file, proc);

    drv_dpm_proc_show_basic_status(file, dpm, proc);

    /* input */
    if (proc->rbuf_in.handle != 0) {
        drv_dpm_proc_show_in_buf(file, &(proc->rbuf_in));
    }

    /* output */
    for (i = 0; i < DPM_RBUF_MAX_NUM; i++) {
        if (proc->rbuf_out[i].handle != 0) {
            drv_dpm_proc_show_out_buf(file, i, &(proc->rbuf_out[i]));
        }
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
