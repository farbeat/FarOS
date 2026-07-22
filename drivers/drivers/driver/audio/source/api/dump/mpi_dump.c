/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: source file for mpi dump
 * Author: audio
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "cmsis_os.h"
#include "securec.h"
#include "audio_type.h"
#include "audio_debug.h"
#include "core_module.h"
#include "mpi_ai.h"
#include "mpi_sea.h"
#include "mpi_track.h"
#include "mpi_ao.h"
#include "mpi_adec.h"
#include "mpi_aenc.h"
#include "mpi_adp.h"
#include "mpi_dump.h"

#define DUMP_INST_MAX_NUM 4
#define DUMP_MOD_CNT 6

typedef td_void (*thread_callback)(td_void *arg);

typedef enum {
    DUMP_DATA_FRAME,
    DUMP_DATA_STREAM
} dump_data_type;

typedef struct {
    /* thread */
    osThreadId_t h_task;
    td_bool task_enable;
    td_bool task_exit_finish;

    /* file */
    FILE *h_file;
    td_u64 save_cur_size; /* max 16GB, need to use 64bit */
    td_u64 save_limit_size;
    td_u32 write_count;

    /* handle */
    td_handle h_adp;
    td_handle h_mod;
    td_u32 mod_out_type;
} mpi_dump_inst;

typedef struct {
    sap_mod_type mod_type;
    const td_char *task_name;
} dump_mod_name_item;

static mpi_dump_inst g_dump_inst[DUMP_INST_MAX_NUM] = {0};

static const dump_mod_name_item g_dump_mod_name_list[DUMP_MOD_CNT] = {
    {SAP_MOD_AI, "dump_ai"},
    {SAP_MOD_SEA, "dump_sea"},
    {SAP_MOD_ADEC, "dump_adec"},
    {SAP_MOD_AENC, "dump_aenc"},
    {SAP_MOD_TRACK, "dump_track"},
    {SAP_MOD_AO, "dump_ao"},
};

static td_bool g_ai_dump_flag = TD_FALSE;
static td_bool g_sea_dump_flag = TD_FALSE;
static td_bool g_aenc_dump_flag = TD_FALSE;
static td_bool g_adec_dump_flag = TD_FALSE;
static td_bool g_track_dump_flag = TD_FALSE;
static td_bool g_ao_dump_flag = TD_FALSE;

#define dump_log(fmt...)                            \
    do {                                            \
        printf("%s[%d]: ", __FUNCTION__, __LINE__); \
        printf(fmt);                                \
        printf("\n");                               \
    } while (0)

static const td_char *dump_get_task_name(td_handle h_mod)
{
    td_u32 i;
    sap_mod_type mod_type = get_sap_mod_type(h_mod);

    for (i = 0; i < DUMP_MOD_CNT; i++) {
        if (g_dump_mod_name_list[i].mod_type == mod_type) {
            return g_dump_mod_name_list[i].task_name;
        }
    }

    return "dump_xx";
}

static mpi_dump_inst *dump_find_inst(td_handle h_mod)
{
    td_u32 i;

    for (i = 0; i < sizeof(g_dump_inst) / sizeof(g_dump_inst[0]); i++) {
        if (g_dump_inst[i].h_mod == h_mod) {
            return &(g_dump_inst[i]);
        }
    }

    return TD_NULL;
}

static mpi_dump_inst *dump_get_free_inst(td_void)
{
    td_u32 i;

    for (i = 0; i < sizeof(g_dump_inst) / sizeof(g_dump_inst[0]); i++) {
        if (g_dump_inst[i].h_mod == 0x0) {
            return &(g_dump_inst[i]);
        }
    }

    return TD_NULL;
}

static td_s32 dump_attach_output(td_handle h_mod, td_u32 mod_out_type, td_handle h_adp)
{
    td_s32 ret;

    ret = mpi_adp_alloc_output_shrb(h_adp, h_mod, mod_out_type);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    switch (get_sap_mod_type(h_mod)) {
        case SAP_MOD_AI:
            ret = mpi_ai_attach_output(h_mod, h_adp);
            break;

        case SAP_MOD_SEA:
            ret = mpi_sea_attach_output(h_mod, (sea_output_type)mod_out_type, h_adp);
            break;

        case SAP_MOD_ADEC:
            ret = mpi_adec_attach_output(h_mod, h_adp);
            break;

        case SAP_MOD_AENC:
            ret = mpi_aenc_attach_output(h_mod, h_adp);
            break;

        case SAP_MOD_TRACK:
            ret = mpi_track_attach_output(h_mod, h_adp);
            break;

        case SAP_MOD_AO:
            ret = mpi_ao_attach_output(h_mod, (ao_port_id)mod_out_type, h_adp);
            break;

        default:
            return AUDIO_FAILURE;
    }

    return ret;
}

static td_s32 dump_detach_output(td_handle h_mod, td_u32 mod_out_type, td_handle h_adp)
{
    td_s32 ret;
    switch (get_sap_mod_type(h_mod)) {
        case SAP_MOD_AI:
            ret = mpi_ai_detach_output(h_mod, h_adp);
            break;

        case SAP_MOD_SEA:
            ret = mpi_sea_detach_output(h_mod, (sea_output_type)mod_out_type, h_adp);
            break;

        case SAP_MOD_ADEC:
            ret = mpi_adec_detach_output(h_mod, h_adp);
            break;

        case SAP_MOD_AENC:
            ret = mpi_aenc_detach_output(h_mod, h_adp);
            break;

        case SAP_MOD_TRACK:
            ret = mpi_track_detach_output(h_mod, h_adp);
            break;

        case SAP_MOD_AO:
            ret = mpi_ao_detach_output(h_mod, (ao_port_id)mod_out_type, h_adp);
            break;

        default:
            return AUDIO_FAILURE;
    }
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return mpi_adp_free_shrb(h_adp);
}

td_s32 mpi_dump_close(td_handle h_mod)
{
    td_s32 ret;
    mpi_dump_inst *dump_inst = dump_find_inst(h_mod);

    if (dump_inst == TD_NULL) {
        return AUDIO_SUCCESS;
    }

    if (dump_inst->task_enable == TD_TRUE) {
        dump_inst->task_enable = TD_FALSE;

        while (dump_inst->task_exit_finish == TD_FALSE) {
            osDelay(1);
        }
    }

    ret = dump_detach_output(h_mod, dump_inst->mod_out_type, dump_inst->h_adp);
    if (ret != AUDIO_SUCCESS) {
        dump_log("call dump_detach_output failed with 0x%x.", ret);
    }

    if (dump_inst->h_file != TD_NULL) {
        fflush(dump_inst->h_file);
        fclose(dump_inst->h_file);
        dump_inst->h_file = TD_NULL;
    }

    if (dump_inst->h_adp != 0x0) {
        ret = mpi_adp_close(dump_inst->h_adp);
        if (ret != AUDIO_SUCCESS) {
            dump_log("call mpi_adp_close failed with 0x%x.", ret);
        }
        dump_inst->h_adp = 0x0;
    }

    dump_inst->h_mod = 0x0;
    return AUDIO_SUCCESS;
}

static td_void get_data_info(const td_void *data, dump_data_type data_type, td_u32 *data_size, td_u8 **data_buf)
{
    if (data_type == DUMP_DATA_FRAME) {
        const audio_frame *frame = (const audio_frame *)data;
        *data_size = frame->bits_bytes;
        *data_buf = (td_u8 *)frame->bits_buffer;
    } else {
        const audio_stream_buf *stream = (const audio_stream_buf *)data;
        *data_size = stream->size;
        *data_buf = (td_u8 *)stream->data;
    }
}

static td_void dump_save_data(mpi_dump_inst *dump_inst, const td_void *data, dump_data_type data_type)
{
    size_t ret;
    td_u32 data_size;
    td_u8 *data_buf;

    get_data_info(data, data_type, &data_size, &data_buf);

    dump_inst->write_count++;
    dump_inst->save_cur_size += data_size;

    if (dump_inst->save_cur_size <= dump_inst->save_limit_size) {
        ret = fwrite((td_void *)data_buf, 1, (size_t)data_size, dump_inst->h_file);
        if (ret != (size_t)data_size) {
            dump_log("fwrite failed with ret = %zu, except len = %u, h_mod = 0x%x.\n",
                ret, data_size, dump_inst->h_mod);
        }
    } else {
        if (dump_inst->h_file != TD_NULL) {
            dump_log("save ok, write_count = %u, save_cur_size = %llu, h_mod = 0x%x.\n",
                dump_inst->write_count, dump_inst->save_cur_size, dump_inst->h_mod);
            fflush(dump_inst->h_file);
            fclose(dump_inst->h_file);
            dump_inst->h_file = TD_NULL;
        }
    }
}

static td_void dump_adp_frame_proc(td_void *args)
{
    td_s32 ret;
    audio_frame frame;
    mpi_dump_inst *dump_inst = (mpi_dump_inst *)args;

    while (dump_inst->task_enable) {
        ret = mpi_adp_acquire_frame(dump_inst->h_adp, &frame);
        if (ret != AUDIO_SUCCESS) {
            osDelay(5); /* 5ms */
            continue;
        }

        dump_save_data(dump_inst, &frame, DUMP_DATA_FRAME);

        ret = mpi_adp_release_frame(dump_inst->h_adp, &frame);
        if (ret != AUDIO_SUCCESS) {
            dump_log("call mpi_adp_release_frame failed with 0x%x.", ret);
        }
    }

    dump_inst->task_exit_finish = TD_TRUE;
}

static td_void dump_adp_stream_proc(td_void *args)
{
    td_s32 ret;
    audio_stream_buf stream;
    mpi_dump_inst *dump_inst = (mpi_dump_inst *)args;

    while (dump_inst->task_enable) {
        ret = mpi_adp_acquire_stream(dump_inst->h_adp, &stream);
        if (ret != AUDIO_SUCCESS) {
            osDelay(5); /* 5ms */
            continue;
        }

        dump_save_data(dump_inst, &stream, DUMP_DATA_STREAM);

        ret = mpi_adp_release_stream(dump_inst->h_adp, &stream);
        if (ret != AUDIO_SUCCESS) {
            dump_log("call mpi_adp_release_stream failed with 0x%x.", ret);
        }
    }

    dump_inst->task_exit_finish = TD_TRUE;
}

static thread_callback dump_get_thread_proc(td_handle h_mod)
{
    switch (get_sap_mod_type(h_mod)) {
        case SAP_MOD_AENC:
            return dump_adp_stream_proc;

        default:
            return dump_adp_frame_proc;
    }
}

td_s32 mpi_dump_open(td_handle h_mod, td_u32 mod_out_type, const td_char *file_name, td_u32 file_size)
{
    td_s32 ret;
    audio_adp_attr adp_attr = {0};
    mpi_dump_inst *dump_inst = dump_get_free_inst();
    osThreadAttr_t task_attr = {0};
    thread_callback func_call = TD_NULL;

    if (dump_inst == TD_NULL) {
        return ERR_SYS_INST_FULL;
    }

    /* destory last */
    if (dump_inst->h_mod != 0x0) {
        (td_void)mpi_dump_close(dump_inst->h_mod);
    }
    memset_s(dump_inst, sizeof(mpi_dump_inst), 0, sizeof(mpi_dump_inst));

    dump_inst->h_mod = h_mod;
    dump_inst->mod_out_type = mod_out_type;
    dump_inst->save_limit_size = file_size;

    dump_inst->h_file = fopen(file_name, "wb");
    if (dump_inst->h_file == TD_NULL) {
        dump_log("open %s failed!", file_name);
        return ERR_SYS_NOT_AVAILABLE;
    }

    /* create adp instance for mod instance output */
    ret = mpi_adp_open(&dump_inst->h_adp, ADP_TYPE_MAX, &adp_attr);
    if (ret != AUDIO_SUCCESS) {
        dump_log("call mpi_adp_open failed with 0x%x.", ret);
        (td_void)mpi_dump_close(h_mod);
        return ret;
    }

    ret = dump_attach_output(h_mod, mod_out_type, dump_inst->h_adp);
    if (ret != AUDIO_SUCCESS) {
        dump_log("call dump_attach_output failed with 0x%x.", ret);
        (td_void)mpi_dump_close(h_mod);
        return ret;
    }

    /* create task for adp instance to read data */
    dump_inst->task_enable = TD_TRUE;
    dump_inst->task_exit_finish = TD_FALSE;

    task_attr.priority = (osPriority_t)24; /* 24:priority */
    task_attr.stack_size = 0x1000;
    task_attr.name = dump_get_task_name(h_mod);
    func_call = dump_get_thread_proc(h_mod);
    dump_inst->h_task = osThreadNew(func_call, (td_void *)dump_inst, &task_attr);
    if (dump_inst->h_task == TD_NULL) {
        dump_log("call osThreadNew failed.");
        (td_void)mpi_dump_close(h_mod);
        return ERR_SYS_NOT_AVAILABLE;
    }

    dump_log("file = %s is saving.", file_name);
    dump_log("file_size = %u kB, h_mod = 0x%x, mod_output = %u, h_adp = 0x%x.",
        (td_u32)(file_size / 1024), h_mod, mod_out_type, dump_inst->h_adp); /* 1024: 1kB */

    return AUDIO_SUCCESS;
}

td_s32 mpi_dump_set_mod_mask(td_u8 dump_mask)
{
    dump_log("dump_mask = 0x%x.", dump_mask);
#ifdef SAP_AI_DUMP_SUPPORT
    g_ai_dump_flag = ((dump_mask & AUDIO_DUMP_MASK_AI) != 0) ? TD_TRUE : TD_FALSE;
#else
    if ((dump_mask & AUDIO_DUMP_MASK_AI) != 0) {
        return ERR_SYS_NOT_SUPPORTED;
    }
#endif

#ifdef SAP_SEA_DUMP_SUPPORT
    g_sea_dump_flag = ((dump_mask & AUDIO_DUMP_MASK_SEA) != 0) ? TD_TRUE : TD_FALSE;
#else
    if ((dump_mask & AUDIO_DUMP_MASK_SEA) != 0) {
        return ERR_SYS_NOT_SUPPORTED;
    }
#endif

#ifdef SAP_AENC_DUMP_SUPPORT
    g_aenc_dump_flag = ((dump_mask & AUDIO_DUMP_MASK_AENC) != 0) ? TD_TRUE : TD_FALSE;
#else
    if ((dump_mask & AUDIO_DUMP_MASK_AENC) != 0) {
        return ERR_SYS_NOT_SUPPORTED;
    }
#endif

#ifdef SAP_ADEC_DUMP_SUPPORT
    g_adec_dump_flag = ((dump_mask & AUDIO_DUMP_MASK_ADEC) != 0) ? TD_TRUE : TD_FALSE;
#else
    if ((dump_mask & AUDIO_DUMP_MASK_ADEC) != 0) {
        return ERR_SYS_NOT_SUPPORTED;
    }
#endif

#ifdef SAP_TRACK_DUMP_SUPPORT
    g_track_dump_flag = ((dump_mask & AUDIO_DUMP_MASK_TRACK) != 0) ? TD_TRUE : TD_FALSE;
#else
    if ((dump_mask & AUDIO_DUMP_MASK_TRACK) != 0) {
        return ERR_SYS_NOT_SUPPORTED;
    }
#endif

#ifdef SAP_AO_DUMP_SUPPORT
    g_ao_dump_flag = ((dump_mask & AUDIO_DUMP_MASK_AO) != 0) ? TD_TRUE : TD_FALSE;
#else
    if ((dump_mask & AUDIO_DUMP_MASK_AO) != 0) {
        return ERR_SYS_NOT_SUPPORTED;
    }
#endif

    return AUDIO_SUCCESS;
}

td_void mpi_dump_get_mod_mask(td_u8 *dump_mask)
{
    td_u8 temp = 0x0;

    temp |= (g_ai_dump_flag == TD_TRUE) ? AUDIO_DUMP_MASK_AI : 0x0;
    temp |= (g_sea_dump_flag == TD_TRUE) ? AUDIO_DUMP_MASK_SEA : 0x0;
    temp |= (g_aenc_dump_flag == TD_TRUE) ? AUDIO_DUMP_MASK_AENC : 0x0;
    temp |= (g_adec_dump_flag == TD_TRUE) ? AUDIO_DUMP_MASK_ADEC : 0x0;
    temp |= (g_track_dump_flag == TD_TRUE) ? AUDIO_DUMP_MASK_TRACK : 0x0;
    temp |= (g_ao_dump_flag == TD_TRUE) ? AUDIO_DUMP_MASK_AO : 0x0;

    *dump_mask = temp;
}

td_bool mpi_dump_get_mod_flag(td_handle h_mod)
{
    sap_mod_type mod_type = get_sap_mod_type(h_mod);
    switch (mod_type) {
        case SAP_MOD_AI:
            return g_ai_dump_flag;

        case SAP_MOD_SEA:
            return g_sea_dump_flag;

        case SAP_MOD_AENC:
            return g_aenc_dump_flag;

        case SAP_MOD_ADEC:
            return g_adec_dump_flag;

        case SAP_MOD_TRACK:
            return g_track_dump_flag;

        case SAP_MOD_AO:
            return g_ao_dump_flag;

        default:
            return TD_FALSE;
    }
}
