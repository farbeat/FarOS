/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: Provides activity recognition API for FarOS Health Service. \n
 *              Includes pedometer, activity type classification, cadence, \n
 *              and distance tracking. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#ifndef FAR_ACTIVITY_H
#define FAR_ACTIVITY_H

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
 * @defgroup health_activity Activity Recognition
 * @ingroup  health_service
 * @brief    运动识别模块，提供计步、活动类型识别、步频及距离追踪功能。
 *           基于IMU加速度计和陀螺仪数据，采用机器学习/阈值算法实现。
 *
 * @if Eng
 * Activity recognition module providing pedometer, activity type classification,
 * cadence, and distance tracking based on IMU sensor fusion.
 * @endif
 * @{
 */

/**
 * @if Eng
 * @brief  Activity recognition configuration structure.
 * @else
 * @brief  运动识别配置结构体。
 * @endif
 */
typedef struct {
    bool     enable_step_count;      /*!< @if Eng Enable step counting.
                                          @else   使能计步功能。 @endif */
    bool     enable_activity_type;   /*!< @if Eng Enable activity type recognition.
                                          @else   使能活动类型识别。 @endif */
    bool     enable_cadence;         /*!< @if Eng Enable cadence calculation.
                                          @else   使能步频计算。 @endif */
    bool     enable_distance;        /*!< @if Eng Enable distance tracking.
                                          @else   使能距离追踪。 @endif */
    uint16_t step_threshold;         /*!< @if Eng Step detection threshold (mg), default 150.
                                          @else   步态检测阈值（mg），默认150。 @endif */
    uint8_t  confidence_threshold;   /*!< @if Eng Activity classification confidence threshold (%), default 70.
                                          @else   活动分类置信度阈值（%），默认70。 @endif */
} far_activity_config_t;

/**
 * @if Eng
 * @brief  Step count information structure.
 * @else
 * @brief  步数信息结构体。
 * @endif
 */
typedef struct {
    uint32_t total_steps;        /*!< @if Eng Total steps since enable.
                                      @else   使能以来的总步数。 @endif */
    uint32_t steps_today;        /*!< @if Eng Steps counted today (reset at 00:00).
                                      @else   今日步数（每日0点重置）。 @endif */
    uint32_t last_update_time;   /*!< @if Eng Last update timestamp (ms).
                                      @else   最后更新时间戳（ms）。 @endif */
} far_step_info_t;

/**
 * @if Eng
 * @brief  Activity recognition result structure.
 * @else
 * @brief  运动识别结果结构体。
 * @endif
 */
typedef struct {
    far_activity_type_t type;           /*!< @if Eng Recognized activity type.
                                             @else   识别出的活动类型。 @endif */
    uint8_t             confidence;     /*!< @if Eng Confidence level 0-100%.
                                             @else   置信度 0-100%。 @endif */
    uint32_t            duration_sec;   /*!< @if Eng Duration of current activity in seconds.
                                             @else   当前活动持续时间，单位：秒。 @endif */
    uint32_t            start_time;     /*!< @if Eng Activity start timestamp (ms).
                                             @else   活动开始时间戳（ms）。 @endif */
} far_activity_result_t;

/**
 * @if Eng
 * @brief  Cadence (steps per minute) information.
 * @else
 * @brief  步频信息（步/分钟）。
 * @endif
 */
typedef struct {
    uint16_t current_spm;       /*!< @if Eng Current cadence, steps per minute.
                                     @else   当前步频，步/分钟。 @endif */
    uint16_t avg_spm;           /*!< @if Eng Average cadence in current activity.
                                     @else   当前活动平均步频。 @endif */
    uint16_t max_spm;           /*!< @if Eng Maximum cadence in current activity.
                                     @else   当前活动最大步频。 @endif */
} far_cadence_info_t;

/**
 * @if Eng
 * @brief  Distance tracking information.
 * @else
 * @brief  距离追踪信息。
 * @endif
 */
typedef struct {
    uint32_t total_distance_m;      /*!< @if Eng Total distance in meters since enable.
                                         @else   使能以来的总距离，单位：米。 @endif */
    uint32_t distance_today_m;      /*!< @if Eng Distance today in meters.
                                         @else   今日距离，单位：米。 @endif */
    float    avg_stride_length_cm;  /*!< @if Eng Average stride length in cm.
                                         @else   平均步长，单位：厘米。 @endif */
} far_distance_info_t;

/**
 * @if Eng
 * @brief  Activity change callback function type.
 * @param  [in] old_type  Previous activity type.
 * @param  [in] new_type  New activity type.
 * @param  [in] confidence Recognition confidence 0-100.
 * @param  [in] user_data User-defined pointer.
 * @else
 * @brief  活动变化回调函数类型。
 * @param  [in] old_type  之前活动类型。
 * @param  [in] new_type  新活动类型。
 * @param  [in] confidence 识别置信度 0-100。
 * @param  [in] user_data 用户自定义指针。
 * @endif
 */
typedef void (*far_activity_change_callback_t)(far_activity_type_t old_type,
                                                far_activity_type_t new_type,
                                                uint8_t confidence,
                                                void *user_data);

/**
 * @if Eng
 * @brief  Initialize the activity recognition module.
 * @param  [in] config    Pointer to configuration.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @retval ERRCODE_FAIL   Initialization failed.
 * @else
 * @brief  初始化运动识别模块。
 * @param  [in] config    配置指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @retval ERRCODE_FAIL   初始化失败。
 * @endif
 */
errcode_t uapi_activity_init(const far_activity_config_t *config);

/**
 * @if Eng
 * @brief  Deinitialize the activity recognition module.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  反初始化运动识别模块。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_activity_deinit(void);

/**
 * @if Eng
 * @brief  Enable activity recognition processing.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  使能运动识别处理。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_activity_enable(void);

/**
 * @if Eng
 * @brief  Disable activity recognition processing.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  禁用运动识别处理。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_activity_disable(void);

/**
 * @if Eng
 * @brief  Get current step count information.
 * @param  [out] info     Pointer to store step info.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  获取当前步数信息。
 * @param  [out] info     存储步数信息的指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_activity_get_step_count(far_step_info_t *info);

/**
 * @if Eng
 * @brief  Reset step counter.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  重置步数计数器。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_activity_reset_step_count(void);

/**
 * @if Eng
 * @brief  Get current activity recognition result.
 * @param  [out] result   Pointer to store activity result.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  获取当前运动识别结果。
 * @param  [out] result   存储运动识别结果的指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_activity_get_type(far_activity_result_t *result);

/**
 * @if Eng
 * @brief  Get current cadence (steps per minute) information.
 * @param  [out] info     Pointer to store cadence info.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  获取当前步频信息（步/分钟）。
 * @param  [out] info     存储步频信息的指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_activity_get_cadence(far_cadence_info_t *info);

/**
 * @if Eng
 * @brief  Get current distance tracking information.
 * @param  [out] info     Pointer to store distance info.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  获取当前距离追踪信息。
 * @param  [out] info     存储距离信息的指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_activity_get_distance(far_distance_info_t *info);

/**
 * @if Eng
 * @brief  Register callback for activity type changes.
 * @param  [in] callback  Callback function pointer.
 * @param  [in] user_data User-defined pointer.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  注册活动类型变化回调。
 * @param  [in] callback  回调函数指针。
 * @param  [in] user_data 用户自定义指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_activity_register_callback(far_activity_change_callback_t callback, void *user_data);

/**
 * @if Eng
 * @brief  Unregister activity type change callback.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  注销活动类型变化回调。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_activity_unregister_callback(void);

/**
 * @if Eng
 * @brief  Feed IMU data to activity recognition algorithm.
 * @note   This API is called internally by sensor callback. Can also be
 *         called manually for testing with recorded data (e.g., CSV files).
 * @param  [in] data      Pointer to IMU data array.
 * @param  [in] len       Number of samples.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  向运动识别算法投喂IMU数据。
 * @note   此API由传感器回调内部调用。也可手动调用以使用录制数据（如CSV文件）进行测试。
 * @param  [in] data      IMU数据数组指针。
 * @param  [in] len       采样数量。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_activity_feed_imu_data(const far_sensor_imu_data_t *data, uint32_t len);

/**
 * @if Eng
 * @brief  Get daily activity summary.
 * @param  [out] steps        Steps today.
 * @param  [out] distance_m   Distance today in meters.
 * @param  [out] active_min   Active minutes today.
 * @retval ERRCODE_SUCC       Success.
 * @else
 * @brief  获取每日活动汇总。
 * @param  [out] steps        今日步数。
 * @param  [out] distance_m   今日距离（米）。
 * @param  [out] active_min   今日活跃分钟数。
 * @retval ERRCODE_SUCC       成功。
 * @endif
 */
errcode_t uapi_activity_get_daily_summary(uint32_t *steps, uint32_t *distance_m, uint16_t *active_min);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* FAR_ACTIVITY_H */
