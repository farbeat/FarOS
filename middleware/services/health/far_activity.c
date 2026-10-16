/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: Activity recognition algorithm implementation skeleton. \n
 *              Includes pedometer, activity type classification, \n
 *              cadence, and distance tracking. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#include "far_activity.h"
#include "far_sensor.h"
#include "debug_print.h"
#include "securec.h"
#include "tcxo.h"

/*
 * ============================================================
 * 运动识别算法骨架实现说明
 * Activity Recognition Skeleton Implementation Notes
 * ============================================================
 *
 * 本文件包含计步、活动类型识别、步频和距离追踪的算法框架。
 * 核心算法逻辑以注释形式说明，需后续填充具体实现。
 *
 * 计步算法参考方案：
 * 1. 峰值检测法（Peak Detection）
 *    - 对加速度计三轴数据进行合成：acc_mag = sqrt(ax² + ay² + az²)
 *    - 应用低通滤波器消除高频噪声（截止频率~3Hz）
 *    - 检测acc_mag的局部峰值，峰值间隔在0.3s-1.5s之间视为有效步态
 *    - 峰值幅度超过动态阈值（基于近期平均值的自适应阈值）
 * 2. 自适应阈值更新
 *    - 阈值 = 近期峰值的移动平均值 × 系数（通常0.6-0.8）
 *    - 每50步更新一次阈值
 *
 * 活动类型识别参考方案（基于IMU）：
 * 1. 特征提取（每3秒窗口）
 *    - 时域：均值、方差、最大值、最小值、过零率
 *    - 频域：FFT主频率、能量分布
 * 2. 分类器（轻量级）
 *    - 决策树 / 阈值规则（适合资源受限设备）
 *    - 或预训练的SVM/神经网络模型（需更多内存）
 * 3. 活动类型映射
 *    - 静止：acc_var < 阈值，无峰值
 *    - 步行：acc_var 中等，峰值频率 1-2Hz
 *    - 跑步：acc_var 高，峰值频率 2-3Hz
 *    - 其他：基于频谱特征匹配
 *
 * 步频算法：
 * - 基于峰值检测的时间间隔计算SPM（Steps Per Minute）
 * - SPM = 60 / 平均步态周期(s)
 *
 * 距离算法：
 * - 步行距离 = 步数 × 步长
 * - 步长估算：基于身高和步频的回归模型
 *   step_length_cm = height_cm × (0.415 + 0.002 × (cadence_spm - 100))
 */

#define ACTIVITY_WINDOW_MS          3000    /* 3秒分析窗口 */
#define STEP_MIN_INTERVAL_MS        300     /* 最小步态间隔 300ms */
#define STEP_MAX_INTERVAL_MS        1500    /* 最大步态间隔 1500ms */
#define ACTIVITY_CONFIDENCE_MIN     50      /* 最小置信度阈值 */
#define DEFAULT_STRIDE_FACTOR       0.415f  /* 默认步长系数 */
#define ACCEL_MAG_BUF_SIZE          128     /* 加速度幅值缓冲区 */

/* 活动识别状态 */
typedef struct {
    bool enabled;
    bool initialized;
    far_activity_config_t config;
    far_activity_type_t current_type;
    uint8_t confidence;
    uint32_t type_start_time;

    /* 计步状态 */
    uint32_t total_steps;
    uint32_t steps_today;
    uint32_t last_step_time;
    uint32_t step_threshold_dynamic;

    /* 步频状态 */
    uint16_t current_cadence;
    uint16_t max_cadence;
    uint32_t cadence_window_start;
    uint32_t cadence_window_steps;

    /* 距离状态 */
    uint32_t total_distance_m;
    uint32_t distance_today_m;
    float avg_stride_cm;

    /* 回调 */
    far_activity_change_callback_t callback;
    void *callback_user_data;

    /* 加速度幅值环形缓冲区（用于峰值检测） */
    int32_t accel_mag_buf[ACCEL_MAG_BUF_SIZE];
    uint16_t buf_write_idx;
    uint16_t buf_count;
} far_activity_state_t;

static far_activity_state_t g_activity = {0};
static far_user_profile_t g_user_profile = {0};

/*
 * 计算加速度合成幅值
 * Calculate acceleration magnitude from 3-axis data
 */
static int32_t activity_calc_accel_mag(const far_sensor_imu_data_t *data)
{
    /* acc_mag = sqrt(ax² + ay² + az²) in mg
     * 使用整数近似：sqrt(x) ≈ isqrt(x) 或查表法
     */
    int64_t ax = (int64_t)data->ax;
    int64_t ay = (int64_t)data->ay;
    int64_t az = (int64_t)data->az;
    int64_t sq = ax * ax + ay * ay + az * az;

    /* 简化整数平方根（牛顿迭代法） */
    int64_t root = sq;
    if (root > 0) {
        for (int i = 0; i < 8; i++) {
            root = (root + sq / root) / 2;
        }
    }
    return (int32_t)root;
}

/*
 * 自适应步态检测
 * Adaptive step detection using peak detection
 */
static bool activity_detect_step(int32_t accel_mag)
{
    /* TODO: 实现峰值检测算法
     *
     * 1. 将新样本加入环形缓冲区
     * 2. 检查缓冲区中的局部峰值
     * 3. 验证峰值条件：
     *    - 幅度 > 动态阈值（基于近期峰值的自适应阈值）
     *    - 与前一步的时间间隔在 [300ms, 1500ms] 范围内
     * 4. 若满足条件则计为一步
     * 5. 更新动态阈值：threshold = 0.7 × 近期峰值移动平均
     */

    /* 骨架：简单阈值检测 */
    uint32_t now = (uint32_t)uapi_tcxo_get_ms();
    if ((accel_mag > (int32_t)g_activity.config.step_threshold) &&
        (now - g_activity.last_step_time > STEP_MIN_INTERVAL_MS)) {
        g_activity.last_step_time = now;
        return true;
    }
    return false;
}

/*
 * 活动类型识别
 * Activity type classification
 */
static far_activity_type_t activity_classify_type(const far_sensor_imu_data_t *data,
                                                   uint32_t len, uint8_t *confidence)
{
    /* TODO: 实现活动分类算法
     *
     * 1. 特征提取（3秒窗口）
     *    - 合成加速度幅值均值、方差
     *    - 主频率（峰值检测频率）
     *    - 陀螺仪角速度方差
     *
     * 2. 基于规则的分类（轻量级实现）
     *    if acc_var < 1000 mg²: -> REST (久坐/静止)
     *    else if 1Hz < peak_freq < 2.2Hz AND acc_var < 5000: -> WALK
     *    else if 2Hz < peak_freq < 3.5Hz AND acc_var > 5000: -> RUN
     *    else if gyro_var > threshold AND acc_var < 3000: -> CYCLE
     *    else: -> UNKNOWN
     *
     * 3. 置信度计算
     *    - 根据特征与类别中心的距离计算
     *    - 距离越近置信度越高
     */

    (void)data;
    (void)len;

    *confidence = 70; /* 骨架默认值 */
    return FAR_ACTIVITY_TYPE_REST; /* 骨架默认返回静止 */
}

/*
 * 计算步长（cm）
 * Calculate stride length based on user height and cadence
 */
static float activity_calc_stride_length(uint16_t cadence_spm)
{
    /* 基于身高和步频的回归模型
     * step_length_cm = height_cm × (0.415 + 0.002 × (cadence_spm - 100))
     * 参考：Typical walking stride = 0.415 × height
     *       Running stride increases with cadence
     */
    float base = DEFAULT_STRIDE_FACTOR;
    float adjustment = 0.002f * (float)((int16_t)cadence_spm - 100);
    float factor = base + adjustment;

    if (factor < 0.3f) {
        factor = 0.3f;
    } else if (factor > 0.8f) {
        factor = 0.8f;
    }

    return (float)g_user_profile.height_cm * factor;
}

/* ============================================================
 * Public APIs
 * ============================================================ */

errcode_t uapi_activity_init(const far_activity_config_t *config)
{
    if (config == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    if (g_activity.initialized) {
        return ERRCODE_SUCC;
    }

    (void)memcpy_s(&g_activity.config, sizeof(far_activity_config_t),
                   config, sizeof(far_activity_config_t));

    g_activity.initialized = true;
    g_activity.enabled = false;
    g_activity.current_type = FAR_ACTIVITY_TYPE_UNKNOWN;
    g_activity.confidence = 0;
    g_activity.total_steps = 0;
    g_activity.steps_today = 0;
    g_activity.last_step_time = 0;
    g_activity.step_threshold_dynamic = config->step_threshold;
    g_activity.current_cadence = 0;
    g_activity.max_cadence = 0;
    g_activity.total_distance_m = 0;
    g_activity.distance_today_m = 0;
    g_activity.avg_stride_cm = 0;
    g_activity.callback = NULL;
    g_activity.callback_user_data = NULL;
    g_activity.buf_write_idx = 0;
    g_activity.buf_count = 0;

    /* 默认用户档案（未设置时使用） */
    g_user_profile.gender = 1;
    g_user_profile.age = 30;
    g_user_profile.height_cm = 170;
    g_user_profile.weight_kg = 700; /* 70.0 kg × 10 */
    g_user_profile.max_hr = 0; /* 自动计算 */
    g_user_profile.rest_hr = 60;

    PRINT("[far_activity] Activity recognition init, threshold=%d\r\n",
          config->step_threshold);
    return ERRCODE_SUCC;
}

errcode_t uapi_activity_deinit(void)
{
    if (!g_activity.initialized) {
        return ERRCODE_SUCC;
    }

    (void)memset_s(&g_activity, sizeof(g_activity), 0, sizeof(g_activity));
    return ERRCODE_SUCC;
}

errcode_t uapi_activity_enable(void)
{
    if (!g_activity.initialized) {
        return ERRCODE_FAIL;
    }
    g_activity.enabled = true;
    PRINT("[far_activity] Activity recognition enabled\r\n");
    return ERRCODE_SUCC;
}

errcode_t uapi_activity_disable(void)
{
    if (!g_activity.initialized) {
        return ERRCODE_FAIL;
    }
    g_activity.enabled = false;
    PRINT("[far_activity] Activity recognition disabled\r\n");
    return ERRCODE_SUCC;
}

errcode_t uapi_activity_get_step_count(far_step_info_t *info)
{
    if (info == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    info->total_steps = g_activity.total_steps;
    info->steps_today = g_activity.steps_today;
    info->last_update_time = g_activity.last_step_time;
    return ERRCODE_SUCC;
}

errcode_t uapi_activity_reset_step_count(void)
{
    g_activity.total_steps = 0;
    g_activity.steps_today = 0;
    return ERRCODE_SUCC;
}

errcode_t uapi_activity_get_type(far_activity_result_t *result)
{
    if (result == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    result->type = g_activity.current_type;
    result->confidence = g_activity.confidence;
    result->start_time = g_activity.type_start_time;
    if (g_activity.type_start_time > 0) {
        result->duration_sec = ((uint32_t)uapi_tcxo_get_ms() - g_activity.type_start_time) / 1000;
    } else {
        result->duration_sec = 0;
    }
    return ERRCODE_SUCC;
}

errcode_t uapi_activity_get_cadence(far_cadence_info_t *info)
{
    if (info == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    info->current_spm = g_activity.current_cadence;
    info->avg_spm = g_activity.current_cadence; /* TODO: 计算平均值 */
    info->max_spm = g_activity.max_cadence;
    return ERRCODE_SUCC;
}

errcode_t uapi_activity_get_distance(far_distance_info_t *info)
{
    if (info == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    info->total_distance_m = g_activity.total_distance_m;
    info->distance_today_m = g_activity.distance_today_m;
    info->avg_stride_length_cm = g_activity.avg_stride_cm;
    return ERRCODE_SUCC;
}

errcode_t uapi_activity_register_callback(far_activity_change_callback_t callback, void *user_data)
{
    if (callback == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    g_activity.callback = callback;
    g_activity.callback_user_data = user_data;
    return ERRCODE_SUCC;
}

errcode_t uapi_activity_unregister_callback(void)
{
    g_activity.callback = NULL;
    g_activity.callback_user_data = NULL;
    return ERRCODE_SUCC;
}

errcode_t uapi_activity_feed_imu_data(const far_sensor_imu_data_t *data, uint32_t len)
{
    if ((data == NULL) || (len == 0)) {
        return ERRCODE_INVALID_PARAM;
    }

    if (!g_activity.enabled) {
        return ERRCODE_SUCC;
    }

    /* 处理每个IMU样本 */
    for (uint32_t i = 0; i < len; i++) {
        int32_t accel_mag = activity_calc_accel_mag(&data[i]);

        /* 计步检测 */
        if (g_activity.config.enable_step_count) {
            if (activity_detect_step(accel_mag)) {
                g_activity.total_steps++;
                g_activity.steps_today++;

                /* 更新步频窗口 */
                g_activity.cadence_window_steps++;
            }
        }

        /* TODO: 活动类型识别（每3秒窗口处理一次） */
        if (g_activity.config.enable_activity_type) {
            /* 累积样本到窗口缓冲区，窗口满时进行分类 */
        }
    }

    /* TODO: 步频计算（每10秒更新一次） */
    if (g_activity.config.enable_cadence) {
        /* SPM = 60 / 平均步态周期 或 10秒内步数 × 6 */
    }

    /* TODO: 距离计算 */
    if (g_activity.config.enable_distance) {
        /* distance += steps × stride_length / 100 */
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_activity_get_daily_summary(uint32_t *steps, uint32_t *distance_m, uint16_t *active_min)
{
    if ((steps == NULL) || (distance_m == NULL) || (active_min == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

    *steps = g_activity.steps_today;
    *distance_m = g_activity.distance_today_m;
    *active_min = 0; /* TODO: 计算今日活跃分钟数 */
    return ERRCODE_SUCC;
}
