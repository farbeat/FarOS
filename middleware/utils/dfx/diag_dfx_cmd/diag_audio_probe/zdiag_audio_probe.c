/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: audio probe by zdiag
 */

#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "errcode.h"
#include "securec.h"
#include "diag.h"
#include "soc_diag_cmd_id.h"
#include "zdiag_audio_probe.h"

typedef enum {
    AUDIO_SYS_DEEP_STANDBY = 0,
    AUDIO_SYS_ACTIVE,
    AUDIO_SYS_ACTIVE_STANDBY,
    AUDIO_SYS_STATE_MAX
} audio_sys_state;

typedef enum {
    AUDIO_STEP_SYSTEM_INIT = 0,
    AUDIO_STEP_CMD_INIT,

    AUDIO_STEP_CMD_PROCESS,
    AUDIO_STEP_MSG_PROCESS,
    AUDIO_STEP_DRIVER_PROCESS,
    AUDIO_STEP_SYSTEM_IDLE,

    AUDIO_STEP_CMD_DEINIT,
    AUDIO_STEP_SYSTEM_DEINIT,

    AUDIO_STEP_MAX,
} audio_run_step;

typedef struct {
    /* run status */
    uint32_t heartbeat;
    audio_run_step run_step;

    /* sys status */
    audio_sys_state sys_state;
    uint32_t sys_active;
    uint32_t sys_active_standby;
    uint32_t sys_deep_standby;

    /* log status */
    uint32_t log_success;
    uint32_t log_busy;
    uint32_t log_total_len;
    uint32_t log_cur_len;

    /* msg status */
    uint32_t msg_success;
    uint32_t msg_illegal_param;
    uint32_t msg_ack_timeout;
    uint32_t msg_underflow;
    uint32_t msg_crc_fail;

    /* msg int status */
    uint32_t msg_int_fail;
    uint32_t msg_int_retrigger;
} probe_status_info;

typedef struct {
    audio_sys_state state;
    const char *name;
} probe_sys_state_item;

typedef struct {
    audio_run_step step;
    const char *name;
} probe_run_step_item;

#define diag_probe_log(fmt...)                      \
    do {                                            \
        printf("%s[%d]: ", __FUNCTION__, __LINE__); \
        printf(fmt);                                \
        printf("\n");                               \
    } while (0)

#define DIAG_PROBE_REPORT probe_report_mocked_shell

#define probe_reg_read(addr, val)  ((val) = *(volatile uint32_t *)(uintptr_t)(uint32_t)(addr))

#define MAX_PRINT_SIZE 256

#define DSP_CORE0 0
#define DSP_CORE1 1
#define DSP_CORE_MAX 2

#define AUDIO_PROBE_STATUS_ADDR 0x87000008
#define PROBE_REG_WIDTH 4

#ifdef CONFIG_ZDIAG_AUDIO_PROBE_SUPPORT
static const probe_sys_state_item g_sys_state_info_list[] = {
    {AUDIO_SYS_DEEP_STANDBY, "deep_standby"},
    {AUDIO_SYS_ACTIVE, "active"},
    {AUDIO_SYS_ACTIVE_STANDBY, "active_standby"},
};

static const probe_run_step_item g_run_step_info_list[] = {
    {AUDIO_STEP_SYSTEM_INIT, "sys_init"},
    {AUDIO_STEP_CMD_INIT, "cmd_init"},

    {AUDIO_STEP_CMD_PROCESS, "cmd_proc"},
    {AUDIO_STEP_MSG_PROCESS, "msg_proc"},
    {AUDIO_STEP_DRIVER_PROCESS, "driver_proc"},
    {AUDIO_STEP_SYSTEM_IDLE, "sys_idle"},

    {AUDIO_STEP_CMD_DEINIT, "cmd_deinit"},
    {AUDIO_STEP_SYSTEM_DEINIT, "sys_deinit"},
};

static probe_status_info g_probe_list[DSP_CORE_MAX] = {0};

static void probe_report_mocked_shell(const char *format, ...)
{
    int32_t ret;
    va_list args;
    char buf[MAX_PRINT_SIZE + 1] = {'\0'}; /* add '\n' */
    char *p = buf;
    uint32_t msg_len;

    /* log */
    va_start(args, format);
    ret = vsnprintf_s(buf, MAX_PRINT_SIZE, MAX_PRINT_SIZE - 1, format, args);
    va_end(args);
    if (ret < 0) {
        diag_probe_log("log snprintf_s failed, ret = 0x%x.", ret);
        return;
    }
    msg_len = (uint32_t)ret;
    buf[msg_len] = '\n';

    /* send to mocked shell */
    ret = (int32_t)uapi_diag_report_packet(
        DIAG_CMD_MOCKED_SHELL_IND, NULL, (const uint8_t *)buf, (uint16_t)msg_len, true);
    if (ret != ERRCODE_SUCC) {
        diag_probe_log("uapi_diag_report_packet failed, ret = 0x%x.", ret);
        return;
    }
}

static const char *get_sys_state_name(audio_sys_state state)
{
    uint32_t i;
    for (i = 0; i < (sizeof(g_sys_state_info_list) / sizeof(g_sys_state_info_list[0])); i++) {
        if (state == g_sys_state_info_list[i].state) {
            return g_sys_state_info_list[i].name;
        }
    }
    return "null";
}

static const char *get_run_step_name(audio_run_step step)
{
    uint32_t i;
    for (i = 0; i < (sizeof(g_run_step_info_list) / sizeof(g_run_step_info_list[0])); i++) {
        if (step == g_run_step_info_list[i].step) {
            return g_run_step_info_list[i].name;
        }
    }
    return "null";
}

static void audio_probe_read(void)
{
    uint32_t i;
    uint32_t *ptr = (uint32_t *)g_probe_list;

    for (i = 0; i < (sizeof(g_probe_list) / PROBE_REG_WIDTH); i++) {
        probe_reg_read(AUDIO_PROBE_STATUS_ADDR + i * PROBE_REG_WIDTH, *ptr);
        ptr++;
    }
}

static void core_probe_output(uint32_t core_id, const probe_status_info *status)
{
    DIAG_PROBE_REPORT("\n[DSP] core_id = %d.\n", core_id);

    /* run status */
    DIAG_PROBE_REPORT("heartbeat = %u, run_step = %s.\n",
        status->heartbeat, get_run_step_name(status->run_step));

    /* sys status */
    DIAG_PROBE_REPORT("sys_state = %s, active = %u, active_standby = %u, deep_standby = %u.\n",
        get_sys_state_name(status->sys_state), status->sys_active,
        status->sys_active_standby, status->sys_deep_standby);

    /* log status */
    DIAG_PROBE_REPORT("log_success = %u, busy = %u, total_len = %u, cur_len = %u.\n",
        status->log_success, status->log_busy, status->log_total_len, status->log_cur_len);

    /* msg status */
    DIAG_PROBE_REPORT(
        "msg_success = %u, illegal_param = %u, ack_timeout = %u, underflow = %u, crc_fail = %u.\n",
        status->msg_success, status->msg_illegal_param, status->msg_ack_timeout,
        status->msg_underflow, status->msg_crc_fail);

    /* msg int status */
    DIAG_PROBE_REPORT("msg_int_fail = %u, msg_int_retrigger = %u.\n", status->msg_int_fail, status->msg_int_retrigger);
}

static void audio_probe_output(void)
{
    core_probe_output(DSP_CORE0, &(g_probe_list[0]));

#ifdef CONFIG_AUDIO_SUPPORT_DUAL_DSP
    core_probe_output(DSP_CORE1, &(g_probe_list[1]));
#endif
}

STATIC errcode_t diag_cmd_audio_probe(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    unused(cmd_param);
    unused(cmd_param_size);
    errcode_t ret = ERRCODE_SUCC;

    diag_probe_log("sizeof(probe_status_info) = %u.\r\n", sizeof(probe_status_info));

    /* read probe */
    audio_probe_read();

    /* then print probe to mocked shell */
    audio_probe_output();

    /* ack first */
    ret = uapi_diag_report_packet(cmd_id, option, (uint8_t *)&ret, sizeof(ret), true);
    if (ret != ERRCODE_SUCC) {
        diag_probe_log("uapi_diag_report_packet error = 0x%x.\r\n", ret);
        return ret;
    }

    return ERRCODE_SUCC;
}

diag_cmd_reg_obj_t g_diag_audio_probe_cmd_tbl[] = {
    {DIAG_CMD_AUDIO_PROBE, DIAG_CMD_AUDIO_PROBE, diag_cmd_audio_probe},
};

errcode_t zdiag_audio_probe_init(void)
{
    return uapi_diag_register_cmd(g_diag_audio_probe_cmd_tbl,
        sizeof(g_diag_audio_probe_cmd_tbl) / sizeof(g_diag_audio_probe_cmd_tbl[0]));
}
#endif
