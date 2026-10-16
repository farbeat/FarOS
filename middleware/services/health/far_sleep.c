/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: Sleep analysis algorithm implementation skeleton. \n
 *              Includes sleep stage detection, sleep scoring, and \n
 *              sleep quality assessment. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#include "far_sleep.h"
#include "far_sensor.h"
#include "far_physio.h"
#include "debug_print.h"
#include "securec.h"
#include "tcxo.h"

/*
 * ============================================================
 * 睡眠分析算法骨架实现说明
 * Sleep Analysis Skeleton Implementation Notes
 * ============================================================
 *
 * 本文件包含睡眠分期检测、睡眠评分和睡眠质量评估的算法框架。
 * 基于IMU运动检测和PPG心率/HRV数据实现睡眠分期算法。
 *
 * ===== 睡眠检测算法 =====
 * 1. 入睡检测
 *    - 连续30分钟无显著运动（加速度方差 < 阈值）
 *    - 心率开始下降（比日间平均低10%以上）
 *    - 两个条件同时满足判定为入睡
 *
 * 2. 清醒检测
 *    - 显著运动持续 > 5分钟
 *    - 或心率显著升高（ > 静息心率 + 20 BPM）
 *
 * ===== 睡眠分期算法（参考Firstbeat/Actigraphy） =====
 * 基于运动和心率特征的分期：
 *
 * 清醒（Wake）：
 *   - 高频大幅度运动
 *   - 心率变异大
 *
 * REM睡眠：
 *   - 几乎无运动（肌张力抑制）
 *   - 心率变异性高（类似清醒但身体不动）
 *   - 可能出现快速眼动相关的微动
 *
 * 浅睡（Light Sleep - N1/N2）：
 *   - 偶发微动
 *   - 心率逐渐降低
 *   - HRV适中
 *
 * 深睡（Deep Sleep - N3）：
 *   - 几乎无运动
 *   - 心率最低且稳定
 *   - HRV低（副交感神经主导）
 *   - 体温下降
 *
 * 分期决策流程（每30秒一个epoch）：
 * 1. 提取特征（30秒窗口）
 *    - 运动特征：运动次数、运动幅度、最大加速度
 *    - 心率特征：平均HR、HR变异性、HR趋势
 * 2. 决策树分类
 *    if 有显著运动: -> WAKE
 *    else if HR < 深睡阈值 AND HRV < 阈值: -> DEEP
 *    else if 无运动 AND HRV高: -> REM
 *    else: -> LIGHT
 *
 * ===== 睡眠评分算法 =====
 * 综合评分（0-100）基于以下因素：
 * - 总睡眠时长（目标7-9小时）
 * - 深睡比例（目标15-25%）
 * - 睡眠效率（睡眠时长/在床时长）
 * - 觉醒次数（越少越好）
 * - HRV恢复（睡眠期间HRV改善程度）
 * - 作息规律（与近期平均入睡/起床时间的偏差）
 */

#define SLEEP_EPOCH_DURATION_SEC    30      /* 每个epoch 30秒 */
#define SLEEP_MOTION_THRESHOLD_MG   150     /* 运动检测阈值 mg */
#define SLEEP_MIN_DURATION_MIN      30      /* 最少记录睡眠时长 */
#define SLEEP_FALL_ASLEEP_MIN       30      /* 入睡判定时长 30分钟 */
#define SLEEP_WAKE_THRESHOLD_MIN    5       /* 清醒判定时长 5分钟 */
#define SLEEP_HR_DROP_PERCENT       10      /* 心率下降百分比阈值 */
#define SLEEP_DEEP_HR_THRESHOLD     55      /* 深睡心率阈值（低于此值） */
#define SLEEP_MAX_EPOCHS            720     /* 最多360分钟/6小时 × 2 epochs/min */

/* 睡眠epoch数据结构 */
typedef struct {
    far_sleep_stage_t stage;
    uint8_t motion_count;       /* 该epoch内的运动次数 */
    uint16_t max_motion_mg;     /* 最大运动幅度 */
    uint8_t avg_hr;             /* 平均心率 */
    uint16_t hr_variability;    /* 心率变异性 */
    uint32_t timestamp;         /* epoch起始时间戳 */
} far_sleep_epoch_t;

/* 睡眠分析状态 */
typedef struct {
    bool initialized;
    bool enabled;
    bool tracking;
    bool is_sleeping;
    far_sleep_config_t config;
    far_sleep_record_t current_record;
    far_sleep_epoch_t epochs[SLEEP_MAX_EPOCHS];
    uint16_t epoch_count;
    uint32_t sleep_start_time;
    uint32_t last_motion_time;
    uint32_t consecutive_no_motion_sec;
    uint8_t daytime_avg_hr;     /* 日间平均心率（用于入睡判断） */

    /* 回调 */
    far_sleep_stage_callback_t stage_callback;
    void *callback_user_data;
} far_sleep_state_t;

static far_sleep_state_t g_sleep = {0};

/*
 * 运动检测（用于睡眠状态判断）
 */
static bool sleep_detect_motion(const far_sensor_imu_data_t *data, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        int32_t ax = (int32_t)data[i].ax;
        int32_t ay = (int32_t)data[i].ay;
        int32_t az = (int32_t)data[i].az;
        int32_t mag = ax * ax + ay * ay + az * az;

        /* 简化：检测是否超过运动阈值 */
        if (mag > (int32_t)(SLEEP_MOTION_THRESHOLD_MG * SLEEP_MOTION_THRESHOLD_MG)) {
            return true;
        }
    }
    return false;
}

/*
 * 睡眠分期决策（单个epoch）
 */
static far_sleep_stage_t sleep_classify_epoch(const far_sleep_epoch_t *epoch)
{
    /* TODO: 实现睡眠分期决策逻辑
     *
     * 1. 基于运动特征判断
     *    - 运动次数 > 阈值: WAKE
     *    - 最大运动幅度 > 阈值: WAKE
     *
     * 2. 基于心率特征判断
     *    - 平均HR < 深睡阈值 AND HRV低: DEEP
     *    - 无运动 AND HRV高: REM
     *    - 其他: LIGHT
     */

    (void)epoch;

    /* 骨架：简单规则 */
    if (epoch->motion_count > 5) {
        return FAR_SLEEP_STAGE_WAKE;
    } else if (epoch->avg_hr < SLEEP_DEEP_HR_THRESHOLD) {
        return FAR_SLEEP_STAGE_DEEP;
    } else if (epoch->motion_count == 0 && epoch->hr_variability > 100) {
        return FAR_SLEEP_STAGE_REM;
    }
    return FAR_SLEEP_STAGE_LIGHT;
}

/*
 * 计算睡眠评分
 */
static uint8_t sleep_calc_score(const far_sleep_record_t *record)
{
    /* TODO: 综合睡眠评分算法
     *
     * 1. 时长评分（40%权重）
     *    - 目标7-9小时，满分
     *    - <6小时或>10小时扣分
     *
     * 2. 深度评分（20%权重）
     *    - 深睡比例目标15-25%
     *    - <10%或>30%扣分
     *
     * 3. 效率评分（20%权重）
     *    - 效率 = 睡眠时长 / 在床时长
     *    - >85%满分
     *
     * 4. 连续性评分（20%权重）
     *    - 觉醒次数越少越好
     *    - 每次觉醒扣5分
     */

    (void)record;
    return 75; /* 骨架默认值 */
}

/* ============================================================
 * Public APIs
 * ============================================================ */

errcode_t uapi_sleep_init(const far_sleep_config_t *config)
{
    if (config == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    if (g_sleep.initialized) {
        return ERRCODE_SUCC;
    }

    (void)memcpy_s(&g_sleep.config, sizeof(far_sleep_config_t),
                   config, sizeof(far_sleep_config_t));

    g_sleep.initialized = true;
    g_sleep.enabled = false;
    g_sleep.tracking = false;
    g_sleep.is_sleeping = false;
    g_sleep.epoch_count = 0;
    g_sleep.sleep_start_time = 0;
    g_sleep.last_motion_time = 0;
    g_sleep.consecutive_no_motion_sec = 0;
    g_sleep.daytime_avg_hr = 70;
    g_sleep.stage_callback = NULL;
    g_sleep.callback_user_data = NULL;

    PRINT("[far_sleep] Sleep analysis init, auto_detect=%d\r\n",
          config->enable_auto_detect);
    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_deinit(void)
{
    (void)memset_s(&g_sleep, sizeof(g_sleep), 0, sizeof(g_sleep));
    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_enable(void)
{
    if (!g_sleep.initialized) {
        return ERRCODE_FAIL;
    }
    g_sleep.enabled = true;
    PRINT("[far_sleep] Sleep analysis enabled\r\n");
    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_disable(void)
{
    g_sleep.enabled = false;
    if (g_sleep.tracking) {
        (void)uapi_sleep_stop_tracking();
    }
    PRINT("[far_sleep] Sleep analysis disabled\r\n");
    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_start_tracking(void)
{
    if (!g_sleep.initialized) {
        return ERRCODE_FAIL;
    }

    g_sleep.tracking = true;
    g_sleep.is_sleeping = true;
    g_sleep.epoch_count = 0;
    g_sleep.sleep_start_time = (uint32_t)uapi_tcxo_get_ms();
    g_sleep.consecutive_no_motion_sec = 0;

    (void)memset_s(&g_sleep.current_record, sizeof(far_sleep_record_t),
                   0, sizeof(far_sleep_record_t));
    g_sleep.current_record.sleep_start_time = g_sleep.sleep_start_time;

    PRINT("[far_sleep] Sleep tracking started\r\n");
    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_stop_tracking(void)
{
    if (!g_sleep.tracking) {
        return ERRCODE_SUCC;
    }

    g_sleep.tracking = false;
    g_sleep.is_sleeping = false;
    g_sleep.current_record.sleep_end_time = (uint32_t)uapi_tcxo_get_ms();

    /* 计算睡眠统计 */
    g_sleep.current_record.total_sleep_min =
        (uint16_t)((g_sleep.current_record.sleep_end_time - g_sleep.sleep_start_time) / 60000);

    /* 计算各分期时长 */
    for (uint16_t i = 0; i < g_sleep.epoch_count; i++) {
        switch (g_sleep.epochs[i].stage) {
            case FAR_SLEEP_STAGE_WAKE:
                g_sleep.current_record.awake_min += SLEEP_EPOCH_DURATION_SEC / 60;
                break;
            case FAR_SLEEP_STAGE_REM:
                g_sleep.current_record.rem_sleep_min += SLEEP_EPOCH_DURATION_SEC / 60;
                break;
            case FAR_SLEEP_STAGE_LIGHT:
                g_sleep.current_record.light_sleep_min += SLEEP_EPOCH_DURATION_SEC / 60;
                break;
            case FAR_SLEEP_STAGE_DEEP:
                g_sleep.current_record.deep_sleep_min += SLEEP_EPOCH_DURATION_SEC / 60;
                break;
            default:
                break;
        }
    }

    /* 计算睡眠评分 */
    g_sleep.current_record.sleep_score = sleep_calc_score(&g_sleep.current_record);

    PRINT("[far_sleep] Sleep tracking stopped, score=%d\r\n",
          g_sleep.current_record.sleep_score);
    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_get_last_record(far_sleep_record_t *record)
{
    if (record == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    if (g_sleep.current_record.sleep_start_time == 0) {
        return ERRCODE_FAIL;
    }

    *record = g_sleep.current_record;
    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_get_score(uint8_t *score)
{
    if (score == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    *score = g_sleep.current_record.sleep_score;
    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_get_quality_factors(far_sleep_quality_factors_t *factors)
{
    if (factors == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    /* TODO: 计算详细睡眠质量因素
     * 1. 时长评分
     * 2. 深度评分
     * 3. 效率评分
     * 4. 规律评分
     * 5. HRV恢复评分
     */

    factors->duration_score = 80;
    factors->depth_score = 75;
    factors->efficiency_score = 85;
    factors->regularity_score = 70;
    factors->hrv_score = 72;

    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_get_stages(uint16_t *deep_min, uint16_t *light_min,
                                 uint16_t *rem_min, uint16_t *awake_min)
{
    if ((deep_min == NULL) || (light_min == NULL) ||
        (rem_min == NULL) || (awake_min == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

    *deep_min = g_sleep.current_record.deep_sleep_min;
    *light_min = g_sleep.current_record.light_sleep_min;
    *rem_min = g_sleep.current_record.rem_sleep_min;
    *awake_min = g_sleep.current_record.awake_min;

    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_register_stage_callback(far_sleep_stage_callback_t callback, void *user_data)
{
    if (callback == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    g_sleep.stage_callback = callback;
    g_sleep.callback_user_data = user_data;
    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_unregister_stage_callback(void)
{
    g_sleep.stage_callback = NULL;
    g_sleep.callback_user_data = NULL;
    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_feed_imu_data(const far_sensor_imu_data_t *data, uint32_t len)
{
    if ((data == NULL) || (len == 0)) {
        return ERRCODE_INVALID_PARAM;
    }

    if (!g_sleep.enabled) {
        return ERRCODE_SUCC;
    }

    /* 检测运动 */
    bool has_motion = sleep_detect_motion(data, len);

    if (has_motion) {
        g_sleep.last_motion_time = (uint32_t)uapi_tcxo_get_ms();
        g_sleep.consecutive_no_motion_sec = 0;
    } else {
        g_sleep.consecutive_no_motion_sec +=
            (len * 1000 / 50); /* 假设50Hz采样 */
    }

    /* TODO: 自动入睡/清醒检测
     * 若 enable_auto_detect:
     *   - 连续30分钟无运动 + HR下降: 自动开始睡眠追踪
     *   - 连续5分钟有运动 + HR升高: 自动停止睡眠追踪
     */

    /* TODO: 累积epoch数据
     * 每30秒汇总一次数据，进行分期决策
     */

    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_feed_hr_data(uint8_t bpm, uint32_t timestamp_ms)
{
    (void)bpm;
    (void)timestamp_ms;

    /* TODO: 使用心率数据辅助睡眠分期
     * 1. 记录每个epoch的平均HR
     * 2. 计算HR变异性
     * 3. 用于DEEP/REM/WAKE区分
     */

    return ERRCODE_SUCC;
}

errcode_t uapi_sleep_is_sleeping(bool *is_sleeping)
{
    if (is_sleeping == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    *is_sleeping = g_sleep.is_sleeping;
    return ERRCODE_SUCC;
}
