/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: logging interface
 * Author:
 * Create:
 */
#include "connectivity_log.h"
#include "log_common.h"
#include "ipc.h"
#include "securec.h"
#include "non_os.h"

static system_config_t *g_system_config_p = (system_config_t *)(uintptr_t)(SYSTEM_CFG_REGION_START);

#if CORE == APPS
static bool g_log_level_inited = false;
static log_level_config_t g_log_level_config = {
    LOG_LEVEL_APP_DEFAULT_CONFIG,
    LOG_LEVEL_BT_DEFAULT_CONFIG,
#if CORE_NUMS > 2
    LOG_LEVEL_DSP_DEFAULT_CONFIG,
#endif
#if CHIP_LIBRA
    LOG_LEVEL_GNSS_DEFAULT_CONFIG,
    LOG_LEVEL_SEC_DEFAULT_CONFIG,
#endif
};

static log_level_e get_log_level_from_sharemem(uint8_t data_index)
{
    if (g_system_config_p == NULL) {
        return (log_level_e)DEFAULT_LOG_LEVEL;
    }

    uint32_t mgc_log_lvl = *((uint32_t *)g_system_config_p + data_index);
    if ((get_log_level_magic(mgc_log_lvl) != LOG_LEVEL_MAGIC) ||
        (get_real_log_level(mgc_log_lvl) >= LOG_LEVEL_MAX) ||
        (get_real_log_level(mgc_log_lvl) == LOG_LEVEL_NONE)) {
        /* Set the log level in sharemem when chip power on */
        mgc_log_lvl = set_log_level_magic(*((uint32_t *)(&g_log_level_config) + data_index));
        *((uint32_t *)g_system_config_p + data_index) = mgc_log_lvl;
    }
    return (log_level_e)get_real_log_level(mgc_log_lvl);
}

/* Init the log level system */
void init_log_level(void)
{
    if (g_log_level_inited) {
        return;
    }

    /* Point to the share memory and clear the area */
    g_system_config_p = (system_config_t *)(uintptr_t)(SYSTEM_CFG_REGION_START);  //lint !e834
    log_level_e log_lvl = (log_level_e)DEFAULT_LOG_LEVEL;

    log_level_e temp_lvl;
    for (uint8_t i = 0; i < (sizeof(log_level_config_t) / sizeof(uint32_t)); i++) {
        temp_lvl = get_log_level_from_sharemem(i);
        *((uint32_t *)(&g_log_level_config) + i) = temp_lvl;
        if (i == (offsetof(log_level_config_t, app_core_level) / sizeof(uint32_t))) {
            log_lvl = temp_lvl;
        }
    }

    g_log_level_inited = true;

    log_set_local_log_level(log_lvl);
}

/* Inform another core to set the log level. */
#ifdef SUPPORT_IPC
static void inform_set_log_level(cores_t core, log_level_e level)
{
    ipc_status_t ipc_returned_value;
    ipc_payload_set_log_level ipc_log_level;

    ipc_log_level.log_level = (uint32_t)level;

    ipc_returned_value = ipc_spin_send_message_timeout(core,
                                                       IPC_ACTION_SET_LOG_LEVEL,
                                                       (ipc_payload *)&ipc_log_level,
                                                       sizeof(ipc_payload_set_log_level),
                                                       IPC_PRIORITY_LOWEST, false, IPC_SPIN_SEND_DEFAULT_TIMEOUT);
    if (ipc_returned_value != IPC_STATUS_OK) {
        UNUSED(ipc_returned_value);
    }
}
#endif

ret_type_e set_log_mode(cores_t core, log_mode_t mode)
{
#if CORE_NUMS > 2
    if (core == CORES_PROTOCOL_CORE) {
        g_system_config_p->log_mode.dsp_core_mode = (uint32_t)mode;
        return RET_TYPE_OK;
    } else {
        return RET_TYPE_ERROR;
    }
#else
    UNUSED(core);
    UNUSED(mode);
    return RET_TYPE_ERROR;
#endif
}

ret_type_e set_log_level(cores_t core, log_level_e level)
{
    /* If the log level isn't inited, init first to set the default log level into the shared memory */
    if (!g_log_level_inited) {
        init_log_level();
    }

    if (level >= LOG_LEVEL_MAX) {
        level = LOG_LEVEL_DBG;
    }

    /* Record the log level if the current core is application core */
    non_os_enter_critical();
    switch (core) {
        case CORES_APPS_CORE:
            g_log_level_config.app_core_level = (uint32_t)level;
            g_system_config_p->log_level.app_core_level = set_log_level_magic((uint32_t)level);
            break;

        case CORES_BT_CORE:
            g_log_level_config.bt_core_level = (uint32_t)level;
            g_system_config_p->log_level.bt_core_level = set_log_level_magic((uint32_t)level);
            break;
#if CORE_NUMS > 2
        case CORES_PROTOCOL_CORE:
            g_log_level_config.dsp_core_level = (uint32_t)level;
            g_system_config_p->log_level.dsp_core_level = set_log_level_magic((uint32_t)level);
            break;
#endif
#if CHIP_LIBRA
        case CORES_GNSS_CORE:
            g_log_level_config.gnss_core_level = (uint32_t)level;
            g_system_config_p->log_level.gnss_core_level = set_log_level_magic((uint32_t)level);
            break;
        case CORES_SEC_CORE:
            g_log_level_config.sec_core_level = (uint32_t)level;
            g_system_config_p->log_level.sec_core_level = set_log_level_magic((uint32_t)level);
            break;
#endif
        default:
            non_os_exit_critical();
            return RET_TYPE_ERROR;
    }
    non_os_exit_critical();
    /* Set the log level of low layer */
    if (core == CORES_APPS_CORE) {
        log_set_local_log_level(level);
    } else {
#ifdef SUPPORT_IPC
        inform_set_log_level(core, level);
#endif
    }

    return RET_TYPE_OK;
}
#endif

ret_type_e get_log_level(cores_t core, log_level_e *level)
{
    if (level == NULL) {
        return RET_TYPE_ERROR;
    }

#if CORE == APPS
    if (!g_log_level_inited) {
        init_log_level();
    }

    if (core == CORES_APPS_CORE) {
        *level = (log_level_e)g_log_level_config.app_core_level;
    } else if (core == CORES_BT_CORE) {
        *level = (log_level_e)g_log_level_config.bt_core_level;
    }
#if CORE_NUMS > 2
    else if (core == CORES_PROTOCOL_CORE) {
        *level = (log_level_e)g_log_level_config.dsp_core_level;
    }
#endif
    else {
#if CHIP_LIBRA
        if (core == CORES_GNSS_CORE) {
            *level = (log_level_e)g_log_level_config.gnss_core_level;
        } else if (core == CORES_SEC_CORE) {
            *level = (log_level_e)g_log_level_config.sec_core_level;
        }
#endif
    }
#else
    UNUSED(core);
    UNUSED(level);
    *level = log_get_local_log_level();
#endif

    return RET_TYPE_OK;
}

ret_type_e get_log_mode(cores_t core, log_mode_t* const mode)
{
#if CORE_NUMS > 2
    if (mode == NULL) {
        return RET_TYPE_ERROR;
    }

    if (core == CORES_PROTOCOL_CORE) {
        *mode = (log_mode_t)g_system_config_p->log_mode.dsp_core_mode;
        return RET_TYPE_OK;
    } else {
        return RET_TYPE_ERROR;
    }
#else
    UNUSED(core);
    UNUSED(mode);
    return RET_TYPE_ERROR;
#endif
}

#ifdef SUPPORT_IPC
ret_type_e send_bt_hook_data(ipc_bt_ap_hook_type_e hook_type, const uint8_t *command, uint32_t length)
{
    errno_t sec_ret;
    ipc_status_t ipc_returned_value;
    ipc_payload_ap_bt_hook_data *hook = (ipc_payload_ap_bt_hook_data *)malloc(sizeof(ipc_payload_ap_bt_hook_data));
    if (hook == NULL) {
        return RET_TYPE_MEMORY_ERROR;
    }

    hook->hook_type = hook_type;
    sec_ret = memcpy_s(hook->command, MAX_AP_BT_HOOK_DATA_LEN, command, length);
    if (sec_ret != EOK) {
        free(hook);
        return RET_TYPE_MEMORY_ERROR;
    }
    hook->command_len = length;

    ipc_returned_value = ipc_spin_send_message_timeout(CORES_BT_CORE,
                                                       IPC_ACTION_AP_BT_HOOK_DATA,
                                                       (ipc_payload *)hook,
                                                       sizeof(ipc_payload_ap_bt_hook_data),
                                                       IPC_PRIORITY_LOWEST, false, IPC_SPIN_SEND_DEFAULT_TIMEOUT);
    if (ipc_returned_value != IPC_STATUS_OK) {
        free(hook);
        return (ret_type_e)ipc_returned_value;
    }

    free(hook);
    return RET_TYPE_OK;
}
#endif

void log_set_sharemem_timestamp(uint64_t rtc_time_ms)
{
    non_os_enter_critical();
    g_system_config_p->log_time = rtc_time_ms;
    non_os_exit_critical();
}

uint64_t log_get_sharemem_timestamp(void)
{
    return g_system_config_p->log_time;
}

void set_slave_cpu_power_mode(slave_cpu_t slave_cpu, slave_cpu_power_mode_t slave_mode)
{
    if ((slave_cpu >= SLAVE_CPU_MAX_NUM) || (slave_mode >= SLAVE_CPU_POWER_MAX_NUM)) {
        return;
    }
    non_os_enter_critical();

#if CHIP_LIBRA
    if (slave_cpu == SLAVE_CPU_DSP0) {
        g_system_config_p->dsp0_power_flag = (uint32_t)slave_mode;
    } else if (slave_cpu == SLAVE_CPU_BT) {
        g_system_config_p->bt_power_flag = (uint32_t)slave_mode;
    } else if (slave_cpu == SLAVE_CPU_GNSS) {
        g_system_config_p->gnss_power_flag = (uint32_t)slave_mode;
    } else if (slave_cpu == SLAVE_CPU_SEC) {
        g_system_config_p->sec_power_flag = (uint32_t)slave_mode;
    }
#elif DSP_EXIST == NO
    if (slave_cpu == SLAVE_CPU_BT) {
        g_system_config_p->bt_power_flag = (uint32_t)slave_mode;
    }
#else
    if (slave_cpu == SLAVE_CPU_DSP0) {
        g_system_config_p->dsp0_power_flag = (uint32_t)slave_mode;
#if DUAL_DSP_EXIST == YES
    } else if (slave_cpu == SLAVE_CPU_DSP1) {
        g_system_config_p->dsp1_power_flag = (uint32_t)slave_mode;
#endif
    } else {
        g_system_config_p->bt_power_flag = (uint32_t)slave_mode;
    }
#endif
    non_os_exit_critical();
}

slave_cpu_power_mode_t get_slave_cpu_power_mode(slave_cpu_t slave_cpu)
{
    if (slave_cpu >= SLAVE_CPU_MAX_NUM) {
        return SLAVE_CPU_POWER_MAX_NUM;
    }
#if CHIP_LIBRA
    if (slave_cpu == SLAVE_CPU_DSP0) {
        return (slave_cpu_power_mode_t)g_system_config_p->dsp0_power_flag;
    } else if (slave_cpu == SLAVE_CPU_GNSS) {
        return (slave_cpu_power_mode_t)g_system_config_p->gnss_power_flag;
    } else if (slave_cpu == SLAVE_CPU_BT) {
        return (slave_cpu_power_mode_t)g_system_config_p->bt_power_flag;
    } else {
        return (slave_cpu_power_mode_t)g_system_config_p->sec_power_flag;
    }
#elif DSP_EXIST == NO
    return (slave_cpu_power_mode_t)g_system_config_p->bt_power_flag;
#else
    if (slave_cpu == SLAVE_CPU_DSP0) {
        return (slave_cpu_power_mode_t)g_system_config_p->dsp0_power_flag;
#if DUAL_DSP_EXIST == YES
    } else if (slave_cpu == SLAVE_CPU_DSP1) {
        return (slave_cpu_power_mode_t)g_system_config_p->dsp1_power_flag;
#endif
    } else {
        return (slave_cpu_power_mode_t)g_system_config_p->bt_power_flag;
    }
#endif
}
