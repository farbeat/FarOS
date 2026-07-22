/*
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: source file for drv ab proc
 * Author: audio
 */

#include "drv_proc_ab.h"
#include "audio_struct.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "drv_audio_proc.h"
#include "drv_audio_osal.h"
#include "core_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef SAP_PROC_SUPPORT

typedef struct {
    sap_mod_type mod_type;
    const td_char *name;
} ab_module_proc_name_item;

typedef struct {
    audio_product_form mod_type;
    const td_char *name;
} ab_module_product_form_item;

typedef struct {
    audio_tws_mode mod_type;
    const td_char *name;
} ab_module_tws_mode_item;

typedef struct {
    audio_tws_role mod_type;
    const td_char *name;
} ab_module_tws_role_item;

typedef struct {
    audio_clock_type clock_type;
    const td_char *name;
} ab_module_clock_type_item;

static const ab_module_proc_name_item g_ab_module_proc_name_list[] = {
    {SAP_MOD_NULL, "null"},
    {SAP_MOD_ADP, "adp"},
    {SAP_MOD_PORT, "port"},
    {SAP_MOD_AI, "ai"},
    {SAP_MOD_SEA, "sea"},
    {SAP_MOD_ADEC, "adec"},
    {SAP_MOD_AENC, "aenc"},
    {SAP_MOD_TRACK, "track"},
    {SAP_MOD_AO, "ao"},
    {SAP_MOD_ANC, "anc"},
    {SAP_MOD_MSG, "msg"},
    {SAP_MOD_SYS, "sys"},
    {SAP_MOD_DPM, "dpm"},
};

static const ab_module_product_form_item g_ab_module_product_form_list[] = {
    {AUDIO_PRODUCT_FORM_TWS_LEFT, "left"},
    {AUDIO_PRODUCT_FORM_TWS_RIGHT, "right"},
    {AUDIO_PRODUCT_FORM_NON_TWS, "none_tws"},
};

static const ab_module_tws_mode_item g_ab_module_tws_mode_list[] = {
    {AUDIO_TWS_MODE_NON_TWS, "none_tws"},
    {AUDIO_TWS_MODE_FORWARD, "forward"},
    {AUDIO_TWS_MODE_LISTEN, "listen"},
    {AUDIO_TWS_MODE_DUAL_TRANSMIT, "dual"},
};

static const ab_module_tws_role_item g_ab_module_tws_role_list[] = {
    {AUDIO_TWS_ROLE_MASTER, "master"},
    {AUDIO_TWS_ROLE_SLAVE, "slave"},
    {AUDIO_TWS_ROLE_FREEMAN, "freeman"},
};

static const ab_module_clock_type_item g_ab_module_clock_type_list[] = {
    {AUDIO_CLOCK_XO, "xo"},
    {AUDIO_CLOCK_PLL, "pll"},
};

static const td_char *get_ab_module_name(sap_mod_type mod_type)
{
    td_u32 i;
    for (i = 0; i < (sizeof(g_ab_module_proc_name_list) / sizeof(g_ab_module_proc_name_list[0])); i++) {
        if (mod_type == g_ab_module_proc_name_list[i].mod_type) {
            return g_ab_module_proc_name_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_tws_product_form_name(audio_product_form mod_type)
{
    td_u32 i;
    for (i = 0; i < (sizeof(g_ab_module_product_form_list) / sizeof(g_ab_module_product_form_list[0])); i++) {
        if (mod_type == g_ab_module_product_form_list[i].mod_type) {
            return g_ab_module_product_form_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_tws_mode_name(audio_tws_mode mod_type)
{
    td_u32 i;
    for (i = 0; i < (sizeof(g_ab_module_tws_mode_list) / sizeof(g_ab_module_tws_mode_list[0])); i++) {
        if (mod_type == g_ab_module_tws_mode_list[i].mod_type) {
            return g_ab_module_tws_mode_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_tws_role_name(audio_tws_role mod_type)
{
    td_u32 i;
    for (i = 0; i < (sizeof(g_ab_module_tws_role_list) / sizeof(g_ab_module_tws_role_list[0])); i++) {
        if (mod_type == g_ab_module_tws_role_list[i].mod_type) {
            return g_ab_module_tws_role_list[i].name;
        }
    }
    return "unknown";
}

static const td_char *get_clock_type_name(audio_clock_type clock_type)
{
    td_u32 i;
    for (i = 0; i < (sizeof(g_ab_module_clock_type_list) / sizeof(g_ab_module_clock_type_list[0])); i++) {
        if (clock_type == g_ab_module_clock_type_list[i].clock_type) {
            return g_ab_module_clock_type_list[i].name;
        }
    }
    return "unknown";
}

static td_void drv_ab_proc_show_attach_status(td_void *file, const ab_proc_item *proc)
{
    td_u32 i;
    const ab_attach_item *item = TD_NULL;
    drv_audio_proc_print_title(file, "attach status");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-5s  " "%-10s  " "%-7s  " "%-6s  " "%-9s  " "%-11s  " "%-8s  " "%-7s  " "\n",
        "index", "src_handle", "src_mod", "src_id", "data_type", "dest_handle", "dest_mod", "dest_id");

    /* array */
    for (i = 0; i < (sizeof(proc->attach_list) / sizeof(proc->attach_list[0])); i++) {
        item = &(proc->attach_list[i]);
        if (item->src_handle == 0 || item->dest_handle == 0) {
            continue;
        }
        drv_audio_osal_proc_print(file,
            "%-5d  " "0x%08x  " "%-7s  " "%-6d  " "%-9d  " "0x%08x   " "%-8s  " "%-7d  " "\n",
            i, item->src_handle, get_ab_module_name(item->src_mod), item->src_id,
            item->data_type, item->dest_handle, get_ab_module_name(item->dest_mod), item->dest_id);
    }
}

static td_void drv_ab_proc_show_base_info(td_void *file, const ab_proc_item *proc)
{
    drv_audio_proc_print_title(file, "base info");

    /* line 1 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10d |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "dsp_version", proc->version_info.dsp_version,
        "dsp_freq(MHz)", proc->status_info.clock_attr.clock_freq / 1000000, /* 1000000: MHz to Hz */
        "dsp_clk", get_clock_type_name(proc->status_info.clock_attr.clock_type),
        "", "");

    /* line 2 */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "build_day", proc->version_info.day,
        "build_year", proc->version_info.year,
        "build_time", proc->version_info.time,
        "", "");
}

static td_void drv_ab_proc_show_tws_info(td_void *file, const ab_proc_item *proc)
{
    const ab_tws_info *tws_info = &proc->tws_info;

    /* 当产品为非TWS时，不打印TWS的相关属性 */
    if (tws_info->product_form == AUDIO_PRODUCT_FORM_NON_TWS) {
        return;
    }

    /* show tws info */
    drv_audio_osal_proc_print(file,
        "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "%-16s: %-10s |" "\n",
        "product_form", get_tws_product_form_name(tws_info->product_form),
        "tws_mono_mode", drv_audio_proc_get_bool(tws_info->tws_mono_mode),
        "tws_mode", get_tws_mode_name(tws_info->tws_mode),
        "tws_role", get_tws_role_name(tws_info->tws_role));
}

td_s32 drv_proc_ab_show(td_void *file, const ab_proc_item *proc)
{
    drv_audio_proc_print_head(file, "ab");

    /* base info */
    drv_ab_proc_show_base_info(file, proc);

    /* tws info */
    drv_ab_proc_show_tws_info(file, proc);

    /* attach status */
    drv_ab_proc_show_attach_status(file, proc);

    drv_audio_proc_print_tail(file);

    return AUDIO_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
