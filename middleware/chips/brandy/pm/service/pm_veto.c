/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm veto manage code
 */

#include "pm_veto.h"
#include "pm_core.h"
#include "errcode.h"
#include "pm_os.h"

#if CONFIG_PM_VETO_TRACK_EN == YES
typedef struct {
    uint64_t veto_time;              // 投票的当前时间
    uint16_t veto_id;
    uint16_t veto_counts;
    uint32_t veto_lr;
    uint32_t veto_timeout;
} pm_veto_track_info_t;

typedef struct {
    pm_veto_track_info_t *track_info;
    uint32_t track_mask;
    uint8_t track_len;
    uint8_t track_loc;
} pm_veto_track_t;

pm_veto_track_t g_pm_veto_track = {0};
#endif

static pm_veto_t g_pm_veto_info = {0};

#if CONFIG_PM_VETO_TRACK_EN == YES
errcode_t pm_veto_start_track(uint8_t veto_mask, uint8_t num)
{
    g_pm_veto_track.track_mask = veto_mask;
    g_pm_veto_track.track_len = num;
    g_pm_veto_track.track_loc = 0;
    g_pm_veto_track.track_info = pm_os_malloc(sizeof(pm_veto_track_info_t) * num);
    if (g_pm_veto_track.track_info == NULL) {
        return ERRCODE_PM_MEM_ALLOC_ERROR;
    }
}

void pm_veto_stop_track(void)
{
    g_pm_veto_track.track_mask = 0;
    pm_os_free(g_pm_veto_track.track_info);
    g_pm_veto_track.track_info = NULL;
}
#endif

bool pm_get_sleep_veto(void)
{
    if (g_pm_veto_info.veto_counts.total_counts != 0) {
        return true;
    }

    if (g_pm_veto_info.veto_timeout_timestamp > pm_core_get_current_ms()) {
        return true;
    }

    // Temporary adaptation of old code
    if (lpc_get_sleep_veto() != 0) {
        return true;
    }
    return false;
}

pm_veto_t *pm_veto_get_info(void)
{
    return &g_pm_veto_info;
}

uint8_t pm_veto_get_id_cnt(pm_id_t veto_id)
{
    return g_pm_veto_info.veto_counts.sub_counts[veto_id];
}

void pm_veto_init(void)
{
    (void)memset_s((void *)&g_pm_veto_info, sizeof(pm_veto_t), 0, sizeof(pm_veto_t));
    g_pm_veto_info.last_veto_id = PM_ID_MAX;
}

errcode_t uapi_pm_add_sleep_veto(pm_id_t veto_id)
{
    if (veto_id >= PM_ID_MAX) {
        return ERRCODE_PM_INVALID_PARAMETER;
    }

    uint32_t status = pm_os_irq_lock();
    g_pm_veto_info.veto_counts.total_counts++;
    g_pm_veto_info.veto_counts.sub_counts[veto_id]++;
    g_pm_veto_info.last_veto_lr = PM_GET_LR;
    g_pm_veto_info.last_veto_id = veto_id;

#if CONFIG_PM_VETO_TRACK_EN == YES
    uint8_t track_loc = g_pm_veto_track.track_loc;
    uint8_t track_len = g_pm_veto_track.track_len;
    if (bit(veto_id) & g_pm_veto_track.track_mask) {
        g_pm_veto_track.track_info[track_loc].veto_id = veto_id;
        g_pm_veto_track.track_info[track_loc].veto_counts = g_pm_veto_info.veto_counts.total_counts;
        g_pm_veto_track.track_info[track_loc].veto_lr = g_pm_veto_info.last_veto_lr;
        g_pm_veto_track.track_info[track_loc].veto_time = pm_core_get_current_ms();

        g_pm_veto_track.track_loc = (track_loc == track_len) ? track_loc++ : 0;
    }
#endif
    pm_os_irq_restore(status);
    return ERRCODE_SUCC;
}

errcode_t uapi_pm_add_sleep_veto_with_timeout(pm_id_t veto_id, uint32_t timeout_ms)
{
    if (veto_id >= PM_ID_MAX) {
        return ERRCODE_PM_INVALID_PARAMETER;
    }

    uint32_t status = pm_os_irq_lock();
    if (timeout_ms == 0) {
        g_pm_veto_info.veto_timeout_timestamp = 0;
    } else {
        g_pm_veto_info.veto_timeout_timestamp = timeout_ms + pm_core_get_current_ms();
    }

#if CONFIG_PM_VETO_TRACK_EN == YES
    uint8_t track_loc = g_pm_veto_track.track_loc;
    uint8_t track_len = g_pm_veto_track.track_len;
    if (bit(veto_id) & g_pm_veto_track.track_mask) {
        g_pm_veto_track.track_info[track_loc].veto_id = veto_id;
        g_pm_veto_track.track_info[track_loc].veto_counts = g_pm_veto_info.veto_counts.total_counts;
        g_pm_veto_track.track_info[track_loc].veto_lr = 0;
        g_pm_veto_track.track_info[track_loc].veto_time = pm_core_get_current_ms();
        g_pm_veto_track.track_info[track_loc].veto_timeout = timeout_ms;

        g_pm_veto_track.track_loc = (track_loc == track_len) ? track_loc++ : 0;
    }
#endif
    pm_os_irq_restore(status);
    return ERRCODE_SUCC;
}

errcode_t uapi_pm_remove_sleep_veto(pm_id_t veto_id)
{
    if (veto_id >= PM_ID_MAX) {
        return ERRCODE_PM_INVALID_PARAMETER;
    }

    uint32_t status = pm_os_irq_lock();
    if (g_pm_veto_info.veto_counts.sub_counts[veto_id] != 0) {
        g_pm_veto_info.veto_counts.sub_counts[veto_id]--;
        g_pm_veto_info.veto_counts.total_counts--;
    }
    pm_os_irq_restore(status);
    return ERRCODE_SUCC;
}