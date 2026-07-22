/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: source file for drv sys proc
 * Author: audio
 */

#include "drv_proc_sys.h"
#include "audio_debug.h"
#include "audio_osal.h"
#include "drv_audio_proc.h"
#include "drv_audio_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef SAP_PROC_SUPPORT
static td_void drv_sys_proc_show_dump_cfg(td_void *file, const audio_debug_cfg *cfg)
{
#ifdef SAP_DUMP_SUPPORT
    td_bool dump_support = TD_TRUE;
#else
    td_bool dump_support = TD_FALSE;
#endif

    drv_audio_proc_print_title(file, "dump mask");

    drv_audio_osal_proc_print(file,
        "%-5s: %-8s |" "%-5s: %-4s |" "%-5s: %-8s |" "%-5s: %-4s |"
        "%-5s: %-8s |" "%-5s: %-4s |" "%-5s: %-8s |" "%-5s: %-4s |"  "\n",
        "dump", drv_audio_proc_get_bool(dump_support),
        "ai", drv_audio_proc_get_bool(check_option_valid(cfg->dump_mask, AUDIO_DUMP_MASK_AI)),
        "sea", drv_audio_proc_get_bool(check_option_valid(cfg->dump_mask, AUDIO_DUMP_MASK_SEA)),
        "aenc", drv_audio_proc_get_bool(check_option_valid(cfg->dump_mask, AUDIO_DUMP_MASK_AENC)),
        "adec", drv_audio_proc_get_bool(check_option_valid(cfg->dump_mask, AUDIO_DUMP_MASK_ADEC)),
        "track", drv_audio_proc_get_bool(check_option_valid(cfg->dump_mask, AUDIO_DUMP_MASK_TRACK)),
        "ao", drv_audio_proc_get_bool(check_option_valid(cfg->dump_mask, AUDIO_DUMP_MASK_AO)),
        "dpm", drv_audio_proc_get_bool(check_option_valid(cfg->dump_mask, AUDIO_DUMP_MASK_DPM)));
}

static td_void drv_sys_proc_show_proc_cfg(td_void *file, const audio_debug_cfg *cfg)
{
#ifdef SAP_PROC_SUPPORT
    td_bool proc_support = TD_TRUE;
#else
    td_bool proc_support = TD_FALSE;
#endif

    drv_audio_proc_print_title(file, "proc mask");

    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "proc_support", drv_audio_proc_get_bool(proc_support),
        "output_uart", drv_audio_proc_get_bool(check_option_valid(cfg->proc_mask, AUDIO_PROC_MASK_UART)),
        "output_diag", drv_audio_proc_get_bool(check_option_valid(cfg->proc_mask, AUDIO_PROC_MASK_DIAG_SHELL)),
        "", "");
}

static td_void drv_sys_proc_show_sync_status(td_void *file, const sys_proc_item *proc)
{
    drv_audio_proc_print_title(file, "sync_status");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "irq_count", proc->irq_count,
        "", "",
        "", "",
        "", "");

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "cur_tog_bias", proc->tog_bias.cur_tog_bias,
        "max_tog_bias", proc->tog_bias.max_tog_bias,
        "min_tog_bias", proc->tog_bias.min_tog_bias,
        "avg_tog_bias", proc->tog_bias.avg_tog_bias);
}

static td_void drv_sys_proc_show_profile_summary(td_void *file, const sys_proc_item *proc)
{
    drv_audio_proc_print_title(file, "profile summary");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "total(kcps)", proc->total_kcps,
        "idle(kcps)", proc->idle_kcps,
        "msg(kcps)", proc->msg_kcps,
        "driver(kcps)", proc->driver_kcps);
}

static td_void drv_sys_proc_show_profile_detail(td_void *file, const sys_proc_item *proc)
{
    td_u32 i;
    const mcps_block *item = TD_NULL;

    drv_audio_proc_print_title(file, "profile detail");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-10s  " "%-10s  " "%-10s  " "\n",
        "handle", "avg(kcps)", "peak(kcps)");

    for (i = 0; i < sizeof(proc->mcps_list) / sizeof(proc->mcps_list[0]); i++) {
        item = &(proc->mcps_list[i]);
        if (item->mod_id == 0) {
            continue;
        }
        drv_audio_osal_proc_print(file,
            "0x%08x  " "%-10d  " "%-10d  " "\n",
            item->mod_id,
            item->ave,
            item->peak);
    }
}

static td_void drv_sys_proc_show_memory_summary(td_void *file, const sys_proc_item *proc)
{
    drv_audio_proc_print_title(file, "memory summary");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "share_mem_total", proc->share_mem_total,
        "share_mem_used", proc->share_mem_used,
        "mem_total", proc->mem_total,
        "mem_used", proc->mem_used);

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: 0x%08x |" "%-16s: %-10d |" "%-16s: %-10d |" "%-16s: %-10d |" "\n",
        "heap_ptr", proc->heap_ptr,
        "heap_cached", proc->heap_cached,
        "malloc_used", proc->malloc_used,
        "malloc_cnt", proc->malloc_cnt);
}

static td_void drv_sys_proc_show_share_mem_detail(td_void *file, const sys_proc_item *proc)
{
    td_u32 i;
    const share_mem_block *item = TD_NULL;

    drv_audio_proc_print_title(file, "share mem detail");

    drv_audio_osal_proc_print(file,
        "%-10s  " "%-10s  " "%-10s  " "\n",
        "buf_addr", "data_size", "info_size");

    for (i = 0; i < sizeof(proc->share_mem_list) / sizeof(proc->share_mem_list[0]); i++) {
        item = &(proc->share_mem_list[i]);
        if (item->addr == 0) {
            continue;
        }

        drv_audio_osal_proc_print(file,
            "0x%08x  " "%-10d  " "%-10d  " "\n",
            item->addr, item->data_size, item->info_size);
    }
}

static td_void drv_sys_proc_show_static_segment_detail(td_void *file, const sys_proc_item *proc)
{
    td_u32 i;
    const segment_block *item = TD_NULL;

    drv_audio_proc_print_title(file, "static segment detail");

    drv_audio_osal_proc_print(file,
        "%-10s  " "%-10s  " "%-10s  " "%-10s  " "\n",
        "name", "size", "used", "free");

    for (i = 0; i < sizeof(proc->static_list) / sizeof(proc->static_list[0]); i++) {
        item = &(proc->static_list[i]);
        if (item->size == 0) {
            continue;
        }
        drv_audio_osal_proc_print(file,
            "%-10s  " "%-10d  " "%-10d  " "%-10d  " "\n",
            item->name, item->size, item->used, item->size - item->used);
    }
}

static td_void drv_sys_proc_show_dynamic_segment_detail(td_void *file, const sys_proc_item *proc)
{
    td_u32 i;
    const segment_block *item = TD_NULL;

    drv_audio_proc_print_title(file, "dynamic segment detail");

    drv_audio_osal_proc_print(file,
        "%-10s  " "%-10s  " "%-10s  " "%-10s  " "\n",
        "name", "size", "used", "free");

    for (i = 0; i < sizeof(proc->dynamic_list) / sizeof(proc->dynamic_list[0]); i++) {
        item = &(proc->dynamic_list[i]);
        if (item->size == 0) {
            continue;
        }
        drv_audio_osal_proc_print(file,
            "%-10s  " "%-10d  " "%-10d  " "%-10d  " "\n",
            item->name, item->size, item->used, item->size - item->used);
    }
}

static td_void drv_sys_proc_show_malloc_detail(td_void *file, const sys_proc_item *proc)
{
    td_u32 i;
    const malloc_block *item = TD_NULL;

    drv_audio_proc_print_title(file, "malloc detail");

    drv_audio_osal_proc_print(file,
        "%-10s  " "%-10s  " "%-10s  " "\n",
        "start", "end", "size");

    for (i = 0; i < sizeof(proc->malloc_list) / sizeof(proc->malloc_list[0]); i++) {
        item = &(proc->malloc_list[i]);
        if (item->size == 0) {
            continue;
        }
        drv_audio_osal_proc_print(file,
            "0x%08x  " "0x%08x  " "%-10d  " "\n",
            item->start_addr, item->start_addr + item->size, item->size);
    }
}

td_s32 drv_proc_sys_show(td_void *file, const sys_proc_item *proc)
{
    drv_audio_proc_print_head(file, "sys");

    /* debug */
    drv_sys_proc_show_dump_cfg(file, &proc->debug_cfg);
    drv_sys_proc_show_proc_cfg(file, &proc->debug_cfg);

    /* sync_status */
    drv_sys_proc_show_sync_status(file, proc);

    /* summary */
    drv_sys_proc_show_profile_summary(file, proc);
    drv_sys_proc_show_memory_summary(file, proc);

    /* profile */
    drv_sys_proc_show_profile_detail(file, proc);

    /* memory */
    drv_sys_proc_show_share_mem_detail(file, proc);
    drv_sys_proc_show_static_segment_detail(file, proc);
    drv_sys_proc_show_dynamic_segment_detail(file, proc);
    drv_sys_proc_show_malloc_detail(file, proc);

    drv_audio_proc_print_tail(file);

    return AUDIO_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
