/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: Provides health data management API for FarOS Health Service. \n
 *              Includes exercise record storage, historical data query, \n
 *              daily summary, and data synchronization. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#ifndef FAR_HEALTH_DATA_H
#define FAR_HEALTH_DATA_H

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
 * @defgroup health_data Health Data Management
 * @ingroup  health_service
 * @brief    健康数据管理模块，提供运动记录存储、历史数据查询、每日汇总及数据同步功能。
 *           支持NV存储和文件系统双模式，确保数据可靠性。
 *
 * @if Eng
 * Health data management module providing exercise record storage,
 * historical data query, daily summary, and data synchronization.
 * Supports dual-mode storage (NV + filesystem) for data reliability.
 * @endif
 * @{
 */

/**
 * @if Eng
 * @brief  Exercise record type enumeration.
 * @else
 * @brief  运动记录类型枚举。
 * @endif
 */
typedef enum {
    FAR_RECORD_TYPE_ACTIVITY    = 0,   /*!< @if Eng Daily activity summary.
                                            @else   日常活动汇总。 @endif */
    FAR_RECORD_TYPE_WORKOUT     = 1,   /*!< @if Eng Structured workout/exercise session.
                                            @else   结构化训练/运动会话。 @endif */
    FAR_RECORD_TYPE_SLEEP       = 2,   /*!< @if Eng Sleep session record.
                                            @else   睡眠会话记录。 @endif */
    FAR_RECORD_TYPE_HRV         = 3,   /*!< @if Eng HRV measurement record.
                                            @else   HRV测量记录。 @endif */
    FAR_RECORD_TYPE_BODY_BATTERY = 4,  /*!< @if Eng Body battery history record.
                                            @else   身体电量历史记录。 @endif */
    FAR_RECORD_TYPE_STRESS      = 5,   /*!< @if Eng Stress monitoring record.
                                            @else   压力监测记录。 @endif */
} far_record_type_t;

/**
 * @if Eng
 * @brief  Workout/exercise session record structure.
 * @else
 * @brief  训练/运动会话记录结构体。
 * @endif
 */
typedef struct {
    char                  record_id[FAR_HEALTH_RECORD_ID_MAX_LEN]; /*!< @if Eng Unique record ID string.
                                                                        @else   唯一记录ID字符串。 @endif */
    far_record_type_t     type;         /*!< @if Eng Record type.
                                             @else   记录类型。 @endif */
    far_activity_type_t   activity_type; /*!< @if Eng Activity type.
                                              @else   活动类型。 @endif */
    far_health_timestamp_t start_time;  /*!< @if Eng Session start time.
                                             @else   会话开始时间。 @endif */
    far_health_timestamp_t end_time;    /*!< @if Eng Session end time.
                                             @else   会话结束时间。 @endif */
    uint32_t              duration_sec; /*!< @if Eng Duration in seconds.
                                             @else   持续时间，单位：秒。 @endif */
    uint32_t              steps;        /*!< @if Eng Steps during session.
                                             @else   会话期间步数。 @endif */
    uint32_t              distance_m;   /*!< @if Eng Distance in meters.
                                             @else   距离，单位：米。 @endif */
    uint32_t              calories_kcal; /*!< @if Eng Calories consumed.
                                              @else   消耗卡路里。 @endif */
    uint8_t               avg_hr;       /*!< @if Eng Average heart rate.
                                             @else   平均心率。 @endif */
    uint8_t               max_hr;       /*!< @if Eng Maximum heart rate.
                                             @else   最大心率。 @endif */
    uint8_t               min_hr;       /*!< @if Eng Minimum heart rate.
                                             @else   最小心率。 @endif */
    uint16_t              avg_cadence;  /*!< @if Eng Average cadence SPM.
                                             @else   平均步频。 @endif */
    uint16_t              max_cadence;  /*!< @if Eng Maximum cadence SPM.
                                             @else   最大步频。 @endif */
    uint8_t               hr_zone_time[FAR_HEALTH_HR_ZONE_COUNT]; /*!< @if Eng Time in each HR zone (minutes).
                                                                       @else   各心率区间时间（分钟）。 @endif */
    uint8_t               vo2max;       /*!< @if Eng VO2max if estimated during session.
                                             @else   会话期间估算的VO2max。 @endif */
    uint16_t              training_load; /*!< @if Eng Training load (EPOC).
                                              @else   训练负荷（EPOC）。 @endif */
} far_workout_record_t;

/**
 * @if Eng
 * @brief  Daily health summary structure.
 * @else
 * @brief  每日健康汇总结构体。
 * @endif
 */
typedef struct {
    far_health_timestamp_t date;        /*!< @if Eng Date of summary.
                                             @else   汇总日期。 @endif */
    uint32_t              total_steps;  /*!< @if Eng Total steps.
                                             @else   总步数。 @endif */
    uint32_t              total_distance_m; /*!< @if Eng Total distance in meters.
                                                 @else   总距离，单位：米。 @endif */
    uint32_t              total_calories_kcal; /*!< @if Eng Total calories.
                                                    @else   总卡路里。 @endif */
    uint32_t              active_calories_kcal; /*!< @if Eng Active exercise calories.
                                                     @else   主动运动卡路里。 @endif */
    uint16_t              active_minutes; /*!< @if Eng Active minutes (moderate+ intensity).
                                               @else   活跃分钟数（中等强度以上）。 @endif */
    uint8_t               avg_resting_hr; /*!< @if Eng Average resting heart rate.
                                               @else   平均静息心率。 @endif */
    uint8_t               avg_stress;   /*!< @if Eng Average stress level.
                                             @else   平均压力水平。 @endif */
    uint8_t               sleep_score;  /*!< @if Eng Sleep quality score.
                                             @else   睡眠质量评分。 @endif */
    uint16_t              sleep_duration_min; /*!< @if Eng Total sleep duration.
                                                   @else   总睡眠时长。 @endif */
    uint8_t               body_battery_high; /*!< @if Eng Highest body battery.
                                                  @else   最高身体电量。 @endif */
    uint8_t               body_battery_low; /*!< @if Eng Lowest body battery.
                                                 @else   最低身体电量。 @endif */
    uint8_t               workout_count; /*!< @if Eng Number of workout sessions.
                                              @else   训练会话数量。 @endif */
} far_daily_summary_t;

/**
 * @if Eng
 * @brief  Data storage mode enumeration.
 * @else
 * @brief  数据存储模式枚举。
 * @endif
 */
typedef enum {
    FAR_STORAGE_MODE_NV     = 0,   /*!< @if Eng NV (Non-Volatile) storage only.
                                        @else   仅NV非易失存储。 @endif */
    FAR_STORAGE_MODE_FS     = 1,   /*!< @if Eng Filesystem storage only.
                                        @else   仅文件系统存储。 @endif */
    FAR_STORAGE_MODE_DUAL   = 2,   /*!< @if Eng Dual mode (NV + filesystem) for redundancy.
                                        @else   双模式（NV+文件系统）冗余存储。 @endif */
} far_storage_mode_t;

/**
 * @if Eng
 * @brief  Data query filter structure.
 * @else
 * @brief  数据查询过滤条件结构体。
 * @endif
 */
typedef struct {
    far_record_type_t     type;         /*!< @if Eng Record type filter, 0xFF=all types.
                                             @else   记录类型过滤，0xFF=所有类型。 @endif */
    far_activity_type_t   activity_type; /*!< @if Eng Activity type filter, 0xFF=all.
                                              @else   活动类型过滤，0xFF=所有。 @endif */
    far_health_timestamp_t start_date;  /*!< @if Eng Start date filter.
                                             @else   开始日期过滤。 @endif */
    far_health_timestamp_t end_date;    /*!< @if Eng End date filter.
                                             @else   结束日期过滤。 @endif */
    uint32_t              min_duration_sec; /*!< @if Eng Minimum duration filter.
                                                 @else   最小持续时间过滤。 @endif */
} far_data_query_filter_t;

/**
 * @if Eng
 * @brief  Data synchronization status callback.
 * @param  [in] progress  Sync progress 0-100%.
 * @param  [in] status    0=success, 1=error, 2=in_progress.
 * @param  [in] user_data User-defined pointer.
 * @else
 * @brief  数据同步状态回调。
 * @param  [in] progress  同步进度 0-100%。
 * @param  [in] status    0=成功，1=错误，2=进行中。
 * @param  [in] user_data 用户自定义指针。
 * @endif
 */
typedef void (*far_data_sync_callback_t)(uint8_t progress, uint8_t status, void *user_data);

/**
 * @if Eng
 * @brief  Initialize the health data management module.
 * @param  [in] storage_mode  Storage mode selection.
 * @retval ERRCODE_SUCC       Success.
 * @retval ERRCODE_FAIL       Initialization failed.
 * @else
 * @brief  初始化健康数据管理模块。
 * @param  [in] storage_mode  存储模式选择。
 * @retval ERRCODE_SUCC       成功。
 * @retval ERRCODE_FAIL       初始化失败。
 * @endif
 */
errcode_t uapi_health_data_init(far_storage_mode_t storage_mode);

/**
 * @if Eng
 * @brief  Deinitialize the health data management module.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  反初始化健康数据管理模块。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_health_data_deinit(void);

/**
 * @if Eng
 * @brief  Save a workout/exercise record.
 * @param  [in] record    Pointer to workout record.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_FAIL   Storage failed.
 * @else
 * @brief  保存训练/运动记录。
 * @param  [in] record    运动记录指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_FAIL   存储失败。
 * @endif
 */
errcode_t uapi_health_data_save_workout(const far_workout_record_t *record);

/**
 * @if Eng
 * @brief  Save a sleep record.
 * @param  [in] record    Pointer to sleep record.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_FAIL   Storage failed.
 * @else
 * @brief  保存睡眠记录。
 * @param  [in] record    睡眠记录指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_FAIL   存储失败。
 * @endif
 */
errcode_t uapi_health_data_save_sleep(const far_sleep_record_t *record);

/**
 * @if Eng
 * @brief  Query historical workout records.
 * @param  [in]  filter       Query filter conditions.
 * @param  [out] records      Buffer to store records.
 * @param  [in]  max_records  Maximum records to retrieve.
 * @param  [out] actual_count Actual records retrieved.
 * @retval ERRCODE_SUCC       Success.
 * @else
 * @brief  查询历史运动记录。
 * @param  [in]  filter       查询过滤条件。
 * @param  [out] records      存储记录的缓冲区。
 * @param  [in]  max_records  最大检索记录数。
 * @param  [out] actual_count 实际检索记录数。
 * @retval ERRCODE_SUCC       成功。
 * @endif
 */
errcode_t uapi_health_data_get_workouts(const far_data_query_filter_t *filter,
                                         far_workout_record_t *records,
                                         uint32_t max_records,
                                         uint32_t *actual_count);

/**
 * @if Eng
 * @brief  Query historical sleep records.
 * @param  [in]  start_date   Start date.
 * @param  [in]  end_date     End date.
 * @param  [out] records      Buffer to store records.
 * @param  [in]  max_records  Maximum records to retrieve.
 * @param  [out] actual_count Actual records retrieved.
 * @retval ERRCODE_SUCC       Success.
 * @else
 * @brief  查询历史睡眠记录。
 * @param  [in]  start_date   开始日期。
 * @param  [in]  end_date     结束日期。
 * @param  [out] records      存储记录的缓冲区。
 * @param  [in]  max_records  最大检索记录数。
 * @param  [out] actual_count 实际检索记录数。
 * @retval ERRCODE_SUCC       成功。
 * @endif
 */
errcode_t uapi_health_data_get_sleep_records(const far_health_timestamp_t *start_date,
                                              const far_health_timestamp_t *end_date,
                                              far_sleep_record_t *records,
                                              uint32_t max_records,
                                              uint32_t *actual_count);

/**
 * @if Eng
 * @brief  Get daily summary for a specific date.
 * @param  [in]  date       Target date.
 * @param  [out] summary    Pointer to store daily summary.
 * @retval ERRCODE_SUCC     Success.
 * @retval ERRCODE_FAIL     No data for this date.
 * @else
 * @brief  获取指定日期的每日汇总。
 * @param  [in]  date       目标日期。
 * @param  [out] summary    存储每日汇总的指针。
 * @retval ERRCODE_SUCC     成功。
 * @retval ERRCODE_FAIL     该日期无数据。
 * @endif
 */
errcode_t uapi_health_data_get_daily_summary(const far_health_timestamp_t *date,
                                              far_daily_summary_t *summary);

/**
 * @if Eng
 * @brief  Delete a record by ID.
 * @param  [in] record_id   Record ID string.
 * @retval ERRCODE_SUCC     Success.
 * @retval ERRCODE_FAIL     Record not found.
 * @else
 * @brief  按ID删除记录。
 * @param  [in] record_id   记录ID字符串。
 * @retval ERRCODE_SUCC     成功。
 * @retval ERRCODE_FAIL     记录未找到。
 * @endif
 */
errcode_t uapi_health_data_delete_record(const char *record_id);

/**
 * @if Eng
 * @brief  Delete all records before a specified date.
 * @param  [in] before_date Records before this date will be deleted.
 * @retval ERRCODE_SUCC     Success.
 * @else
 * @brief  删除指定日期之前的所有记录。
 * @param  [in] before_date 该日期之前的记录将被删除。
 * @retval ERRCODE_SUCC     成功。
 * @endif
 */
errcode_t uapi_health_data_delete_before(const far_health_timestamp_t *before_date);

/**
 * @if Eng
 * @brief  Get total storage usage of health data.
 * @param  [out] used_bytes   Used storage in bytes.
 * @param  [out] total_bytes  Total available storage in bytes.
 * @retval ERRCODE_SUCC       Success.
 * @else
 * @brief  获取健康数据总存储使用量。
 * @param  [out] used_bytes   已使用存储，单位：字节。
 * @param  [out] total_bytes  总可用存储，单位：字节。
 * @retval ERRCODE_SUCC       成功。
 * @endif
 */
errcode_t uapi_health_data_get_storage_usage(uint32_t *used_bytes, uint32_t *total_bytes);

/**
 * @if Eng
 * @brief  Start data synchronization (e.g., to phone/cloud).
 * @param  [in] callback  Sync progress callback.
 * @param  [in] user_data User-defined pointer.
 * @retval ERRCODE_SUCC   Sync started.
 * @else
 * @brief  开始数据同步（如同步到手机/云端）。
 * @param  [in] callback  同步进度回调。
 * @param  [in] user_data 用户自定义指针。
 * @retval ERRCODE_SUCC   同步已开始。
 * @endif
 */
errcode_t uapi_health_data_sync_start(far_data_sync_callback_t callback, void *user_data);

/**
 * @if Eng
 * @brief  Export records to a file in JSON/CSV format.
 * @param  [in]  filename   Output filename.
 * @param  [in]  filter     Query filter.
 * @retval ERRCODE_SUCC     Export success.
 * @retval ERRCODE_FAIL     Export failed.
 * @else
 * @brief  将记录导出为JSON/CSV格式文件。
 * @param  [in]  filename   输出文件名。
 * @param  [in]  filter     查询过滤条件。
 * @retval ERRCODE_SUCC     导出成功。
 * @retval ERRCODE_FAIL     导出失败。
 * @endif
 */
errcode_t uapi_health_data_export(const char *filename, const far_data_query_filter_t *filter);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* FAR_HEALTH_DATA_H */
