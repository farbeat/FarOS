/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: Provides physiological indicator analysis API for FarOS Health Service. \n
 *              Includes heart rate, HRV, calorie, VO2max, training load, \n
 *              recovery time, stress, and body battery. \n
 *              Designed with reference to Firstbeat core concepts. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#ifndef FAR_PHYSIO_H
#define FAR_PHYSIO_H

#include <stdint.h>
#include <stdbool.h>
#include "errcode.h"
#include "far_health_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup health_physio Physiological Indicators
 * @ingroup  health_service
 * @brief    生理指标分析模块，参考Firstbeat核心概念框架设计。
 *           提供心率监测、HRV分析、卡路里计算、VO2max估算、训练负荷、
 *           恢复时间、压力指数及身体电量等高级生理参数。
 *
 * @if Eng
 * Physiological indicator analysis module designed with reference to Firstbeat
 * core concepts. Provides heart rate monitoring, HRV analysis, calorie
 * calculation, VO2max estimation, training load, recovery time, stress index,
 * and body battery level.
 * @endif
 * @{
 */

/**
 * @if Eng
 * @brief  Heart rate zone time distribution structure.
 * @else
 * @brief  心率区间时间分布结构体。
 * @endif
 */
typedef struct {
    uint16_t zone_time_sec[FAR_HEALTH_HR_ZONE_COUNT];  /*!< @if Eng Time spent in each zone (seconds).
                                                            @else   各区间停留时间（秒）。 @endif */
    uint8_t  zone_percent[FAR_HEALTH_HR_ZONE_COUNT];   /*!< @if Eng Percentage of time in each zone.
                                                            @else   各区间时间占比（%）。 @endif */
} far_hr_zone_distribution_t;

/**
 * @if Eng
 * @brief  Heart rate information structure.
 * @else
 * @brief  心率信息结构体。
 * @endif
 */
typedef struct {
    uint8_t  current_bpm;           /*!< @if Eng Current heart rate in BPM.
                                         @else   当前心率，单位：次/分钟。 @endif */
    uint8_t  resting_bpm;           /*!< @if Eng Measured resting heart rate.
                                         @else   实测静息心率。 @endif */
    uint8_t  max_bpm;               /*!< @if Eng Maximum heart rate (220-age or user-defined).
                                         @else   最大心率（220-年龄或用户自定义）。 @endif */
    far_hr_zone_t current_zone;     /*!< @if Eng Current heart rate zone.
                                         @else   当前心率区间。 @endif */
    uint8_t  signal_quality;        /*!< @if Eng PPG signal quality 0-100%.
                                         @else   PPG信号质量 0-100%。 @endif */
    uint32_t last_update_ms;        /*!< @if Eng Last update timestamp.
                                         @else   最后更新时间戳。 @endif */
} far_hr_info_t;

/**
 * @if Eng
 * @brief  HRV (Heart Rate Variability) analysis result structure.
 * @note   Reference Firstbeat HRV analysis methodology.
 * @else
 * @brief  HRV（心率变异性）分析结果结构体。
 * @note   参考Firstbeat HRV分析方法。
 * @endif
 */
typedef struct {
    uint16_t sdnn_ms;           /*!< @if Eng SDNN (Standard Deviation of NN intervals), unit: ms.
                                     @else   SDNN（正常RR间期标准差），单位：ms。 @endif */
    uint16_t rmssd_ms;          /*!< @if Eng RMSSD (Root Mean Square of Successive Differences), unit: ms.
                                     @else   RMSSD（相邻RR间期差值的均方根），单位：ms。 @endif */
    uint16_t lf_power;          /*!< @if Eng LF (Low Frequency) power 0.04-0.15 Hz, unit: ms^2.
                                     @else   LF（低频）功率 0.04-0.15 Hz，单位：ms²。 @endif */
    uint16_t hf_power;          /*!< @if Eng HF (High Frequency) power 0.15-0.4 Hz, unit: ms^2.
                                     @else   HF（高频）功率 0.15-0.4 Hz，单位：ms²。 @endif */
    uint16_t lf_hf_ratio;       /*!< @if Eng LF/HF ratio x100 (for integer representation).
                                     @else   LF/HF比值 x100（整数表示）。 @endif */
    uint8_t  hrv_score;         /*!< @if Eng HRV score 0-100 (higher = better autonomic balance).
                                     @else   HRV评分 0-100（越高=自主神经平衡越好）。 @endif */
    uint32_t analysis_duration_sec; /*!< @if Eng Analysis window duration in seconds.
                                         @else   分析窗口时长，单位：秒。 @endif */
} far_hrv_result_t;

/**
 * @if Eng
 * @brief  Calorie consumption information structure.
 * @note   Based on Firstbeat METs (Metabolic Equivalent of Task) model.
 * @else
 * @brief  卡路里消耗信息结构体。
 * @note   基于Firstbeat METs（代谢当量）模型。
 * @endif
 */
typedef struct {
    uint32_t total_kcal;        /*!< @if Eng Total calories consumed (BMR + active), unit: kcal.
                                     @else   总消耗卡路里（基础代谢+主动运动），单位：千卡。 @endif */
    uint32_t active_kcal;       /*!< @if Eng Active exercise calories, unit: kcal.
                                     @else   主动运动消耗卡路里，单位：千卡。 @endif */
    uint32_t bmr_kcal;          /*!< @if Eng Basal Metabolic Rate calories, unit: kcal.
                                     @else   基础代谢消耗卡路里，单位：千卡。 @endif */
    uint16_t current_met;       /*!< @if Eng Current MET value x10 (e.g., 35 = 3.5 METs).
                                     @else   当前MET值 x10（例如35=3.5 METs）。 @endif */
    uint32_t last_update_ms;    /*!< @if Eng Last update timestamp.
                                     @else   最后更新时间戳。 @endif */
} far_calorie_info_t;

/**
 * @if Eng
 * @brief  VO2max estimation result structure.
 * @note   Reference Firstbeat VO2max estimation methodology using heart rate
 *         and speed/power data during running/cycling activities.
 * @else
 * @brief  VO2max估算结果结构体。
 * @note   参考Firstbeat VO2max估算方法，利用跑步/骑行过程中的心率和速度/功率数据。
 * @endif
 */
typedef struct {
    uint8_t  vo2max_ml_kg_min;  /*!< @if Eng VO2max value, unit: ml/kg/min.
                                     @else   VO2max值，单位：ml/kg/min。 @endif */
    uint8_t  confidence;        /*!< @if Estimation confidence 0-100%.
                                     @else   估算置信度 0-100%。 @endif */
    uint32_t last_test_time;    /*!< @if Eng Timestamp of last estimation.
                                     @else   上次估算时间戳。 @endif */
    uint8_t  fitness_level;     /*!< @if Eng Fitness level 1-10 (1=poor, 10=elite).
                                     @else   体能等级 1-10（1=差，10=精英）。 @endif */
} far_vo2max_result_t;

/**
 * @if Eng
 * @brief  Training load information structure.
 * @note   Based on Firstbeat EPOC (Excess Post-exercise Oxygen Consumption)
 *         accumulation model. Training load is the accumulated EPOC over
 *         a training session.
 * @else
 * @brief  训练负荷信息结构体。
 * @note   基于Firstbeat EPOC（运动后过量氧耗）累积模型。
 *         训练负荷为单次训练过程中的EPOC累积量。
 * @endif
 */
typedef struct {
    uint16_t current_epoc;      /*!< @if Eng Current EPOC value, unit: ml/kg.
                                     @else   当前EPOC值，单位：ml/kg。 @endif */
    uint32_t session_load;      /*!< @if Eng Accumulated training load this session.
                                     @else   本次训练累积负荷。 @endif */
    uint32_t weekly_load;       /*!< @if Eng Accumulated training load this week (7 days).
                                     @else   本周（7天）累积训练负荷。 @endif */
    uint8_t  load_zone;         /*!< @if Eng Load zone: 0=low, 1=optimal, 2=high, 3=overreaching.
                                     @else   负荷区间：0=低，1=最佳，2=高，3=过度。 @endif */
} far_training_load_t;

/**
 * @if Eng
 * @brief  Recovery time recommendation structure.
 * @note   Based on Firstbeat recovery time model using EPOC decay
 *         and HRV recovery patterns.
 * @else
 * @brief  恢复时间建议结构体。
 * @note   基于Firstbeat恢复时间模型，利用EPOC衰减和HRV恢复模式。
 * @endif
 */
typedef struct {
    uint16_t recovery_hours;    /*!< @if Eng Recommended recovery time in hours.
                                     @else   建议恢复时间，单位：小时。 @endif */
    uint16_t recovery_minutes;  /*!< @if Eng Additional minutes.
                                     @else   额外分钟数。 @endif */
    uint8_t  recovery_score;    /*!< @if Eng Recovery status 0-100 (100=fully recovered).
                                     @else   恢复状态 0-100（100=完全恢复）。 @endif */
    bool     is_recovered;      /*!< @if Eng true if fully recovered.
                                     @else   true表示已完全恢复。 @endif */
} far_recovery_info_t;

/**
 * @if Eng
 * @brief  Stress index information structure.
 * @note   Based on Firstbeat HRV-derived stress analysis using
 *         continuous LF/HF ratio monitoring during daytime.
 * @else
 * @brief  压力指数信息结构体。
 * @note   基于Firstbeat HRV衍生压力分析，利用日间连续LF/HF比值监测。
 * @endif
 */
typedef struct {
    uint8_t  current_stress;    /*!< @if Eng Current stress level 0-100 (0=relaxed, 100=very stressed).
                                     @else   当前压力水平 0-100（0=放松，100=压力极大）。 @endif */
    uint8_t  avg_stress;        /*!< @if Eng Average stress level today.
                                     @else   今日平均压力水平。 @endif */
    uint16_t stress_duration_min; /*!< @if Eng Duration of elevated stress today (minutes).
                                       @else   今日高压力持续时间（分钟）。 @endif */
    uint16_t rest_duration_min;   /*!< @if Eng Duration of rest/recovery today (minutes).
                                       @else   今日休息/恢复持续时间（分钟）。 @endif */
} far_stress_info_t;

/**
 * @if Eng
 * @brief  Body Battery (energy reserve) information structure.
 * @note   Based on Firstbeat Body Battery concept combining HRV-derived
 *         stress/recovery analysis with activity and sleep data to estimate
 *         the body's energy reserves throughout the day.
 * @else
 * @brief  身体电量（能量储备）信息结构体。
 * @note   基于Firstbeat Body Battery概念，结合HRV衍生的压力/恢复分析
 *         以及活动和睡眠数据，估算全天身体能量储备。
 * @endif
 */
typedef struct {
    uint8_t  current_level;     /*!< @if Eng Current body battery level 0-100.
                                     @else   当前身体电量 0-100。 @endif */
    uint8_t  highest_today;     /*!< @if Eng Highest level today.
                                     @else   今日最高电量。 @endif */
    uint8_t  lowest_today;      /*!< @if Eng Lowest level today.
                                     @else   今日最低电量。 @endif */
    bool     is_charging;       /*!< @if Eng true if body battery is increasing (rest/sleep).
                                     @else   true表示身体电量在增加（休息/睡眠中）。 @endif */
    int8_t   charge_rate;       /*!< @if Eng Charge/discharge rate per hour (+/-).
                                     @else   每小时充放电速率（+/-）。 @endif */
} far_body_battery_t;

/**
 * @if Eng
 * @brief  Initialize the physiological indicator module.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_FAIL   Initialization failed.
 * @else
 * @brief  初始化生理指标分析模块。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_FAIL   初始化失败。
 * @endif
 */
errcode_t uapi_physio_init(void);

/**
 * @if Eng
 * @brief  Deinitialize the physiological indicator module.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  反初始化生理指标分析模块。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_physio_deinit(void);

/* ===== Heart Rate APIs ===== */

/**
 * @if Eng
 * @brief  Initialize heart rate monitoring.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_FAIL   Initialization failed.
 * @else
 * @brief  初始化心率监测。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_FAIL   初始化失败。
 * @endif
 */
errcode_t uapi_physio_hr_init(void);

/**
 * @if Eng
 * @brief  Deinitialize heart rate monitoring.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  反初始化心率监测。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_physio_hr_deinit(void);

/**
 * @if Eng
 * @brief  Get current heart rate information.
 * @param  [out] info     Pointer to store HR info.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  获取当前心率信息。
 * @param  [out] info     存储心率信息的指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_physio_hr_get_current(far_hr_info_t *info);

/**
 * @if Eng
 * @brief  Get resting heart rate (requires minimum 7 days of data).
 * @param  [out] resting_bpm  Pointer to store resting HR.
 * @retval ERRCODE_SUCC       Success.
 * @retval ERRCODE_FAIL       Insufficient data (need 7+ days).
 * @else
 * @brief  获取静息心率（需要至少7天数据）。
 * @param  [out] resting_bpm  存储静息心率的指针。
 * @retval ERRCODE_SUCC       成功。
 * @retval ERRCODE_FAIL       数据不足（需要7天以上）。
 * @endif
 */
errcode_t uapi_physio_hr_get_resting(uint8_t *resting_bpm);

/**
 * @if Eng
 * @brief  Get heart rate zone distribution for current session.
 * @param  [out] distribution Pointer to store zone distribution.
 * @retval ERRCODE_SUCC       Success.
 * @else
 * @brief  获取当前训练心率区间分布。
 * @param  [out] distribution 存储区间分布的指针。
 * @retval ERRCODE_SUCC       成功。
 * @endif
 */
errcode_t uapi_physio_hr_get_zone_distribution(far_hr_zone_distribution_t *distribution);

/* ===== HRV APIs ===== */

/**
 * @if Eng
 * @brief  Perform HRV analysis on recent RR interval data.
 * @note   Analysis window should be at least 5 minutes for reliable results.
 * @param  [out] result   Pointer to store HRV result.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_FAIL   Insufficient data for analysis.
 * @else
 * @brief  对近期RR间期数据进行HRV分析。
 * @note   分析窗口应至少5分钟以获得可靠结果。
 * @param  [out] result   存储HRV结果的指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_FAIL   数据不足，无法分析。
 * @endif
 */
errcode_t uapi_physio_hrv_analyze(far_hrv_result_t *result);

/**
 * @if Eng
 * @brief  Feed RR interval data for HRV analysis.
 * @param  [in] rr_intervals_ms Array of RR intervals in milliseconds.
 * @param  [in] len               Number of RR intervals.
 * @retval ERRCODE_SUCC           Success.
 * @else
 * @brief  投喂RR间期数据进行HRV分析。
 * @param  [in] rr_intervals_ms RR间期数组，单位：毫秒。
 * @param  [in] len               RR间期数量。
 * @retval ERRCODE_SUCC           成功。
 * @endif
 */
errcode_t uapi_physio_hrv_feed_rr_data(const uint16_t *rr_intervals_ms, uint32_t len);

/* ===== Calorie APIs ===== */

/**
 * @if Eng
 * @brief  Get calorie consumption information.
 * @param  [out] info     Pointer to store calorie info.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  获取卡路里消耗信息。
 * @param  [out] info     存储卡路里信息的指针。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_physio_calorie_get_info(far_calorie_info_t *info);

/**
 * @if Eng
 * @brief  Get total calories consumed today.
 * @param  [out] total_kcal    Total calories.
 * @param  [out] active_kcal   Active exercise calories.
 * @retval ERRCODE_SUCC        Success.
 * @else
 * @brief  获取今日总消耗卡路里。
 * @param  [out] total_kcal    总卡路里。
 * @param  [out] active_kcal   主动运动卡路里。
 * @retval ERRCODE_SUCC        成功。
 * @endif
 */
errcode_t uapi_physio_calorie_get_today(uint32_t *total_kcal, uint32_t *active_kcal);

/* ===== VO2max APIs ===== */

/**
 * @if Eng
 * @brief  Get current VO2max estimate.
 * @param  [out] result   Pointer to store VO2max result.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_FAIL   No valid estimation available.
 * @else
 * @brief  获取当前VO2max估算值。
 * @param  [out] result   存储VO2max结果的指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_FAIL   无有效估算值。
 * @endif
 */
errcode_t uapi_physio_vo2max_get(far_vo2max_result_t *result);

/**
 * @if Eng
 * @brief  Trigger a VO2max estimation session.
 * @note   Requires active running or cycling activity with HR data.
 *         Typically needs 15+ minutes of sustained effort.
 * @param  [in] activity_type Activity type (run/cycle).
 * @retval ERRCODE_SUCC       Estimation started.
 * @retval ERRCODE_NOT_SUPPORT Activity type not supported for VO2max.
 * @else
 * @brief  触发VO2max估算会话。
 * @note   需要正在进行跑步或骑行活动且有心率数据。
 *         通常需要15分钟以上的持续运动。
 * @param  [in] activity_type 活动类型（跑步/骑行）。
 * @retval ERRCODE_SUCC       估算已开始。
 * @retval ERRCODE_NOT_SUPPORT 该活动类型不支持VO2max估算。
 * @endif
 */
errcode_t uapi_physio_vo2max_estimate(far_activity_type_t activity_type);

/* ===== Training Load APIs ===== */

/**
 * @if Eng
 * @brief  Get current training load information.
 * @param  [out] load     Pointer to store training load info.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  获取当前训练负荷信息。
 * @param  [out] load     存储训练负荷信息的指针。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_physio_training_load_get(far_training_load_t *load);

/**
 * @if Eng
 * @brief  Reset training load accumulation for current session.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  重置当前训练负荷累积。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_physio_training_load_reset(void);

/* ===== Recovery Time APIs ===== */

/**
 * @if Eng
 * @brief  Get recommended recovery time.
 * @param  [out] recovery Pointer to store recovery info.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  获取建议恢复时间。
 * @param  [out] recovery 存储恢复信息的指针。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_physio_recovery_time_get(far_recovery_info_t *recovery);

/* ===== Stress APIs ===== */

/**
 * @if Eng
 * @brief  Get current stress level information.
 * @param  [out] stress   Pointer to store stress info.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  获取当前压力水平信息。
 * @param  [out] stress   存储压力信息的指针。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_physio_stress_get(far_stress_info_t *stress);

/**
 * @if Eng
 * @brief  Enable/disable continuous stress monitoring.
 * @param  [in] enable    true=enable, false=disable.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  使能/禁用连续压力监测。
 * @param  [in] enable    true=使能，false=禁用。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_physio_stress_enable(bool enable);

/* ===== Body Battery APIs ===== */

/**
 * @if Eng
 * @brief  Get current body battery level.
 * @param  [out] battery  Pointer to store body battery info.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  获取当前身体电量。
 * @param  [out] battery  存储身体电量信息的指针。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_physio_body_battery_get(far_body_battery_t *battery);

/**
 * @if Eng
 * @brief  Enable/disable body battery tracking.
 * @param  [in] enable    true=enable, false=disable.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  使能/禁用身体电量追踪。
 * @param  [in] enable    true=使能，false=禁用。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_physio_body_battery_enable(bool enable);

/**
 * @if Eng
 * @brief  Feed heart rate data to physiological algorithms.
 * @note   Called internally by sensor callback. Can be used for testing.
 * @param  [in] bpm       Heart rate in BPM.
 * @param  [in] timestamp_ms Timestamp in milliseconds.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  向生理算法投喂心率数据。
 * @note   由传感器回调内部调用。可用于测试。
 * @param  [in] bpm       心率，单位：次/分钟。
 * @param  [in] timestamp_ms 时间戳，单位：毫秒。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_physio_feed_hr_data(uint8_t bpm, uint32_t timestamp_ms);

/**
 * @if Eng
 * @brief  Feed activity data to calorie calculation algorithm.
 * @param  [in] activity_type Current activity type.
 * @param  [in] intensity     Activity intensity 0-100.
 * @param  [in] duration_sec  Duration in seconds.
 * @retval ERRCODE_SUCC       Success.
 * @else
 * @brief  向卡路里计算算法投喂活动数据。
 * @param  [in] activity_type 当前活动类型。
 * @param  [in] intensity     活动强度 0-100。
 * @param  [in] duration_sec  持续时间，单位：秒。
 * @retval ERRCODE_SUCC       成功。
 * @endif
 */
errcode_t uapi_physio_feed_activity_data(far_activity_type_t activity_type,
                                          uint8_t intensity, uint32_t duration_sec);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* FAR_PHYSIO_H */
