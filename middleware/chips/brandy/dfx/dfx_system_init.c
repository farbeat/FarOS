/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: dfx system init
 * This file should be changed only infrequently and with great care.
 */

#include <stdint.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "errcode.h"
#include "dfx_adapt_layer.h"
#include "diag.h"
#include "soc_diag_cmd_id.h"
#include "diag_cmd_connect.h"
#include "diag_cmd_filter.h"
#include "diag_cmd_password.h"
#include "diag_cmd_beat_heart.h"
#include "diag_cmd_get_mem_info.h"
#include "diag_cmd_get_task_info.h"
#include "diag_cmd_mem_read_write.h"
#include "diag_cmd_stat.h"
#include "diag_mocked_shell.h"
#include "sample_data.h"
#include "osal_task.h"
#include "osal_msgqueue.h"
#include "diag_ind_src.h"
#include "diag_filter.h"
#include "diag_msg.h"
#include "diag_dfx.h"
#ifdef SUPPORT_DIAG_V2_PROTOCOL
#include "diag_service.h"
#include "diag_cmd_dispatch.h"
#endif /* SUPPORT_DIAG_V2_PROTOCOL */
#include "diag_rom_api.h"
#include "diag_channel.h"
#include "zdiag_adapt_layer.h"
#include "zdiag_adapt_sdt.h"
#include "soc_log_uart_instance.h"
#include "sample_data_adapt.h"
#include "dfx_channel.h"
#include "diag_audio.h"
#include "transmit.h"
#include "log_file.h"
#include "soc_log_strategy.h"
#include "soc_log_impl_str.h"

#ifdef __LITEOS__
#include "los_printf_pri.h"
#endif

#ifdef CONFIG_OTA_UPDATE_SUPPORT
#include "diag_update.h"
#endif

#ifdef CONFIG_ZDIAG_NV_SUPPORT
#include "zdiag_nv.h"
#endif

#ifdef CONFIG_ZDIAG_AUDIO_PROC_SUPPORT
#include "zdiag_audio_proc.h"
#endif

#ifdef CONFIG_ZDIAG_AUDIO_DUMP_SUPPORT
#include "zdiag_audio_dump.h"
#endif

#ifdef CONFIG_ZDIAG_AUDIO_PROBE_SUPPORT
#include "zdiag_audio_probe.h"
#endif

#ifdef CONFIG_DIAG_GPU_PROC_SUPPORT
#include "diag_gpu_proc.h"
#endif

#ifdef CONFIG_DIAG_WITH_SECURE
#include "diag_secure.h"
#endif

#ifdef FT_SINGLE_UART
#ifdef SUPPORT_AT_CMD
#include "at_cmd_api.h"
#endif
#endif

#ifdef CONFIG_NV_FEATURE_SUPPORT
#include "nv.h"
#include "key_id.h"
#endif

#include "mon_log_output.h"

#define DFX_MSG_TASK_PRIO       5

const diag_cmd_reg_obj_t g_diag_default_cmd_tbl[] = {
    { DIAG_CMD_CONNECT_RANDOM, DIAG_CMD_PWD_CHANGE, diag_cmd_password },
    { DIAG_CMD_HOST_CONNECT, DIAG_CMD_HOST_DISCONNECT, diag_cmd_hso_connect_disconnect },
#ifndef SUPPORT_DIAG_V2_PROTOCOL
    { DIAG_CMD_MSG_RPT_AIR, DIAG_CMD_MSG_RPT_USR, diag_cmd_filter_set },
    { DIAG_CMD_MSG_CFG_SET_AIR, DIAG_CMD_MSG_CFG_SET_LEVEL, diag_cmd_filter_set },
#else
    { DIAG_CMD_MSG_CFG_SET_SYS, DIAG_CMD_MSG_CFG_SET_LEVEL, diag_cmd_filter_set },
#endif
    { DIAG_CMD_HEART_BEAT, DIAG_CMD_HEART_BEAT, diag_cmd_beat_heart },
    { DIAG_CMD_GET_TASK_INFO, DIAG_CMD_GET_TASK_INFO, diag_cmd_get_task_info},
    { DIAG_CMD_GET_MEM_INFO, DIAG_CMD_GET_MEM_INFO, diag_cmd_get_mem_info},
    { DIAG_CMD_MEM_MEM32, DIAG_CMD_MEM_W4, diag_cmd_mem_operate },
    { DIAG_CMD_ID_SAMPLE, DIAG_CMD_ID_SAMPLE, diag_cmd_sample_data },
    { DIAG_CMD_CHANGE_UART_CONFIG, DIAG_CMD_GET_UART_CONFIG, diag_cmd_uart_config },
    { DIAG_CMD_SET_OFFLINE_LOG, DIAG_CMD_SET_OFFLINE_LOG, diag_cmd_set_offline_log },
    { DIAG_CMD_ID_DIAG_DFX_REQ, DIAG_CMD_ID_DIAG_DFX_END, diag_dfx_cmd },
    { DIAG_CMD_ID_DBG_STAT_QUERY, DIAG_CMD_ID_DBG_STAT_QUERY, diag_cmd_stat_query },
#ifdef SUPPORT_MEDIA_AUDIO_MANAGER
    { DIAG_CMD_AUDIO_SEE_EFFECT_REQ, DIAG_CMD_AUDIO_SEE_EFFECT_REQ, diag_see_param_trans },
    { DIAG_CMD_AUDIO_SAVE_SEE_EFFECT_REQ, DIAG_CMD_AUDIO_SAVE_SEE_EFFECT_REQ, diag_see_param_save },
#endif
};

static unsigned long g_dfx_osal_queue_id = DFX_INVALID_QUEUE_ID;
static unsigned long g_dfx_transmit_queue_id = DFX_INVALID_QUEUE_ID;

static errcode_t register_default_diag_cmd(void)
{
    return uapi_diag_register_cmd(g_diag_default_cmd_tbl,
        sizeof(g_diag_default_cmd_tbl) / sizeof(g_diag_default_cmd_tbl[0]));
}

static int msg_process_proc(uint32_t msg_id, uint8_t *data, uint16_t size)
{
    zdiag_dfx_rev_msg();
    switch (msg_id) {
        case DFX_MSG_ID_DIAG_PKT:
            zdiag_dfx_rev_pkt_msg();
            diag_msg_proc((uint16_t)msg_id, data, size);
            break;
        case DFX_MSG_ID_SDT_MSG:
            zdiag_adapt_sdt_msg_dispatch(msg_id, data, size);
            break;
        case DFX_MSG_ID_BEAT_HEART:
            zdiag_dfx_rev_beat_herat_msg();
            diag_beat_heart_process();
            break;
#if CONFIG_DFX_SUPPORT_TRANSMIT_FILE == DFX_YES
        case DFX_MSG_ID_TRANSMIT_FILE:
            transmit_msg_proc(msg_id, data, size);
            break;
#endif
        default:
            break;
    }
    return ERRCODE_SUCC;
}

int msg_process_thread(void *data)
{
    uint8_t msg_data[DFX_MSG_MAX_SIZE + DFX_MSG_ID_LEN];
    uint32_t msg_data_size = (uint32_t)sizeof(msg_data);
    uint32_t msg_id;
    uint64_t start_time = 0;
    uint64_t end_time = 0;
    unused(data);

    while (true) {
        (void)memset_s(msg_data, sizeof(msg_data), 0, sizeof(msg_data));

        osal_msg_queue_read_copy(g_dfx_osal_queue_id, msg_data, &msg_data_size, OSAL_MSGQ_WAIT_FOREVER);

        diag_dfx_current_msg_num((uint16_t)osal_msg_queue_get_msg_num(g_dfx_osal_queue_id));

        msg_id = *((uint32_t*)&msg_data[0]);

        start_time = uapi_systick_get_ms();
        msg_process_proc(msg_id, &msg_data[DFX_MSG_ID_LEN], DFX_MSG_MAX_SIZE);
        end_time = uapi_systick_get_ms();

        diag_dfx_msg_process_time((uint32_t)(end_time - start_time));
    }

    return ERRCODE_SUCC;
}

errcode_t thread_msg_event_init(void)
{
    int ret;
    ret = osal_msg_queue_create("dfx_msg", DFX_QUEUE_MAX_SIZE, &g_dfx_osal_queue_id,
                                0, DFX_MSG_MAX_SIZE + DFX_MSG_ID_LEN);
    if (ret != 0) {
        return ERRCODE_FAIL;
    }

    return ERRCODE_SUCC;
}

#if (CONFIG_DFX_SUPPORT_TRANSMIT_FILE == DFX_YES && !defined(MEMORY_MINI))
int transmit_process_thread(void *data)
{
    uint8_t msg_data[DFX_MSG_MAX_SIZE + DFX_MSG_ID_LEN];
    uint32_t msg_data_size = (uint32_t)sizeof(msg_data);
    uint32_t msg_id;
    uint64_t start_time = 0;
    uint64_t end_time = 0;

    unused(data);

    while (true) {
        (void)memset_s(msg_data, sizeof(msg_data), 0, sizeof(msg_data));

        osal_msg_queue_read_copy(g_dfx_transmit_queue_id, msg_data, &msg_data_size, OSAL_MSGQ_WAIT_FOREVER);

        diag_dfx_current_transmit_msg_num((uint16_t)osal_msg_queue_get_msg_num(g_dfx_transmit_queue_id));

        msg_id = *((uint32_t*)&msg_data[0]);

        start_time = uapi_systick_get_ms();
        transmit_msg_proc(msg_id, &msg_data[DFX_MSG_ID_LEN], DFX_MSG_MAX_SIZE);
        end_time = uapi_systick_get_ms();
        diag_transmit_msg_process_time((uint32_t)(end_time - start_time));
    }

    return ERRCODE_SUCC;
}

errcode_t transmit_thread_msg_init(void)
{
    int ret;
    ret = osal_msg_queue_create("transmit", DFX_QUEUE_MAX_SIZE, &g_dfx_transmit_queue_id,
                                0, DFX_MSG_MAX_SIZE + DFX_MSG_ID_LEN);
    if (ret != 0) {
        return ERRCODE_FAIL;
    }

    return ERRCODE_SUCC;
}
#endif /* CONFIG_DFX_SUPPORT_TRANSMIT_FILE == DFX_YES */

static void cmd_shell_proc(uint8_t *data, uint32_t data_len)
{
#ifdef FT_SINGLE_UART
    if (data[0] == 'A' && data[1] == 'T') {
#ifdef SUPPORT_AT_CMD
        data[data_len++] = '\r';
        data[data_len++] = '\n';
        at_cmd_msg_receive_handler((uint8_t *)data, data_len);
#endif
    } else {
        diag_debug_cmd_proc(data, data_len);
    }
#else
    diag_debug_cmd_proc(data, data_len);
#endif
    dfx_log_debug("cmd shell: %s", data);
}

#if defined(__FREERTOS__)
int printf(const char *restrict fmt, ...)
{
    int ret;
    va_list ap;
    va_start(ap, fmt);
    if (zdiag_is_enable() == false || zdiag_mocked_shell_is_enabled() == false) {
        ret = vUartVprintk(0, fmt, ap);
    } else {
        ret = zdiag_mocked_shell_print(fmt, ap);
    }

    va_end(ap);
    return ret;
}
#endif

#if CONFIG_DFX_SUPPORT_SOC_LOG == DFX_YES
static int32_t dfx_str_out_put_callback(const uint8_t *data,  uint32_t len)
{
    int ret;
#if CONFIG_DFX_SUPPORT_DIAG_ADAPT_SOC_LOG == DFX_YES
    if (zdiag_is_enable() == false || zdiag_mocked_shell_is_enabled() == false) {
        ret = uapi_uart_write(SW_DEBUG_UART_BUS, data, len, 0);
    } else {
        ret = zdiag_mocked_shell_uart_puts(data, len);
    }
#else
    ret = uapi_uart_write(SW_DEBUG_UART_BUS, data, len, 0);
#endif
    return ret;
}
#endif

static errcode_t audio_dfx_init(void)
{
    errcode_t ret;

#ifdef CONFIG_ZDIAG_AUDIO_PROC_SUPPORT
    ret = zdiag_audio_proc_init();
    if (ret != ERRCODE_SUCC) {
        printf("zdiag_audio_proc_init failed with 0x%x.\r\n", ret);
        return ret;
    }
#endif

#ifdef CONFIG_ZDIAG_AUDIO_DUMP_SUPPORT
    ret = zdiag_audio_dump_init();
    if (ret != ERRCODE_SUCC) {
        printf("zdiag_audio_dump_init failed with 0x%x.\r\n", ret);
        return ret;
    }
#endif

#ifdef CONFIG_ZDIAG_AUDIO_PROBE_SUPPORT
    ret = zdiag_audio_probe_init();
    if (ret != ERRCODE_SUCC) {
        printf("zdiag_audio_probe_init failed with 0x%x.\r\n", ret);
        return ret;
    }
#endif

    unused(ret);

    return ERRCODE_SUCC;
}

static errcode_t zdiag_component_init(void)
{
    errcode_t ret = ERRCODE_SUCC;
#if (CONFIG_DFX_SUPPORT_DIAG_VRTTUAL_SHELL == DFX_YES)
    zdiag_mocked_shell_init();
    zdiag_mocked_shell_register_cmd_data_proc(cmd_shell_proc);
#endif /* CONFIG_DFX_SUPPORT_DIAG_VRTTUAL_SHELL */

#if CONFIG_DFX_SUPPORT_TRANSMIT_FILE == DFX_YES
    uapi_transmit_init();
#endif

#ifdef CONFIG_OTA_UPDATE_SUPPORT
    ret = zdiag_update_init();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
#endif

#ifdef CONFIG_ZDIAG_NV_SUPPORT
    ret = zdiag_nv_init();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
#endif

#ifdef CONFIG_DIAG_WITH_SECURE
    ret = diag_secure_ctx_init();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
#endif

    ret = audio_dfx_init();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

#ifdef CONFIG_DIAG_GPU_PROC_SUPPORT
    ret = diag_gpu_proc_init();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
#endif

    return ret;
}

unsigned long dfx_get_osal_queue_id(void)
{
    return g_dfx_osal_queue_id;
}

unsigned long dfx_get_transmit_queue_id(void)
{
    return g_dfx_transmit_queue_id;
}

errcode_t dfx_system_init(void)
{
    errcode_t ret;
    diag_rom_api_t rom_api;

    uapi_diag_service_init();
#if CONFIG_DFX_SUPPORT_OFFLINE_LOG_FILE == DFX_YES
    uapi_logfile_init();
#endif
    ret = register_default_diag_cmd();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    zdiag_filter_init();

#ifndef FORBID_AUTO_LOG_REPORT
    diag_auto_log_report_enable();
#else
#if CONFIG_DFX_SUPPORT_DIAG_BEAT_HEART == DFX_YES
    ret = diag_beat_heart_init();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
#endif
#endif

    ret = diag_register_channel();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    rom_api.report_sys_msg = uapi_zdiag_report_sys_msg_instance;
    diag_rom_api_register(&rom_api);

#if CONFIG_DFX_SUPPORT_SOC_LOG == DFX_YES
    dfx_write_data_interface_t impl;
    (void)dfx_serial_get_write_data_impl(&impl);
    (void)soc_log_register_write_impl(&impl);
    dfx_register_soc_log_output_handler(dfx_str_out_put_callback);
#endif

    ret = zdiag_component_init();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    diag_highest_level_proc(DIAG_LOG_LEVEL_WARN);
#ifdef USER_VERSION
    mon_log_output_ctrl_init();
    mon_log_output_enable(true);
#endif
    return ERRCODE_SUCC;
}

void dfx_set_log_enable(void)
{
#ifdef CONFIG_NV_FEATURE_SUPPORT
    uint8_t flag = 0;
    uint16_t length;
    errcode_t ret = uapi_nv_read(NV_ID_OFFLINELOG_ENBALE_FLAG, sizeof(flag), &length, &flag);
    if (ret != ERRCODE_SUCC) {
        return;
    }
    if (flag != 0) {
        uapi_zdiag_set_offline_log_enable(true);
    } else {
        uapi_zdiag_set_offline_log_enable(false);
    }
#endif
}
