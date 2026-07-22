/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: los status
 * Author:
 * Create:
 */
#include "los_status.h"
#ifndef BUILD_APPLICATION_SSB
#if USE_RPC_MODE == YES
#include "rpc_auto_generated_application.h"
#endif
#endif
#include "systick.h"
#include "non_os.h"
#include "securec.h"
#include "pmu_interrupt.h"
#include "tcxo.h"
#ifndef BUILD_APPLICATION_SSB
#include "log_common.h"
#include "log_def.h"

static uint32_t g_enter_core_deep_sleep_time = 0;
static uint32_t g_enter_core_light_sleep_time = 0;
static uint64_t g_los_system_sleep_base_time = 0;
static uint64_t g_los_system_start_up_base_time = 0;
static uint32_t g_wakeup_source[WAKEUP_SOURCE_MAX] = {0};
static uint64_t g_los_system_work_base_time = 0;
static uint32_t g_enter_core_work_time = 0;
static uint32_t g_los_system_total_run_time = 0;

#define WAKEUP_SOURCE_COUNT MCPU_DSP_GPIO_INT_WAKEUP
#define WAIT_JLINK_CONNECT_TIME 300

void los_system_start_up_init(void)
{
    g_los_system_start_up_base_time = uapi_systick_get_ms();
    g_los_system_work_base_time = g_los_system_start_up_base_time;
}

uint64_t los_system_system_run_time(void)
{
    return (uapi_systick_get_ms() - g_los_system_start_up_base_time);
}

uint32_t los_m_core_deep_sleep_time(void)
{
    return g_enter_core_deep_sleep_time;
}

uint32_t los_m_core_light_sleep_time(void)
{
    return g_enter_core_light_sleep_time;
}

void los_status_enter_sleep_time(void)
{
    g_los_system_sleep_base_time = uapi_systick_get_ms();
    g_enter_core_work_time += (uint32_t)(uapi_systick_get_ms() - g_los_system_work_base_time);
}

void los_status_exit_sleep_time(lpc_sleep_mode_e sleep_mode)
{
    uint16_t event_id;
    g_los_system_work_base_time = uapi_systick_get_ms();
    if (sleep_mode == LPC_SLEEP_MODE_LIGHT) {
        g_enter_core_light_sleep_time += (uint32_t)(uapi_systick_get_ms() - g_los_system_sleep_base_time);
    } else {
        event_id = pmu_lpm_wakeup_get_all_status(LPM_EVT_ACTION);
        los_status_wakeup_source_statistics(event_id);
        g_enter_core_deep_sleep_time += (uint32_t)(uapi_systick_get_ms() - g_los_system_sleep_base_time);
    }
    g_los_system_total_run_time = (uint32_t)(uapi_systick_get_ms() - g_los_system_start_up_base_time);
#if !defined(BUILD_APPLICATION_STANDARD)
    oml_pf_log_print4(LOG_BCORE_PLT_DRIVER_SYSSTATUS, LOG_NUM_DRIVER_SYSSTATUS, LOG_LEVEL_INFO,
                      "[App] Time statistics: runtime(%d ms), work(%d ms), deep_sleep(%d ms), light_sleep(%d ms)",
                      g_los_system_total_run_time, g_enter_core_work_time, g_enter_core_deep_sleep_time,
                      g_enter_core_light_sleep_time);
#else
    UNUSED(g_los_system_total_run_time);
    UNUSED(g_enter_core_work_time);
#endif
}

void los_status_sleep_time_clear(void)
{
    non_os_enter_critical();
    g_enter_core_light_sleep_time = 0;
    g_enter_core_deep_sleep_time = 0;
    g_los_system_start_up_base_time = uapi_systick_get_ms();
    non_os_exit_critical();
}

#if USE_RPC_MODE == YES
bool los_get_system_sleep_count(uint32_t *system_sleep_time, uint32_t *bt_core_sleep_time)
{
    bool ret_code = false;
    if (command_get_system_sleep_count(&ret_code, system_sleep_time, bt_core_sleep_time) == RPC_ERR_OK) {
        return true;
    }

    *system_sleep_time = 0;
    *bt_core_sleep_time = 0;
    return false;
}
#endif

void los_status_wakeup_source_statistics(uint16_t interrupt_id)
{
    uint8_t bit_count;
    for (bit_count = 0; bit_count <= WAKEUP_SOURCE_COUNT; bit_count++) {
        if ((interrupt_id & BIT(bit_count)) != 0) {
            g_wakeup_source[bit_count]++;
            break;
        }
    }
    if ((pmu_lpm_audio_wakeup_get_status(LPM_EVT_ACTION)) != 0) {
        g_wakeup_source[MCPU_AUDIO_WAKEUP]++;
    }

    if ((interrupt_id & BIT(MCPU_DAP_WAKEUP)) != 0) {
        uapi_tcxo_delay_ms((uint64_t)WAIT_JLINK_CONNECT_TIME);
    }
}

bool los_status_get_wakeup_source_statistics(m_wakeup_source *m_core_wakeup_source)
{
    if (memcpy_s(m_core_wakeup_source, sizeof(m_wakeup_source), g_wakeup_source, sizeof(m_wakeup_source)) == EOK) {
        if (memset_s(g_wakeup_source, sizeof(m_wakeup_source), 0, sizeof(m_wakeup_source)) == EOK) {
            return true;
        }
    }
    return false;
}

#if USE_RPC_MODE == YES
bool get_system_wakeup_source(system_wakeup_source_t *system_wakeup_source)
{
    bool ret = false;
    uint16_t max_len = sizeof(system_wakeup_source_t);
    uint16_t len;
    if (command_get_wakeup_src(&ret, max_len, &len, (uint32_t *)system_wakeup_source) != RPC_ERR_OK) {
        return false;
    }

    if (!ret || max_len != len) {
        return false;
    }
    return true;
}
#endif
#endif
