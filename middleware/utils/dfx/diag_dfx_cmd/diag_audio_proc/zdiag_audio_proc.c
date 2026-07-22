/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: audio proc by zdiag
 */

#include <stdio.h>
#include <string.h>
#include "errcode.h"
#include "diag.h"
#include "soc_diag_cmd_id.h"
#include "zdiag_audio_proc.h"

/* Fix no define warning */
int32_t ai_read_proc(void *file, void *private);
int32_t sea_read_proc(void *file, void *private);
int32_t adec_read_proc(void *file, void *private);
int32_t aenc_read_proc(void *file, void *private);
int32_t ao_read_proc(void *file, void *private);
int32_t ab_read_proc(void *file, void *private);
int32_t sys_read_proc(void *file, void *private);

typedef int32_t (*audio_proc_exec_function)(void *file, void *private);

typedef struct {
    uint16_t cmd_id;
    const char *name;
    audio_proc_exec_function exec_func;
} proc_mod_item;

#ifdef CONFIG_ZDIAG_AUDIO_PROC_SUPPORT
static const proc_mod_item g_proc_mod_list[] = {
    {DIAG_CMD_AUDIO_PROC_AI, "ai", ai_read_proc},
    {DIAG_CMD_AUDIO_PROC_SEA, "sea", sea_read_proc},
    {DIAG_CMD_AUDIO_PROC_ADEC, "adec", adec_read_proc},
    {DIAG_CMD_AUDIO_PROC_AENC, "aenc", aenc_read_proc},
    {DIAG_CMD_AUDIO_PROC_AO, "ao", ao_read_proc},
    {DIAG_CMD_AUDIO_PROC_AB, "ab", ab_read_proc},
    {DIAG_CMD_AUDIO_PROC_SYS, "sys", sys_read_proc},
};

static int32_t run_mod_proc(uint16_t cmd_id)
{
    uint32_t i;
    for (i = 0; i < (sizeof(g_proc_mod_list) / sizeof(g_proc_mod_list[0])); i++) {
        if (cmd_id == g_proc_mod_list[i].cmd_id) {
            g_proc_mod_list[i].exec_func(NULL, NULL);
            return ERRCODE_SUCC;
        }
    }

    printf("%s,%d:run_mod_proc fail, cmd_id = 0x%x.\n", __FUNCTION__, __LINE__, cmd_id);
    return ERRCODE_FAIL;
}

STATIC errcode_t diag_cmd_audio_proc(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    unused(cmd_param);
    unused(cmd_param_size);
    errcode_t ret = ERRCODE_SUCC;

    /* ack first */
    ret = uapi_diag_report_packet(cmd_id, option, (uint8_t *)&ret, sizeof(ret), true);
    if (ret != ERRCODE_SUCC) {
        printf("%s,%d:uapi_diag_report_packet error = 0x%x.\r\n", __FUNCTION__, __LINE__, ret);
        return ret;
    }

    /* then print proc to mocked shell */
    return (errcode_t)run_mod_proc(cmd_id);
}

diag_cmd_reg_obj_t g_diag_audio_proc_cmd_tbl[] = {
    {DIAG_CMD_AUDIO_PROC_AI, DIAG_CMD_AUDIO_PROC_SYS, diag_cmd_audio_proc},
};

errcode_t zdiag_audio_proc_init(void)
{
    return uapi_diag_register_cmd(g_diag_audio_proc_cmd_tbl,
        sizeof(g_diag_audio_proc_cmd_tbl) / sizeof(g_diag_audio_proc_cmd_tbl[0]));
}
#endif
