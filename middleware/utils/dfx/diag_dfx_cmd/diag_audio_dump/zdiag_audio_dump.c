/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: audio dump by zdiag
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cmsis_os.h"
#include "errcode.h"
#include "securec.h"
#include "diag.h"
#include "soc_diag_cmd_id.h"
#include "soc_uapi_ai.h"
#include "soc_uapi_sea.h"
#include "soc_uapi_aenc.h"
#include "soc_uapi_adec.h"
#include "soc_uapi_sound.h"
#include "soc_uapi_adp.h"
#include "zdiag_audio_dump.h"

#define DUMP_INST_NUM_MAX 3

#define MAX_PRINT_SIZE 256

#define FILE_PATH_LEN 256
#define DUMP_ROOT_PATH "/user"

#define BYTES_IN_1KB 1024
#define FILE_SIZE_KB_MIN 1 /* 1kB */
#define FILE_SIZE_KB_MAX (128 * BYTES_IN_1KB) /* 128MB */

typedef void (*thread_callback)(void *arg);

#define diag_dump_log(fmt...)                       \
    do {                                            \
        printf("%s[%d]: ", __FUNCTION__, __LINE__); \
        printf(fmt);                                \
        printf("\n");                               \
    } while (0)

#define diag_dump_report(fmt...)                               \
    do {                                                       \
        dump_report_mocked_shell(__FUNCTION__, __LINE__, fmt); \
    } while (0)

typedef enum {
    DUMP_DATA_FRAME,
    DUMP_DATA_STREAM,
    DUMP_DATA_MAX
} dump_data_type;

typedef enum {
    DUMP_MOD_AI,
    DUMP_MOD_SEA,
    DUMP_MOD_ADEC,
    DUMP_MOD_AENC,
    DUMP_MOD_TRACK,
    DUMP_MOD_AO,
    DUMP_MOD_MAX
} dump_mod_type;

/* sync from sap_mod_type */
typedef enum {
    AUDIO_MOD_AI = 0x03,
    AUDIO_MOD_SEA = 0x05,
    AUDIO_MOD_ADEC = 0x06,
    AUDIO_MOD_AENC = 0x07,
    AUDIO_MOD_TRACK = 0x08,
    AUDIO_MOD_AO = 0x09,
    AUDIO_MOD_MAX
} audio_sap_type;

typedef struct {
    dump_mod_type mod_type;
    audio_sap_type sap_type;
    const char *mod_name;
    const char *suffix;
    const char *task_name;
} dump_mod_info_item;

typedef struct {
    /* thread */
    osThreadId_t task_id;
    bool task_enable;

    /* file */
    FILE *h_file;
    char file_path[FILE_PATH_LEN];
    uint64_t save_cur_size; /* max 16GB, need to use 64bit */
    uint64_t save_limit_size;
    bool save_over;

    /* handle */
    uint32_t h_mod;
    uint32_t mod_out_type;
    dump_mod_type mod_type;
    uint32_t h_adp;
} audio_dump_inst;

#ifdef CONFIG_ZDIAG_AUDIO_DUMP_SUPPORT
static const dump_mod_info_item g_dump_mod_info_list[] = {
    {DUMP_MOD_AI, AUDIO_MOD_AI, "ai", "pcm", "dump_ai"},
    {DUMP_MOD_SEA, AUDIO_MOD_SEA, "sea", "pcm", "dump_sea"},
    {DUMP_MOD_ADEC, AUDIO_MOD_ADEC, "adec", "pcm", "dump_adec"},
    {DUMP_MOD_AENC, AUDIO_MOD_AENC, "aenc", "stream", "dump_aenc"},
    {DUMP_MOD_TRACK, AUDIO_MOD_TRACK, "track", "pcm", "dump_track"},
    {DUMP_MOD_AO, AUDIO_MOD_AO, "ao", "pcm", "dump_ao"},
};

static audio_dump_inst g_audio_dump_inst[DUMP_INST_NUM_MAX] = {0};

static void dump_report_mocked_shell(const char *fn_name, uint32_t line_num, const char *format, ...)
{
    int32_t ret;
    va_list args;
    char buf[MAX_PRINT_SIZE + 1] = {'\0'}; /* add '\n' */
    char *p = buf;
    uint32_t msg_len;

    if (fn_name == NULL) {
        return;
    }

    /* head */
    ret = snprintf_s(buf, MAX_PRINT_SIZE, MAX_PRINT_SIZE - 1,
        "[%s:%u] ", fn_name, line_num);
    if (ret < 0) {
        diag_dump_log("head snprintf_s failed, ret = 0x%x.", ret);
        return;
    }
    msg_len = (uint32_t)ret;

    /* log */
    va_start(args, format);
    ret = vsnprintf_s(buf + msg_len, MAX_PRINT_SIZE - msg_len,
        MAX_PRINT_SIZE - msg_len - 1, format, args);
    va_end(args);
    if (ret < 0) {
        diag_dump_log("log snprintf_s failed, ret = 0x%x.", ret);
        return;
    }
    msg_len += (uint32_t)ret;
    buf[msg_len] = '\n';

    /* send to mocked shell */
    ret = (int32_t)uapi_diag_report_packet(
        DIAG_CMD_MOCKED_SHELL_IND, NULL, (const uint8_t *)buf, (uint16_t)msg_len, true);
    if (ret != ERRCODE_SUCC) {
        diag_dump_log("uapi_diag_report_packet failed, ret = 0x%x.", ret);
        return;
    }
}

static int32_t get_dump_mod_type(uint32_t handle, dump_mod_type *type)
{
    uint32_t i;
    uint32_t sap_type = ((uint32_t)handle & 0xFF); /* get sap type */
    for (i = 0; i < (sizeof(g_dump_mod_info_list) / sizeof(g_dump_mod_info_list[0])); i++) {
        if (sap_type == g_dump_mod_info_list[i].sap_type) {
            *type = g_dump_mod_info_list[i].mod_type;
            return ERRCODE_SUCC;
        }
    }
    return ERRCODE_FAIL;
}

static int32_t get_dump_file_name(dump_mod_type mod_type, uint32_t h_mod, uint32_t out_type, char *file_path)
{
    uint32_t i;
    for (i = 0; i < (sizeof(g_dump_mod_info_list) / sizeof(g_dump_mod_info_list[0])); i++) {
        if (mod_type == g_dump_mod_info_list[i].mod_type) {
            int32_t ret = snprintf_s(file_path, FILE_PATH_LEN - 1, FILE_PATH_LEN - 1, "%s/dump_%s_0x%x_0x%x.%s",
                DUMP_ROOT_PATH, g_dump_mod_info_list[i].mod_name, h_mod, out_type, g_dump_mod_info_list[i].suffix);
            if (ret < 0) {
                diag_dump_log("snprintf_s failed, ret = 0x%x.", ret);
                return ERRCODE_FAIL;
            }
            return ERRCODE_SUCC;
        }
    }
    return ERRCODE_FAIL;
}

static const char *get_dump_task_name(dump_mod_type mod_type)
{
    uint32_t i;
    for (i = 0; i < (sizeof(g_dump_mod_info_list) / sizeof(g_dump_mod_info_list[0])); i++) {
        if (mod_type == g_dump_mod_info_list[i].mod_type) {
            return g_dump_mod_info_list[i].task_name;
        }
    }
    return "dump_xx";
}

static const char *get_dump_mod_name(dump_mod_type mod_type)
{
    uint32_t i;
    for (i = 0; i < (sizeof(g_dump_mod_info_list) / sizeof(g_dump_mod_info_list[0])); i++) {
        if (mod_type == g_dump_mod_info_list[i].mod_type) {
            return g_dump_mod_info_list[i].mod_name;
        }
    }
    return "mod_xx";
}

static audio_dump_inst *dump_get_free_inst(void)
{
    uint32_t i;
    for (i = 0; i < sizeof(g_audio_dump_inst) / sizeof(g_audio_dump_inst[0]); i++) {
        if (g_audio_dump_inst[i].h_mod == 0x0) {
            return &(g_audio_dump_inst[i]);
        }
    }
    diag_dump_log("no free inst.");
    return NULL;
}

static int32_t check_file_size(uint32_t dump_file_size)
{
    if (dump_file_size < FILE_SIZE_KB_MIN || dump_file_size > FILE_SIZE_KB_MAX) {
        diag_dump_report("invalid file_size = %d kB, shoule be [%d, %d].",
            dump_file_size, FILE_SIZE_KB_MIN, FILE_SIZE_KB_MAX);
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

static int32_t sea_check_output_type(uapi_sea_output_type sea_type)
{
    if (sea_type >= UAPI_SEA_OUTPUT_MAX) {
        diag_dump_report("invalid sea_type = %u.", sea_type);
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

static int32_t snd_check_output_port(uapi_snd_out_port ao_port)
{
    if (!(ao_port >= UAPI_SND_OUT_PORT_DAC0 && ao_port <= UAPI_SND_OUT_PORT_DAC3) &&
        !(ao_port >= UAPI_SND_OUT_PORT_I2S0 && ao_port <= UAPI_SND_OUT_PORT_I2S4) &&
        !(ao_port == UAPI_SND_OUT_PORT_SPDIF0) &&
        !(ao_port >= UAPI_SND_OUT_PORT_HDMI0 && ao_port <= UAPI_SND_OUT_PORT_HDMI1) &&
        !(ao_port >= UAPI_SND_OUT_PORT_ARC0 && ao_port <= UAPI_SND_OUT_PORT_ARC1) &&
        !(ao_port >= UAPI_SND_OUT_PORT_USB0 && ao_port <= UAPI_SND_OUT_PORT_USB1) &&
        !(ao_port >= UAPI_SND_OUT_PORT_BT0 && ao_port <= UAPI_SND_OUT_PORT_BT1) &&
        !(ao_port >= UAPI_SND_OUT_PORT_CAST0 && ao_port <= UAPI_SND_OUT_PORT_CAST1)) {
        diag_dump_report("invalid ao_port = %u.", ao_port);
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

static int32_t check_mod_output_type(uint32_t h_mod, uint32_t output_type)
{
    int32_t ret;
    dump_mod_type mod_type;

    ret = get_dump_mod_type(h_mod, &mod_type);
    if (ret != ERRCODE_SUCC) {
        diag_dump_report("get_dump_mod_type failed, ret = 0x%x.", ret);
        return ret;
    }

    switch (mod_type) {
        case DUMP_MOD_SEA: {
            uapi_sea_output_type sea_type = (uapi_sea_output_type)output_type;
            ret = sea_check_output_type(sea_type);
            break;
        }

        case DUMP_MOD_AO: {
            uapi_snd_out_port ao_port = (uapi_snd_out_port)output_type;
            ret = snd_check_output_port(ao_port);
            break;
        }

        default:
            ret = ERRCODE_SUCC;
            break;
    }

    return ret;
}

static int32_t dump_mod_attach(audio_dump_inst *dump_inst)
{
    int32_t ret;

    switch (dump_inst->mod_type) {
        case DUMP_MOD_AI:
            ret = uapi_ai_attach_output(dump_inst->h_mod, dump_inst->h_adp);
            break;

        case DUMP_MOD_SEA: {
            uapi_sea_output_type type = (uapi_sea_output_type)dump_inst->mod_out_type;
            ret = uapi_sea_attach_output(dump_inst->h_mod, type, dump_inst->h_adp);
            break;
        }

        case DUMP_MOD_ADEC:
            ret = uapi_adec_attach_output(dump_inst->h_mod, dump_inst->h_adp);
            break;

        case DUMP_MOD_AENC:
            ret = uapi_aenc_attach_output(dump_inst->h_mod, dump_inst->h_adp);
            break;

        case DUMP_MOD_TRACK:
            ret = uapi_snd_attach_track_output(dump_inst->h_mod, dump_inst->h_adp);
            break;

        case DUMP_MOD_AO: {
            uapi_snd_out_port port = (uapi_snd_out_port)dump_inst->mod_out_type;
            ret = uapi_snd_attach_output(dump_inst->h_mod, port, dump_inst->h_adp);
            break;
        }

        default:
            return ERRCODE_FAIL;
    }

    return ret;
}

static int32_t dump_mod_detach(audio_dump_inst *dump_inst)
{
    int32_t ret;

    switch (dump_inst->mod_type) {
        case DUMP_MOD_AI:
            ret = uapi_ai_detach_output(dump_inst->h_mod, dump_inst->h_adp);
            break;

        case DUMP_MOD_SEA: {
            uapi_sea_output_type type = (uapi_sea_output_type)dump_inst->mod_out_type;
            ret = uapi_sea_detach_output(dump_inst->h_mod, type, dump_inst->h_adp);
            break;
        }

        case DUMP_MOD_ADEC:
            ret = uapi_adec_detach_output(dump_inst->h_mod, dump_inst->h_adp);
            break;

        case DUMP_MOD_AENC:
            ret = uapi_aenc_detach_output(dump_inst->h_mod, dump_inst->h_adp);
            break;

        case DUMP_MOD_TRACK:
            ret = uapi_snd_detach_track_output(dump_inst->h_mod, dump_inst->h_adp);
            break;

        case DUMP_MOD_AO: {
            uapi_snd_out_port port = (uapi_snd_out_port)dump_inst->mod_out_type;
            ret = uapi_snd_detach_output(dump_inst->h_mod, port, dump_inst->h_adp);
            break;
        }

        default:
            return ERRCODE_FAIL;
    }

    return ret;
}

static void dump_destory_thread(audio_dump_inst *dump_inst)
{
    if (dump_inst->task_enable == true) {
        dump_inst->task_enable = false;
    }

    if (dump_inst->task_id != NULL) {
        osThreadTerminate(dump_inst->task_id);
        dump_inst->task_id = NULL;
    }
}

static void dump_release_inst(audio_dump_inst *dump_inst)
{
    int32_t ret;

    ret = dump_mod_detach(dump_inst);
    if (ret != ERRCODE_SUCC) {
        diag_dump_log("dump_mod_detach failed, ret = 0x%x.", ret);
    }

    if (dump_inst->h_file != NULL) {
        fflush(dump_inst->h_file);
        fclose(dump_inst->h_file);
        dump_inst->h_file = NULL;
    }

    if (dump_inst->h_adp != 0) {
        uapi_adp_destroy(dump_inst->h_adp);
        dump_inst->h_adp = 0;
    }

    dump_inst->h_mod = 0x0;
}

static int32_t dump_sys_init(void)
{
    int32_t ret;

    ret = uapi_adp_init();
    if (ret != ERRCODE_SUCC) {
        diag_dump_log("uapi_adp_init failed, ret = 0x%x.", ret);
        return ret;
    }
    return ERRCODE_SUCC;
}

static void dump_sys_deinit(void)
{
    int32_t ret;

    ret = uapi_adp_deinit();
    if (ret != ERRCODE_SUCC) {
        diag_dump_log("uapi_adp_deinit failed, ret = 0x%x.", ret);
    }
}

static void get_data_info(void *data, dump_data_type data_type, uint32_t *data_size, uint8_t **data_buf)
{
    if (data_type == DUMP_DATA_FRAME) {
        uapi_audio_frame *frame = (uapi_audio_frame *)data;
        *data_size = frame->bits_bytes;
        *data_buf = (uint8_t *)frame->bits_buffer;
    } else {
        uapi_stream_buf *stream = (uapi_stream_buf *)data;
        *data_size = stream->size;
        *data_buf = (uint8_t *)stream->data;
    }
}

static void dump_save_data(audio_dump_inst *dump_inst, void *data, dump_data_type data_type)
{
    int32_t ret;
    uint32_t data_size;
    uint32_t write_size;
    uint8_t *data_buf = NULL;

    get_data_info(data, data_type, &data_size, &data_buf);

    dump_inst->save_cur_size += data_size;

    if (dump_inst->save_cur_size <= dump_inst->save_limit_size) {
        if (dump_inst->h_file != NULL) {
            write_size = fwrite((void *)data_buf, 1, data_size, dump_inst->h_file);
            if (write_size != data_size) {
                diag_dump_log("fwrite failed with write size = %u, except len = %u.", write_size, data_size);
            }
        } else {
            diag_dump_log("h_file is null.");
        }
    }

    if ((dump_inst->save_cur_size >= dump_inst->save_limit_size) && (dump_inst->save_over == false)) {
        dump_inst->save_over = true;
        diag_dump_report("file saved ok, file path: %s.", dump_inst->file_path);
    }
}

static void dump_adp_frame_proc(void *args)
{
    int32_t ret;
    uapi_audio_frame frame;
    audio_dump_inst *dump_inst = (audio_dump_inst *)args;

    while (dump_inst->task_enable) {
        ret = uapi_adp_acquire_frame(dump_inst->h_adp, &frame);
        if (ret != ERRCODE_SUCC) {
            osDelay(5); /* 5ms */
            continue;
        }

        dump_save_data(dump_inst, &frame, DUMP_DATA_FRAME);

        ret = uapi_adp_release_frame(dump_inst->h_adp, &frame);
        if (ret != ERRCODE_SUCC) {
            diag_dump_log("uapi_adp_release_frame failed, ret = 0x%x.", ret);
        }

        if (dump_inst->save_over == true) {
            break;
        }
    }

    /* dump finish */
    dump_release_inst(dump_inst);
    dump_sys_deinit();
}

static void dump_adp_stream_proc(void *args)
{
    int32_t ret;
    uapi_stream_buf stream;
    audio_dump_inst *dump_inst = (audio_dump_inst *)args;

    while (dump_inst->task_enable) {
        ret = uapi_adp_acquire_stream(dump_inst->h_adp, &stream);
        if (ret != ERRCODE_SUCC) {
            osDelay(5); /* 5ms */
            continue;
        }

        dump_save_data(dump_inst, &stream, DUMP_DATA_STREAM);

        ret = uapi_adp_release_stream(dump_inst->h_adp, &stream);
        if (ret != ERRCODE_SUCC) {
            diag_dump_log("uapi_adp_release_stream failed, ret = 0x%x.", ret);
        }

        if (dump_inst->save_over == true) {
            break;
        }
    }

    /* dump finish */
    dump_release_inst(dump_inst);
    dump_sys_deinit();
}

static thread_callback dump_get_thread_proc(dump_mod_type mod_type)
{
    switch (mod_type) {
        case DUMP_MOD_AENC:
            return dump_adp_stream_proc;

        default:
            return dump_adp_frame_proc;
    }
}

static void dump_close_inst(audio_dump_inst *dump_inst)
{
    dump_destory_thread(dump_inst);
    dump_release_inst(dump_inst);
}

static int32_t dump_open_inst(audio_dump_inst *dump_inst)
{
    int32_t ret;
    uapi_adp_attr adp_attr;
    osThreadAttr_t task_attr = {0};
    thread_callback func_call = NULL;

    dump_inst->h_file = fopen(dump_inst->file_path, "wb");
    if (dump_inst->h_file == NULL) {
        diag_dump_log("open %s failed.", dump_inst->file_path);
        return ERRCODE_FAIL;
    }

    /* create adp for mod output */
    uapi_adp_get_def_attr(&adp_attr);
    ret = uapi_adp_create(&dump_inst->h_adp, &adp_attr);
    if (ret != ERRCODE_SUCC) {
        diag_dump_log("uapi_adp_create failed, ret = 0x%x.", ret);
        goto fail;
    }

    /* attach adp to mod */
    ret = dump_mod_attach(dump_inst);
    if (ret != ERRCODE_SUCC) {
        diag_dump_log("dump_mod_attach failed, ret = 0x%x.", ret);
        goto fail;
    }

    func_call = dump_get_thread_proc(dump_inst->mod_type);

    /* create task for save data */
    dump_inst->task_enable = true;
    task_attr.priority = 24; /* 24: normal priority */
    task_attr.stack_size = 0x1000; /* 4k */
    task_attr.name = get_dump_task_name(dump_inst->mod_type);
    dump_inst->task_id = osThreadNew(func_call, (void *)dump_inst, &task_attr);
    if (dump_inst->task_id == NULL) {
        diag_dump_log("osThreadNew failed.");
        ret = ERRCODE_FAIL;
        goto fail;
    }

    diag_dump_report("file is saving."
        "file_path = %s, file_size = %d kB, mod_name = %s, mod_handle = 0x%x, mod_output = %u.",
        dump_inst->file_path, (uint32_t)(dump_inst->save_limit_size / BYTES_IN_1KB),
        get_dump_mod_name(dump_inst->mod_type), dump_inst->h_mod, dump_inst->mod_out_type);

    return ERRCODE_SUCC;

fail:
    dump_close_inst(dump_inst);
    return ret;
}

static int32_t dump_check_param(uint32_t file_size, uint32_t h_mod, uint32_t mod_out_type)
{
    int32_t ret;
    dump_mod_type mod_type;

    /* file size */
    ret = check_file_size(file_size);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    /* handle */
    if (h_mod == 0x0) {
        diag_dump_log("h_mod is null.");
        return ERRCODE_FAIL;
    }
    if (get_dump_mod_type(h_mod, &mod_type) != ERRCODE_SUCC) {
        diag_dump_log("get_dump_mod_type fail, h_mod = 0x%x.", h_mod);
        return ERRCODE_FAIL;
    }

    /* module output type */
    if (check_mod_output_type(h_mod, mod_out_type) != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }

    return ERRCODE_SUCC;
}

static int32_t audio_dump_entry(uint32_t file_size, uint32_t h_mod, uint32_t mod_out_type)
{
    int32_t ret;
    audio_dump_inst *dump_inst = NULL;

    if (dump_check_param(file_size, h_mod, mod_out_type) != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }

    dump_inst = dump_get_free_inst();
    if (dump_inst == NULL) {
        return ERRCODE_FAIL;
    }
    /* destory last thread */
    dump_destory_thread(dump_inst);

    ret = dump_sys_init();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    memset_s(dump_inst, sizeof(*dump_inst), 0, sizeof(*dump_inst));

    dump_inst->h_mod = h_mod;
    dump_inst->mod_out_type = mod_out_type;
    dump_inst->save_limit_size = (uint64_t)(file_size * BYTES_IN_1KB);
    ret = get_dump_mod_type(h_mod, &dump_inst->mod_type);
    if (ret != ERRCODE_SUCC) {
        goto sys_deinit;
    }
    ret = get_dump_file_name(dump_inst->mod_type, h_mod, mod_out_type, dump_inst->file_path);
    if (ret != ERRCODE_SUCC) {
        diag_dump_report("get_dump_file_name failed, ret = 0x%x.", ret);
        goto sys_deinit;
    }

    ret = dump_open_inst(dump_inst);
    if (ret != ERRCODE_SUCC) {
        goto sys_deinit;
    }

    return ERRCODE_SUCC;

sys_deinit:
    dump_inst->h_mod = 0x0;
    dump_sys_deinit();

    return ret;
}

STATIC errcode_t diag_cmd_audio_dump(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    unused(cmd_param_size);
    errcode_t ret;
    audio_dump_cmd_item *dump_cmd = (audio_dump_cmd_item *)cmd_param;

    /* start dump */
    ret = (errcode_t)audio_dump_entry(dump_cmd->file_size, dump_cmd->mod_handle, dump_cmd->mod_output);
    if (ret != ERRCODE_SUCC) {
        diag_dump_log("audio_dump_entry failed, ret = 0x%x.", ret);
    }

    /* ack */
    return uapi_diag_report_packet(cmd_id, option, (uint8_t *)&ret, sizeof(ret), true);
}

diag_cmd_reg_obj_t g_diag_audio_dump_cmd_tbl[] = {
    {DIAG_CMD_AUDIO_DUMP, DIAG_CMD_AUDIO_DUMP, diag_cmd_audio_dump},
};

errcode_t zdiag_audio_dump_init(void)
{
    return uapi_diag_register_cmd(g_diag_audio_dump_cmd_tbl,
        sizeof(g_diag_audio_dump_cmd_tbl) / sizeof(g_diag_audio_dump_cmd_tbl[0]));
}
#endif
