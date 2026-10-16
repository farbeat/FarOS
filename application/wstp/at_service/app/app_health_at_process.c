/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: FarOS Health Service AT command processing. \n
 *              Provides AT commands for health algorithm debugging \n
 *              and data query. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#include "app_health_at_process.h"
#include "app_msg_manager.h"
#include "app_tx_process.h"
#include "app_common_api.h"
#include "at_cmd_api.h"
#include "debug_print.h"
#include "securec.h"

#include "far_health_common.h"
#include "far_sensor.h"
#include "far_activity.h"
#include "far_physio.h"
#include "far_sleep.h"
#include "far_health_data.h"

/*
 * ============================================================
 * 运动健康AT命令处理
 * Health Service AT Command Processing
 * ============================================================
 *
 * 支持的AT命令列表：
 *
 * AT^HEALTHINIT          - 初始化运动健康服务
 * AT^HEALTHDEINIT        - 反初始化运动健康服务
 * AT^HEALTHVER           - 获取版本号
 * AT^HEALTHSTEP          - 获取步数信息
 * AT^HEALTHACT           - 获取当前活动类型
 * AT^HEALTHCAD           - 获取步频信息
 * AT^HEALTHDIST          - 获取距离信息
 * AT^HEALTHHR            - 获取心率信息
 * AT^HEALTHHRV           - 执行HRV分析
 * AT^HEALTHCAL           - 获取卡路里信息
 * AT^HEALTHVO2           - 获取VO2max估算值
 * AT^HEALTHLOAD          - 获取训练负荷
 * AT^HEALTHREC           - 获取恢复时间
 * AT^HEALTHSTR           - 获取压力指数
 * AT^HEALTHBAT           - 获取身体电量
 * AT^HEALTHSLEEP         - 获取最近睡眠记录
 * AT^HEALTHSLEEPSTAGE    - 获取睡眠分期
 * AT^HEALTHSLEEPSCORE    - 获取睡眠评分
 * AT^HEALTHDAILY         - 获取每日汇总
 * AT^HEALTHSENSOR        - 传感器控制（使能/禁用/采样率）
 * AT^HEALTHPROFILE       - 设置用户档案
 * AT^HEALTHSTATUS        - 获取模块运行状态
 */

/* AT命令处理函数类型 */
typedef uint32_t (*health_at_handler_t)(uint8_t *para, uint32_t para_len, char *str_cmd);

/* AT命令表条目 */
typedef struct {
    const char *cmd;
    health_at_handler_t handler;
    const char *help;
} health_at_cmd_entry_t;

/*
 * AT^HEALTHINIT - 初始化运动健康服务
 */
static uint32_t health_at_init_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    errcode_t ret = uapi_health_service_init();
    wstp_print("\r\n[HEALTH] Init %s\r\n", (ret == ERRCODE_SUCC) ? "OK" : "FAIL");
    return ret;
}

/*
 * AT^HEALTHDEINIT - 反初始化运动健康服务
 */
static uint32_t health_at_deinit_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    errcode_t ret = uapi_health_service_deinit();
    wstp_print("\r\n[HEALTH] Deinit %s\r\n", (ret == ERRCODE_SUCC) ? "OK" : "FAIL");
    return ret;
}

/*
 * AT^HEALTHVER - 获取版本号
 */
static uint32_t health_at_version_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    uint8_t major, minor, patch;
    errcode_t ret = uapi_health_get_version(&major, &minor, &patch);
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\n[HEALTH] Version: %d.%d.%d\r\n", major, minor, patch);
    }
    return ret;
}

/*
 * AT^HEALTHSTEP - 获取步数信息
 */
static uint32_t health_at_step_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_step_info_t info = {0};
    errcode_t ret = uapi_activity_get_step_count(&info);
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\n[HEALTH] Steps: total=%lu, today=%lu\r\n",
                   info.total_steps, info.steps_today);
    }
    return ret;
}

/*
 * AT^HEALTHACT - 获取当前活动类型
 */
static uint32_t health_at_activity_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_activity_result_t result = {0};
    errcode_t ret = uapi_activity_get_type(&result);
    if (ret == ERRCODE_SUCC) {
        const char *type_names[] = {"UNKNOWN", "REST", "WALK", "RUN", "CYCLE",
                                     "SWIM", "HIKE", "ELLIPTICAL", "ROWING",
                                     "STAIR", "OTHER"};
        const char *name = (result.type <= FAR_ACTIVITY_TYPE_SPORT_OTHER) ?
                            type_names[result.type] : "INVALID";
        wstp_print("\r\n[HEALTH] Activity: %s, confidence=%d%%, duration=%lus\r\n",
                   name, result.confidence, result.duration_sec);
    }
    return ret;
}

/*
 * AT^HEALTHCAD - 获取步频信息
 */
static uint32_t health_at_cadence_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_cadence_info_t info = {0};
    errcode_t ret = uapi_activity_get_cadence(&info);
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\n[HEALTH] Cadence: current=%u, avg=%u, max=%u SPM\r\n",
                   info.current_spm, info.avg_spm, info.max_spm);
    }
    return ret;
}

/*
 * AT^HEALTHDIST - 获取距离信息
 */
static uint32_t health_at_distance_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_distance_info_t info = {0};
    errcode_t ret = uapi_activity_get_distance(&info);
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\n[HEALTH] Distance: total=%lum, today=%lum, stride=%.1fcm\r\n",
                   info.total_distance_m, info.distance_today_m,
                   (double)info.avg_stride_length_cm);
    }
    return ret;
}

/*
 * AT^HEALTHHR - 获取心率信息
 */
static uint32_t health_at_hr_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_hr_info_t info = {0};
    errcode_t ret = uapi_physio_hr_get_current(&info);
    if (ret == ERRCODE_SUCC) {
        const char *zone_names[] = {"WARMUP", "FAT_BURN", "AEROBIC",
                                     "THRESHOLD", "ANAEROBIC"};
        wstp_print("\r\n[HEALTH] HR: current=%u, rest=%u, max=%u, zone=%s, quality=%u%%\r\n",
                   info.current_bpm, info.resting_bpm, info.max_bpm,
                   zone_names[info.current_zone], info.signal_quality);
    }
    return ret;
}

/*
 * AT^HEALTHHRV - 执行HRV分析
 */
static uint32_t health_at_hrv_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_hrv_result_t result = {0};
    errcode_t ret = uapi_physio_hrv_analyze(&result);
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\n[HEALTH] HRV: SDNN=%ums, RMSSD=%ums\r\n",
                   result.sdnn_ms, result.rmssd_ms);
        wstp_print("[HEALTH] HRV: LF=%u, HF=%u, LF/HF=%.2f, score=%u\r\n",
                   result.lf_power, result.hf_power,
                   (double)result.lf_hf_ratio / 100.0, result.hrv_score);
    } else {
        wstp_print("\r\n[HEALTH] HRV analysis failed (insufficient data)\r\n");
    }
    return ret;
}

/*
 * AT^HEALTHCAL - 获取卡路里信息
 */
static uint32_t health_at_calorie_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_calorie_info_t info = {0};
    errcode_t ret = uapi_physio_calorie_get_info(&info);
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\n[HEALTH] Calories: total=%lu, active=%lu, BMR=%lu, MET=%.1f\r\n",
                   info.total_kcal, info.active_kcal, info.bmr_kcal,
                   (double)info.current_met / 10.0);
    }
    return ret;
}

/*
 * AT^HEALTHVO2 - 获取VO2max估算值
 */
static uint32_t health_at_vo2max_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_vo2max_result_t result = {0};
    errcode_t ret = uapi_physio_vo2max_get(&result);
    if (ret == ERRCODE_SUCC && result.vo2max_ml_kg_min > 0) {
        wstp_print("\r\n[HEALTH] VO2max: %u ml/kg/min, confidence=%u%%, level=%u\r\n",
                   result.vo2max_ml_kg_min, result.confidence, result.fitness_level);
    } else {
        wstp_print("\r\n[HEALTH] VO2max: not available (need running/cycling data)\r\n");
    }
    return ret;
}

/*
 * AT^HEALTHLOAD - 获取训练负荷
 */
static uint32_t health_at_load_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_training_load_t load = {0};
    errcode_t ret = uapi_physio_training_load_get(&load);
    if (ret == ERRCODE_SUCC) {
        const char *zone_names[] = {"LOW", "OPTIMAL", "HIGH", "OVERREACHING"};
        wstp_print("\r\n[HEALTH] Load: EPOC=%u, session=%lu, weekly=%lu, zone=%s\r\n",
                   load.current_epoc, load.session_load, load.weekly_load,
                   zone_names[load.load_zone]);
    }
    return ret;
}

/*
 * AT^HEALTHREC - 获取恢复时间
 */
static uint32_t health_at_recovery_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_recovery_info_t recovery = {0};
    errcode_t ret = uapi_physio_recovery_time_get(&recovery);
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\n[HEALTH] Recovery: %uh%um, score=%u, %s\r\n",
                   recovery.recovery_hours, recovery.recovery_minutes,
                   recovery.recovery_score,
                   recovery.is_recovered ? "recovered" : "not recovered");
    }
    return ret;
}

/*
 * AT^HEALTHSTR - 获取压力指数
 */
static uint32_t health_at_stress_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_stress_info_t stress = {0};
    errcode_t ret = uapi_physio_stress_get(&stress);
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\n[HEALTH] Stress: current=%u, avg=%u, stressed=%umin, rest=%umin\r\n",
                   stress.current_stress, stress.avg_stress,
                   stress.stress_duration_min, stress.rest_duration_min);
    }
    return ret;
}

/*
 * AT^HEALTHBAT - 获取身体电量
 */
static uint32_t health_at_battery_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_body_battery_t battery = {0};
    errcode_t ret = uapi_physio_body_battery_get(&battery);
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\n[HEALTH] Body Battery: current=%u, high=%u, low=%u, %s\r\n",
                   battery.current_level, battery.highest_today, battery.lowest_today,
                   battery.is_charging ? "charging" : "draining");
    }
    return ret;
}

/*
 * AT^HEALTHSLEEP - 获取最近睡眠记录
 */
static uint32_t health_at_sleep_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    far_sleep_record_t record = {0};
    errcode_t ret = uapi_sleep_get_last_record(&record);
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\n[HEALTH] Sleep: total=%umin, deep=%u, light=%u, REM=%u, awake=%u\r\n",
                   record.total_sleep_min, record.deep_sleep_min,
                   record.light_sleep_min, record.rem_sleep_min, record.awake_min);
        wstp_print("[HEALTH] Sleep: score=%u, awakenings=%u\r\n",
                   record.sleep_score, record.awake_count);
    } else {
        wstp_print("\r\n[HEALTH] Sleep: no record available\r\n");
    }
    return ret;
}

/*
 * AT^HEALTHSLEEPSCORE - 获取睡眠评分
 */
static uint32_t health_at_sleep_score_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    uint8_t score = 0;
    errcode_t ret = uapi_sleep_get_score(&score);
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\n[HEALTH] Sleep Score: %u/100\r\n", score);
    }
    return ret;
}

/*
 * AT^HEALTHDAILY - 获取每日汇总
 */
static uint32_t health_at_daily_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    uint32_t steps, distance_m;
    uint16_t active_min;
    errcode_t ret = uapi_activity_get_daily_summary(&steps, &distance_m, &active_min);
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\n[HEALTH] Daily: steps=%lu, distance=%lum, active=%umin\r\n",
                   steps, distance_m, active_min);
    }
    return ret;
}

/*
 * AT^HEALTHSENSOR - 传感器控制
 * AT^HEALTHSENSOR=0,1  使能IMU
 * AT^HEALTHSENSOR=0,0  禁用IMU
 * AT^HEALTHSENSOR=1,1  使能PPG
 * AT^HEALTHSENSOR=1,0  禁用PPG
 */
static uint32_t health_at_sensor_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL || para_len == 0) {
        return ERRCODE_INVALID_PARAM;
    }

    uint8_t sensor_type = 0;
    uint8_t enable = 0;
    /* 简单解析: para格式为 "type,enable" */
    if (sscanf_s((char *)para, "%hhu,%hhu", &sensor_type, &enable) == 2) {
        errcode_t ret = uapi_sensor_enable(sensor_type, (bool)enable);
        wstp_print("\r\n[HEALTH] Sensor %d %s\r\n", sensor_type,
                   (ret == ERRCODE_SUCC) ? (enable ? "enabled" : "disabled") : "failed");
        return ret;
    }
    return ERRCODE_INVALID_PARAM;
}

/*
 * AT^HEALTHSTATUS - 获取模块运行状态
 */
static uint32_t health_at_status_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    uint32_t imu_power = 0, ppg_power = 0;
    (void)uapi_sensor_get_power_estimate(0, &imu_power);
    (void)uapi_sensor_get_power_estimate(1, &ppg_power);

    wstp_print("\r\n[HEALTH] Status:\r\n");
    wstp_print("  IMU power: %lu uA\r\n", imu_power);
    wstp_print("  PPG power: %lu uA\r\n", ppg_power);
    wstp_print("  Total sensor power: %lu uA\r\n", imu_power + ppg_power);

    uint32_t used = 0, total = 0;
    (void)uapi_health_data_get_storage_usage(&used, &total);
    wstp_print("  Storage: %lu/%lu bytes\r\n", used, total);

    return ERRCODE_SUCC;
}

/* ============================================================
 * AT Command Table
 * ============================================================ */

static const at_cmd_table_t g_health_at_cmd[] = {
    { "AT^HEALTHINIT",      health_at_init_handle,         "" },
    { "AT^HEALTHDEINIT",    health_at_deinit_handle,       "" },
    { "AT^HEALTHVER",       health_at_version_handle,      "" },
    { "AT^HEALTHSTEP",      health_at_step_handle,         "" },
    { "AT^HEALTHACT",       health_at_activity_handle,     "" },
    { "AT^HEALTHCAD",       health_at_cadence_handle,      "" },
    { "AT^HEALTHDIST",      health_at_distance_handle,     "" },
    { "AT^HEALTHHR",        health_at_hr_handle,           "" },
    { "AT^HEALTHHRV",       health_at_hrv_handle,          "" },
    { "AT^HEALTHCAL",       health_at_calorie_handle,      "" },
    { "AT^HEALTHVO2",       health_at_vo2max_handle,       "" },
    { "AT^HEALTHLOAD",      health_at_load_handle,         "" },
    { "AT^HEALTHREC",       health_at_recovery_handle,     "" },
    { "AT^HEALTHSTR",       health_at_stress_handle,       "" },
    { "AT^HEALTHBAT",       health_at_battery_handle,      "" },
    { "AT^HEALTHSLEEP",     health_at_sleep_handle,        "" },
    { "AT^HEALTHSLEEPSCORE", health_at_sleep_score_handle, "" },
    { "AT^HEALTHDAILY",     health_at_daily_handle,        "" },
    { "AT^HEALTHSENSOR",    health_at_sensor_handle,       "" },
    { "AT^HEALTHSTATUS",    health_at_status_handle,       "" },
};

errcode_t app_health_at_init(void)
{
    PRINT("[app_health_at] Health AT commands registered: %u\r\n",
          (uint32_t)(sizeof(g_health_at_cmd) / sizeof(at_cmd_table_t)));
    return ERRCODE_SUCC;
}

errcode_t app_health_at_process(uint8_t msg_id, const uint8_t *payload, uint32_t len)
{
    (void)msg_id;
    (void)payload;
    (void)len;

    /* TODO: 处理来自AT命令分发的健康消息
     * 根据msg_id分发到对应的处理函数
     */

    return ERRCODE_SUCC;
}

void *app_health_get_cmd_table(uint32_t *cmd_count)
{
    if (cmd_count != NULL) {
        *cmd_count = sizeof(g_health_at_cmd) / sizeof(at_cmd_table_t);
    }
    return (void *)g_health_at_cmd;
}
