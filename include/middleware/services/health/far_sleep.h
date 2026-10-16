/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: Provides sleep analysis API for FarOS Health Service. \n
 *              Includes sleep stage detection, sleep scoring, and \n
 *              sleep quality assessment. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#ifndef FAR_SLEEP_H
#define FAR_SLEEP_H

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
 * @defgroup health_sleep Sleep Analysis
 * @ingroup  health_service
 * @brief    睡眠分析模块，提供睡眠分期检测、睡眠评分及睡眠质量评估功能。
 *           基于IMU运动检测和PPG心率/HRV数据实现睡眠分期算法。
 *
 * @if Eng
 * Sleep analysis module providing sleep stage detection, sleep scoring,
 * and sleep quality assessment based on IMU motion detection and PPG
 * heart rate/HRV data.
 * @endif
 * @{
 */

/**
 * @if Eng
 * @brief  Sleep stage segment structure.
 * @else
 * @brief  睡眠分期段结构体。
 * @endif
 */
typedef struct {
    far_sleep_stage_t stage;        /*!< @if Eng Sleep stage type.
                                         @else   睡眠分期类型。 @endif */
    uint16_t          duration_min; /*!< @if Eng Duration of this stage segment in minutes.
                                         @else   该分期段持续时间，单位：分钟。 @endif */
    uint32_t          start_time;   /*!< @if Eng Start timestamp (ms).
                                         @else   开始时间戳（ms）。 @endif */
} far_sleep_segment_t;

/**
 * @if Eng
 * @brief  Sleep record structure.
 * @else
 * @brief  睡眠记录结构体。
 * @endif
 */
typedef struct {
    uint32_t              sleep_start_time;     /*!< @if Eng Sleep session start timestamp (ms).
                                                     @else   睡眠开始时间戳（ms）。 @endif */
    uint32_t              sleep_end_time;       /*!< @if Eng Sleep session end timestamp (ms).
                                                     @else   睡眠结束时间戳（ms）。 @endif */
    uint16_t              total_sleep_min;      /*!< @if Eng Total sleep duration in minutes.
                                                     @else   总睡眠时长，单位：分钟。 @endif */
    uint16_t              deep_sleep_min;       /*!< @if Eng Deep sleep duration in minutes.
                                                     @else   深睡时长，单位：分钟。 @endif */
    uint16_t              light_sleep_min;      /*!< @if Eng Light sleep duration in minutes.
                                                     @else   浅睡时长，单位：分钟。 @endif */
    uint16_t              rem_sleep_min;        /*!< @if Eng REM sleep duration in minutes.
                                                     @else   REM睡眠时长，单位：分钟。 @endif */
    uint16_t              awake_min;            /*!< @if Eng Awake duration in minutes.
                                                     @else   清醒时长，单位：分钟。 @endif */
    uint8_t               awake_count;          /*!< @if Eng Number of awakenings.
                                                     @else   觉醒次数。 @endif */
    uint8_t               sleep_score;          /*!< @if Eng Sleep quality score 0-100.
                                                     @else   睡眠质量评分 0-100。 @endif */
    far_sleep_segment_t   segments[FAR_HEALTH_MAX_SLEEP_SEGMENTS]; /*!< @if Eng Sleep stage segments.
                                                                        @else   睡眠分期段数组。 @endif */
    uint8_t               segment_count;        /*!< @if Eng Number of valid segments.
                                                     @else   有效段数量。 @endif */
} far_sleep_record_t;

/**
 * @if Eng
 * @brief  Sleep analysis configuration structure.
 * @else
 * @brief  睡眠分析配置结构体。
 * @endif
 */
typedef struct {
    bool     enable_auto_detect;    /*!< @if Eng Enable automatic sleep detection.
                                         @else   使能自动睡眠检测。 @endif */
    uint8_t  bedtime_hour;          /*!< @if Eng Expected bedtime hour (0-23), 255=auto.
                                         @else   预期入睡时间（0-23），255=自动。 @endif */
    uint8_t  wake_time_hour;        /*!< @if Eng Expected wake time hour (0-23), 255=auto.
                                         @else   预期起床时间（0-23），255=自动。 @endif */
    uint16_t min_sleep_duration_min; /*!< @if Eng Minimum sleep duration to record (minutes).
                                          @else   最小记录睡眠时长（分钟）。 @endif */
    bool     enable_hrv_analysis;   /*!< @if Eng Enable HRV-based sleep quality analysis.
                                         @else   使能基于HRV的睡眠质量分析。 @endif */
} far_sleep_config_t;

/**
 * @if Eng
 * @brief  Sleep quality factors structure (for detailed analysis).
 * @else
 * @brief  睡眠质量因素结构体（用于详细分析）。
 * @endif
 */
typedef struct {
    uint8_t duration_score;     /*!< @if Eng Score for total sleep duration (0-100).
                                     @else   睡眠时长评分 0-100。 @endif */
    uint8_t depth_score;        /*!< @if Eng Score for deep sleep proportion (0-100).
                                     @else   深睡比例评分 0-100。 @endif */
    uint8_t efficiency_score;   /*!< @if Eng Score for sleep efficiency (0-100).
                                     @else   睡眠效率评分 0-100。 @endif */
    uint8_t regularity_score;   /*!< @if Eng Score for sleep schedule regularity (0-100).
                                     @else   作息规律评分 0-100。 @endif */
    uint8_t hrv_score;          /*!< @if Eng Score for HRV recovery during sleep (0-100).
                                     @else   睡眠HRV恢复评分 0-100。 @endif */
} far_sleep_quality_factors_t;

/**
 * @if Eng
 * @brief  Sleep state change callback function type.
 * @param  [in] new_stage New sleep stage.
 * @param  [in] prev_stage Previous sleep stage.
 * @param  [in] duration_min Duration in previous stage (minutes).
 * @param  [in] user_data User-defined pointer.
 * @else
 * @brief  睡眠状态变化回调函数类型。
 * @param  [in] new_stage 新睡眠分期。
 * @param  [in] prev_stage 之前睡眠分期。
 * @param  [in] duration_min 上一分期持续时间（分钟）。
 * @param  [in] user_data 用户自定义指针。
 * @endif
 */
typedef void (*far_sleep_stage_callback_t)(far_sleep_stage_t new_stage,
                                            far_sleep_stage_t prev_stage,
                                            uint16_t duration_min,
                                            void *user_data);

/**
 * @if Eng
 * @brief  Initialize the sleep analysis module.
 * @param  [in] config    Pointer to sleep configuration.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  初始化睡眠分析模块。
 * @param  [in] config    睡眠配置指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_sleep_init(const far_sleep_config_t *config);

/**
 * @if Eng
 * @brief  Deinitialize the sleep analysis module.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  反初始化睡眠分析模块。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sleep_deinit(void);

/**
 * @if Eng
 * @brief  Enable sleep analysis processing.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  使能睡眠分析处理。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sleep_enable(void);

/**
 * @if Eng
 * @brief  Disable sleep analysis processing.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  禁用睡眠分析处理。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sleep_disable(void);

/**
 * @if Eng
 * @brief  Manually start sleep tracking (for scheduled sleep mode).
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  手动开始睡眠追踪（用于计划睡眠模式）。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sleep_start_tracking(void);

/**
 * @if Eng
 * @brief  Manually stop sleep tracking.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  手动停止睡眠追踪。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sleep_stop_tracking(void);

/**
 * @if Eng
 * @brief  Get the most recent sleep record.
 * @param  [out] record   Pointer to store sleep record.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_FAIL   No sleep record available.
 * @else
 * @brief  获取最近的睡眠记录。
 * @param  [out] record   存储睡眠记录的指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_FAIL   无可用睡眠记录。
 * @endif
 */
errcode_t uapi_sleep_get_last_record(far_sleep_record_t *record);

/**
 * @if Eng
 * @brief  Get sleep quality score.
 * @param  [out] score    Pointer to store sleep score (0-100).
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  获取睡眠质量评分。
 * @param  [out] score    存储睡眠评分的指针（0-100）。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sleep_get_score(uint8_t *score);

/**
 * @if Eng
 * @brief  Get detailed sleep quality factors.
 * @param  [out] factors  Pointer to store quality factors.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  获取详细睡眠质量因素。
 * @param  [out] factors  存储质量因素的指针。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sleep_get_quality_factors(far_sleep_quality_factors_t *factors);

/**
 * @if Eng
 * @brief  Get sleep stage breakdown (durations for each stage).
 * @param  [out] deep_min     Deep sleep duration in minutes.
 * @param  [out] light_min    Light sleep duration in minutes.
 * @param  [out] rem_min      REM sleep duration in minutes.
 * @param  [out] awake_min    Awake duration in minutes.
 * @retval ERRCODE_SUCC       Success.
 * @else
 * @brief  获取睡眠分期明细（各阶段时长）。
 * @param  [out] deep_min     深睡时长（分钟）。
 * @param  [out] light_min    浅睡时长（分钟）。
 * @param  [out] rem_min      REM睡眠时长（分钟）。
 * @param  [out] awake_min    清醒时长（分钟）。
 * @retval ERRCODE_SUCC       成功。
 * @endif
 */
errcode_t uapi_sleep_get_stages(uint16_t *deep_min, uint16_t *light_min,
                                 uint16_t *rem_min, uint16_t *awake_min);

/**
 * @if Eng
 * @brief  Register callback for sleep stage changes.
 * @param  [in] callback  Callback function pointer.
 * @param  [in] user_data User-defined pointer.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  注册睡眠分期变化回调。
 * @param  [in] callback  回调函数指针。
 * @param  [in] user_data 用户自定义指针。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sleep_register_stage_callback(far_sleep_stage_callback_t callback, void *user_data);

/**
 * @if Eng
 * @brief  Unregister sleep stage change callback.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  注销睡眠分期变化回调。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sleep_unregister_stage_callback(void);

/**
 * @if Eng
 * @brief  Feed IMU data to sleep analysis algorithm.
 * @note   Called internally during sleep tracking. Used for motion-based
 *         sleep stage detection.
 * @param  [in] data      Pointer to IMU data.
 * @param  [in] len       Number of samples.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  向睡眠分析算法投喂IMU数据。
 * @note   睡眠追踪期间内部调用。用于基于运动的睡眠分期检测。
 * @param  [in] data      IMU数据指针。
 * @param  [in] len       采样数量。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sleep_feed_imu_data(const far_sensor_imu_data_t *data, uint32_t len);

/**
 * @if Eng
 * @brief  Feed heart rate data to sleep analysis algorithm.
 * @note   Called internally. Used for HR-based sleep quality assessment.
 * @param  [in] bpm       Heart rate in BPM.
 * @param  [in] timestamp_ms Timestamp in milliseconds.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  向睡眠分析算法投喂心率数据。
 * @note   内部调用。用于基于心率的睡眠质量评估。
 * @param  [in] bpm       心率，单位：次/分钟。
 * @param  [in] timestamp_ms 时间戳，单位：毫秒。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sleep_feed_hr_data(uint8_t bpm, uint32_t timestamp_ms);

/**
 * @if Eng
 * @brief  Check if user is currently sleeping.
 * @param  [out] is_sleeping true if sleeping, false if awake.
 * @retval ERRCODE_SUCC     Success.
 * @else
 * @brief  检查用户当前是否在睡眠中。
 * @param  [out] is_sleeping true=睡眠中，false=清醒。
 * @retval ERRCODE_SUCC     成功。
 * @endif
 */
errcode_t uapi_sleep_is_sleeping(bool *is_sleeping);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* FAR_SLEEP_H */
