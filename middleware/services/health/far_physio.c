/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: Physiological indicator analysis implementation skeleton. \n
 *              Includes heart rate, HRV, calorie, VO2max, training load, \n
 *              recovery time, stress, and body battery. \n
 *              Designed with reference to Firstbeat core concepts. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#include "far_physio.h"
#include "far_activity.h"
#include "debug_print.h"
#include "securec.h"
#include "tcxo.h"
#include "math.h"

/*
 * ============================================================
 * 生理指标算法骨架实现说明
 * Physiological Indicator Skeleton Implementation Notes
 * ============================================================
 *
 * 本文件包含心率、HRV、卡路里、VO2max、训练负荷、恢复时间、
 * 压力指数和身体电量等生理指标的算法框架。
 * 参考Firstbeat核心概念设计，具体算法逻辑以注释形式说明。
 *
 * ===== 心率监测算法 =====
 * PPG信号处理流程：
 * 1. 预处理：DC消除（移动平均）、带通滤波（0.5-4Hz）
 * 2. 峰值检测：检测PPG波形的峰值点（对应心跳）
 * 3. 间隔验证：峰值间隔需在300ms-2000ms之间（30-200 BPM）
 * 4. 动态阈值：自适应调整峰值检测阈值
 * 5. 输出：BPM = 60000 / 平均RR间隔(ms)
 *
 * ===== HRV分析算法（参考Firstbeat） =====
 * 输入：连续5分钟以上的RR间期序列
 * 1. 异常值剔除：去除RR间期超出平均±20%的数据点
 * 2. 时域分析：
 *    - SDNN = sqrt(mean((RR_i - mean(RR))²))
 *    - RMSSD = sqrt(mean((RR_i+1 - RR_i)²))
 * 3. 频域分析：
 *    - 对RR间期进行重采样（4Hz）
 *    - 应用FFT或AR模型进行频谱分析
 *    - LF: 0.04-0.15 Hz（交感神经+副交感神经）
 *    - HF: 0.15-0.4 Hz（副交感神经）
 * 4. HRV评分：基于年龄标准化的百分位数
 *
 * ===== 卡路里计算（参考Firstbeat METs模型） =====
 * 总卡路里 = BMR + 主动运动卡路里
 * BMR (Harris-Benedict方程):
 *   男: 88.362 + 13.397×体重(kg) + 4.799×身高(cm) - 5.677×年龄
 *   女: 447.593 + 9.247×体重(kg) + 3.098×身高(cm) - 4.330×年龄
 * 主动运动卡路里 = MET × 体重(kg) × 时间(小时)
 * MET估算基于活动类型和心率区间
 *
 * ===== VO2max估算（参考Firstbeat） =====
 * 基于跑步/骑行中的心率和速度/功率数据：
 * VO2max ≈ (v × 斜率系数) / (HR_ratio) × 校正因子
 * 其中 HR_ratio = (HR_max - HR_rest) / (HR_exercise - HR_rest)
 * 需要至少15分钟持续运动，心率≥最大心率的70%
 *
 * ===== 训练负荷（参考Firstbeat EPOC模型） =====
 * EPOC(t) = EPOC(t-1) + (MET(t) - 1) × k - EPOC(t-1) / τ
 * 其中：
 *   - k: 个体校准系数
 *   - τ: EPOC衰减时间常数（约30-60分钟）
 * 训练负荷 = 单次运动中的EPOC峰值或累积量
 *
 * ===== 身体电量（参考Firstbeat Body Battery） =====
 * BodyBattery(t) = BodyBattery(t-1) + Δcharge - Δdrain
 * 充电速率（休息/睡眠）基于HRV恢复状态
 * 放速率（活动/压力）基于EPOC累积和HR升高
 * 范围：0-100
 */

#define RR_BUF_SIZE                 512     /* RR间期缓冲区 */
#define HRV_MIN_SAMPLES             150     /* HRV最少样本数（5分钟@0.5Hz） */
#define HR_MAX_BPM                  220     /* 最大心率上限 */
#define HR_MIN_BPM                  30      /* 最小心率下限 */
#define CALORIE_BMR_UPDATE_INTERVAL 3600    /* BMR更新间隔 1小时 */
#define VO2MAX_MIN_DURATION_MIN     15      /* VO2max最小运动时长 */
#define STRESS_WINDOW_MIN           3       /* 压力分析窗口 3分钟 */
#define BODY_BATTERY_MAX            100
#define BODY_BATTERY_MIN            0

/* 生理指标模块状态 */
typedef struct {
    bool initialized;

    /* 心率状态 */
    bool hr_enabled;
    uint8_t current_hr;
    uint8_t resting_hr;
    uint8_t max_hr;
    uint8_t signal_quality;
    uint32_t hr_last_update;
    uint16_t hr_zone_time_sec[FAR_HEALTH_HR_ZONE_COUNT];

    /* HRV状态 */
    uint16_t rr_intervals[RR_BUF_SIZE];
    uint16_t rr_write_idx;
    uint16_t rr_count;
    far_hrv_result_t last_hrv;

    /* 卡路里状态 */
    uint32_t total_calories;
    uint32_t active_calories;
    uint32_t bmr_calories;
    uint16_t current_met;
    uint32_t calorie_last_update;

    /* VO2max状态 */
    far_vo2max_result_t vo2max;
    bool vo2max_test_in_progress;
    uint32_t vo2max_test_start_time;

    /* 训练负荷状态 */
    uint16_t current_epoc;
    uint32_t session_load;
    uint32_t weekly_load;

    /* 恢复时间状态 */
    far_recovery_info_t recovery;

    /* 压力状态 */
    bool stress_enabled;
    far_stress_info_t stress;
    uint16_t stress_window[STRESS_WINDOW_MIN * 60]; /* 3分钟窗口 */

    /* 身体电量状态 */
    bool body_battery_enabled;
    far_body_battery_t body_battery;
} far_physio_state_t;

static far_physio_state_t g_physio = {0};
static far_user_profile_t g_physio_profile = {0};

/* ============================================================
 * Internal Helper Functions
 * ============================================================ */

/*
 * 计算最大心率（220-年龄或用户自定义）
 */
static uint8_t physio_calc_max_hr(uint8_t age)
{
    if (g_physio_profile.max_hr > 0) {
        return g_physio_profile.max_hr;
    }
    /* Tanaka公式：208 - 0.7 × 年龄（比220-年龄更准确） */
    uint8_t max_hr = (uint8_t)(208.0f - 0.7f * (float)age);
    if (max_hr > HR_MAX_BPM) {
        max_hr = HR_MAX_BPM;
    }
    return max_hr;
}

/*
 * 计算心率储备百分比（%HRR）
 */
static uint8_t physio_calc_hrr_percent(uint8_t hr, uint8_t rest_hr, uint8_t max_hr)
{
    if (max_hr <= rest_hr) {
        return 0;
    }
    uint16_t hrr = (uint16_t)(hr - rest_hr) * 100 / (uint16_t)(max_hr - rest_hr);
    if (hrr > 100) {
        hrr = 100;
    }
    return (uint8_t)hrr;
}

/*
 * 根据%HRR确定心率区间
 */
static far_hr_zone_t physio_get_hr_zone(uint8_t hrr_percent)
{
    if (hrr_percent < 60) {
        return FAR_HR_ZONE_WARMUP;
    } else if (hrr_percent < 70) {
        return FAR_HR_ZONE_FAT_BURN;
    } else if (hrr_percent < 80) {
        return FAR_HR_ZONE_AEROBIC;
    } else if (hrr_percent < 90) {
        return FAR_HR_ZONE_THRESHOLD;
    } else {
        return FAR_HR_ZONE_ANAEROBIC;
    }
}

/*
 * 计算BMR（基础代谢率）
 * Harris-Benedict方程
 */
static uint16_t physio_calc_bmr(void)
{
    float weight = (float)g_physio_profile.weight_kg / 10.0f;
    float height = (float)g_physio_profile.height_cm;
    float age = (float)g_physio_profile.age;
    float bmr;

    if (g_physio_profile.gender == 1) { /* Male */
        bmr = 88.362f + 13.397f * weight + 4.799f * height - 5.677f * age;
    } else { /* Female */
        bmr = 447.593f + 9.247f * weight + 3.098f * height - 4.330f * age;
    }

    if (bmr < 800.0f) {
        bmr = 800.0f;
    } else if (bmr > 3000.0f) {
        bmr = 3000.0f;
    }

    return (uint16_t)bmr;
}

/*
 * 根据活动类型和心率估算MET值
 * 参考Firstbeat METs模型
 */
static uint16_t physio_estimate_met(far_activity_type_t activity, uint8_t hrr_percent)
{
    /* 基础MET值（久坐=1.0） */
    float base_met = 1.0f;

    switch (activity) {
        case FAR_ACTIVITY_TYPE_REST:
            base_met = 1.0f;
            break;
        case FAR_ACTIVITY_TYPE_WALK:
            base_met = 3.5f;
            break;
        case FAR_ACTIVITY_TYPE_RUN:
            base_met = 8.0f;
            break;
        case FAR_ACTIVITY_TYPE_CYCLE:
            base_met = 6.0f;
            break;
        case FAR_ACTIVITY_TYPE_SWIM:
            base_met = 7.0f;
            break;
        case FAR_ACTIVITY_TYPE_HIKE:
            base_met = 5.0f;
            break;
        default:
            base_met = 2.0f;
            break;
    }

    /* 根据心率强度调整MET */
    float intensity_factor = 1.0f + (float)hrr_percent / 100.0f;
    float met = base_met * intensity_factor;

    return (uint16_t)(met * 10); /* x10 for integer storage */
}

/*
 * 计算SDNN
 */
static uint16_t physio_calc_sdnn(const uint16_t *rr_data, uint16_t count)
{
    /* TODO: 实现SDNN计算
     * 1. 计算RR间期平均值
     * 2. 计算每个RR间期与平均值的差的平方
     * 3. 求均方 = sum(sq_diff) / count
     * 4. SDNN = sqrt(均方)
     */
    (void)rr_data;
    return count > 0 ? 45 : 0; /* 骨架默认值 45ms */
}

/*
 * 计算RMSSD
 */
static uint16_t physio_calc_rmssd(const uint16_t *rr_data, uint16_t count)
{
    /* TODO: 实现RMSSD计算
     * RMSSD = sqrt(mean((RR_i+1 - RR_i)²))
     */
    (void)rr_data;
    return count > 1 ? 35 : 0; /* 骨架默认值 35ms */
}

/* ============================================================
 * Public APIs
 * ============================================================ */

errcode_t uapi_physio_init(void)
{
    if (g_physio.initialized) {
        return ERRCODE_SUCC;
    }

    (void)memset_s(&g_physio, sizeof(g_physio), 0, sizeof(g_physio));

    /* 默认用户档案 */
    g_physio_profile.gender = 1;
    g_physio_profile.age = 30;
    g_physio_profile.height_cm = 170;
    g_physio_profile.weight_kg = 700;
    g_physio_profile.max_hr = 0;
    g_physio_profile.rest_hr = 60;

    g_physio.max_hr = physio_calc_max_hr(g_physio_profile.age);
    g_physio.resting_hr = g_physio_profile.rest_hr;
    g_physio.initialized = true;

    PRINT("[far_physio] Physio module init, max_hr=%d\r\n", g_physio.max_hr);
    return ERRCODE_SUCC;
}

errcode_t uapi_physio_deinit(void)
{
    (void)memset_s(&g_physio, sizeof(g_physio), 0, sizeof(g_physio));
    return ERRCODE_SUCC;
}

/* ===== Heart Rate APIs ===== */

errcode_t uapi_physio_hr_init(void)
{
    g_physio.hr_enabled = true;
    g_physio.current_hr = 0;
    g_physio.signal_quality = 0;
    return ERRCODE_SUCC;
}

errcode_t uapi_physio_hr_deinit(void)
{
    g_physio.hr_enabled = false;
    return ERRCODE_SUCC;
}

errcode_t uapi_physio_hr_get_current(far_hr_info_t *info)
{
    if (info == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    info->current_bpm = g_physio.current_hr;
    info->resting_bpm = g_physio.resting_hr;
    info->max_bpm = g_physio.max_hr;

    uint8_t hrr = physio_calc_hrr_percent(g_physio.current_hr,
                                           g_physio.resting_hr, g_physio.max_hr);
    info->current_zone = physio_get_hr_zone(hrr);
    info->signal_quality = g_physio.signal_quality;
    info->last_update_ms = g_physio.hr_last_update;

    return ERRCODE_SUCC;
}

errcode_t uapi_physio_hr_get_resting(uint8_t *resting_bpm)
{
    if (resting_bpm == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    /* TODO: 需要至少7天的心率数据才能计算可靠的静息心率
     * 1. 获取过去7天睡眠期间的最低心率
     * 2. 取第7百分位数作为静息心率
     * 3. 若数据不足，返回预设值
     */

    *resting_bpm = g_physio.resting_hr;
    return ERRCODE_SUCC;
}

errcode_t uapi_physio_hr_get_zone_distribution(far_hr_zone_distribution_t *distribution)
{
    if (distribution == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    uint32_t total_time = 0;
    for (int i = 0; i < FAR_HEALTH_HR_ZONE_COUNT; i++) {
        distribution->zone_time_sec[i] = g_physio.hr_zone_time_sec[i];
        total_time += g_physio.hr_zone_time_sec[i];
    }

    for (int i = 0; i < FAR_HEALTH_HR_ZONE_COUNT; i++) {
        if (total_time > 0) {
            distribution->zone_percent[i] = (uint8_t)(g_physio.hr_zone_time_sec[i] * 100 / total_time);
        } else {
            distribution->zone_percent[i] = 0;
        }
    }

    return ERRCODE_SUCC;
}

/* ===== HRV APIs ===== */

errcode_t uapi_physio_hrv_analyze(far_hrv_result_t *result)
{
    if (result == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    if (g_physio.rr_count < HRV_MIN_SAMPLES) {
        return ERRCODE_FAIL; /* 数据不足 */
    }

    /* TODO: 完整HRV分析
     * 1. 异常值剔除
     * 2. 时域分析：SDNN, RMSSD
     * 3. 频域分析：LF, HF, LF/HF（需FFT实现）
     * 4. 评分计算
     */

    result->sdnn_ms = physio_calc_sdnn(g_physio.rr_intervals, g_physio.rr_count);
    result->rmssd_ms = physio_calc_rmssd(g_physio.rr_intervals, g_physio.rr_count);
    result->lf_power = 800;   /* 骨架默认值 */
    result->hf_power = 600;   /* 骨架默认值 */
    result->lf_hf_ratio = 133; /* 1.33 x100 */
    result->hrv_score = 65;   /* 骨架默认值 */
    result->analysis_duration_sec = g_physio.rr_count * 1000 / 60; /* 近似 */

    g_physio.last_hrv = *result;
    return ERRCODE_SUCC;
}

errcode_t uapi_physio_hrv_feed_rr_data(const uint16_t *rr_intervals_ms, uint32_t len)
{
    if ((rr_intervals_ms == NULL) || (len == 0)) {
        return ERRCODE_INVALID_PARAM;
    }

    for (uint32_t i = 0; i < len; i++) {
        g_physio.rr_intervals[g_physio.rr_write_idx] = rr_intervals_ms[i];
        g_physio.rr_write_idx = (g_physio.rr_write_idx + 1) % RR_BUF_SIZE;
        if (g_physio.rr_count < RR_BUF_SIZE) {
            g_physio.rr_count++;
        }
    }

    return ERRCODE_SUCC;
}

/* ===== Calorie APIs ===== */

errcode_t uapi_physio_calorie_get_info(far_calorie_info_t *info)
{
    if (info == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    info->total_kcal = g_physio.total_calories;
    info->active_kcal = g_physio.active_calories;
    info->bmr_kcal = g_physio.bmr_calories;
    info->current_met = g_physio.current_met;
    info->last_update_ms = g_physio.calorie_last_update;

    return ERRCODE_SUCC;
}

errcode_t uapi_physio_calorie_get_today(uint32_t *total_kcal, uint32_t *active_kcal)
{
    if ((total_kcal == NULL) || (active_kcal == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

    *total_kcal = g_physio.total_calories;
    *active_kcal = g_physio.active_calories;
    return ERRCODE_SUCC;
}

/* ===== VO2max APIs ===== */

errcode_t uapi_physio_vo2max_get(far_vo2max_result_t *result)
{
    if (result == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    *result = g_physio.vo2max;
    return ERRCODE_SUCC;
}

errcode_t uapi_physio_vo2max_estimate(far_activity_type_t activity_type)
{
    if ((activity_type != FAR_ACTIVITY_TYPE_RUN) &&
        (activity_type != FAR_ACTIVITY_TYPE_CYCLE)) {
        return ERRCODE_NOT_SUPPORT;
    }

    /* TODO: 启动VO2max估算会话
     * 1. 记录起始时间
     * 2. 持续收集心率和速度/功率数据
     * 3. 验证运动强度（HR >= 70% max_hr）
     * 4. 15分钟后执行估算公式
     */

    g_physio.vo2max_test_in_progress = true;
    g_physio.vo2max_test_start_time = (uint32_t)uapi_tcxo_get_ms();

    PRINT("[far_physio] VO2max estimation started for activity %d\r\n", activity_type);
    return ERRCODE_SUCC;
}

/* ===== Training Load APIs ===== */

errcode_t uapi_physio_training_load_get(far_training_load_t *load)
{
    if (load == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    load->current_epoc = g_physio.current_epoc;
    load->session_load = g_physio.session_load;
    load->weekly_load = g_physio.weekly_load;

    /* TODO: 计算负荷区间
     * 基于个人历史数据确定低/最佳/高/过度区间
     */
    load->load_zone = 1; /* 骨架默认：最佳区间 */

    return ERRCODE_SUCC;
}

errcode_t uapi_physio_training_load_reset(void)
{
    g_physio.current_epoc = 0;
    g_physio.session_load = 0;
    return ERRCODE_SUCC;
}

/* ===== Recovery Time APIs ===== */

errcode_t uapi_physio_recovery_time_get(far_recovery_info_t *recovery)
{
    if (recovery == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    /* TODO: 基于EPOC衰减模型计算恢复时间
     * recovery_time(hours) ≈ EPOC_peak / 个体的EPOC清除速率
     * 清除速率与静息心率、年龄、训练水平相关
     */

    recovery->recovery_hours = 12;  /* 骨架默认值 */
    recovery->recovery_minutes = 0;
    recovery->recovery_score = 75;  /* 骨架默认值 */
    recovery->is_recovered = (recovery->recovery_score >= 95);

    return ERRCODE_SUCC;
}

/* ===== Stress APIs ===== */

errcode_t uapi_physio_stress_get(far_stress_info_t *stress)
{
    if (stress == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    *stress = g_physio.stress;
    return ERRCODE_SUCC;
}

errcode_t uapi_physio_stress_enable(bool enable)
{
    g_physio.stress_enabled = enable;
    PRINT("[far_physio] Stress monitoring %s\r\n", enable ? "enabled" : "disabled");
    return ERRCODE_SUCC;
}

/* ===== Body Battery APIs ===== */

errcode_t uapi_physio_body_battery_get(far_body_battery_t *battery)
{
    if (battery == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    *battery = g_physio.body_battery;
    return ERRCODE_SUCC;
}

errcode_t uapi_physio_body_battery_enable(bool enable)
{
    g_physio.body_battery_enabled = enable;
    PRINT("[far_physio] Body battery tracking %s\r\n", enable ? "enabled" : "disabled");
    return ERRCODE_SUCC;
}

/* ===== Feed APIs ===== */

errcode_t uapi_physio_feed_hr_data(uint8_t bpm, uint32_t timestamp_ms)
{
    if ((bpm < HR_MIN_BPM) || (bpm > HR_MAX_BPM)) {
        return ERRCODE_INVALID_PARAM;
    }

    g_physio.current_hr = bpm;
    g_physio.hr_last_update = timestamp_ms;

    /* 更新心率区间时间 */
    uint8_t hrr = physio_calc_hrr_percent(bpm, g_physio.resting_hr, g_physio.max_hr);
    far_hr_zone_t zone = physio_get_hr_zone(hrr);
    g_physio.hr_zone_time_sec[zone]++;

    /* 更新RR间期缓冲区 */
    uint16_t rr_ms = (uint16_t)(60000 / bpm);
    uapi_physio_hrv_feed_rr_data(&rr_ms, 1);

    /* TODO: 更新EPOC（训练负荷）
     * EPOC(t) = EPOC(t-1) + (MET - 1) × k - EPOC(t-1) / τ
     */

    /* TODO: 更新压力指数（基于HRV变化）
     * 压力与HRV呈负相关：HRV下降 → 压力上升
     */

    /* TODO: 更新身体电量
     * 若HRV恢复（HF增加）→ 充电
     * 若EPOC累积 / HR升高 → 放电
     */

    return ERRCODE_SUCC;
}

errcode_t uapi_physio_feed_activity_data(far_activity_type_t activity_type,
                                          uint8_t intensity, uint32_t duration_sec)
{
    /* TODO: 更新卡路里计算
     * MET = f(activity_type, intensity)
     * active_calories += MET × weight(kg) × duration(hour)
     */

    uint8_t hrr = intensity; /* 简化：用intensity作为HRR近似 */
    uint16_t met = physio_estimate_met(activity_type, hrr);
    g_physio.current_met = met;

    float weight_kg = (float)g_physio_profile.weight_kg / 10.0f;
    float duration_hour = (float)duration_sec / 3600.0f;
    float met_val = (float)met / 10.0f;
    uint32_t calories = (uint32_t)(met_val * weight_kg * duration_hour);

    g_physio.active_calories += calories;
    g_physio.total_calories = g_physio.bmr_calories + g_physio.active_calories;
    g_physio.calorie_last_update = (uint32_t)uapi_tcxo_get_ms();

    return ERRCODE_SUCC;
}
